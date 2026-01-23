# EPB 프로젝트 As-Is 코드 팩트 추출

## 1. Boot & Runtime Flow

### 1.1 main() 진입 이후 초기화 흐름

**파일**: `EPB/Main_Api.c` / **함수**: `main()`

```
main() 시작
    ↓
SubEpbInitialize()                    // 하드웨어 초기화 함수
    ├── BoardDisableInterrupts()      // 인터럽트 비활성화
    ├── DisableWatchdog()             // 워치독 비활성화  
    ├── MEInitialize()                // MCU 초기화
    ├── CGMInitialize()               // 클럭 생성 모듈 초기화
    ├── PORTInitialize()              // GPIO 포트 초기화
    ├── STMInitialize()               // 시스템 타이머 초기화
    ├── ADCInitialize()               // ADC 초기화
    ├── SpiInitialize()               // SPI 초기화
    ├── CanInitialize()               // CAN 초기화
    └── BoardEnableInterrupts()       // 인터럽트 활성화
    ↓
ElectronicParkBrake_initialize()      // EPB 로직 초기화 (Simulink 생성)
    ↓
TaskScheduler_Initialization(TASK_5000ms)  // 태스크 스케줄러 초기화
    ↓
EepromReadAll()                       // EEPROM 데이터 읽기
    ↓
while(1) {                           // 메인 루프 시작
    TaskScheduler_ActivateTask()      // 태스크 스케줄러 실행
}
```

### 1.2 while loop 진입 이후 주기 실행 흐름

**타이머 ISR 기반 실행**:

```
STM_CH0_ISR() [EPB/Peripheral/Timer/Stm_Api.c:27]
    ↓ (1ms 주기 - Time3*10 = 64000 클럭)
TaskScheduler_TaskCount() [EPB/Main_Api.c:185]
    ├── g_TaskControl.Flag.Enable = TRUE
    └── g_TaskControl.TickCount++
    ↓
L9369_TASK_1ms() [EPB/Application/MotorControl/Spi_L9369_Api.c:1823]
    └── (MtDrvStatus == NORMAL일 때만)

메인 루프:
TaskScheduler_ActivateTask() [EPB/Main_Api.c:289]
    ↓ (Flag.Enable == TRUE일 때)
TaskScheduler_TaskCalculation() [EPB/Main_Api.c:118]
    ├── TickCount % 20 == 0 → 5ms + 10ms + 20ms 실행
    ├── TickCount % 10 == 0 → 5ms + 10ms 실행  
    └── TickCount % 5 == 0 → 5ms 실행
```

**근거**: 
- STM 타이머 설정: `STM.CH[0].CMP.R = (uint32_t)(Time3*10)` (Stm_Api.c:14)
- Time3 정의: `#define Time3 6400 // 1 ms` (typedefs.h:103)
- ISR 주석: `//1msec period, Match on System Timer Mode channel 0` (Stm_Api.c:16)

## 2. Task Map (코드 기준 확정)

### 2.1 실제 태스크 주기 및 호출 함수

| 주기 | 실행 조건 | 호출 함수 | 파일 위치 | 실행 순서 |
|------|-----------|-----------|-----------|-----------|
| **1ms** | STM ISR | `L9369_TASK_1ms()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1823` | ISR 내 |
| **5ms** | `TickCount % 5 == 0` | `TaskScheduler_5ms()` | `EPB/Main_Api.c:225` | 1 |
| **10ms** | `TickCount % 10 == 0` | `TaskScheduler_5ms()` → `TaskScheduler_10ms()` | `EPB/Main_Api.c:208` | 1→2 |
| **20ms** | `TickCount % 20 == 0` | `TaskScheduler_5ms()` → `TaskScheduler_10ms()` → `TaskScheduler_20ms()` | `EPB/Main_Api.c:191` | 1→2→3 |

### 2.2 각 태스크별 호출 함수 상세

#### 1ms 태스크 (STM ISR 내)

