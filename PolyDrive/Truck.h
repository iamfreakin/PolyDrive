#pragma once
#include "Car.h"

class Truck : public Car {
public:
    Truck(std::string n, float speed);
    float Move(float distance) override;
};
