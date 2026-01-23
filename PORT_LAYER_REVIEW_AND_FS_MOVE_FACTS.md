# Port 모듈 레이어 정리 및 FS 이동 대상 근거 분석

## 전제 조건

**팀 역할**: BSW 팀  
**작업 범위**: ASW(EPB Switch 로직)는 건드리지 않고, BSW/MCAL/FS 레이어 경계만 정리  
**목표**: ASW가 하드웨어에 직접 의존하지 않고 FS/BSW를 통해 입력/제어를 받도록 구조 정리

## 1. 레이어 책임 정리 (근거 기반)

### 1.1 MCAL Layer - 하드웨어 직접 접근

**정의**: PORT/PCR 레지스터 및 GPIO 핀에 대한 직접 접근만 수행

| 함수명 | 하드웨어 접근 | 근거 |
|--------|---------------|------|
| `Mcal_Port_ConfigurePads()` | `SIU.PCR[PORT_PIN_*].R` 27개 핀 설정 | PCR 레지스터 직접 쓰기 |
| `Mcal_Port_SetInitialStates()` | `SIU.GPDO[PORT_PIN_*].R` 13개 핀 초기화 | GPIO 출력 레지스터 직접 쓰기 |
| `Mcal_Port_ReadPin_SbcFault()` | `SIU.GPDI[PORT_PIN_E10].R` 읽기 | GPIO 입력 레지스터 직접 읽기 |
| `Mcal_Port_WritePin_L9369Ctrl()` | `SIU.GPDO[PORT_PIN_A5].R` 쓰기 | GPIO 출력 레지스터 직접 쓰기 |

**MCAL 순수성 검증**:
- ✅ 레지스터 접근만 수행
- ✅ 비즈니스 로직 없음
- ✅ 상태 판단 없음
- ✅ 시스템 상태 변경 없음

### 1.2 BSW Layer - 하드웨어 추상화 서비스

**정의**: 하드웨어 값을 의미 있는 신호로 해석하는 서비스 제공

| 함수명 | 서비스 내용 | 근거 |
|--------|-------------|------|
| `Bsw_Port_GetIgnitionState()` | ADC 값(`g_ADC.Physical.Power.IGN`)을 IGN ON/OFF로 해석 | 하드웨어 값 → 의미 신호 변환 |
| `Bsw_Port_GetSbcFaultState()` | SBC Fault 핀 상태 읽기 서비스 | MCAL 래핑 |
| `Bsw_Port_SetMotorDriverControl()` | 모터 드라이버 제어 핀 설정 서비스 | MCAL 래핑 |

**BSW 책임 범위**:
- ✅ 하드웨어 값 읽기/쓰기 추상화
- ✅ 신호 의미 해석 (예: ADC 값 > 6000mV → IGN ON)
- ✅ 폴라리티 처리
- ❌ 시스템 상태 전이 (FS 책임)
- ❌ 정책 판단 (FS 책임)

### 1.3 FS Layer - 시스템 상태 관리 및 정책

**정의**: 시스템 상태 전이, 정책 판단, ASW 입력 제공

| 함수명 | 현재 위치 | 책임 내용 | 문제점 |
|--------|-----------|-----------|--------|
| `Fs_Port_MonitorAll()` | FS | 전체 포트 모니터링 통합 | ✅ 적절 |
| `Bsw_Port_UpdateIgnitionLogic()` | **BSW** | IGN 상태 머신, 시스템 상태 전이 | ❌ **FS로 이동 필요** |
| `Bsw_Port_UpdateMotorDriverControl()` | **BSW** | 모터 드라이버 제어 정책 | ❌ **FS로 이동 필요** |
| `Bsw_Port_UpdateFaultStatus()` | **BSW** | Fault 상태 업데이트 | ⚠️ BSW 유지 가능 (단순 매핑) |

### 1.4 ASW Layer - 애플리케이션 로직

**정의**: EPB 기능 관련 애플리케이션 로직

