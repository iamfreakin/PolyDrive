# Specification: Architecture & Core Loop

## 1. 개요
PolyDrive는 매니저 패턴을 기반으로 하는 텍스트 시뮬레이션 엔진입니다. `Main.cpp`는 최소한의 진입점 역할을 수행하며, 실질적인 상태 관리는 `WorldManager`가 담당합니다.

## 2. 주요 구성 요소
- **WorldManager**: 게임의 전역 상태(돈, 에너지, 날짜, 플레이어 위치)를 관리하며, 다른 매니저들을 조율합니다.
- **UIManager**: 콘솔 출력 및 사용자 입력을 시각화합니다.
- **MapManager**: 2D 격자 맵 시스템 및 충돌/도시 감지를 처리합니다.

## 3. 게임 루프 생명주기
1. **Initialize**: `WorldData`로부터 초기 데이터를 로드하고 각 매니저를 인스턴스화합니다.
2. **Render**: `UIManager`가 현재 상태를 화면에 출력합니다.
3. **Input**: `_getch()`를 통해 비차단 입력을 대기합니다.
4. **Update**: 입력에 따라 상태를 변경하고 패널티(Towing) 조건을 체크합니다.
5. **Repeat**: `Exit` 명령 전까지 위 과정을 반복합니다.
