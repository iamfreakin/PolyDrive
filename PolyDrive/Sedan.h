#pragma once
#include "Car.h"

class Sedan : public Car {
public:
    Sedan(std::string n, float speed, float eff, int dur, int p)
        : Car(n, speed, eff, dur, p) {}

    void ShowSpec() const override {
        std::cout << "[Sedan]";
        Car::ShowSpec();
    }
};
