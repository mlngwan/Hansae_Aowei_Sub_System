# Port 모듈 표준 패턴 최종 적용 완료

## 변경 요약

Port_Api.c를 전사 표준 아키텍처(B안 정석)로 리팩토링 완료:
- **FS Derive**: 상태/명령 결정 후 반환값으로 제공
- **ASW Apply**: FS 반환값 소비 후 실행
- **BSW Service**: Get/Set 서비스만 제공 (Update 함수 제거)
- **Enum 타입**: 매직 넘버 제거, 타입 안전성 확보
- **Intent 보존**: IGN ON/OFF 조건, 카운터 로직, 타이밍 모두 동일

## 1. 적용된 변경 사항

### 1.1 타입 정의 추가

```c
/* System State - FS derives, ASW applies */
typedef enum {
	FS_SYS_ON = 0,
	FS_SYS_DOWN = 1,
	FS_SYS_OFF = 2
} Fs_SystemState_t;

/* Motor Control Command - FS derives, ASW applies */
typedef enum {
	FS_MOTOR_CTRL_DISABLE = 0,
	FS_MOTOR_CTRL_ENABLE = 1
} Fs_MotorCtrlCmd_t;
```

**근거**: 매직 넘버 금지, 타입 안전성 확보, 코드 가독성 향상

### 1.2 BSW Layer - Update 함수 제거

**제거된 함수**:
```c
-void Bsw_Port_UpdateIgnitionLogic(void);
-void Bsw_Port_UpdateMotorDriverControl(void);
-void Bsw_Port_UpdateFaultStatus(void);
```

**유지된 함수**:
```c
uint8 Bsw_Port_GetIgnitionState(void);        // Physical → Logical
uint8 Bsw_Port_GetSbcFaultState(void);        // Fault pin read
void Bsw_Port_SetMotorDriverControl(uint8);   // Motor driver control
```

**근거**: BSW는 서비스만 제공, 정책/상태전이는 FS 담당

### 1.3 FS Layer - Derive 함수 추가 (반환값 기반)

```c
/* FS Derive: System State from IGN */
Fs_SystemState_t Fs_Port_DeriveSystemStateFromIgn(void)
{
	Fs_SystemState_t derivedState;
	uint8 ignLogical = Bsw_Port_GetIgnitionState();
	
	if(ignLogical) {
		derivedState = FS_SYS_ON;
		g_PORT.IgnOffCount = 0;
	} 
	else {
		derivedState = FS_SYS_DOWN;
		if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
			g_PORT.IgnOffCount = IGN_OFF_TIME;
			derivedState = FS_SYS_OFF;
		}
	}
	
	return derivedState;
}

/* FS Derive: Motor Control Command */
Fs_MotorCtrlCmd_t Fs_Port_DeriveMotorCtrlCmd(void)
{
	if(g_TC277.fMainfault == FALSE) {
		return FS_MOTOR_CTRL_DISABLE;
	} else {
		return FS_MOTOR_CTRL_ENABLE;
	}
}

/* FS Derive: SBC Fault Logical */
uint8 Fs_Port_DeriveSbcFaultLogical(void)
{
	return Bsw_Port_GetSbcFaultState();
}
```

**핵심 변경**:
- ❌ `SetSystemOn/Down/Off()` 직접 호출 제거
- ✅ 상태/명령을 결정(derive)하고 반환값으로 제공
- ✅ 정책(IGN OFF 2초 카운터) 적용은 FS에서 수행

**근거**: FS는 결정만, 실행은 ASW 담당 → 레이어 책임 명확화

### 1.4 ASW Layer - Apply 함수 추가

```c
/* ASW Apply: System State */
void Asw_System_ApplySystemState(Fs_SystemState_t state)
{
	switch(state) {
		case FS_SYS_ON:
			SetSystemOn();
			break;
		case FS_SYS_DOWN:
			SetSystemDown();
			break;
		case FS_SYS_OFF:
			SetSystemOff();
			break;
		default:
			break;
	}
}

/* ASW Apply: Motor Control Command */
void Asw_Port_ApplyMotorCtrlCmd(Fs_MotorCtrlCmd_t cmd)
{
	uint8 enable = (cmd == FS_MOTOR_CTRL_ENABLE) ? TRUE : FALSE;
	Bsw_Port_SetMotorDriverControl(enable);
}

/* ASW Apply: SBC Fault Status */
void Asw_Port_ApplySbcFaultStatus(uint8 faultLogical)
{
	g_PORT.SUB_EPB_SBC_FAULT = faultLogical;
}
```

