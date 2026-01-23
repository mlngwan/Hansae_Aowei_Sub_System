# EPB 프로젝트 As-Is 코드 이해 문서

**작성일**: 2026-01-12  
**범위**: Aowei Proto4 Sub Ver0.01  
**목적**: 리팩토링 전 현재 코드 동작 방식을 "있는 그대로" 분석 및 문서화  
**주의**: 개선안/리팩토링 제안은 포함하지 않음

---

## 1. Boot & Runtime Flow

### 1.1 전체 흐름 (Main → While Loop)

```
┌─────────────────────────────────────────────────────────────────┐
│ Power On → Reset Handler → startup.s (어셈블리)                │
│ - Stack 초기화                                                   │
│ - BSS/DATA 섹션 초기화                                          │
│ - main() 호출                                                   │
└─────────────────┬───────────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────────────────────────────┐
│ main() [Main_Api.c / line 326]                                  │
│                                                                  │
│ 1. SubEpbInitialize() 호출                                      │
│    └─ 하드웨어 초기화 수행                                       │
│                                                                  │
│ 2. ElectronicParkBrake_initialize() 호출                        │
│    └─ Simulink 생성 코드 초기화                                │
│                                                                  │
│ 3. TaskScheduler_Initialization(TASK_5000ms) 호출              │
│    └─ 태스크 스케줄러 초기화                                    │
│    └─ APP_Init() 호출 (EB 스택)                                │
│                                                                  │
│ 4. EepromReadAll() 호출                                        │
│    └─ EEPROM에서 저장된 파라미터 읽음                          │
└─────────────────┬───────────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────────────────────────────┐
│ while(1) [Main_Api.c / line 336]                                │
│                                                                  │
│ Loop:                                                            │
│   └─ TaskScheduler_ActivateTask()                              │
│       └─ TickCount 체크 → 주기 태스크 실행                     │
│           (5ms / 10ms / 20ms)                                   │
└─────────────────────────────────────────────────────────────────┘
```

### 1.2 SubEpbInitialize() 상세 (Main_Api.c / line 311)

**호출 함수 순서**:

1. `BoardDisableInterrupts()` - 인터럽트 비활성화
2. `DisableWatchdog()` - Watchdog 비활성화
3. `MEInitialize()` - Mode Entry (클럭 설정)
4. `CGMInitialize()` - Clock Generation Module 초기화
5. `PORTInitialize()` - GPIO/Port 초기화
6. `STMInitialize()` - System Timer 초기화
7. `ADCInitialize()` - ADC 모듈 초기화
8. `SpiInitialize()` - SPI 컨트롤러 초기화
9. `CanInitialize()` - CAN 컨트롤러 초기화
10. `BoardEnableInterrupts()` - 인터럽트 재활성화

**출처**: Main_Api.c lines 311-321

### 1.3 TaskScheduler_Initialization() 상세 (Main_Api.c / line 89)

**초기화 항목**:

- `g_TaskControl.TickCount = 0` - 소프트웨어 타이머 카운터
- `g_TaskControl.Tick = SYSTEM_TICK_5MS` - 시스템 기본 틱 (5ms)
- `g_TaskControl.Flag.SystemDown = TRUE` - 초기 시스템 다운 상태
- `g_TaskControl.Flag.SystemOff = FALSE`
- `g_TaskControl.Flag.PowerOff = FALSE`
- `g_MAIN.SystemStatus = SYSTEM_STATUS_INIT` - 초기화 상태
- `g_MAIN.MtDrvStatus = MTDRV_STATUS_INIT` - 모터드라이버 초기화 상태
- `APP_Init()` 호출 - EB CAN 스택 초기화

**출처**: Main_Api.c lines 66-87

---

## 2. Task Map & Scheduling

### 2.1 태스크 실행 계층 구조

```
┌──────────────────────────────────────────────────────────────────┐
│ TickCount 증가 (타이머 ISR 또는 메인 루프 내)                   │
│ TaskScheduler_TaskCount() 호출 [line 195]                       │
└──────────────────┬───────────────────────────────────────────────┘
                   ↓
┌──────────────────────────────────────────────────────────────────┐
│ TaskScheduler_ActivateTask() [line 277]                         │
│ - TickCount % 20 == 0? → TASK_20ms 실행                        │
│ - TickCount % 10 == 0? → TASK_10ms 실행                        │
│ - TickCount % 5 == 0?  → TASK_5ms 실행                         │
└──────────────────┬───────────────────────────────────────────────┘
                   ↓
        (각 주기별 태스크 실행)
```

### 2.2 주기별 태스크 호출 맵

| 주기     | 태스크 함수            | 파일       | 호출 라인 | 주요 수행 내용                                                |
| -------- | ---------------------- | ---------- | --------- | ------------------------------------------------------------- |
| **5ms**  | `TaskScheduler_5ms()`  | Main_Api.c | 225       | 전원관리, CAN 송신, ADC체크, L9369/CANSBC 감시, 다운로드 처리 |
| **10ms** | `TaskScheduler_10ms()` | Main_Api.c | 208       | 모터 ADC 평균화, 모터 테스트/구동, 진단 작업                  |
| **20ms** | `TaskScheduler_20ms()` | Main_Api.c | 202       | Watchdog 체크, TC277 상태 체크, ECU 리셋 체크                 |

