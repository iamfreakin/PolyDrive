#include "Bus.h"

Bus::Bus(std::string n, float speed) : Car(n, speed) {}

float Bus::Move(float distance) {
    float travelTime = distance / baseSpeed;
    float stopTime = 0.5f; 
    float totalMoveTime = travelTime + stopTime;

    totalDistance += distance;
    totalTime += totalMoveTime;
    return totalMoveTime;
}
