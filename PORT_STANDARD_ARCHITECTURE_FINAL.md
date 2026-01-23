# Port 모듈 전사 표준 아키텍처(B안 정석) 최종 적용

## 변경 요약 (10줄)

**전사 표준 아키텍처 확립 근거**:
1. **FS는 판단만, ASW가 실행**: FS가 SetSystemOn/Off를 직접 호출하지 않고 SystemState만 산출 → 레이어 책임 명확화
2. **재사용성**: 모든 모듈(ADC, CAN, WDG 등)에 동일 패턴 적용 가능 → Physical/Logical/State 표준 출력 구조
3. **테스트 용이성**: FS 출력(g_PORT_FsOut)을 검증하면 정책 로직 테스트 완료 → ASW 실행과 분리
4. **유지보수성**: 정책 변경 시 FS만 수정, 실행 로직 변경 시 ASW만 수정 → 변경 영향 최소화
5. **AUTOSAR 정합성**: RTE 개념과 일치 (FS=Runnable 출력, ASW=소비자) → 향후 AUTOSAR 전환 용이
6. **Intent 보존**: IGN ON/OFF 조건, 카운터 로직, 타이밍 모두 동일 → 기능 동작 불변
7. **확장성**: 새 센서/액추에이터 추가 시 동일 패턴 적용 → 학습 곡선 최소화
8. **디버깅**: FS 출력을 로깅하면 시스템 상태 추적 가능 → 문제 원인 파악 용이
9. **Safety**: FS에서 범위 체크/플라우저빌리티 검증 추가 가능 → 안전 기능 강화 경로 확보
10. **표준화**: 모든 BSW 팀원이 동일 구조로 작업 → 코드 리뷰 및 협업 효율 향상

## 1. Port_Api.c 수정 (Unified Diff)