**호출 관계**:

- 5ms마다: `TaskScheduler_5ms()` 실행
- 10ms마다: `TaskScheduler_5ms()` + `TaskScheduler_10ms()` 실행
- 20ms마다: `TaskScheduler_5ms()` + `TaskScheduler_10ms()` + `TaskScheduler_20ms()` 실행

**실행 조건**:

```c
// Main_Api.c / TaskScheduler_TaskCalculation() line 136
if ((g_TaskControl.TickCount % (uint16)TASK_20ms) == 0U) {
    // 20ms, 10ms, 5ms 모두 실행
}
else if ((g_TaskControl.TickCount % (uint16)TASK_10ms) == 0U) {
    // 10ms, 5ms 실행
}
else if ((g_TaskControl.TickCount % (uint16)TASK_5ms) == 0U) {
    // 5ms만 실행
}
```

**출처**: Main_Api.c lines 136-153

---

## 3. Call Tree (TaskScheduler_5ms 기준)

### 3.1 System Down (Shutdown) 경로

```
TaskScheduler_5ms() [Main_Api.c:225]
└─ IF (g_TaskControl.Flag.SystemDown == TRUE)
   ├─ TxCanMessage()
   │   ├─ CAN_Task() [Can_Api.c:160]
   │   │   ├─ CAN_AllPolling() - CAN 메시지 수신 처리
   │   │   ├─ APP_Manage() - APP 레이어 관리
   │   │   └─ EB_Manage() - EB 스택 관리 (COM, TP, CAN, DEM, UDS)
   │   └─ Tx_Can_100ms() - 모터상태, 진단정보 송신
   ├─ CheckAdcStatus() - ADC 상태 모니터링
   ├─ CheckPortStatus() - GPIO 포트 상태 체크
   ├─ CheckCANSBCRegStatus(SPI_CANSBC_RUN) - CANSBC(TLE9461) 레지스터 체크
   │   └─ SPI 통신 수행
   ├─ PowerOff() 처리
   │   ├─ CAN 비활성화
   │   ├─ L9369 Sleep
   │   ├─ 인터럽트 비활성화
   │   └─ CANSBC Sleep
   └─ SaveParamData() - EEPROM에 파라미터 저장

[주목]
- g_TaskControl.Flag.SystemDown: IGN OFF 상태 플래그
- g_TaskControl.Flag.SystemOff: IGN OFF 유지 1초+ 플래그
- PowerOffCount > SYSTEM_POWER_OFF_TIME (4000ms/5ms=800카운트) → 실제 전원차단
```

**출처**: Main_Api.c lines 232-255

### 3.2 System Normal (정상 동작) 경로

```
TaskScheduler_5ms() [Main_Api.c:225]
└─ ELSE (g_MAIN.SystemStatus == SYSTEM_STATUS_INIT)
   │
   └─ IF (g_MAIN.SystemStatus == SYSTEM_STATUS_INIT)
      ├─ CheckWakeupStatus(WAKEUP_START) - 웨이크업 상태 확인
      ├─ CheckPortStatus()
      ├─ CheckAdcStatus()
      └─ g_MAIN.SystemStatus = TLE9461Init(FALSE) - CANSBC 초기화
   │
   └─ ELSE IF (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
      ├─ TxCanMessage()
      ├─ CheckPortStatus()
      ├─ CheckAdcStatus()
      ├─ CheckCANSBCRegStatus(SPI_CANSBC_RUN)
      └─ L9369 (모터드라이버) 초기화/상태체크
          ├─ IF (g_TC277.fMainfault == TRUE)
          │  ├─ IF (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
          │  │  └─ CheckL9369RegStatus(SPI_L9369_RUN)
          │  └─ ELSE
          │     └─ g_MAIN.MtDrvStatus = L9369Init(FALSE)
          └─ ELSE
             └─ g_MAIN.MtDrvStatus = MTDRV_STATUS_INIT
```

**출처**: Main_Api.c lines 259-275

### 3.3 TaskScheduler_10ms 경로

```
TaskScheduler_10ms() [Main_Api.c:208]
└─ IF (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
   ├─ IF (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
   │  ├─ L9369AdcAvr() - 모터 ADC 값 평균화 (SPI 읽기)
   │  ├─ EpbMotorTestLogic() - 모터 테스트 로직
   │  │   └─ 특정 조건에서 모터 테스트 실행
   │  └─ EpbMotorDriveProcess() [Spi_L9369_Api.c:1100]
   │      └─ 핵심: 모터 제어 로직 실행
   │         ├─ SPI 명령 생성 및 전송
   │         ├─ ADC 값 읽기
   │         └─ ElectronicParkBrake_step() 호출 (Simulink 로직)
   └─ Diagnostic_Task_10ms() [FailSafe_Api.c:33] - 10ms 진단 작업
```

