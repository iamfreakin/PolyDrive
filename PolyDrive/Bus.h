#pragma once
#include "Car.h"

class Bus : public Car {
public:
    Bus(std::string n, float speed);
    float Move(float distance) override;
};
