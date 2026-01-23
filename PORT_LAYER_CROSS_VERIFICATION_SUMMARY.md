# Port 모듈 레이어링 교차검증 및 FS/BSW 경계 재정리 결과

## 1. 핵심 점검 결과 요약

### ✅ 점검 포인트 1: SetSystemOn/Down/Off 호출 위치

**문제 발견**:
- ❌ `Bsw_Port_UpdateIgnitionLogic()`가 BSW Layer에 있으면서 `SetSystemOn/Down/Off()` 호출
- ❌ BSW가 시스템 상태 전이를 직접 수행 → 레이어 책임 위반

**해결 방안**:
- ✅ `Bsw_Port_UpdateIgnitionLogic()` → `Fs_Port_ManageIgnitionState()`로 이동
- ✅ BSW는 신호 제공만, FS가 상태 전이 수행

**Intent 보존**:
- ✅ IGN ON/OFF 판단 조건 동일: `IGN > 6000mV`
- ✅ 카운터 로직 동일: `++g_PORT.IgnOffCount > IGN_OFF_TIME`
- ✅ 함수 호출 순서 동일: `SetSystemOn()` → `SetSystemDown()` → `SetSystemOff()`

### ✅ 점검 포인트 2: "FS = 필터/게이트" 관점 반영

**기존 문제**:
- ❌ FS가 단순 wrapper(`Fs_Port_MonitorAll()`)만 제공
- ❌ Physical/Logical/State 제공 구조 없음

**개선 사항**:
```c
/* FS Physical Signal Provision */
uint16 Fs_Port_GetIgnitionPhysical(void)  // Physical: IGN voltage (mV)
{
    return g_ADC.Physical.Power.IGN;
}

/* FS Logical Signal Provision */
uint8 Fs_Port_GetIgnitionLogical(void)    // Logical: IGN ON/OFF
{
    return Bsw_Port_GetIgnitionState();
}

uint8 Fs_Port_GetSbcFaultLogical(void)    // Logical: SBC Fault
{
    return Bsw_Port_GetSbcFaultState();
}

/* FS State Management */
void Fs_Port_ManageIgnitionState(void)    // State: System ON/DOWN/OFF
{
    // IGN state machine + SetSystemOn/Down/Off
}
```

**FS 역할 명확화**:
- ✅ **Physical**: 하드웨어 물리값 제공 (예: IGN 전압 mV)
- ✅ **Logical**: 의미 신호 제공 (예: IGN ON/OFF)
- ✅ **State**: 시스템 상태 관리 (예: System ON/DOWN/OFF)

### ✅ 점검 포인트 3: MotorDriverControl / SBC Fault 경계

**MotorDriverControl 분석**:

| 함수 | 레이어 | 책임 | 판단 |
|------|--------|------|------|
| `Bsw_Port_SetMotorDriverControl()` | BSW | 핀 제어 수행 | ✅ BSW 적절 |
| `Bsw_Port_UpdateMotorDriverControl()` | **BSW → FS** | TC277 고장 시 비활성화 정책 | ❌ FS로 이동 필요 |

**변경 사항**:
```diff
-void Bsw_Port_UpdateMotorDriverControl(void)  // BSW에서 정책 판단 ❌
+void Fs_Port_ManageMotorDriver(void)          // FS에서 정책 판단 ✅
 {
     if(g_TC277.fMainfault == FALSE) {
         Bsw_Port_SetMotorDriverControl(FALSE);  // BSW는 수행만
     }
 }
```

**SBC Fault 분석**:

| 함수 | 레이어 | 책임 | 판단 |
|------|--------|------|------|
| `Bsw_Port_GetSbcFaultState()` | BSW | 핀 읽기 | ✅ BSW 적절 |
| `Bsw_Port_UpdateFaultStatus()` | **BSW → FS** | 전역 상태 업데이트 | ⚠️ FS로 이동 (상태 관리) |

