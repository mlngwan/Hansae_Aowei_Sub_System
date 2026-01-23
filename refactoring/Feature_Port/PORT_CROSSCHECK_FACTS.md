# Port 레이어링 크로스체크 - 프로젝트 전체 증거 기반 검증

## 개요

**목적**: Port_Api.c의 현재 레이어링(MCAL/BSW/FS/ASW)이 AUTOSAR-like 관점에서 타당한지 검증  
**방법**: 프로젝트 전체 검색을 통한 증거 기반 분석 (추측 금지)  
**검증 일시**: 2026-01-14  
**검증 범위**: Port_Api.c + 의존 모듈(Main_Api.c, Adc_Api.c 등)

---

## 1. SetSystemOn/Down/Off 호출 위치 분석

### 1.1 함수 정의 위치 (Main_Api.c)

| 함수명            | 정의 위치              | 레이어         | 동작                                                                                                                                                      |
| ----------------- | ---------------------- | -------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `SetSystemOn()`   | `EPB/Main_Api.c:33-39` | **ASW (Main)** | `SSMInput.SSMInIgnition = TRUE`<br>`g_TaskControl.Flag.SystemDown = FALSE`<br>`g_TaskControl.Flag.SystemOff = FALSE`<br>`g_TaskControl.PowerOffCount = 0` |
| `SetSystemDown()` | `EPB/Main_Api.c:47-51` | **ASW (Main)** | `SSMInput.SSMInIgnition = FALSE`<br>`g_TaskControl.Flag.SystemDown = TRUE`                                                                                |
| `SetSystemOff()`  | `EPB/Main_Api.c:40-45` | **ASW (Main)** | `if (SystemDown == TRUE)`<br>`g_TaskControl.Flag.SystemOff = TRUE`                                                                                        |

**증거 코드 (Main_Api.c:33-51)**:

```c
void SetSystemOn(void)
{
	SSMInput.SSMInIgnition = TRUE;             // ← ASW 입력 버스 업데이트
	g_TaskControl.Flag.SystemDown = FALSE;     // ← 시스템 상태 플래그
	g_TaskControl.Flag.SystemOff = FALSE;
	g_TaskControl.PowerOffCount = 0;
}

void SetSystemOff(void)
{
	if (g_TaskControl.Flag.SystemDown == TRUE) {
		g_TaskControl.Flag.SystemOff = TRUE;
	}
}

void SetSystemDown(void)
{
	SSMInput.SSMInIgnition = FALSE;            // ← ASW 입력 버스 업데이트
	g_TaskControl.Flag.SystemDown = TRUE;
}
```

**레이어 분류**:

- 이 함수들은 **Main_Api.c**에 위치 → **ASW (System Manager) 레이어**
- `SSMInput` (ASW 입력 버스), `g_TaskControl` (시스템 상태 플래그) 업데이트
- **결론**: 시스템 상태 관리 함수는 ASW 책임

### 1.2 호출 위치 분석

| 호출 함수         | 호출 위치        | 호출하는 레이어                          | 문제점             |
| ----------------- | ---------------- | ---------------------------------------- | ------------------ |
| `SetSystemOn()`   | `Port_Api.c:120` | **BSW** (`Bsw_Port_UpdateIgnitionLogic`) | ❌ **레이어 위반** |
| `SetSystemDown()` | `Port_Api.c:123` | **BSW** (`Bsw_Port_UpdateIgnitionLogic`) | ❌ **레이어 위반** |
| `SetSystemOff()`  | `Port_Api.c:126` | **BSW** (`Bsw_Port_UpdateIgnitionLogic`) | ❌ **레이어 위반** |

**증거 코드 (Port_Api.c:115-128)**:

```c
void Bsw_Port_UpdateIgnitionLogic(void)  // ← BSW 레이어 함수
{
	/* IGN state machine logic */
	if(Bsw_Port_GetIgnitionState()) {
		SetSystemOn();                    // ← ASW 함수 호출 (레이어 위반!)
		g_PORT.IgnOffCount = 0;
	}
	else {
		SetSystemDown();                  // ← ASW 함수 호출 (레이어 위반!)
		if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
			g_PORT.IgnOffCount = IGN_OFF_TIME;
			SetSystemOff();               // ← ASW 함수 호출 (레이어 위반!)
		}
	}
}
```

**AUTOSAR 위반 사항**:

```
현재:
  BSW (Bsw_Port_UpdateIgnitionLogic)
    → ASW (SetSystemOn/Down/Off)
      → SSMInput, g_TaskControl 업데이트

올바른 흐름 (AUTOSAR-like):
  BSW → FS (Physical/Logical/State 제공)
    → ASW (상태 전이 결정)
      → SetSystemOn/Down/Off 호출
```

### 1.3 호출 체인 전체 분석

```
TaskScheduler_5ms()               [ASW - Main_Api.c]
  ↓
CheckPortStatus()                 [Legacy Wrapper - Port_Api.c:276]
  ↓
Fs_Port_MonitorAll()              [FS - Port_Api.c:148]
  ↓
Bsw_Port_UpdateIgnitionLogic()    [BSW - Port_Api.c:115] ❌
  ↓
SetSystemOn/Down/Off()            [ASW - Main_Api.c:33-51]
  ↓
SSMInput.SSMInIgnition 업데이트   [ASW Input Bus]
g_TaskControl.Flag.* 업데이트     [System State]
```

