#pragma once
#include "Car.h"

class Truck : public Car {
public:
    Truck(std::string n, float speed, float eff, float cond, int p)
        : Car(n, speed, eff, cond, p) {}

    void ShowSpec() const override {
        std::cout << "[Truck    ]";
        Car::ShowSpec();
    }
};
