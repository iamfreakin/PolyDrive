# 03. 상속과 다형성의 실재 (Vehicles/*.h)

상속을 통해 어떻게 코드를 재사용하고, 다형성을 구현하는지 학습합니다.

## 1. 상속 (Inheritance)
```cpp
// Bus.h
class Bus : public Car { 
public:
    Bus(std::string n, float speed, float eff, int dur, int p)
        : Car(n, speed, eff, dur, p) {} // 부모 생성자 호출
};
```
- `Bus`는 `Car`의 모든 특징을 물려받습니다.
- 부모 클래스의 생성자를 호출하기 위해 **멤버 초기화 리스트**를 사용하여 필수 데이터를 전달합니다.

## 2. 오버라이딩 (Overriding)
```cpp
// SportsCar.h
void ShowSpec() const override {
    std::cout << "[SportsCar] " << name;
    Car::ShowSpec(); // 부모의 공통 출력 로직 재사용
}
```
- `override` 키워드: 부모의 함수를 정확히 재정의했는지 컴파일러가 체크해줍니다.
- 부모의 기능을 호출(`Car::ShowSpec()`)하면서 자식만의 고유한 내용(`[SportsCar]`)을 덧붙일 수 있습니다.

## 3. 다형성 (Polymorphism)의 마법
```cpp
Car* myVehicle = new Bus("Tayo", ...);
myVehicle->ShowSpec(); // [Bus] Tayo... 가 실행됨 (동적 바인딩)
```
- 컴파일러는 `myVehicle`이 `Car*` 타입인 것만 알지만, 실행 시점에 실제 객체가 `Bus`임을 찾아내어 자식의 함수를 호출합니다. 이것이 OOP의 꽃인 **동적 바인딩(Dynamic Binding)**입니다.
