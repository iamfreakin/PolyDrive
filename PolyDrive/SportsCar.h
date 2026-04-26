#pragma once
#include "Car.h"

class SportsCar : public Car {
public:
    SportsCar(std::string n, float speed, float eff, int dur, int p)
        : Car(n, speed, eff, dur, p) {}

    void ShowSpec() const override {
        std::cout << "[SportsCar] " << std::left << std::setw(12) << name;
        Car::ShowSpec();
    }
};