**문제점**: BSW가 ASW 함수를 직접 호출 → **레이어 역전 (Layering Inversion)**

---

## 2. Port_Api.c 함수 레이어 분류

### 2.1 현재 레이어 섹션별 함수 목록

#### MCAL Layer (Lines 15-79)

| 함수명                           | 분류              | 근거                                                                 |
| -------------------------------- | ----------------- | -------------------------------------------------------------------- |
| `Mcal_Port_ConfigurePads()`      | **Raw HW Access** | SIU.PCR[] 레지스터 직접 쓰기 (27개 핀)<br>PORT*PINCFG*\* 상수 사용   |
| `Mcal_Port_SetInitialStates()`   | **Raw HW Access** | SIU.GPDO[] 레지스터 직접 쓰기 (13개 핀)<br>TRUE/FALSE 초기값 설정    |
| `Mcal_Port_ReadPin_SbcFault()`   | **Raw HW Access** | SIU.GPDI[E10] 레지스터 직접 읽기<br>`return PORT_SUB_EPB_SBC_FAULT;` |
| `Mcal_Port_WritePin_L9369Ctrl()` | **Raw HW Access** | SIU.GPDO[A5] 레지스터 직접 쓰기<br>`PORT_SUB_L9369_CTRL = state;`    |

**MCAL 순수성 검증**:

- ✅ 비즈니스 로직 없음 (조건문, 계산식 없음)
- ✅ 레지스터 접근만 수행 (PCR/GPDO/GPDI)
- ✅ 상태 판단 없음
- ✅ AUTOSAR MCAL 정의에 부합

#### BSW Layer (Lines 81-143)

| 함수명                                | 분류                           | 근거                                                                                      | AUTOSAR 적합성     |
| ------------------------------------- | ------------------------------ | ----------------------------------------------------------------------------------------- | ------------------ |
| `Bsw_Port_GetIgnitionState()`         | **HW-independent Service**     | `g_ADC.Physical.Power.IGN > IGN_ON` 조건 판단<br>TRUE/FALSE 반환 (신호 해석)              | ✅ BSW 책임        |
| `Bsw_Port_GetSbcFaultState()`         | **HW-independent Service**     | `Mcal_Port_ReadPin_SbcFault()` 래핑<br>핀 상태 → 신호 변환                                | ✅ BSW 책임        |
| `Bsw_Port_SetMotorDriverControl()`    | **HW-independent Service**     | `Mcal_Port_WritePin_L9369Ctrl(enable)` 래핑<br>제어 명령 서비스 제공                      | ✅ BSW 책임        |
| `Bsw_Port_UpdateIgnitionLogic()`      | **Policy/State Derivation** ❌ | **SetSystemOn/Down/Off 호출**<br>**g_PORT.IgnOffCount 관리**<br>**시스템 상태 전이 결정** | ❌ **FS/ASW 책임** |
| `Bsw_Port_UpdateMotorDriverControl()` | **Policy/State Derivation** ❌ | **g_TC277.fMainfault 기반 정책**<br>**모터 드라이버 제어 결정**                           | ❌ **FS 책임**     |
| `Bsw_Port_UpdateFaultStatus()`        | **HW-independent Service**     | `g_PORT.SUB_EPB_SBC_FAULT = GetSbcFaultState()`<br>단순 매핑                              | ⚠️ BSW/FS 경계     |

**증거 코드 (문제 함수)**:

```c
/* Port_Api.c:115-128 */
void Bsw_Port_UpdateIgnitionLogic(void)  // ← BSW로 분류되었으나
{
	if(Bsw_Port_GetIgnitionState()) {
		SetSystemOn();                   // ← ASW 함수 호출 (정책 결정)
		g_PORT.IgnOffCount = 0;          // ← 상태 변수 관리
	}
	else {
		SetSystemDown();                 // ← ASW 함수 호출 (정책 결정)
		if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {  // ← 정책 로직 (2초 타임아웃)
			SetSystemOff();              // ← ASW 함수 호출 (정책 결정)
		}
	}
}

/* Port_Api.c:130-136 */
void Bsw_Port_UpdateMotorDriverControl(void)  // ← BSW로 분류되었으나
{
	if(g_TC277.fMainfault == FALSE) {    // ← 시스템 전체 상태 기반 정책 판단
		Bsw_Port_SetMotorDriverControl(FALSE);  // ← 안전 정책 적용
	}
}
```

**AUTOSAR 위반 근거**:

1. **BSW는 정책 결정을 하지 않음**: `SetSystemOn/Off` 호출 권한 없음
2. **BSW는 시스템 상태를 관리하지 않음**: `g_PORT.IgnOffCount` 증가/리셋 책임 없음
3. **BSW는 타임아웃 정책을 소유하지 않음**: `IGN_OFF_TIME (200*10ms=2초)` 정책은 FS/ASW 책임

#### FS Layer (Lines 145-156)

| 함수명                 | 분류                        | 근거                                                                                                                             | AUTOSAR 적합성                                              |
| ---------------------- | --------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- |
| `Fs_Port_MonitorAll()` | **Policy/State Derivation** | BSW 함수 3개 조율<br>`Bsw_Port_UpdateIgnitionLogic()`<br>`Bsw_Port_UpdateMotorDriverControl()`<br>`Bsw_Port_UpdateFaultStatus()` | ⚠️ **Pass-through만 수행**<br>(실제 정책 로직은 BSW에 있음) |

**증거 코드**:

