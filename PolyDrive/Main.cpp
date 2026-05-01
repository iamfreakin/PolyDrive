#include "WorldManager.h"
#include "UIManager.h"
#include <iostream>
#include <conio.h> // _getch()

int main() {
    WorldManager wm;
    UIManager ui;
    int choice = -1;
    int currentMode = 0; // 0: Map, 1: Move, 3: Garage, 4: Shop

    while (choice != 0) {
        ui.DrawGame(wm);
        ui.DrawMainContent(wm, currentMode);
        ui.DrawMenu(wm);
        
        // _getch()를 사용하여 입력 처리
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
                currentMode = 3; // Garage로 강제 이동
            } else {
                int result = wm.GetMapManager()->MovePlayer(dx, dy, wm.GetEnergyRef(), wm.GetCurrentCar()->GetEfficiency());
                if (result == 0) {
                    ui.SetLog("Cannot move! (Boundary or No Energy)");
                } else if (result == 2) {
                    City* city = wm.GetMapManager()->GetCityAtPlayer();
                    wm.SetCurrentCity(city);
                    ui.SetLog("Arrived at " + city->GetName() + "!");
                    currentMode = 0;
                } else {
                    wm.SetCurrentCity(nullptr); // 도로 위
                    ui.SetLog("Moving...");
                    currentMode = 0;
                }
            }
            continue;
        }

        // 2. 메뉴 선택 처리
        choice = input - '0'; // ASCII to int

        switch (choice) {
            case 1: { // Cargo (City Only)
                if (wm.GetCurrentCity()) {
                    ui.DrawGame(wm);
                    ui.DrawMainContent(wm, 1);
                    std::cout << " Select route (0 to cancel): ";
                    int rIdx;
                    if (std::cin >> rIdx && rIdx > 0) {
                        std::string msg;
                        if (wm.Travel(rIdx - 1, msg)) {
                            // 이동 성공 시 맵 상의 위치도 업데이트
                            City* newCity = wm.GetCurrentCity();
                            wm.GetMapManager()->SetPlayerPos(newCity->GetX(), newCity->GetY());
                            ui.SetLog(msg);
                        } else {
                            ui.SetLog("[Error] " + msg);
                        }
                    }
                } else {
                    ui.SetLog("Cargo can only be picked up in a city!");
                }
                currentMode = 0;
                break;
            }
            case 2: { // Rest
                wm.RestDay();
                ui.SetLog("Rested. Energy refilled and Shop refreshed!");
                currentMode = 0;
                break;
            }
            case 3: { // Garage
                ui.DrawGame(wm);
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
            case 4: { // Shop (City Only)
                if (wm.GetCurrentCity()) {
                    ui.DrawGame(wm);
                    ui.DrawMainContent(wm, 4);
                    std::cout << " Select car to buy (0 to cancel): ";
                    int sIdx;
                    if (std::cin >> sIdx && sIdx > 0) {
                        std::string msg;
                        if (wm.BuyCar(sIdx - 1, msg)) {
                            ui.SetLog(msg);
                        } else {
                            ui.SetLog("[Error] " + msg);
                        }
                    }
                } else {
                    ui.SetLog("Shop is only available in a city!");
                }
                currentMode = 0;
                break;
            }
            case 0: // Exit
                break;
            default:
                ui.SetLog("Invalid key! Use WASD to move or 1-4 for menu.");
                currentMode = 0;
                break;
        }
    }

    return 0;
}
