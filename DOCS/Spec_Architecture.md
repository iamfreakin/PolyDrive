# 🏗️ Architecture Specification: Systems & Flow

PolyDrive의 핵심 매니저 클래스와 데이터 흐름을 정의합니다.

## 1. World & Mission System

### 1.1 World Expansion
- **40x40 Grid World**: 탐험 영역이 400에서 1600칸으로 확장되었습니다.
- **Dynamic City Placement**: `WorldData.h`를 통해 도시의 절대 좌표를 관리하며, 카메라 스크롤을 통해 가변적으로 출력됩니다.

### 1.2 Mission Delivery System
- **AcceptMission**: 도시에서 특정 목적지까지의 운송 업무를 수락합니다.
- **Navigation**: 목적지 방향과 거리를 계산하여 플레이어에게 가이드를 제공합니다.
- **CompleteMission**: 목적지 도시에 도착 시 자동으로 체크되어 보상(Gold)을 지급합니다.

## 2. Core Logic Components

### `WorldManager`
- 게임의 상태(돈, 날짜, 에너지, 현재 미션)를 총괄합니다.
- `std::unique_ptr<MapManager>`를 소유하여 물리적 이동을 관리합니다.
- **Towing Service (견인 서비스)**: 
  - 에너지 고갈 혹은 차량 파손 시 발생.
  - **Dynamic Relocation**: 플레이어의 현재 위치$(px, py)$에서 모든 도시$(cx, cy)$까지의 유클리드 거리($\sqrt{(cx-px)^2 + (cy-py)^2}$)를 계산하여 가장 가까운 도시로 강제 소환합니다.
  - **Penalty**: 1000G 차감 (최소 0G 유지).
  - **Recovery**: 견인 즉시 에너지 50% 회복 및 파손 차량의 경우 최소 주행 가능 상태(내구도 20%)로 긴급 수리 지원.

### `MapManager`
- 플레이어의 좌표(`px`, `py`)와 맵 경계 체크를 담당합니다.
- `MovePlayer`: 이동 효율에 따른 에너지 소모와 차량 내구도 감소를 처리합니다.

## 3. Class Inheritance (Vehicle)
- **`Car` (Abstract Base)**: `Drive()`, `Repair()`, `ShowSpec()` 인터페이스 제공.
  - **`Sedan`**: 밸런스형.
  - **`Truck`**: 높은 내구도, 낮은 연비.
  - **`SportsCar`**: 높은 연비(속도), 낮은 내구도.
  - **`Bus`**: 안정적인 운송 효율.

## 4. Execution Loop
1. **Render**: `UIManager`가 통합 뷰를 출력.
2. **Input**: `_getch()`를 통한 무한 대기 방지 및 즉각적 반응.
3. **Update**: 이동 처리, 미션 완료 체크, 패널티(에너지 0) 확인.
4. **Repeat**: `choice == 0` 일 때까지 반복.
