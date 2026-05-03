#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cmath>

UIManager::UIManager() : lastLog("Welcome to PolyDrive!"), lastMode(-1) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
    
    // 콘솔 창 크기 (가로 200, 세로 40)
    system("mode con: cols=200 lines=40");
    system("cls");
}

void UIManager::SetCursor(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, coord);
}

void UIManager::HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void UIManager::Render(const WorldManager& wm, int mode) {
    std::vector<std::string> leftPane;  // 좌측: 카메라 맵
    std::vector<std::string> rightPane; // 우측: 정보 및 메뉴

    // 1. 좌측 카메라 맵 생성
    MapManager* mm = wm.GetMapManager();
    int px = mm->GetPlayerX();
    int py = mm->GetPlayerY();
    
    // 카메라 범위 설정 (21x21)
    int camSize = 21;
    int halfCam = camSize / 2;
    int startX = px - halfCam;
    int startY = py - halfCam;

    // 맵 경계 클램핑 (카메라가 맵 밖으로 나가지 않게)
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (startX + camSize > mm->GetWidth()) startX = mm->GetWidth() - camSize;
    if (startY + camSize > mm->GetHeight()) startY = mm->GetHeight() - camSize;

    // 내비게이션 방향 계산
    std::string navArrow = "";
    std::string navDist = "";
    const Mission& mission = wm.GetCurrentMission();
    if (mission.isActive) {
        int dx = mission.destination->GetX() - px;
        int dy = mission.destination->GetY() - py;
        
        if (std::abs(dx) <= 1 && std::abs(dy) <= 1) navArrow = "[HERE]";
        else {
            if (dy < 0) navArrow += "N"; else if (dy > 0) navArrow += "S";
            if (dx < 0) navArrow += "W"; else if (dx > 0) navArrow += "E";
        }
        navDist = std::to_string((int)std::sqrt(dx*dx + dy*dy)) + "km";
    }

    leftPane.push_back(" [ WORLD VIEW ]  Nav: " + (navArrow.empty() ? "N/A" : navArrow + " (" + navDist + ")"));
    leftPane.push_back("------------------------------------------");
    
    for (int y = startY; y < startY + camSize; ++y) {
        std::stringstream ss;
        ss << "  ";
        for (int x = startX; x < startX + camSize; ++x) {
            if (x == px && y == py) ss << "@ ";
            else {
                bool isCity = false;
                for (City* c : wm.GetAllCities()) {
                    if (c->GetX() == x && c->GetY() == y) { isCity = true; break; }
                }
                ss << (isCity ? "C " : ". ");
            }
        }
        leftPane.push_back(ss.str());
    }
    leftPane.push_back("------------------------------------------");
    leftPane.push_back(" Player Pos: (" + std::to_string(px) + ", " + std::to_string(py) + ")");
    
    while (leftPane.size() < 30) leftPane.push_back("");

    // 2. 우측 정보창 데이터 생성
    rightPane.push_back("==========================================================================================");
    rightPane.push_back("   PolyDrive - Highway Delivery Simulator (Dashboard)");
    rightPane.push_back("==========================================================================================");
    
    std::stringstream ssStat;
    ssStat << " [Day " << std::setw(3) << wm.GetDay() << "] | "
           << " [Cash: " << std::setw(6) << wm.GetMoney() << " G] | "
           << " [Energy: " << std::setw(3) << wm.GetEnergy() << "%]";
    rightPane.push_back(ssStat.str());

    std::string missionStr = " [Mission] ";
    if (mission.isActive) {
        missionStr += "Deliver [" + mission.cargoName + "] to " + mission.destination->GetName() + "!";
    } else {
        missionStr += "NONE (Visit a city to accept a mission)";
    }
    rightPane.push_back(missionStr);
    rightPane.push_back("------------------------------------------------------------------------------------------");

    rightPane.push_back(" Location: " + (wm.GetCurrentCity() ? wm.GetCurrentCity()->GetName() : "On the Road (Driving)"));
    if (wm.GetCurrentCar()) {
        std::stringstream ssCar;
        ssCar << " Car: " << std::left << std::setw(15) << wm.GetCurrentCar()->GetName() 
              << " | Efficiency: " << std::fixed << std::setprecision(1) << wm.GetCurrentCar()->GetEfficiency() 
              << " | HP: " << wm.GetCurrentCar()->GetCondition() << "%";
        rightPane.push_back(ssCar.str());
    } else {
        rightPane.push_back(" Car: NONE (Please select or buy a car!)");
    }
    rightPane.push_back("------------------------------------------------------------------------------------------");

    rightPane.push_back(" [LOG]");
    rightPane.push_back(" > " + lastLog);
    rightPane.push_back("------------------------------------------------------------------------------------------");

    std::string content = GetMainContentStr(wm, mode);
    std::stringstream ssContent(content);
    std::string line;
    while (std::getline(ssContent, line)) {
        rightPane.push_back(line);
    }
    
    while (rightPane.size() < 30) rightPane.push_back("");

    // 3. 결합 및 출력
    std::ostringstream combined;
    size_t maxLines = max(leftPane.size(), rightPane.size());
    for (size_t i = 0; i < maxLines; ++i) {
        std::string left = (i < leftPane.size()) ? leftPane[i] : "";
        std::string right = (i < rightPane.size()) ? rightPane[i] : "";
        combined << std::left << std::setw(50) << left << " | " << right 
                 << "                                                                                \n";
    }
    
    combined << "\n" << GetMenuStr(wm);

    SetCursor(0, 0);
    std::cout << combined.str() << std::flush;
}

