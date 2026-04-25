#include "Truck.h"

Truck::Truck(std::string n, float speed) : Car(n, speed) {}

float Truck::Move(float distance) {
    float travelTime = distance / baseSpeed;

    totalDistance += distance;
    totalTime += travelTime;
    return travelTime;
}
