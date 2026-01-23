# EPB 프로젝트 As-Is 코드 이해 문서

**문서 목적**: 처음 이 프로젝트를 접하는 개발자가 현재 코드 구조를 이해할 수 있도록 작성  
**작성 기준**: 실제 코드 분석 결과만 포함 (추측/개선안 제외)  
**작성일**: 2026-01-12

---

## 1. 한 페이지 요약

### 1.1 시스템 개요

EPB(Electronic Park Brake) 프로젝트는 **NXP XPC560XB MCU** 기반의 임베디드 시스템으로, **Bare-metal 환경**에서 동작합니다. RTOS 없이 **타이머 ISR 기반 주기 태스크 스케줄링** 방식을 사용합니다.

### 1.2 핵심 실행 구조

```
┌─────────────────────────────────────────────────────────────┐
│  STM 타이머 ISR (1ms 주기)                                  │
│  └─ TickCount 증가 + 스케줄러 플래그 설정                   │
└─────────────────────┬───────────────────────────────────────┘
                      ↓
┌─────────────────────────────────────────────────────────────┐
│  메인 루프 (while(1))                                       │
│  └─ TaskScheduler_ActivateTask()                           │
│      ├─ 5ms 태스크: CAN 통신, 전원 관리, 상태 체크         │
│      ├─ 10ms 태스크: EPB 모터 제어, 진단                   │
│      └─ 20ms 태스크: Watchdog, 상태 모니터링               │
└─────────────────────────────────────────────────────────────┘
```

**타이밍 소스**: STM(System Timer Module) 하드웨어 타이머  
**기본 Tick**: 1ms (ISR 주기)  
**태스크 주기**: 1ms, 5ms, 10ms, 20ms

### 1.3 주요 구성 요소

| 구성 요소                 | 역할                             | 파일 위치                                        |
| ------------------------- | -------------------------------- | ------------------------------------------------ |
| **Main_Api.c**            | 시스템 초기화 및 태스크 스케줄러 | `EPB/Main_Api.c`                                 |
| **L9369 드라이버**        | 모터 드라이버 IC 제어 (SPI 통신) | `EPB/Application/MotorControl/Spi_L9369_Api.c`   |
| **TLE9461 드라이버**      | CAN SBC(System Basis Chip) 제어  | `EPB/Application/MotorControl/Spi_TLE9461_Api.c` |
| **ElectronicParkBrake.c** | EPB 제어 로직 (Simulink 생성)    | `EPB/AppLogic/ElectronicParkBrake.c`             |
| **Can_Api.c**             | CAN 통신 관리                    | `EPB/Application/Diagnostics/Can_Api.c`          |
| **APP_Prg.c**             | UDS 진단 서비스 (2359줄)         | `application/APP_Prg.c`                          |

### 1.4 전역 상태 관리

시스템의 모든 상태는 **2개의 전역 구조체**에 집중되어 있습니다:

| 전역 변수       | 용도                 | 주요 필드                                                      |
| --------------- | -------------------- | -------------------------------------------------------------- |
| `g_TaskControl` | 태스크 스케줄링 제어 | `TickCount`, `Flag.Enable`, `Flag.SystemDown`, `PowerOffCount` |
| `g_MAIN`        | 시스템 상태 관리     | `SystemStatus`, `MtDrvStatus`, `PowerOffOk`                    |

이 구조체들은 코드 전반에서 직접 접근되며, **God Object 패턴**의 특징을 보입니다.

### 1.5 주요 동작 흐름 요약

```
전원 ON → main() 진입
    ↓
하드웨어 초기화 (CAN, SPI, ADC, Timer 등)
    ↓
Simulink EPB 로직 초기화
    ↓
EEPROM 데이터 로드
    ↓
while(1) 루프 진입 (무한 반복)
    ├─ 1ms ISR: TickCount++, 모터 동기화
    ├─ 5ms: CAN 송수신, 전원 상태 체크
    ├─ 10ms: EPB 모터 구동, 진단
    └─ 20ms: Watchdog, 상태 모니터링
```

---

## 2. 전체 실행 흐름

이 섹션에서는 시스템이 전원이 켜진 후 어떻게 동작하는지를 **시간 순서대로** 설명합니다.

### 2.1 부팅 시퀀스 (Boot Sequence)

#### 2.1.1 Reset Handler → main() 진입

전원이 켜지면 MCU의 Reset Handler가 실행되고, startup 코드(어셈블리)가 스택과 메모리를 초기화한 후 `main()` 함수를 호출합니다.

**파일**: `EPB/Main_Api.c`  
**함수**: `main()`

#### 2.1.2 하드웨어 초기화 단계 (SubEpbInitialize)

`main()` 함수는 가장 먼저 `SubEpbInitialize()` 함수를 호출하여 하드웨어를 초기화합니다.

```
SubEpbInitialize()
├── BoardDisableInterrupts()      // 초기화 중 인터럽트 방지
├── DisableWatchdog()             // 워치독 타이머 비활성화
├── MEInitialize()                // MCU 모드 설정
├── CGMInitialize()               // 클럭 생성 모듈 설정
├── PORTInitialize()              // GPIO 핀 설정
├── STMInitialize()               // 시스템 타이머 설정 (1ms ISR)
├── ADCInitialize()               // ADC 모듈 설정
├── SpiInitialize()               // SPI 컨트롤러 설정
├── CanInitialize()               // CAN 컨트롤러 설정
└── BoardEnableInterrupts()       // 인터럽트 활성화
```

**특징**:

- 각 드라이버는 MCU의 **레지스터를 직접 조작**하여 초기화합니다
- `STMInitialize()`에서 **1ms 주기 타이머 인터럽트**를 설정합니다
- `BoardEnableInterrupts()` 이후부터 ISR이 동작합니다

**근거**: `EPB/Main_Api.c` main() 함수 내 SubEpbInitialize() 호출

#### 2.1.3 애플리케이션 초기화

하드웨어 초기화 후, 애플리케이션 레벨 초기화를 수행합니다:

```
ElectronicParkBrake_initialize()
└── Simulink 생성 코드의 초기화 함수 실행
    └── EPB 제어 로직의 내부 상태 변수 초기화

TaskScheduler_Initialization(TASK_5000ms)
├── g_TaskControl 구조체 초기화
│   ├── TickCount = 0
│   ├── Flag.SystemDown = TRUE (초기 상태)
│   └── 기타 플래그 초기화
├── g_MAIN 구조체 초기화
│   ├── SystemStatus = INIT
│   └── MtDrvStatus = INIT
└── APP_Init() 호출 (EB CAN 스택 초기화)

EepromReadAll()
└── EEPROM에서 저장된 파라미터를 읽어 메모리로 로드
```

**근거**: `EPB/Main_Api.c` main() 함수 내 순차 호출

#### 2.1.4 메인 루프 진입

모든 초기화가 완료되면 `while(1)` 무한 루프에 진입합니다:

```c
while(1) {
    TaskScheduler_ActivateTask();  // 태스크 스케줄러 실행
}
```

이 시점부터 시스템은 **타이머 ISR과 메인 루프가 협력하여** 주기 태스크를 실행합니다.

---

### 2.2 Runtime 실행 구조

#### 2.2.1 타이머 ISR의 역할 (1ms 주기)

**파일**: `EPB/Peripheral/Timer/Stm_Api.c`  
**함수**: `STM_CH0_ISR()`  
**실행 주기**: 1ms

STM 타이머 하드웨어는 1ms마다 인터럽트를 발생시킵니다. ISR에서는 다음 작업을 수행합니다:

```
STM_CH0_ISR()
├── 인터럽트 플래그 클리어 (STM.CH[0].CIR.B.CIF)
├── TaskScheduler_TaskCount() 호출
│   ├── g_TaskControl.TickCount++ (틱 카운터 증가)
│   └── g_TaskControl.Flag.Enable = TRUE (스케줄러 활성화)
└── L9369_TASK_1ms() 호출
    └── 모터 드라이버 동기화 신호 토글 (MtDrvStatus == NORMAL일 때만)
```

**타이머 설정 근거**:

- `STM.CH[0].CMP.R = (uint32_t)(Time3*10)` (Stm_Api.c:14)
- `Time3 = 6400` (1ms 기준, typedefs.h:103)
- ISR 주석: `//1msec period, Match on System Timer Mode channel 0`

**핵심 역할**:

- `TickCount`를 증가시켜 **시스템 시간 기준**을 제공
- `Flag.Enable`을 설정하여 메인 루프의 태스크 실행을 허용
- 모터 드라이버의 **1ms 동기화 신호** 생성 (타이밍 크리티컬)

#### 2.2.2 메인 루프의 태스크 스케줄링

메인 루프는 `TaskScheduler_ActivateTask()` 함수를 반복 호출합니다.

**파일**: `EPB/Main_Api.c`  
**함수**: `TaskScheduler_ActivateTask()` (line 289)

```
TaskScheduler_ActivateTask()
├── if (g_TaskControl.Flag.Enable == TRUE)  // ISR이 설정한 플래그 확인
│   ├── Flag.Enable = FALSE  // 플래그 리셋
│   └── TaskScheduler_TaskCalculation() 호출
│       └── TickCount 값에 따라 주기 태스크 실행
└── TaskScheduler_TaskCount() 호출
    └── (추가 동기화 처리)
```

#### 2.2.3 주기별 태스크 선택 로직

**함수**: `TaskScheduler_TaskCalculation()` (line 118)

`TickCount` 값을 **나머지 연산(modulo)**으로 확인하여 실행할 태스크를 결정합니다:

```
TaskScheduler_TaskCalculation()
├── if (TickCount % 20 == 0)  // 20ms 주기 체크
│   ├── TaskScheduler_5ms() 실행
│   ├── TaskScheduler_10ms() 실행
│   └── TaskScheduler_20ms() 실행
├── else if (TickCount % 10 == 0)  // 10ms 주기 체크
│   ├── TaskScheduler_5ms() 실행
│   └── TaskScheduler_10ms() 실행
└── else if (TickCount % 5 == 0)   // 5ms 주기 체크
    └── TaskScheduler_5ms() 실행
```

**실행 예시** (TickCount 기준):

- TickCount = 1: 아무것도 실행 안함
- TickCount = 5: 5ms 태스크만 실행
- TickCount = 10: 5ms + 10ms 태스크 실행
- TickCount = 15: 5ms 태스크만 실행
- TickCount = 20: 5ms + 10ms + 20ms 태스크 모두 실행

**근거**: `EPB/Main_Api.c:118-153` 라인 분석

#### 2.2.4 태스크 실행 순서 정리

| TickCount | 1ms ISR  | 5ms Task | 10ms Task | 20ms Task |
| --------- | -------- | -------- | --------- | --------- |
| 매 1ms    | ✓ (항상) | -        | -         | -         |
| 5ms 배수  | ✓        | ✓        | -         | -         |
| 10ms 배수 | ✓        | ✓        | ✓         | -         |
| 20ms 배수 | ✓        | ✓        | ✓         | ✓         |

**주의사항**:

- **1ms ISR은 항상 실행**되며, 메인 루프와 독립적으로 동작
- 5ms/10ms/20ms 태스크는 **메인 루프에서 순차 실행** (비동기 아님)
- 20ms 시점에는 모든 태스크가 한 번에 실행되므로 **실행 시간이 가장 김**

---

### 2.3 시스템 상태 전이

시스템은 `g_MAIN.SystemStatus` 값에 따라 다르게 동작합니다.

#### 2.3.1 상태 정의

| 상태                   | 값        | 의미                         |
| ---------------------- | --------- | ---------------------------- |
| `SYSTEM_STATUS_INIT`   | 0         | 시스템 초기화 중 (부팅 직후) |
| `SYSTEM_STATUS_NORMAL` | 1         | 정상 동작 상태               |
| (기타)                 | 확인 필요 | POWER_DOWN, EMERGENCY 등     |

