# EPB 프로젝트 As-Is 코드 분석 문서

## 1. Boot & Runtime Flow

### 1.1 초기화 흐름 (main 함수 기준)

**파일**: `EPB/Main_Api.c` / **함수**: `main()`

```
main() 시작
    ↓
SubEpbInitialize()                    // 하드웨어 초기화
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
while(1) {                           // 메인 루프
    TaskScheduler_ActivateTask()      // 태스크 스케줄러 실행
}
```

### 1.2 TaskScheduler_Initialization 상세

**파일**: `EPB/Main_Api.c` / **함수**: `TaskScheduler_Initialization()`

- `g_TaskControl` 전역 구조체 초기화
- `g_MAIN` 전역 구조체 초기화  
- `APP_Init()` 호출 (CAN 스택 초기화)

### 1.3 Runtime 주기 실행

**파일**: `EPB/Main_Api.c` / **함수**: `TaskScheduler_ActivateTask()`

- 5ms 타이머 인터럽트에서 `TaskScheduler_TaskCount()` 호출
- `g_TaskControl.Flag.Enable = TRUE` 설정
- 메인 루프에서 `TaskScheduler_TaskCalculation()` 실행

## 2. Task Map

### 2.1 태스크 주기별 실행 함수

| 주기 | 실행 조건 | 호출 함수 | 파일 위치 | 실행 순서 |
|------|-----------|-----------|-----------|-----------|
| **5ms** | `TickCount % 5 == 0` | `TaskScheduler_5ms()` | `EPB/Main_Api.c:225` | 1 |
| **10ms** | `TickCount % 10 == 0` | `TaskScheduler_5ms()` → `TaskScheduler_10ms()` | `EPB/Main_Api.c:208` | 1→2 |
| **20ms** | `TickCount % 20 == 0` | `TaskScheduler_5ms()` → `TaskScheduler_10ms()` → `TaskScheduler_20ms()` | `EPB/Main_Api.c:191` | 1→2→3 |

