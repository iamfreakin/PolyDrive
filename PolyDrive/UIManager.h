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
    void Render(const WorldManager& wm, int mode);

    // 로그 설정
    void SetLog(std::string msg) { lastLog = msg; }
    void ClearLog() { lastLog = ""; }
    
private:
    // 섹션별 문자열 생성 함수
    std::string GetHeaderStr();
    std::string GetStatusStr(const WorldManager& wm);
    std::string GetMainContentStr(const WorldManager& wm, int mode);
    std::string GetMenuStr(const WorldManager& wm);
};
