#pragma once
#include "Car.h"

class Bus : public Car {
public:
    Bus(std::string n, float speed, float eff, int dur, int p)
        : Car(n, speed, eff, dur, p) {}

    void ShowSpec() const override {
        std::cout << "[Bus] " << std::left << std::setw(12) << name;
        Car::ShowSpec();
    }
};