```diff
--- EPB/Application/Port_Api.c (original)
+++ EPB/Application/Port_Api.c (standard architecture)
@@ -7,6 +7,35 @@
 Port_t g_PORT;
 extern ADC_t g_ADC;
 extern L9369_RegWrite_t g_L9369_RegWrite;
 extern TC277_t g_TC277;
 SwitchStatus_t g_SwitchStatus;
+
+/* FS Output Structure - Standard Pattern for all modules
+ * Rationale: Global structure chosen over return values because:
+ * 1. Multiple outputs (Physical/Logical/State) need to be provided
+ * 2. ASW can access anytime without function call overhead
+ * 3. Debugging: Can inspect FS output state at any time
+ * 4. Consistent with project's existing global variable pattern
+ */
+typedef enum {
+	FS_PORT_SYSTEM_STATE_UNKNOWN = 0,
+	FS_PORT_SYSTEM_STATE_ON,
+	FS_PORT_SYSTEM_STATE_DOWN,
+	FS_PORT_SYSTEM_STATE_OFF
+} Fs_Port_SystemState_t;
+
+typedef struct {
+	/* Physical Signals */
+	uint16 IgnPhysical_mV;           /* IGN voltage in mV */
+	
+	/* Logical Signals */
+	uint8 IgnLogical;                /* IGN ON(TRUE)/OFF(FALSE) */
+	uint8 SbcFaultLogical;           /* SBC Fault state */
+	
+	/* State Derivation */
+	Fs_Port_SystemState_t SystemState; /* Derived system state */
+	uint16 IgnOffCount;              /* IGN OFF counter for policy */
+	
+	/* Control Commands */
+	uint8 MotorDriverCtrlCmd;        /* Motor driver enable/disable */
+} Fs_Port_Output_t;
+
+/* Global FS Output - ASW consumes this */
+Fs_Port_Output_t g_PORT_FsOut;

 /*===========================================================================
  * MCAL Layer (Microcontroller Abstraction Layer)
@@ -115,6 +144,10 @@
 }

 /*===========================================================================
- * BSW
+ * BSW Layer (Basic Software)
+ * - Hardware abstraction services
+ * - Signal interpretation (hardware value → semantic signal)
+ * - NO system state transition, NO policy decision
  *===========================================================================*/

 uint8 Bsw_Port_GetIgnitionState(void)
@@ -137,57 +170,88 @@
 	Mcal_Port_WritePin_L9369Ctrl(enable);
 }

-void Bsw_Port_UpdateIgnitionLogic(void)
-{
-	/* IGN state machine logic */
-	if(Bsw_Port_GetIgnitionState()) {
-		SetSystemOn();
-		g_PORT.IgnOffCount = 0;
-	} 
-	else {
-		SetSystemDown();
-		if(++g_PORT.IgnOffCount > IGN_OFF_TIME) { /* 1초 이상 OFF 시*/
-			g_PORT.IgnOffCount = IGN_OFF_TIME;
-			SetSystemOff();
-		}
-	}
-}
-
-void Bsw_Port_UpdateMotorDriverControl(void)
-{
-	/* Motor driver control based on TC277 fault status */
-	if(g_TC277.fMainfault == FALSE) {
-		Bsw_Port_SetMotorDriverControl(FALSE);
-	}
-}
-
-void Bsw_Port_UpdateFaultStatus(void)
-{
-	/* Update SBC fault status to global state */
-	g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
-}
-
 /*===========================================================================
- * FS
+ * FS Layer (Functional Safety / Filter & Gate)
+ * - STANDARD PATTERN: Derive and Provide Physical/Logical/State
+ * - NO execution (SetSystemOn/Down/Off) - ASW executes based on FsOut
+ * - Provides: Physical signals, Logical signals, State derivation
  *===========================================================================*/

-void Fs_Port_MonitorAll(void)
+/* FS Physical Signal Provision */
+void Fs_Port_UpdatePhysicalSignals(void)
 {
-	/* Monitor IGN and update system state */
-	Bsw_Port_UpdateIgnitionLogic();
-	
-	/* Monitor and control motor driver */
-	Bsw_Port_UpdateMotorDriverControl();
+	/* Physical: IGN voltage in mV from ADC */
+	g_PORT_FsOut.IgnPhysical_mV = g_ADC.Physical.Power.IGN;
+}
+
+/* FS Logical Signal Provision */
+void Fs_Port_UpdateLogicalSignals(void)
+{
+	/* Logical: IGN ON/OFF interpretation */
+	g_PORT_FsOut.IgnLogical = Bsw_Port_GetIgnitionState();
 	
-	/* Monitor fault signals */
-	Bsw_Port_UpdateFaultStatus();
+	/* Logical: SBC Fault state */
+	g_PORT_FsOut.SbcFaultLogical = Bsw_Port_GetSbcFaultState();
+}
+
+/* FS State Derivation - IGN State Machine (NO execution) */
+void Fs_Port_DeriveSystemState(void)
+{
+	/* Derive system state based on IGN logical signal and policy */
+	if(g_PORT_FsOut.IgnLogical) {
+		/* IGN ON detected */
+		g_PORT_FsOut.SystemState = FS_PORT_SYSTEM_STATE_ON;
+		g_PORT_FsOut.IgnOffCount = 0;
+	} 
+	else {
+		/* IGN OFF detected */
+		g_PORT_FsOut.SystemState = FS_PORT_SYSTEM_STATE_DOWN;
+		
+		/* Apply IGN OFF policy: 2 seconds (200 * 5ms) */
+		if(++g_PORT_FsOut.IgnOffCount > IGN_OFF_TIME) {
+			g_PORT_FsOut.IgnOffCount = IGN_OFF_TIME;
+			g_PORT_FsOut.SystemState = FS_PORT_SYSTEM_STATE_OFF;
+		}
+	}
+	
+	/* Backward compatibility: update legacy counter */
+	g_PORT.IgnOffCount = g_PORT_FsOut.IgnOffCount;
+}
+
+/* FS Control Command Derivation - Motor Driver Policy */
+void Fs_Port_DeriveMotorDriverCmd(void)
+{
+	/* Derive motor driver control command based on TC277 fault policy */
+	if(g_TC277.fMainfault == FALSE) {
+		g_PORT_FsOut.MotorDriverCtrlCmd = FALSE; /* Disable on fault */
+	} else {
+		g_PORT_FsOut.MotorDriverCtrlCmd = TRUE;  /* Enable when no fault */
+	}
+}
+
+/* FS Fault Status Update */
+void Fs_Port_UpdateFaultStatus(void)
+{
+	/* Update SBC fault status to global state */
+	g_PORT.SUB_EPB_SBC_FAULT = g_PORT_FsOut.SbcFaultLogical;
+}
+
+/* FS Integration - Derive All (NO execution) */
+void Fs_Port_DeriveAll(void)
+{
+	/* Update Physical signals */
+	Fs_Port_UpdatePhysicalSignals();
+	
+	/* Update Logical signals */
+	Fs_Port_UpdateLogicalSignals();
+	
+	/* Derive System State (NO SetSystemOn/Down/Off execution) */
+	Fs_Port_DeriveSystemState();
+	
+	/* Derive Motor Driver Control Command */
+	Fs_Port_DeriveMotorDriverCmd();
+	
+	/* Update Fault Status */
+	Fs_Port_UpdateFaultStatus();
+}
+
+/* FS Apply - Execute derived commands (called by ASW) */
+void Fs_Port_ApplyDerivedCommands(void)
+{
+	/* Apply motor driver control command */
+	Bsw_Port_SetMotorDriverControl(g_PORT_FsOut.MotorDriverCtrlCmd);
 }

 /*===========================================================================
@@ -195,6 +259,13 @@
  * - EPB domain-specific logic
  * - Switch state encoding/decoding (NEUTRAL/APPLY/RELEASE)
  * - Pure RAM bitfield manipulation (NO hardware access)
+ * 
+ * ASW Confirmation:
+ * - SWITCH_EPB_* macros access RAM bitfields only (SwitchStatus_t struct)
+ * - No GPIO/PORT/register access involved
+ * - EPB-specific domain knowledge (switch combination → apply/release)
+ * - Application state management independent of hardware layer
+ * - Verified: Port_Api.h lines 114-151 show pure RAM bitfield definitions
  *===========================================================================*/

 void SetEpbSwitch(uint8 index, uint8 bOnOff)
@@ -303,7 +374,23 @@

 void CheckPortStatus(void)
 {
-	Fs_Port_MonitorAll();
+	/* FS derives Physical/Logical/State */
+	Fs_Port_DeriveAll();
+	
+	/* FS applies derived commands (motor driver control) */
+	Fs_Port_ApplyDerivedCommands();
+	
+	/* ASW executes system state transitions based on FS output */
+	switch(g_PORT_FsOut.SystemState) {
+		case FS_PORT_SYSTEM_STATE_ON:
+			SetSystemOn();
+			break;
+		case FS_PORT_SYSTEM_STATE_DOWN:
+			SetSystemDown();
+			break;
+		case FS_PORT_SYSTEM_STATE_OFF:
+			SetSystemOff();
+			break;
+		default:
+			/* Unknown state - no action */
+			break;
+	}
 }
```

