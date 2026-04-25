#pragma once
#include "WorldData.h"
#include <map>
#include <vector>
#include <string>

class WorldManager {
private:
    std::map<std::string, City> cities;
    std::string currentCity;

public:
    WorldManager();
    
    void InitializeData();
    std::string GetCurrentCity() const;
    std::vector<Route> GetAvailableRoutes() const;
    void MoveTo(const std::string& destination);
};
