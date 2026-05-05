#include "WorldManager.h"
#include "Bus.h"
#include "SportsCar.h"
#include "Truck.h"
#include "Sedan.h"
#include <algorithm>
#include <ctime>
#include <cmath>

WorldManager::WorldManager() 
    : money(WorldData::INITIAL_MONEY), energy(WorldData::INITIAL_ENERGY), day(1), 
      totalRepairs(0), gameCleared(false), currentCity(nullptr), currentCar(nullptr) {
    srand(static_cast<unsigned int>(time(NULL)));
    finalScore = { 0, 0, 0, 0, "" };

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

    // 3. MapManager 초기화 (40x40)
    mapManager = std::make_unique<MapManager>(40, 40, allCities);
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
            itemShopList.push_back({ ItemType::REPAIR_KIT, "Basic Repair Kit", 800, 80.0f });
        }
    }
}

void WorldManager::RestDay() {
    day++;
    energy = 100;
    GenerateShop();
}

bool WorldManager::AcceptMission(int routeIdx, std::string& outMsg) {
    if (currentMission.isActive) {
        outMsg = "Already have an active mission!";
        return false;
    }
    const auto& routes = GetCurrentRoutes();
    if (routeIdx < 0 || routeIdx >= (int)routes.size()) return false;

    const Route& target = routes[routeIdx];
    
    std::string cargoPool[] = {"Apples", "Electronics", "Medical Kits", "Spare Parts", "Luxury Goods", "Daily Supplies"};
    currentMission.cargoName = cargoPool[rand() % 6];
    currentMission.destination = target.destination;
    currentMission.reward = target.baseReward;
    currentMission.isActive = true;
    currentMission.isFinal = false;

    outMsg = "Mission Accepted: Deliver [" + currentMission.cargoName + "] to " + currentMission.destination->GetName() + "!";
    return true;
}

void WorldManager::CompleteMission(std::string& outMsg) {
    if (!currentMission.isActive || !currentCity) return;

    if (currentCity == currentMission.destination) {
        if (currentMission.isFinal) {
            gameCleared = true;
            CalculateFinalScore();
            outMsg = " [★ GAME CLEAR ★] You've completed the Final Mission!";
        } else {
            float rewardMod = 0.8f + (rand() % 41) / 100.0f;
            int finalReward = (int)(currentMission.reward * rewardMod);
            money += finalReward;
            outMsg = " [MISSION COMPLETE] Delivered " + currentMission.cargoName + "! Reward: " + std::to_string(finalReward) + "G";
        }
        currentMission.isActive = false;
        currentMission.destination = nullptr;
    }
}

bool WorldManager::CheckFinalMissionTrigger() {
    return (money >= 50000 && !currentMission.isActive && !gameCleared);
}

void WorldManager::TriggerFinalMission() {
    City* furthest = nullptr;
    float maxDist = -1.0f;
    for (City* c : allCities) {
        if (c == currentCity) continue;
        float dx = (float)(c->GetX() - currentCity->GetX());
        float dy = (float)(c->GetY() - currentCity->GetY());
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist > maxDist) {
            maxDist = dist;
            furthest = c;
        }
    }

    if (furthest) {
        currentMission.cargoName = "THE GOLDEN CARGO";
        currentMission.destination = furthest;
        currentMission.reward = 0;
        currentMission.isActive = true;
        currentMission.isFinal = true;
    }
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

bool WorldManager::RepairAtCity(std::string& outMsg) {
    if (!currentCity) { outMsg = "Repair is only available in a city!"; return false; }
    if (!currentCar) { outMsg = "No car to repair!"; return false; }
    
    float damage = 100.0f - currentCar->GetCondition();
    if (damage <= 0.1f) { outMsg = "Your car is already in perfect condition!"; return false; }
    
    int cost = (int)(damage * 25); // 1%당 25G (수리키트보다 비싸지만 확실함)
    if (money < cost) { outMsg = "Not enough money! (Required: " + std::to_string(cost) + "G)"; return false; }
    
    money -= cost;
    currentCar->Repair(100.0f);
    outMsg = "Car fully repaired! (Cost: " + std::to_string(cost) + "G)";
    return true;
}

void WorldManager::TowingService(std::string& outMsg) {
    money -= 1000;
    if (money < 0) money = 0;
    totalRepairs++;
    
    int px = mapManager->GetPlayerX();
    int py = mapManager->GetPlayerY();
    City* closest = nullptr;
    float minDist = 99999.0f;

    for (City* c : allCities) {
        float dx = (float)(c->GetX() - px);
        float dy = (float)(c->GetY() - py);
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < minDist) {
            minDist = dist;
            closest = c;
        }
    }

    if (closest) {
        currentCity = closest;
        mapManager->SetPlayerPos(closest->GetX(), closest->GetY());
        outMsg = "[TOWING] You were towed to the nearest city: " + closest->GetName() + ". (Cost: 1000G)";
    }

    energy = 50;
    if (currentCar && currentCar->GetCondition() <= 0) {
        currentCar->Repair(20.0f);
    }
}

void WorldManager::CalculateFinalScore() {
    int score = 10000;
    score -= (day * 100);
    score -= (totalRepairs * 500);
    score += ((int)garage.size() * 1000);

    if (score < 0) score = 0;

    finalScore.totalScore = score;
    finalScore.days = day;
    finalScore.repairs = totalRepairs;
    finalScore.carsOwned = (int)garage.size();

    if (score >= 9000) finalScore.rank = "S (GOD DRIVER)";
    else if (score >= 7000) finalScore.rank = "A (EXPERT)";
    else if (score >= 5000) finalScore.rank = "B (PRO)";
    else if (score >= 3000) finalScore.rank = "C (ROOKIE)";
    else finalScore.rank = "F (TOWING LOVER)";
}

void WorldManager::SelectCar(int garageIdx) {
    if (garageIdx >= 0 && garageIdx < (int)garage.size()) {
        currentCar = garage[garageIdx].get();
    }
}

const std::vector<Route>& WorldManager::GetCurrentRoutes() const {
    return currentCity->GetRoutes();
}
