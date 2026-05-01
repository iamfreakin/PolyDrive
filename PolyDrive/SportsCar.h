#pragma once
#include "Car.h"

class SportsCar : public Car {
public:
    SportsCar(std::string n, float speed, float eff, float cond, int p)
        : Car(n, speed, eff, cond, p) {}

    void ShowSpec() const override {
        std::cout << "[SportsCar]";
        Car::ShowSpec();
    }
};
