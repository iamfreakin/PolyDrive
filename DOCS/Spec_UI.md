# Specification: UI & Rendering

## 1. 화면 구성
- **Header**: 현재 날짜, 자산, 에너지 정보.
- **Main Content**: `Map`, `Cargo`, `Garage`, `Shop`, `Inventory` 모드에 따른 가변 출력.
- **Log System**: 최근 발생한 이벤트를 하단에 고정 출력하여 사용자 피드백 제공.

## 2. 렌더링 최적화 (TODO)
현재 `system("cls")` 기반의 전체 렌더링 방식을 사용 중이나, 다음 단계로 최적화가 필요합니다.
- **목표**: `SetConsoleCursorPosition`을 활용하여 변경된 텍스트 영역만 갱신.
- **기대 효과**: 이동 시 화면 깜빡임 제거 및 반응 속도 향상.
