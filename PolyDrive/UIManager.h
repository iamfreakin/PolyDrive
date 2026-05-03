#pragma once
#include "WorldManager.h"
#include <string>
#include <windows.h>

class UIManager {
private:
    std::string lastLog;
    HANDLE hConsole;
    int lastMode; // 이전 모드 추적용

    // 내부 유틸리티
    void SetCursor(int x, int y);
    void HideCursor();

public:
    UIManager();
    
    // 전체 화면을 다시 그림 (최적화 버전)
    void DrawGame(const WorldManager& wm, int mode);
    
    // 섹션별 출력 함수
    void DrawHeader();
    void DrawStatus(const WorldManager& wm);
    void DrawMenu(const WorldManager& wm);
    void DrawMainContent(const WorldManager& wm, int mode); // 0: Map, 1: Cargo, 3: Garage, 4: Shop, 5: Inv
    
    // 로그 설정
    void SetLog(std::string msg) { lastLog = msg; }
    void ClearLog() { lastLog = ""; }
};
