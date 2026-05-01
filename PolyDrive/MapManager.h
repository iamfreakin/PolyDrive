#pragma once
#include <vector>
#include "City.h"

class MapManager {
private:
    int width, height;
    int playerX, playerY;
    const std::vector<City*>& cities; // WorldManager의 도시 목록 참조

public:
    MapManager(int w, int h, const std::vector<City*>& cityList);

    void SetPlayerPos(int x, int y) { playerX = x; playerY = y; }
    
    // WASD 이동 처리
    // return: 0(이동불가/에너지부족), 1(이동성공), 2(도시 도착)
    int MovePlayer(int dx, int dy, int& energy, float efficiency);

    // 현재 플레이어 위치에 도시가 있는지 반환
    City* GetCityAtPlayer() const;

    // Getter
    int GetPlayerX() const { return playerX; }
    int GetPlayerY() const { return playerY; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};
