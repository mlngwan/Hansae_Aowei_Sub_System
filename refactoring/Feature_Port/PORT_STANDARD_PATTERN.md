# PORT 표준 패턴 - FS Derive / ASW Apply 아키텍처

## 개요

**패턴명**: FS Derive / ASW Apply Standard  
**적용 대상**: Port_Api, Motor Control, Power Management 등 시스템 정책 모듈  
**목표**: 레이어 명확화, 테스트 용이성 향상, 정책 재사용성 증대  
**상태**: Port_Api 리팩터링에서 검증된 표준 패턴

---

## 1. 표준 아키텍처 규칙

### 1.1 레이어별 책임 정의

#### MCAL (Microcontroller Abstraction Layer)

```
책임: 하드웨어 직접 접근 (Wiring)
- 레지스터 읽기/쓰기
- GPIO 핀 제어
- ADC 샘플링
- 하드웨어 주소 매핑

제약:
  ❌ 비즈니스 로직 금지
  ❌ 조건문 금지
  ❌ 상태 판단 금지
  ❌ 정책 결정 금지
```

**Port_Api 적용**:

```c
/* MCAL: Raw HW access only */
void Mcal_Port_ConfigurePads(void) {
    PCR_SUB_UBB_EN = PORT_PINCFG_OUTPUT_MODE;     // 레지스터 쓰기
    PCR_SUB_UBVR_EN = PORT_PINCFG_OUTPUT_MODE;    // 레지스터 쓰기
}
```

#### BSW (Basic Software)

```
책임: 하드웨어 추상화 서비스 (Signal Interpretation)
- 원시값(Raw) → 의미 신호(Logical) 변환
- 센서 값 해석
- 액추에이터 제어 명령 제공
- 단위 변환

제약:
  ❌ 정책 결정 금지
  ❌ 시스템 상태 전이 금지
  ❌ 외부 모듈 상태 변경 금지
  ❌ 상태 머신 금지
```

**Port_Api 적용**:

```c
/* BSW: Signal interpretation only */
uint8 Bsw_Port_GetIgnitionState(void) {
    if(g_ADC.Physical.Power.IGN > IGN_ON) {
        return TRUE;   // 신호 해석만
    } else {
        return FALSE;
    }
}
```

#### FS (Functional Safety)

```
책임: 시스템 상태 도출 및 정책 결정 (Derive)
- Physical → Logical → State 변환
- 센서 데이터 기반 상태 머신
- 시스템 정책 판단
- 명령 생성 (ASW에 전달용)

제약:
  ✅ ASW 함수 호출 허용 (via callbacks/outputs)
  ✅ 상태 변수 관리 허용
  ✅ 타이밍 로직 허용
  ❌ 비-시스템 정책 금지 (ASW 도메인 로직)
```

**Port_Api 적용**:

```c
/* FS: System state derivation and policy */
void Fs_Port_ManageIgnitionState(void) {
    if(Bsw_Port_GetIgnitionState()) {
        /* 정책 결정: IGN ON → SetSystemOn() */
        SetSystemOn();
        g_PORT.IgnOffCount = 0;
    } else {
        SetSystemDown();
        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
            SetSystemOff();  /* 2초 타임아웃 정책 */
        }
    }
}
```

#### ASW (Application Software)

```
책임: 정책 적용 (Apply) 및 애플리케이션 로직
- FS 도출 결과 기반 액션 실행
- 도메인 특화 로직 (EPB, Motor 등)
- 상태 이력 관리
- 사용자 인터페이스

제약:
  ✅ 모든 비즈니스 로직 허용
  ✅ 상태 머신 허용
  ❌ 하드웨어 직접 접근 금지 (MCAL/BSW 경유 필수)
```

**Port_Api 적용**:

```c
/* ASW: EPB application logic (hardware-independent) */
void SetEpbSwitch(uint8 index, uint8 bOnOff) {
    SwitchStatus_t *pStatus = (SwitchStatus_t*)&g_SwitchStatus;

    /* EPB 도메인 지식: NEUTRAL/APPLY/RELEASE 인코딩 */
    SWITCH_EPB_SW1(pStatus) = bOnOff;
    SWITCH_EPB_STATUS(pStatus) = epbStatus;  /* RAM 기반만 */
}
```