**근거**: `EPB/Main_Api.c` 내 SystemStatus 사용 패턴

#### 2.3.2 상태 전이 흐름

```
부팅 시작
    ↓
SystemStatus = INIT
    ├─ 5ms 태스크에서 TLE9461(CAN SBC) 초기화 시도
    └─ 초기화 성공 시
        ↓
SystemStatus = NORMAL
    ├─ 모터 드라이버(L9369) 초기화 시작
    └─ 정상 동작 (EPB 제어 활성화)
```

**전원 다운 시**:

```
IGN OFF 신호 감지
    ↓
g_TaskControl.Flag.SystemDown = TRUE
    ├─ 5ms 태스크에서 CAN 메시지 송신 유지
    ├─ PowerOffCount 증가 (카운트다운 시작)
    └─ 일정 시간 후 (4초)
        ↓
PowerOff() 함수 실행
    ├─ CAN 버스 비활성화
    ├─ 모터 드라이버 슬립 모드
    └─ g_MAIN.PowerOffOk = TRUE
```

**근거**: `EPB/Main_Api.c` 내 TaskScheduler_5ms() 분기 로직

#### 2.3.3 상태별 태스크 동작 차이

**5ms 태스크 예시**:

```
TaskScheduler_5ms()
├── if (Flag.SystemDown == TRUE)  // 전원 다운 중
│   ├─ CAN 메시지 송신 (상태 알림)
│   ├─ 센서 체크 (ADC, Port)
│   ├─ 파라미터 EEPROM 저장
│   └─ PowerOff 카운트다운
├── else if (SystemStatus == INIT)  // 초기화 중
│   ├─ 웨이크업 상태 체크
│   └─ CAN SBC 초기화
└── else if (SystemStatus == NORMAL)  // 정상 동작
    ├─ CAN 통신 유지
    ├─ 센서 체크
    └─ 모터 드라이버 초기화/상태 체크
```

**근거**: `EPB/Main_Api.c:225-285` TaskScheduler_5ms() 함수 분석

---

### 2.4 실행 흐름 타임라인 예시

실제 시스템이 동작할 때의 타임라인을 예시로 정리합니다:

```
T=0ms: 전원 ON
  └─ main() 시작

T=10ms: SubEpbInitialize() 완료 (추정)
  └─ 하드웨어 초기화 완료

T=11ms: ElectronicParkBrake_initialize() 완료
  └─ Simulink 로직 초기화

T=12ms: TaskScheduler_Initialization() 완료
  ├─ g_TaskControl, g_MAIN 초기화
  └─ SystemStatus = INIT

T=20ms: EepromReadAll() 완료 (추정)
  └─ EEPROM 데이터 로드

T=21ms: while(1) 루프 진입
  └─ 주기 태스크 시작

--- 이후 반복 ---

T=22ms: 1ms ISR 실행 (TickCount=1)
T=23ms: 1ms ISR 실행 (TickCount=2)
...
T=26ms: 1ms ISR 실행 (TickCount=5)
  └─ 메인 루프: 5ms 태스크 실행
      ├─ SystemStatus == INIT
      └─ TLE9461 초기화 시작

T=31ms: 1ms ISR 실행 (TickCount=10)
  └─ 메인 루프: 5ms + 10ms 태스크 실행

T=41ms: 1ms ISR 실행 (TickCount=20)
  └─ 메인 루프: 5ms + 10ms + 20ms 태스크 실행

(이후 패턴 반복...)
```

**주의**: 위 시간은 설명을 위한 예시이며, 실제 초기화 시간은 다를 수 있습니다.

---

### 2.5 실행 흐름 요약

1. **부팅 단계**: main() → 하드웨어 초기화 → 애플리케이션 초기화 → while(1) 진입
2. **Runtime 단계**: 1ms ISR + 메인 루프 협력 방식
3. **태스크 실행**: TickCount % 주기로 5/10/20ms 태스크 선택
4. **상태 관리**: SystemStatus에 따라 태스크 내부 동작 분기
5. **전원 관리**: SystemDown 플래그로 전원 다운 시퀀스 제어

**핵심 특징**:

- **단순한 구조**: RTOS 없이 타이머 ISR + while 루프로 구현
- **명확한 타이밍**: 1ms ISR 기준으로 모든 주기가 결정됨
- **전역 상태 중심**: g_TaskControl, g_MAIN 구조체에 시스템 상태 집중

---

## 3. 주기별 태스크 역할 정리

이 섹션에서는 각 주기별로 **어떤 함수가 실행되며, 왜 그 주기에 배치되었는지**를 정리합니다.

### 3.1 전체 태스크 주기 구조

| 주기     | 실행 조건             | 태스크 함수            | 파일 위치                                           | 실행 방식             |
| -------- | --------------------- | ---------------------- | --------------------------------------------------- | --------------------- |
| **1ms**  | STM ISR 발생 시       | `L9369_TASK_1ms()`     | `EPB/Application/MotorControl/Spi_L9369_Api.c:1823` | 인터럽트 내 직접 실행 |
| **5ms**  | `TickCount % 5 == 0`  | `TaskScheduler_5ms()`  | `EPB/Main_Api.c:225`                                | 메인 루프에서 실행    |
| **10ms** | `TickCount % 10 == 0` | `TaskScheduler_10ms()` | `EPB/Main_Api.c:208`                                | 메인 루프에서 실행    |
| **20ms** | `TickCount % 20 == 0` | `TaskScheduler_20ms()` | `EPB/Main_Api.c:191`                                | 메인 루프에서 실행    |

**실행 순서**:

- 10ms 시점: `TaskScheduler_5ms()` → `TaskScheduler_10ms()`
- 20ms 시점: `TaskScheduler_5ms()` → `TaskScheduler_10ms()` → `TaskScheduler_20ms()`

**근거**: `EPB/Main_Api.c:118-153` TaskScheduler_TaskCalculation() 함수

---

### 3.2 1ms 태스크 (ISR 레벨)

#### 3.2.1 실행 함수

**파일**: `EPB/Application/MotorControl/Spi_L9369_Api.c`  
**함수**: `L9369_TASK_1ms()` (line 1823)

#### 3.2.2 실행 조건

```
STM_CH0_ISR() 내부에서 호출
└── if (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
    └── L9369_TASK_1ms() 실행
```

**조건**: 모터 드라이버가 정상 동작 상태일 때만 실행

#### 3.2.3 수행 작업

| 작업                      | 함수               | 목적                                                |
| ------------------------- | ------------------ | --------------------------------------------------- |
| **모터 동기화 신호 토글** | `L9369_Sync_1ms()` | L9369 모터 드라이버 IC의 동기화 신호를 1ms마다 토글 |

**상세 동작**:

- `PORT_SUB_L9369_SYNC1` 핀을 High/Low 토글
- L9369 IC와 MCU 간 타이밍 동기화 유지

**근거**: `EPB/Application/MotorControl/Spi_L9369_Api.c:1791` L9369_Sync_1ms() 함수

#### 3.2.4 1ms 주기를 사용하는 이유

- **타이밍 크리티컬**: 모터 드라이버와의 동기화는 정확한 타이밍 필요
- **ISR 실행**: 메인 루프의 지연에 영향받지 않고 정확한 1ms 간격 보장
- **하드웨어 요구사항**: L9369 IC의 데이터시트에서 요구하는 동기화 신호 주기 (추정)

---

### 3.3 5ms 태스크 (고빈도 작업)

#### 3.3.1 실행 함수

**파일**: `EPB/Main_Api.c`  
**함수**: `TaskScheduler_5ms()` (line 225)

#### 3.3.2 주요 역할

5ms 태스크는 **시스템 상태에 따라 다른 작업을 수행**합니다. 가장 자주 실행되므로 통신, 센서 체크 등 **실시간성이 중요한 작업**을 처리합니다.

#### 3.3.3 시스템 상태별 작업 (상태 분기)

**분기 기준**: `g_TaskControl.Flag.SystemDown`, `g_MAIN.SystemStatus`

##### (A) SystemDown == TRUE (전원 다운 중)

| 작업                      | 함수                                   | 파일 위치                                           | 목적                              |
| ------------------------- | -------------------------------------- | --------------------------------------------------- | --------------------------------- |
| **CAN 메시지 송신**       | `TxCanMessage()`                       | `EPB/Application/Diagnostics/Can_Api.c:168`         | 시스템 상태를 CAN 네트워크에 알림 |
| **ADC 상태 체크**         | `CheckAdcStatus()`                     | `EPB/Peripheral/ADC/Adc_Api.c:134`                  | 전압, 센서 값 모니터링            |
| **포트 상태 체크**        | `CheckPortStatus()`                    | `EPB/Application/Port_Api.c:135`                    | GPIO 핀 상태 확인                 |
| **CAN SBC 레지스터 체크** | `CheckCANSBCRegStatus(SPI_CANSBC_RUN)` | `EPB/Application/MotorControl/Spi_TLE9461_Api.c:19` | CAN 트랜시버 상태 체크            |
| **파라미터 저장**         | `SaveParamData()`                      | 확인 필요                                           | EEPROM에 중요 데이터 저장         |
| **전원 차단 카운트다운**  | `PowerOffCount++`                      | `EPB/Main_Api.c:238`                                | 일정 시간 후 PowerOff() 호출      |

**전원 차단 로직**:

```
if (PowerOffCount > SYSTEM_POWER_OFF_TIME) {
    PowerOff();  // 약 4초 후 실제 전원 차단
}
```

##### (B) SystemStatus == INIT (초기화 중)

| 작업                   | 함수                              | 목적                      |
| ---------------------- | --------------------------------- | ------------------------- |
| **웨이크업 상태 체크** | `CheckWakeupStatus(WAKEUP_START)` | 시스템 웨이크업 조건 확인 |
| **포트 상태 체크**     | `CheckPortStatus()`               | GPIO 핀 상태 확인         |
| **ADC 상태 체크**      | `CheckAdcStatus()`                | 센서 값 모니터링          |
| **CAN SBC 초기화**     | `TLE9461Init(FALSE)`              | CAN 트랜시버 IC 초기화    |

**상태 전이**:

- `TLE9461Init()` 성공 시 → `SystemStatus = NORMAL`로 변경

##### (C) SystemStatus == NORMAL (정상 동작)

| 작업                          | 함수                                       | 목적                                    |
| ----------------------------- | ------------------------------------------ | --------------------------------------- |
| **CAN 메시지 송신**           | `TxCanMessage()`                           | 정상 CAN 통신 유지                      |
| **포트 상태 체크**            | `CheckPortStatus()`                        | GPIO 핀 상태 확인                       |
| **ADC 상태 체크**             | `CheckAdcStatus()`                         | 센서 값 모니터링                        |
| **CAN SBC 레지스터 체크**     | `CheckCANSBCRegStatus(SPI_CANSBC_RUN)`     | CAN 트랜시버 상태 체크                  |
| **모터 드라이버 초기화/체크** | `L9369Init()` 또는 `CheckL9369RegStatus()` | L9369 상태에 따라 초기화 또는 상태 체크 |

**모터 드라이버 제어 로직**:

```
if (g_TC277.fMainfault == TRUE) {  // TC277 오류 발생
    if (MtDrvStatus == NORMAL) {
        CheckL9369RegStatus(SPI_L9369_RUN);  // 레지스터 체크
    } else {
        MtDrvStatus = L9369Init(FALSE);  // 초기화
    }
} else {  // TC277 정상
    MtDrvStatus = MTDRV_STATUS_INIT;  // 초기화 상태로 리셋
}
```

**근거**: `EPB/Main_Api.c:225-285` TaskScheduler_5ms() 함수 분석

#### 3.3.4 5ms 주기를 사용하는 이유

- **CAN 통신**: 일반적으로 5~10ms 주기로 메시지 송수신 (차량 네트워크 표준)
- **센서 모니터링**: ADC, GPIO 상태를 빠르게 체크하여 이상 징후 조기 감지
- **전원 관리**: SystemDown 상태에서 빠른 응답 필요
- **실시간성**: 사용자 입력(스위치), 센서 변화에 빠르게 반응

