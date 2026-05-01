#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Car.h"
#include "WorldData.h"

class WorldManager {
private:
    int money;
    int energy;
    int day;
    std::string currentCity;
    Car* currentCar;
    std::vector<std::unique_ptr<Car>> garage;
    std::vector<std::unique_ptr<Car>> shopList;

public:
    WorldManager();
    ~WorldManager();

    // 상점 생성 (6대 랜덤)
    void GenerateShop();
    
    // 휴식 (에너지 충전, 상점 갱신)
    void RestDay();
    
    // 이동 로직 (성공 여부 반환)
    bool Travel(int routeIdx, std::string& outMsg);
    
    // 차량 구매
    bool BuyCar(int shopIdx, std::string& outMsg);
    
    // 운행 차량 선택
    void SelectCar(int garageIdx);

    // Getter
    int GetMoney() const { return money; }
    int GetEnergy() const { return energy; }
    int GetDay() const { return day; }
    std::string GetCurrentCity() const { return currentCity; }
    Car* GetCurrentCar() const { return currentCar; }
    const std::vector<std::unique_ptr<Car>>& GetGarage() const { return garage; }
    const std::vector<std::unique_ptr<Car>>& GetShopList() const { return shopList; }
    
    // 현재 도시의 경로 리스트 반환
    std::vector<Route> GetCurrentRoutes() const;
};
