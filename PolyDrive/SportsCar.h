#pragma once
#include "Car.h"

class SportsCar : public Car {
public:
    SportsCar(std::string n, float speed);
    float Move(float distance) override;
};
