#include "SportsCar.h"

SportsCar::SportsCar(std::string n, float speed) : Car(n, speed) {}

float SportsCar::Move(float distance) {
    float travelTime = distance / (baseSpeed * 1.2f);
    
    totalDistance += distance;
    totalTime += travelTime;
    return travelTime;
}
