#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

UIManager::UIManager() : lastLog("Welcome to PolyDrive!") {}

void UIManager::DrawGame(const WorldManager& wm) {
    system("cls");
    
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
                  << " / Dur: " << wm.GetCurrentCar()->GetDurability() << "/3)\n";
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
        case 0: { // Map View (Movement)
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
        case 1: { // Move (Contextual)
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
                std::cout << " " << i + 1 << ". ";
                garage[i]->ShowSpec();
            }
            break;
        }
        case 4: { // Shop
            std::cout << " [ Car Shop - Rest to Refresh ]\n";
            auto& shop = wm.GetShopList();
            for (int i = 0; i < (int)shop.size(); ++i) {
                std::cout << " " << i + 1 << ". ";
                shop[i]->ShowSpec();
            }
            break;
        }
    }
    std::cout << "----------------------------------------------------------------------\n";
}

void UIManager::DrawMenu(const WorldManager& wm) {
    std::cout << " [Movement] W,A,S,D to move\n";
    if (wm.GetCurrentCity()) {
        std::cout << " [City Menu] 1.Cargo  2.Rest  3.Garage  4.Shop  0.Exit\n";
    } else {
        std::cout << " [Road Menu] 2.Rest  3.Garage  0.Exit\n";
    }
    std::cout << " >> ";
}
