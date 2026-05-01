#pragma once
#include <vector>
#include "City.h"
#include "Car.h"

class MapManager {
private:
    int width, height;
    int playerX, playerY;
    const std::vector<City*>& cities;

public:
    MapManager(int w, int h, const std::vector<City*>& cityList);

    void SetPlayerPos(int x, int y) { playerX = x; playerY = y; }
    
    // WASD 이동 처리
    // car 포인터를 직접 받아 HP를 감소시킴
    int MovePlayer(int dx, int dy, int& energy, Car* car);

    City* GetCityAtPlayer() const;

    int GetPlayerX() const { return playerX; }
    int GetPlayerY() const { return playerY; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};
