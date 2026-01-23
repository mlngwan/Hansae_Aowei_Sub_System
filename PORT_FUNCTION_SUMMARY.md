# Port_Api.c 함수 역할 요약

## MCAL Layer (4개 함수)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `Mcal_Port_ConfigurePads()` | 모든 PORT 핀의 입출력 모드 및 기능 설정 (PCR 레지스터 직접 접근) | MCAL |
| `Mcal_Port_SetInitialStates()` | 모든 PORT 핀의 초기 출력 상태 설정 (HIGH/LOW) | MCAL |
| `Mcal_Port_ReadPin_SbcFault()` | SBC Fault 핀 상태 읽기 (레지스터 직접 접근) | MCAL |
| `Mcal_Port_WritePin_L9369Ctrl()` | L9369 모터 드라이버 제어 핀 쓰기 (레지스터 직접 접근) | MCAL |

**책임**: 레지스터/핀 직접 접근만 수행

---

## BSW Layer (3개 함수)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `Bsw_Port_GetIgnitionState()` | IGN 전압을 논리 상태(ON/OFF)로 변환 (Physical → Logical) | BSW |
| `Bsw_Port_GetSbcFaultState()` | SBC Fault 핀 상태 읽기 서비스 (MCAL 래핑) | BSW |
| `Bsw_Port_SetMotorDriverControl()` | L9369 모터 드라이버 Enable/Disable 제어 서비스 (MCAL 래핑) | BSW |

**책임**: HW 독립 서비스 제공 (Get/Set), Physical → Logical 변환

---

## FS Layer (3개 함수)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `Fs_Port_DeriveSystemStateFromIgn()` | IGN 상태로부터 시스템 상태(ON/DOWN/OFF) 결정 (2초 OFF 정책 적용) | FS |
| `Fs_Port_DeriveMotorCtrlCmd()` | TC277 통신 상태로부터 모터 제어 명령(ENABLE/DISABLE) 결정 | FS |
| `Fs_Port_DeriveSbcFaultLogical()` | SBC Fault 논리 상태 산출 (BSW 서비스 호출) | FS |

**책임**: 정책 적용, 상태/명령 결정 후 반환 (실행 금지)

---

## ASW Layer (6개 함수)

### ASW Apply 함수 (3개)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `Asw_System_ApplySystemState()` | FS가 결정한 시스템 상태를 실행 (SetSystemOn/Down/Off 호출) | ASW |
| `Asw_Port_ApplyMotorCtrlCmd()` | FS가 결정한 모터 제어 명령을 실행 (BSW 서비스 호출) | ASW |
| `Asw_Port_ApplySbcFaultStatus()` | FS가 산출한 SBC Fault 상태를 전역 변수에 저장 | ASW |

**책임**: FS 반환값 기반 실행만 수행 (정책/판단 금지)

### EPB Domain 함수 (2개)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `SetEpbSwitch()` | EPB 스위치 상태 설정 및 Apply/Release 판단 (RAM 비트필드 조작) | ASW |
| `GetEpbSwitch()` | EPB 스위치 상태 읽기 (RAM 비트필드 조회) | ASW |

**책임**: EPB 도메인 로직 (순수 RAM 조작, HW 접근 없음)

### Integration 함수 (1개)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `CheckPortStatus()` | PORT 상태 모니터링 및 제어 (표준 패턴: FS Derive → ASW Apply) | ASW |

**책임**: FS Derive → ASW Apply 패턴 통합 실행

---

## 레거시 호환 함수 (3개)

| 함수명 | 역할 | 레이어 |
|--------|------|--------|
| `PORTInitialize()` | PORT 모듈 초기화 (Pad 설정 + 초기 상태 설정) | Wrapper |
| `SetPadConfiguration()` | Pad 설정 래퍼 함수 (레거시 호환) | Wrapper |
| `SetPortInitialize()` | Port 초기화 래퍼 함수 (레거시 호환) | Wrapper |

**책임**: 레거시 코드 호환성 유지

---

## 레이어별 함수 개수

| 레이어 | 함수 개수 | 비율 |
|--------|-----------|------|
| MCAL | 4 | 21% |
| BSW | 3 | 16% |
| FS | 3 | 16% |
| ASW | 6 | 32% |
| Wrapper | 3 | 16% |
| **합계** | **19** | **100%** |

---

## 표준 패턴 흐름

