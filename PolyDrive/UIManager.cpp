#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

UIManager::UIManager() : lastLog("Welcome to PolyDrive!") {}

void UIManager::DrawGame(const WorldManager& wm) {
    system("cls"); // 화면 초기화
    
    std::cout << "======================================================================\n";
    std::cout << "   PolyDrive - Highway Delivery Simulator\n";
    std::cout << "======================================================================\n";

    // 상단 상태바 (간략화된 버전)
    std::cout << " [Day " << std::setw(3) << wm.GetDay() << "] | ";
    std::cout << " [Cash: " << std::setw(6) << wm.GetMoney() << " G] | ";
    std::cout << " [Energy: " << std::setw(3) << wm.GetEnergy() << "%]\n";
    std::cout << "----------------------------------------------------------------------\n";

    // 현재 위치 및 차량 정보
    std::cout << " Current Location: " << wm.GetCurrentCity()->GetName() << "\n";
    if (wm.GetCurrentCar()) {
        std::cout << " Current Car: " << wm.GetCurrentCar()->GetName() 
                  << " (Eff: " << wm.GetCurrentCar()->GetEfficiency() 
                  << " / Dur: " << wm.GetCurrentCar()->GetDurability() << "/3)\n";
    } else {
        std::cout << " Current Car: NONE (Please select or buy a car!)\n";
    }
    std::cout << "----------------------------------------------------------------------\n";

    // 로그 출력
    if (!lastLog.empty()) {
        std::cout << " Log: " << lastLog << "\n";
        std::cout << "----------------------------------------------------------------------\n";
    }
}

void UIManager::DrawMainContent(const WorldManager& wm, int mode) {
    switch (mode) {
        case 1: { // Move
            std::cout << " [ Available Routes ]\n";
            const auto& routes = wm.GetCurrentRoutes();
            for (int i = 0; i < (int)routes.size(); ++i) {
                std::cout << " " << i + 1 << ". To " << std::left << std::setw(10) << routes[i].destination->GetName() 
                          << " | Dist: " << std::setw(3) << (int)routes[i].distance << "km"
                          << " | Reward: ~" << routes[i].baseReward << "G\n";
            }
            break;
        }
        case 3: { // Garage
            std::cout << " [ Your Garage ]\n";
            auto& garage = wm.GetGarage();
            if (garage.empty()) std::cout << " (Empty)\n";
            for (int i = 0; i < garage.size(); ++i) {
                std::cout << " " << i + 1 << ". ";
                garage[i]->ShowSpec();
            }
            break;
        }
        case 4: { // Shop
            std::cout << " [ Car Shop - Rest to Refresh ]\n";
            auto& shop = wm.GetShopList();
            for (int i = 0; i < shop.size(); ++i) {
                std::cout << " " << i + 1 << ". ";
                shop[i]->ShowSpec();
            }
            break;
        }
    }
    std::cout << "----------------------------------------------------------------------\n";
}

void UIManager::DrawMenu() {
    std::cout << " [Menu] 1.Move  2.Rest  3.Select  4.Shop  0.Exit\n";
    std::cout << " >> ";
}
