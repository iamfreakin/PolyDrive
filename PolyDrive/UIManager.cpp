#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

UIManager::UIManager() : lastLog("Welcome to PolyDrive!"), lastMode(-1) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
    system("cls"); // 최초 1회만 화면 초기화
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

void UIManager::DrawGame(const WorldManager& wm, int mode) {
    // 모드가 변경되었을 때만 화면을 지움 (잔상 제거용)
    if (lastMode != mode) {
        system("cls");
        lastMode = mode;
    }

    SetCursor(0, 0); // 커서를 원점으로 이동하여 덮어쓰기 (깜빡임 방지)
    
    std::cout << "======================================================================\n";
    std::cout << "   PolyDrive - Highway Delivery Simulator\n";
    std::cout << "======================================================================\n";

    std::cout << " [Day " << std::setw(3) << wm.GetDay() << "] | ";
    std::cout << " [Cash: " << std::setw(6) << wm.GetMoney() << " G] | ";
    std::cout << " [Energy: " << std::setw(3) << wm.GetEnergy() << "%]\n";
    std::cout << "----------------------------------------------------------------------\n";

    std::cout << " Current Location: " << (wm.GetCurrentCity() ? wm.GetCurrentCity()->GetName() : "On the Road") << "\n";
    if (wm.GetCurrentCar()) {
        std::cout << " Current Car: " << wm.GetCurrentCar()->GetName() 
                  << " (Eff: " << wm.GetCurrentCar()->GetEfficiency() 
                  << " / HP: " << std::fixed << std::setprecision(1) << wm.GetCurrentCar()->GetCondition() << "%)\n";
    } else {
        std::cout << " Current Car: NONE (Please select or buy a car!)\n";
    }
    std::cout << "----------------------------------------------------------------------\n";

    if (!lastLog.empty()) {
        std::cout << " Log: " << lastLog << "\n";
        std::cout << "----------------------------------------------------------------------\n";
    }
}

void UIManager::DrawMainContent(const WorldManager& wm, int mode) {
    switch (mode) {
        case 0: { // Map View
            MapManager* mm = wm.GetMapManager();
            std::cout << " [ Map - Use WASD to Move ]\n";
            for (int y = 0; y < mm->GetHeight(); ++y) {
                std::cout << "  ";
                for (int x = 0; x < mm->GetWidth(); ++x) {
                    if (x == mm->GetPlayerX() && y == mm->GetPlayerY()) {
                        std::cout << "@ ";
                    } else {
                        bool isCity = false;
                        for (City* c : wm.GetAllCities()) {
                            if (c->GetX() == x && c->GetY() == y) {
                                isCity = true; break;
                            }
                        }
                        std::cout << (isCity ? "C " : ". ");
                    }
                }
                std::cout << "\n";
            }
            break;
        }
        case 1: { // Cargo
            if (!wm.GetCurrentCity()) {
                std::cout << " You are on the road. Reach a city to deliver cargo!\n";
            } else {
                std::cout << " [ Available Routes from " << wm.GetCurrentCity()->GetName() << " ]\n";
                const auto& routes = wm.GetCurrentRoutes();
                for (int i = 0; i < (int)routes.size(); ++i) {
                    std::cout << " " << i + 1 << ". To " << std::left << std::setw(10) << routes[i].destination->GetName() 
                              << " | Dist: " << std::setw(3) << (int)routes[i].distance << "km"
                              << " | Reward: ~" << routes[i].baseReward << "G\n";
                }
            }
            break;
        }
        case 3: { // Garage
            std::cout << " [ Your Garage ]\n";
            auto& garage = wm.GetGarage();
            if (garage.empty()) std::cout << " (Empty)\n";
            for (int i = 0; i < (int)garage.size(); ++i) {
                std::cout << " " << i + 1 << (garage[i].get() == wm.GetCurrentCar() ? "* " : ". ");
                garage[i]->ShowSpec();
            }
            break;
        }
        case 4: { // Shop
            std::cout << " [ Car Shop ]\n";
            auto& shop = wm.GetShopList();
            for (int i = 0; i < (int)shop.size(); ++i) {
                std::cout << " " << i + 1 << ". ";
                shop[i]->ShowSpec();
            }
            std::cout << "\n [ Item Shop ]\n";
            auto& iShop = wm.GetItemShopList();
            if (iShop.empty()) std::cout << " (Sold Out)\n";
            for (int i = 0; i < (int)iShop.size(); ++i) {
                std::cout << " " << i + 7 << ". " << std::left << std::setw(20) << iShop[i].name 
                          << " | Price: " << std::setw(5) << iShop[i].price << "G"
                          << " | Effect: +" << (int)iShop[i].effectValue << "\n";
            }
            break;
        }
        case 5: { // Inventory
            std::cout << " [ Your Inventory ]\n";
            auto& inv = wm.GetInventory();
            if (inv.empty()) std::cout << " (Empty)\n";
            for (int i = 0; i < (int)inv.size(); ++i) {
                std::cout << " " << i + 1 << ". " << std::left << std::setw(20) << inv[i].name 
                          << " | Effect: +" << (int)inv[i].effectValue << "\n";
            }
            break;
        }
    }
    std::cout << "----------------------------------------------------------------------\n";
}

void UIManager::DrawMenu(const WorldManager& wm) {
    std::cout << " [Movement] W,A,S,D to move\n";
    if (wm.GetCurrentCity()) {
        std::cout << " [City Menu] 1.Cargo  2.Rest  3.Garage  4.Shop  5.Inv  0.Exit\n";
    } else {
        std::cout << " [Road Menu] 2.Rest  3.Garage  5.Inv  0.Exit\n";
    }
    std::cout << " >> ";
}