| 조건 | 호출 함수 | 파일 위치 | 기능 |
|------|-----------|-----------|------|
| `MtDrvStatus == NORMAL` | `L9369_Sync_1ms()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1791` | L9369 동기화 신호 토글 |

#### 5ms 태스크 (`TaskScheduler_5ms`)

**시스템 상태별 분기**:

| 조건 | 호출 함수 | 파일 위치 | 기능 |
|------|-----------|-----------|------|
| `SystemDown == TRUE` | `TxCanMessage()` | `EPB/Application/Diagnostics/Can_Api.c:168` | CAN 메시지 송신 |
| | `CheckAdcStatus()` | `EPB/Peripheral/ADC/Adc_Api.c:134` | ADC 상태 체크 |
| | `CheckPortStatus()` | `EPB/Application/Port_Api.c:135` | 포트 상태 체크 |
| | `CheckCANSBCRegStatus(SPI_CANSBC_RUN)` | `EPB/Application/MotorControl/Spi_TLE9461_Api.c:19` | CAN SBC 레지스터 체크 |
| `SystemStatus == INIT` | `CheckWakeupStatus(WAKEUP_START)` | `EPB/Main_Api.c:155` | 웨이크업 상태 체크 |
| | `TLE9461Init(FALSE)` | 확인 필요 | TLE9461 초기화 |
| `SystemStatus == NORMAL` | `CheckL9369RegStatus(SPI_L9369_RUN)` | `EPB/Application/MotorControl/Spi_L9369_Api.c:900` | L9369 레지스터 체크 |
| | `L9369Init(FALSE)` | 확인 필요 | L9369 초기화 |

#### 10ms 태스크 (`TaskScheduler_10ms`)

| 조건 | 호출 함수 | 파일 위치 | 기능 |
|------|-----------|-----------|------|
| `SystemStatus == NORMAL && MtDrvStatus == NORMAL` | `L9369AdcAvr()` | 확인 필요 | L9369 ADC 평균 처리 |
| | `EpbMotorTestLogic()` | 확인 필요 | EPB 모터 테스트 로직 |
| | `EpbMotorDriveProcess()` | 확인 필요 | EPB 모터 구동 처리 |
| `SystemStatus == NORMAL` | `Diagnostic_Task_10ms()` | 확인 필요 | 진단 태스크 |

#### 20ms 태스크 (`TaskScheduler_20ms`)

| 조건 | 호출 함수 | 파일 위치 | 기능 |
|------|-----------|-----------|------|
| `SystemStatus == NORMAL && MtDrvStatus == NORMAL` | `CheckL9369Watchdog()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1557` | L9369 워치독 체크 |
| `SystemStatus == NORMAL` | `CANSBCWatchdog()` | 확인 필요 | CAN SBC 워치독 |
| | `EcuResetCheck()` | 확인 필요 | ECU 리셋 체크 |
| | `CheckTC277Status()` | 확인 필요 | TC277 상태 체크 |

## 3. Call Tree (TaskScheduler_10ms 기준)

```
TaskScheduler_10ms() [EPB/Main_Api.c:208]
├── if (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL) [line 211]
│   ├── if (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL) [line 213]
│   │   ├── L9369AdcAvr() - L9369 ADC 값 평균 계산 및 노이즈 필터링
│   │   ├── EpbMotorTestLogic() - EPB 모터 테스트 시퀀스 실행
│   │   └── EpbMotorDriveProcess() - EPB 모터 실제 구동 명령 처리
│   └── Diagnostic_Task_10ms() [line 218] - DTC 생성 및 진단 처리
└── else [line 221] - 시스템 비정상 시 아무 작업 안함
```

