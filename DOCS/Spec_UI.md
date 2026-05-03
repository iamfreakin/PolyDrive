# Specification: UI & Rendering

## 1. 화면 구성
- **Header**: 현재 날짜, 자산, 에너지 정보.
- **Main Content**: `Map`, `Cargo`, `Garage`, `Shop`, `Inventory` 모드에 따른 가변 출력.
- **Log System**: 최근 발생한 이벤트를 하단에 고정 출력하여 사용자 피드백 제공.

## 2. 렌더링 최적화 (완료)
- **Windows API 활용**: `SetConsoleCursorPosition`을 사용하여 화면 전체를 지우지 않고 커서 위치를 (0,0)으로 이동시켜 덮어쓰는 방식을 도입했습니다.
- **깜빡임 제거**: 불필요한 `system("cls")` 호출을 제거하여 이동 및 UI 갱신 시 발생하는 화면 깜빡임을 해결했습니다.
- **가변 콘텐츠 처리**: UI 모드(`Map`, `Shop` 등)가 변경될 때만 `system("cls")`를 호출하여 잔상을 방지하는 스마트 클리어 로직을 적용했습니다.
- **커서 숨기기**: 게임 몰입도를 위해 콘솔 커서를 숨김 처리했습니다.