| 함수명 | 로직 내용 | ASW인 근거 |
|--------|-----------|------------|
| `SetEpbSwitch()` | EPB 스위치 상태 인코딩 및 Apply/Release 판단 | 1. EPB 도메인 지식 필요<br>2. 비트 마스킹 및 상태 조합 로직<br>3. NEUTRAL/APPLY/RELEASE 의미 해석<br>4. 애플리케이션 상태 관리 |
| `GetEpbSwitch()` | EPB 스위치 상태 디코딩 | 1. EPB 도메인 지식 필요<br>2. 스위치 인덱스 매핑<br>3. 애플리케이션 상태 조회 |

**ASW 유지 근거**:
1. **도메인 로직**: EPB 스위치 조합(SW1~SW7)의 의미는 EPB 애플리케이션 고유 지식
2. **상태 인코딩**: 비트 조합을 NEUTRAL/APPLY/RELEASE로 해석하는 것은 EPB 기능 로직
3. **BSW 범위 초과**: BSW는 하드웨어 추상화만 담당, 애플리케이션 의미 해석은 ASW 책임
4. **재사용성**: 다른 프로젝트에서 EPB 스위치 로직은 재사용 불가 (EPB 전용)

## 2. FS로 이동해야 하는 코드 식별

### 2.1 이동 대상 #1: `Bsw_Port_UpdateIgnitionLogic()`

**현재 위치**: BSW Layer  
**현재 코드**:
```c
void Bsw_Port_UpdateIgnitionLogic(void)
{
	/* IGN state machine logic */
	if(Bsw_Port_GetIgnitionState()) {
		SetSystemOn();
		g_PORT.IgnOffCount = 0;
	} 
	else {
		SetSystemDown();
		if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
			g_PORT.IgnOffCount = IGN_OFF_TIME;
			SetSystemOff();
		}
	}
}
```

**FS로 이동해야 하는 이유**:

| 근거 | 설명 |
|------|------|
| **시스템 상태 전이** | `SetSystemOn()`, `SetSystemDown()`, `SetSystemOff()` 호출 → 시스템 전체 상태 변경 |
| **정책 판단** | IGN OFF 2초(200회) 후 시스템 종료 → 시스템 정책 |
| **상태 관리** | `g_PORT.IgnOffCount` 카운터 관리 → 시스템 상태 변수 |
| **BSW 범위 초과** | BSW는 "IGN이 ON인가?"만 답해야 함. "시스템을 켜라/끄라"는 FS 책임 |

**이동 시 동작 불변 근거**:
- ✅ 조건문 로직 동일: `if(Bsw_Port_GetIgnitionState())`
- ✅ 카운터 로직 동일: `++g_PORT.IgnOffCount > IGN_OFF_TIME`
- ✅ 함수 호출 순서 동일: `SetSystemOn()` → `SetSystemDown()` → `SetSystemOff()`
- ✅ 타이밍 동일: 5ms 주기 호출 유지

### 2.2 이동 대상 #2: `Bsw_Port_UpdateMotorDriverControl()`

**현재 위치**: BSW Layer  
**현재 코드**:
```c
void Bsw_Port_UpdateMotorDriverControl(void)
{
	/* Motor driver control based on TC277 fault status */
	if(g_TC277.fMainfault == FALSE) {
		Bsw_Port_SetMotorDriverControl(FALSE);
	}
}
```

**FS로 이동해야 하는 이유**:

| 근거 | 설명 |
|------|------|
| **정책 판단** | TC277 고장 시 모터 드라이버 비활성화 → 안전 정책 |
| **시스템 제어** | 모터 드라이버 활성화/비활성화 → 시스템 제어 결정 |
| **외부 상태 의존** | `g_TC277.fMainfault` 참조 → 시스템 전체 상태 기반 판단 |
| **BSW 범위 초과** | BSW는 "모터 드라이버 핀을 HIGH/LOW로 설정"만 담당. "언제 끌지"는 FS 책임 |

**이동 시 동작 불변 근거**:
- ✅ 조건문 로직 동일: `if(g_TC277.fMainfault == FALSE)`
- ✅ 제어 명령 동일: `Bsw_Port_SetMotorDriverControl(FALSE)`
- ✅ 타이밍 동일: 5ms 주기 호출 유지

### 2.3 유지 대상: `Bsw_Port_UpdateFaultStatus()`