**각 노드 기능 요약**:
- **L9369AdcAvr()**: 모터 드라이버 IC(L9369)의 ADC 값들을 평균화하여 노이즈 제거
- **EpbMotorTestLogic()**: EPB 모터의 테스트 시퀀스 및 상태 검증 로직 수행
- **EpbMotorDriveProcess()**: 실제 EPB 모터 구동 명령 처리 및 PWM 제어
- **Diagnostic_Task_10ms()**: DTC(Diagnostic Trouble Code) 생성, 고장 진단 등 진단 관련 처리

**근거**: `EPB/Main_Api.c:208-224` 라인 분석

## 4. Global State (God Object)

### 4.1 주요 전역 구조체 목록

| 전역 변수 | 타입 | 선언 위치 | 크기(추정) | 용도 |
|-----------|------|-----------|------------|------|
| `g_TaskControl` | `TaskControl_t` | `EPB/Main_Api.c:29` | ~100 bytes | 태스크 스케줄러 제어 |
| `g_MAIN` | `Main_t` | `EPB/Main_Api.c:30` | ~10 bytes | 메인 시스템 상태 |
| `g_TC277` | `TC277_t` | extern | 확인 필요 | TC277 통신 상태 |
| `g_CAN` | `Can_t` | `EPB/Application/Diagnostics/Can_Api.c:11` | ~20 bytes | CAN 통신 상태 |
| `g_UDS` | `Uds_t` | `EPB/Application/Diagnostics/Can_Api.c:12` | ~50 bytes | UDS 진단 상태 |
| `g_ADC` | `ADC_t` | extern | 확인 필요 | ADC 데이터 |
| `g_L9369_RegWrite` | `L9369_RegWrite_t` | `EPB/Application/MotorControl/Spi_L9369Data_Types.c:10` | 확인 필요 | L9369 레지스터 쓰기 |
| `g_L9369_Status` | `L9369_Status_t` | `EPB/Application/MotorControl/Spi_L9369_Api.c:16` | 확인 필요 | L9369 상태 |

### 4.2 g_TaskControl 필드별 Read/Write 매트릭스

| 필드 | Read 위치 | Write 위치 | 용도 |
|------|-----------|------------|------|
| `TickCount` | `EPB/Main_Api.c:107,120,126,133` | `EPB/Main_Api.c:189` | 시스템 틱 카운터 |
| `Flag.Enable` | `EPB/Main_Api.c:301` | `EPB/Main_Api.c:85,188,302` | 태스크 스케줄러 활성화 플래그 |
| `Flag.SystemDown` | `EPB/Main_Api.c:42,228` | `EPB/Main_Api.c:36,50,86` | IGN OFF 상태 플래그 |
| `Flag.SystemOff` | `EPB/Main_Api.c:236` | `EPB/Main_Api.c:37,43,87` | IGN OFF 1초 유지 플래그 |
| `PowerOffCount` | `EPB/Main_Api.c:238,241` | `EPB/Main_Api.c:38,84,238` | 파워 오프 카운터 |
| `TaskRun` | 확인 필요 | `EPB/Main_Api.c:121,127,134` | 현재 실행 중인 태스크 ID |
| `TaskCount_5ms_Cnt` | 확인 필요 | `EPB/Main_Api.c:122,128,134` | 5ms 태스크 실행 횟수 |
| `TaskCount_10ms_Cnt` | 확인 필요 | `EPB/Main_Api.c:123,129` | 10ms 태스크 실행 횟수 |
| `TaskCount_20ms_Cnt` | 확인 필요 | `EPB/Main_Api.c:124` | 20ms 태스크 실행 횟수 |

### 4.3 g_MAIN 필드별 Read/Write 매트릭스