std::string UIManager::GetMainContentStr(const WorldManager& wm, int mode) {
    std::stringstream ss;
    switch (mode) {
        case 0: ss << " [ Map View Active ]\n Navigate the 40x40 world using WASD keys.\n Follow the 'Nav' indicator to reach your destination.\n"; break;
        case 1: {
            ss << " [ Available Routes from " << (wm.GetCurrentCity() ? wm.GetCurrentCity()->GetName() : "Current City") << " ]\n";
            const auto& routes = wm.GetCurrentRoutes();
            if (routes.empty()) ss << " (No routes available here)\n";
            for (int i = 0; i < (int)routes.size(); ++i) {
                ss << " " << i + 1 << ". To " << std::left << std::setw(12) << routes[i].destination->GetName() 
                   << " | Dist: " << std::setw(3) << (int)routes[i].distance << "km"
                   << " | Reward: ~" << routes[i].baseReward << "G\n";
            }
            break;
        }
        case 3: {
            ss << " [ Your Garage ]\n";
            auto& garage = wm.GetGarage();
            if (garage.empty()) ss << " (Empty)\n";
            for (int i = 0; i < (int)garage.size(); ++i) {
                ss << " " << i + 1 << (garage[i].get() == wm.GetCurrentCar() ? "* " : ". ") 
                   << std::left << std::setw(15) << garage[i]->GetName() 
                   << " HP: " << std::setw(5) << garage[i]->GetCondition() << "%\n";
            }
            break;
        }
        case 4: {
            ss << " [ Shop - Buy Cars & Items ]\n";
            auto& shop = wm.GetShopList();
            for (int i = 0; i < (int)shop.size(); ++i) {
                ss << " " << i + 1 << ". " << std::left << std::setw(15) << shop[i]->GetName() 
                   << " Price: " << std::setw(5) << shop[i]->GetPrice() << "G\n";
            }
            ss << " -- Items --\n";
            auto& iShop = wm.GetItemShopList();
            for (int i = 0; i < (int)iShop.size(); ++i) {
                ss << " " << i + 7 << ". " << std::left << std::setw(15) << iShop[i].name 
                   << " Price: " << std::setw(5) << iShop[i].price << "G\n";
            }
            break;
        }
        case 5: {
            ss << " [ Inventory ]\n";
            auto& inv = wm.GetInventory();
            if (inv.empty()) ss << " (Empty)\n";
            for (int i = 0; i < (int)inv.size(); ++i) {
                ss << " " << i + 1 << ". " << std::left << std::setw(20) << inv[i].name 
                   << " | Effect: +" << (int)inv[i].effectValue << "\n";
            }
            break;
        }
    }
    return ss.str();
}

std::string UIManager::GetMenuStr(const WorldManager& wm) {
    std::stringstream ss;
    ss << " [CONTROL] WASD: Move | 1: Cargo | 2: Rest | 3: Garage | 4: Shop | 5: Inv | 0: Exit\n";
    ss << " >>                                                                                                    ";
    return ss.str();
}

std::string UIManager::GetHeaderStr() { return ""; }
std::string UIManager::GetStatusStr(const WorldManager& wm) { return ""; }