```c
/* Port_Api.c:148-156 */
void Fs_Port_MonitorAll(void)
{
	/* Monitor IGN and update system state */
	Bsw_Port_UpdateIgnitionLogic();      // ← BSW 호출만 (정책은 BSW에)

	/* Monitor and control motor driver */
	Bsw_Port_UpdateMotorDriverControl(); // ← BSW 호출만 (정책은 BSW에)

	/* Monitor fault signals */
	Bsw_Port_UpdateFaultStatus();        // ← BSW 호출만
}
```

**문제점**: FS가 **Pass-through**만 수행, 실제 정책 로직은 BSW에 구현됨 → **FS 레이어가 역할 수행 못함**

#### ASW Layer (Lines 158-260)

| 함수명           | 분류                         | 근거                                                                                  | AUTOSAR 적합성 |
| ---------------- | ---------------------------- | ------------------------------------------------------------------------------------- | -------------- |
| `SetEpbSwitch()` | **Application/Domain Logic** | EPB 스위치 비트 인코딩<br>NEUTRAL/APPLY/RELEASE 판단<br>g_SwitchStatus (RAM) 업데이트 | ✅ ASW 책임    |
| `GetEpbSwitch()` | **Application/Domain Logic** | EPB 스위치 상태 디코딩<br>g_SwitchStatus (RAM) 읽기                                   | ✅ ASW 책임    |

**증거 코드**:

```c
/* Port_Api.c:163-224 */
void SetEpbSwitch(uint8 index, uint8 bOnOff)
{
	SwitchStatus_t *pStatus = (SwitchStatus_t*)&g_SwitchStatus;  // ← RAM 접근
	uint8 epbStatus = (uint8)SWITCH_EPB_STATUS(pStatus);

	// ... 비트 마스킹 로직 (EPB 도메인 지식 필요)

	SWITCH_EPB_STATUS(pStatus) = (uint8)epbStatus;  // ← RAM 쓰기만

	if (epbStatus == EPB_SW_NEUTRAL) {              // ← EPB 애플리케이션 정책
		SWITCH_EPB_APPLY(pStatus) = FALSE;
		SWITCH_EPB_RELEASE(pStatus) = FALSE;
	}
	// ...
}
```

**SWITCH*EPB*\* 매크로 정의 (Port_Api.h:142-151)**:

```c
#define SWITCH_EPB_SW1(P)		P->epb_sw1       // ← 구조체 필드 접근 (RAM)
#define SWITCH_EPB_SW2(P)		P->epb_sw2
#define SWITCH_EPB_APPLY(P)		P->epb_apply
#define SWITCH_EPB_STATUS(P)	P->epb_status
```

**레지스터 접근 여부 검증**:

- ✅ **레지스터 접근 없음**: `SwitchStatus_t` 구조체는 RAM 기반 (Port_Api.h:115-133)
- ✅ **GPIO 핀 접근 없음**: 매크로는 `P->epb_sw*` 포인터 필드만 참조
- ✅ **순수 ASW 로직**: EPB 도메인 지식(NEUTRAL/APPLY/RELEASE) 필요
- ✅ **AUTOSAR 적합**: ASW가 애플리케이션 상태 관리

#### Legacy Interface (Lines 262-284)

| 함수명                  | 분류               | 근거                                         |
| ----------------------- | ------------------ | -------------------------------------------- |
| `PORTInitialize()`      | **Legacy Wrapper** | SetPadConfiguration + SetPortInitialize 조합 |
| `SetPadConfiguration()` | **Legacy Wrapper** | Mcal_Port_ConfigurePads() 호출               |
| `SetPortInitialize()`   | **Legacy Wrapper** | Mcal_Port_SetInitialStates() 호출            |
| `CheckPortStatus()`     | **Legacy Wrapper** | Fs_Port_MonitorAll() 호출                    |

---

## 3. FS-as-Filter 적용 가능성 분석

### 3.1 Physical/Logical/State 변수 조사

#### 3.1.1 g_ADC.Physical.\* (ADC 물리값)

**정의 위치**: `EPB/Peripheral/ADC/Adc_Api.h`  
**업데이트 위치**: `EPB/Peripheral/ADC/Adc_Api.c:150-158`

| 변수명                        | 타입   | 단위 | 사용처                                              |
| ----------------------------- | ------ | ---- | --------------------------------------------------- |
| `g_ADC.Physical.Power.IGN`    | uint16 | mV   | **Port_Api.c:94** (`Bsw_Port_GetIgnitionState()`)   |
| `g_ADC.Physical.Power.UBB`    | uint16 | mV   | **Adc_Api.c:167** (`SSMInput.SSMInVoltage`)         |
| `g_ADC.Physical.Power.UBVR`   | uint16 | mV   | -                                                   |
| `g_ADC.Physical.Motor.MOTORA` | uint16 | mV   | **Adc_Api.c:170** (`PbcInDriver.MotorVoltageLeft`)  |
| `g_ADC.Physical.Motor.MOTORB` | uint16 | mV   | **Adc_Api.c:171** (`PbcInDriver.MotorVoltageRight`) |

**증거 코드 (Adc_Api.c:150-173)**:

```c
/* Physical 값 계산 (BSW Adc) */
g_ADC.Physical.Power.IGN = Bsw_Adc_ConvertToPhysical(...) * ADC_REF;
g_ADC.Physical.Power.UBB = Bsw_Adc_ConvertToPhysical(...) * ADC_REF;
g_ADC.Physical.Motor.MOTORA = Mcal_Adc_ReadL9369Motor(...) * ADC_REF;

/* ASW Input Bus 업데이트 */
SSMInput.SSMInVoltage = g_ADC.Physical.Power.UBB;        // ← ASW 입력
PbcInDriver.MotorVoltageLeft = g_ADC.Physical.Motor.MOTORA;  // ← ASW 입력
```

#### 3.1.2 SSMInput.\* (ASW 입력 버스)

**정의 위치**: `EPB/AppLogic/ElectronicParkBrake_types.h:79`  
**업데이트 위치**:

- `Main_Api.c:35, 49` (SetSystemOn/Down)
- `Adc_Api.c:167` (SSMInVoltage)

| 변수명                     | 타입     | 업데이트 위치                                                            | 소비자                              |
| -------------------------- | -------- | ------------------------------------------------------------------------ | ----------------------------------- |
| `SSMInput.SSMInIgnition`   | uint16_T | **Main_Api.c:35** (`SetSystemOn`)<br>**Main_Api.c:49** (`SetSystemDown`) | **StandStillManager.c:66, 83, 104** |
| `SSMInput.SSMInVoltage`    | uint16_T | **Adc_Api.c:167**                                                        | EPB Logic                           |
| `SSMInput.SSMInEPB_OpMode` | uint16_T | **Spi_ExtEEPROM_Api.c:37**                                               | EPB Logic                           |

**증거 코드 (Main_Api.c:33-51)**:

```c
void SetSystemOn(void)
{
	SSMInput.SSMInIgnition = TRUE;   // ← ASW 입력 버스 업데이트
	g_TaskControl.Flag.SystemDown = FALSE;
	// ...
}

void SetSystemDown(void)
{
	SSMInput.SSMInIgnition = FALSE;  // ← ASW 입력 버스 업데이트
	g_TaskControl.Flag.SystemDown = TRUE;
}
```

**ASW 소비 증거 (StandStillManager.c:66, 83)**:

```c
if (((int32_T)SSMInput.SSMInIgnition) == 0) {  // ← ASW가 IGN 상태 읽음
    // IGN OFF 로직
}

if (((int32_T)SSMInput.SSMInIgnition) == 1) {  // ← ASW가 IGN 상태 읽음
    // IGN ON 로직
}
```

#### 3.1.3 g_PORT.\* (Port 상태 변수)

**정의 위치**: `EPB/Application/Port_Api.c:8`  
**타입**: `Port_t` (Port_Api.h:107-112)

| 변수명                     | 타입             | 읽기               | 쓰기                         | 의미                       |
| -------------------------- | ---------------- | ------------------ | ---------------------------- | -------------------------- |
| `g_PORT.IgnOffCount`       | uint16           | **Port_Api.c:124** | **Port_Api.c:120, 124, 125** | IGN OFF 카운터 (10ms 단위) |
| `g_PORT.SUB_EPB_SBC_FAULT` | uint8 (bitfield) | -                  | **Port_Api.c:142**           | SBC Fault 상태 플래그      |

**증거 코드 (Port_Api.c:120-126)**:

```c
if(Bsw_Port_GetIgnitionState()) {
	SetSystemOn();
	g_PORT.IgnOffCount = 0;              // ← 리셋
} else {
	SetSystemDown();
	if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {  // ← 증가 및 비교
		g_PORT.IgnOffCount = IGN_OFF_TIME;     // ← 포화
		SetSystemOff();
	}
}
```

#### 3.1.4 g_TaskControl.\* (시스템 상태 플래그)

**정의 위치**: `EPB/Main_Api.c:28` (static)  
**업데이트 위치**: `Main_Api.c:35-50` (SetSystemOn/Down/Off)

| 변수명                          | 타입   | 업데이트 위치         | 의미             |
| ------------------------------- | ------ | --------------------- | ---------------- |
| `g_TaskControl.Flag.SystemDown` | bool   | **Main_Api.c:36, 50** | 시스템 DOWN 상태 |
| `g_TaskControl.Flag.SystemOff`  | bool   | **Main_Api.c:37, 43** | 시스템 OFF 상태  |
| `g_TaskControl.PowerOffCount`   | uint32 | **Main_Api.c:38**     | 전원 OFF 카운터  |

### 3.2 FS-as-Filter 권장 구조

#### 현재 (문제):

```
BSW (Bsw_Port_UpdateIgnitionLogic)
  ├─ Bsw_Port_GetIgnitionState()       → g_ADC.Physical.Power.IGN 읽기
  ├─ SetSystemOn/Down/Off() 호출       → SSMInput, g_TaskControl 업데이트
  └─ g_PORT.IgnOffCount 관리            → 상태 변수 직접 관리
```

#### 권장 (FS-as-Filter):

```
FS Layer
  ├─ Physical: Fs_Port_GetIgnitionVoltage()     → g_ADC.Physical.Power.IGN 반환
  ├─ Logical:  Fs_Port_GetIgnitionState()       → TRUE/FALSE (6000mV 기준)
  └─ State:    Fs_Port_GetIgnitionOffDuration() → IgnOffCount * 10ms

ASW (Main)
  ├─ Fs_Port_GetIgnitionState() 조회
  ├─ Fs_Port_GetIgnitionOffDuration() 조회
  ├─ if(ignState == TRUE) → SetSystemOn()      ← ASW가 결정
  └─ if(ignOffDuration > 2000ms) → SetSystemOff()  ← ASW가 결정
```