**현재 위치**: BSW Layer  
**현재 코드**:
```c
void Bsw_Port_UpdateFaultStatus(void)
{
	/* Update SBC fault status to global state */
	g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
}
```

**BSW 유지 근거**:
- ✅ 단순 매핑: 하드웨어 값 → 전역 상태 복사
- ✅ 정책 없음: 조건 판단이나 상태 전이 없음
- ✅ BSW 책임: 하드웨어 상태를 상위 레이어가 사용 가능한 형태로 제공
- ⚠️ 선택적 FS 이동 가능: 전역 상태 관리 관점에서 FS로 이동 가능하나 필수는 아님

## 3. ASW 연계 관점 정리

### 3.1 ASW 입력/출력 의존성

| ASW 함수 | 참조 데이터 | 데이터 출처 | 레이어 경로 |
|----------|-------------|-------------|-------------|
| `SetEpbSwitch()` | `g_SwitchStatus` (읽기/쓰기) | ASW 내부 상태 | ASW 자체 관리 |
| `GetEpbSwitch()` | `g_SwitchStatus` (읽기) | ASW 내부 상태 | ASW 자체 관리 |

**분석 결과**: 
- ✅ ASW는 현재 하드웨어에 직접 의존하지 않음
- ✅ `g_SwitchStatus`는 ASW 내부 상태로 독립적
- ✅ 외부 입력은 CAN/스위치를 통해 간접적으로 제공됨 (별도 경로)

### 3.2 데이터 흐름 다이어그램

```
┌─────────────────────────────────────────────────────────────────┐
│ Hardware Layer                                                   │
├─────────────────────────────────────────────────────────────────┤
│ - GPIO Pins (IGN, SBC_FAULT, L9369_CTRL)                        │
│ - ADC (IGN voltage)                                              │
│ - TC277 (fMainfault)                                             │
└────────────┬────────────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────────────┐
│ MCAL Layer                                                       │
├─────────────────────────────────────────────────────────────────┤
│ Mcal_Port_ReadPin_SbcFault()    → uint8 (핀 상태)               │
│ Mcal_Port_WritePin_L9369Ctrl()  ← uint8 (제어 명령)             │
└────────────┬────────────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────────────┐
│ BSW Layer                                                        │
├─────────────────────────────────────────────────────────────────┤
│ Bsw_Port_GetIgnitionState()     → TRUE/FALSE (IGN 상태)         │
│ Bsw_Port_GetSbcFaultState()     → uint8 (Fault 상태)            │
│ Bsw_Port_SetMotorDriverControl() ← uint8 (제어 명령)            │
└────────────┬────────────────────────────────────────────────────┘
             │
             ▼
┌─────────────────────────────────────────────────────────────────┐
│ FS Layer (현재 + 이동 예정)                                      │
├─────────────────────────────────────────────────────────────────┤
│ [현재]                                                           │
│ Fs_Port_MonitorAll()            → 통합 모니터링                 │
│                                                                  │
│ [이동 예정]                                                      │
│ Fs_Port_ManageIgnitionState()   → SetSystemOn/Down/Off 호출     │
│ Fs_Port_ManageMotorDriver()     → 모터 드라이버 정책 제어       │
│ Fs_Port_UpdateFaultStatus()     → Fault 상태 관리               │
└────────────┬────────────────────────────────────────────────────┘
             │
             ├──────────────────────────────────────┐
             │                                      │
             ▼                                      ▼
┌──────────────────────────┐      ┌──────────────────────────────┐
│ System State             │      │ ASW Layer                    │
├──────────────────────────┤      ├──────────────────────────────┤
│ g_PORT.IgnOffCount       │      │ SetEpbSwitch()               │
│ g_PORT.SUB_EPB_SBC_FAULT │      │ GetEpbSwitch()               │
│ SSMInput.SSMInIgnition   │      │                              │
│ g_TaskControl.*          │      │ g_SwitchStatus (독립 상태)   │
└──────────────────────────┘      └──────────────────────────────┘
```

### 3.3 ASW 독립성 검증

**현재 상태**:
```
ASW (SetEpbSwitch/GetEpbSwitch)
    ↓
g_SwitchStatus (ASW 내부 상태)
    ↓
(하드웨어 의존 없음)
```

