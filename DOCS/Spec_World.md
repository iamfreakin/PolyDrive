# Specification: World & Map System

## 1. Dual Map Structure
PolyDrive는 두 가지 레이어의 지도 시스템을 동시에 운영합니다.

### A. Grid Layer (Exploration)
- **MapManager**: 20x20 크기의 2D 공간 관리.
- **이동**: WASD 입력에 따라 플레이어 좌표(`x`, `y`) 갱신.
- **감지**: 좌표 이동 시마다 해당 좌표에 도시 객체가 존재하는지 검사.

### B. Graph Layer (Transportation)
- **City (Node)**: 이름과 좌표 정보를 가짐.
- **Route (Edge)**: 도시 간 연결, 거리, 기본 보상 정보를 가짐.
- **로직**: 플레이어가 도시에 있을 때만 인접한 노드로의 `Travel` 메뉴 활성화.

## 2. 데이터 정의 (`WorldData`)
- 모든 도시 좌표와 노선 정보는 하드코딩되지 않고 `WorldData.h` 내의 정적 데이터 구조로 관리되어 확장이 용이합니다.
