# 03. 상속과 다형성의 실재 (Vehicles/*.h)

상속을 통해 어떻게 코드를 재사용하고, 다형성을 구현하는지 학습합니다.

## 1. 상속 (Inheritance)
```cpp
class Bus : public Car { ... };
```
- `Bus`는 `Car`의 모든 특징을 물려받습니다.
- 부모 클래스의 생성자를 호출하기 위해 `: Car(n, speed, eff, dur, p)`와 같이 **멤버 초기화 리스트**를 사용합니다.

## 2. 오버라이딩 (Overriding)
부모가 물려준 함수를 자식의 입맛에 맞게 재정의하는 것입니다.
```cpp
void ShowSpec() const override {
    std::cout << "[Bus] " << name;
    Car::ShowSpec(); // 부모의 기능을 호출하여 중복 제거
}
```
- `override` 키워드: "이 함수는 부모의 함수를 재정의한 거야!"라고 명시하는 역할을 하며, 오타 등을 방지해줍니다.

## 3. 다형성 (Polymorphism)의 마법
다형성이란 "하나의 모습이 여러 가지 형태를 띠는 것"입니다.

```cpp
Car* myVehicle = new Bus("Tayo", ...);
myVehicle->ShowSpec(); // [Bus] Tayo... 출력

myVehicle = new SportsCar("Flash", ...);
myVehicle->ShowSpec(); // [SportsCar] Flash... 출력
```

컴퓨터는 `myVehicle`이 그냥 `Car*` 타입인 것만 알지만, 실행 시점에(Runtime) 실제 객체가 무엇인지 찾아가서 그에 맞는 함수를 호출합니다. 이것이 OOP의 꽃인 **동적 바인딩(Dynamic Binding)**입니다.