**검증 결과**:
- ✅ ASW는 하드웨어에 직접 접근하지 않음
- ✅ ASW는 FS/BSW를 직접 호출하지 않음
- ✅ ASW는 독립적인 상태(`g_SwitchStatus`)만 관리
- ✅ 레이어 분리 목표 달성

## 4. 수정 가이드 제안

### 4.1 BSW → FS 이동 계획

**Step 1: BSW에서 FS로 함수 이동**

```diff
/*===========================================================================
 * BSW Layer
 *===========================================================================*/

 uint8 Bsw_Port_GetIgnitionState(void)
 {
     if(g_ADC.Physical.Power.IGN > IGN_ON) {
         return TRUE;
     } else {
         return FALSE;
     }
 }

 uint8 Bsw_Port_GetSbcFaultState(void)
 {
     return Mcal_Port_ReadPin_SbcFault();
 }

 void Bsw_Port_SetMotorDriverControl(uint8 enable)
 {
     Mcal_Port_WritePin_L9369Ctrl(enable);
 }

-void Bsw_Port_UpdateIgnitionLogic(void)
-{
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
-    if(g_TC277.fMainfault == FALSE) {
-        Bsw_Port_SetMotorDriverControl(FALSE);
-    }
-}

 void Bsw_Port_UpdateFaultStatus(void)
 {
     g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
 }

/*===========================================================================
 * FS Layer
 *===========================================================================*/

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
+void Fs_Port_ManageMotorDriver(void)
+{
+    /* Motor driver control policy based on system fault status */
+    if(g_TC277.fMainfault == FALSE) {
+        Bsw_Port_SetMotorDriverControl(FALSE);
+    }
+}
+
+void Fs_Port_UpdateFaultStatus(void)
+{
+    /* Update system fault status from BSW */
+    Bsw_Port_UpdateFaultStatus();
+}

 void Fs_Port_MonitorAll(void)
 {
-    Bsw_Port_UpdateIgnitionLogic();
-    Bsw_Port_UpdateMotorDriverControl();
-    Bsw_Port_UpdateFaultStatus();
+    Fs_Port_ManageIgnitionState();
+    Fs_Port_ManageMotorDriver();
+    Fs_Port_UpdateFaultStatus();
 }
```

### 4.2 레이어 책임 명확화

**MCAL Layer (변경 없음)**:
```c
// 하드웨어 직접 접근만 수행
uint8 Mcal_Port_ReadPin_SbcFault(void);
void Mcal_Port_WritePin_L9369Ctrl(uint8 state);
```

**BSW Layer (서비스 제공만)**:
```c
// 하드웨어 값을 의미 신호로 변환하는 서비스
uint8 Bsw_Port_GetIgnitionState(void);        // IGN ON/OFF 판단
uint8 Bsw_Port_GetSbcFaultState(void);        // Fault 상태 읽기
void Bsw_Port_SetMotorDriverControl(uint8);   // 모터 드라이버 제어
void Bsw_Port_UpdateFaultStatus(void);        // Fault 상태 매핑
```

**FS Layer (정책 및 상태 관리)**:
```c
// 시스템 상태 전이 및 정책 판단
void Fs_Port_ManageIgnitionState(void);   // IGN 상태 머신 + SetSystemOn/Down/Off
void Fs_Port_ManageMotorDriver(void);     // 모터 드라이버 제어 정책
void Fs_Port_UpdateFaultStatus(void);     // 시스템 Fault 상태 관리
void Fs_Port_MonitorAll(void);            // 통합 모니터링
```

**ASW Layer (변경 없음)**:
```c
// EPB 애플리케이션 로직 (독립 유지)
void SetEpbSwitch(uint8 index, uint8 bOnOff);
uint8 GetEpbSwitch(uint8 index);
```

### 4.3 호출 흐름 변경

**변경 전**:
```
TaskScheduler_5ms()
    ↓
CheckPortStatus()
    ↓
Fs_Port_MonitorAll()
    ↓
├─ Bsw_Port_UpdateIgnitionLogic() [BSW에서 시스템 상태 변경 ❌]
├─ Bsw_Port_UpdateMotorDriverControl() [BSW에서 정책 판단 ❌]
└─ Bsw_Port_UpdateFaultStatus()
```

