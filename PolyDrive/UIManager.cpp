#include "UIManager.h"
#include <iostream>
#include <iomanip>

UIManager::UIManager() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ClearScreen();
}

void UIManager::SetCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void UIManager::ClearScreen() {
    system("cls");
    DrawFrame();
}

void UIManager::DrawFrame() {
    // Vertical divider
    for (int i = 0; i < 25; ++i) {
        SetCursor(40, i);
        std::cout << "|";
    }
}

void UIManager::UpdateNavigation(const std::string& location, const std::vector<Route>& routes, const std::string& log) {
    // Clear Navigation Area
    for (int i = 1; i < 20; ++i) {
        SetCursor(0, i);
        std::cout << std::string(39, ' ');
    }

    SetCursor(2, 1);
    std::cout << "[ Navigation ]";
    SetCursor(2, 3);
    std::cout << "Current City: " << location;

    SetCursor(2, 5);
    std::cout << "Available Destinations:";
    for (int i = 0; i < (int)routes.size(); ++i) {
        SetCursor(4, 6 + i);
        std::cout << "(" << i + 1 << ") " << routes[i].destination 
                  << " (" << routes[i].distance << " km)";
    }

    SetCursor(2, 15);
    std::cout << "-------------------------------";
    SetCursor(2, 16);
    std::cout << "LOG: " << log;
    
    SetCursor(2, 22);
    std::cout << ">> Select Destination (0 to Back): ";
}

void UIManager::UpdateDashboard(Car* currentCar) {
    // Clear Dashboard Area
    for (int i = 1; i < 12; ++i) {
        SetCursor(42, i);
        std::cout << std::string(37, ' ');
    }

    SetCursor(45, 1);
    std::cout << "=======================";
    SetCursor(45, 2);
    std::cout << "   TOTAL DASHBOARD";
    SetCursor(45, 3);
    std::cout << "=======================";

    if (currentCar) {
        SetCursor(45, 5);
        std::cout << "- Vehicle: " << currentCar->GetName();
        SetCursor(45, 6);
        std::cout << "- Distance: " << std::fixed << std::setprecision(1) << currentCar->GetTotalDistance() << " km";
        SetCursor(45, 7);
        std::cout << "- Total Time: " << currentCar->GetTotalTime() << " hours";
    } else {
        SetCursor(45, 5);
        std::cout << "- Vehicle: [None]";
    }
    SetCursor(45, 9);
    std::cout << "=======================";
}

void UIManager::ShowVehicleSelection(const std::vector<Car*>& garage) {
    // Clear Navigation Area for selection
    for (int i = 1; i < 20; ++i) {
        SetCursor(0, i);
        std::cout << std::string(39, ' ');
    }

    SetCursor(2, 1);
    std::cout << "[ Garage ]";
    SetCursor(2, 3);
    std::cout << "Select your vehicle:";

    for (int i = 0; i < (int)garage.size(); ++i) {
        SetCursor(4, 5 + i);
        std::cout << "(" << i + 1 << ") " << garage[i]->GetName();
    }
    SetCursor(4, 5 + (int)garage.size());
    std::cout << "(0) Exit Program";

    SetCursor(2, 22);
    std::cout << ">> Select Vehicle: ";
}