**출처**: Main_Api.c lines 208-222

### 3.4 TaskScheduler_20ms 경로

```
TaskScheduler_20ms() [Main_Api.c:202]
└─ IF (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
   ├─ IF (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
   │  └─ CheckL9369Watchdog() - L9369 Watchdog 주기 갱신
   ├─ CANSBCWatchdog() - CANSBC Watchdog 주기 갱신
   ├─ EcuResetCheck() - ECU 리셋 요청 확인
   └─ CheckTC277Status() - TC277 상태 체크
```

**출처**: Main_Api.c lines 202-206

---

## 4. God Objects / Global State

### 4.1 주요 전역 구조체 목록

| 구조체명                | 정의파일              | 크기(bytes) | 용도                                 | 초기화 함수                      |
| ----------------------- | --------------------- | ----------- | ------------------------------------ | -------------------------------- |
| `g_TaskControl`         | Main_Api.h            | 48+         | 태스크 스케줄링, 상태 플래그, 타이머 | TaskScheduler_Initialization()   |
| `g_MAIN`                | Main_Api.h            | 8           | 시스템 상태, 드라이버 상태           | TaskScheduler_Initialization()   |
| `g_ADC`                 | Adc_Api.h             | 16          | ADC 물리값, Raw값, 플래그            | ADCInitialize()                  |
| `g_L9369_Status`        | Spi_L9369Data_Types.h | ?           | 모터A/B 상태, 전류값, 오류플래그     | (미상, 확인필요)                 |
| `g_TC277`               | (미확인)              | ?           | TC277 센서/상태                      | (미확인)                         |
| `g_CANSBC_Status`       | Spi_TLE9461_Api.c:14  | ?           | CANSBC(CAN SBC) 상태                 | TLE9461Init()                    |
| `g_SwitchStatus`        | (미확인)              | ?           | EPB 스위치 상태                      | (미확인)                         |
| `ElectronicParkBrake_Y` | ElectronicParkBrake.h | ?           | Simulink 출력 (모터명령, 상태)       | ElectronicParkBrake_initialize() |

**출처**:

- Main_Api.c line 28: `static TaskControl_t g_TaskControl;`
- Main_Api.c line 29: `Main_t g_MAIN;`
- Adc_Api.h line 54: `typedef struct ... ADC_t;`

### 4.2 TaskControl_t 필드 상세 (Read/Write 분석)

```c
// Main_Api.h lines 11-50 정의
typedef struct {
    // 기본 타이머/제어
    uint16 TickCount;           // [READ] 모든 태스크 / [WRITE] TaskScheduler_TaskCount()
    uint16 Tick;                // [READ] 초기화만 / [WRITE] 초기화만
    uint16 TaskRun;             // [WRITE] TaskScheduler_TaskCalculation()
    uint16 TaskMax;             // [WRITE] 초기화만
    uint16 StepCount;           // [미사용, 확인필요]

    // Flag 비트필드
    Flag.Enable;                // [READ] TaskScheduler_ActivateTask() / [WRITE] TaskScheduler_ActivateTask()
    Flag.SystemDown;            // [READ] TaskScheduler_5ms() / [WRITE] SetSystemDown(), SetSystemOn()
    Flag.SystemOff;             // [READ] TaskScheduler_5ms() / [WRITE] SetSystemOff()
    Flag.PowerOff;              // [READ/WRITE] PowerOff()
    Flag.EmergencyMode;         // [미사용, 확인필요]
    Flag.SystemRunning;         // [WRITE] 초기화만
    Flag.StartDownload;         // [미사용, 확인필요]

    // 카운터
    PowerOffCount;              // [WRITE] TaskScheduler_5ms() (전원차단 카운다운)
    WakeupStatusTemp;           // [WRITE] CheckWakeupStatus()
    WakeupStatus;               // [READ/WRITE] CheckWakeupStatus(), 웨이크업 로직
    WakeupStep;                 // [WRITE] CheckWakeupStatus()

    // 성능 모니터링 (선택사항)
    TaskCount_5ms_Cnt;          // [WRITE] ExecuteTask() - 5ms 태스크 실행 횟수
    TaskCount_10ms_Cnt;         // [WRITE] ExecuteTask() - 10ms 태스크 실행 횟수
    TaskCount_20ms_Cnt;         // [WRITE] ExecuteTask() - 20ms 태스크 실행 횟수
    TaskCount_*_TimeElapsed;    // [WRITE] ExecuteTask() - 각 태스크 실행 시간
    TaskCount_*_TimeElapsed_Max;// [WRITE] ExecuteTask() - 최대 실행 시간
} TaskControl_t;
```

**Read/Write 매트릭스**:

| 필드            | TaskScheduler_5ms | TaskScheduler_10ms | TaskScheduler_20ms | TaskScheduler_ActivateTask | 초기화 | 기타 함수           |
| --------------- | ----------------- | ------------------ | ------------------ | -------------------------- | ------ | ------------------- |
| TickCount       | **R**             | -                  | -                  | **R**                      | W      | Task Count          |
| Flag.SystemDown | **R**             | -                  | -                  | -                          | W      | SetSystemDown()     |
| Flag.SystemOff  | **R**             | -                  | -                  | -                          | W      | SetSystemOff()      |
| PowerOffCount   | **RW**            | -                  | -                  | -                          | W      | -                   |
| WakeupStatus    | **RW**            | -                  | -                  | -                          | W      | CheckWakeupStatus() |

**출처**: Main_Api.h lines 11-50, Main_Api.c lines 66-87

### 4.3 Main_t 필드 분석

```c
// Main_Api.h line 48 정의
typedef struct {
    uint8 SystemStatus;         // [READ] TaskScheduler_5ms() / [WRITE] 초기화, TLE9461Init()
    uint8 MtDrvStatus;          // [READ] 모터 제어 태스크 / [WRITE] L9369Init()
    uint8 PowerOffOk;           // [READ/WRITE] PowerOff()
    uint8 Session;              // [WRITE] CheckWakeupStatus() (UDS 세션)
    uint8 L9369_InitStep;       // [WRITE] 모터드라이버 초기화 상태 추적
    uint8 TLE9461_InitStep;     // [WRITE] CANSBC 초기화 상태 추적
    unsigned int Debug : 1;     // [WRITE] 초기화 (DEBUG_MAIN_ALIVE)
} Main_t;
```

| 필드           | 용도                      | 변수값                                          |
| -------------- | ------------------------- | ----------------------------------------------- |
| `SystemStatus` | 시스템 전체 상태 관리     | INIT(0), NORMAL(1), POWER_DOWN(2), EMERGENCY(3) |
| `MtDrvStatus`  | 모터드라이버(L9369) 상태  | INIT(0), NORMAL(1)                              |
| `PowerOffOk`   | 전원차단 명령 완료 플래그 | 0/1                                             |
| `Session`      | 현재 UDS 세션             | (UDS 스택에서 설정)                             |

**출처**: Main_Api.h lines 47-54

---

## 5. Hardware Touch Points

### 5.1 직접 레지스터 접근 목록

| 레지스터            | 파일                | 함수                          | 접근타입 | 목적                 |
| ------------------- | ------------------- | ----------------------------- | -------- | -------------------- |
| `CAN_0`             | board_irq.c         | FlexCanESRClear()             | **W**    | CAN 에러 상태 클리어 |
| `CAN_0.MCR`         | board_irq.c         | FlexCanESRClear()             | RW       | Freeze/Halt 제어     |
| `CAN_0.ESR`         | board_irq.c         | FlexCanESRClear()             | W        | 에러 플래그 클리어   |
| `ME` (Mode Entry)   | Cgm_Api.c, Me_Api.c | MEInitialize()                | RW       | 클럭 모드 설정       |
| `SIUL` (Serial I/O) | board.c, Port_Api.c | PORTInitialize()              | W        | GPIO 포트 설정       |
| `ADC`               | (MCAL)              | ADCInitialize(), GetRawData() | RW       | ADC 샘플링           |

**출처**:

- board_irq.c lines 176-193: CAN ESR 클리어
- Cgm_Api.c, Me_Api.c: 클럭 설정 (파일내용 미확인)

### 5.2 SPI/MCAL 드라이버 호출

| 드라이버  | 함수명                      | 호출처                      | 목적                      |
| --------- | --------------------------- | --------------------------- | ------------------------- |
| **SPI**   | `SpiInitialize()`           | SubEpbInitialize()          | SPI 컨트롤러 초기화       |
|           | `Spi_L9369_SetDuty()`       | EpbMotorDriveProcess()      | PWM 듀티 설정 (모터)      |
|           | `L9369AdcAvr()`             | TaskScheduler_10ms()        | L9369 ADC 평균화 (SPI RX) |
|           | `L9369Init()`               | TaskScheduler_5ms()         | L9369 초기화              |
|           | `CheckL9369RegStatus()`     | TaskScheduler_5ms()         | L9369 레지스터 상태 체크  |
|           | `CheckL9369Watchdog()`      | TaskScheduler_20ms()        | L9369 Watchdog 갱신       |
| **CAN**   | `CanInitialize()`           | SubEpbInitialize()          | CAN 컨트롤러 초기화       |
|           | `CAN_AllPolling()`          | TxCanMessage() → CAN_Task() | CAN 메시지 RX 폴링        |
|           | `COM_ModeModify()`          | PowerOff(), (CAN 스택)      | CAN BUS OFF/ON 제어       |
|           | `CAN_BusOff()`              | PowerOff(), (CAN 스택)      | CAN BUS 오프              |
| **ADC**   | `ADCInitialize()`           | SubEpbInitialize()          | ADC 초기화                |
|           | `GetAdcAverage()`           | (ADC 래퍼)                  | ADC 값 읽기               |
|           | `CheckAdcStatus()`          | TaskScheduler_5ms()         | ADC 상태 체크             |
| **Timer** | `STMInitialize()`           | SubEpbInitialize()          | System Timer 초기화       |
|           | `TaskScheduler_TaskCount()` | (타이머 ISR?)               | Tick 카운트 증가          |

