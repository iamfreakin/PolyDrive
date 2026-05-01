#pragma once
#include "Car.h"

class Sedan : public Car {
public:
    Sedan(std::string n, float speed, float eff, float cond, int p)
        : Car(n, speed, eff, cond, p) {}

    void ShowSpec() const override {
        std::cout << "[Sedan    ]";
        Car::ShowSpec();
    }
};