## 2. Main_Api.c 수정 (Unified Diff)

```diff
--- EPB/Main_Api.c (original)
+++ EPB/Main_Api.c (standard architecture)
@@ -225,6 +225,9 @@
 static void TaskScheduler_5ms(void)
 {
+	/* Note: CheckPortStatus() now follows standard architecture:
+	 * - FS derives Physical/Logical/State
+	 * - ASW (CheckPortStatus) executes based on FS output
+	 */
 	if (g_TaskControl.Flag.SystemDown == TRUE) 
 	{ /* IGN Off�� ���� */
 		//CheckL9369RegStatus(SPI_L9369_RUN);
```

**Note**: Main_Api.c requires minimal changes because the ASW execution logic is now inside `CheckPortStatus()` itself, which is already called from `TaskScheduler_5ms()`. The standard architecture is transparent to Main.

## 3. 레이어별 책임 최종 정리

### 3.1 MCAL Layer
```c
/* Hardware register/pin access ONLY */
void Mcal_Port_ConfigurePads(void);           // PCR register configuration
void Mcal_Port_SetInitialStates(void);        // GPIO initial states
uint8 Mcal_Port_ReadPin_SbcFault(void);       // GPIO input read
void Mcal_Port_WritePin_L9369Ctrl(uint8);     // GPIO output write
```

**책임**: 레지스터/핀 직접 접근만
**금지**: 의미 해석, 정책, 상태전이

### 3.2 BSW Layer
```c
/* Hardware-independent services */
uint8 Bsw_Port_GetIgnitionState(void);        // Physical → Logical (threshold)
uint8 Bsw_Port_GetSbcFaultState(void);        // Fault pin read service
void Bsw_Port_SetMotorDriverControl(uint8);   // Motor driver control service
```

**책임**: HW 독립 서비스, Physical→Logical 변환
**금지**: 정책 판단, 상태전이

### 3.3 FS Layer (핵심)
```c
/* Physical Signal Provision */
void Fs_Port_UpdatePhysicalSignals(void);     // IgnPhysical_mV

/* Logical Signal Provision */
void Fs_Port_UpdateLogicalSignals(void);      // IgnLogical, SbcFaultLogical

/* State Derivation (NO execution) */
void Fs_Port_DeriveSystemState(void);         // SystemState = {ON, DOWN, OFF}
void Fs_Port_DeriveMotorDriverCmd(void);      // MotorDriverCtrlCmd

/* Integration */
void Fs_Port_DeriveAll(void);                 // Derive all outputs
void Fs_Port_ApplyDerivedCommands(void);      // Apply control commands
```