| 필드 | Read 위치 | Write 위치 | 용도 |
|------|-----------|------------|------|
| `SystemStatus` | `EPB/Main_Api.c:194,211,255,262` | `EPB/Main_Api.c:99,259` | 시스템 상태 (INIT/NORMAL) |
| `MtDrvStatus` | `EPB/Main_Api.c:33,197,270` | `EPB/Main_Api.c:100,275,277` | 모터 드라이버 상태 |
| `PowerOffOk` | `EPB/Main_Api.c:55,74` | `EPB/Main_Api.c:68` | 파워 오프 완료 플래그 |
| `Debug` | 확인 필요 | `EPB/Main_Api.c:101` | 디버그 플래그 |
| `L9369_InitStep` | 확인 필요 | `EPB/Main_Api.c:277` | L9369 초기화 단계 |
| `Session` | 확인 필요 | `EPB/Main_Api.c:확인 필요` | UDS 세션 정보 |

**근거**: `EPB/Main_Api.h:52-95` (TaskControl_t 구조체), `EPB/Main_Api.h:97-103` (Main_t 구조체)

## 5. Hardware Touch Points

### 5.1 직접 레지스터 접근

| 레지스터 | 접근 위치 | 파일 위치 | 기능 |
|----------|-----------|-----------|------|
| `STM.CH[0].CMP.R` | `STMInitialize()` | `EPB/Peripheral/Timer/Stm_Api.c:14` | 타이머 비교값 설정 |
| `STM.CH[0].CIR.B.CIF` | `STMInitialize()`, `STM_CH0_ISR()` | `EPB/Peripheral/Timer/Stm_Api.c:17,28` | 인터럽트 플래그 클리어 |
| `STM.CH[0].CCR.B.CEN` | `STMInitialize()` | `EPB/Peripheral/Timer/Stm_Api.c:18` | 인터럽트 활성화 |
| `STM.CNT.R` | `STMInitialize()`, `STM_CH0_ISR()` | `EPB/Peripheral/Timer/Stm_Api.c:20,29` | 타이머 카운터 초기화 |
| `STM.CR.R` | `STMInitialize()` | `EPB/Peripheral/Timer/Stm_Api.c:21` | 타이머 제어 레지스터 |
| `INTC.PSR[30].R` | `STMInitialize()` | `EPB/Peripheral/Timer/Stm_Api.c:23` | 인터럽트 우선순위 설정 |
| `ADC.MCR.R` | `ADCInitialize()` | `EPB/Peripheral/ADC/Adc_Api.c:13` | ADC 제어 레지스터 |
| `ADC.MCR.B.OWREN` | `ADCInitialize()` | `EPB/Peripheral/ADC/Adc_Api.c:14` | ADC 오버라이트 활성화 |
| `ADC.CDR[adc_ch].R` | `GetAdcValue()` | `EPB/Peripheral/ADC/Adc_Api.c:61` | ADC 데이터 레지스터 읽기 |
| `PORT_SUB_UBVR_EN` | `PowerOff()` | `EPB/Main_Api.c:58` | UBVR 전원 제어 |
| `PORT_SUB_UBB_EN` | `PowerOff()` | `EPB/Main_Api.c:59` | UBB 전원 제어 |

### 5.2 GPIO 매크로 기반 접근

| 매크로 | 실제 레지스터 | 파일 위치 | 기능 |
|--------|---------------|-----------|------|
| `PORT_SUB_L9369_SYNC1` | `SIU.GPDO[PORT_PIN_C9].R` | `EPB/Application/Port_Api.h:136` | L9369 동기화 신호 |
| `PORT_SUB_L9369_CTRL` | `SIU.GPDO[PORT_PIN_A5].R` | `EPB/Application/Port_Api.h:135` | L9369 제어 신호 |
| `PORT_SUB_L9369_RST` | `SIU.GPDO[PORT_PIN_E3].R` | `EPB/Application/Port_Api.h` | L9369 리셋 신호 |
| `PORT_CAN_SBC_CS` | `SIU.GPDO[PORT_PIN_E7].R` | `EPB/Application/Port_Api.h` | CAN SBC 칩 선택 |

### 5.3 특수 IC 제어 함수

