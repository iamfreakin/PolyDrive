# 02. 추상 클래스 설계 (Car.h)

모든 차량의 뼈대가 되는 `Car` 클래스는 직접 객체를 만들 수 없는 **추상 클래스**로 설계되었습니다.

## 1. 멤버 변수 보호 (Protected)
```cpp
// Car.h
protected:
    std::string name;
    float baseSpeed;
    float efficiency; 
    int durability;   
    int price;
```
- `private`이 아닌 `protected`를 사용하여, 자식 클래스들이 부모의 변수에 직접 접근할 수 있도록 허용하면서도 외부로부터는 보호(캡슐화)합니다.

## 2. 순수 가상 함수와 인터페이스
```cpp
// Car.h
virtual ~Car() {} // 가상 소멸자 필수!
virtual float Move(float distance) { ... }
virtual void ShowSpec() const = 0; // 순수 가상 함수
```
- **가상 소멸자**: 부모 포인터로 자식 객체를 삭제할 때 메모리 누수를 방지하기 위해 필수적입니다.
- **순수 가상 함수 (`= 0`)**: "차량이라면 반드시 스펙을 보여줄 수 있어야 한다"는 규칙을 정의합니다. 이 함수가 있으면 `Car` 객체를 직접 생성할 수 없게 되어 설계의 안정성이 높아집니다.
