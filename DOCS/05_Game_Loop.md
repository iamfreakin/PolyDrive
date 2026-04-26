# 05. 게임 루프와 시스템 협력 (Main.cpp)

개별 클래스들이 어떻게 모여 하나의 완성된 게임으로 작동하는지 설명합니다.

## 1. 메인 루프 구조
게임은 꺼지기 전까지 무한히 반복되는 루프 안에서 돌아갑니다.
```cpp
// Main.cpp 핵심 루프
while (true) {
    ui.DrawGame(wm); // 1. 현재 상태(돈, 에너지 등) 출력
    ui.DrawMenu();    // 2. 사용자가 할 수 있는 행동 출력
    
    int choice;
    std::cin >> choice; // 3. 사용자 입력 받기
    
    if (choice == 1) { // 4. 선택에 따른 로직 수행
        // 이동 관련 UI 및 WorldManager 로직 호출
    }
    // ...
}
```

## 2. 매니저 간의 역할 분담 (Separation of Concerns)
- **WorldManager**: 게임의 데이터와 비즈니스 로직(돈 계산, 이동 처리, 상점 생성)을 담당합니다.
- **UIManager**: 데이터를 가공하여 화면에 예쁘게 그려주는 역할만 담당합니다.
- **Main**: 두 매니저를 연결하고 게임의 흐름을 제어하는 컨트롤러 역할을 합니다.

## 3. 다형성의 실제 활용
```cpp
// UIManager.cpp
void UIManager::DrawMainContent(const WorldManager& wm, int mode) {
    auto& shop = wm.GetShopList();
    for (int i = 0; i < shop.size(); ++i) {
        shop[i]->ShowSpec(); // 어떤 차든 상관없이 ShowSpec() 호출!
    }
}
```
- UI 매니저는 리스트에 담긴 객체가 구체적으로 `Bus`인지 `Truck`인지 따지지 않습니다. 부모인 `Car`의 인터페이스(`ShowSpec`)만 알고 있으면 다형성을 통해 모든 종류의 차량을 일관되게 화면에 그릴 수 있습니다.
