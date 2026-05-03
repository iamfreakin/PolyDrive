# 08. 현대적 메모리 관리 (Smart Pointers)

이 프로젝트는 현대적 C++(Modern C++)의 핵심인 **스마트 포인터**를 사용하여 메모리를 안전하게 관리합니다. 기존의 `new`와 `delete`를 직접 사용하는 방식에서 벗어나, 객체의 소유권을 명확히 하고 메모리 누수를 원천적으로 차단하는 방법을 배웁니다.

---

## 1. std::unique_ptr를 통한 소유권 관리

`std::unique_ptr`는 특정 객체의 **독점적인 소유권**을 갖는 스마트 포인터입니다.

### 변경 전 (Raw Pointer)
```cpp
// WorldManager.h
std::vector<Car*> garage;

// WorldManager.cpp (소멸자에서 직접 삭제 필요)
WorldManager::~WorldManager() {
    for (auto car : garage) delete car;
}
```

### 변경 후 (Unique Pointer)
```cpp
// WorldManager.h
std::vector<std::unique_ptr<Car>> garage;

// WorldManager.cpp (소멸자가 비어있어도 자동 삭제됨)
WorldManager::~WorldManager() { }
```

**교육 포인트:**
- **RAII (Resource Acquisition Is Initialization):** 객체가 범위를 벗어나거나 벡터에서 제거될 때 메모리가 자동으로 해제됩니다.
- **안전성:** 실수로 `delete`를 누락하여 발생하는 메모리 누수(Memory Leak)를 방지합니다.

---

## 2. 소유권 이전 (Ownership Transfer)

`unique_ptr`는 복사할 수 없으며, 오직 **이동(Move)**만 가능합니다. 이를 통해 객체의 주인이 누구인지 명확히 합니다.

### 차량 구매 로직 (`std::move`)
```cpp
bool WorldManager::BuyCar(int shopIdx, std::string& outMsg) {
    // shopList[shopIdx]에 있던 소유권을 garage로 완전히 넘김
    garage.push_back(std::move(shopList[shopIdx]));
    shopList.erase(shopList.begin() + shopIdx);
    return true;
}
```

**교육 포인트:**
- `std::move`를 사용하면 포인터의 제어권이 통째로 옮겨갑니다. 이전 주인(`shopList`)은 더 이상 해당 객체를 가리키지 않게 되어 '이중 해제(Double Free)' 위험이 사라집니다.

---

## 3. 소유 vs 참조 (Ownership vs Observation)

모든 곳에서 스마트 포인터를 쓸 필요는 없습니다. 객체를 소유하지 않고 단순히 **참조**만 할 때는 생포인터(Raw Pointer)나 참조자를 쓰는 것이 권장됩니다.

### 현재 운행 차량 선택 (`get()`)
```cpp
// 소유는 garage가 하고, currentCar는 바라보기만 함
void WorldManager::SelectCar(int garageIdx) {
    currentCar = garage[garageIdx].get(); // .get()으로 주소값만 전달
}
```

**교육 포인트:**
- `currentCar`는 차량이 파괴될 때 함께 사라지는 것이 아니므로 `unique_ptr`가 아닌 `Car*`를 사용합니다. 
- 이를 통해 **"누가 이 객체의 생명주기를 책임지는가?"**에 대한 설계 의도를 명확히 전달할 수 있습니다.

---

## 4. 현대적 객체 생성 (std::make_unique)

`new` 키워드를 직접 쓰는 대신 `std::make_unique`를 사용합니다.

```cpp
auto newCar = std::make_unique<SportsCar>("Ferrari", 140.0f, 6.0f, 3, 5000);
```

**교육 포인트:**
- 예외 상황에서도 메모리 안전성을 보장하며, 가독성이 좋아집니다.

---

이러한 변화를 통해 PolyDrive 프로젝트는 더 견고하고 전문적인 코드 베이스를 갖추게 되었습니다. 스마트 포인터는 실무 C++ 개발에서 선택이 아닌 필수입니다.
