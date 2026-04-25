#pragma once
#include <string>

class Car {
protected:
    std::string name;
    float baseSpeed;
    float totalDistance;
    float totalTime;

public:
    Car(std::string n, float speed);
    virtual ~Car();

    // 순수 가상 함수
    virtual float Move(float distance) = 0;

    // 가상 함수
    virtual void Display() const;

    // Getter
    std::string GetName() const;
    float GetTotalDistance() const;
    float GetTotalTime() const;
};