### 3.3 최소 변경 경로 (Minimal-Change Path)

**Step 1: FS 서비스 포트 추가 (Port_Api.c FS 섹션)**

```c
/* FS Layer - Service Ports */

// Physical: Raw value (mV)
uint16 Fs_Port_GetIgnitionVoltage_mV(void) {
    return g_ADC.Physical.Power.IGN;
}

// Logical: Boolean interpretation
uint8 Fs_Port_GetIgnitionState(void) {
    return Bsw_Port_GetIgnitionState();  // 기존 BSW 재사용
}

// State: Duration (ms)
uint32 Fs_Port_GetIgnitionOffDuration_ms(void) {
    return g_PORT.IgnOffCount * 10U;  // 10ms 단위 → ms 변환
}
```

**Step 2: BSW 함수 이름 변경 (정책 로직 제거 명시)**

```c
/* BSW Layer - Services only (NO policy) */

// 기존: Bsw_Port_UpdateIgnitionLogic()
// 변경: (삭제 - 정책 로직은 FS/ASW로 이동)

// 유지: Signal interpretation services
uint8 Bsw_Port_GetIgnitionState(void);
void Bsw_Port_SetMotorDriverControl(uint8 enable);
```

**Step 3: FS에서 정책 로직 수행 (ASW 대신)**

```c
/* FS Layer - Policy Logic */

void Fs_Port_ManageIgnitionState(void) {
    if(Fs_Port_GetIgnitionState()) {
        SetSystemOn();             // FS가 ASW 함수 호출 (허용됨)
        g_PORT.IgnOffCount = 0;
    } else {
        SetSystemDown();
        if(++g_PORT.IgnOffCount > 200U) {
            g_PORT.IgnOffCount = 200U;
            SetSystemOff();
        }
    }
}
```

**Step 4: 호출 체인 수정 없음**

```c
/* 기존 호출 체인 유지 */
CheckPortStatus() → Fs_Port_MonitorAll() → Fs_Port_ManageIgnitionState()
```

**변경 규모**:

- ✅ 새 파일 생성 없음
- ✅ 함수 시그니처 변경 없음 (외부 인터페이스 유지)
- ✅ 함수 이동만 수행 (BSW → FS)
- ✅ 서비스 포트 3개 추가 (선택적)

---

## 4. SWITCH*EPB*\* 매크로 증거 기반 분석

### 4.1 매크로 정의 증거

**위치**: `EPB/Application/Port_Api.h:142-151`

```c
/* 구조체 필드 접근 매크로 (레지스터 접근 없음) */
#define SWITCH_EPB_SW1(P)		P->epb_sw1       // ← RAM 필드
#define SWITCH_EPB_SW2(P)		P->epb_sw2
#define SWITCH_EPB_SW3(P)		P->epb_sw3
#define SWITCH_EPB_SW4(P)		P->epb_sw4
#define SWITCH_EPB_SW5(P)		P->epb_sw5
#define SWITCH_EPB_SW7(P)		P->epb_sw7
#define SWITCH_EPB_SW_LIGHT(P)	P->epb_sw_light
#define SWITCH_EPB_APPLY(P)		P->epb_apply      // ← RAM 필드
#define SWITCH_EPB_RELEASE(P)	P->epb_release
#define SWITCH_EPB_STATUS(P)	P->epb_status
```

**구조체 정의 (Port_Api.h:115-133)**:

```c
typedef struct {
	unsigned int bls		: 1;
	unsigned int ign		: 1;
	unsigned int esc_on_off	: 1;
	unsigned int avh		: 1;
	unsigned int hdc		: 1;
	unsigned int pb			: 1;
	unsigned int clutch		: 1;
	unsigned int epb_sw1 	: 1; /* 8 */   // ← 비트필드 (RAM)
	unsigned int epb_sw2 	: 1;
	unsigned int epb_sw3 	: 1;
	unsigned int epb_sw4 	: 1;
	unsigned int epb_sw5 	: 1;
	unsigned int epb_sw7 	: 1;
	unsigned int epb_sw_light: 1;
	unsigned int epb_apply	: 1;          // ← 비트필드 (RAM)
	unsigned int epb_release: 1; /* 16 */
	uint8 epb_status;                     // ← RAM
	uint8 reserved; /* 32 */
} SwitchStatus_t;
```

**전역 변수 선언 (Port_Api.c:12)**:

```c
SwitchStatus_t g_SwitchStatus;  // ← RAM 전역 변수 (레지스터 아님)
```

### 4.2 레지스터 접근 여부 검증

| 항목            | 증거                                                                           | 결론                        |
| --------------- | ------------------------------------------------------------------------------ | --------------------------- |
| **매크로 정의** | `#define SWITCH_EPB_SW1(P) P->epb_sw1`                                         | ✅ 포인터 필드 접근만       |
| **구조체 타입** | `typedef struct { unsigned int epb_sw1:1; }`                                   | ✅ RAM 비트필드             |
| **전역 변수**   | `SwitchStatus_t g_SwitchStatus;`                                               | ✅ RAM 할당 (레지스터 없음) |
| **GPIO 참조**   | 프로젝트 전체 검색: `SWITCH_EPB_*` + `PCR_*` 또는 `PORT_*`                     | ✅ **발견 안됨**            |
| **매크로 확장** | `SWITCH_EPB_SW1(&g_SwitchStatus) = bOnOff` → `g_SwitchStatus.epb_sw1 = bOnOff` | ✅ RAM 쓰기만               |