### 1.2 표준 데이터 흐름

#### HW → Sensor → FS → ASW → HW Control

```
┌─────────────────────────────────────────────────────────────────┐
│ Hardware Layer (Sensors, Actuators)                            │
└─────────────────────────────────────────────────────────────────┘
                              ↓ (Read)
┌─────────────────────────────────────────────────────────────────┐
│ MCAL: Raw Values                                                │
│ - GPIO Pins (HIGH/LOW)                                          │
│ - ADC Samples (0-1023)                                          │
│ - Register States                                               │
└─────────────────────────────────────────────────────────────────┘
                              ↓ (Convert)
┌─────────────────────────────────────────────────────────────────┐
│ BSW: Physical Values                                            │
│ - Voltage (mV)          [from ADC 0-1023 → 0-3300mV]           │
│ - Pin State (TRUE/FALSE) [from GPIO 1/0]                       │
│ - Temperature (°C)      [from sensor]                           │
└─────────────────────────────────────────────────────────────────┘
                              ↓ (Interpret)
┌─────────────────────────────────────────────────────────────────┐
│ FS: Logical Values + State Machine                             │
│ - IGN_State (ON/OFF)     [from IGN_Voltage > 6000mV]           │
│ - Motor_State (RUNNING/STOPPED) [from current sense]           │
│ - System_Mode (ON/DOWN/OFF)     [state machine]                │
│ - Command (SetSystemOn/Down/Off) [policy output]               │
└─────────────────────────────────────────────────────────────────┘
                              ↓ (Apply)
┌─────────────────────────────────────────────────────────────────┐
│ ASW: Application Logic + Decisions                             │
│ - EPB Mode (APPLY/RELEASE)       [user/policy input]          │
│ - Motor Control (FORWARD/REVERSE) [EPB logic]                  │
│ - CAN Messages (TX/RX)            [communication]             │
│ - System Actions (Shutdown, etc)  [top-level control]         │
└─────────────────────────────────────────────────────────────────┘
                              ↓ (Execute)
┌─────────────────────────────────────────────────────────────────┐
│ MCAL: Set Values                                                │
│ - GPIO Outputs (HIGH/LOW)                                       │
│ - PWM Duty (0-100%)                                             │
│ - CAN Transmission                                              │
└─────────────────────────────────────────────────────────────────┘
                              ↓ (Control)
┌─────────────────────────────────────────────────────────────────┐
│ Hardware Layer (Actuators)                                      │
│ - Motor Driver (Enable/Disable)                                 │
│ - Solenoid (ON/OFF)                                             │
│ - LED Indicator (ON/OFF)                                        │
└─────────────────────────────────────────────────────────────────┘
```

### 1.3 레이어 간 호출 방향 규칙

```
✅ 허용된 호출 방향:
  MCAL → (내부만, 외부 호출 받지 않음)
  BSW  → MCAL
  FS   → BSW, ASW (콜백 또는 return value via Outputs)
  ASW  → FS, BSW

❌ 금지된 호출 방향:
  MCAL → BSW (HW layer이 SW layer 호출)
  MCAL → FS
  MCAL → ASW
  BSW  → FS (정책 필요시 FS로 이동)
  BSW  → ASW (정책 필요시 FS로 이동)
```

---

## 2. Port_Api 적용 예시

### 2.1 SystemState Derive Pattern (FS)

#### Before: BSW에 정책이 있음 (잘못된 패턴)

```c
/* ❌ Port_Api.c: BSW 섹션 */
void Bsw_Port_UpdateIgnitionLogic(void) {
    if(Bsw_Port_GetIgnitionState()) {
        SetSystemOn();  /* ❌ BSW가 ASW 함수 호출 */
        g_PORT.IgnOffCount = 0;
    } else {
        SetSystemDown();  /* ❌ BSW가 정책 결정 */
        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
            SetSystemOff();  /* ❌ BSW가 타임아웃 정책 구현 */
        }
    }
}
```

**문제점**:

- BSW가 정책 결정 (IGN OFF 2초 타임아웃)
- BSW가 ASW 함수 직접 호출
- 정책 변경 시 BSW 테스트 필요

#### After: FS에서 정책 도출 (올바른 패턴)