**근거**: ASW는 FS 반환값을 소비하여 실행만 담당

### 1.5 Integration Point - CheckPortStatus()

```c
void CheckPortStatus(void)
{
	/* Standard Pattern: FS Derive → ASW Apply */
	
	/* 1. FS derives system state from IGN */
	Fs_SystemState_t sysState = Fs_Port_DeriveSystemStateFromIgn();
	
	/* 2. ASW applies system state */
	Asw_System_ApplySystemState(sysState);
	
	/* 3. FS derives motor control command */
	Fs_MotorCtrlCmd_t motorCmd = Fs_Port_DeriveMotorCtrlCmd();
	
	/* 4. ASW applies motor control command */
	Asw_Port_ApplyMotorCtrlCmd(motorCmd);
	
	/* 5. FS derives and ASW applies SBC fault status */
	Asw_Port_ApplySbcFaultStatus(Fs_Port_DeriveSbcFaultLogical());
}
```

**근거**: FS Derive → ASW Apply 패턴을 명확히 표현

## 2. Intent 보존 검증

| 검증 항목 | 변경 전 | 변경 후 | 동일성 |
|-----------|---------|---------|--------|
| IGN ON 조건 | `IGN > 6000mV` | `IGN > 6000mV` | ✅ |
| IGN OFF 카운터 | 200회(2초) | 200회(2초) | ✅ |
| SetSystemOn 호출 | IGN ON 시 | IGN ON 시 (via Asw_System_ApplySystemState) | ✅ |
| SetSystemOff 호출 | IGN OFF 2초 후 | IGN OFF 2초 후 (via Asw_System_ApplySystemState) | ✅ |
| 모터 드라이버 제어 | TC277 고장 시 OFF | TC277 고장 시 OFF (via Asw_Port_ApplyMotorCtrlCmd) | ✅ |
| 호출 주기 | 5ms | 5ms | ✅ |
| 호출 위치 | TaskScheduler_5ms → CheckPortStatus | TaskScheduler_5ms → CheckPortStatus | ✅ |

**검증 결과**: 모든 Intent 보존 확인

## 3. 레이어별 함수 정리

### 3.1 MCAL Layer (변경 없음)
```
Mcal_Port_ConfigurePads()
Mcal_Port_SetInitialStates()
Mcal_Port_ReadPin_SbcFault()
Mcal_Port_WritePin_L9369Ctrl()
```

### 3.2 BSW Layer (Update 함수 제거)
```
✅ Bsw_Port_GetIgnitionState()
✅ Bsw_Port_GetSbcFaultState()
✅ Bsw_Port_SetMotorDriverControl()
❌ Bsw_Port_UpdateIgnitionLogic() [제거]
❌ Bsw_Port_UpdateMotorDriverControl() [제거]
❌ Bsw_Port_UpdateFaultStatus() [제거]
```

### 3.3 FS Layer (Derive 함수 추가)
```
✅ Fs_Port_DeriveSystemStateFromIgn() [신규]
✅ Fs_Port_DeriveMotorCtrlCmd() [신규]
✅ Fs_Port_DeriveSbcFaultLogical() [신규]
❌ Fs_Port_MonitorAll() [제거]
```

### 3.4 ASW Layer (Apply 함수 추가)
```
✅ Asw_System_ApplySystemState() [신규]
✅ Asw_Port_ApplyMotorCtrlCmd() [신규]
✅ Asw_Port_ApplySbcFaultStatus() [신규]
✅ SetEpbSwitch() [유지 - EPB 도메인 로직]
✅ GetEpbSwitch() [유지 - EPB 도메인 로직]
```

## 4. 표준 패턴 흐름

```
CheckPortStatus() - Integration Point
│
├─ 1. FS Derive: Fs_Port_DeriveSystemStateFromIgn()
│   └─> Returns: FS_SYS_ON/DOWN/OFF
│
├─ 2. ASW Apply: Asw_System_ApplySystemState(sysState)
│   └─> Executes: SetSystemOn/Down/Off()
│
├─ 3. FS Derive: Fs_Port_DeriveMotorCtrlCmd()
│   └─> Returns: FS_MOTOR_CTRL_ENABLE/DISABLE
│
├─ 4. ASW Apply: Asw_Port_ApplyMotorCtrlCmd(motorCmd)
│   └─> Executes: Bsw_Port_SetMotorDriverControl()
│
└─ 5. FS Derive + ASW Apply: SBC Fault Status
    └─> Fs_Port_DeriveSbcFaultLogical() → Asw_Port_ApplySbcFaultStatus()
```