| IC 이름 | 제어 함수 | 파일 위치 | 기능 |
|---------|-----------|-----------|------|
| L9369 | `CheckL9369RegStatus()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:900` | 모터 드라이버 레지스터 체크 |
| L9369 | `CheckL9369Watchdog()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1557` | 모터 드라이버 워치독 |
| L9369 | `L9369_Sync_1ms()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1791` | 1ms 동기화 신호 생성 |
| TLE9461 | `CheckCANSBCRegStatus()` | `EPB/Application/MotorControl/Spi_TLE9461_Api.c:19` | CAN SBC 레지스터 체크 |
| TC277 | `CheckTC277Status()` | 확인 필요 | TC277 상태 체크 |

**근거**: 각 파일의 레지스터 직접 접근 코드 분석

## 6. Feature Inventory

### 6.1 시스템 관리 기능

| 기능 | 구현 위치 | 주요 함수 | 설명 |
|------|-----------|-----------|------|
| **전원 관리** | `EPB/Main_Api.c` | `SetSystemOn/Off/Down()`, `PowerOff()` | IGN ON/OFF 처리, 파워 다운 시퀀스 |
| **태스크 스케줄링** | `EPB/Main_Api.c` | `TaskScheduler_*()` | 1/5/10/20ms 주기 태스크 관리 |
| **워치독 관리** | 여러 파일 | `CheckL9369Watchdog()`, `CANSBCWatchdog()` | 시스템 안전성 보장 |
| **웨이크업 관리** | `EPB/Main_Api.c` | `CheckWakeupStatus()` | 시스템 웨이크업 상태 관리 |

### 6.2 통신 기능

| 기능 | 구현 위치 | 주요 함수 | 설명 |
|------|-----------|-----------|------|
| **CAN 통신** | `EPB/Application/Diagnostics/Can_Api.c` | `TxCanMessage()`, `CAN_Task()` | CAN 메시지 송수신 |
| **UDS 진단** | `application/APP_Prg.c` | `UDS_*()`, `RDBI_*()`, `WDBI_*()` | 진단 서비스 제공 |
| **SPI 통신** | `EPB/Peripheral/SPI/` | `SpiInitialize()` | 외부 IC와 SPI 통신 |
| **UART 통신** | `EPB/Peripheral/UART/` | 확인 필요 | 디버그/통신용 UART |

### 6.3 EPB 제어 기능

| 기능 | 구현 위치 | 주요 함수 | 설명 |
|------|-----------|-----------|------|
| **EPB 로직** | `EPB/AppLogic/ElectronicParkBrake.c` | `ElectronicParkBrake_step()` | Simulink 생성 EPB 제어 로직 |
| **모터 제어** | `EPB/Application/MotorControl/` | `EpbMotorDriveProcess()` | EPB 모터 구동 제어 |
| **센서 처리** | `EPB/Peripheral/ADC/` | `CheckAdcStatus()` | ADC 센서 데이터 처리 |
| **스위치 처리** | `EPB/Application/Port_Api.c` | `CheckPortStatus()` | EPB 스위치 상태 처리 |

### 6.4 진단 및 저장 기능

| 기능 | 구현 위치 | 주요 함수 | 설명 |
|------|-----------|-----------|------|
| **DTC 관리** | `application/APP_Prg.c` | `DTC_Status_Cycle()`, `ERAE_ClearDTC()` | 고장 코드 생성/관리 |
| **EEPROM 관리** | `EPB/Peripheral/Flash/` | `EepromReadAll()`, `SaveParamData()` | 비휘발성 데이터 저장 |
| **보안 액세스** | `application/APP_Prg.c` | `PROG_Sa1/Sa2Managment()` | Seed & Key 보안 |
| **핑거프린트** | `application/APP_Prg.c` | `RDBI_Fingerprint()`, `WDBI_Fingerprint()` | 제품 식별 정보 |

## 7. Hotspot 리스트 (분석용)

### 7.1 구조적 복잡/위험 파일 TOP 5