**변경 사항**:
```diff
-void Bsw_Port_UpdateFaultStatus(void)  // BSW에서 상태 관리 ❌
+void Fs_Port_UpdateFaultStatus(void)   // FS에서 상태 관리 ✅
 {
     g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
 }
```

### ✅ 점검 포인트 4: Switch ASW 여부 확정

**SWITCH_EPB_* 매크로 분석**:

```c
// Port_Api.h 정의
typedef struct {
    unsigned int epb_sw1 : 1;    // RAM bitfield
    unsigned int epb_sw2 : 1;    // RAM bitfield
    unsigned int epb_sw3 : 1;    // RAM bitfield
    unsigned int epb_sw4 : 1;    // RAM bitfield
    unsigned int epb_sw5 : 1;    // RAM bitfield
    unsigned int epb_sw7 : 1;    // RAM bitfield
    unsigned int epb_apply : 1;  // RAM bitfield
    unsigned int epb_release : 1; // RAM bitfield
    uint8 epb_status;            // RAM byte
} SwitchStatus_t;

#define SWITCH_EPB_SW1(P)    P->epb_sw1    // RAM access only
#define SWITCH_EPB_APPLY(P)  P->epb_apply  // RAM access only
```

**검증 결과**:
- ✅ **순수 RAM 비트필드 조작**: GPIO/PORT/레지스터 접근 없음
- ✅ **EPB 도메인 로직**: 스위치 조합 → NEUTRAL/APPLY/RELEASE 해석
- ✅ **애플리케이션 상태 관리**: `g_SwitchStatus` 독립 관리

**ASW 확정 근거**:

| 근거 | 설명 |
|------|------|
| **1. 하드웨어 독립** | GPIO/PORT 접근 없음, 순수 RAM 조작 |
| **2. 도메인 지식** | EPB 스위치 조합의 의미는 EPB 애플리케이션 고유 |
| **3. 상태 인코딩** | 비트 조합 → NEUTRAL(0x00)/APPLY(0x40)/RELEASE(0x04) 해석 |
| **4. 재사용 불가** | EPB 전용 로직, 다른 프로젝트에서 재사용 불가 |
| **5. BSW 범위 초과** | BSW는 하드웨어 추상화만, 애플리케이션 의미 해석은 ASW |

## 2. 변경 사항 상세 (Unified Diff)

### 2.1 BSW Layer 정리

```diff
 /*===========================================================================
- * BSW
+ * BSW Layer (Basic Software)
+ * - Hardware abstraction services
+ * - Signal interpretation (hardware value → semantic signal)
+ * - NO system state transition, NO policy decision
  *===========================================================================*/

 uint8 Bsw_Port_GetIgnitionState(void)
 {
     /* IGN signal interpretation from ADC physical value */
     if(g_ADC.Physical.Power.IGN > IGN_ON) {
         return TRUE;  /* IGN ON */
     } else {
         return FALSE; /* IGN OFF */
     }
 }

 uint8 Bsw_Port_GetSbcFaultState(void)
 {
     /* Read SBC fault pin state */
     return Mcal_Port_ReadPin_SbcFault();
 }

 void Bsw_Port_SetMotorDriverControl(uint8 enable)
 {
     /* Control L9369 motor driver enable pin */
     Mcal_Port_WritePin_L9369Ctrl(enable);
 }

-void Bsw_Port_UpdateIgnitionLogic(void)
-{
-    /* IGN state machine logic */
-    if(Bsw_Port_GetIgnitionState()) {
-        SetSystemOn();
-        g_PORT.IgnOffCount = 0;
-    } 
-    else {
-        SetSystemDown();
-        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
-            g_PORT.IgnOffCount = IGN_OFF_TIME;
-            SetSystemOff();
-        }
-    }
-}
-
-void Bsw_Port_UpdateMotorDriverControl(void)
-{
-    /* Motor driver control based on TC277 fault status */
-    if(g_TC277.fMainfault == FALSE) {
-        Bsw_Port_SetMotorDriverControl(FALSE);
-    }
-}
-
-void Bsw_Port_UpdateFaultStatus(void)
-{
-    /* Update SBC fault status to global state */
-    g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
-}
```