---

### 3.4 10ms 태스크 (모터 제어 & 진단)

#### 3.4.1 실행 함수

**파일**: `EPB/Main_Api.c`  
**함수**: `TaskScheduler_10ms()` (line 208)

#### 3.4.2 실행 조건 및 작업

**전체 조건**: `g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL`

##### (A) MtDrvStatus == NORMAL (모터 드라이버 정상)

| 작업                 | 함수                     | 파일 위치 | 목적                                        |
| -------------------- | ------------------------ | --------- | ------------------------------------------- |
| **L9369 ADC 평균화** | `L9369AdcAvr()`          | 확인 필요 | 모터 전류 등 ADC 값 평균 계산 (노이즈 제거) |
| **EPB 모터 테스트**  | `EpbMotorTestLogic()`    | 확인 필요 | 모터 테스트 시퀀스 실행                     |
| **EPB 모터 구동**    | `EpbMotorDriveProcess()` | 확인 필요 | **실제 모터 제어 로직 실행** (핵심)         |

##### (B) 모든 상태에서 실행

| 작업            | 함수                     | 목적                       |
| --------------- | ------------------------ | -------------------------- |
| **진단 태스크** | `Diagnostic_Task_10ms()` | DTC 생성 및 고장 진단 처리 |

**근거**: `EPB/Main_Api.c:208-224` TaskScheduler_10ms() 함수

#### 3.4.3 각 작업의 역할

**L9369AdcAvr()**:

- L9369 모터 드라이버 IC의 ADC 값(전류, 전압 등)을 여러 번 읽어 평균 계산
- 노이즈 필터링을 통해 안정적인 센서 값 획득
- 평균값은 모터 제어 로직에서 사용

**EpbMotorTestLogic()**:

- EPB 모터의 테스트 시퀀스 실행
- 모터 응답성, 전류 소비, 위치 센서 등을 검증
- 테스트 결과는 진단 데이터로 저장

**EpbMotorDriveProcess()**:

- **EPB 모터 제어의 핵심 함수**
- Simulink 생성 로직(`ElectronicParkBrake_step()`) 호출 가능성 높음
- PWM 듀티 계산 및 SPI 명령 전송
- 모터 위치 제어, 힘 제어 등 실제 구동 처리

**Diagnostic_Task_10ms()**:

- DTC(Diagnostic Trouble Code) 생성
- 고장 진단 로직 실행
- 오류 카운터 업데이트

#### 3.4.4 10ms 주기를 사용하는 이유

- **모터 제어 주기**: 전기 모터 제어는 일반적으로 10~20ms 주기 사용
- **ADC 평균화**: 여러 샘플을 평균하기 위한 적절한 간격
- **제어 안정성**: 너무 빠른 주기는 불필요한 연산, 너무 느린 주기는 제어 성능 저하
- **진단 처리**: DTC 생성 및 오류 감지는 10ms 주기가 적절

---

### 3.5 20ms 태스크 (감시 & 모니터링)

#### 3.5.1 실행 함수

**파일**: `EPB/Main_Api.c`  
**함수**: `TaskScheduler_20ms()` (line 191)

#### 3.5.2 실행 조건 및 작업

**전체 조건**: `g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL`

##### (A) MtDrvStatus == NORMAL (모터 드라이버 정상)

| 작업                    | 함수                   | 파일 위치                                           | 목적                           |
| ----------------------- | ---------------------- | --------------------------------------------------- | ------------------------------ |
| **L9369 Watchdog 체크** | `CheckL9369Watchdog()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1557` | 모터 드라이버 IC Watchdog 갱신 |

##### (B) 모든 상태에서 실행

| 작업                 | 함수                 | 파일 위치 | 목적                          |
| -------------------- | -------------------- | --------- | ----------------------------- |
| **CAN SBC Watchdog** | `CANSBCWatchdog()`   | 확인 필요 | CAN 트랜시버 IC Watchdog 갱신 |
| **ECU 리셋 체크**    | `EcuResetCheck()`    | 확인 필요 | ECU 리셋 요청 확인            |
| **TC277 상태 체크**  | `CheckTC277Status()` | 확인 필요 | TC277 통신 상태 확인          |

**근거**: `EPB/Main_Api.c:191-206` TaskScheduler_20ms() 함수

#### 3.5.3 각 작업의 역할

**CheckL9369Watchdog()**:

- L9369 모터 드라이버 IC의 Watchdog 타이머를 주기적으로 갱신
- Watchdog 타임아웃 방지 (타임아웃 시 IC가 안전 모드로 전환)

**CANSBCWatchdog()**:

- TLE9461 CAN SBC(System Basis Chip)의 Watchdog 갱신
- CAN 통신 안정성 보장

**EcuResetCheck()**:

- 진단 서비스(UDS)를 통한 ECU 리셋 요청 확인
- 리셋 조건 만족 시 소프트웨어 리셋 수행

**CheckTC277Status()**:

- TC277 (외부 IC, 용도 확인 필요) 통신 상태 체크
- 통신 오류 시 오류 플래그 설정

#### 3.5.4 20ms 주기를 사용하는 이유

- **Watchdog 주기**: 일반적으로 Watchdog은 10~50ms 주기로 갱신
- **비긴급 감시**: 실시간 제어가 아닌 시스템 건강도 체크
- **부하 분산**: 가장 느린 주기에 배치하여 시스템 부하 최소화
- **IC 사양**: L9369, TLE9461 데이터시트에서 권장하는 Watchdog 주기 (추정)

---

### 3.6 주기별 우선순위 및 실행 시간 고려사항

#### 3.6.1 주기별 중요도

| 주기     | 중요도 | 실시간성        | 실행 빈도 (1초 기준) |
| -------- | ------ | --------------- | -------------------- |
| **1ms**  | 최고   | 매우 높음 (ISR) | 1000회               |
| **5ms**  | 높음   | 높음            | 200회                |
| **10ms** | 중간   | 중간            | 100회                |
| **20ms** | 낮음   | 낮음            | 50회                 |

#### 3.6.2 실행 시간 제약

**1ms ISR**:

- 실행 시간: 수십 μs 이내 (추정)
- 제약: 다음 ISR 발생 전에 완료해야 함 (1ms 이내)
- 위험: ISR 지연 시 다른 태스크도 지연

**5ms/10ms/20ms 태스크**:

- 메인 루프에서 순차 실행 (비선점)
- 20ms 시점에는 3개 태스크 모두 실행 → 가장 긴 실행 시간
- 제약: 다음 주기 시작 전에 완료해야 함

**근거**: 일반적인 임베디드 시스템 설계 원칙

#### 3.6.3 주기별 작업 배치 원칙 (추정)

1. **1ms**: 하드웨어 동기화 신호 등 타이밍 크리티컬 작업
2. **5ms**: 통신, 센서 체크 등 고빈도 작업
3. **10ms**: 제어 로직, 진단 등 중간 빈도 작업
4. **20ms**: Watchdog, 모니터링 등 저빈도 작업

---

## 4. Global State 설명

이 섹션에서는 **시스템 전체 상태를 관리하는 전역 구조체**를 설명합니다. 이 구조체들은 코드 전반에서 직접 접근되며, **God Object 패턴**의 특징을 보입니다.

### 4.1 전역 구조체 개요

EPB 시스템은 크게 **2개의 핵심 전역 구조체**로 상태를 관리합니다:

| 전역 변수         | 타입            | 선언 위치           | 주요 역할            |
| ----------------- | --------------- | ------------------- | -------------------- |
| **g_TaskControl** | `TaskControl_t` | `EPB/Main_Api.c:29` | 태스크 스케줄러 제어 |
| **g_MAIN**        | `Main_t`        | `EPB/Main_Api.c:30` | 시스템 상태 관리     |

**근거**: `EPB/Main_Api.h:52-103` 구조체 정의

#### 4.1.1 기타 전역 구조체

| 전역 변수          | 타입               | 용도                | 크기(추정) |
| ------------------ | ------------------ | ------------------- | ---------- |
| `g_TC277`          | `TC277_t`          | TC277 통신 상태     | 확인 필요  |
| `g_CAN`            | `Can_t`            | CAN 통신 상태       | ~20 bytes  |
| `g_UDS`            | `Uds_t`            | UDS 진단 상태       | ~50 bytes  |
| `g_ADC`            | `ADC_t`            | ADC 데이터          | 확인 필요  |
| `g_L9369_RegWrite` | `L9369_RegWrite_t` | L9369 레지스터 쓰기 | 확인 필요  |
| `g_L9369_Status`   | `L9369_Status_t`   | L9369 상태          | 확인 필요  |

---

### 4.2 g_TaskControl 구조체 상세

#### 4.2.1 구조체 정의

**파일**: `EPB/Main_Api.h` (lines 52-95)  
**선언**: `static TaskControl_t g_TaskControl;` (Main_Api.c:29)  
**크기**: 약 100 bytes (추정)

#### 4.2.2 주요 필드 목록

| 필드                               | 타입     | 용도                           |
| ---------------------------------- | -------- | ------------------------------ |
| **TickCount**                      | `uint16` | 시스템 틱 카운터 (1ms 단위)    |
| **Tick**                           | `uint16` | 시스템 기본 틱 (5ms 고정)      |
| **TaskRun**                        | `uint16` | 현재 실행 중인 태스크 ID       |
| **TaskMax**                        | `uint16` | 최대 틱 카운트 (5000ms)        |
| **Flag.Enable**                    | `1bit`   | 태스크 스케줄러 활성화 플래그  |
| **Flag.SystemDown**                | `1bit`   | IGN OFF 상태 플래그            |
| **Flag.SystemOff**                 | `1bit`   | IGN OFF 1초 유지 플래그        |
| **Flag.PowerOff**                  | `1bit`   | 전원 차단 완료 플래그          |
| **PowerOffCount**                  | `uint32` | 파워 오프 카운터 (5ms 단위)    |
| **WakeupStatus**                   | `uint8`  | 웨이크업 상태                  |
| **WakeupStep**                     | `uint8`  | 웨이크업 단계                  |
| **TaskCount_5ms_Cnt**              | `uint32` | 5ms 태스크 실행 횟수           |
| **TaskCount_10ms_Cnt**             | `uint32` | 10ms 태스크 실행 횟수          |
| **TaskCount_20ms_Cnt**             | `uint32` | 20ms 태스크 실행 횟수          |
| **TaskCount\_\*\_TimeElapsed**     | `uint32` | 각 태스크 실행 시간 (디버그용) |
| **TaskCount\_\*\_TimeElapsed_Max** | `uint32` | 각 태스크 최대 실행 시간       |

#### 4.2.3 필드별 Read/Write 분석

**TickCount**:

- **Read**: `EPB/Main_Api.c:107,120,126,133` (태스크 스케줄러)
- **Write**: `EPB/Main_Api.c:189` (TaskScheduler_TaskCount)
- **용도**: 1ms ISR에서 증가, 주기 태스크 선택 기준

**Flag.Enable**:

- **Read**: `EPB/Main_Api.c:301` (TaskScheduler_ActivateTask)
- **Write**: `EPB/Main_Api.c:85,188,302` (초기화, ISR, 메인 루프)
- **용도**: ISR과 메인 루프 간 동기화 신호

**Flag.SystemDown**:

- **Read**: `EPB/Main_Api.c:42,228` (전원 관리 로직)
- **Write**: `EPB/Main_Api.c:36,50,86` (SetSystemDown, SetSystemOn 함수)
- **용도**: IGN OFF 상태 관리

**Flag.SystemOff**:

- **Read**: `EPB/Main_Api.c:236` (PowerOff 조건 체크)
- **Write**: `EPB/Main_Api.c:37,43,87` (SetSystemOff 함수)
- **용도**: IGN OFF 1초 이상 유지 플래그

