# 04. 벡터와 메모리 관리 (WorldManager.cpp)

객체들을 저장하고 관리하는 **컨테이너**와 **메모리 관리**에 대해 학습합니다.

## 1. std::vector와 포인터 저장
`vector`는 크기가 자유롭게 변하는 동적 배열입니다. 우리 게임에서는 차량들을 담는 가방 역할을 합니다.
```cpp
// WorldManager.h
std::vector<Car*> garage; 
```
- **왜 포인터(`Car*`)를 저장하는가?**: 다형성을 구현하기 위해서입니다. 그냥 `vector<Car>`라고 선언하면 자식 클래스들의 고유 정보가 잘려나가는 **슬라이싱(Slicing)** 현상이 발생합니다. 자식 객체의 주소를 담아야만 나중에 그 객체가 버스인지 트럭인지 알 수 있습니다.

## 2. 동적 할당과 소유권
- **`new`**: 힙(Heap) 메모리에 객체를 생성합니다. 이 메모리는 우리가 수동으로 지워주기 전까지 사라지지 않습니다.
- **`delete`**: `new`로 만든 객체를 메모리에서 해제합니다.

```cpp
// WorldManager.cpp (Travel 중 내구도 소진 시)
if (currentCar->GetDurability() <= 0) {
    auto it = std::find(garage.begin(), garage.end(), currentCar);
    if (it != garage.end()) garage.erase(it); // 1. 리스트(가방)에서 주소 빼기
    delete currentCar; // 2. 실제 힙 메모리에서 객체 파괴
}
```

## 3. 소멸자에서의 일괄 해제
```cpp
// WorldManager.cpp
WorldManager::~WorldManager() {
    for (auto car : garage) delete car; // 차고 비우기
    for (auto car : shopList) delete car; // 상점 매물 비우기
}
```
- 프로그램이 종료되거나 매니저 객체가 사라질 때, 관리하던 모든 동적 할당 객체들을 순회하며 `delete` 해줍니다. 이를 통해 **메모리 누수(Memory Leak)**를 방지합니다.
