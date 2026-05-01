#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Car.h"
#include "WorldData.h"
#include "City.h"
#include "MapManager.h"
#include "Item.h"

class WorldManager {
private:
    int money;
    int energy;
    int day;
    City* currentCity;
    std::vector<City*> allCities; 
    std::unique_ptr<MapManager> mapManager;
    Car* currentCar;
    std::vector<std::unique_ptr<Car>> garage;
    std::vector<std::unique_ptr<Car>> shopList;
    
    // 아이템 관련
    std::vector<Item> inventory;
    std::vector<Item> itemShopList;

public:
    WorldManager();
    ~WorldManager();

    void GenerateShop();
    void RestDay();
    bool Travel(int routeIdx, std::string& outMsg);
    bool BuyCar(int shopIdx, std::string& outMsg);
    void SelectCar(int garageIdx);

    // 아이템 관련 메서드
    bool BuyItem(int itemIdx, std::string& outMsg);
    bool UseItem(int invIdx, std::string& outMsg);
    
    // 패널티 처리 (견인)
    void TowingService(std::string& outMsg);

    // Getter
    int GetMoney() const { return money; }
    int& GetEnergyRef() { return energy; }
    int GetEnergy() const { return energy; }
    int GetDay() const { return day; }
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
