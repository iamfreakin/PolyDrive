#pragma once
#include <string>
#include <iostream>
#include <iomanip>

class Car {
protected:
    std::string name;
    float baseSpeed;
    float efficiency; 
    float condition;  // 내구도 HP (0.0 ~ 100.0)
    int price;

public:
    Car(std::string n, float speed, float eff, float cond, int p)
        : name(n), baseSpeed(speed), efficiency(eff), condition(cond), price(p) {}
    
    virtual ~Car() {}

    // 이동 로직: 소요 시간을 반환하고 내구도를 차감함
    virtual float Move(float distance) {
        // 거리나 환경에 따라 차감 (기본은 외부에서 제어하거나 여기서 거리 비례 차감)
        return distance / baseSpeed;
    }

    // 내구도 직접 차감
    void ReduceCondition(float amount) {
        condition -= amount;
        if (condition < 0) condition = 0;
    }

    // 수리
    void Repair(float amount) {
        condition += amount;
        if (condition > 100.0f) condition = 100.0f;
    }

    virtual void ShowSpec() const {
        std::cout << std::fixed << std::setprecision(1)
                  << " | Speed: " << std::setw(5) << baseSpeed 
                  << " | Eff: " << std::setw(4) << efficiency 
                  << " | HP: " << std::setw(5) << condition << "%"
                  << " | Price: " << std::setw(5) << price << "G\n";
    }

    // Getter
    std::string GetName() const { return name; }
    float GetCondition() const { return condition; }
    int GetPrice() const { return price; }
    float GetEfficiency() const { return efficiency; }
};