**결론**: ✅ **SWITCH*EPB*\* 매크로는 레지스터를 전혀 건드리지 않음** (순수 RAM 접근)

### 4.3 SetEpbSwitch/GetEpbSwitch ASW 분류 근거

| 근거                        | 설명                                                         |
| --------------------------- | ------------------------------------------------------------ |
| **1. RAM 기반 상태 관리**   | `g_SwitchStatus` RAM 변수만 읽기/쓰기                        |
| **2. EPB 도메인 지식 필요** | NEUTRAL/APPLY/RELEASE 의미 해석 (EPB 전용)                   |
| **3. 비트 인코딩 로직**     | 7개 스위치를 `epb_status` uint8로 인코딩 (애플리케이션 로직) |
| **4. 하드웨어 독립적**      | GPIO 접근 없음, CAN 메시지/Simulink 등에서 호출 가능         |
| **5. BSW 재사용 불가**      | 다른 프로젝트에서 EPB 스위치 로직 재사용 불가 (EPB 전용)     |

**AUTOSAR 분류**: ✅ **ASW (Application Software)** - EPB 애플리케이션 로직

---

## 5. AUTOSAR-like 비판적 분석

### 5.1 주요 위반 사항 (Critical Deviations)

#### 위반 #1: BSW → ASW 직접 호출 (Layering Inversion)

| 항목          | 현재                                                       | AUTOSAR 표준   | 위험도      |
| ------------- | ---------------------------------------------------------- | -------------- | ----------- |
| **호출 방향** | BSW → ASW (`Bsw_Port_UpdateIgnitionLogic` → `SetSystemOn`) | BSW → FS → ASW | 🔴 **High** |
| **문제점**    | 레이어 역전, BSW가 ASW 종속                                | -              |
| **영향**      | BSW 재사용 불가, ASW 변경 시 BSW 컴파일 에러               | -              |

**근거**:

```c
/* Port_Api.c:115-128 (BSW 섹션) */
void Bsw_Port_UpdateIgnitionLogic(void) {  // ← BSW 함수
    SetSystemOn();                         // ← ASW 함수 호출 (위반!)
}
```

#### 위반 #2: BSW에서 정책 결정 (Policy in BSW)

| 항목          | 현재                                                | AUTOSAR 표준 | 위험도      |
| ------------- | --------------------------------------------------- | ------------ | ----------- |
| **정책 위치** | BSW (`++g_PORT.IgnOffCount > IGN_OFF_TIME`)         | FS/ASW       | 🔴 **High** |
| **문제점**    | BSW가 타임아웃 정책 소유 (200\*10ms=2초)            | -            |
| **영향**      | BSW는 설정 가능한 서비스여야 하나 정책이 하드코딩됨 | -            |

**근거**:

```c
/* Port_Api.c:124-126 (BSW 섹션) */
if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {  // ← BSW가 정책 판단 (위반!)
    SetSystemOff();
}
```

#### 위반 #3: FS 레이어가 실제 역할 안함 (FS Bypass)

| 항목        | 현재                       | AUTOSAR 표준                | 위험도        |
| ----------- | -------------------------- | --------------------------- | ------------- |
| **FS 역할** | Pass-through (BSW 호출만)  | Physical/Logical/State 제공 | 🟡 **Medium** |
| **문제점**  | FS 존재 의미 없음          | -                           |
| **영향**    | FS가 Filter/Gate 역할 못함 | -                           |

**근거**:

```c
/* Port_Api.c:148-156 (FS 섹션) */
void Fs_Port_MonitorAll(void) {
    Bsw_Port_UpdateIgnitionLogic();  // ← Pass-through만 (실제 로직은 BSW에)
}
```

### 5.2 허용 가능한 편차 (Acceptable Deviations)

#### 편차 #1: ASW 함수가 Port_Api.c에 위치

| 항목     | 현재                                      | AUTOSAR 표준  | 허용 여부   |
| -------- | ----------------------------------------- | ------------- | ----------- |
| **위치** | `SetEpbSwitch/GetEpbSwitch` in Port_Api.c | 별도 ASW 파일 | ✅ **허용** |
| **근거** | Legacy 리팩터 단계, 파일 분리는 Phase 2   | -             |
| **조건** | 주석으로 ASW 섹션 명확히 구분             | -             |

#### 편차 #2: Legacy Wrapper 존재

| 항목     | 현재                                              | AUTOSAR 표준      | 허용 여부   |
| -------- | ------------------------------------------------- | ----------------- | ----------- |
| **함수** | `CheckPortStatus()`, `PORTInitialize()`           | 직접 FS/MCAL 호출 | ✅ **허용** |
| **근거** | 기존 코드 호환성 유지 필요                        | -                 |
| **조건** | 주석으로 Legacy 명시, 내부적으로 정상 레이어 호출 | -                 |

### 5.3 위험도 평가 매트릭스

| 위반 사항               | 위험도    | 재작업 규모            | 우선순위 |
| ----------------------- | --------- | ---------------------- | -------- |
| **BSW → ASW 직접 호출** | 🔴 High   | Medium (함수 이동)     | **P1**   |
| **BSW에서 정책 결정**   | 🔴 High   | Medium (함수 이동)     | **P1**   |
| **FS Bypass**           | 🟡 Medium | Low (서비스 포트 추가) | **P2**   |
| **ASW가 Port_Api.c에**  | 🟢 Low    | High (파일 분리)       | **P3**   |

