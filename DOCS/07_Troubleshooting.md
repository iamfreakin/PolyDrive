# 07. Troubleshooting Log

프로젝트 개발 과정에서 발생한 기술적 문제와 C++ 언어 특성에 따른 실수하기 쉬운 포인트들을 정리합니다.

---

### TS-01: 상속 구조에서의 메모리 누수 (Virtual Destructor)

**Issue**
- `Car` 클래스를 상속받은 `Bus`, `Sedan` 등의 객체를 `std::vector<Car*>`에 담아 관리할 때, 객체를 `delete` 해도 자식 클래스의 소멸자가 호출되지 않는 문제.

**Analysis**
- C++에서 기본 클래스(Base Class) 포인터로 파생 클래스(Derived Class) 객체를 가리킬 때, 소멸자가 `virtual`로 선언되어 있지 않으면 컴파일러는 포인터의 타입인 `Car` 소멸자만 호출하고 작업을 끝냄.
- 이로 인해 자식 클래스에서 별도로 할당한 자원이 있거나 특정 해제 로직이 있을 경우 메모리 누수가 발생함.

**Solution**
- 부모 클래스인 `Car` 클래스의 소멸자에 `virtual` 키워드를 추가함.

```cpp
// Car.h
class Car {
public:
    virtual ~Car() {} // 가상 소멸자 선언으로 자식 소멸자 호출 보장
};
```

**Takeaway**
- 다형성을 사용하는 상속 구조에서는 반드시 부모 클래스의 소멸자를 `virtual`로 선언해야 안전한 메모리 해제가 가능하다.

---

### TS-02: 객체 파괴 후 댕글링 포인터 (Dangling Pointer)

**Issue**
- 차량 내구도가 0이 되어 `delete currentCar;`를 실행한 후, 프로그램이 다음 루프에서 충돌(Crash)하거나 잘못된 메모리에 접근하는 현상.

**Analysis**
- `delete`는 메모리를 해제할 뿐, 해당 포인터 변수가 들고 있는 주소값을 지우지는 않음.
- 해제된 주소를 여전히 가리키고 있는 포인터를 '댕글링 포인터'라고 하며, 이를 통해 멤버 함수에 접근하면 정의되지 않은 동작(Undefined Behavior)이 발생함.

**Solution**
- `delete` 직후 포인터를 `nullptr`로 초기화하고, 사용 전 반드시 유효성 검사를 수행함.

```cpp
// WorldManager.cpp
if (currentCar->GetDurability() <= 0) {
    delete currentCar;
    currentCar = nullptr; // 안전을 위해 null 처리
}

// 사용 시 검사
if (currentCar != nullptr) {
    currentCar->Move(distance);
}
```

**Takeaway**
- 객체의 생명주기가 끝난 포인터는 즉시 `nullptr`로 만들어 "비어 있음"을 명시적으로 표현해야 한다.

---

### TS-03: 포인터 소유권 이전 및 중복 삭제 (Double Free)

**Issue**
- 상점에서 차량을 구매할 때 `shopList`와 `garage` 양쪽에서 동일한 포인터를 참조하다가, 프로그램 종료 시 두 번 삭제를 시도하여 런타임 에러가 발생하는 위험.

**Analysis**
- `WorldManager`는 상점 목록(`shopList`)과 보유 목록(`garage`)을 각각 관리함.
- 구매 시 `shopList`에서 `garage`로 주소만 복사하고 `shopList`에서 제거하지 않으면, 나중에 `WorldManager` 소멸자에서 같은 주소를 두 번 `delete` 하게 됨.

**Solution**
- 포인터를 옮길 때 한쪽 목록에서 완전히 제거(Erase)하여 소유권을 명확히 이전함.

```cpp
// WorldManager.cpp
bool WorldManager::BuyCar(int shopIdx, std::string& outMsg) {
    Car* target = shopList[shopIdx];
    garage.push_back(target); // 차고로 추가
    shopList.erase(shopList.begin() + shopIdx); // 상점 목록에서 제거 (소유권 이전)
    return true;
}
```

**Takeaway**
- 생 포인터(Raw Pointer)를 관리할 때는 해당 객체를 "누가 관리하고 해제할 것인가"에 대한 명확한 규칙이 필요하다.
