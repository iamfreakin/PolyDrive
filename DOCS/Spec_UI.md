# 🖥️ UI Specification: Rendering & Interface

PolyDrive는 Windows 콘솔 환경에서 ANSI 이스케이프 코드를 활용한 고성능 렌더링 엔진을 사용합니다.

## 1. Rendering Architecture

### 1.1 Optimized Buffering
- **`UIManager::Render()`**: 화면 전체를 한 번에 출력하여 깜빡임을 최소화합니다.
- **`std::ostringstream`**: 모든 출력 내용을 메모리 버퍼에 담은 뒤 `std::cout`으로 한 번에 밀어넣습니다.
- **NOMINMAX**: Windows API와의 충돌을 방지하기 위해 사용됩니다.

### 1.2 Viewport & Layout
- **Resolution**: 가로 200칸, 세로 45칸 (`mode con: cols=200 lines=45`).
- **Split-View**:
  - **Left (50 cols)**: World View (21x21 Camera Scroll).
  - **Right (150 cols)**: Dashboard (Status, Mission, Log, Details).

## 2. Key UI Functions

### `void Render(const WorldManager& wm, int mode)`
- 전체 화면 렌더링의 핵심 함수. 
- 좌우 패널을 결합하고, 입력 커서를 특정 위치(`SetCursor(4, 42)`)에 고정하여 UI 밀림을 방지합니다.

### `int GetPlainLength(const std::string& s)`
- ANSI 색상 코드가 포함된 문자열에서 **실제 출력되는 문자의 너비**를 계산합니다.
- 멀티바이트(한글) 문자를 인식하여 2칸으로 계산하는 로직을 포함합니다.

### `std::string GetGaugeBar(float value, float max, int width, std::string color)`
- 수치 데이터를 시각적인 막대 그래프(`[■■■■     ]`)로 변환합니다.
- `Energy`와 `Car Condition` 표시용으로 사용됩니다.

## 3. Visual System

### 3.1 Color Palette (ANSI)
- `GREEN + BOLD`: 플레이어(@), 로그 제목
- `CYAN + BOLD`: 도시(C), 위치 정보
- `YELLOW + BOLD`: 에너지, 미션 물품, 내비게이션 화살표
- `RED`: 차량 파손 경고, 퇴장 메뉴
- `GRAY`: 일반 지형(.), 구분선, 비활성 정보

### 3.2 Symbols
- `@`: Player (Current Position)
- `C`: City (Visit for mission/shop)
- `.`: Road / Terrain
- `Nav`: 목적지 방향(N/S/E/W) 및 거리(km) 표시
