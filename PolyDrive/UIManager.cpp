#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

UIManager::UIManager() : lastLog("Welcome to PolyDrive!"), lastMode(-1) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
    
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);

    system("mode con: cols=200 lines=45");
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

// ANSI 코드를 제외한 실제 문자열 길이를 계산하는 헬퍼
int GetPlainLength(const std::string& s) {
    int len = 0;
    bool inEscape = false;
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == '\033') inEscape = true;
        else if (inEscape && s[i] == 'm') inEscape = false;
        else if (!inEscape) {
            // 한글 등 멀티바이트 문자는 2칸으로 계산 (간이 구현)
            if ((unsigned char)s[i] >= 0x80) {
                len += 2;
                i++; // 다음 바이트 건너뜀
            } else {
                len += 1;
            }
        }
    }
    return len;
}

std::string GetGaugeBar(float value, float maxVal, int width, std::string color) {
    std::stringstream ss;
    int filled = (int)((value / maxVal) * width);
    if (filled < 0) filled = 0;
    if (filled > width) filled = width;

    ss << "[";
    ss << color;
    for (int i = 0; i < filled; ++i) ss << "■";
    ss << "\033[0m"; // RESET
    for (int i = filled; i < width; ++i) ss << " ";
    ss << "]";
    return ss.str();
}

void UIManager::Render(const WorldManager& wm, int mode) {
    std::vector<std::string> leftPane;
    std::vector<std::string> rightPane;

    // 1. 좌측 카메라 맵 생성
    MapManager* mm = wm.GetMapManager();
    int px = mm->GetPlayerX();
    int py = mm->GetPlayerY();
    
    int camSize = 21;
    int halfCam = camSize / 2;
    int startX = std::max(0, std::min(px - halfCam, mm->GetWidth() - camSize));
    int startY = std::max(0, std::min(py - halfCam, mm->GetHeight() - camSize));

    const Mission& mission = wm.GetCurrentMission();
    std::string navInfo = GRAY + "N/A" + RESET;
    if (mission.isActive) {
        int dx = mission.destination->GetX() - px;
        int dy = mission.destination->GetY() - py;
        std::string arrow = "";
        if (std::abs(dx) <= 1 && std::abs(dy) <= 1) arrow = "[HERE]";
        else {
            if (dy < 0) arrow += "N"; else if (dy > 0) arrow += "S";
            if (dx < 0) arrow += "W"; else if (dx > 0) arrow += "E";
        }
        int dist = (int)std::sqrt(dx*dx + dy*dy);
        navInfo = YELLOW + BOLD + arrow + RESET + " (" + std::to_string(dist) + "km)";
    }

    leftPane.push_back(CYAN + BOLD + " [ WORLD VIEW ] " + RESET + " Nav: " + navInfo);
    leftPane.push_back(GRAY + "------------------------------------------" + RESET);
    
    for (int y = startY; y < startY + camSize; ++y) {
        std::stringstream ss;
        ss << "  ";
        for (int x = startX; x < startX + camSize; ++x) {
            if (x == px && y == py) ss << GREEN + BOLD + "@ " + RESET;
            else {
                City* cityAt = nullptr;
                for (City* c : wm.GetAllCities()) {
                    if (c->GetX() == x && c->GetY() == y) { cityAt = c; break; }
                }
                if (cityAt) ss << CYAN + BOLD + "C " + RESET;
                else ss << GRAY + ". " + RESET;
            }
        }
        leftPane.push_back(ss.str());
    }
    leftPane.push_back(GRAY + "------------------------------------------" + RESET);
    leftPane.push_back(" Pos: (" + std::to_string(px) + ", " + std::to_string(py) + ")");
    
    // 2. 우측 대시보드 생성
    rightPane.push_back(BLUE + BOLD + "==========================================================================================" + RESET);
    rightPane.push_back(BOLD + "   PolyDrive - Highway Delivery Simulator " + RESET + GRAY + "(Dashboard)" + RESET);
    rightPane.push_back(BLUE + BOLD + "==========================================================================================" + RESET);
    
    std::stringstream ssStat;
    ssStat << " " << YELLOW << "[Day " << std::setw(3) << wm.GetDay() << "]" << RESET << " | "
           << WHITE << BOLD << "Cash: " << std::setw(6) << wm.GetMoney() << " G" << RESET << " | "
           << "Energy: " << GetGaugeBar((float)wm.GetEnergy(), 100.0f, 10, YELLOW) << " " << wm.GetEnergy() << "%";
    rightPane.push_back(ssStat.str());

    std::string missionStr = " " + MAGENTA + BOLD + "[Mission] " + RESET;
    if (mission.isActive) {
        missionStr += "Deliver [" + YELLOW + mission.cargoName + RESET + "] to " + CYAN + BOLD + mission.destination->GetName() + RESET + "!";
    } else {
        missionStr += GRAY + "No active mission. Visit a city." + RESET;
    }
    rightPane.push_back(missionStr);
    rightPane.push_back(GRAY + "------------------------------------------------------------------------------------------" + RESET);

    rightPane.push_back(" Location: " + (wm.GetCurrentCity() ? CYAN + BOLD + wm.GetCurrentCity()->GetName() + RESET : GRAY + "Driving on Highway" + RESET));
    if (wm.GetCurrentCar()) {
        std::stringstream ssCar;
        ssCar << " Car: " << WHITE << BOLD << std::left << std::setw(15) << wm.GetCurrentCar()->GetName() << RESET
              << " | Eff: " << wm.GetCurrentCar()->GetEfficiency() 
              << " | HP: " << GetGaugeBar(wm.GetCurrentCar()->GetCondition(), 100.0f, 10, RED) << " " << (int)wm.GetCurrentCar()->GetCondition() << "%";
        rightPane.push_back(ssCar.str());
    } else {
        rightPane.push_back(" Car: " + RED + "NONE (Please select or buy a car!)" + RESET);
    }
    rightPane.push_back(GRAY + "------------------------------------------------------------------------------------------" + RESET);

    rightPane.push_back(GREEN + BOLD + " [LOG]" + RESET);
    rightPane.push_back(" > " + lastLog);
    rightPane.push_back(GRAY + "------------------------------------------------------------------------------------------" + RESET);

    std::string content = GetMainContentStr(wm, mode);
    std::stringstream ssContent(content);
    std::string line;
    while (std::getline(ssContent, line)) {
        rightPane.push_back(line);
    }

    // 3. 결합 및 출력 (충분한 높이 확보: 40줄)
    SetCursor(0, 0);
    std::ostringstream combined;
    for (size_t i = 0; i < 40; ++i) {
        // 좌측
        std::string left = (i < leftPane.size()) ? leftPane[i] : "";
        combined << left;
        
        int lpLen = GetPlainLength(left);
        for(int j = 0; j < (50 - lpLen); ++j) combined << " ";

        // 구분선
        combined << " | ";

        // 우측
        std::string right = (i < rightPane.size()) ? rightPane[i] : "";
        combined << right;

        // 우측 잔상 제거용 패딩 (가로 120칸 정도 더 채움)
        int rpLen = GetPlainLength(right);
        for(int j = 0; j < (120 - rpLen); ++j) combined << " ";
        
        combined << "\n";
    }
    
    combined << "\n" << GetMenuStr(wm);
    // 메뉴 줄 잔상 제거 패딩
    combined << "                                                                                ";

    std::cout << combined.str() << std::flush;
    
    // 입력 커서 고정 (y=42 정도, 메뉴 출력 아래)
    SetCursor(4, 42); 
}

