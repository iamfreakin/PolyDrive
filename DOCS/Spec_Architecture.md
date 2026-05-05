# 🏗️ [TECHNICAL SPEC] SYSTEM ARCHITECTURE
## 1. 개요 (Overview)
본 문서는 PolyDrive 프로젝트의 핵심 시스템 아키텍처 및 객체 생명주기 관리 전략을 기술합니다. PolyDrive는 고도로 모듈화된 **Manager-Pattern**을 기반으로 설계되어, 논리적 관심사를 엄격히 분리하고 데이터 지향적인 확장을 지향합니다.

---

## 2. 핵심 매니저 시스템 (Manager Clusters)

### 2.1 WorldManager (The Controller)
- **역할**: 게임의 전역 상태(State)와 자산 데이터를 소유하는 Central Kernel.
- **주요 책임**:
  - `Money`, `Energy`, `Day` 등 핵심 수치 데이터 소유 및 갱신.
  - `std::unique_ptr`를 활용한 차량 가라지(Garage) 및 상점(Shop) 생명주기 관리.
  - 미션 시스템의 트리거 및 완료 조건 검증.
  - 게임의 승리/패배 조건 판정 및 결과 데이터(`ScoreResult`) 산출.

### 2.2 MapManager (The Physical Engine)
- **역할**: 2차원 그리드 시스템 내에서의 좌표 연산 및 충돌 물리 담당.
- **주요 책임**:
  - `40x40` 그리드 경계 체크 및 플레이어 좌표(`playerX`, `playerY`) 갱신.
  - 차량의 효율성(`Efficiency`)을 기반으로 한 이동 에너지 소모량 계산.
  - 그리드 좌표와 도시 객체 간의 위치 동기화 및 도착 판정.

### 2.3 UIManager (The Visualizer)
- **역할**: ANSI Escape Code 기반의 고성능 콘솔 출력 엔진.
- **주요 책임**:
  - `SetConsoleCursorPosition` API를 이용한 렌더링 최적화.
  - 현재 모드(Map, Shop, Garage 등)에 따른 화면 컨텍스트 스위칭.
  - 게임 로그(Log) 시스템의 시간 순차적 출력 및 관리.

---

## 3. 메모리 관리 전략 (Memory Strategy)

PolyDrive는 **현대적 C++ (C++20)** 표준을 따르며, 직접적인 `new`/`delete` 사용을 지양하고 RAII 원칙을 고수합니다.

- **Unique Ownership**: 모든 `Car` 객체와 매니저 클래스는 `std::unique_ptr`를 통해 소유권을 명확히 하며, 소유권 이전이 필요한 경우에만 `std::move`를 사용합니다.
- **Pointer Passing**: 렌더링이나 계산을 위해 객체에 접근할 때는 원시 포인터(`Raw Pointer`)를 통해 '관찰'만 수행하며, 수명 연장이 필요한 경우에만 참조(`&`)를 사용합니다.
- **Static World Data**: 변경되지 않는 월드 상수(도시 좌표, 이름 등)는 `WorldData` 네임스페이스 내의 `static` 데이터를 사용하여 런타임 오버헤드를 최소화합니다.

---

## 4. 확장성 설계 (Scalability)

- **OCP (Open-Closed Principle)**: 새로운 차량 타입 추가 시 `Car` 클래스를 상속받는 것만으로 기존 코드 수정 없이 시스템에 편입 가능합니다.
- **Data-Driven Approach**: `WorldData.h`의 데이터 수정만으로 게임의 밸런스, 맵 구성, 미션 경로를 동적으로 변경할 수 있습니다.

---
*Last Updated: 2024-05-06 | PolyDrive Development Team*
