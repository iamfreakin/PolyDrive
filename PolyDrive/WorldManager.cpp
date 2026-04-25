#include "WorldManager.h"
#include <algorithm>

WorldManager::WorldManager() {
    InitializeData();
    currentCity = "Gwangju"; // Start City
}

void WorldManager::InitializeData() {
    // Define all cities in the world
    std::vector<std::string> allCityNames = { "Gwangju", "Daejeon", "Busan", "Seoul", "Gangneung" };
    
    // Distance mapping for any pair (approximated km)
    std::map<std::pair<std::string, std::string>, float> distances = {
        {{"Gwangju", "Daejeon"}, 170.0f}, {{"Gwangju", "Busan"}, 260.0f}, {{"Gwangju", "Seoul"}, 290.0f}, {{"Gwangju", "Gangneung"}, 350.0f},
        {{"Daejeon", "Seoul"}, 160.0f}, {{"Daejeon", "Busan"}, 200.0f}, {{"Daejeon", "Gangneung"}, 240.0f},
        {{"Busan", "Seoul"}, 390.0f}, {{"Busan", "Gangneung"}, 300.0f},
        {{"Seoul", "Gangneung"}, 220.0f}
    };

    for (const auto& start : allCityNames) {
        City city;
        city.name = start;
        for (const auto& end : allCityNames) {
            if (start == end) continue; // Skip same city

            float dist = 0.0f;
            // Find distance in the map (check both directions)
            if (distances.count({start, end})) dist = distances[{start, end}];
            else if (distances.count({end, start})) dist = distances[{end, start}];
            else dist = 250.0f; // Default if not found

            city.routes.push_back({ end, dist });
        }
        cities[start] = city;
    }
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