### 2.2 각 태스크별 호출 함수 상세

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
├── if (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
│   ├── if (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
│   │   ├── L9369AdcAvr() - L9369 ADC 값 평균 계산
│   │   ├── EpbMotorTestLogic() - EPB 모터 테스트 로직 실행
│   │   └── EpbMotorDriveProcess() - EPB 모터 구동 처리
│   └── Diagnostic_Task_10ms() - 진단 관련 10ms 주기 태스크
└── else - 시스템이 정상 상태가 아닐 때 아무 작업 안함
```

**각 노드 기능 요약**:
- **L9369AdcAvr()**: 모터 드라이버 IC의 ADC 값들을 평균화하여 노이즈 제거
- **EpbMotorTestLogic()**: EPB 모터의 테스트 시퀀스 및 상태 검증 로직
- **EpbMotorDriveProcess()**: 실제 EPB 모터 구동 명령 처리 및 제어
- **Diagnostic_Task_10ms()**: DTC 생성, 고장 진단 등 진단 관련 처리

## 4. God Object / Global State

### 4.1 주요 전역 구조체

| 전역 변수 | 타입 | 선언 위치 | 용도 |
|-----------|------|-----------|------|
| `g_TaskControl` | `TaskControl_t` | `EPB/Main_Api.c:29` | 태스크 스케줄러 제어 |
| `g_MAIN` | `Main_t` | `EPB/Main_Api.c:30` | 메인 시스템 상태 |
| `g_TC277` | `TC277_t` | extern | TC277 통신 상태 |
| `g_CAN` | `Can_t` | `EPB/Application/Diagnostics/Can_Api.c:11` | CAN 통신 상태 |
| `g_UDS` | `Uds_t` | `EPB/Application/Diagnostics/Can_Api.c:12` | UDS 진단 상태 |
| `g_ADC` | `ADC_t` | extern | ADC 데이터 |
| `g_L9369_RegWrite` | `L9369_RegWrite_t` | `EPB/Application/MotorControl/Spi_L9369Data_Types.c:10` | L9369 레지스터 쓰기 |
| `g_L9369_Status` | `L9369_Status_t` | `EPB/Application/MotorControl/Spi_L9369_Api.c:16` | L9369 상태 |

### 4.2 g_TaskControl 필드별 Read/Write 매트릭스

| 필드 | Read 위치 | Write 위치 | 용도 |
|------|-----------|------------|------|
| `TickCount` | `EPB/Main_Api.c:107,120,126,133` | `EPB/Main_Api.c:189` | 시스템 틱 카운터 |
| `Flag.Enable` | `EPB/Main_Api.c:301` | `EPB/Main_Api.c:85,188,302` | 태스크 스케줄러 활성화 플래그 |
| `Flag.SystemDown` | `EPB/Main_Api.c:42,228` | `EPB/Main_Api.c:36,50,86` | IGN OFF 상태 플래그 |
| `Flag.SystemOff` | `EPB/Main_Api.c:236` | `EPB/Main_Api.c:37,43,87` | IGN OFF 1초 유지 플래그 |
| `PowerOffCount` | `EPB/Main_Api.c:238,241` | `EPB/Main_Api.c:38,84,238` | 파워 오프 카운터 |
| `TaskRun` | 확인 필요 | `EPB/Main_Api.c:121,127,134` | 현재 실행 중인 태스크 |
| `TaskCount_*_Cnt` | 확인 필요 | `EPB/Main_Api.c:122-124,128-129,134` | 각 태스크 실행 횟수 |

### 4.3 g_MAIN 필드별 Read/Write 매트릭스

| 필드 | Read 위치 | Write 위치 | 용도 |
|------|-----------|------------|------|
| `SystemStatus` | `EPB/Main_Api.c:194,211,255,262` | `EPB/Main_Api.c:99,259` | 시스템 상태 (INIT/NORMAL) |
| `MtDrvStatus` | `EPB/Main_Api.c:33,197,270` | `EPB/Main_Api.c:100,275,277` | 모터 드라이버 상태 |
| `PowerOffOk` | `EPB/Main_Api.c:55,74` | `EPB/Main_Api.c:68` | 파워 오프 완료 플래그 |
| `Debug` | 확인 필요 | `EPB/Main_Api.c:101` | 디버그 플래그 |
| `L9369_InitStep` | 확인 필요 | `EPB/Main_Api.c:277` | L9369 초기화 단계 |

## 5. Hardware Touch Points

### 5.1 직접 레지스터 접근

| 접근 위치 | 레지스터/매크로 | 파일 위치 | 기능 |
|-----------|-----------------|-----------|------|
| `PowerOff()` | `PORT_SUB_UBVR_EN` | `EPB/Main_Api.c:58` | UBVR 전원 제어 |
| `PowerOff()` | `PORT_SUB_UBB_EN` | `EPB/Main_Api.c:59` | UBB 전원 제어 |
| Port_Api.h | `SIU.GPDO[PORT_PIN_*].R` | `EPB/Application/Port_Api.h:135-137` | GPIO 출력 제어 |
| Port_Api.h | `SIU.GPDI[PORT_PIN_*].R` | `EPB/Application/Port_Api.h` | GPIO 입력 읽기 |
| Port_Api.h | `SIU.PCR[PORT_PIN_*].R` | `EPB/Application/Port_Api.h` | 핀 설정 레지스터 |

### 5.2 하드웨어 드라이버 호출

| 함수명 | 파일 위치 | 호출 위치 | 기능 |
|--------|-----------|-----------|------|
| `MEInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | MCU 초기화 |
| `CGMInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | 클럭 초기화 |
| `PORTInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | 포트 초기화 |
| `STMInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | 타이머 초기화 |
| `ADCInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | ADC 초기화 |
| `SpiInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | SPI 초기화 |
| `CanInitialize()` | 확인 필요 | `EPB/Main_Api.c:SubEpbInitialize` | CAN 초기화 |

### 5.3 특수 IC 제어

| IC 이름 | 제어 함수 | 파일 위치 | 기능 |
|---------|-----------|-----------|------|
| L9369 | `CheckL9369RegStatus()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:900` | 모터 드라이버 레지스터 체크 |
| L9369 | `CheckL9369Watchdog()` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1557` | 모터 드라이버 워치독 |
| TLE9461 | `CheckCANSBCRegStatus()` | `EPB/Application/MotorControl/Spi_TLE9461_Api.c:19` | CAN SBC 레지스터 체크 |
| TC277 | `CheckTC277Status()` | 확인 필요 | TC277 상태 체크 |

## 6. Feature Inventory

### 6.1 시스템 관리 기능

| 기능 | 구현 위치 | 주요 함수 | 설명 |
|------|-----------|-----------|------|
| **전원 관리** | `EPB/Main_Api.c` | `SetSystemOn/Off/Down()`, `PowerOff()` | IGN ON/OFF 처리, 파워 다운 시퀀스 |
| **태스크 스케줄링** | `EPB/Main_Api.c` | `TaskScheduler_*()` | 5/10/20ms 주기 태스크 관리 |
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

## 7. Dependency Graph (텍스트)

```
EPB/Main_Api.c
├── depends on → Watchdog_Api.h
├── depends on → Me_Api.h  
├── depends on → Cgm_Api.h
├── depends on → Port_Api.h
├── depends on → Stm_Api.h
├── depends on → Adc_Api.h
├── depends on → Dspi_Api.h
├── depends on → Can_Api.h
├── depends on → UDS_Api.h
├── depends on → Logic_Api.h
├── depends on → ElectronicParkBrake.h
└── depends on → FailSafe_Api.h

application/APP_Prg.c  
├── depends on → EB_Prj.h
├── depends on → Board.h
├── depends on → CCP.h
├── depends on → common.h
├── depends on → Adc_Api.h
├── depends on → DEM_Priv.h
├── depends on → Diagnostics_CAN.h
├── depends on → Dspi_Api.h
├── depends on → Spi_ExtEEPROM_Api.h
├── depends on → Can_Api.h
└── depends on → ElectronicParkBrake.h

EPB/AppLogic/ElectronicParkBrake.c
├── depends on → ElectronicParkBrake.h
├── depends on → ElectronicParkBrake_private.h
├── depends on → ParkingBrakeControl.h
└── depends on → StandStillManager.h

EPB/Application/*/
├── 각 API 파일들이 common.h에 의존
├── 하드웨어 관련 파일들이 Port_Api.h에 의존
└── 진단 관련 파일들이 Can_Api.h에 의존
```

## 8. Hotspot 리스트 (위험/복잡도 기준)

### 8.1 TOP 5 위험/복잡 파일

| 순위 | 파일명 | 위험도 | 이유 |
|------|--------|--------|------|
| **1** | `application/APP_Prg.c` | 매우 높음 | **2359줄의 거대한 파일**. UDS 진단, DTC 관리, 보안 액세스, CAN 콜백 등 **다양한 기능이 한 파일에 혼재**. 함수 간 의존성 복잡. |
| **2** | `EPB/Main_Api.c` | 높음 | **시스템 전체 제어의 중심점**. 태스크 스케줄링, 전원 관리, 상태 머신이 **강결합**되어 있음. `g_TaskControl`, `g_MAIN` 전역 상태 의존성 높음. |
| **3** | `EPB/AppLogic/ElectronicParkBrake.c` | 중간 | **Simulink 자동 생성 코드**로 **수정 불가**. 복잡한 상태 머신과 제어 로직. 디버깅 시 **코드 추적 어려움**. |
| **4** | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 높음 | **L9369 모터 드라이버 제어**. 하드웨어 직접 제어, **타이밍 크리티컬**, SPI 통신 오류 시 **모터 오동작 위험**. |
| **5** | `EPB/common.h` | 중간 | **전역 Flag_Type 구조체들 다수 선언**. 시스템 전반에서 **공유되는 상태 변수들**. 변경 시 **광범위한 영향**. |

### 8.2 각 Hotspot별 상세 위험 요소

#### application/APP_Prg.c
- **라인 수**: 2359줄 (확인됨)
- **기능 혼재**: UDS 서비스, DTC 관리, CAN 콜백, 보안 액세스가 한 파일에 존재
- **전역 상태 의존**: `m_astDtcLocalStatus`, `g_UDS`, `g_CAN` 등 다수 전역 변수 사용
- **복잡한 함수**: `GenerateKeyEx()` (보안 알고리즘), `DTC_Status_Cycle()` (DTC 로직)

#### EPB/Main_Api.c  
- **시스템 중심점**: 모든 태스크 스케줄링과 시스템 상태 관리
- **전역 상태 집중**: `g_TaskControl`, `g_MAIN` 구조체에 시스템 전체 상태 집중
- **복잡한 분기**: `TaskScheduler_5ms()` 내부의 다중 if-else 분기
- **타이밍 의존성**: 5ms 인터럽트와 메인 루프 간 동기화

#### EPB/AppLogic/ElectronicParkBrake.c
- **자동 생성 코드**: Simulink에서 생성되어 **직접 수정 불가**
- **복잡한 상태 머신**: StandStillManager, ParkingBrakeControl 내부 상태 복잡
- **디버깅 어려움**: 모델 기반 코드로 **런타임 디버깅 제한적**

#### EPB/Application/MotorControl/Spi_L9369_Api.c
- **하드웨어 직접 제어**: L9369 모터 드라이버 IC 직접 제어
- **안전 크리티컬**: 모터 제어 오류 시 **차량 안전 위험**
- **타이밍 크리티컬**: SPI 통신 타이밍과 모터 제어 타이밍
- **복잡한 레지스터 맵**: L9369 IC의 다수 레지스터 관리

#### EPB/common.h
- **전역 플래그 집중**: `ubFlag_Fault1~6`, `ubMutex_Flag1~7` 등 **다수 전역 플래그**
- **시스템 전반 영향**: 모든 모듈에서 include하여 **변경 시 전체 빌드 필요**
- **타입 정의 혼재**: 플래그 정의와 함수 선언이 한 파일에 존재