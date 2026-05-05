# 🖥️ [TECHNICAL SPEC] UI/UX & RENDERING ENGINE
## 1. 렌더링 철학 (Rendering Philosophy)
PolyDrive는 텍스트 기반 콘솔 환경임에도 불구하고, 현대적인 게임의 사용자 경험(UX)을 제공하기 위해 **'상태 기반 부분 갱신(State-based Partial Update)'** 렌더링 방식을 채택합니다.

---

## 2. 렌더링 엔진 상세 (The Rendering Engine)

### 2.1 ANSI Escape Code 활용
- **Color Styling**: 16진수 색상 코드가 아닌 ANSI 표준 코드를 사용하여 시스템 리소스 소모 없이 시각적 가독성을 확보합니다.
  - `RED`: 위험 상태 (내구도 저하, 자금 부족)
  - `GREEN`: 긍정 상태 (미션 완료, 자산 풍부)
  - `YELLOW`: 경고 및 중요 알림 (최종 미션 트리거)
  - `CYAN/MAGENTA`: UI 구분 및 강조

### 2.2 Flicker-Free 기술
- **콘솔 커서 제어**: `system("cls")`는 화면 전체를 지우고 다시 그려 심한 깜빡임을 유발합니다. PolyDrive는 Windows API의 `SetConsoleCursorPosition`을 사용하여 커서를 `(0, 0)`으로 이동시킨 후 데이터를 덮어씌우는 방식으로 부드러운 화면 전환을 구현합니다.
- **화면 구성 (Layout)**:
  - **Header**: 게임 제목 및 버전 정보.
  - **Status Bar**: 현재 자금, 에너지, 날짜, 활성 미션 요약.
  - **Main Content**: 모드에 따른 동적 콘텐츠 (Map, Shop, Garage 등).
  - **Log System**: 최근 발생한 이벤트를 하단에 순차적으로 노출.

---

## 3. 사용자 조작 인터페이스 (UX Design)

### 3.1 Context-Sensitive Input
- 입력은 `_getch()`를 통해 비동기적으로 처리되어 엔터 키 없이 즉각적인 반응성을 보장합니다.
- **필드 모드**: `WASD` 이동에 집중된 조작.
- **도시/메뉴 모드**: 숫자 키(`1~6`)를 통한 기능 선택 중심.

### 3.2 Visual Feedback
- 이동 시마다 로그 창에 "Moving..." 메시지 출력.
- 도시 도착 시 색상 반전을 통한 시각적 알림.
- 고립 상황 시 화면 상단에 굵은 빨간색 텍스트로 경고 표시.

---

## 4. 인터페이스 사양 (UI Layout Map)

| 영역 | 행 위치 | 주요 정보 |
| :--- | :--- | :--- |
| **Header** | 1-2 | 프로젝트 타이틀 |
| **Status** | 4-6 | Money, Energy, Day, Current Mission |
| **Main** | 8-28 | World Map (40x20 View) or Menu Content |
| **Log** | 30-32 | 최근 3개 메시지 및 입력 프롬프트 |

---
*Last Updated: 2024-05-06 | PolyDrive Development Team*
