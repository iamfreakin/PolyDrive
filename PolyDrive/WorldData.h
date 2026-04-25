#pragma once
#include <string>
#include <vector>

struct Route {
    std::string destination;
    float distance;
};

struct City {
    std::string name;
    std::vector<Route> routes;
};