## 5. 표준 패턴 확정 근거 (10줄)

1. **FS Derive**: FS는 상태/명령을 결정(derive)만 하고 반환값으로 제공 → 실행 책임 분리
2. **ASW Apply**: ASW는 FS 반환값을 받아 실행(apply)만 수행 → 레이어 책임 명확
3. **BSW Service**: BSW는 Get/Set 서비스만 제공, Update/정책 함수 제거 → HW 독립 서비스 레이어
4. **Enum 타입**: 매직 넘버 금지, Fs_SystemState_t/Fs_MotorCtrlCmd_t 사용 → 타입 안전성
5. **반환값 기반**: FS 함수는 반환값으로 결정 전달 → 함수 인터페이스 명확
6. **Intent 보존**: IGN ON/OFF 조건, 카운터 로직, 타이밍 모두 동일 → 기능 동작 불변
7. **재사용성**: 동일 패턴을 ADC, CAN, WDG 등 모든 모듈에 적용 가능 → 표준화
8. **테스트 용이**: FS 반환값만 검증하면 정책 테스트 완료 → 단위 테스트 간소화
9. **유지보수**: 정책 변경 시 FS만, 실행 변경 시 ASW만 수정 → 변경 영향 최소화
10. **확장성**: 새 모듈 추가 시 동일 패턴 적용 → 학습 곡선 최소화

## 6. 표준 패턴 템플릿 (향후 모듈 적용)

### 6.1 타입 정의
```c
typedef enum {
    FS_<MODULE>_STATE_<VALUE1> = 0,
    FS_<MODULE>_STATE_<VALUE2> = 1,
} Fs_<Module>State_t;

typedef enum {
    FS_<MODULE>_CMD_<VALUE1> = 0,
    FS_<MODULE>_CMD_<VALUE2> = 1,
} Fs_<Module>Cmd_t;
```

### 6.2 FS Derive 함수
```c
Fs_<Module>State_t Fs_<Module>_Derive<State>From<Source>(void)
{
    uint8 logical = Bsw_<Module>_Get<Signal>State();
    
    Fs_<Module>State_t derivedState;
    if(/* policy condition */) {
        derivedState = FS_<MODULE>_STATE_<VALUE>;
    }
    
    return derivedState;
}
```

### 6.3 ASW Apply 함수
```c
void Asw_<Module>_Apply<State>(Fs_<Module>State_t state)
{
    switch(state) {
        case FS_<MODULE>_STATE_<VALUE1>:
            Execute<Action1>();
            break;
        case FS_<MODULE>_STATE_<VALUE2>:
            Execute<Action2>();
            break;
        default:
            break;
    }
}
```

### 6.4 Integration Point
```c
void Check<Module>Status(void)
{
    Fs_<Module>State_t state = Fs_<Module>_Derive<State>From<Source>();
    Asw_<Module>_Apply<State>(state);
}
```

## 7. 향후 모듈 적용 예시

### 7.1 ADC 모듈
```c
/* Type */
typedef enum {
    FS_ADC_BATTERY_OK = 0,
    FS_ADC_BATTERY_LOW = 1,
    FS_ADC_BATTERY_HIGH = 2
} Fs_AdcBatteryState_t;

/* FS Derive */
Fs_AdcBatteryState_t Fs_Adc_DeriveBatteryStateFromVoltage(void)
{
    uint16 voltage = Bsw_Adc_GetBatteryVoltage();
    
    if(voltage < BATTERY_LOW_THRESHOLD) {
        return FS_ADC_BATTERY_LOW;
    } else if(voltage > BATTERY_HIGH_THRESHOLD) {
        return FS_ADC_BATTERY_HIGH;
    } else {
        return FS_ADC_BATTERY_OK;
    }
}

/* ASW Apply */
void Asw_Adc_ApplyBatteryState(Fs_AdcBatteryState_t state)
{
    switch(state) {
        case FS_ADC_BATTERY_LOW:
            TriggerLowBatteryWarning();
            break;
        case FS_ADC_BATTERY_HIGH:
            TriggerHighBatteryWarning();
            break;
        case FS_ADC_BATTERY_OK:
            ClearBatteryWarnings();
            break;
    }
}

/* Integration */
void CheckAdcStatus(void)
{
    Fs_AdcBatteryState_t battState = Fs_Adc_DeriveBatteryStateFromVoltage();
    Asw_Adc_ApplyBatteryState(battState);
}
```

