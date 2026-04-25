#include <iostream>
#include <vector>
#include <string>
#include "Car.h"
#include "WorldManager.h"
#include "Bus.h"
#include "SportsCar.h"
#include "Truck.h"
#include "UIManager.h"

using namespace std;

int main() {
    WorldManager world;
    UIManager ui;
    vector<Car*> garage;

    garage.push_back(new Bus("Express Bus", 80.0f));
    garage.push_back(new SportsCar("Porsche", 150.0f));
    garage.push_back(new Truck("Heavy Truck", 60.0f));

    Car* selectedCar = nullptr;
    string lastLog = "Welcome to PolyDrive!";

    while (true) {
        ui.UpdateDashboard(selectedCar);
        ui.ShowVehicleSelection(garage);

        int carChoice;
        if (!(cin >> carChoice)) break;

        if (carChoice == 0) break;
        if (carChoice > 0 && carChoice <= (int)garage.size()) {
            selectedCar = garage[carChoice - 1];
            lastLog = "Selected vehicle: " + selectedCar->GetName();
        } else {
            lastLog = "Invalid vehicle selection.";
            continue;
        }

        while (true) {
            ui.UpdateDashboard(selectedCar);
            vector<Route> routes = world.GetAvailableRoutes();
            ui.UpdateNavigation(world.GetCurrentCity(), routes, lastLog);

            int routeChoice;
            if (!(cin >> routeChoice)) break;

            if (routeChoice == 0) {
                lastLog = "Returned to garage.";
                break;
            }
            
            if (routeChoice > 0 && routeChoice <= (int)routes.size()) {
                Route selectedRoute = routes[routeChoice - 1];
                
                float travelTime = selectedCar->Move(selectedRoute.distance);
                world.MoveTo(selectedRoute.destination);

                lastLog = "Arrived at " + selectedRoute.destination + " (" + to_string(travelTime).substr(0,4) + "h)";
            } else {
                lastLog = "Invalid destination selection.";
            }
        }
    }

    for (Car* car : garage) {
        delete car;
    }
    
    ui.ClearScreen();
    cout << "Exiting program..." << endl;
    return 0;
}
