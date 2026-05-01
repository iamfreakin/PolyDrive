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
    auto starter = std::make_unique<Sedan>("Starter Sedan", 90.0f, 8.0f, 3, 0);
    currentCar = starter.get();
    garage.push_back(std::move(starter));
}

WorldManager::~WorldManager() {
    // std::unique_ptr handles memory automatically
}

void WorldManager::GenerateShop() {
    // std::unique_ptr vectors automatically delete objects on clear()
    shopList.clear();

    for (int i = 0; i < 6; ++i) {
        CarType type = static_cast<CarType>(rand() % (int)CarType::COUNT);
        float modifier = 0.7f + (rand() % 61) / 100.0f; // 0.7 ~ 1.3
        
        std::string name = WorldData::NAME_POOLS[type][rand() % 20];
        int dur = (rand() % 3) + 1; // 내구도 1~3

        std::unique_ptr<Car> newCar = nullptr;
        switch (type) {
            case CarType::BUS:
                newCar = std::make_unique<Bus>(name, 70.0f * modifier, 4.0f * modifier, dur, (int)(3000 * modifier));
                break;
            case CarType::SPORTSCAR:
                newCar = std::make_unique<SportsCar>(name, 140.0f * modifier, 6.0f * modifier, dur, (int)(5000 * modifier));
                break;
            case CarType::TRUCK:
                newCar = std::make_unique<Truck>(name, 60.0f * modifier, 10.0f * modifier, dur, (int)(4000 * modifier));
                break;
            case CarType::SEDAN:
                newCar = std::make_unique<Sedan>(name, 90.0f * modifier, 8.0f * modifier, dur, (int)(2500 * modifier));
                break;
        }
        if (newCar) shopList.push_back(std::move(newCar));
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
        auto it = std::find_if(garage.begin(), garage.end(), 
            [this](const std::unique_ptr<Car>& p) { return p.get() == currentCar; });
        
        if (it != garage.end()) garage.erase(it);
        currentCar = nullptr;
    }

    return true;
}

bool WorldManager::BuyCar(int shopIdx, std::string& outMsg) {
    if (shopIdx < 0 || shopIdx >= (int)shopList.size()) return false;

    auto& target = shopList[shopIdx];
    if (money < target->GetPrice()) {
        outMsg = "Not enough money!";
        return false;
    }

    money -= target->GetPrice();
    outMsg = "Purchased " + target->GetName() + "!";
    
    // std::move transfers ownership from shopList to garage
    garage.push_back(std::move(shopList[shopIdx]));
    shopList.erase(shopList.begin() + shopIdx);
    
    return true;
}

void WorldManager::SelectCar(int garageIdx) {
    if (garageIdx >= 0 && garageIdx < (int)garage.size()) {
        // .get() provides the raw pointer for observation without taking ownership
        currentCar = garage[garageIdx].get();
    }
}

std::vector<Route> WorldManager::GetCurrentRoutes() const {
    return WorldData::CITY_ROUTES.at(currentCity);
}