```
CheckPortStatus() [ASW Integration]
│
├─ 1. Fs_Port_DeriveSystemStateFromIgn() [FS]
│   ├─ Bsw_Port_GetIgnitionState() [BSW]
│   │   └─ g_ADC.Physical.Power.IGN [Data]
│   └─ Returns: Fs_SystemState_t
│
├─ 2. Asw_System_ApplySystemState() [ASW]
│   └─ SetSystemOn/Down/Off() [Main]
│
├─ 3. Fs_Port_DeriveMotorCtrlCmd() [FS]
│   ├─ g_TC277.fMainfault [Data]
│   └─ Returns: Fs_MotorCtrlCmd_t
│
├─ 4. Asw_Port_ApplyMotorCtrlCmd() [ASW]
│   ├─ Bsw_Port_SetMotorDriverControl() [BSW]
│   │   └─ Mcal_Port_WritePin_L9369Ctrl() [MCAL]
│   │       └─ PORT_SUB_L9369_CTRL [Register]
│
└─ 5. Asw_Port_ApplySbcFaultStatus() [ASW]
    └─ Fs_Port_DeriveSbcFaultLogical() [FS]
        └─ Bsw_Port_GetSbcFaultState() [BSW]
            └─ Mcal_Port_ReadPin_SbcFault() [MCAL]
                └─ PORT_SUB_EPB_SBC_FAULT [Register]
```

---

## 함수 호출 관계

### 초기화 경로
```
main()
└─ SubEpbInitialize()
    └─ PORTInitialize()
        ├─ SetPadConfiguration()
        │   └─ Mcal_Port_ConfigurePads()
        └─ SetPortInitialize()
            └─ Mcal_Port_SetInitialStates()
```

### 주기 실행 경로 (5ms)
```
TaskScheduler_5ms()
└─ CheckPortStatus()
    ├─ Fs_Port_DeriveSystemStateFromIgn()
    │   └─ Bsw_Port_GetIgnitionState()
    ├─ Asw_System_ApplySystemState()
    ├─ Fs_Port_DeriveMotorCtrlCmd()
    ├─ Asw_Port_ApplyMotorCtrlCmd()
    │   └─ Bsw_Port_SetMotorDriverControl()
    │       └─ Mcal_Port_WritePin_L9369Ctrl()
    └─ Asw_Port_ApplySbcFaultStatus()
        └─ Fs_Port_DeriveSbcFaultLogical()
            └─ Bsw_Port_GetSbcFaultState()
                └─ Mcal_Port_ReadPin_SbcFault()
```

---

## 함수별 상세 설명

### MCAL Layer

#### 1. Mcal_Port_ConfigurePads()
- **역할**: 모든 PORT 핀의 입출력 모드 및 기능 설정
- **접근**: PCR 레지스터 직접 접근
- **설정 항목**:
  - 입출력 모드 (INPUT/OUTPUT)
  - Pull-up/Pull-down
  - Alternate 기능
  - Analog 모드
- **호출 시점**: 초기화 시 1회

#### 2. Mcal_Port_SetInitialStates()
- **역할**: 모든 PORT 핀의 초기 출력 상태 설정
- **접근**: PORT 레지스터 직접 접근
- **설정 항목**:
  - L9369 SPI 핀 (CS, MOSI, SCLK, WAU, RST)
  - CAN/EEPROM SPI 핀
  - TC277 SPI 핀
  - 전원 Enable 핀 (UBVR_EN, UBB_EN)
- **호출 시점**: 초기화 시 1회

#### 3. Mcal_Port_ReadPin_SbcFault()
- **역할**: SBC Fault 핀 상태 읽기
- **접근**: PORT_SUB_EPB_SBC_FAULT 레지스터 직접 읽기
- **반환**: uint8 (0=정상, 1=고장)
- **호출 시점**: 5ms 주기

#### 4. Mcal_Port_WritePin_L9369Ctrl()
- **역할**: L9369 모터 드라이버 제어 핀 쓰기
- **접근**: PORT_SUB_L9369_CTRL 레지스터 직접 쓰기
- **파라미터**: uint8 state (0=Disable, 1=Enable)
- **호출 시점**: 5ms 주기

---

### BSW Layer

#### 1. Bsw_Port_GetIgnitionState()
- **역할**: IGN 전압을 논리 상태로 변환
- **입력**: g_ADC.Physical.Power.IGN (mV 단위)
- **출력**: uint8 (TRUE=IGN ON, FALSE=IGN OFF)
- **임계값**: IGN_ON (6000mV)
- **호출 시점**: 5ms 주기

#### 2. Bsw_Port_GetSbcFaultState()
- **역할**: SBC Fault 핀 상태 읽기 서비스
- **구현**: Mcal_Port_ReadPin_SbcFault() 래핑
- **반환**: uint8 (fault state)
- **호출 시점**: 5ms 주기

#### 3. Bsw_Port_SetMotorDriverControl()
- **역할**: L9369 모터 드라이버 Enable/Disable 제어
- **구현**: Mcal_Port_WritePin_L9369Ctrl() 래핑
- **파라미터**: uint8 enable (TRUE/FALSE)
- **호출 시점**: 5ms 주기

---

### FS Layer