### 2.2 FS Layer 강화

```diff
 /*===========================================================================
- * FS
+ * FS Layer (Functional Safety / Filter & Gate)
+ * - Provides Physical/Logical/State to ASW
+ * - System state management and policy decisions
+ * - State transition logic
  *===========================================================================*/

+/* FS Physical/Logical Signal Provision */
+uint8 Fs_Port_GetIgnitionLogical(void)
+{
+    /* Logical: IGN ON/OFF state */
+    return Bsw_Port_GetIgnitionState();
+}
+
+uint16 Fs_Port_GetIgnitionPhysical(void)
+{
+    /* Physical: IGN voltage in mV */
+    return g_ADC.Physical.Power.IGN;
+}
+
+uint8 Fs_Port_GetSbcFaultLogical(void)
+{
+    /* Logical: SBC Fault state */
+    return Bsw_Port_GetSbcFaultState();
+}
+
+/* FS State Management - IGN State Machine */
+void Fs_Port_ManageIgnitionState(void)
+{
+    /* IGN state machine - system state transition policy */
+    if(Bsw_Port_GetIgnitionState()) {
+        SetSystemOn();
+        g_PORT.IgnOffCount = 0;
+    } 
+    else {
+        SetSystemDown();
+        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
+            g_PORT.IgnOffCount = IGN_OFF_TIME;
+            SetSystemOff();
+        }
+    }
+}
+
+/* FS Policy - Motor Driver Control */
+void Fs_Port_ManageMotorDriver(void)
+{
+    /* Motor driver control policy based on TC277 fault status */
+    if(g_TC277.fMainfault == FALSE) {
+        Bsw_Port_SetMotorDriverControl(FALSE);
+    }
+}
+
+/* FS State Update - Fault Status */
+void Fs_Port_UpdateFaultStatus(void)
+{
+    /* Update SBC fault status to global state */
+    g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
+}
+
+/* FS Integration - Monitor All */
 void Fs_Port_MonitorAll(void)
 {
     /* Monitor IGN and update system state */
-    Bsw_Port_UpdateIgnitionLogic();
+    Fs_Port_ManageIgnitionState();
     
     /* Monitor and control motor driver */
-    Bsw_Port_UpdateMotorDriverControl();
+    Fs_Port_ManageMotorDriver();
     
     /* Monitor fault signals */
-    Bsw_Port_UpdateFaultStatus();
+    Fs_Port_UpdateFaultStatus();
 }
```

### 2.3 ASW Layer 주석 강화

```diff
 /*===========================================================================
- * ASW
+ * ASW Layer (Application Software)
+ * - EPB domain-specific logic
+ * - Switch state encoding/decoding (NEUTRAL/APPLY/RELEASE)
+ * - Pure RAM bitfield manipulation (NO hardware access)
+ * 
+ * ASW Confirmation:
+ * - SWITCH_EPB_* macros access RAM bitfields only (SwitchStatus_t struct)
+ * - No GPIO/PORT/register access involved
+ * - EPB-specific domain knowledge (switch combination → apply/release)
+ * - Application state management independent of hardware layer
  *===========================================================================*/

 void SetEpbSwitch(uint8 index, uint8 bOnOff)
 {
     // ... (기존 코드 동일)
 }
```

## 3. 레이어 책임 재정의 (최종)

### 3.1 MCAL Layer

**책임**: 하드웨어 직접 접근만
```c
void Mcal_Port_ConfigurePads(void);           // PCR 레지스터 설정
void Mcal_Port_SetInitialStates(void);        // GPIO 초기 상태
uint8 Mcal_Port_ReadPin_SbcFault(void);       // GPIO 입력 읽기
void Mcal_Port_WritePin_L9369Ctrl(uint8);     // GPIO 출력 쓰기
```

**특징**:
- ✅ 레지스터 접근만
- ✅ 비즈니스 로직 없음
- ✅ 상태 판단 없음

### 3.2 BSW Layer

