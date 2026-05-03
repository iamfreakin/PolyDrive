# Specification: Vehicle System

## 1. 클래스 구조
`Car` 클래스는 추상 기반 클래스로 설계되어 다양한 차량 타입의 공통 인터페이스를 정의합니다.

- **Base Class**: `Car`
- **Derived Classes**: `Bus`, `SportsCar`, `Truck`, `Sedan`

## 2. 다형성 인터페이스
- `virtual float Move(float distance)`: 차량 특성에 따른 이동 시간 계산 및 리소스 소모 처리.
- `virtual void ShowSpec() const`: 각 차량 특유의 스탯 출력.

## 3. 핵심 스탯 공식
- **에너지 소모**: `거리 / 연비(Efficiency)`
- **내구도(HP) 감소**:
    - 격자 이동 시: 고정 -1%
    - 도시 간 운송(Travel) 시: 고정 -15%
- **수리(Repair)**: 아이템을 통해 % 단위로 회복하며 최대 100%를 초과할 수 없음.