**PowerOffCount**:

- **Read**: `EPB/Main_Api.c:238,241` (PowerOff 조건 체크)
- **Write**: `EPB/Main_Api.c:38,84,238` (초기화, 카운트 증가)
- **용도**: 전원 차단까지 카운트다운 (5ms × 800 = 4초)

**TaskCount\_\*\_Cnt** (실행 횟수):

- **Write**: `EPB/Main_Api.c:122,128,134` (ExecuteTask 함수)
- **용도**: 각 태스크의 총 실행 횟수 추적 (디버그/통계)

**근거**: `EPB/Main_Api.c` 내 각 필드 접근 코드 분석

#### 4.2.4 g_TaskControl의 역할

1. **타이밍 제어**: TickCount로 시스템 시간 관리
2. **태스크 스케줄링**: 주기별 태스크 실행 판단
3. **전원 관리**: SystemDown, PowerOff 플래그로 전원 상태 제어
4. **성능 모니터링**: 실행 횟수, 실행 시간 추적

---

### 4.3 g_MAIN 구조체 상세

#### 4.3.1 구조체 정의

**파일**: `EPB/Main_Api.h` (lines 97-103)  
**선언**: `Main_t g_MAIN;` (Main_Api.c:30)  
**크기**: 약 10 bytes (추정)

#### 4.3.2 주요 필드 목록

| 필드                 | 타입    | 용도                  | 가능한 값            |
| -------------------- | ------- | --------------------- | -------------------- |
| **SystemStatus**     | `uint8` | 시스템 상태           | INIT(0), NORMAL(1)   |
| **MtDrvStatus**      | `uint8` | 모터 드라이버 상태    | INIT(0), NORMAL(1)   |
| **PowerOffOk**       | `uint8` | 파워 오프 완료 플래그 | 0 (진행중), 1 (완료) |
| **Session**          | `uint8` | UDS 세션 정보         | 확인 필요            |
| **L9369_InitStep**   | `uint8` | L9369 초기화 단계     | 확인 필요            |
| **TLE9461_InitStep** | `uint8` | TLE9461 초기화 단계   | 확인 필요            |
| **Debug**            | `1bit`  | 디버그 플래그         | 0/1                  |

#### 4.3.3 필드별 Read/Write 분석

**SystemStatus**:

- **Read**: `EPB/Main_Api.c:194,211,255,262` (모든 태스크 스케줄러)
- **Write**: `EPB/Main_Api.c:99,259` (초기화, 상태 전이)
- **용도**: 시스템 전체 상태 관리 (INIT → NORMAL)

**MtDrvStatus**:

- **Read**: `EPB/Main_Api.c:33,197,270` (1ms ISR, 태스크)
- **Write**: `EPB/Main_Api.c:100,275,277` (초기화, L9369Init 결과)
- **용도**: 모터 드라이버 초기화 상태 추적

**PowerOffOk**:

- **Read**: `EPB/Main_Api.c:55,74` (PowerOff 함수)
- **Write**: `EPB/Main_Api.c:68` (전원 차단 완료 시)
- **용도**: 전원 차단 시퀀스 완료 표시

**L9369_InitStep**:

- **Write**: `EPB/Main_Api.c:277` (L9369Init 결과)
- **용도**: L9369 초기화 진행 상태 추적

**근거**: `EPB/Main_Api.c` 내 각 필드 접근 코드 분석

#### 4.3.4 g_MAIN의 역할

1. **시스템 상태 관리**: SystemStatus로 전체 시스템 모드 제어
2. **모터 상태 추적**: MtDrvStatus로 모터 제어 가능 여부 판단
3. **초기화 진행도**: 각 IC의 초기화 단계 추적
4. **전원 관리**: PowerOffOk로 전원 차단 완료 확인

---

### 4.4 God Object 패턴의 특징

#### 4.4.1 왜 God Object로 분류되는가?

**정의**: God Object는 **시스템의 많은 기능과 상태를 하나의 객체에 집중**시킨 안티패턴입니다.

**EPB 시스템의 특징**:

1. **광범위한 접근**:

   - g_TaskControl, g_MAIN은 거의 모든 파일에서 `extern`으로 접근
   - 모듈 간 인터페이스 없이 직접 필드 접근

2. **다양한 책임**:

   - g_TaskControl: 타이밍, 스케줄링, 전원 관리, 성능 모니터링
   - g_MAIN: 상태 관리, 초기화, 진단 세션

3. **높은 결합도**:

   - 하나의 필드를 변경하면 여러 파일에 영향
   - 예: SystemStatus 변경 시 모든 태스크 스케줄러 영향

4. **테스트 어려움**:
   - 전역 상태로 인해 단위 테스트 작성 곤란
   - Mock 객체 생성 불가능

#### 4.4.2 전역 상태 접근 패턴

**직접 접근 예시**:

```c
// EPB/Main_Api.c:228
if (g_TaskControl.Flag.SystemDown == TRUE) {
    // 전원 다운 처리
}

// EPB/Main_Api.c:211
if (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL) {
    // 정상 동작
}
```

**필드 단위 쓰기**:

```c
// EPB/Main_Api.c:189
g_TaskControl.TickCount++;

// EPB/Main_Api.c:277
g_MAIN.L9369_InitStep = L9369Init(FALSE);
```

**근거**: Main_Api.c 내 전역 구조체 직접 접근 패턴

#### 4.4.3 전역 상태 의존성 그래프 (개념)

```
g_TaskControl, g_MAIN (중심)
    ↑ 접근 ↑
    ├── TaskScheduler_*ms() (Main_Api.c)
    ├── L9369_TASK_1ms() (Spi_L9369_Api.c)
    ├── PowerOff() (Main_Api.c)
    ├── SetSystemDown/On/Off() (Main_Api.c)
    ├── CheckWakeupStatus() (Main_Api.c)
    └── (기타 다수 함수)
```

**특징**:

- 중심에서 밖으로 의존성이 퍼지는 **Star 패턴**
- 캡슐화(Encapsulation) 없음
- Getter/Setter 함수 없이 직접 접근

#### 4.4.4 전역 상태 관리의 현실적 이유 (추정)

1. **Bare-metal 환경**: RTOS 없이 단순한 구조 선호
2. **실시간 성능**: 함수 호출 오버헤드 최소화
3. **레거시 코드**: 기존 방식 유지
4. **개발 속도**: 빠른 프로토타입 개발

**주의**: 이는 현상 설명일 뿐, 개선 방향 제시가 아닙니다.

---

### 4.5 전역 구조체 초기화 흐름

```
TaskScheduler_Initialization() [EPB/Main_Api.c]
├── g_TaskControl 초기화
│   ├── TickCount = 0
│   ├── Tick = SYSTEM_TICK_5MS (5ms)
│   ├── TaskMax = TASK_5000ms (5000ms)
│   ├── Flag.Enable = FALSE
│   ├── Flag.SystemDown = TRUE (초기 상태)
│   ├── Flag.SystemOff = FALSE
│   ├── Flag.PowerOff = FALSE
│   ├── PowerOffCount = 0
│   └── 기타 카운터 초기화
│
└── g_MAIN 초기화
    ├── SystemStatus = SYSTEM_STATUS_INIT (0)
    ├── MtDrvStatus = MTDRV_STATUS_INIT (0)
    ├── PowerOffOk = 0
    ├── Debug = DEBUG_MAIN_ALIVE
    └── 기타 필드 초기화
```

**근거**: `EPB/Main_Api.c` TaskScheduler_Initialization() 함수

---

## 5. EPB 제어 흐름 상세

이 섹션에서는 **EPB(Electronic Park Brake) 모터 제어가 실제로 어떻게 수행되는지**를 10ms 태스크 기준으로 상세히 설명합니다.

### 5.1 EPB 제어의 전체 구조

```
┌─────────────────────────────────────────────────────────────────┐
│ 10ms 태스크 (TaskScheduler_10ms)                                │
│ └─ SystemStatus == NORMAL && MtDrvStatus == NORMAL              │
└──────────────────┬──────────────────────────────────────────────┘
                   ↓
┌──────────────────────────────────────────────────────────────────┐
│ 1. L9369AdcAvr()                                                 │
│    └─ 모터 전류 등 ADC 값 평균화 (노이즈 제거)                  │
└──────────────────┬──────────────────────────────────────────────┘
                   ↓
┌──────────────────────────────────────────────────────────────────┐
│ 2. EpbMotorTestLogic()                                           │
│    └─ 모터 테스트 시퀀스 실행                                    │
└──────────────────┬──────────────────────────────────────────────┘
                   ↓
┌──────────────────────────────────────────────────────────────────┐
│ 3. EpbMotorDriveProcess() ★핵심★                                │
│    ├─ ElectronicParkBrake_step() 호출 (Simulink 로직)          │
│    ├─ PWM 듀티 계산                                             │
│    └─ L9369 SPI 명령 전송                                       │
└──────────────────────────────────────────────────────────────────┘
```

**근거**: `EPB/Main_Api.c:208-224` TaskScheduler_10ms() 함수

---

### 5.2 L9369AdcAvr() - ADC 평균화

#### 5.2.1 목적

L9369 모터 드라이버 IC의 **ADC 값을 여러 번 샘플링하여 평균**을 계산합니다. 이를 통해 노이즈를 제거하고 안정적인 센서 값을 획득합니다.

#### 5.2.2 처리 대상 (추정)

| ADC 채널    | 측정 대상           | 용도                     |
| ----------- | ------------------- | ------------------------ |
| 모터 A 전류 | 좌측 모터 구동 전류 | 과전류 감지, 피드백 제어 |
| 모터 B 전류 | 우측 모터 구동 전류 | 과전류 감지, 피드백 제어 |
| 전원 전압   | L9369 공급 전압     | 전압 이상 감지           |
| 온도        | IC 내부 온도        | 과열 보호                |

#### 5.2.3 평균화 알고리즘 (추정)

```
매 10ms 호출 시:
├─ 현재 ADC 값 읽기 (SPI 통신)
├─ 버퍼에 저장 (N개 샘플 유지)
├─ 평균값 계산 (sum / N)
└─ 평균값을 제어 로직에서 사용
```

**근거**: 함수명 `L9369AdcAvr()` 및 일반적인 ADC 평균화 기법

---

### 5.3 EpbMotorTestLogic() - 모터 테스트

#### 5.3.1 목적

EPB 모터의 **테스트 시퀀스**를 실행하여 모터가 정상 동작하는지 검증합니다.

#### 5.3.2 테스트 항목 (추정)

| 테스트               | 내용                                 | 판정 기준              |
| -------------------- | ------------------------------------ | ---------------------- |
| **응답성 테스트**    | 짧은 PWM 신호 인가 후 전류 변화 확인 | 일정 시간 내 전류 상승 |
| **전류 소비 테스트** | 정격 구동 시 전류값 확인             | 규격 범위 내 전류      |
| **위치 센서 테스트** | 모터 회전 후 위치 센서 값 변화 확인  | 센서 값 변화 감지      |
| **역기전력 테스트**  | 모터 정지 시 역기전력 측정           | 정상 범위 역기전력     |

#### 5.3.3 테스트 결과 처리

- 테스트 실패 시: DTC 생성, `MtDrvStatus` 변경
- 테스트 성공 시: 정상 제어 모드 유지

**근거**: 함수명 및 일반적인 모터 테스트 절차

---

### 5.4 EpbMotorDriveProcess() - 핵심 모터 제어

#### 5.4.1 함수 위치

**파일**: `EPB/Application/MotorControl/Spi_L9369_Api.c` (추정)  
**호출**: `TaskScheduler_10ms()` → `EpbMotorDriveProcess()`

#### 5.4.2 주요 역할

이 함수는 **EPB 모터 제어의 핵심**으로, 다음 작업을 수행합니다:

1. **Simulink 로직 호출**: `ElectronicParkBrake_step()` 실행
2. **PWM 듀티 계산**: 목표 힘/위치에 따른 PWM 듀티 계산
3. **SPI 명령 전송**: L9369 IC로 PWM 명령 전송

#### 5.4.3 Call Tree (추정)

```
EpbMotorDriveProcess()
├── ElectronicParkBrake_step() [Simulink 생성]
│   ├── 입력: 센서 값, 스위치 상태, 차량 속도 등
│   ├── 처리: 상태 머신 실행, PID 제어, 힘 계산
│   └── 출력: 목표 PWM 듀티, 상태 플래그
│
├── PWM 듀티 → L9369 레지스터 값 변환
│   └── 듀티(%) → 레지스터 값 (0-255 등)
│
└── L9369_SetPWM() (SPI 통신)
    ├── MOSI 데이터 구성 (레지스터 주소 + 값)
    ├── CS 신호 Low
    ├── SPI 전송
    └── CS 신호 High
```

**근거**: `EPB/Main_Api.c:208-224` 호출 위치 및 일반적인 모터 제어 흐름

---

### 5.5 ElectronicParkBrake_step() - Simulink 로직

#### 5.5.1 파일 정보

**파일**: `EPB/AppLogic/ElectronicParkBrake.c`  
**특징**: Simulink Model-Based Design으로 자동 생성된 코드

#### 5.5.2 입력/출력 (추정)

**입력 (예상)**:

| 입력 신호          | 타입    | 출처                     |
| ------------------ | ------- | ------------------------ |
| EPB 스위치 상태    | boolean | GPIO 포트 읽기           |
| 차량 속도          | float   | CAN 메시지 수신          |
| 브레이크 페달 상태 | boolean | CAN 메시지 수신          |
| 모터 전류 피드백   | float   | L9369 ADC 평균값         |
| 모터 위치          | float   | 센서 또는 전류 기반 추정 |

**출력 (예상)**:

| 출력 신호          | 타입  | 용도                     |
| ------------------ | ----- | ------------------------ |
| 목표 PWM 듀티 (좌) | float | 좌측 모터 제어           |
| 목표 PWM 듀티 (우) | float | 우측 모터 제어           |
| EPB 상태           | enum  | Apply/Release/Hold/Error |
| 제어 모드          | enum  | Normal/Test/Emergency    |

#### 5.5.3 내부 로직 (추정)

**상태 머신**:

```
IDLE (대기)
    ↓ (스위치 입력)
APPLYING (체결 중)
    ↓ (목표 힘 도달)
APPLIED (체결 완료)
    ↓ (해제 요청)
RELEASING (해제 중)
    ↓ (위치 0 도달)
IDLE
```

**제어 알고리즘**:

- **PID 제어**: 목표 힘 vs 실제 전류 오차 기반
- **힘 필터링**: 전류 값을 힘으로 변환 (캘리브레이션 데이터 사용)
- **안전 로직**: 과전류, 타임아웃, 위치 이상 감지

**근거**: `EPB/Main_Api.c:3. Call Tree (TaskScheduler_10ms 기준)` 및 EPB 시스템 일반 동작

---

### 5.6 L9369 SPI 통신

#### 5.6.1 L9369 IC 개요

**IC 이름**: L9369 (모터 드라이버 IC)  
**제조사**: STMicroelectronics (추정)  
**통신 방식**: SPI (Serial Peripheral Interface)

#### 5.6.2 SPI 통신 프로토콜 (추정)

**기본 구조**:

```
CS (Chip Select) 신호 Low
    ↓
MOSI: [Command][Address][Data]
    ↓
MISO: [Status][Response Data]
    ↓
CS 신호 High
```

**레지스터 맵 (예시)**:

| 주소 | 레지스터   | 기능                        |
| ---- | ---------- | --------------------------- |
| 0x00 | CTRL_REG   | 제어 모드 설정              |
| 0x01 | PWM_A      | 모터 A PWM 듀티             |
| 0x02 | PWM_B      | 모터 B PWM 듀티             |
| 0x10 | STATUS_REG | 상태 플래그 (오류, 온도 등) |
| 0x20 | ADC_CURR_A | 모터 A 전류 ADC 값          |
| 0x21 | ADC_CURR_B | 모터 B 전류 ADC 값          |

#### 5.6.3 PWM 설정 시퀀스 (예시)

```c
// 1. CS 신호 Low
PORT_CAN_SBC_CS = 0;

// 2. PWM 듀티 설정 (MOSI)
SPI_Write(0x01, pwm_duty_A);  // 모터 A
SPI_Write(0x02, pwm_duty_B);  // 모터 B

// 3. 제어 모드 활성화
SPI_Write(0x00, CTRL_ENABLE);

// 4. CS 신호 High
PORT_CAN_SBC_CS = 1;
```

**근거**: `EPB/Application/MotorControl/Spi_L9369_Api.c` 파일 존재 및 일반적인 SPI 모터 드라이버 프로토콜

---

### 5.7 EPB 제어 흐름 타임라인

실제 EPB Apply(체결) 시나리오를 예시로 정리합니다.

```
T=0ms: 사용자가 EPB 스위치 ON
    ↓
T=5ms: (5ms 태스크)
    ├─ CheckPortStatus() → 스위치 상태 읽기
    └─ CAN 메시지 송신 (EPB 요청 알림)
    ↓
T=10ms: (10ms 태스크) ★제어 시작★
    ├─ L9369AdcAvr() → 현재 전류 = 0A (평균)
    ├─ EpbMotorTestLogic() → PASS
    └─ EpbMotorDriveProcess()
        ├─ ElectronicParkBrake_step()
        │   ├─ 입력: 스위치 ON, 속도 0, 전류 0A
        │   ├─ 상태 전이: IDLE → APPLYING
        │   └─ 출력: PWM 30% (초기 구동)
        └─ L9369_SetPWM(30%)
    ↓
T=20ms: (10ms 태스크 재실행)
    ├─ L9369AdcAvr() → 현재 전류 = 5A
    └─ EpbMotorDriveProcess()
        ├─ ElectronicParkBrake_step()
        │   ├─ 입력: 전류 5A (목표 10A)
        │   ├─ PID 제어: 오차 5A → PWM 증가
        │   └─ 출력: PWM 50%
        └─ L9369_SetPWM(50%)
    ↓
... (반복)
    ↓
T=500ms: 목표 힘 도달
    ├─ 현재 전류 = 10A (목표 도달)
    └─ EpbMotorDriveProcess()
        ├─ ElectronicParkBrake_step()
        │   ├─ 상태 전이: APPLYING → APPLIED
        │   └─ 출력: PWM 유지 (Hold 모드)
        └─ L9369_SetPWM(40%) [유지 듀티]
    ↓
이후: APPLIED 상태 유지 (10ms마다 모니터링)
```

**주의**: 위 타임라인은 설명을 위한 예시이며, 실제 값은 다를 수 있습니다.

---

### 5.8 EPB 제어 관련 전역 변수

EPB 제어는 다음 전역 구조체를 사용합니다:

| 전역 변수               | 역할                       | 주요 필드                 |
| ----------------------- | -------------------------- | ------------------------- |
| `g_L9369_Status`        | L9369 IC 상태              | 전류값, 온도, 오류 플래그 |
| `g_L9369_RegWrite`      | L9369 레지스터 쓰기 데이터 | PWM 듀티, 제어 모드       |
| `g_SwitchStatus`        | EPB 스위치 상태            | Apply/Release 요청        |
| `ElectronicParkBrake_Y` | Simulink 출력              | PWM 명령, EPB 상태        |

**근거**: `EPB/Application/MotorControl/Spi_L9369Data_Types.c:10`, `EPB/Application/MotorControl/Spi_L9369_Api.c:16`

---

### 5.9 EPB 제어의 특징 및 복잡성

#### 5.9.1 왜 이해하기 어려운가?

1. **Simulink 자동 생성 코드**:

   - `ElectronicParkBrake.c`는 사람이 작성한 코드가 아님
   - 주석이 부족하고 변수명이 자동 생성됨
   - 원본 Simulink 모델 없이는 로직 파악 어려움

2. **하드웨어 의존성**:

   - L9369 IC의 SPI 프로토콜 이해 필요
   - 타이밍 크리티컬 (1ms 동기화, 10ms 제어 주기)
   - 레지스터 직접 조작

3. **복잡한 제어 로직**:

   - 상태 머신 + PID 제어 + 안전 로직
   - 다양한 입력 신호 통합 (CAN, GPIO, ADC)
   - 예외 처리 (과전류, 타임아웃, 센서 오류)

4. **전역 상태 의존**:
   - g_MAIN.MtDrvStatus, g_L9369_Status 등 여러 전역 변수
   - 모듈 간 결합도 높음

**근거**: `EPB/AppLogic/ElectronicParkBrake.c` Simulink 생성 코드 특성

#### 5.9.2 디버깅 시 주의사항

1. **실시간 제약**: 10ms 주기를 놓치면 제어 불안정
2. **하드웨어 의존**: 실제 하드웨어 없이 테스트 어려움
3. **안전 크리티컬**: 모터 오동작 시 차량 안전 위험
4. **Simulink 코드**: 런타임 디버깅 제한적

---

## 6. Hardware 접근 지점 요약

이 섹션에서는 **코드가 하드웨어를 직접 제어하는 지점**을 정리합니다. EPB 시스템은 MCU 레지스터와 외부 IC를 직접 조작하여 동작합니다.

### 6.1 Hardware 접근 계층 구조

```
┌──────────────────────────────────────────────────────────────┐
│ Application Layer (ASW)                                      │
│ - ElectronicParkBrake.c (Simulink)                          │
│ - Can_Api.c, Logic_Api.c                                    │
└───────────────────┬──────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────────────────┐
│ Device Driver Layer                                          │
│ - Spi_L9369_Api.c (모터 드라이버)                           │
│ - Spi_TLE9461_Api.c (CAN SBC)                               │
│ - Spi_TC277_Api.c                                           │
└───────────────────┬──────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────────────────┐
│ Peripheral Driver Layer (MCAL)                               │
│ - Adc_Api.c (ADC 레지스터)                                  │
│ - Stm_Api.c (Timer 레지스터)                                │
│ - Dspi_Api.c (SPI 레지스터)                                 │
│ - Port_Api.c (GPIO 레지스터)                                │
└───────────────────┬──────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────────────────┐
│ Hardware (MCU Registers)                                     │
│ - STM.CH[0].CMP.R (Timer)                                   │
│ - ADC.CDR[ch].R (ADC)                                       │
│ - SIU.GPDO[pin].R (GPIO)                                    │
└──────────────────────────────────────────────────────────────┘
```

**특징**: 계층 간 명확한 인터페이스 없이 **직접 접근**이 많음

---

### 6.2 MCU 레지스터 직접 접근

#### 6.2.1 System Timer (STM) 레지스터

**파일**: `EPB/Peripheral/Timer/Stm_Api.c`  
**함수**: `STMInitialize()`, `STM_CH0_ISR()`

| 레지스터              | 접근 위치         | 접근 타입 | 목적                     |
| --------------------- | ----------------- | --------- | ------------------------ |
| `STM.CH[0].CMP.R`     | `Stm_Api.c:14`    | **Write** | 타이머 비교값 설정 (1ms) |
| `STM.CH[0].CIR.B.CIF` | `Stm_Api.c:17,28` | **Write** | 인터럽트 플래그 클리어   |
| `STM.CH[0].CCR.B.CEN` | `Stm_Api.c:18`    | **Write** | 인터럽트 활성화          |
| `STM.CNT.R`           | `Stm_Api.c:20,29` | **Write** | 타이머 카운터 리셋       |
| `STM.CR.R`            | `Stm_Api.c:21`    | **Write** | 타이머 제어 레지스터     |
| `INTC.PSR[30].R`      | `Stm_Api.c:23`    | **Write** | 인터럽트 우선순위 설정   |

**코드 예시**:

```c
// Stm_Api.c:14
STM.CH[0].CMP.R = (uint32_t)(Time3*10);  // 1ms = 64000 클럭

// Stm_Api.c:28 (ISR 내)
STM.CH[0].CIR.B.CIF = 1U;  // 인터럽트 플래그 클리어
```

**근거**: `EPB/Peripheral/Timer/Stm_Api.c` 레지스터 접근 분석

#### 6.2.2 ADC 레지스터

**파일**: `EPB/Peripheral/ADC/Adc_Api.c`  
**함수**: `ADCInitialize()`, `GetAdcValue()`

| 레지스터          | 접근 위치      | 접근 타입 | 목적                       |
| ----------------- | -------------- | --------- | -------------------------- |
| `ADC.MCR.R`       | `Adc_Api.c:13` | **Write** | ADC 제어 모드 설정         |
| `ADC.MCR.B.OWREN` | `Adc_Api.c:14` | **Write** | ADC 오버라이트 모드 활성화 |
| `ADC.CDR[ch].R`   | `Adc_Api.c:61` | **Read**  | ADC 데이터 레지스터 읽기   |

**코드 예시**:

```c
// Adc_Api.c:61
uint16 adc_value = ADC.CDR[adc_ch].R;  // ADC 값 읽기
```

**근거**: `EPB/Peripheral/ADC/Adc_Api.c:13,14,61` 레지스터 접근

#### 6.2.3 GPIO 레지스터 (매크로 기반)

**파일**: `EPB/Application/Port_Api.h`  
**매크로 정의**: GPIO 핀을 매크로로 추상화

| 매크로                 | 실제 레지스터             | 용도                         |
| ---------------------- | ------------------------- | ---------------------------- |
| `PORT_SUB_L9369_SYNC1` | `SIU.GPDO[PORT_PIN_C9].R` | L9369 동기화 신호 (1ms 토글) |
| `PORT_SUB_L9369_CTRL`  | `SIU.GPDO[PORT_PIN_A5].R` | L9369 제어 신호              |
| `PORT_SUB_L9369_RST`   | `SIU.GPDO[PORT_PIN_E3].R` | L9369 리셋 신호              |
| `PORT_CAN_SBC_CS`      | `SIU.GPDO[PORT_PIN_E7].R` | CAN SBC 칩 선택 (SPI)        |
| `PORT_SUB_UBVR_EN`     | `SIU.GPDO[?].R`           | UBVR 전원 제어               |
| `PORT_SUB_UBB_EN`      | `SIU.GPDO[?].R`           | UBB 전원 제어                |

**코드 예시**:

```c
// L9369_Sync_1ms() 내부 (추정)
PORT_SUB_L9369_SYNC1 ^= 1;  // 토글 (0→1 또는 1→0)
```

**근거**: `EPB/Application/Port_Api.h:135,136` 매크로 정의

---

### 6.3 외부 IC 제어 (SPI 통신)

EPB 시스템은 **3개의 주요 외부 IC**를 SPI로 제어합니다.

#### 6.3.1 L9369 (모터 드라이버 IC)

**파일**: `EPB/Application/MotorControl/Spi_L9369_Api.c`

| 제어 함수                | 파일 위치              | 호출 주기    | 목적                           |
| ------------------------ | ---------------------- | ------------ | ------------------------------ |
| `L9369Init()`            | 확인 필요              | 5ms (조건부) | L9369 초기화 시퀀스            |
| `CheckL9369RegStatus()`  | `Spi_L9369_Api.c:900`  | 5ms          | 레지스터 상태 체크 (오류 감지) |
| `CheckL9369Watchdog()`   | `Spi_L9369_Api.c:1557` | 20ms         | Watchdog 타이머 갱신           |
| `L9369_Sync_1ms()`       | `Spi_L9369_Api.c:1791` | 1ms (ISR)    | 동기화 신호 토글               |
| `L9369AdcAvr()`          | 확인 필요              | 10ms         | ADC 평균값 계산                |
| `EpbMotorDriveProcess()` | 확인 필요              | 10ms         | PWM 듀티 설정 (SPI 쓰기)       |

**SPI 통신 패턴**:

```
CS 신호 Low (GPIO)
    ↓
SPI 송신 (MOSI): [Command][Address][Data]
    ↓
SPI 수신 (MISO): [Status][Response]
    ↓
CS 신호 High (GPIO)
```

**근거**: `EPB/Application/MotorControl/Spi_L9369_Api.c` 함수 위치 정보

#### 6.3.2 TLE9461 (CAN SBC)

**파일**: `EPB/Application/MotorControl/Spi_TLE9461_Api.c`

| 제어 함수                | 파일 위치              | 호출 주기       | 목적               |
| ------------------------ | ---------------------- | --------------- | ------------------ |
| `TLE9461Init()`          | 확인 필요              | 5ms (초기화 시) | CAN SBC 초기화     |
| `CheckCANSBCRegStatus()` | `Spi_TLE9461_Api.c:19` | 5ms             | 레지스터 상태 체크 |
| `CANSBCWatchdog()`       | 확인 필요              | 20ms            | Watchdog 갱신      |

**TLE9461 역할**:

- CAN 트랜시버 (물리 계층)
- 전원 관리 (System Basis Chip)
- Watchdog 기능
- 웨이크업 신호 감지

**근거**: `EPB/Application/MotorControl/Spi_TLE9461_Api.c:19` 함수 위치

#### 6.3.3 TC277

**제어 함수**: `CheckTC277Status()` (파일 위치 확인 필요)  
**호출 주기**: 20ms

**역할**: 확인 필요 (외부 센서 또는 통신 모듈로 추정)

**근거**: `EPB/Main_Api.c` 내 함수 호출 패턴

---

### 6.4 CAN 통신 (하드웨어 접근)

#### 6.4.1 CAN 레지스터 접근

**파일**: `boards/EvaXPC560XB/board_irq.c`  
**함수**: `FlexCanESRClear()`

| 레지스터              | 접근 위치            | 접근 타입 | 목적                            |
| --------------------- | -------------------- | --------- | ------------------------------- |
| `CAN_0.MCR`           | `board_irq.c` (추정) | **RW**    | CAN 제어 레지스터 (Freeze/Halt) |
| `CAN_0.ESR`           | `board_irq.c` (추정) | **Write** | CAN 오류 상태 클리어            |
| `CAN_0.ESR.B.TWRNINT` | `board_irq.c` (추정) | **Write** | TX Warning 인터럽트 클리어      |
| `CAN_0.ESR.B.RWRNINT` | `board_irq.c` (추정) | **Write** | RX Warning 인터럽트 클리어      |
| `CAN_0.ESR.B.BOFFINT` | `board_irq.c` (추정) | **Write** | Bus Off 인터럽트 클리어         |

**근거**: 입력 문서에는 레지스터 접근 언급만 있음 (상세 라인 정보 불확실)

#### 6.4.2 CAN 통신 함수

**파일**: `EPB/Application/Diagnostics/Can_Api.c`

| 함수               | 호출 주기 | 목적                          |
| ------------------ | --------- | ----------------------------- |
| `TxCanMessage()`   | 5ms       | CAN 메시지 송신 (EPB 상태 등) |
| `CAN_Task()`       | 5ms       | CAN 송수신 처리               |
| `CAN_AllPolling()` | 5ms       | CAN 메일박스 폴링             |

**근거**: `EPB/Application/Diagnostics/Can_Api.c:168` TxCanMessage() 위치

---

### 6.5 Hardware 접근의 복잡성

#### 6.5.1 왜 구조적으로 복잡한가?

1. **계층 간 명확한 경계 없음**:

   - Application Layer에서 GPIO 직접 접근 (PORT 매크로)
   - Device Driver가 레지스터 직접 조작
   - 추상화 계층(HAL) 불충분

2. **하드웨어 의존적 코드 분산**:

   - 레지스터 접근이 여러 파일에 분산
   - 예: GPIO는 Port_Api.c, Spi_L9369_Api.c, Main_Api.c 등 여러 곳

3. **매크로 기반 추상화**:

   - `PORT_SUB_L9369_SYNC1` 같은 매크로 사용
   - 실제 레지스터 주소는 매크로 정의로 숨겨짐
   - 디버깅 시 추적 어려움

4. **타이밍 크리티컬**:
   - 1ms ISR 내 GPIO 토글
   - SPI 통신 타이밍 (CS 신호, 클럭)
   - ADC 샘플링 타이밍

#### 6.5.2 하드웨어 접근 패턴 예시

**패턴 1: 직접 레지스터 쓰기**

```c
// Stm_Api.c:14
STM.CH[0].CMP.R = (uint32_t)(Time3*10);
```

**패턴 2: 매크로를 통한 GPIO 접근**

```c
// Port_Api.h 정의
#define PORT_SUB_L9369_SYNC1  SIU.GPDO[PORT_PIN_C9].R

// Spi_L9369_Api.c 사용
PORT_SUB_L9369_SYNC1 = 1;  // High
```

**패턴 3: SPI를 통한 IC 제어**

```c
// Spi_L9369_Api.c (추정)
CS_Low();
SPI_Transfer(CMD_WRITE_PWM, pwm_value);
CS_High();
```

#### 6.5.3 포팅 시 수정 필요 부분

다른 MCU로 포팅 시 다음 부분을 수정해야 합니다:

| 영역               | 수정 파일         | 난이도    |
| ------------------ | ----------------- | --------- |
| **Timer 레지스터** | `Stm_Api.c`       | 높음      |
| **ADC 레지스터**   | `Adc_Api.c`       | 높음      |
| **GPIO 매크로**    | `Port_Api.h`      | 중간      |
| **SPI 레지스터**   | `Dspi_Api.c`      | 높음      |
| **CAN 레지스터**   | `board_irq.c`     | 높음      |
| **인터럽트 벡터**  | `vector_vle_eb.s` | 매우 높음 |

**근거**: 레지스터 직접 접근 코드 분석

---

### 6.6 Hardware 접근 요약표

| 하드웨어           | 접근 파일               | 접근 방식         | 주기                 |
| ------------------ | ----------------------- | ----------------- | -------------------- |
| **STM (Timer)**    | `Stm_Api.c`             | 레지스터 직접     | 초기화, ISR          |
| **ADC**            | `Adc_Api.c`             | 레지스터 직접     | 초기화, 5ms          |
| **GPIO (PORT)**    | `Port_Api.h`, 여러 파일 | 매크로 (레지스터) | 1ms, 5ms             |
| **SPI Controller** | `Dspi_Api.c`            | 레지스터 직접     | 초기화, SPI 전송 시  |
| **CAN Controller** | `board_irq.c`           | 레지스터 직접     | 초기화, ISR          |
| **L9369 (SPI)**    | `Spi_L9369_Api.c`       | SPI 통신          | 1ms, 5ms, 10ms, 20ms |
| **TLE9461 (SPI)**  | `Spi_TLE9461_Api.c`     | SPI 통신          | 5ms, 20ms            |
| **TC277 (SPI)**    | 확인 필요               | SPI 통신          | 20ms                 |

**핵심 특징**:

- **레지스터 직접 접근**이 많음 (HAL 부족)
- **타이밍 크리티컬** (1ms ISR, SPI 통신)
- **여러 파일에 분산**된 하드웨어 접근 코드

---

## 7. Hotspot 설명 (이해하기 어려운 파일)

이 섹션에서는 **코드 이해 및 유지보수 관점에서 복잡하거나 위험한 파일**을 설명합니다. 개선 방향은 제시하지 않고, **현상만** 설명합니다.

### 7.1 Hotspot 파일 TOP 5

