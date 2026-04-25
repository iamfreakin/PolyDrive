#include "WorldManager.h"

WorldManager::WorldManager() {
    InitializeData();
    currentCity = "Gwangju"; // Start City
}

void WorldManager::InitializeData() {
    // Distance data in km
    cities["Gwangju"] = { "Gwangju", { {"Daejeon", 170.0f}, {"Busan", 260.0f} } };
    cities["Daejeon"] = { "Daejeon", { {"Seoul", 160.0f}, {"Busan", 200.0f} } };
    cities["Busan"] = { "Busan", { {"Seoul", 390.0f}, {"Gangneung", 300.0f} } };
    cities["Seoul"] = { "Seoul", { {"Gangneung", 220.0f}, {"Gwangju", 290.0f} } };
    cities["Gangneung"] = { "Gangneung", { {"Seoul", 220.0f} } };
}

std::string WorldManager::GetCurrentCity() const {
    return currentCity;
}

std::vector<Route> WorldManager::GetAvailableRoutes() const {
    if (cities.find(currentCity) != cities.end()) {
        return cities.at(currentCity).routes;
    }
    return {};
}

void WorldManager::MoveTo(const std::string& destination) {
    currentCity = destination;
}
