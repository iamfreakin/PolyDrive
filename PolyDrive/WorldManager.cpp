#include "WorldManager.h"
#include "Bus.h"
#include "SportsCar.h"
#include "Truck.h"
#include "Sedan.h"
#include <algorithm>
#include <ctime>

WorldManager::WorldManager() 
    : money(WorldData::INITIAL_MONEY), energy(WorldData::INITIAL_ENERGY), day(1), 
      currentCity("Gwangju"), currentCar(nullptr) {
    srand(static_cast<unsigned int>(time(NULL)));
    GenerateShop();
    
    // 초기 기본 차량 지급 (Sedan)
    currentCar = new Sedan("Starter Sedan", 90.0f, 8.0f, 3, 0);
    garage.push_back(currentCar);
}

WorldManager::~WorldManager() {
    for (auto car : garage) delete car;
    for (auto car : shopList) delete car;
}

void WorldManager::GenerateShop() {
    // 기존 상점 매물 삭제
    for (auto car : shopList) delete car;
    shopList.clear();

    for (int i = 0; i < 6; ++i) {
        CarType type = static_cast<CarType>(rand() % (int)CarType::COUNT);
        float modifier = 0.7f + (rand() % 61) / 100.0f; // 0.7 ~ 1.3
        
        std::string name = WorldData::NAME_POOLS[type][rand() % 20];
        int dur = (rand() % 3) + 1; // 내구도 1~3

        Car* newCar = nullptr;
        switch (type) {
            case CarType::BUS:
                newCar = new Bus(name, 70.0f * modifier, 4.0f * modifier, dur, (int)(3000 * modifier));
                break;
            case CarType::SPORTSCAR:
                newCar = new SportsCar(name, 140.0f * modifier, 6.0f * modifier, dur, (int)(5000 * modifier));
                break;
            case CarType::TRUCK:
                newCar = new Truck(name, 60.0f * modifier, 10.0f * modifier, dur, (int)(4000 * modifier));
                break;
            case CarType::SEDAN:
                newCar = new Sedan(name, 90.0f * modifier, 8.0f * modifier, dur, (int)(2500 * modifier));
                break;
        }
        if (newCar) shopList.push_back(newCar);
    }
}

void WorldManager::RestDay() {
    day++;
    energy = 100;
    GenerateShop();
}

bool WorldManager::Travel(int routeIdx, std::string& outMsg) {
    if (!currentCar) {
        outMsg = "No car selected!";
        return false;
    }

    auto routes = GetCurrentRoutes();
    if (routeIdx < 0 || routeIdx >= routes.size()) return false;

    Route target = routes[routeIdx];
    int requiredEnergy = (int)(target.distance / currentCar->GetEfficiency());

    if (energy < requiredEnergy) {
        outMsg = "Not enough energy!";
        return false;
    }

    // 이동 처리
    energy -= requiredEnergy;
    float travelTime = currentCar->Move(target.distance);
    
    // 보상 계산 (80% ~ 120%)
    float rewardMod = 0.8f + (rand() % 41) / 100.0f;
    int finalReward = (int)(target.baseReward * rewardMod);
    money += finalReward;

    currentCity = target.destination;
    outMsg = "Arrived at " + currentCity + "! Reward: " + std::to_string(finalReward) + "G";

    // 내구도 0 체크
    if (currentCar->GetDurability() <= 0) {
        outMsg += "\n[CRITICAL] Your car has been destroyed!";
        auto it = std::find(garage.begin(), garage.end(), currentCar);
        if (it != garage.end()) garage.erase(it);
        delete currentCar;
        currentCar = nullptr;
    }

    return true;
}

bool WorldManager::BuyCar(int shopIdx, std::string& outMsg) {
    if (shopIdx < 0 || shopIdx >= shopList.size()) return false;

    Car* target = shopList[shopIdx];
    if (money < target->GetPrice()) {
        outMsg = "Not enough money!";
        return false;
    }

    money -= target->GetPrice();
    garage.push_back(target);
    shopList.erase(shopList.begin() + shopIdx);
    outMsg = "Purchased " + target->GetName() + "!";
    return true;
}

void WorldManager::SelectCar(int garageIdx) {
    if (garageIdx >= 0 && garageIdx < garage.size()) {
        currentCar = garage[garageIdx];
    }
}

std::vector<Route> WorldManager::GetCurrentRoutes() const {
    return WorldData::CITY_ROUTES.at(currentCity);
}