```c
/* ✅ Port_Api.c: FS 섹션 */

/* FS Outputs: Physical → Logical → State */
uint16 Fs_Port_GetIgnitionVoltage_mV(void) {
    return g_ADC.Physical.Power.IGN;  /* Physical */
}

uint8 Fs_Port_GetIgnitionState(void) {
    return Bsw_Port_GetIgnitionState();  /* Logical */
}

uint32 Fs_Port_GetIgnitionOffDuration_ms(void) {
    return g_PORT.IgnOffCount * 10U;  /* State (derived) */
}

/* FS Derive: 정책 구현 */
void Fs_Port_ManageIgnitionState(void) {
    /* FS가 정책 결정 */
    if(Fs_Port_GetIgnitionState()) {
        SetSystemOn();  /* ✅ FS가 ASW 함수 호출 (허용) */
        g_PORT.IgnOffCount = 0;
    } else {
        SetSystemDown();
        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
            g_PORT.IgnOffCount = IGN_OFF_TIME;
            SetSystemOff();  /* ✅ FS 정책: 2초 타임아웃 */
        }
    }
}

void Fs_Port_MonitorAll(void) {
    Fs_Port_ManageIgnitionState();  /* FS 호출 */
    Fs_Port_ManageMotorDriver();
    Fs_Port_ManageSystemFaults();
}
```

**개선점**:

- FS가 정책 소유 (IGN OFF 2초 타임아웃)
- BSW는 신호 제공만 (순수 서비스)
- ASW가 FS 출력을 소비 (향후 개선)

### 2.2 SystemState Apply Pattern (ASW - 향후 개선)

#### 목표: ASW가 FS Outputs를 소비하여 액션 결정

```c
/* Port_Api.c: ASW 섹션 (향후 개선) */

/* ASW가 FS 출력을 기반으로 액션 실행 */
void Port_HandleSystemStateChanges(void) {
    uint8 ignState = Fs_Port_GetIgnitionState();
    uint32 offDuration = Fs_Port_GetIgnitionOffDuration_ms();

    /* ASW 정책: FS 출력 기반 액션 */
    if(ignState == TRUE) {
        /* ASW 결정: IGN ON → 시스템 리소스 할당 */
        EPB_PrepareForOperation();
    } else {
        if(offDuration > 2000U) {  /* FS State 참조 */
            /* ASW 결정: 타임아웃 → 정리 작업 */
            EPB_Shutdown();
        }
    }
}
```

---

## 3. 재사용 가능한 체크리스트

### 3.1 새 모듈 추가 시 검증 항목

```
☐ MCAL 레이어
  ☐ 하드웨어 접근만 수행
  ☐ 비즈니스 로직 없음
  ☐ 조건문 없음
  ☐ 상태 판단 없음

☐ BSW 레이어
  ☐ MCAL 함수 호출로 신호 획득
  ☐ 신호 해석/변환만 수행
  ☐ 정책 결정 없음
  ☐ 상태 머신 없음
  ☐ 외부 모듈 상태 변경 없음

☐ FS 레이어
  ☐ Physical 값 제공 (센서 원시값)
  ☐ Logical 값 제공 (해석된 신호)
  ☐ State 값 제공 (도출된 상태)
  ☐ Policy 구현 (정책 결정)
  ☐ ASW 콜백/return으로 출력

☐ ASW 레이어
  ☐ FS 출력 기반 액션
  ☐ 도메인 특화 로직
  ☐ 하드웨어 직접 접근 없음
  ☐ MCAL/BSW 경유 필수
```

### 3.2 리팩터링 기준: Policy Detection

```
🚨 리팩터 필수 신호:
  ❌ BSW 함수에서 "SetSystem*" 호출
  ❌ BSW 함수에서 "++counter > threshold" 판단
  ❌ BSW 함수에서 "if(external_state)" 조건
  ❌ BSW 함수에서 "WriteActuator()" 결정
  ❌ BSW 함수가 ASW/System 함수 호출

⚠️  리팩터 권장 신호:
  ⚠️  FS가 Pass-through만 수행
  ⚠️  FS가 MCAL/BSW 호출만 함
  ⚠️  Policy가 분산됨 (BSW/FS 혼재)

✅ 올바른 신호:
  ✅ MCAL: 레지스터 접근만
  ✅ BSW: 신호 제공만
  ✅ FS: 정책 구현
  ✅ ASW: 도메인 로직
```