---

## 6. 최소 리팩터링 단계 (Smallest Refactor Steps)

### Phase 1: 레이어 위반 해소 (P1 - 필수)

**목표**: BSW → ASW 직접 호출 제거, 정책 로직 FS로 이동

**Step 1.1: BSW 함수 이름 변경**

```diff
/* Port_Api.c:115-128 */
-void Bsw_Port_UpdateIgnitionLogic(void)  // ← 삭제
+// (정책 로직 제거 - FS로 이동)

/* BSW는 서비스만 제공 (유지) */
uint8 Bsw_Port_GetIgnitionState(void);
void Bsw_Port_SetMotorDriverControl(uint8 enable);
```

**Step 1.2: FS에 정책 함수 추가**

```diff
/* Port_Api.c:148-156 (FS 섹션) */
+void Fs_Port_ManageIgnitionState(void) {
+    /* IGN state machine - Policy logic */
+    if(Bsw_Port_GetIgnitionState()) {
+        SetSystemOn();             // ← FS가 ASW 호출 (허용됨)
+        g_PORT.IgnOffCount = 0;
+    } else {
+        SetSystemDown();
+        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
+            g_PORT.IgnOffCount = IGN_OFF_TIME;
+            SetSystemOff();
+        }
+    }
+}

+void Fs_Port_ManageMotorDriver(void) {
+    /* Motor driver control policy */
+    if(g_TC277.fMainfault == FALSE) {
+        Bsw_Port_SetMotorDriverControl(FALSE);
+    }
+}

 void Fs_Port_MonitorAll(void) {
-    Bsw_Port_UpdateIgnitionLogic();
-    Bsw_Port_UpdateMotorDriverControl();
+    Fs_Port_ManageIgnitionState();    // ← FS 함수 호출
+    Fs_Port_ManageMotorDriver();      // ← FS 함수 호출
     Bsw_Port_UpdateFaultStatus();
 }
```

**변경 규모**:

- 삭제: 2개 BSW 함수
- 추가: 2개 FS 함수
- 변경: Fs_Port_MonitorAll() 호출 경로 수정
- **동작 불변**: ✅ 로직 이동만, 조건문/상태 동일

### Phase 2: FS-as-Filter 구현 (P2 - 권장)

**목표**: FS가 Physical/Logical/State 제공하여 ASW가 소비

**Step 2.1: FS 서비스 포트 추가**

```c
/* Port_Api.c - FS 섹션 */

/* Physical: Raw sensor value */
uint16 Fs_Port_GetIgnitionVoltage_mV(void) {
    return g_ADC.Physical.Power.IGN;
}

/* Logical: Interpreted boolean */
uint8 Fs_Port_GetIgnitionState(void) {
    return Bsw_Port_GetIgnitionState();
}

/* State: Derived timing information */
uint32 Fs_Port_GetIgnitionOffDuration_ms(void) {
    return g_PORT.IgnOffCount * 10U;  // 10ms tick → ms
}
```

**Step 2.2: ASW에서 FS 서비스 소비 (선택적)**

```c
/* Main_Api.c - 향후 개선 */
void CheckIgnitionAndUpdateSystem(void) {
    uint8 ignState = Fs_Port_GetIgnitionState();  // ← FS 서비스 호출
    uint32 offDuration = Fs_Port_GetIgnitionOffDuration_ms();

    if(ignState) {
        SetSystemOn();
    } else {
        SetSystemDown();
        if(offDuration > 2000U) {  // 2초 타임아웃
            SetSystemOff();
        }
    }
}
```

**변경 규모**:

- 추가: 3개 FS 서비스 포트 (선택적)
- 변경: ASW 호출 경로 (향후 개선)

### Phase 3: ASW 파일 분리 (P3 - 장기)

**목표**: SetEpbSwitch/GetEpbSwitch를 별도 파일로 분리

**Step 3.1: 파일 생성**

```
EPB/Application/EpbSwitch_Api.c
EPB/Application/EpbSwitch_Api.h
```

**Step 3.2: 함수 이동**

```c
/* EpbSwitch_Api.c */
#include "EpbSwitch_Api.h"

extern SwitchStatus_t g_SwitchStatus;  // Port_Api.c에서 extern

void SetEpbSwitch(uint8 index, uint8 bOnOff) {
    // (기존 로직 이동)
}

uint8 GetEpbSwitch(uint8 index) {
    // (기존 로직 이동)
}
```

**변경 규모**:

- 추가: 2개 파일 (EpbSwitch_Api.c/h)
- 삭제: Port_Api.c ASW 섹션
- 변경: 빌드 설정 (새 파일 컴파일 추가)

---

## 7. 권장 사항 요약

### 7.1 즉시 조치 필요 (P1)

| 항목                        | 조치                                                                                      | 예상 시간 | 위험도             |
| --------------------------- | ----------------------------------------------------------------------------------------- | --------- | ------------------ |
| **BSW → ASW 호출 제거**     | `Bsw_Port_UpdateIgnitionLogic` 함수를 FS로 이동<br>이름: `Fs_Port_ManageIgnitionState`    | 30분      | 🟢 Low (이동만)    |
| **BSW 정책 로직 제거**      | `Bsw_Port_UpdateMotorDriverControl` 함수를 FS로 이동<br>이름: `Fs_Port_ManageMotorDriver` | 15분      | 🟢 Low (이동만)    |
| **Fs_Port_MonitorAll 수정** | BSW 호출 → FS 호출로 변경                                                                 | 5분       | 🟢 Low (호출 수정) |

