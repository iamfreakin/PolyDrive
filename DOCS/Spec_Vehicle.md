# 🏎️ [TECHNICAL SPEC] VEHICLE PHYSICS & DYNAMICS
## 1. 차량 물리 모델 (Vehicle Physics Model)
PolyDrive의 차량 시스템은 추상화된 물리 모델을 기반으로 하며, 각 차량 타입은 **속도(Speed), 효율성(Efficiency), 내구도(Condition)**의 상호작용을 통해 고유한 주행감을 제공합니다.

---

## 2. 차량 스펙 데이터 구조 (Vehicle Specs)

### 2.1 속도 (Speed)
- **정의**: 미션 수행 시 소요되는 가상의 시간 단위를 결정합니다.
- **영향**: 향후 구현될 시간 제한 미션에서 핵심 수치로 작용합니다.

### 2.2 효율성 (Efficiency)
- **정의**: 그리드 이동 한 칸당 소모되는 에너지의 역수입니다.
- **공식**: `Energy Loss = ceil(10.0 / Efficiency)`
- **전략적 가치**: 효율이 높은 `Truck`은 장거리 운송에 유리하며, 효율이 낮은 `SportsCar`는 단거리 쾌속 이동에 최적화됩니다.

### 2.3 내구도 (Condition/HP)
- **감소 로직**: 모든 차량은 이동 한 칸당 `1.0%`의 고정 내구도 손실을 입습니다.
- **파괴 상태**: 내구도가 `0%`에 도달하면 차량은 즉시 운행 불가 상태가 되며, 견인 서비스가 강제 호출됩니다.

---

## 3. 클래스 계층 구조 (Class Hierarchy)

### 3.1 Base Class: `Car` (Abstract)
- 모든 차량의 공통 인터페이스 및 공통 속성(`name`, `baseSpeed` 등) 정의.
- `virtual Move()` 및 `virtual ShowSpec()`을 통한 다형성 보장.

### 3.2 Derived Classes (Concrete)
- **Sedan**: 표준 성능, 안정적인 범용 차량.
- **SportsCar**: 최고 속도 특화, 매우 낮은 효율 및 높은 유지비.
- **Truck**: 최고의 에너지 효율, 낮은 속도, 물류 최적화.
- **Bus**: 높은 안정성과 중간 스펙의 조화.

---

## 4. 동적 스펙 생성 (Random Generation)

상점에 등장하는 모든 차량은 생성 시점에 무작위 변수(`Random Factor`)를 할당받습니다.
- **Base Value**: 각 차량 타입의 기본값 (예: Sedan Speed 90).
- **Modifier**: `0.7x ~ 1.3x` 사이의 무작위 배율 적용.
- 이를 통해 플레이어는 동일한 모델명이라도 더 우수한 스펙의 차량을 찾기 위해 상점을 지속적으로 방문해야 하는 동기를 부여받습니다.

---
*Last Updated: 2024-05-06 | PolyDrive Development Team*
