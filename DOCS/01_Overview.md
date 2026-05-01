# 01. PolyDrive 전체 구조 및 OOP 설계

이 문서는 **PolyDrive** 게임의 전체적인 설계 구조와 객체 지향 프로그래밍(OOP)이 왜 필요한지에 대해 설명합니다.

## 1. 프로젝트 설계 목표
- **상속(Inheritance)**을 통해 중복 코드를 제거합니다.
- **다형성(Polymorphism)**을 통해 하나의 인터페이스로 다양한 차량을 제어합니다.
- **캡슐화(Encapsulation)**를 통해 데이터와 로직을 매니저 클래스에 안전하게 보관합니다.

---

## 2. 클래스 다이어그램 (Class Diagram)
객체 간의 관계를 한눈에 보여주는 설계도입니다.

```mermaid
classDiagram
    class Car {
        <<Abstract>>
        #string name
        #float baseSpeed
        #float efficiency
        #int durability
        #int price
        +Move(float dist) float
        +ShowSpec() void const
        +~Car()*
    }
    class Bus { +ShowSpec() void const }
    class SportsCar { +ShowSpec() void const }
    class Truck { +ShowSpec() void const }
    class Sedan { +ShowSpec() void const }

    class City {
        -string name
        -int x, y
        -vector~Route~ routes
        +AddRoute(City* dest, float dist, int reward) void
        +GetName() string
    }

    class Item {
        <<struct>>
        +ItemType type
        +string name
        +int price
        +float effectValue
    }

    class WorldManager {
        -int money
        -int energy
        -int day
        -City* currentCity
        -vector~City*~ allCities
        -Car* currentCar
        -vector~unique_ptr~Car~~ garage
        -vector~unique_ptr~Car~~ shopList
        -vector~Item~ inventory
        -vector~Item~ itemShopList
        +GenerateShop() void
        +BuyItem(int idx, string& outMsg) bool
        +UseItem(int idx, string& outMsg) bool
        +TowingService(string& outMsg) void
    }

    Car <|-- Bus : 상속(Is-A)
    Car <|-- SportsCar
    Car <|-- Truck
    Car <|-- Sedan
    WorldManager o-- Car : 포함(Has-A)
    WorldManager o-- City : 관리
    WorldManager o-- Item : 소유(Inventory)
    City "1" *-- "many" City : 연결(Graph)
```

### 핵심 포인트:
- **Is-A 관계**: "Bus는 Car이다." (상속)
- **Has-A 관계**: "WorldManager는 Car(들)과 Item(들)을 가지고 있다." (포함/인벤토리)
- **Graph 구조**: 도시(`City`)들은 서로를 참조하며 연결망을 형성합니다.

---

## 3. 게임 플로우차트 (Flowchart)
사용자의 선택에 따라 로직이 어떻게 흐르는지 보여주는 순서도입니다.

```mermaid
graph TD
    Start([게임 시작]) --> Init[초기화: 5000G, 스타터 세단 지급, 도시 그래프 구축]
    Init --> Loop{메인 루프}
    
    Loop --> UI[대시보드 출력: 맵, 상태창, 로그]
    UI --> Input{사용자 동작 선택}
    
    Input -- WASD 이동 --> Move[이동 처리: 에너지 -N, 차량 HP -1%]
    Move --> Hazard{에너지/HP 고갈?}
    Hazard -- 예 --> Tow[긴급 견인: 광주 이동, -1000G] --> Loop
    Hazard -- 아니오 --> Loop

    Input -- 1. 화물운송 --> Travel[이동 로직: 에너지 계산 & HP -15%] --> Loop
    Input -- 2. 휴식 --> Refill[날짜 증가, 에너지 100%, 상점 갱신] --> Loop
    Input -- 4. 상점 --> Shop[차량/아이템 구매] --> Loop
    Input -- 5. 인벤토리 --> Use[아이템 사용: 에너지/HP 회복] --> Loop
```

---

## 5. 단계별 학습 가이드
이 프로젝트를 깊이 있게 이해하려면 아래 순서대로 문서를 읽어보세요.

1. **[02. Car Class](02_Car_Class.md)**: 모든 차량의 근간이 되는 추상 기반 클래스 설계
2. **[03. Inheritance](03_Inheritance.md)**: 자식 클래스에서 부모의 기능을 확장하고 재정의하는 법
3. **[04. Vector Management](04_Vector_Management.md)**: 동적 할당된 객체들을 안전하게 관리하고 해제하는 기술
4. **[05. Game Loop](05_Game_Loop.md)**: 매니저 클래스들이 협력하여 게임을 구동하는 원리
5. **[06. Shop System](06_Shop_System.md)**: **상속과 다형성의 정점.** 상점에서 무작위 객체가 생성되고 관리되는 과정
6. **[07. Troubleshooting](07_Troubleshooting.md)**: **실전 문제 해결.** 개발 중 겪은 C++ 메모리 및 설계 이슈 정리
7. **[08. Graph System](08_Graph_System.md)**: **심화 설계.** 문자열 기반 시스템을 객체 지향적 그래프 구조로 리팩토링하는 법
8. **[09. Item and Condition](09_Item_and_Condition.md)**: **전략적 확장.** HP 시스템과 인벤토리를 통한 리스크 관리 설계

---

## 4. 왜 OOP인가?
(이하 생략)
