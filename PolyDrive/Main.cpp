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
        // [최종 미션 트리거 확인]
        if (wm.CheckFinalMissionTrigger()) {
            wm.TriggerFinalMission();
            ui.SetLog(BOLD + YELLOW + "!!! [SPECIAL MISSION] THE GOLDEN CARGO IS AVAILABLE !!!" + RESET);
        }

        // [게임 클리어 확인]
        if (wm.IsGameCleared()) {
            system("cls");
            const auto& score = wm.GetFinalScore();
            std::cout << "\n\n";
            std::cout << "  " << YELLOW << BOLD << "====================================================" << RESET << "\n";
            std::cout << "  " << WHITE << BOLD << "              MISSION ACCOMPLISHED!                " << RESET << "\n";
            std::cout << "  " << YELLOW << BOLD << "====================================================" << RESET << "\n\n";
            std::cout << "   - Total Score  : " << GREEN << BOLD << score.totalScore << RESET << " pts\n";
            std::cout << "   - Days Taken   : " << score.days << " days\n";
            std::cout << "   - Total Repairs: " << score.repairs << " times\n";
            std::cout << "   - Cars Owned   : " << score.carsOwned << " cars\n\n";
            std::cout << "   - FINAL RANK   : " << CYAN << BOLD << score.rank << RESET << "\n\n";
            std::cout << "  " << GRAY << "====================================================" << RESET << "\n";
            std::cout << "   Press any key to exit the game...";
            _getch();
            break;
        }

        // [최적화] 통합된 Render 함수 하나로 화면 출력
        ui.Render(wm, currentMode);
        
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
                currentMode = 5;
            } else {
                int result = wm.GetMapManager()->MovePlayer(dx, dy, wm.GetEnergyRef(), wm.GetCurrentCar());
                if (result == 0) {
                    ui.SetLog("Cannot move! (Boundary or No Energy)");
                } else if (result == 2) {
                    City* city = wm.GetMapManager()->GetCityAtPlayer();
                    wm.SetCurrentCity(city);
                    
                    std::string mMsg;
                    wm.CompleteMission(mMsg);
                    
                    std::string logMsg = "Arrived at " + city->GetName() + "!";
                    if (!mMsg.empty()) logMsg += mMsg;
                    ui.SetLog(logMsg);
                    
                    currentMode = 0;
                } else {
                    wm.SetCurrentCity(nullptr);
                    ui.SetLog("Moving...");
                    currentMode = 0;
                }

                if (wm.GetEnergy() <= 0 || (wm.GetCurrentCar() && wm.GetCurrentCar()->GetCondition() <= 0)) {
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
                    currentMode = 1;
                    ui.Render(wm, currentMode);
                    std::cout << " Select cargo to deliver (0 to cancel): ";
                    int rIdx;
                    if (std::cin >> rIdx && rIdx > 0) {
                        std::string msg;
                        if (wm.AcceptMission(rIdx - 1, msg)) ui.SetLog(msg);
                        else ui.SetLog("[Error] " + msg);
                    }
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
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
                currentMode = 3;
                ui.Render(wm, currentMode);
                std::cout << " Select car to drive (0 to cancel): ";
                int gIdx;
                if (std::cin >> gIdx && gIdx > 0) {
                    wm.SelectCar(gIdx - 1);
                    ui.SetLog("Car changed!");
                }
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                currentMode = 0;
                break;
            }
            case 4: { // Shop
                if (wm.GetCurrentCity()) {
                    currentMode = 4;
                    ui.Render(wm, currentMode);
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
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                } else {
                    ui.SetLog("Shop is only available in a city!");
                }
                currentMode = 0;
                break;
            }
            case 5: { // Inventory
                currentMode = 5;
                ui.Render(wm, currentMode);
                std::cout << " Select item to use (0 to cancel): ";
                int iIdx;
                if (std::cin >> iIdx && iIdx > 0) {
                    std::string msg;
                    if (wm.UseItem(iIdx - 1, msg)) ui.SetLog(msg);
                    else ui.SetLog("[Error] " + msg);
                }
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                currentMode = 0;
                break;
            }
            case 6: { // Repair (City Service)
                std::string msg;
                if (wm.RepairAtCity(msg)) ui.SetLog(msg);
                else ui.SetLog("[Error] " + msg);
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