**책임**: 하드웨어 추상화 서비스 제공
```c
uint8 Bsw_Port_GetIgnitionState(void);        // IGN ON/OFF 판단
uint8 Bsw_Port_GetSbcFaultState(void);        // SBC Fault 읽기
void Bsw_Port_SetMotorDriverControl(uint8);   // 모터 드라이버 제어
```

**특징**:
- ✅ 하드웨어 값 → 의미 신호 변환
- ✅ 서비스 제공만
- ❌ 시스템 상태 전이 금지
- ❌ 정책 판단 금지

### 3.3 FS Layer

**책임**: Physical/Logical/State 제공 + 정책 판단
```c
/* Physical Signal */
uint16 Fs_Port_GetIgnitionPhysical(void);     // IGN 전압 (mV)

/* Logical Signal */
uint8 Fs_Port_GetIgnitionLogical(void);       // IGN ON/OFF
uint8 Fs_Port_GetSbcFaultLogical(void);       // SBC Fault

/* State Management */
void Fs_Port_ManageIgnitionState(void);       // IGN 상태 머신
void Fs_Port_ManageMotorDriver(void);         // 모터 드라이버 정책
void Fs_Port_UpdateFaultStatus(void);         // Fault 상태 관리

/* Integration */
void Fs_Port_MonitorAll(void);                // 통합 모니터링
```

**특징**:
- ✅ Physical/Logical/State 제공
- ✅ 시스템 상태 전이 수행
- ✅ 정책 판단 수행
- ✅ ASW에 필요한 신호 게이트 역할

### 3.4 ASW Layer

**책임**: EPB 애플리케이션 로직
```c
void SetEpbSwitch(uint8 index, uint8 bOnOff); // EPB 스위치 인코딩
uint8 GetEpbSwitch(uint8 index);              // EPB 스위치 디코딩
```

**특징**:
- ✅ 순수 RAM 조작 (하드웨어 독립)
- ✅ EPB 도메인 지식 필요
- ✅ NEUTRAL/APPLY/RELEASE 의미 해석
- ✅ 재사용 불가 (EPB 전용)

## 4. 데이터 흐름 (최종)

```
┌─────────────────────────────────────────────────────────────┐
│ Hardware                                                     │
│ - GPIO (IGN, SBC_FAULT, L9369_CTRL)                         │
│ - ADC (IGN voltage)                                          │
└────────────┬────────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────────┐
│ MCAL Layer                                                   │
│ - Mcal_Port_ReadPin_SbcFault()                              │
│ - Mcal_Port_WritePin_L9369Ctrl()                            │
└────────────┬────────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────────┐
│ BSW Layer (Service Only)                                    │
│ - Bsw_Port_GetIgnitionState() → TRUE/FALSE                  │
│ - Bsw_Port_GetSbcFaultState() → uint8                       │
│ - Bsw_Port_SetMotorDriverControl() ← uint8                  │
└────────────┬────────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────────┐
│ FS Layer (Filter & Gate)                                    │
│                                                              │
│ [Physical Signal Provision]                                 │
│ - Fs_Port_GetIgnitionPhysical() → uint16 (mV)              │
│                                                              │
│ [Logical Signal Provision]                                  │
│ - Fs_Port_GetIgnitionLogical() → TRUE/FALSE                │
│ - Fs_Port_GetSbcFaultLogical() → uint8                     │
│                                                              │
│ [State Management]                                           │
│ - Fs_Port_ManageIgnitionState() → SetSystemOn/Down/Off     │
│ - Fs_Port_ManageMotorDriver() → Policy decision            │
│ - Fs_Port_UpdateFaultStatus() → State update               │
└────────────┬────────────────────────────────────────────────┘
             │
             ├──────────────────────────────────┐
             │                                  │
             ▼                                  ▼
┌──────────────────────┐      ┌──────────────────────────────┐
│ System State         │      │ ASW Layer                    │
│ - g_PORT.*           │      │ - SetEpbSwitch()             │
│ - SSMInput.*         │      │ - GetEpbSwitch()             │
│ - g_TaskControl.*    │      │                              │
└──────────────────────┘      │ g_SwitchStatus (독립)        │
                              └──────────────────────────────┘
```

