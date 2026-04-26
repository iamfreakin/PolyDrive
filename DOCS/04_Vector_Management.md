# 04. 벡터와 메모리 관리 (WorldManager.cpp)

객체들을 저장하고 관리하는 **컨테이너**와 **메모리 관리**에 대해 학습합니다.

## 1. std::vector와 포인터 저장
```cpp
// WorldManager.h
std::vector<Car*> garage; 
```
- **왜 포인터(`Car*`)를 저장하는가?**: 다형성을 구현하기 위해서입니다. 그냥 `vector<Car>`라고 선언하면 자식 클래스들의 고유 정보가 잘려나가는 **슬라이싱(Slicing)** 현상이 발생합니다.

## 2. 동적 할당과 소유권
```cpp
// WorldManager.cpp (Travel 중 내구도 소진 시)
if (currentCar->GetDurability() <= 0) {
    auto it = std::find(garage.begin(), garage.end(), currentCar);
    if (it != garage.end()) garage.erase(it); // 리스트에서 제거
    delete currentCar; // 실제 메모리 해제
}
```
- `new`로 생성한 객체는 반드시 `delete`로 지워주어야 합니다. 특히 `erase`는 리스트에서 주소값만 지울 뿐, 실제 힙 메모리의 객체는 지우지 않으므로 직접 `delete`를 호출해야 합니다.

## 3. 소멸자에서의 일괄 해제
```cpp
// WorldManager.cpp
WorldManager::~WorldManager() {
    for (auto car : garage) delete car;
    for (auto car : shopList) delete car;
}
```
- 프로그램이 종료될 때, 매니저가 관리하던 모든 동적 할당 객체들을 순회하며 `delete` 해줍니다. 이것이 **RAII(Resource Acquisition Is Initialization)** 패턴의 기초적인 형태입니다.
