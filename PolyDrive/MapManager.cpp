#include "MapManager.h"
#include <cmath>

MapManager::MapManager(int w, int h, const std::vector<City*>& cityList)
    : width(w), height(h), cities(cityList), playerX(0), playerY(0) {}

int MapManager::MovePlayer(int dx, int dy, int& energy, Car* car) {
    if (!car) return 0;

    int nextX = playerX + dx;
    int nextY = playerY + dy;

    // 1. 경계 체크
    if (nextX < 0 || nextX >= width || nextY < 0 || nextY >= height) return 0;

    // 2. 에너지 소모 계산
    int requiredEnergy = static_cast<int>(std::ceil(10.0f / car->GetEfficiency()));
    if (energy < requiredEnergy) return 0;

    // 3. 이동 확정
    playerX = nextX;
    playerY = nextY;
    energy -= requiredEnergy;
    
    // 이동 시 내구도 1.0% 차감
    car->ReduceCondition(1.0f);

    // 4. 도시 도착 여부 확인
    if (GetCityAtPlayer() != nullptr) return 2;

    return 1;
}

City* MapManager::GetCityAtPlayer() const {
    for (City* c : cities) {
        if (c->GetX() == playerX && c->GetY() == playerY) {
            return c;
        }
    }
    return nullptr;
}
