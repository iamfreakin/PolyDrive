# 08. Graph-Based City System

이 문서는 단순 문자열 기반의 위치 관리 시스템에서 **객체 지향적인 그래프(Graph) 구조**로 전환된 설계에 대해 설명합니다.

## 1. 설계 배경
기존 시스템에서는 도시가 단순히 `string`으로 관리되었습니다. 이는 다음과 같은 한계가 있었습니다.
- **데이터 무결성**: 도시 이름 오타 발생 시 런타임 에러 추적이 어려움.
- **확장성 부족**: 각 도시의 좌표, 특수 상점, 인카운터 확률 등 상세 정보를 추가하기 어려움.
- **탐색의 한계**: 인접한 도시를 찾거나 경로를 계산하는 알고리즘(Dijkstra 등) 적용이 불가능함.

이를 해결하기 위해 **Node(City)**와 **Edge(Route)** 중심의 그래프 구조로 리팩토링되었습니다.

---

## 2. 핵심 클래스 구조

### 2.1 City (Node)
각 도시는 고유한 객체로 존재하며, 자신의 위치 좌표와 연결된 경로 목록을 직접 보유합니다.
```cpp
class City {
private:
    std::string name;
    int x, y; // 격자 이동 시스템을 위한 좌표
    std::vector<Route> routes; // 이 도시에서 갈 수 있는 경로들
};
```

### 2.2 Route (Edge)
경로는 단순한 데이터 묶음이 아니라, 목적지 `City` 객체를 직접 가리키는 포인터를 포함합니다.
```cpp
struct Route {
    City* destination; // 목적지 객체 포인터
    float distance;
    int baseReward;
};
```

---

## 3. WorldManager에서의 그래프 구축
`WorldManager`는 게임 시작 시 `WorldData`의 초기화 리스트를 바탕으로 동적으로 그래프를 생성합니다.

1.  **객체 생성**: `WorldData::CITIES` 정보를 읽어 `allCities` 벡터에 모든 `City` 객체를 할당합니다.
2.  **관계 연결**: `WorldData::ROUTES` 정보를 읽어 각 `City` 객체의 `AddRoute`를 호출, 서로를 포인터로 연결합니다.
3.  **메모리 관리**: 생성된 모든 `City` 객체는 `WorldManager`의 소멸자에서 안전하게 `delete`됩니다.

---

## 4. 향후 확장성 (Grid Movement)
현재 각 `City` 객체에는 `x, y` 좌표가 포함되어 있습니다. 이는 향후 다음과 같은 기능을 구현하는 기반이 됩니다.

- **격자 맵 렌더링**: `(x, y)` 좌표를 바탕으로 콘솔 화면에 도시의 위치를 표시.
- **방향키 이동**: 플레이어가 격자를 이동하다가 특정 도시의 `(x, y)`에 도달하면 해당 도시 메뉴 활성화.
- **동적 경로 계산**: 두 좌표 사이의 실제 거리를 계산하여 에너지 소모량 산출.

---

## 5. 변경된 데이터 흐름
- **기존**: `currentCity (string)` -> `WorldData::CITY_ROUTES` 맵 검색 -> 결과 출력
- **현재**: `currentCity (City*)` -> `currentCity->GetRoutes()` 직접 참조 -> 결과 출력
    - *이점: 맵 검색 과정이 생략되어 성능이 향상되고 코드가 간결해짐.*