**출처**:

- Main_Api.c line 311: SubEpbInitialize()
- Main_Api.c line 213: TaskScheduler_10ms() 호출 항목
- Spi_L9369_Api.c line 1100: EpbMotorDriveProcess()

### 5.3 인터럽트 핸들러

| ISR 이름         | 파일            | 트리거        | 처리 내용                        |
| ---------------- | --------------- | ------------- | -------------------------------- |
| `BOARD_ISR_CAN0` | board_irq.c:159 | CAN 메일박스  | CAN_LLD_MailboxInterrupt() 호출  |
| (Timer ISR)      | (미확인)        | 시스템 타이머 | TaskScheduler_TaskCount() (추정) |
| (CAN 에러)       | board_irq.c:172 | CAN BUS OFF   | FlexCanESRClear()                |

**출처**: board_irq.c lines 159-193

---

## 6. Feature Inventory

### 6.1 구현된 기능 목록

| 기능군          | 기능명                    | 상태 | 구현파일                | 주기            |
| --------------- | ------------------------- | ---- | ----------------------- | --------------- |
| **전원관리**    | 시스템 시작 (Startup)     | ✓    | Main_Api.c              | 초기화          |
|                 | IGN OFF 감지              | ✓    | Main_Api.c              | 5ms             |
|                 | 전원차단 (Power Down)     | ✓    | Main_Api.c              | 5ms             |
|                 | EEPROM 파라미터 저장      | ✓    | Main_Api.c              | 5ms (off대기중) |
| **모터제어**    | 모터 PWM 구동             | ✓    | Spi_L9369_Api.c         | 10ms            |
|                 | 모터 ADC (전류) 모니터링  | ✓    | Spi_L9369_Api.c         | 10ms            |
|                 | 모터 테스트 로직          | ✓    | Spi_L9369_Api.c         | 10ms            |
|                 | L9369 Watchdog            | ✓    | Spi_L9369_Api.c         | 20ms            |
| **CAN통신**     | CAN 메시지 수신           | ✓    | Can_Api.c, EB스택       | 5ms (폴링)      |
|                 | CAN 메시지 송신 (EPB상태) | ✓    | Can_Api.c               | 5ms             |
|                 | CAN BUS OFF 감지          | ✓    | board_irq.c, EB스택     | ISR             |
| **진단/DEM**    | DTC 관리                  | ✓    | EB 스택, FailSafe_Api.c | 10ms            |
|                 | 모터 오버커렌트 진단      | ✓    | FailSafe_Api.c          | 10ms            |
|                 | CAN 메시지 손실 진단      | ✓    | Can_Api.c               | 5ms             |
| **UDS**         | SecurityAccess (Key 생성) | ✓    | APP_Prg.c (EB스택)      | 온디맨드        |
|                 | 진단세션 관리             | ✓    | EB 스택                 | 온디맨드        |
|                 | ECU 리셋                  | ✓    | Main_Api.c              | 5ms (체크)      |
| **Watchdog**    | WDG 초기화                | ✓    | Watchdog_Api.c          | 초기화          |
|                 | WDG 클리어                | ?    | (미확인, 정기적 호출?)  | ?               |
| **ADC모니터링** | 전원전압 (UBVR, IGN, UBB) | ✓    | Adc_Api.c               | 5ms             |
|                 | 모터 전류값 (ADC)         | ✓    | Spi_L9369_Api.c         | 10ms            |
| **Port/GPIO**   | Port 상태 체크            | ✓    | Port_Api.c              | 5ms             |
|                 | L9369/CANSBC 상태         | ✓    | Spi\_\* 함수들          | 5ms/10ms        |

### 6.2 미구현/미확인 기능

| 기능                 | 현상                                                 | 확인필요     |
| -------------------- | ---------------------------------------------------- | ------------ |
| Watchdog 클리어 주기 | WDG 초기화는 있으나, 정기적 refresh 함수 호출 미확인 | **확인필요** |
| 웨이크업 제어        | CheckWakeupStatus()는 있으나, 실제 동작 방식 불명확  | **확인필요** |
| TC277 역할           | g_TC277 구조체 존재하나, 정의/용도 불명확            | **확인필요** |
| CCP 프로토콜         | ccpDaq() 호출 있으나, 자세한 동작 미확인             | **확인필요** |
| EEPROM 저장          | SaveParamData() 호출 있으나, 저장항목 미상           | **확인필요** |

---

## 7. Dependency Graph (텍스트)

### 7.1 파일 의존성 (상향식)