## 5. 검증 체크리스트

### 5.1 레이어 경계 검증

| 검증 항목 | 결과 | 근거 |
|-----------|------|------|
| MCAL 순수성 | ✅ | 레지스터 접근만, 로직 없음 |
| BSW 서비스 제공 | ✅ | 신호 변환만, 상태 전이 없음 |
| FS 필터/게이트 | ✅ | Physical/Logical/State 제공 |
| FS 정책 판단 | ✅ | SetSystemOn/Off, 모터 드라이버 정책 |
| ASW 하드웨어 독립 | ✅ | 순수 RAM 조작, GPIO 접근 없음 |

### 5.2 Intent 보존 검증

| 검증 항목 | 변경 전 | 변경 후 | 동일성 |
|-----------|---------|---------|--------|
| IGN ON 조건 | `IGN > 6000mV` | `IGN > 6000mV` | ✅ |
| IGN OFF 카운터 | 200회(2초) | 200회(2초) | ✅ |
| SetSystemOn 호출 | IGN ON 시 | IGN ON 시 | ✅ |
| SetSystemOff 호출 | IGN OFF 2초 후 | IGN OFF 2초 후 | ✅ |
| 모터 드라이버 제어 | TC277 고장 시 OFF | TC277 고장 시 OFF | ✅ |
| Switch 인코딩 | 비트 마스킹 | 비트 마스킹 | ✅ |

### 5.3 Wiring 변경 검증

| 변경 항목 | 변경 전 위치 | 변경 후 위치 | 이유 |
|-----------|--------------|--------------|------|
| IGN 상태 머신 | BSW | FS | 시스템 상태 전이 |
| 모터 드라이버 정책 | BSW | FS | 정책 판단 |
| Fault 상태 관리 | BSW | FS | 상태 관리 |
| Physical 신호 제공 | 없음 | FS | 필터/게이트 역할 |
| Logical 신호 제공 | BSW | FS | 필터/게이트 역할 |

## 6. 요약

### 6.1 주요 변경 사항

1. **BSW → FS 이동**:
   - `Bsw_Port_UpdateIgnitionLogic()` → `Fs_Port_ManageIgnitionState()`
   - `Bsw_Port_UpdateMotorDriverControl()` → `Fs_Port_ManageMotorDriver()`
   - `Bsw_Port_UpdateFaultStatus()` → `Fs_Port_UpdateFaultStatus()`

2. **FS 강화**:
   - Physical 신호 제공: `Fs_Port_GetIgnitionPhysical()`
   - Logical 신호 제공: `Fs_Port_GetIgnitionLogical()`, `Fs_Port_GetSbcFaultLogical()`
   - State 관리: `Fs_Port_ManageIgnitionState()`, `Fs_Port_ManageMotorDriver()`

3. **ASW 확정**:
   - `SetEpbSwitch()`, `GetEpbSwitch()`는 ASW 유지
   - 순수 RAM 조작, 하드웨어 독립
   - EPB 도메인 로직

### 6.2 레이어 책임 최종 정리

| 레이어 | 책임 | 금지 사항 |
|--------|------|-----------|
| **MCAL** | 레지스터 접근 | 로직, 판단 |
| **BSW** | 신호 변환 서비스 | 상태 전이, 정책 |
| **FS** | Physical/Logical/State 제공, 정책 판단 | 하드웨어 직접 접근 |
| **ASW** | 애플리케이션 로직 | 하드웨어 접근 |

### 6.3 검증 결과

- ✅ SetSystemOn/Down/Off 호출이 FS로 이동
- ✅ FS가 Physical/Logical/State 제공 구조 확립
- ✅ BSW는 순수 서비스 제공 레이어로 정리
- ✅ ASW(Switch)는 하드웨어 독립 확정
- ✅ Intent 완전 보존
- ✅ Wiring만 변경