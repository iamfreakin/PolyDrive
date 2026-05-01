#pragma once
#include <string>
#include <vector>

class City;

// 도시 간 연결 정보 (Edge)
struct Route {
    City* destination;
    float distance;
    int baseReward;
};

// 도시 클래스 (Node)
class City {
private:
    std::string name;
    int x, y; // 격자 이동을 위한 좌표
    std::vector<Route> routes;

public:
    City(std::string n, int x, int y) : name(n), x(x), y(y) {}

    void AddRoute(City* dest, float dist, int reward) {
        routes.push_back({ dest, dist, reward });
    }

    // Getter
    std::string GetName() const { return name; }
    int GetX() const { return x; }
    int GetY() const { return y; }
    const std::vector<Route>& GetRoutes() const { return routes; }
};
