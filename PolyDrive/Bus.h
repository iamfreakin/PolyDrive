#pragma once
#include "Car.h"

class Bus : public Car {
public:
    Bus(std::string n, float speed, float eff, float cond, int p)
        : Car(n, speed, eff, cond, p) {}

    void ShowSpec() const override {
        std::cout << "[Bus      ]";
        Car::ShowSpec();
    }
};