**변경 후**:
```
TaskScheduler_5ms()
    ↓
CheckPortStatus()
    ↓
Fs_Port_MonitorAll()
    ↓
├─ Fs_Port_ManageIgnitionState() [FS에서 시스템 상태 관리 ✅]
│   └─ Bsw_Port_GetIgnitionState() [BSW는 신호만 제공]
├─ Fs_Port_ManageMotorDriver() [FS에서 정책 판단 ✅]
│   └─ Bsw_Port_SetMotorDriverControl() [BSW는 제어만 수행]
└─ Fs_Port_UpdateFaultStatus() [FS에서 상태 관리 ✅]
    └─ Bsw_Port_UpdateFaultStatus() [BSW는 매핑만 수행]
```

### 4.4 동작 불변 검증 체크리스트

| 검증 항목 | 변경 전 | 변경 후 | 동일성 |
|-----------|---------|---------|--------|
| IGN ON 조건 | `IGN > 6000mV` | `IGN > 6000mV` | ✅ |
| IGN OFF 카운터 | 200회(2초) | 200회(2초) | ✅ |
| SetSystemOn 호출 | IGN ON 시 | IGN ON 시 | ✅ |
| SetSystemOff 호출 | IGN OFF 2초 후 | IGN OFF 2초 후 | ✅ |
| 모터 드라이버 제어 | TC277 고장 시 OFF | TC277 고장 시 OFF | ✅ |
| Fault 상태 업데이트 | 핀 상태 → 전역 변수 | 핀 상태 → 전역 변수 | ✅ |
| 호출 주기 | 5ms | 5ms | ✅ |

## 5. 요약

### 5.1 레이어 책임 재정의

| 레이어 | 책임 | 예시 |
|--------|------|------|
| **MCAL** | 하드웨어 직접 접근 | 레지스터 읽기/쓰기 |
| **BSW** | 하드웨어 추상화 서비스 | IGN ON/OFF 판단, 핀 제어 서비스 |
| **FS** | 시스템 상태 관리 및 정책 | SetSystemOn/Off 호출, 모터 드라이버 정책 |
| **ASW** | 애플리케이션 로직 | EPB 스위치 상태 관리 (독립) |

### 5.2 FS 이동 대상 요약

| 함수 | 현재 위치 | 이동 후 | 이동 이유 |
|------|-----------|---------|-----------|
| `Bsw_Port_UpdateIgnitionLogic()` | BSW | `Fs_Port_ManageIgnitionState()` | 시스템 상태 전이 + 정책 판단 |
| `Bsw_Port_UpdateMotorDriverControl()` | BSW | `Fs_Port_ManageMotorDriver()` | 안전 정책 + 시스템 제어 |
| `Bsw_Port_UpdateFaultStatus()` | BSW | `Fs_Port_UpdateFaultStatus()` (선택) | 시스템 상태 관리 |

### 5.3 ASW 독립성 유지

- ✅ ASW(`SetEpbSwitch`, `GetEpbSwitch`)는 변경 없음
- ✅ ASW는 하드웨어에 직접 의존하지 않음
- ✅ ASW는 독립적인 상태(`g_SwitchStatus`)만 관리
- ✅ FS/BSW 레이어 정리가 ASW에 영향 없음

### 5.4 BSW 팀 작업 범위

**수행 작업**:
1. BSW에서 FS로 함수 이동 (정책/상태 관리 코드)
2. BSW는 순수 서비스 제공 레이어로 정리
3. FS는 시스템 상태 관리 레이어로 강화

**수행하지 않는 작업**:
1. ASW 로직 변경 (EPB Switch 관련)
2. Intent 변경 (조건문, 계산식, 상태 전이 로직)
3. 새 파일/폴더 생성

**결과**:
- ASW는 하드웨어 독립적으로 유지
- BSW는 순수 하드웨어 추상화 서비스 제공
- FS는 시스템 정책 및 상태 관리 담당
- 레이어 경계 명확화로 유지보수성 향상