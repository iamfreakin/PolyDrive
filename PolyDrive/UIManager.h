#pragma once
#include "WorldManager.h"
#include <string>

class UIManager {
private:
    std::string lastLog;

public:
    UIManager();
    
    // 전체 화면을 다시 그림
    void DrawGame(const WorldManager& wm);
    
    // 섹션별 출력 함수
    void DrawHeader();
    void DrawStatus(const WorldManager& wm);
    void DrawMenu();
    void DrawMainContent(const WorldManager& wm, int mode); // 1: Move, 3: Garage, 4: Shop
    
    // 로그 설정
    void SetLog(std::string msg) { lastLog = msg; }
    void ClearLog() { lastLog = ""; }
};
