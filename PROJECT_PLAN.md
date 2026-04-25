# Car Polymorphism Simulator (PolyDrive)

C++의 상속과 다형성을 활용하여 다양한 차량의 주행 특성을 시뮬레이션하는 콘솔 기반 어플리케이션입니다.

## 1. 핵심 목표 (Core Objectives)
- **상속과 다형성 증명**: 동일한 `Move()` 명령이 차종에 따라 다르게 계산되는 동적 바인딩(Dynamic Binding) 구현.
- **시스템 모듈화**: 헤더(.h)와 소스(.cpp) 파일을 분리하여 엔진 아키텍처의 기초 확립.
- **데이터 관리**: `std::vector`를 사용해 차량 목록과 스테이지 정보를 효율적으로 관리.

## 2. 클래스 아키텍처 (Class Architecture)

### 부모 클래스: `Car` (추상 클래스)
- **속성 (protected)**: `name`, `baseSpeed`, `totalDistance`, `totalTime`.
- **기능 (virtual)**:
    - `virtual float Move(float distance) = 0;`: 거리당 소요 시간을 계산하는 순수 가상 함수.
    - `virtual void Display() const;`: 차량 정보를 출력하는 가상 함수.
    - `virtual ~Car();`: 메모리 누수 방지를 위한 가상 소멸자.

### 자식 클래스 (Specialization)
- **Bus**: 이동 시 반드시 '정류장 정차 시간(0.5h)'이 추가됨.
- **SportsCar**: 고속 주행 특화, 동일 거리 대비 시간 단축 계산식 적용.
- **Truck**: 속도는 느리지만 안정적인 주행 데이터 처리.

## 3. 월드 및 데이터 시스템 (World Data)

### 도시 간 거리 데이터
| 출발지 | 목적지 | 거리 (km) |
| :--- | :--- | :--- |
| 광주 | 대전 | 170 |
| 광주 | 부산 | 260 |
| 대전 | 서울 | 160 |
| 대전 | 부산 | 200 |
| 부산 | 서울 | 390 |
| 부산 | 강릉 | 300 |
| 서울 | 강릉 | 220 |
| 서울 | 광주 | 290 |

## 4. UI 레이아웃 (2-Layer Split Screen)

### 좌측: Navigation Layer (선택 및 로그)
- 현재 위치 표시 및 이동 가능한 목적지 리스트.
- 차량 선택 메뉴 및 주행 결과 로그.

### 우측: Dashboard Layer (실시간 상태)
- **Total Distance**: 누적 주행 거리 (km).
- **Total Time**: 누적 주행 시간 (hours).
- **Current Vehicle**: 현재 탑승 중인 차량의 상세 스펙.

## 5. 기술적 강조 포인트 (Technical Highlights)
1. **동적 바인딩 (Dynamic Binding)**: `vtable`을 통한 런타임 다형성 구현 설명.
2. **메모리 안전성**: 가상 소멸자를 통한 포인터 객체 삭제 시 안정성 확보.
3. **캡슐화 (Encapsulation)**: 멤버 변수 보호 및 인터페이스를 통한 데이터 조작.

---
**Summary**: 이 프로젝트는 사용자가 "어떤 차로 어디를 갈지" 결정하는 과정에서 발생하는 데이터 변화를 상속과 다형성 기술로 처리하는 시뮬레이터입니다.