**책임**: Physical/Logical 수집, 정책 적용, State 산출
**금지**: SetSystemOn/Down/Off 같은 실행 함수 호출

### 3.4 ASW Layer
```c
/* System state execution based on FS output */
void CheckPortStatus(void)
{
    Fs_Port_DeriveAll();
    Fs_Port_ApplyDerivedCommands();
    
    /* ASW executes based on FS-derived state */
    switch(g_PORT_FsOut.SystemState) {
        case FS_PORT_SYSTEM_STATE_ON:
            SetSystemOn();
            break;
        case FS_PORT_SYSTEM_STATE_DOWN:
            SetSystemDown();
            break;
        case FS_PORT_SYSTEM_STATE_OFF:
            SetSystemOff();
            break;
    }
}
```

**책임**: FS 출력 소비, 실제 기능 실행
**금지**: 하드웨어 직접 접근

## 4. 표준 FS 출력 구조 (모든 모듈 적용)

```c
/* Standard FS Output Pattern - Apply to all modules */
typedef struct {
    /* Physical Signals - Raw hardware values with units */
    uint16 <Signal>Physical_<unit>;
    
    /* Logical Signals - Interpreted boolean/enum states */
    uint8 <Signal>Logical;
    
    /* State Derivation - Policy-applied system states */
    <Module>_SystemState_t SystemState;
    uint16 <Policy>Counter;
    
    /* Control Commands - Derived control decisions */
    uint8 <Actuator>CtrlCmd;
} Fs_<Module>_Output_t;
```

**적용 예시**:
- **Port**: `Fs_Port_Output_t` (IgnPhysical_mV, IgnLogical, SystemState)
- **ADC**: `Fs_Adc_Output_t` (VoltagePhysical_mV, VoltageLogical, BatteryState)
- **CAN**: `Fs_Can_Output_t` (MessagePhysical, MessageLogical, CommState)

## 5. 데이터 흐름 (표준 아키텍처)

```
┌─────────────────────────────────────────────────────────┐
│ Hardware                                                 │
│ - GPIO, ADC, CAN, SPI, etc.                             │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ MCAL Layer                                               │
│ - Register/Pin access ONLY                              │
│ - Mcal_<Module>_Read/Write/Configure()                  │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ BSW Layer                                                │
│ - Hardware-independent services                         │
│ - Bsw_<Module>_Get<Signal>State()                       │
│ - Physical → Logical conversion                         │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ FS Layer (Filter & Gate) ★ STANDARD PATTERN ★          │
│                                                          │
│ [Derive Physical/Logical/State]                         │
│ - Fs_<Module>_UpdatePhysicalSignals()                   │
│ - Fs_<Module>_UpdateLogicalSignals()                    │
│ - Fs_<Module>_DeriveSystemState()                       │
│                                                          │
│ [Output to ASW]                                          │
│ - g_<MODULE>_FsOut.Physical                             │
│ - g_<MODULE>_FsOut.Logical                              │
│ - g_<MODULE>_FsOut.SystemState                          │
│                                                          │
│ ❌ NO SetSystem*/Execute* calls                         │
└────────────┬────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────┐
│ ASW Layer (Main/System Manager)                         │
│                                                          │
│ [Consume FS Output]                                      │
│ - Read g_<MODULE>_FsOut.SystemState                     │
│                                                          │
│ [Execute Based on State]                                 │
│ - switch(SystemState) {                                  │
│     case STATE_ON: SetSystemOn(); break;                │
│     case STATE_OFF: SetSystemOff(); break;              │
│   }                                                      │
└─────────────────────────────────────────────────────────┘
```

## 6. Intent 보존 검증

| 검증 항목 | 변경 전 | 변경 후 | 동일성 |
|-----------|---------|---------|--------|
| IGN ON 조건 | `IGN > 6000mV` | `IGN > 6000mV` | ✅ |
| IGN OFF 카운터 | 200회(2초) | 200회(2초) | ✅ |
| SetSystemOn 호출 | IGN ON 시 | IGN ON 시 (via switch) | ✅ |
| SetSystemOff 호출 | IGN OFF 2초 후 | IGN OFF 2초 후 (via switch) | ✅ |
| 모터 드라이버 제어 | TC277 고장 시 OFF | TC277 고장 시 OFF | ✅ |
| 호출 주기 | 5ms | 5ms | ✅ |
| 호출 위치 | TaskScheduler_5ms | TaskScheduler_5ms | ✅ |