### 3.3 단위 테스트 설계 원칙

```
MCAL 테스트:
  ✓ Register mock을 이용한 HW 시뮬레이션
  ✓ GPIO 설정값 검증
  ✓ ADC 샘플링 검증
  → 하드웨어 없이 테스트 가능

BSW 테스트:
  ✓ MCAL 함수 mock
  ✓ 물리값 입력 → 신호 출력 검증
  ✓ Unit conversion 검증
  ✓ 범위/예외 케이스 검증
  → 정책 없으므로 간단한 테스트

FS 테스트:
  ✓ BSW 함수 mock
  ✓ 시나리오 기반 테스트
  ✓ 상태 머신 검증
  ✓ 타이밍 조건 검증
  ✓ ASW 콜백 호출 검증
  → 정책 테스트에 집중

ASW 테스트:
  ✓ FS 출력 mock
  ✓ 도메인 로직 검증
  ✓ 사용자 시나리오 테스트
  ✓ 에러 처리 검증
  → 비즈니스 로직 테스트
```

---

## 4. 표준 패턴 고급 (Optional)

### 4.1 Safe State 패턴

```c
/* FS: Safe state 구현 */
typedef enum {
    STATE_NORMAL,
    STATE_DEGRADED,
    STATE_SAFE
} SystemState_t;

SystemState_t Fs_GetSystemState(void) {
    /* 여러 센서 기반 상태 도출 */
    if(CriticalFaultDetected()) {
        return STATE_SAFE;      /* Safe state로 전환 */
    }
    if(DegradedCondition()) {
        return STATE_DEGRADED;
    }
    return STATE_NORMAL;
}

void Fs_ApplySafeState(void) {
    SystemState_t state = Fs_GetSystemState();

    if(state == STATE_SAFE) {
        SetSystemOff();        /* 안전 상태: 시스템 OFF */
        DisableAllActuators();
        EnableWatchdog();
    }
}
```

### 4.2 Callback 패턴 (ASW Decoupling)

```c
/* FS: Policy 결정 후 콜백 호출 */
typedef void (*SystemActionCallback_t)(uint8 action);

static SystemActionCallback_t g_SystemActionCallback = NULL;

void Fs_RegisterSystemActionCallback(SystemActionCallback_t cb) {
    g_SystemActionCallback = cb;
}

void Fs_ManageSystemState(void) {
    if(Fs_Port_GetIgnitionState() == FALSE &&
       Fs_Port_GetIgnitionOffDuration_ms() > 2000U) {
        /* Policy 결정 → ASW 콜백 호출 */
        if(g_SystemActionCallback) {
            g_SystemActionCallback(ACTION_SHUTDOWN);
        }
    }
}

/* ASW: 콜백 구현 */
void ASW_HandleSystemAction(uint8 action) {
    switch(action) {
        case ACTION_SHUTDOWN:
            SetSystemOff();
            break;
        default:
            break;
    }
}
```

### 4.3 Output Bus 패턴 (아키텍처 진화)

```c
/* FS: Output structure 정의 */
typedef struct {
    uint16 ignVoltage_mV;           /* Physical */
    uint8  ignState;                /* Logical */
    uint32 ignOffDuration_ms;       /* State */
    uint8  motorState;              /* Logical */
    uint32 motorCurrent_mA;         /* Physical */
    uint8  systemMode;              /* State */
} PortFsOutputs_t;

static PortFsOutputs_t g_PortFsOutputs;

/* FS: Outputs 업데이트 */
void Fs_Port_UpdateOutputs(void) {
    g_PortFsOutputs.ignVoltage_mV = Fs_Port_GetIgnitionVoltage_mV();
    g_PortFsOutputs.ignState = Fs_Port_GetIgnitionState();
    g_PortFsOutputs.ignOffDuration_ms = Fs_Port_GetIgnitionOffDuration_ms();
    g_PortFsOutputs.systemMode = DeriveSystemMode();
}

/* ASW: Output bus 소비 */
void ASW_HandleFsOutputs(void) {
    PortFsOutputs_t *outputs = &g_PortFsOutputs;

    if(outputs->ignState == TRUE) {
        PrepareSystem();
    } else if(outputs->ignOffDuration_ms > 2000U) {
        ShutdownSystem();
    }
}
```

