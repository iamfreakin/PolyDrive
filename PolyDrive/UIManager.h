#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include "WorldData.h"
#include "Car.h"

class UIManager {
private:
    HANDLE hConsole;
    void SetCursor(int x, int y);
    void DrawFrame();

public:
    UIManager();
    void ClearScreen();
    void UpdateNavigation(const std::string& location, const std::vector<Route>& routes, const std::string& log);
    void UpdateDashboard(Car* currentCar);
    void ShowVehicleSelection(const std::vector<Car*>& garage);
};