### 7.2 CAN 모듈
```c
/* Type */
typedef enum {
    FS_CAN_COMM_OK = 0,
    FS_CAN_COMM_TIMEOUT = 1,
    FS_CAN_COMM_ERROR = 2
} Fs_CanCommState_t;

/* FS Derive */
Fs_CanCommState_t Fs_Can_DeriveCommStateFromMessage(void)
{
    uint8 msgValid = Bsw_Can_GetMessageValid();
    uint16 timeout = Bsw_Can_GetTimeoutCounter();
    
    if(timeout > CAN_TIMEOUT_THRESHOLD) {
        return FS_CAN_COMM_TIMEOUT;
    } else if(!msgValid) {
        return FS_CAN_COMM_ERROR;
    } else {
        return FS_CAN_COMM_OK;
    }
}

/* ASW Apply */
void Asw_Can_ApplyCommState(Fs_CanCommState_t state)
{
    switch(state) {
        case FS_CAN_COMM_OK:
            ProcessCanMessages();
            break;
        case FS_CAN_COMM_TIMEOUT:
            HandleCanTimeout();
            break;
        case FS_CAN_COMM_ERROR:
            HandleCanError();
            break;
    }
}

/* Integration */
void CheckCanStatus(void)
{
    Fs_CanCommState_t commState = Fs_Can_DeriveCommStateFromMessage();
    Asw_Can_ApplyCommState(commState);
}
```

## 8. 표준 패턴 체크리스트

### 8.1 타입 정의
- [x] Enum 타입 정의 (매직 넘버 금지)
- [x] Fs_<Module>State_t 형식 준수
- [x] Fs_<Module>Cmd_t 형식 준수

### 8.2 BSW Layer
- [x] Get/Set 서비스만 존재
- [x] Update/Policy 함수 제거
- [x] Physical → Logical 변환 허용

### 8.3 FS Layer
- [x] Derive 함수명 사용 (Fs_<Module>_Derive<State>From<Source>)
- [x] 반환값 기반 설계
- [x] ❌ SetSystem*/Execute* 직접 호출 없음

### 8.4 ASW Layer
- [x] Apply 함수명 사용 (Asw_<Module>_Apply<State>)
- [x] FS 반환값 기반 switch/if 분기
- [x] 실제 실행 함수 호출

### 8.5 Integration
- [x] Check<Module>Status() 패턴
- [x] FS Derive → ASW Apply 흐름
- [x] Intent 보존 검증

## 9. 적용 완료 요약

### 9.1 변경된 파일
- `EPB/Application/Port_Api.c`: 표준 패턴 적용 완료

### 9.2 변경 내용
- ✅ Enum 타입 정의 (Fs_SystemState_t, Fs_MotorCtrlCmd_t)
- ✅ BSW Update 함수 제거 (3개)
- ✅ FS Derive 함수 추가 (3개, 반환값 기반)
- ✅ ASW Apply 함수 추가 (3개)
- ✅ CheckPortStatus() 표준 패턴 적용
- ✅ Intent 보존 검증 완료

### 9.3 표준 패턴 확정
- **FS Derive**: 상태/명령 결정 + 반환
- **ASW Apply**: 반환값 소비 + 실행
- **BSW Service**: Get/Set만 제공
- **MCAL Access**: 레지스터/핀만 접근

### 9.4 향후 적용
- ADC, CAN, WDG, STM 등 모든 모듈에 동일 패턴 적용 가능
- 표준 템플릿 및 체크리스트 제공
- 학습 곡선 최소화, 유지보수성 향상

## 10. 결론

Port 모듈을 전사 표준 아키텍처(B안 정석)로 리팩토링 완료. FS Derive / ASW Apply 패턴을 확립하여 향후 모든 모듈 리팩토링의 기준으로 사용 가능.