```
Hardware Registers
    ↑
Peripheral Drivers (MCAL)
├─ board.c (GPIO, INTC)
├─ Adc_Api.c (ADC)
├─ Stm_Api.c (Timer)
├─ Dspi_Api.c (SPI)
├─ Can_Api.c (CAN)
└─ Watchdog_Api.c
    ↑
Device Drivers (모터/센서)
├─ Spi_L9369_Api.c (모터드라이버) ← board.c, Dspi_Api.c
├─ Spi_TC277_Api.c (센서) ← board.c, Dspi_Api.c
├─ Spi_TLE9461_Api.c (CAN SBC) ← board.c, Dspi_Api.c
└─ Spi_ExtEEPROM_Api.c (메모리) ← board.c, Dspi_Api.c
    ↑
Application Logic (ASW)
├─ ElectronicParkBrake.c (Simulink, 모터제어) ← Spi_L9369_Api.c, Adc_Api.c
├─ Logic_Api.c (앱로직 래퍼) ← ElectronicParkBrake.c
├─ Can_Api.c (CAN 통신) ← EB 스택, Spi_L9369_Api.c, Adc_Api.c
├─ Diagnostics.c (진단) ← Can_Api.c, EB 스택
├─ FailSafe_Api.c (안전) ← ElectronicParkBrake.c, Diagnostics.c
├─ Port_Api.c (포트관리) ← Adc_Api.c, Spi_L9369_Api.c
└─ Interface.c (신호 맵핑)
    ↑
Main Task Scheduler (Core)
├─ Main_Api.c ← 모든 드라이버, ASW
└─ APP_Prg.c (EB 스택 통합)
    ↑
Entry Point
└─ main() in Main_Api.c
```

**읽기 방향**: 아래 → 위 (상향 의존)  
**예시**: ElectronicParkBrake.c는 Spi_L9369_Api.c, Adc_Api.c에 의존

**출처**: 파일 include 문 분석 (Main_Api.c lines 1-27 등)

---

## 8. Hotspot 리스트 (TOP 5)

### 8.1 복잡도/리스크 분석

#### **#1: ElectronicParkBrake.c (Simulink 생성 코드)**

**파일**: `EPB/AppLogic/ElectronicParkBrake.c` (442 라인)  
**이유**:

1. **Simulink 자동생성** → 사람이 직접 수정할 수 없음
2. **복합 제어로직** 내포
   - 모터 피드백 제어 (전류 필터링, PID)
   - 상태머신 (Apply/Release 상태 관리)
   - AVH (Automatic Valet Hold) 로직
   - 가속도/속도 필터링 (다단계)
3. **정적 상태변수 다수**
   - `PbcApplyCurrentTemp`, `FilteredCurrentL`, `FilteredCurrentR` 등
   - 상태 추적 불명확
4. **Simulink 모델 원본 부재**
   - 생성 시점, 모델 버전 불명
   - 역엔지니어링 어려움
5. **하드웨어 의존도 높음**
   - ADC 값 읽기, SPI 명령 생성이 섞여있을 가능성
   - 계층 분리 불명확

**영향범위**: 모터제어 → 시스템 안정성 직결

**출처**: ElectronicParkBrake.h line 361 (ElectronicParkBrake_step)

---

#### **#2: Main_Api.c (Task Scheduler + Global State)**

**파일**: `EPB/Main_Api.c` (~350 라인)  
**이유**:

1. **전역상태 중앙집중**
   - `g_TaskControl`, `g_MAIN` 거의 모든 곳에서 접근
   - 필드 단위 읽기/쓰기 규칙 명확하지 않음
2. **상태 전이 복잡성**

   ```
   SystemDown → SystemOff (1초 대기) → PowerOff (4초 후)
   → 부분적 Sleep (L9369, CANSBC)
   → 완전 전원차단
   ```

   - 상태 다이어그램 없음
   - 타이밍 임계값 하드코딩 (SYSTEM_POWER_OFF_TIME=4000ms/5ms=800)

3. **조건부 실행 깊이**

   - TaskScheduler_5ms() 내 중첩 if-else 6단계
   - 가독성 저하, 오류 유입 가능성 높음

4. **태스크 계층화 불완전**
   - 5ms/10ms/20ms 구분이 모듈식이 아님
   - ExecuteTask() 래퍼는 있으나, 실제 논리는 직접 포함

**영향범위**: 전체 시스템 생명주기 제어

**출처**: Main_Api.c lines 195-285

---

#### **#3: Spi_L9369_Api.c (모터드라이버 제어)**

**파일**: `EPB/Application/MotorControl/Spi_L9369_Api.c` (1700+ 라인)  
**이유**:

1. **거대한 파일** → 단일 책임 원칙 위배
   - L9369 초기화, 레지스터 읽기/쓰기, 모터 구동, ADC 평균화, 테스트 로직 모두 포함
2. **SPI 프로토콜 복잡성**

   - MOSI/MISO 프로토콜 직접 구현
   - 레지스터 구조 파악 어려움
   - CRC/Checksum 계산 (추정)

3. **모터 구동 알고리즘 불명확**

   - `EpbMotorDriveProcess()` (line 1100) 호출 경로
   - 내부에서 `ElectronicParkBrake_step()` 호출하는지 여부 (확인필요)
   - PWM 명령 생성 방식

4. **오류 처리 미흡**
   - 오버커렌트, 센서 오류 감지 로직 있는지 미확인
   - 오류 발생 시 복구 절차 불명