**총 예상 시간**: 50분  
**동작 불변 보장**: ✅ 로직 이동만, 조건문/상태 동일  
**컴파일 검증**: 필수  
**테스트**: IGN ON/OFF, 모터 드라이버 제어 검증

### 7.2 권장 조치 (P2)

| 항목                    | 조치                                                                                                       | 예상 시간 | 효과                   |
| ----------------------- | ---------------------------------------------------------------------------------------------------------- | --------- | ---------------------- |
| **FS 서비스 포트 추가** | `Fs_Port_GetIgnitionVoltage_mV()`<br>`Fs_Port_GetIgnitionState()`<br>`Fs_Port_GetIgnitionOffDuration_ms()` | 30분      | FS-as-Filter 구조 달성 |
| **ASW 호출 경로 개선**  | Main_Api.c에서 FS 서비스 소비 (선택적)                                                                     | 1시간     | ASW 독립성 향상        |

**총 예상 시간**: 1.5시간  
**효과**: FS 레이어 의미 부여, ASW 테스트 용이성 향상

### 7.3 장기 조치 (P3)

| 항목               | 조치                                                     | 예상 시간 | 효과               |
| ------------------ | -------------------------------------------------------- | --------- | ------------------ |
| **ASW 파일 분리**  | EpbSwitch_Api.c/h 생성<br>SetEpbSwitch/GetEpbSwitch 이동 | 2시간     | 레이어 구조 명확화 |
| **빌드 설정 수정** | Makefile/프로젝트 파일 업데이트                          | 30분      | -                  |

**총 예상 시간**: 2.5시간  
**효과**: AUTOSAR 표준 레이어 구조 완성

---

## 8. 결론

### 8.1 현재 레이어링 평가

| 레이어   | 평가        | 근거                                          |
| -------- | ----------- | --------------------------------------------- |
| **MCAL** | ✅ **적합** | 순수 HW 접근만, 비즈니스 로직 없음            |
| **BSW**  | ❌ **위반** | SetSystemOn/Off 호출, 정책 결정 (FS 책임)     |
| **FS**   | ⚠️ **약함** | Pass-through만, 실제 Filter/Gate 역할 안함    |
| **ASW**  | ✅ **적합** | EPB 도메인 로직, RAM 기반, 레지스터 접근 없음 |

### 8.2 주요 발견 사항

1. ✅ **SWITCH*EPB*\* 매크로**: RAM 기반, 레지스터 접근 없음 → ASW 분류 타당
2. ❌ **BSW → ASW 호출**: `Bsw_Port_UpdateIgnitionLogic` → `SetSystemOn/Off` (레이어 위반)
3. ❌ **BSW 정책 소유**: `++g_PORT.IgnOffCount > IGN_OFF_TIME` (FS 책임)
4. ⚠️ **FS 역할 미흡**: Pass-through만, Physical/Logical/State 미제공

### 8.3 최종 권장 사항

**우선순위 1 (필수)**:

- BSW 정책 함수 2개를 FS로 이동 (50분 작업)
- 동작 불변 보장 (로직 이동만)

**우선순위 2 (권장)**:

- FS 서비스 포트 3개 추가 (30분 작업)
- FS-as-Filter 구조 달성

**우선순위 3 (장기)**:

- ASW 파일 분리 (2.5시간 작업)
- AUTOSAR 표준 레이어 구조 완성

**총 예상 시간**: Phase 1(50분) + Phase 2(1.5시간) + Phase 3(2.5시간) = **4.5시간**

---

## 부록: 증거 수집 로그

### 검색 키워드 및 결과

| 키워드                                     | 결과 수 | 주요 발견                                  |
| ------------------------------------------ | ------- | ------------------------------------------ |
| `SetSystemOn\|SetSystemDown\|SetSystemOff` | 20+     | Main_Api.c 정의, Port_Api.c BSW에서 호출   |
| `SWITCH_EPB_STATUS\|SWITCH_EPB_SW1`        | 10      | Port_Api.h 매크로 정의, RAM 기반           |
| `Physical\.\|Logical\.\|SSMInput`          | 20+     | g_ADC.Physical.\*, SSMInput.SSMInIgnition  |
| `g_PORT\.`                                 | 30+     | IgnOffCount, SUB_EPB_SBC_FAULT             |
| `g_TaskControl`                            | 20+     | Main_Api.c static 변수, 시스템 상태 플래그 |

### 파일 읽기 로그

1. `Port_Api.c:1-100` - MCAL/BSW 섹션
2. `Port_Api.c:100-284` - BSW/FS/ASW/Legacy 섹션
3. `Port_Api.h:110-170` - Port_t, SwitchStatus_t 정의
4. `Main_Api.c:1-60` - SetSystemOn/Down/Off 정의

### 프로젝트 구조 검증

- ✅ MCAL: 순수 HW 접근 (PCR/GPDO/GPDI)
- ❌ BSW: ASW 함수 호출 (레이어 위반 확인)
- ⚠️ FS: Pass-through (역할 미흡 확인)
- ✅ ASW: RAM 기반, 도메인 로직 (적합 확인)

---

**문서 버전**: 1.0  
**작성일**: 2026-01-14  
**검증자**: BSW Team  
**다음 단계**: Phase 1 리팩터링 실행 (50분 예상)
