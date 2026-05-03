#include "WorldManager.h"
#include "UIManager.h"
#include <iostream>
#include <conio.h> 

int main() {
    WorldManager wm;
    UIManager ui;
    int choice = -1;
    int currentMode = 0; // 0: Map, 1: Cargo, 3: Garage, 4: Shop, 5: Inv

    while (choice != 0) {
        ui.DrawGame(wm, currentMode);
        ui.DrawMainContent(wm, currentMode);
        ui.DrawMenu(wm);
        
        int input = _getch();

        // 1. WASD 이동 처리
        if (input == 'w' || input == 'W' || input == 'a' || input == 'A' || 
            input == 's' || input == 'S' || input == 'd' || input == 'D') {
            
            int dx = 0, dy = 0;
            if (input == 'w' || input == 'W') dy = -1;
            else if (input == 's' || input == 'S') dy = 1;
            else if (input == 'a' || input == 'A') dx = -1;
            else if (input == 'd' || input == 'D') dx = 1;

            if (!wm.GetCurrentCar()) {
                ui.SetLog("Select a car first to move!");
                currentMode = 3;
            } else if (wm.GetCurrentCar()->GetCondition() <= 0) {
                ui.SetLog("Car is destroyed! Repair it or change car.");
                currentMode = 5; // 인벤토리(수리) 모드로 유도
            } else {
                int result = wm.GetMapManager()->MovePlayer(dx, dy, wm.GetEnergyRef(), wm.GetCurrentCar());
                if (result == 0) {
                    ui.SetLog("Cannot move! (Boundary or No Energy)");
                } else if (result == 2) {
                    City* city = wm.GetMapManager()->GetCityAtPlayer();
                    wm.SetCurrentCity(city);
                    
                    std::string mMsg;
                    wm.CompleteMission(mMsg); // 미션 완료 체크
                    ui.SetLog("Arrived at " + city->GetName() + "!" + mMsg);
                    
                    currentMode = 0;
                } else {
                    wm.SetCurrentCity(nullptr);
                    ui.SetLog("Moving...");
                    currentMode = 0;
                }

                // 패널티 체크 (이동 후 상태 확인)
                if (wm.GetEnergy() <= 0 || wm.GetCurrentCar()->GetCondition() <= 0) {
                    std::string tMsg;
                    wm.TowingService(tMsg);
                    ui.SetLog(tMsg);
                }
            }
            continue;
        }

        // 2. 메뉴 선택 처리
        choice = input - '0';

        switch (choice) {
            case 1: { // Cargo (Mission Acceptance)
                if (wm.GetCurrentCity()) {
                    ui.DrawGame(wm, 1);
                    ui.DrawMainContent(wm, 1);
                    std::cout << " Select cargo to deliver (0 to cancel): ";
                    int rIdx;
                    if (std::cin >> rIdx && rIdx > 0) {
                        std::string msg;
                        if (wm.AcceptMission(rIdx - 1, msg)) {
                            ui.SetLog(msg);
                        } else {
                            ui.SetLog("[Error] " + msg);
                        }
                    }
                } else {
                    ui.SetLog("Missions can only be accepted in a city!");
                }
                currentMode = 0;
                break;
            }
            case 2: { // Rest
                wm.RestDay();
                ui.SetLog("Rested. Day passed, Energy refilled, Shop refreshed!");
                currentMode = 0;
                break;
            }
            case 3: { // Garage
                ui.DrawGame(wm, 3);
                ui.DrawMainContent(wm, 3);
                std::cout << " Select car to drive (0 to cancel): ";
                int gIdx;
                if (std::cin >> gIdx && gIdx > 0) {
                    wm.SelectCar(gIdx - 1);
                    ui.SetLog("Car changed!");
                }
                currentMode = 0;
                break;
            }
            case 4: { // Shop
                if (wm.GetCurrentCity()) {
                    ui.DrawGame(wm, 4);
                    ui.DrawMainContent(wm, 4);
                    std::cout << " Buy Car(1-6) or Item(7-10) (0 to cancel): ";
                    int sIdx;
                    if (std::cin >> sIdx && sIdx > 0) {
                        std::string msg;
                        if (sIdx <= 6) {
                            if (wm.BuyCar(sIdx - 1, msg)) ui.SetLog(msg);
                            else ui.SetLog("[Error] " + msg);
                        } else {
                            if (wm.BuyItem(sIdx - 7, msg)) ui.SetLog(msg);
                            else ui.SetLog("[Error] " + msg);
                        }
                    }
                } else {
                    ui.SetLog("Shop is only available in a city!");
                }
                currentMode = 0;
                break;
            }
            case 5: { // Inventory
                ui.DrawGame(wm, 5);
                ui.DrawMainContent(wm, 5);
                std::cout << " Select item to use (0 to cancel): ";
                int iIdx;
                if (std::cin >> iIdx && iIdx > 0) {
                    std::string msg;
                    if (wm.UseItem(iIdx - 1, msg)) ui.SetLog(msg);
                    else ui.SetLog("[Error] " + msg);
                }
                currentMode = 0;
                break;
            }
            case 0: break;
            default:
                ui.SetLog("Invalid key! Use WASD to move or 1-5 for menu.");
                currentMode = 0;
                break;
        }
    }

    return 0;
}