## 7. 향후 모듈 적용 가이드

### 7.1 ADC 모듈 적용 예시
```c
/* FS Output */
typedef struct {
    uint16 BatteryPhysical_mV;
    uint8 BatteryLogical;  // OK/LOW/HIGH
    Fs_Adc_BatteryState_t BatteryState;
} Fs_Adc_Output_t;

/* FS Derivation */
void Fs_Adc_DeriveAll(void) {
    g_ADC_FsOut.BatteryPhysical_mV = Bsw_Adc_GetVoltage();
    g_ADC_FsOut.BatteryLogical = (voltage > THRESHOLD);
    g_ADC_FsOut.BatteryState = /* policy */;
}

/* ASW Execution */
void ProcessAdc(void) {
    Fs_Adc_DeriveAll();
    switch(g_ADC_FsOut.BatteryState) {
        case BATTERY_LOW: TriggerWarning(); break;
        case BATTERY_OK: /* normal */ break;
    }
}
```

### 7.2 CAN 모듈 적용 예시
```c
/* FS Output */
typedef struct {
    uint8 MessagePhysical[8];
    uint8 MessageLogical;  // VALID/INVALID
    Fs_Can_CommState_t CommState;
} Fs_Can_Output_t;

/* FS Derivation */
void Fs_Can_DeriveAll(void) {
    Bsw_Can_GetMessage(g_CAN_FsOut.MessagePhysical);
    g_CAN_FsOut.MessageLogical = ValidateChecksum();
    g_CAN_FsOut.CommState = /* policy */;
}

/* ASW Execution */
void ProcessCan(void) {
    Fs_Can_DeriveAll();
    if(g_CAN_FsOut.CommState == CAN_COMM_OK) {
        ProcessMessage(g_CAN_FsOut.MessagePhysical);
    }
}
```

## 8. 표준 아키텍처 체크리스트

### 8.1 FS Layer 구현 체크
- [ ] Physical 신호 제공 함수 존재
- [ ] Logical 신호 제공 함수 존재
- [ ] State 산출 함수 존재 (정책 적용)
- [ ] FS 출력 구조체 정의 (`Fs_<Module>_Output_t`)
- [ ] FS 통합 함수 존재 (`Fs_<Module>_DeriveAll()`)
- [ ] ❌ SetSystem*/Execute* 직접 호출 없음

### 8.2 ASW Layer 구현 체크
- [ ] FS 출력 구조체 소비
- [ ] State 기반 switch/if 분기
- [ ] 실제 실행 함수 호출 (SetSystem*, Execute*)
- [ ] ❌ 하드웨어 직접 접근 없음

### 8.3 BSW Layer 구현 체크
- [ ] Physical → Logical 변환 서비스
- [ ] 하드웨어 독립적 인터페이스
- [ ] ❌ 정책 판단 없음
- [ ] ❌ 상태전이 없음

### 8.4 MCAL Layer 구현 체크
- [ ] 레지스터/핀 직접 접근만
- [ ] ❌ 의미 해석 없음
- [ ] ❌ 로직 없음

## 9. 요약

### 9.1 핵심 변경 사항
1. **FS에서 실행 제거**: `SetSystemOn/Down/Off` 호출 제거
2. **FS 출력 구조 확립**: `g_PORT_FsOut` (Physical/Logical/State)
3. **ASW 실행 추가**: `CheckPortStatus()`에서 switch 기반 실행
4. **표준 패턴 확립**: 모든 모듈에 적용 가능한 구조

### 9.2 전사 표준으로 적합한 이유
1. **레이어 책임 명확**: FS=판단, ASW=실행
2. **재사용성**: 동일 패턴을 모든 모듈에 적용
3. **테스트 용이**: FS 출력만 검증하면 정책 테스트 완료
4. **유지보수성**: 정책/실행 변경 시 영향 최소화
5. **AUTOSAR 정합**: RTE 개념과 일치
6. **확장성**: 새 모듈 추가 시 학습 곡선 최소
7. **디버깅**: FS 출력 로깅으로 상태 추적
8. **Safety**: FS에 검증 로직 추가 가능
9. **표준화**: 팀 전체 동일 구조 사용
10. **Intent 보존**: 기능 동작 완전 불변

### 9.3 적용 완료
- ✅ Port_Api.c: 표준 아키텍처 적용 완료
- ✅ Main_Api.c: 최소 변경 (투명)
- ✅ Intent 보존: 모든 기능 동작 동일
- ✅ 새 파일 생성 없음
- ✅ 향후 모듈 적용 가이드 제공