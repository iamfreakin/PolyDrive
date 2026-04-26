#pragma once
#include "Car.h"

class Truck : public Car {
public:
    Truck(std::string n, float speed, float eff, int dur, int p)
        : Car(n, speed, eff, dur, p) {}

    void ShowSpec() const override {
        std::cout << "[Truck] " << std::left << std::setw(12) << name;
        Car::ShowSpec();
    }
};
