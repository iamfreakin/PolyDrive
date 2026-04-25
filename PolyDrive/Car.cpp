#include "Car.h"
#include <iostream>
#include <iomanip>

Car::Car(std::string n, float speed) 
    : name(n), baseSpeed(speed), totalDistance(0.0f), totalTime(0.0f) {}

Car::~Car() {}

void Car::Display() const {
    std::cout << std::left << std::setw(15) << name 
              << " | Speed: " << std::setw(5) << baseSpeed << " km/h"
              << " | Total: " << std::fixed << std::setprecision(1) << totalDistance << " km"
              << " (" << totalTime << " hours)" << std::endl;
}

std::string Car::GetName() const { return name; }
float Car::GetTotalDistance() const { return totalDistance; }
float Car::GetTotalTime() const { return totalTime; }
