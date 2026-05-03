#pragma once
#define NOMINMAX
#include "WorldManager.h"
#include <string>
#include <windows.h>

class UIManager {
private:
    std::string lastLog;
    HANDLE hConsole;
    int lastMode;

    // ANSI 색상 코드 정의
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BOLD = "\033[1m";
    const std::string GRAY = "\033[90m";

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
