# 🏎️ PolyDrive: Modern C++ Logistics Simulator

**PolyDrive**는 현대적인 C++ 기술을 활용하여 제작된 그리드 기반 물류 시뮬레이션 게임입니다. 대한민국 주요 도시를 배경으로 한 가상의 맵에서 다양한 차량을 운전하며 화물을 배송하고, 자산을 관리하여 최고의 운송업자가 되는 것을 목표로 합니다.

---

## 📂 1. Module Architecture & File Map (기능별 파일 구조)

이 프로젝트는 기능적 역할에 따라 모듈이 엄격히 분리되어 있습니다. 각 기능이 구현된 위치는 다음과 같습니다.

### 🎮 Core Logic (커널 및 제어)
| 파일명 | 역할 및 주요 기능 | 핵심 로직 |
| :--- | :--- | :--- |
| **`Main.cpp`** | 게임 엔트리 포인트 및 메인 루프 | 사용자 입력 처리(`_getch`), 프레임 제어, 전역 상태 전이 |
| **`WorldManager.h/cpp`** | 게임의 '두뇌' 역할 (State Machine) | 자산 관리, 날짜 시스템, 미션 수락/완료 로직, 수리 및 견인 서비스 |
| **`MapManager.h/cpp`** | 좌표 및 물리 엔진 | 그리드 기반 플레이어 좌표 관리, 이동 가능 여부 체크, 도시 충돌 판정 |

### 🚛 Vehicle System (차량 및 다형성)
| 파일명 | 역할 및 주요 기능 | 핵심 로직 |
| :--- | :--- | :--- |
| **`Car.h`** | 차량 최상위 추상 클래스 | 내구도(`condition`) 관리, 이동 시 자원 소모 인터페이스 정의 |
| **`Sedan/Bus/Truck/SportsCar.h`** | 개별 차량 구체 클래스 | 각 차량 타입별 고유 스펙(속도, 효율) 및 특화 로직 |

### 🗺️ World Data & Environment (데이터 및 환경)
| 파일명 | 역할 및 주요 기능 | 핵심 로직 |
| :--- | :--- | :--- |
| **`City.h`** | 도시(Node) 및 경로(Edge) 정의 | 도시 간 거리 정보 및 그래프 구조 데이터 관리 |
| **`WorldData.h`** | 게임 내 모든 상수 및 데이터 풀 | 도시 좌표 리스트, 차량 이름 풀(`NAME_POOLS`), 초기 자산 설정 |
| **`Item.h`** | 소비 아이템 정의 | 연료, 수리 키트 등 아이템 타입 및 효과 정의 |

### 🖥️ Visualization (렌더링)
| 파일명 | 역할 및 주요 기능 | 핵심 로직 |
| :--- | :--- | :--- |
| **`UIManager.h/cpp`** | 콘솔 렌더링 엔진 | ANSI Escape Code 기반 색상 출력, 화면 부분 갱신(Cursor Control), 로그 시스템 |

---

## 🛠️ 2. Technical Deep Dive (핵심 기술 구현 상세)

### 🧬 Polymorphic Vehicle Management (다형성 및 메모리 관리)
- **RAII 패턴**: `WorldManager`는 `std::vector<std::unique_ptr<Car>>`를 사용하여 차량의 생명주기를 관리합니다. 이는 메모리 누수를 방지하고 객체 소유권을 명확히 합니다.
- **Dynamic Spec Generation**: 차량 구매 시 `WorldData`의 베이스 스펙에 무작위 배율(`0.7f ~ 1.3f`)을 적용하여 매번 다른 성능의 차량이 상점에 등장합니다.

### 🧭 Hybrid Movement System (그리드-그래프 하이브리드)
- **Local Grid**: 플레이어는 40x40 그리드 위에서 자유롭게 이동합니다 (`MapManager`).
- **Global Graph**: 도시 간의 거리는 유클리드 거리가 아닌 `WorldData::ROUTES`에 정의된 그래프 가중치를 기반으로 미션 보상이 산정됩니다.
- **Cost Calculation**: 이동 한 칸당 소모되는 에너지는 `ceil(10.0 / car->Efficiency)` 공식을 따르며, 이는 효율이 낮은 차량일수록 급격한 에너지 소모를 유발합니다.

### 🎨 State-Based Rendering Engine
- **Flicker Reduction**: `system("cls")` 사용을 지양하고, `UIManager`에서 Windows API의 `SetConsoleCursorPosition`을 활용하여 변경된 부분만 덮어씌우는 방식으로 렌더링 최적화를 진행 중입니다.
- **ANSI Styling**: `\033[...m` 코드를 사용하여 별도의 라이브러리 없이 풍부한 색상과 강조 효과를 구현했습니다.

---

## 📈 3. Economic Logic (경제 시스템 상세)

### 📦 Mission Algorithm
1. **Trigger**: 도시 도착 시 해당 도시에서 출발하는 `Route` 데이터 중 하나를 무작위로 선택하여 미션을 생성합니다.
2. **Reward**: `Base Reward` × `Random Multiplier(0.8 ~ 1.2)`를 통해 보상이 결정됩니다.
3. **The Golden Cargo**: 플레이어 자산이 50,000G에 도달하면, 현재 위치에서 가장 멀리 떨어진 도시를 목적지로 하는 최종 클리어 미션이 자동 활성화됩니다.

### 🚨 Penalty System
- **Towing Service**: 고립 상황(에너지 0 또는 내구도 0) 발생 시 `TowingService()`가 호출됩니다.
- **Logic**: 현재 좌표에서 가장 가까운 도시를 검색하여 강제 소환하며, 패널티 금액 차감 후 차량을 최소 운행 가능 상태(내구도 20%)로 긴급 복구합니다.

---

## 🚀 4. Expansion Guide (시스템 확장 방법)

본 프로젝트는 **Open-Closed Principle (OCP)**을 준수하여 설계되었습니다.

1. **새로운 차량 추가**: `Car` 클래스를 상속받는 새로운 헤더 파일을 만들고, `WorldManager::GenerateShop`에 타입만 등록하면 즉시 적용됩니다.
2. **맵 확장**: `WorldData.h`의 `CITIES` 리스트에 새 좌표를 추가하는 것만으로 맵 크기나 도시 배치를 변경할 수 있습니다.
3. **아이템 효과 변경**: `Item.h`의 `ItemType`과 `WorldManager::UseItem`의 switch-case 문을 수정하여 새로운 소모품 기능을 추가할 수 있습니다.

---

## ⌨️ 5. Control Reference (상세 조작 가이드)

| Key | Mode | Action |
| :--- | :--- | :--- |
| **W, A, S, D** | Field | 상/하/좌/우 이동 (에너지 및 내구도 소모) |
| **1** | City | 화물 수송 미션 수락 (목적지 및 보상 확인) |
| **2** | Any | 휴식 (에너지 100% 회복, 날짜 +1, 상점 목록 갱신) |
| **3** | Any | 가라지 진입 (보유한 차량 중 운전할 차량 선택) |
| **4** | City | 상점 진입 (차량 구매 및 소모품 보충) |
| **5** | Any | 인벤토리 열기 (연료 보충, 긴급 수리 키트 사용) |
| **6** | City | 전문 수리소 (자금을 소모하여 차량 내구도 100% 복구) |
| **0** | Any | 게임 저장 및 종료 |

---

*Copyright © 2024 PolyDrive Team. Built with Passion for C++.*