---

## 5. 표준 적용 로드맵

### Phase 0: Port_Api (진행 중)

- ✅ MCAL: 순수 HW 접근
- ✅ BSW: 신호 제공만
- 🔄 FS: 정책 구현 (리팩터링 중)
- ✅ ASW: EPB 로직 (이미 올바름)

**결과**: MCAL/BSW/FS/ASW 명확한 분리

### Phase 1: Motor Control (1-2주)

- 모터 드라이버 제어 정책 분석
- FS로 이동 가능한 정책 식별
- 회귀 테스트 계획

**예상 변경**: Motor Fault 기반 제어 정책 → FS로 이동

### Phase 2: Power Management (2-3주)

- 전원 OFF 시퀀싱 정책 분석
- 타이밍 조건 FS로 이동
- 안전 상태 구현

**예상 변경**: Power-off timeout, safe shutdown → FS

### Phase 3: Architecture Consolidation (3-4주)

- 공통 FS Framework 정의
- Output Bus 표준화
- ASW-FS 인터페이스 정의

**결과**: 프로젝트 전체 일관된 레이어 구조

---

## 6. 주의사항 (Pitfalls to Avoid)

### ❌ Anti-patterns

1. **FS에 비즈니스 로직 넣기**

   ```c
   /* ❌ 잘못된 예 */
   void Fs_ManageEpbMode(void) {
       if(userCommand == EPB_APPLY) {      /* ❌ ASW 로직 */
           ApplyEpb();
       }
   }
   ```

2. **BSW에서 상태 관리**

   ```c
   /* ❌ 잘못된 예 */
   void Bsw_UpdateMotorState(void) {
       if(++motorCounter > threshold) {    /* ❌ 정책 결정 */
           motorState = MOTOR_OFF;
       }
   }
   ```

3. **MCAL에서 로직**
   ```c
   /* ❌ 잘못된 예 */
   void Mcal_ReadAndInterpret(void) {
       uint8 rawValue = ReadRegister();
       if(rawValue > THRESHOLD) {          /* ❌ 로직 */
           WriteRegister(VALUE);
       }
   }
   ```

### ✅ Best practices

1. **명확한 함수 이름**

   ```c
   /* ✅ 역할 명시 */
   Fs_Port_GetIgnitionVoltage_mV()        /* Physical */
   Fs_Port_GetIgnitionState()              /* Logical */
   Fs_Port_GetIgnitionOffDuration_ms()     /* State */
   Fs_Port_ManageIgnitionState()           /* Policy */
   ```

2. **주석으로 책임 명시**

   ```c
   /* ✅ 레이어 책임 표시 */
   void Fs_Port_ManageIgnitionState(void) {
       /* FS Responsibility: System state management
        * - Input: Bsw_Port_GetIgnitionState() (BSW signal)
        * - Logic: IGN OFF 2sec timeout policy
        * - Output: SetSystemOn/Down/Off (ASW action)
        */
   }
   ```

3. **타입/단위 명시**
   ```c
   /* ✅ 단위 명시 */
   uint16 Fs_Port_GetIgnitionVoltage_mV(void) {
       return g_ADC.Physical.Power.IGN;  /* Unit: mV */
   }
   ```

---

## 7. 결론

### 표준 패턴 3가지 핵심 원칙

1. **MCAL**: 하드웨어만
2. **BSW**: 신호만
3. **FS**: 정책만 → **ASW**: 로직만

### 적용 효과

| 효과            | 근거                           |
| --------------- | ------------------------------ |
| 테스트 용이성 ↑ | 레이어별 독립 테스트 가능      |
| 재사용성 ↑      | BSW/FS 정책 변경 영향 최소화   |
| 유지보수성 ↑    | 책임 명확, 버그 위치 파악 용이 |
| 안전성 ↑        | 정책 집중화, 검증 용이         |

---

**문서 버전**: 1.0  
**작성일**: 2026-01-14  
**다음 단계**: PORT_CROSSCHECK_FACTS_final.md 검증