std::string UIManager::GetMainContentStr(const WorldManager& wm, int mode) {
    std::stringstream ss;
    switch (mode) {
        case 0: ss << " " << WHITE << BOLD << "[ Map View ]" << RESET << "\n Use WASD to navigate. Visit 'C' to open city menu.\n"; break;
        case 1: {
            ss << " " << CYAN << BOLD << "[ Available Cargo ]" << RESET << " at " << (wm.GetCurrentCity() ? wm.GetCurrentCity()->GetName() : "City") << "\n";
            const auto& routes = wm.GetCurrentRoutes();
            for (int i = 0; i < (int)routes.size(); ++i) {
                ss << " " << YELLOW << i + 1 << "." << RESET << " To " << std::left << std::setw(12) << routes[i].destination->GetName() 
                   << " | Dist: " << std::setw(3) << (int)routes[i].distance << "km"
                   << " | Reward: " << GREEN << routes[i].baseReward << "G" << RESET << "\n";
            }
            break;
        }
        case 3: {
            ss << " " << MAGENTA << BOLD << "[ Your Garage ]" << RESET << "\n";
            auto& garage = wm.GetGarage();
            for (int i = 0; i < (int)garage.size(); ++i) {
                bool isCur = (garage[i].get() == wm.GetCurrentCar());
                ss << " " << (isCur ? GREEN + BOLD + "* " : GRAY + ". ") << i + 1 << ". " << RESET
                   << std::left << std::setw(15) << (isCur ? BOLD + garage[i]->GetName() : garage[i]->GetName()) << RESET
                   << " Condition: " << (int)garage[i]->GetCondition() << "%\n";
            }
            break;
        }
        case 4: {
            ss << " " << YELLOW << BOLD << "[ Car Shop ]" << RESET << "\n";
            auto& shop = wm.GetShopList();
            for (int i = 0; i < (int)shop.size(); ++i) {
                ss << " " << i + 1 << ". " << std::left << std::setw(15) << shop[i]->GetName() 
                   << " Price: " << GREEN << shop[i]->GetPrice() << "G" << RESET << "\n";
            }
            ss << " " << YELLOW << BOLD << "-- Items --" << RESET << "\n";
            auto& iShop = wm.GetItemShopList();
            for (int i = 0; i < (int)iShop.size(); ++i) {
                ss << " " << i + 7 << ". " << std::left << std::setw(15) << iShop[i].name 
                   << " Price: " << GREEN << iShop[i].price << "G" << RESET << "\n";
            }
            break;
        }
        case 5: {
            ss << " " << GREEN << BOLD << "[ Inventory ]" << RESET << "\n";
            auto& inv = wm.GetInventory();
            if (inv.empty()) ss << GRAY << " (Inventory is empty)" << RESET << "\n";
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
    ss << BOLD << " [CONTROL]" << RESET << " " << YELLOW << "WASD" << RESET << ":Move | " 
       << YELLOW << "1" << RESET << ":Cargo | " << YELLOW << "2" << RESET << ":Rest | " 
       << YELLOW << "3" << RESET << ":Garage | " << YELLOW << "4" << RESET << ":Shop | " 
       << YELLOW << "5" << RESET << ":Inv | " << RED << "0" << RESET << ":Exit\n";
    ss << " >> ";
    return ss.str();
}

std::string UIManager::GetHeaderStr() { return ""; }
std::string UIManager::GetStatusStr(const WorldManager& wm) { return ""; }