| 순위 | 파일명 | 복잡도 | 위험 요소 |
|------|--------|--------|-----------|
| **1** | `application/APP_Prg.c` | 매우 높음 | **2359줄 거대 파일**. UDS 진단, DTC 관리, 보안 액세스, CAN 콜백 등 **다양한 기능 혼재**. 함수 간 복잡한 의존성. |
| **2** | `EPB/Main_Api.c` | 높음 | **시스템 전체 제어 중심점**. 태스크 스케줄링, 전원 관리, 상태 머신이 **강결합**. `g_TaskControl`, `g_MAIN` 전역 상태 집중. |
| **3** | `EPB/AppLogic/ElectronicParkBrake.c` | 중간 | **Simulink 자동 생성 코드**로 **수정 불가능**. 복잡한 상태 머신과 제어 로직. 디버깅 시 **코드 추적 어려움**. |
| **4** | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 높음 | **L9369 모터 드라이버 직접 제어**. 하드웨어 의존적, **타이밍 크리티컬**. SPI 통신 오류 시 **모터 오동작 위험**. |
| **5** | `EPB/Peripheral/Timer/Stm_Api.c` | 중간 | **시스템 타이머 ISR 구현**. 1ms 주기 인터럽트로 **전체 시스템 타이밍 기준점**. ISR 내 함수 호출로 **인터럽트 지연 위험**. |

### 7.2 각 Hotspot별 상세 위험 분석

#### application/APP_Prg.c
- **파일 크기**: 2359줄 (확인됨)
- **기능 혼재**: UDS 서비스 함수 50개 이상, DTC 관리, CAN 콜백, 보안 알고리즘이 한 파일
- **전역 의존성**: `m_astDtcLocalStatus[]`, `g_UDS`, `g_CAN` 등 다수 전역 변수 직접 조작
- **복잡 함수**: `GenerateKeyEx()` (200줄 보안 알고리즘), `DTC_Status_Cycle()` (복잡한 DTC 상태 관리)

#### EPB/Main_Api.c  
- **시스템 중심점**: 모든 태스크 스케줄링과 시스템 상태 관리가 집중
- **전역 상태 집중**: `g_TaskControl` (15개 필드), `g_MAIN` (6개 필드) 구조체에 시스템 전체 상태
- **복잡한 분기**: `TaskScheduler_5ms()` 내부 3단계 if-else 분기 (SystemDown/SystemStatus 기준)
- **타이밍 의존성**: 1ms ISR과 메인 루프 간 `Flag.Enable` 기반 동기화

#### EPB/AppLogic/ElectronicParkBrake.c
- **자동 생성**: Simulink Model-Based Design으로 생성되어 **직접 수정 금지**
- **복잡한 상태 머신**: `StandStillManager`, `ParkingBrakeControl` 내부 다중 상태 머신
- **디버깅 제약**: 모델 기반 코드로 **런타임 디버깅 및 수정 제한적**

#### EPB/Application/MotorControl/Spi_L9369_Api.c
- **하드웨어 직접 제어**: L9369 모터 드라이버 IC 레지스터 직접 조작
- **안전 크리티컬**: 모터 제어 오류 시 **차량 주차 브레이크 오동작 위험**
- **타이밍 크리티컬**: SPI 통신 타이밍과 모터 PWM 제어 타이밍 동기화 필요
- **복잡한 레지스터**: L9369 IC의 50개 이상 레지스터 관리

#### EPB/Peripheral/Timer/Stm_Api.c
- **시스템 타이밍 기준**: 1ms STM 인터럽트가 전체 시스템 타이밍 기준점
- **ISR 내 함수 호출**: `TaskScheduler_TaskCount()`, `L9369_TASK_1ms()` 호출로 **인터럽트 지연 가능성**
- **하드웨어 의존**: STM 레지스터 직접 조작으로 **포팅 시 수정 필요**

**근거**: 각 파일의 라인 수, 함수 복잡도, 전역 변수 의존성, 하드웨어 접근 패턴 분석