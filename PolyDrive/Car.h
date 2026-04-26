#pragma once
#include <string>
#include <iostream>
#include <iomanip>

class Car {
protected:
    std::string name;
    float baseSpeed;
    float efficiency; // 연비 (거리당 에너지 소모율의 역수 개념)
    int durability;   // 남은 이동 가능 횟수 (1~3)
    int price;

public:
    Car(std::string n, float speed, float eff, int dur, int p)
        : name(n), baseSpeed(speed), efficiency(eff), durability(dur), price(p) {}
    
    virtual ~Car() {}

    // 이동 로직: 소요 시간을 반환하고 내구도를 1 감소시킴
    virtual float Move(float distance) {
        durability--;
        return distance / baseSpeed;
    }

    // 차량 스펙 출력
    virtual void ShowSpec() const {
        std::cout << " [" << name << "]\n";
        std::cout << " - Speed: " << std::fixed << std::setprecision(1) << baseSpeed << " km/h\n";
        std::cout << " - Efficiency: " << efficiency << " km/E\n";
        std::cout << " - Durability: " << durability << " / 3\n";
        std::cout << " - Price: " << price << " G\n";
    }

    // Getter
    std::string GetName() const { return name; }
    int GetDurability() const { return durability; }
    int GetPrice() const { return price; }
    float GetEfficiency() const { return efficiency; }
};
