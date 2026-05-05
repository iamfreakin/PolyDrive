#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Car.h"
#include "WorldData.h"
#include "City.h"
#include "MapManager.h"
#include "Item.h"

struct Mission {
    City* destination = nullptr;
    int reward = 0;
    std::string cargoName = "";
    bool isActive = false;
    bool isFinal = false; // 최종 미션 여부
};

struct ScoreResult {
    int totalScore;
    int days;
    int repairs;
    int carsOwned;
    std::string rank;
};

class WorldManager {
private:
    int money;
    int energy;
    int day;
    int totalRepairs; // 총 견인/수리 횟수
    bool gameCleared; // 게임 클리어 여부
    ScoreResult finalScore;

    City* currentCity;
    std::vector<City*> allCities; 
    std::unique_ptr<MapManager> mapManager;
    Car* currentCar;
    std::vector<std::unique_ptr<Car>> garage;
    std::vector<std::unique_ptr<Car>> shopList;
    
    // 아이템 관련
    std::vector<Item> inventory;
    std::vector<Item> itemShopList;

    Mission currentMission; // 현재 배달 미션

public:
    WorldManager();
    ~WorldManager();

    void GenerateShop();
    void RestDay();
    
    // 미션 관련
    bool AcceptMission(int routeIdx, std::string& outMsg);
    void CompleteMission(std::string& outMsg);
    const Mission& GetCurrentMission() const { return currentMission; }
    
    // 최종 미션 트리거 확인
    bool CheckFinalMissionTrigger();
    void TriggerFinalMission();

    bool BuyCar(int shopIdx, std::string& outMsg);
    void SelectCar(int garageIdx);

    // 아이템 및 수리 관련 메서드
    bool BuyItem(int itemIdx, std::string& outMsg);
    bool UseItem(int invIdx, std::string& outMsg);
    bool RepairAtCity(std::string& outMsg); // 도시 수리점 서비스 추가
    
    // 패널티 처리 (견인)
    void TowingService(std::string& outMsg);

    // 점수 계산
    void CalculateFinalScore();
    const ScoreResult& GetFinalScore() const { return finalScore; }
    bool IsGameCleared() const { return gameCleared; }

    // Getter
    int GetMoney() const { return money; }
    int& GetEnergyRef() { return energy; }
    int GetEnergy() const { return energy; }
    int GetDay() const { return day; }
    int GetTotalRepairs() const { return totalRepairs; }
    City* GetCurrentCity() const { return currentCity; }
    void SetCurrentCity(City* c) { currentCity = c; }
    MapManager* GetMapManager() const { return mapManager.get(); }
    Car* GetCurrentCar() const { return currentCar; }
    const std::vector<City*>& GetAllCities() const { return allCities; }
    const std::vector<std::unique_ptr<Car>>& GetGarage() const { return garage; }
    const std::vector<std::unique_ptr<Car>>& GetShopList() const { return shopList; }
    const std::vector<Item>& GetInventory() const { return inventory; }
    const std::vector<Item>& GetItemShopList() const { return itemShopList; }
    
    const std::vector<Route>& GetCurrentRoutes() const;
};