#### 1. Fs_Port_DeriveSystemStateFromIgn()
- **역할**: IGN 상태로부터 시스템 상태 결정
- **입력**: Bsw_Port_GetIgnitionState() 결과
- **출력**: Fs_SystemState_t (FS_SYS_ON/DOWN/OFF)
- **정책**:
  - IGN ON → FS_SYS_ON, IgnOffCount=0
  - IGN OFF → FS_SYS_DOWN
  - IGN OFF 2초(200*5ms) 이상 → FS_SYS_OFF
- **호출 시점**: 5ms 주기

#### 2. Fs_Port_DeriveMotorCtrlCmd()
- **역할**: TC277 통신 상태로부터 모터 제어 명령 결정
- **입력**: g_TC277.fMainfault (TRUE=정상, FALSE=고장)
- **출력**: Fs_MotorCtrlCmd_t (FS_MOTOR_CTRL_ENABLE/DISABLE)
- **정책**:
  - fMainfault==TRUE → ENABLE
  - fMainfault==FALSE → DISABLE
- **호출 시점**: 5ms 주기

#### 3. Fs_Port_DeriveSbcFaultLogical()
- **역할**: SBC Fault 논리 상태 산출
- **입력**: Bsw_Port_GetSbcFaultState() 결과
- **출력**: uint8 (fault state)
- **정책**: 없음 (단순 전달)
- **호출 시점**: 5ms 주기

---

### ASW Layer

#### 1. Asw_System_ApplySystemState()
- **역할**: FS가 결정한 시스템 상태를 실행
- **입력**: Fs_SystemState_t
- **실행**:
  - FS_SYS_ON → SetSystemOn()
  - FS_SYS_DOWN → SetSystemDown()
  - FS_SYS_OFF → SetSystemOff()
- **호출 시점**: 5ms 주기

#### 2. Asw_Port_ApplyMotorCtrlCmd()
- **역할**: FS가 결정한 모터 제어 명령을 실행
- **입력**: Fs_MotorCtrlCmd_t
- **실행**: Bsw_Port_SetMotorDriverControl() 호출
- **호출 시점**: 5ms 주기

#### 3. Asw_Port_ApplySbcFaultStatus()
- **역할**: FS가 산출한 SBC Fault 상태를 전역 변수에 저장
- **입력**: uint8 faultLogical
- **실행**: g_PORT.SUB_EPB_SBC_FAULT = faultLogical
- **호출 시점**: 5ms 주기

#### 4. SetEpbSwitch()
- **역할**: EPB 스위치 상태 설정 및 Apply/Release 판단
- **입력**: uint8 index (EPB_SW1~7), uint8 bOnOff
- **실행**:
  - 비트필드 업데이트
  - Apply/Release 상태 판단
- **접근**: RAM 비트필드만 (HW 접근 없음)
- **호출 시점**: 이벤트 기반

#### 5. GetEpbSwitch()
- **역할**: EPB 스위치 상태 읽기
- **입력**: uint8 index (EPB_SW1~7)
- **출력**: uint8 (0/1)
- **접근**: RAM 비트필드만
- **호출 시점**: 이벤트 기반

#### 6. CheckPortStatus()
- **역할**: PORT 상태 모니터링 및 제어 통합
- **패턴**: FS Derive → ASW Apply
- **실행 순서**:
  1. FS derives system state
  2. ASW applies system state
  3. FS derives motor control command
  4. ASW applies motor control command
  5. FS derives and ASW applies SBC fault status
- **호출 시점**: 5ms 주기 (TaskScheduler_5ms)

---

## 표준 패턴 적용 요약

### FS Derive 함수 (3개)
- `Fs_Port_DeriveSystemStateFromIgn()`
- `Fs_Port_DeriveMotorCtrlCmd()`
- `Fs_Port_DeriveSbcFaultLogical()`

**특징**:
- 상태/명령을 결정(derive)만 수행
- 반환값으로 결정 전달
- 실행 함수 직접 호출 금지

### ASW Apply 함수 (3개)
- `Asw_System_ApplySystemState()`
- `Asw_Port_ApplyMotorCtrlCmd()`
- `Asw_Port_ApplySbcFaultStatus()`

**특징**:
- FS 반환값을 소비하여 실행만 수행
- 정책/판단 금지
- FS 결정을 신뢰하고 적용

### Integration 함수 (1개)
- `CheckPortStatus()`

**특징**:
- FS Derive → ASW Apply 패턴 통합
- 5ms 주기 실행
- 향후 모든 모듈에 적용되는 표준 패턴

---

## 결론

Port_Api.c는 총 19개 함수로 구성되며, MCAL/BSW/FS/ASW 레이어별로 명확히 분리되어 있습니다. 표준 패턴(FS Derive / ASW Apply)을 완벽히 구현하여 향후 모든 모듈 리팩토링의 참조로 사용 가능합니다.