**영향범위**: 모터제어 신뢰성, 안전성

**출처**: Spi_L9369_Api.c (파일내용 부분만 확인)

---

#### **#4: Can_Api.c + EB 스택 (CAN 통신/DTC)**

**파일**: `EPB/Application/Diagnostics/Can_Api.c` + `application/APP_Prg.c`  
**이유**:

1. **CAN 스택과 ASW 혼재**

   - TxCanMessage() → CAN_Task() → EB_Manage() 호출 체인
   - EB 스택(외부 라이브러리)의 내부 동작 불투명
   - COM, TP, UDS, DEM 등 계층 불명확

2. **DTC 관리 로직 산재**

   - DTC_CAN_Missing(), DTC_HW_Cycle() 등 진단 함수
   - Pending → Confirmed 상태 전이 기준 불명확
   - EEPROM 저장 시점/조건 미상

3. **신호 매핑 직관성 부족**

   - IL_u8Wr_EPBMotorAstate() 등 함수 다수
   - 신호 → CAN 메시지 매핑 규칙 (코드만으로 파악 어려움)

4. **CAN BUS OFF 처리**
   - board_irq.c의 ISR과 APP_Prg.c의 콜백이 분산
   - 복구 절차 (CAN_BusOff → CAN_BusOn) 순서 관계 불명

**영향범위**: 통신/진단 신뢰성

**출처**: Can_Api.c line 160 (TxCanMessage), APP_Prg.c lines 600-700 (CAN_BusOffInd)

---

#### **#5: 전역 구조체/변수의 필드 단위 접근 규칙 부재**

**현상**: 코드 전반  
**이유**:

1. **필드 단위 동시성 이슈**

   - g_TaskControl.Flag (비트필드, 공유 접근)
   - 인터럽트 ISR과 메인 루프에서 동시 접근 가능성
   - Mutex/Lock 없음 (확인필요)

2. **상태 일관성 관리 부재**

   - g_MAIN.SystemStatus와 g_TaskControl.Flag.SystemDown의 동기화 규칙 불명확
   - 예: SystemStatus==INIT일 때, SystemDown의 예상값?

3. **전역 상태 추적 어려움**

   - breakpoint 디버깅 시에도 상태 변화 경로 추적 어려움
   - 어떤 함수가 언제 어느 필드를 쓰는지 명시적 문서 부재

4. **테스트 불가능**
   - 전역 상태 초기화 없이 단위테스트 불가
   - Mock/Stub 작성 어려움

**영향범위**: 전체 코드 유지보수성

**출처**: Main_Api.h lines 11-54 (TaskControl_t 정의), Main_Api.c lines 66-87 (초기화)

---

## 9. 실행 타이밍 분석

### 9.1 TickCount 기반 태스크 실행 흐름

```
시스템 타이머 ISR (주기 불명, 추정: 1ms 또는 5ms)
    ↓
TaskScheduler_TaskCount() [Main_Api.c:195]
    ↓
g_TaskControl.TickCount++ (카운터 증가)
    ↓
메인 루프로 돌아옴
    ↓
while(1) → TaskScheduler_ActivateTask()
    ↓
TaskScheduler_TaskCalculation()
    ├─ if (TickCount % 20 == 0) → 5ms/10ms/20ms 태스크 모두 실행
    ├─ else if (TickCount % 10 == 0) → 5ms/10ms 실행
    └─ else if (TickCount % 5 == 0) → 5ms 실행
```

**문제점**:

- **기본 Tick 주기 불명확** (1ms? 5ms?)
- **ISR 호출 위치 미확인** (STM ISR? SysTick?)
- **TaskScheduler_TaskCount() 호출처 미상** (확인필요)

**출처**: Main_Api.c lines 195, 277 (추정)

### 9.2 예상 타임라인 (초기화 후)

```
T=0ms
  │
  ├─ SubEpbInitialize()
  │   ├─ BoardDisableInterrupts()
  │   ├─ DisableWatchdog()
  │   ├─ MEInitialize() [~수ms]
  │   ├─ CGMInitialize() [~수ms]
  │   ├─ STMInitialize() [인터럽트 설정]
  │   ├─ ADCInitialize()
  │   ├─ SpiInitialize()
  │   ├─ CanInitialize()
  │   └─ BoardEnableInterrupts() [이후 ISR 활성화]
  │
  ├─ ElectronicParkBrake_initialize() [상태변수 초기화]
  │
  ├─ TaskScheduler_Initialization() [스케줄러 초기화]
  │
  ├─ EepromReadAll() [EEPROM RX 대기, ~수십ms?]
  │
  └─ while(1) 진입
      │
      ├─ TaskScheduler_ActivateTask()
      │   └─ (TickCount 증가 시에만 실행)
      │
      T=5ms: TaskScheduler_5ms() 시작
      │
      T=10ms: TaskScheduler_5ms() + TaskScheduler_10ms()
      │
      T=20ms: 3개 태스크 모두 실행
      │
      ... (반복)
```