| 순위  | 파일명                                         | 복잡도    | 이해 난이도 | 위험도    |
| ----- | ---------------------------------------------- | --------- | ----------- | --------- |
| **1** | `application/APP_Prg.c`                        | 매우 높음 | ⭐⭐⭐⭐⭐  | 높음      |
| **2** | `EPB/Main_Api.c`                               | 높음      | ⭐⭐⭐⭐    | 높음      |
| **3** | `EPB/AppLogic/ElectronicParkBrake.c`           | 중간      | ⭐⭐⭐⭐⭐  | 중간      |
| **4** | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 높음      | ⭐⭐⭐⭐    | 매우 높음 |
| **5** | `EPB/Peripheral/Timer/Stm_Api.c`               | 중간      | ⭐⭐⭐      | 높음      |

**근거**: `EPB_AsIs_코드팩트_추출.md` 섹션 7 분석 결과

---

### 7.2 #1: application/APP_Prg.c

#### 7.2.1 파일 기본 정보

**파일 크기**: 2359줄 (확인됨)  
**위치**: `application/APP_Prg.c`  
**역할**: UDS 진단 서비스, DTC 관리, 보안 액세스, CAN 콜백

#### 7.2.2 왜 이해하기 어려운가?

**1. 거대한 파일 크기**:

- 2359줄의 단일 파일에 다양한 기능 혼재
- 스크롤 하나로 전체 파악 불가능
- 파일 네비게이션 어려움

**2. 기능 혼재**:

```
APP_Prg.c 내부 기능:
├── UDS 서비스 함수 (50개 이상)
│   ├── RDBI (Read Data By Identifier) 10개+
│   ├── WDBI (Write Data By Identifier) 10개+
│   ├── Security Access (Seed & Key)
│   └── Routine Control
├── DTC 관리
│   ├── DTC_Status_Cycle() (복잡한 상태 관리)
│   └── ERAE_ClearDTC()
├── CAN 콜백 함수
│   ├── CAN_BusOffInd()
│   └── CAN_RxIndication()
└── 보안 알고리즘
    └── GenerateKeyEx() (200줄 보안 알고리즘)
```

**3. 전역 변수 직접 조작**:

- `m_astDtcLocalStatus[]` 배열 직접 접근
- `g_UDS`, `g_CAN` 구조체 직접 수정
- 여러 함수가 동일 전역 변수를 읽고 씀

**4. 복잡한 함수**:

- `GenerateKeyEx()`: 200줄 보안 알고리즘 (주석 부족)
- `DTC_Status_Cycle()`: 복잡한 DTC 상태 전이 로직
- `RDBI_Fingerprint()`, `WDBI_Fingerprint()`: 제품 식별 정보 처리

**근거**: `EPB_AsIs_코드팩트_추출.md` 섹션 7.2

#### 7.2.3 구체적인 복잡성 요소

**다중 책임**:

- **진단 서비스**: UDS 프로토콜 50개 이상 서비스
- **상태 관리**: DTC Pending → Confirmed 상태 전이
- **보안**: Seed & Key 알고리즘
- **통신**: CAN 콜백 처리

**높은 결합도**:

- EB 스택 (외부 라이브러리)와 강결합
- 다른 모듈(CAN, EEPROM, EPB 로직)에 직접 의존
- 함수 간 복잡한 호출 관계

**디버깅 어려움**:

- 2359줄 파일에서 특정 함수 찾기 어려움
- 전역 변수 조작으로 인한 부작용 추적 곤란
- UDS 서비스 동작 확인을 위해 진단 툴 필요

---

### 7.3 #2: EPB/Main_Api.c

#### 7.3.1 파일 기본 정보

**파일 크기**: 약 350줄 (추정)  
**위치**: `EPB/Main_Api.c`  
**역할**: 시스템 전체 제어 중심점 (태스크 스케줄링, 전원 관리, 상태 머신)

#### 7.3.2 왜 이해하기 어려운가?

**1. 시스템 전체 제어 중심점**:

- `main()` 함수 포함
- 모든 태스크 스케줄러 집중
- 전원 관리 로직
- 상태 전이 로직

**2. 전역 상태 집중**:

```
EPB/Main_Api.c에서 관리하는 전역 구조체:
├── g_TaskControl (15개 필드)
│   ├── TickCount
│   ├── Flag.SystemDown/SystemOff/PowerOff
│   ├── PowerOffCount
│   └── TaskCount_*_Cnt (성능 모니터링)
└── g_MAIN (6개 필드)
    ├── SystemStatus (INIT/NORMAL)
    ├── MtDrvStatus
    ├── PowerOffOk
    └── 초기화 단계 (L9369_InitStep, TLE9461_InitStep)
```

**3. 복잡한 분기 로직**:

```c
TaskScheduler_5ms() {
    if (Flag.SystemDown == TRUE) {
        // 전원 다운 처리 (6개 함수 호출)
    } else if (SystemStatus == INIT) {
        // 초기화 중 (4개 함수 호출)
    } else if (SystemStatus == NORMAL) {
        // 정상 동작 (5개 함수 호출)
        if (g_TC277.fMainfault == TRUE) {
            if (MtDrvStatus == NORMAL) {
                // ...
            } else {
                // ...
            }
        } else {
            // ...
        }
    }
}
```

**4. 타이밍 의존성**:

- 1ms ISR과 메인 루프 간 `Flag.Enable` 기반 동기화
- ISR에서 `TickCount++` → 메인 루프에서 주기 판정
- 동기화 오류 시 시스템 전체 오동작 가능

**근거**: `EPB_AsIs_코드팩트_추출.md` 섹션 7.2

#### 7.3.3 구체적인 복잡성 요소

**강결합**:

- 거의 모든 모듈이 Main_Api.c의 전역 구조체 접근
- 태스크 스케줄링, 전원 관리, 상태 전이가 한 파일에 집중
- 하나의 필드 변경 시 시스템 전체에 영향

**상태 전이 복잡도**:

```
SystemDown → SystemOff (1초 대기)
    ↓
PowerOffCount 증가 (5ms × 800 = 4초)
    ↓
PowerOff() 호출
    ├─ CAN 비활성화
    ├─ L9369 Sleep
    └─ PowerOffOk = TRUE
```

- 타이밍 기반 상태 전이 (하드코딩된 임계값)
- 상태 다이어그램 없음

**테스트 어려움**:

- 전역 상태로 인해 단위 테스트 불가능
- 초기화 순서 의존성 (SubEpbInitialize → ElectronicParkBrake_initialize → TaskScheduler_Initialization)

---

### 7.4 #3: EPB/AppLogic/ElectronicParkBrake.c

#### 7.4.1 파일 기본 정보

**파일 크기**: 442줄  
**위치**: `EPB/AppLogic/ElectronicParkBrake.c`  
**역할**: EPB 제어 로직 (Simulink Model-Based Design으로 자동 생성)

#### 7.4.2 왜 이해하기 어려운가?

**1. Simulink 자동 생성 코드**:

- 사람이 작성한 코드가 아님
- **직접 수정 금지** (Simulink 모델에서만 수정 가능)
- 주석 부족 및 자동 생성 변수명

**예시**:

```c
// 자동 생성된 변수명 (추정)
rtb_PbcApplyCurrentTemp
rtb_FilteredCurrentL
rtb_Switch_idx_0
rtDW.UnitDelay_DSTATE[3]
```

**2. 복잡한 상태 머신**:

- `StandStillManager`: 차량 정지 상태 관리
- `ParkingBrakeControl`: EPB Apply/Release 제어
- 내부 상태 머신 다수 중첩

**3. 디버깅 제약**:

- 런타임 디버깅 시 중간 변수 추적 어려움
- 블록 다이어그램 없이 코드만으로 로직 파악 불가능
- Simulink 모델 원본이 없으면 역엔지니어링 매우 어려움

**4. 하드웨어 의존도 (추정)**:

- ADC 값 읽기, SPI 명령 생성이 코드에 섞여있을 가능성
- 계층 분리 불명확

**근거**: `EPB_AsIs_코드팩트_추출.md` 섹션 7.2

#### 7.4.3 구체적인 복잡성 요소

**모델 기반 설계의 제약**:

- Simulink 모델 버전 불명
- 생성 시점 불명
- 모델 파일(.slx) 부재 시 수정 불가능

**복잡한 제어 로직**:

- 모터 피드백 제어 (전류 필터링, PID)
- 상태 머신 (Apply/Release 상태 관리)
- AVH (Automatic Valet Hold) 로직 (추정)
- 가속도/속도 필터링 (다단계)

**정적 상태 변수**:

- 블록 다이어그램 없이 상태 추적 불가능
- 초기화 시점 및 초기값 불명확

---

### 7.5 #4: EPB/Application/MotorControl/Spi_L9369_Api.c

#### 7.5.1 파일 기본 정보

**파일 크기**: 1700줄 이상 (추정)  
**위치**: `EPB/Application/MotorControl/Spi_L9369_Api.c`  
**역할**: L9369 모터 드라이버 IC 직접 제어

#### 7.5.2 왜 이해하기 어려운가?

**1. 하드웨어 직접 제어**:

- L9369 IC 레지스터 50개 이상 관리
- SPI 프로토콜 직접 구현 (MOSI/MISO)
- CRC/Checksum 계산 (추정)

**2. 안전 크리티컬**:

- 모터 제어 오류 시 **차량 주차 브레이크 오동작 위험**
- 과전류, 센서 오류 감지 로직 필수
- 오류 발생 시 복구 절차 중요

**3. 타이밍 크리티컬**:

- SPI 통신 타이밍 (CS 신호, 클럭)
- 모터 PWM 제어 타이밍 동기화 필요
- 1ms ISR에서 동기화 신호 생성

**4. 거대한 파일**:

- L9369 초기화, 레지스터 읽기/쓰기, 모터 구동, ADC 평균화, 테스트 로직 모두 포함
- 단일 책임 원칙 위배

**근거**: `EPB_AsIs_코드팩트_추출.md` 섹션 7.2

#### 7.5.3 구체적인 복잡성 요소

**다양한 기능 혼재**:

```
Spi_L9369_Api.c 내부:
├── L9369Init() - 초기화 시퀀스
├── CheckL9369RegStatus() - 레지스터 상태 체크
├── CheckL9369Watchdog() - Watchdog 갱신
├── L9369_Sync_1ms() - 1ms 동기화 신호
├── L9369AdcAvr() - ADC 평균화
├── EpbMotorTestLogic() - 모터 테스트
└── EpbMotorDriveProcess() - 모터 구동 (핵심)
```

**SPI 프로토콜 복잡성**:

- L9369 데이터시트 필수
- 레지스터 맵 이해 필요
- 오류 코드 해석 필요

**디버깅 어려움**:

- 하드웨어 없이 테스트 불가능
- 오실로스코프 필요 (SPI 신호 확인)
- 모터 동작 이상 시 원인 파악 어려움 (SPI? 모터? 센서?)

---

### 7.6 #5: EPB/Peripheral/Timer/Stm_Api.c

#### 7.6.1 파일 기본 정보

**파일 크기**: 중간  
**위치**: `EPB/Peripheral/Timer/Stm_Api.c`  
**역할**: 시스템 타이머 ISR 구현

#### 7.6.2 왜 이해하기 어려운가?

**1. 시스템 타이밍 기준점**:

- 1ms STM 인터럽트가 **전체 시스템 타이밍 기준**
- 이 ISR이 지연되면 시스템 전체가 지연됨
- 타이밍 정확도가 시스템 동작에 직접 영향

**2. ISR 내 함수 호출**:

```c
STM_CH0_ISR() {
    // 인터럽트 플래그 클리어
    STM.CH[0].CIR.B.CIF = 1U;

    // 타이머 카운터 리셋
    STM.CNT.R = 0;

    // 스케줄러 함수 호출 (위험!)
    TaskScheduler_TaskCount();

    // 모터 동기화 신호 (조건부)
    if (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL) {
        L9369_TASK_1ms();  // GPIO 토글
    }
}
```

**인터럽트 지연 가능성**:

- ISR 내에서 함수 호출 → 실행 시간 증가
- 다음 ISR 발생 전에 완료하지 못하면 시스템 오동작

**3. 하드웨어 의존**:

- STM 레지스터 직접 조작
- 포팅 시 전체 수정 필요
- MCU 데이터시트 필수

**근거**: `EPB_AsIs_코드팩트_추출.md` 섹션 7.2

#### 7.6.3 구체적인 복잡성 요소

**타이밍 계산**:

```c
// Time3 = 6400 (1ms)
STM.CH[0].CMP.R = (uint32_t)(Time3*10);  // 64000 클럭
```

- 클럭 주파수 의존적 (MCU 설정에 따라 변경)
- 타이밍 오차 누적 가능성

**ISR 우선순위**:

```c
INTC.PSR[30].R = 1;  // 우선순위 1 (높음)
```

- 다른 인터럽트와 우선순위 충돌 가능성
- 우선순위 설계 문서 필요

**포팅 난이도**:

- STM은 NXP 특정 타이머 모듈
- 다른 MCU는 다른 타이머 모듈 사용 (TIM, GPT 등)
- 레지스터 구조 완전히 다름

---

### 7.7 Hotspot 파일 간 의존성

```
APP_Prg.c (UDS/DTC)
    ↓ (CAN 메시지)
Can_Api.c
    ↓ (상태 확인)
Main_Api.c (g_MAIN, g_TaskControl)
    ↓ (태스크 실행)
Spi_L9369_Api.c (모터 제어)
    ↓ (Simulink 로직)
ElectronicParkBrake.c
    ↑
Stm_Api.c (1ms ISR)
    └─ TaskScheduler_TaskCount()
    └─ L9369_TASK_1ms()
```

**특징**:

- **Main_Api.c가 중심** (허브 역할)
- **Stm_Api.c가 타이밍 제공** (1ms ISR)
- **APP_Prg.c는 독립적** (진단 서비스)

---

### 7.8 Hotspot 종합 분석

#### 7.8.1 공통 특징

1. **도메인 전문성 필요**:

   - 모터 제어 (PID, PWM)
   - CAN 프로토콜 (UDS, DTC)
   - SPI 드라이버 (레지스터, 타이밍)

2. **상태 관리 분산**:

   - g_MAIN, g_TaskControl, g_L9369_Status 등 여러 전역 구조체
   - 상태 일관성 유지 어려움

3. **주석/문서 부족**:

   - Simulink 생성 코드: 자동 생성 주석만
   - SPI 레지스터 조작: 데이터시트 의존
   - 상태 전이: 다이어그램 없음

4. **조건문 중첩**:

   - SystemDown/INIT/NORMAL 상태별 다른 동작
   - MtDrvStatus 조건 추가
   - 3~4단계 중첩 if-else

5. **오류 처리 불명확**:
   - 일부 함수의 return 값 무시
   - 오류 발생 시 복구 절차 불명확

#### 7.8.2 이해 난이도 요약

| 파일                      | 크기       | 책임       | 하드웨어 의존 | 안전 크리티컬 | 총점  |
| ------------------------- | ---------- | ---------- | ------------- | ------------- | ----- |
| **APP_Prg.c**             | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐          | ⭐⭐⭐        | 15/20 |
| **Main_Api.c**            | ⭐⭐⭐     | ⭐⭐⭐⭐⭐ | ⭐⭐⭐        | ⭐⭐⭐⭐      | 15/20 |
| **ElectronicParkBrake.c** | ⭐⭐⭐     | ⭐⭐⭐⭐   | ⭐⭐⭐        | ⭐⭐⭐⭐⭐    | 15/20 |
| **Spi_L9369_Api.c**       | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐   | ⭐⭐⭐⭐⭐    | ⭐⭐⭐⭐⭐    | 19/20 |
| **Stm_Api.c**             | ⭐⭐       | ⭐⭐⭐     | ⭐⭐⭐⭐⭐    | ⭐⭐⭐⭐      | 14/20 |

**가장 위험한 파일**: `Spi_L9369_Api.c` (19/20점)

- 하드웨어 직접 제어 + 안전 크리티컬 + 복잡도 높음

---

## 부록: 용어 및 약어 정의

### A.1 시스템 약어

| 약어     | 전체 이름                    | 설명                                   |
| -------- | ---------------------------- | -------------------------------------- |
| **EPB**  | Electronic Park Brake        | 전자식 주차 브레이크                   |
| **ECU**  | Electronic Control Unit      | 전자 제어 장치 (MCU + 소프트웨어)      |
| **MCU**  | Microcontroller Unit         | 마이크로컨트롤러 (NXP XPC560XB)        |
| **ISR**  | Interrupt Service Routine    | 인터럽트 핸들러                        |
| **RTOS** | Real-Time Operating System   | 실시간 운영체제 (본 프로젝트는 미사용) |
| **PWM**  | Pulse Width Modulation       | 펄스 폭 변조 (모터 제어)               |
| **ADC**  | Analog-to-Digital Converter  | 아날로그→디지털 변환기                 |
| **GPIO** | General Purpose Input/Output | 범용 입출력 핀                         |
| **SPI**  | Serial Peripheral Interface  | 동기식 직렬 통신 (IC 간 통신)          |
| **CAN**  | Controller Area Network      | 차량 네트워크 통신 프로토콜            |
| **UDS**  | Unified Diagnostic Services  | 통합 진단 서비스 (ISO 14229)           |
| **DTC**  | Diagnostic Trouble Code      | 진단 오류 코드 (예: P0001)             |
| **IGN**  | Ignition                     | 시동 (IGN ON/OFF)                      |

### A.2 통신 프로토콜

| 약어     | 전체 이름                 | 설명                                  |
| -------- | ------------------------- | ------------------------------------- |
| **EB**   | Elektrobit                | CAN 스택 라이브러리 제공 회사         |
| **COM**  | Communication Manager     | CAN 신호 관리 계층                    |
| **TP**   | Transport Protocol        | 멀티프레임 CAN 통신 (긴 메시지)       |
| **DEM**  | Diagnostics Event Manager | DTC 생성 및 관리 계층                 |
| **RDBI** | Read Data By Identifier   | UDS 서비스 0x22 (데이터 읽기)         |
| **WDBI** | Write Data By Identifier  | UDS 서비스 0x2E (데이터 쓰기)         |
| **CCP**  | CAN Calibration Protocol  | 캘리브레이션 프로토콜 (측정/튜닝)     |
| **SBC**  | System Basis Chip         | CAN 트랜시버 + 전원 관리 IC (TLE9461) |

### A.3 하드웨어 모듈

| 약어       | 전체 이름                              | 설명                         |
| ---------- | -------------------------------------- | ---------------------------- |
| **STM**    | System Timer Module                    | 시스템 타이머 (1ms ISR 생성) |
| **INTC**   | Interrupt Controller                   | 인터럽트 우선순위 관리       |
| **SIU**    | System Integration Unit                | GPIO 포트 제어               |
| **DSPI**   | Deserial Serial Peripheral Interface   | SPI 컨트롤러                 |
| **ME**     | Mode Entry                             | MCU 동작 모드 제어           |
| **CGM**    | Clock Generation Module                | 클럭 생성 모듈               |
| **EEPROM** | Electrically Erasable Programmable ROM | 비휘발성 메모리              |
| **WDG**    | Watchdog                               | 시스템 감시 타이머           |

### A.4 외부 IC

| IC 이름     | 역할                           | 통신 방식 |
| ----------- | ------------------------------ | --------- |
| **L9369**   | 모터 드라이버 IC (듀얼 채널)   | SPI       |
| **TLE9461** | CAN SBC (트랜시버 + 전원 관리) | SPI       |
| **TC277**   | (용도 확인 필요)               | SPI       |

### A.5 소프트웨어 계층

| 약어     | 전체 이름                         | 설명                                     |
| -------- | --------------------------------- | ---------------------------------------- |
| **ASW**  | Application Software              | 애플리케이션 계층                        |
| **BSW**  | Basic Software                    | 기본 소프트웨어 (드라이버 계층)          |
| **MCAL** | Microcontroller Abstraction Layer | MCU 하드웨어 추상화 계층                 |
| **RTE**  | Runtime Environment               | AUTOSAR 런타임 환경 (본 프로젝트 미사용) |
| **HAL**  | Hardware Abstraction Layer        | 하드웨어 추상화 계층 (부분적)            |

### A.6 EPB 용어

| 용어               | 설명                                             |
| ------------------ | ------------------------------------------------ |
| **Apply**          | EPB 체결 (주차 브레이크 작동)                    |
| **Release**        | EPB 해제 (주차 브레이크 해제)                    |
| **Hold**           | EPB 유지 (체결 상태 유지)                        |
| **AVH**            | Automatic Valet Hold (자동 차량 정지 유지, 추정) |
| **Clamping Force** | 체결력 (모터 전류로 추정)                        |
| **Duty**           | PWM 듀티 사이클 (0-100%)                         |

### A.7 상태 값 정의

| 상태 변수         | 값                       | 의미                    |
| ----------------- | ------------------------ | ----------------------- |
| `SystemStatus`    | SYSTEM_STATUS_INIT (0)   | 시스템 초기화 중        |
|                   | SYSTEM_STATUS_NORMAL (1) | 정상 동작 상태          |
| `MtDrvStatus`     | MTDRV_STATUS_INIT (0)    | 모터 드라이버 초기화 중 |
|                   | MTDRV_STATUS_NORMAL (1)  | 모터 드라이버 정상 동작 |
| `Flag.SystemDown` | TRUE                     | IGN OFF (전원 다운 중)  |
|                   | FALSE                    | IGN ON (정상)           |

### A.8 타이밍 상수

| 상수                    | 값        | 설명                             |
| ----------------------- | --------- | -------------------------------- |
| `SYSTEM_TICK_5MS`       | 5000 usec | 시스템 기본 틱 (5ms)             |
| `TASK_5ms`              | 5         | 5ms 태스크 주기                  |
| `TASK_10ms`             | 10        | 10ms 태스크 주기                 |
| `TASK_20ms`             | 20        | 20ms 태스크 주기                 |
| `TASK_5000ms`           | 5000      | 최대 틱 카운트 (5초)             |
| `SYSTEM_POWER_OFF_TIME` | 800       | 전원 차단 카운트 (4초 = 800×5ms) |
| `Time3`                 | 6400      | 1ms 타이머 클럭 (64000 클럭/10)  |

### A.9 주요 함수 명명 규칙

| 접두사        | 의미             | 예시                  |
| ------------- | ---------------- | --------------------- |
| `Check*`      | 상태 체크 함수   | `CheckAdcStatus()`    |
| `Set*`        | 상태 설정 함수   | `SetSystemDown()`     |
| `Get*`        | 값 읽기 함수     | `GetAdcValue()`       |
| `*Initialize` | 초기화 함수      | `ADCInitialize()`     |
| `*Task*`      | 태스크 함수      | `TaskScheduler_5ms()` |
| `RDBI_*`      | UDS Read 서비스  | `RDBI_Fingerprint()`  |
| `WDBI_*`      | UDS Write 서비스 | `WDBI_Fingerprint()`  |

### A.10 파일 명명 규칙

| 패턴        | 의미                | 예시                    |
| ----------- | ------------------- | ----------------------- |
| `*_Api.c/h` | API 인터페이스 제공 | `Spi_L9369_Api.c`       |
| `*_Types.h` | 타입 정의           | `Spi_L9369Data_Types.h` |
| `*_Prg.c`   | 프로그램 로직       | `APP_Prg.c`             |
| `*_Pub.h`   | Public 헤더         | `APP_Pub.h`             |
| `*_Cbk.h`   | Callback 정의       | `APP_Cbk.h`             |

---

**문서 종료**

**작성 완료일**: 2026-01-12  
**문서 버전**: 1.0  
**상태**: As-Is 분석 완료 (리팩토링 방향 제외)

**다음 단계** (별도 문서):

- To-Be 설계 (AUTOSAR-like 리팩토링)
- 개선 로드맵
- 테스트 전략
