#include "WorldManager.h"

WorldManager::WorldManager() {
    InitializeData();
    currentCity = "광주"; // 시작 도시
}

void WorldManager::InitializeData() {
    // 데이터 시트에 기반한 경로 설정
    cities["광주"] = { "광주", { {"대전", 170.0f}, {"부산", 260.0f} } };
    cities["대전"] = { "대전", { {"서울", 160.0f}, {"부산", 200.0f} } };
    cities["부산"] = { "부산", { {"서울", 390.0f}, {"강릉", 300.0f} } };
    cities["서울"] = { "서울", { {"강릉", 220.0f}, {"광주", 290.0f} } };
    cities["강릉"] = { "강릉", { {"서울", 220.0f} } }; // 되돌아가는 경로 등 추가 가능
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
