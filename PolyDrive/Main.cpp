#include "WorldManager.h"
#include "UIManager.h"
#include <iostream>

int main() {
    WorldManager wm;
    UIManager ui;
    int choice = -1;

    while (choice != 0) {
        ui.DrawGame(wm);
        ui.DrawMenu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1: { // Move
                ui.DrawGame(wm);
                ui.DrawMainContent(wm, 1);
                std::cout << " Select route (0 to cancel): ";
                int rIdx;
                if (std::cin >> rIdx && rIdx > 0) {
                    std::string msg;
                    if (wm.Travel(rIdx - 1, msg)) {
                        ui.SetLog(msg);
                    } else {
                        ui.SetLog("[Error] " + msg);
                    }
                }
                break;
            }
            case 2: { // Rest
                wm.RestDay();
                ui.SetLog("Rested. Energy refilled and Shop refreshed!");
                break;
            }
            case 3: { // Select Car
                ui.DrawGame(wm);
                ui.DrawMainContent(wm, 3);
                std::cout << " Select car to drive (0 to cancel): ";
                int gIdx;
                if (std::cin >> gIdx && gIdx > 0) {
                    wm.SelectCar(gIdx - 1);
                    ui.SetLog("Car changed!");
                }
                break;
            }
            case 4: { // Shop
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
                break;
            }
            case 0: // Exit
                ui.SetLog("Exiting game... Goodbye!");
                break;
            default:
                ui.SetLog("Invalid option!");
                break;
        }
    }

    return 0;
}