**확인필요**: EEPROM 읽기 완료 시간, 초기화 총 소요시간

---

## 10. 의문점 & 확인필요 항목

| 항목                                                           | 질문                                                         | 영향도 |
| -------------------------------------------------------------- | ------------------------------------------------------------ | ------ |
| **시스템 기본 Tick**                                           | 타이머 ISR 주기는 1ms인가 5ms인가?                           | 높음   |
| **TaskScheduler_TaskCount() 호출**                             | 어디서 호출되는가? (ISR? 메인루프?)                          | 높음   |
| **Watchdog 갱신**                                              | 정기적 ClearWatchdogTimer() 호출이 있는가?                   | 높음   |
| **TC277 역할**                                                 | TC277은 무엇인가? (센서? 동글?)                              | 중간   |
| **ElectronicParkBrake_step() 내부**                            | Simulink 로직이 모터 SPI 명령을 직접 생성하는가?             | 높음   |
| **EpbMotorDriveProcess()와 ElectronicParkBrake_step()의 관계** | 어느 것이 main인가?                                          | 높음   |
| **웨이크업 로직**                                              | CheckWakeupStatus()의 실제 동작 (EPB WKP 신호?)              | 중간   |
| **CANSBC(TLE9461) 역할**                                       | CAN transceiver? Power management?                           | 중간   |
| **L9369 ADC 평균화**                                           | L9369AdcAvr()는 L9369 내부 ADC를 읽는가, 아니면 MCU ADC인가? | 높음   |
| **DTC Pending/Confirmed 기준**                                 | test_fail_counter 임계값이 정의된 곳?                        | 중간   |
| **EEPROM 저장항목**                                            | SaveParamData()는 무엇을 저장하는가?                         | 낮음   |

---

## 11. 파일별 라인수 & 복잡도 추정

| 파일                  | 라인수 | 함수수 | 구조체수 | 복잡도 추정               |
| --------------------- | ------ | ------ | -------- | ------------------------- |
| Main_Api.c            | 350    | 10+    | 2        | **높음** (상태관리)       |
| ElectronicParkBrake.c | 442    | ?      | ?        | **높음** (Simulink)       |
| Spi_L9369_Api.c       | 1700+  | 20+    | ?        | **높음** (거대파일)       |
| Can_Api.c             | ~200   | 5+     | ?        | **중간** (스택연계)       |
| FailSafe_Api.c        | ~100   | 3+     | ?        | **낮음**                  |
| APP_Prg.c             | 2359   | 20+    | 5+       | **매우높음** (기존EB스택) |

**총 코드량**: ~5500+ 라인 (EB 스택 제외)

---

## 12. 문서 생성 이력

| 항목                  | 상태                 |
| --------------------- | -------------------- |
| Boot Flow 분석        | ✓ 완료               |
| Task Schedule 분석    | ✓ 완료               |
| Call Tree 작성        | ✓ 완료               |
| Global State 매트릭스 | ✓ 완료 (필드단위)    |
| Hardware Touch Points | ✓ 완료               |
| Feature Inventory     | ✓ 완료               |
| Dependency Graph      | ✓ 완료               |
| Hotspot 분석          | ✓ 완료 (개선안 제외) |
| 확인필요 사항         | ✓ 리스트업           |

---

## 부록: 약자 정의

| 약자       | 의미                              | 용도                       |
| ---------- | --------------------------------- | -------------------------- |
| **EPB**    | Electronic Park Brake             | 전자식 주차 브레이크       |
| **EB**     | Elektrobit                        | CAN 스택 라이브러리 회사   |
| **ASW**    | Application Software              | 애플리케이션 계층          |
| **BSW**    | Basic Software                    | 기본 소프트웨어 (드라이버) |
| **MCAL**   | Microcontroller Abstraction Layer | MCU 하드웨어 추상화        |
| **CAN**    | Controller Area Network           | 자동차 통신 프로토콜       |
| **COM**    | Communication Manager             | CAN 신호 관리 (EB 스택)    |
| **TP**     | Transport Protocol                | 멀티프레임 통신 (EB 스택)  |
| **UDS**    | Unified Diagnostic Services       | 진단 서비스                |
| **DEM**    | Diagnostics Event Manager         | DTC 관리 (EB 스택)         |
| **DTC**    | Diagnostic Trouble Code           | 진단 오류 코드             |
| **SPI**    | Serial Peripheral Interface       | 동기식 직렬 통신           |
| **PWM**    | Pulse Width Modulation            | 펄스폭 변조 (모터제어)     |
| **ADC**    | Analog-to-Digital Converter       | 아날로그→디지털 변환기     |
| **ISR**    | Interrupt Service Routine         | 인터럽트 핸들러            |
| **WDG**    | Watchdog                          | 시스템 감시용 타이머       |
| **CANSBC** | CAN System Base Chip              | CAN 트랜시버 IC (TLE9461)  |

---

**문서 버전**: 1.0  
**최종 작성**: 2026-01-12  
**상태**: 완료 (확인필요 항목 6개 남음)
