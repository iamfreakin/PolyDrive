#include "WorldManager.h"
#include "Bus.h"
#include "SportsCar.h"
#include "Truck.h"
#include "Sedan.h"
#include <algorithm>
#include <ctime>

WorldManager::WorldManager() 
    : money(WorldData::INITIAL_MONEY), energy(WorldData::INITIAL_ENERGY), day(1), 
      currentCity(nullptr), currentCar(nullptr) {
    srand(static_cast<unsigned int>(time(NULL)));

    // 1. 도시 생성
    for (const auto& data : WorldData::CITIES) {
        City* newCity = new City(data.name, data.x, data.y);
        allCities.push_back(newCity);
        if (data.name == "Gwangju") currentCity = newCity;
    }

    // 2. 도시 간 경로(Edge) 연결
    for (const auto& rData : WorldData::ROUTES) {
        City* from = nullptr; City* to = nullptr;
        for (City* c : allCities) {
            if (c->GetName() == rData.from) from = c;
            if (c->GetName() == rData.to) to = c;
        }
        if (from && to) from->AddRoute(to, rData.distance, rData.reward);
    }

    // 3. MapManager 초기화 (20x20)
    mapManager = std::make_unique<MapManager>(20, 20, allCities);
    if (currentCity) mapManager->SetPlayerPos(currentCity->GetX(), currentCity->GetY());

    GenerateShop();
    
    // 초기 기본 차량 지급 (Sedan)
    auto starter = std::make_unique<Sedan>("Starter Sedan", 90.0f, 8.0f, 100.0f, 0);
    currentCar = starter.get();
    garage.push_back(std::move(starter));
}

WorldManager::~WorldManager() {
    for (City* c : allCities) delete c;
    allCities.clear();
}

void WorldManager::GenerateShop() {
    shopList.clear();
    itemShopList.clear();

    // 차량 생성 (6대)
    for (int i = 0; i < 6; ++i) {
        CarType type = static_cast<CarType>(rand() % (int)CarType::COUNT);
        float modifier = 0.7f + (rand() % 61) / 100.0f; 
        std::string name = WorldData::NAME_POOLS[type][rand() % 20];
        
        std::unique_ptr<Car> newCar = nullptr;
        switch (type) {
            case CarType::BUS: newCar = std::make_unique<Bus>(name, 70.0f * modifier, 4.0f * modifier, 100.0f, (int)(3000 * modifier)); break;
            case CarType::SPORTSCAR: newCar = std::make_unique<SportsCar>(name, 140.0f * modifier, 6.0f * modifier, 100.0f, (int)(5000 * modifier)); break;
            case CarType::TRUCK: newCar = std::make_unique<Truck>(name, 60.0f * modifier, 10.0f * modifier, 100.0f, (int)(4000 * modifier)); break;
            case CarType::SEDAN: newCar = std::make_unique<Sedan>(name, 90.0f * modifier, 8.0f * modifier, 100.0f, (int)(2500 * modifier)); break;
        }
        if (newCar) shopList.push_back(std::move(newCar));
    }

    // 아이템 생성 (4개)
    for (int i = 0; i < 4; ++i) {
        if (rand() % 2 == 0) {
            itemShopList.push_back({ ItemType::FUEL, "Emergency Fuel", 500, 30.0f });
        } else {
            itemShopList.push_back({ ItemType::REPAIR_KIT, "Basic Repair Kit", 800, 25.0f });
        }
    }
}

void WorldManager::RestDay() {
    day++;
    energy = 100;
    GenerateShop();
}

bool WorldManager::Travel(int routeIdx, std::string& outMsg) {
    if (!currentCar) { outMsg = "No car selected!"; return false; }
    const auto& routes = GetCurrentRoutes();
    if (routeIdx < 0 || routeIdx >= (int)routes.size()) return false;

    const Route& target = routes[routeIdx];
    int requiredEnergy = (int)(target.distance / currentCar->GetEfficiency());
    if (energy < requiredEnergy) { outMsg = "Not enough energy!"; return false; }

    energy -= requiredEnergy;
    currentCar->Move(target.distance);
    currentCar->ReduceCondition(15.0f); // 장거리 운송 시 큰폭 감소
    
    float rewardMod = 0.8f + (rand() % 41) / 100.0f;
    int finalReward = (int)(target.baseReward * rewardMod);
    money += finalReward;

    currentCity = target.destination;
    outMsg = "Arrived at " + currentCity->GetName() + "! Reward: " + std::to_string(finalReward) + "G";

    if (currentCar->GetCondition() <= 0) {
        outMsg += "\n[CRITICAL] Your car has been destroyed!";
        auto it = std::find_if(garage.begin(), garage.end(), [this](const std::unique_ptr<Car>& p) { return p.get() == currentCar; });
        if (it != garage.end()) garage.erase(it);
        currentCar = nullptr;
    }
    return true;
}

bool WorldManager::BuyCar(int shopIdx, std::string& outMsg) {
    if (shopIdx < 0 || shopIdx >= (int)shopList.size()) return false;
    auto& target = shopList[shopIdx];
    if (money < target->GetPrice()) { outMsg = "Not enough money!"; return false; }

    money -= target->GetPrice();
    outMsg = "Purchased " + target->GetName() + "!";
    garage.push_back(std::move(shopList[shopIdx]));
    shopList.erase(shopList.begin() + shopIdx);
    return true;
}

bool WorldManager::BuyItem(int itemIdx, std::string& outMsg) {
    if (itemIdx < 0 || itemIdx >= (int)itemShopList.size()) return false;
    const auto& target = itemShopList[itemIdx];
    if (money < target.price) { outMsg = "Not enough money!"; return false; }

    money -= target.price;
    inventory.push_back(target);
    outMsg = "Bought " + target.name + "!";
    itemShopList.erase(itemShopList.begin() + itemIdx);
    return true;
}

bool WorldManager::UseItem(int invIdx, std::string& outMsg) {
    if (invIdx < 0 || invIdx >= (int)inventory.size()) return false;
    const auto& item = inventory[invIdx];

    if (item.type == ItemType::FUEL) {
        energy += (int)item.effectValue;
        if (energy > 100) energy = 100;
        outMsg = "Used " + item.name + ". Energy + " + std::to_string((int)item.effectValue);
    } else if (item.type == ItemType::REPAIR_KIT) {
        if (!currentCar) { outMsg = "No car to repair!"; return false; }
        currentCar->Repair(item.effectValue);
        outMsg = "Used " + item.name + ". Car Condition + " + std::to_string((int)item.effectValue) + "%";
    }

    inventory.erase(inventory.begin() + invIdx);
    return true;
}

void WorldManager::TowingService(std::string& outMsg) {
    money -= 1000;
    if (money < 0) money = 0;
    
    // Gwangju로 강제 소환
    for (City* c : allCities) {
        if (c->GetName() == "Gwangju") {
            currentCity = c;
            mapManager->SetPlayerPos(c->GetX(), c->GetY());
            break;
        }
    }
    energy = 50; // 견인 후 약간의 에너지 지급
    outMsg = "[TOWING] You were towed to Gwangju. (Cost: 1000G)";
}

void WorldManager::SelectCar(int garageIdx) {
    if (garageIdx >= 0 && garageIdx < (int)garage.size()) {
        currentCar = garage[garageIdx].get();
    }
}

const std::vector<Route>& WorldManager::GetCurrentRoutes() const {
    return currentCity->GetRoutes();
}
