# 05. 게임 루프와 시스템 협력 (Main.cpp)

개별 클래스들이 어떻게 모여 하나의 완성된 게임으로 작동하는지 설명합니다.

## 1. 메인 루프 구조
```cpp
// Main.cpp
while (true) {
    ui.DrawGame(wm); // 1. 상태 출력
    ui.DrawMenu();    // 2. 메뉴 출력
    int choice = ...; // 3. 사용자 입력
    
    if (choice == 1) { // 4. 로직 수행
        ui.DrawMainContent(wm, 1);
        // ... wm.Travel() 호출 등
    }
}
```
- 게임은 무한 루프 내에서 **입력 -> 로직 처리 -> 출력**의 과정을 반복합니다.

## 2. 매니저 간의 역할 분담 (Separation of Concerns)
- **WorldManager**: 게임의 데이터와 비즈니스 로직(돈 계산, 이동, 상점 생성)을 담당합니다.
- **UIManager**: 화면을 그리고 사용자에게 정보를 보여주는 인터페이스를 담당합니다.
- 이렇게 역할을 분리하면 코드를 유지보수하기 훨씬 쉬워집니다.

## 3. 다형성의 실제 활용
```cpp
// UIManager.cpp
for (auto car : wm.GetShopList()) {
    car->ShowSpec(); // 어떤 차든 상관없이 정보를 출력!
}
```
- UI 매니저는 리스트에 담긴 객체가 무엇인지 구체적으로 몰라도 됩니다. 부모 클래스인 `Car`의 인터페이스(`ShowSpec`)만 알고 있으면 모든 종류의 차량을 화면에 그릴 수 있습니다.
