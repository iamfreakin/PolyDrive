#include <iostream>
#include <vector>
#include <string>
#include "Car.h"
#include "WorldManager.h"
#include "Bus.h"
#include "SportsCar.h"
#include "Truck.h"

using namespace std;

void ShowStatus(const WorldManager& wm, Car* currentCar) {
    cout << "\n========================================" << endl;
    cout << " [Current Location]: " << wm.GetCurrentCity() << endl;
    if (currentCar) {
        cout << " [Current Vehicle ]: " << currentCar->GetName() << endl;
        cout << " [Total Distance  ]: " << currentCar->GetTotalDistance() << " km" << endl;
        cout << " [Total Time      ]: " << currentCar->GetTotalTime() << " hours" << endl;
    } else {
        cout << " [Current Vehicle ]: None (Please select a vehicle)" << endl;
    }
    cout << "========================================\n" << endl;
}

int main() {
    WorldManager world;
    vector<Car*> garage;

    // Add vehicles to garage
    garage.push_back(new Bus("Express Bus", 80.0f));
    garage.push_back(new SportsCar("Porsche", 150.0f));
    garage.push_back(new Truck("Heavy Truck", 60.0f));

    Car* selectedCar = nullptr;

    while (true) {
        ShowStatus(world, selectedCar);

        // 1. Vehicle Selection
        cout << "1. Which vehicle would you like to drive?" << endl;
        for (int i = 0; i < (int)garage.size(); ++i) {
            cout << "  (" << i + 1 << ") " << garage[i]->GetName() << endl;
        }
        cout << "  (0) Exit Program" << endl;
        cout << ">> Select: ";
        
        int carChoice;
        if (!(cin >> carChoice)) break;

        if (carChoice == 0) break;
        if (carChoice > 0 && carChoice <= (int)garage.size()) {
            selectedCar = garage[carChoice - 1];
        } else {
            cout << "Invalid selection. Please try again." << endl;
            continue;
        }

        // 2. Destination Selection
        vector<Route> routes = world.GetAvailableRoutes();
        cout << "\n2. Where would you like to go?" << endl;
        for (int i = 0; i < (int)routes.size(); ++i) {
            cout << "  (" << i + 1 << ") " << routes[i].destination 
                 << " (" << routes[i].distance << " km)" << endl;
        }
        cout << "  (0) Back to Menu" << endl;
        cout << ">> Select: ";

        int routeChoice;
        if (!(cin >> routeChoice)) break;

        if (routeChoice == 0) continue;
        if (routeChoice > 0 && routeChoice <= (int)routes.size()) {
            Route selectedRoute = routes[routeChoice - 1];
            
            // Movement logic (Polymorphism in action!)
            float travelTime = selectedCar->Move(selectedRoute.distance);
            world.MoveTo(selectedRoute.destination);

            cout << "\n[Notice] Arrived at " << selectedRoute.destination << "!" << endl;
            cout << "[Result] Travel time: " << travelTime << " hours." << endl;
        } else {
            cout << "Invalid selection. Please try again." << endl;
        }
    }

    // Cleanup memory
    for (Car* car : garage) {
        delete car;
    }
    garage.clear();

    cout << "Exiting program..." << endl;
    return 0;
}
