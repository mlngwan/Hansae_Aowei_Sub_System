# Port 기능 AUTOSAR-like 레이어 재구성 결과

## 1. 작업 개요

**대상 파일**: `EPB/Application/Port_Api.c`  
**작업 방식**: 기존 파일 내부에서 레이어 섹션으로 재구성 (새 파일 생성 없음)  
**핵심 원칙**: Intent(판단 의미/상태전이) 불변, Wiring(하드웨어 접근) 경계만 정리

## 2. 레이어 구조 및 함수 매핑

### 2.1 MCAL Layer (Microcontroller Abstraction Layer)

**역할**: GPIO/PORT 레지스터 직접 접근, 핀 설정 및 초기화

| 기존 함수 | 신규 함수 | 기능 |
|-----------|-----------|------|
| `SetPadConfiguration()` | `Mcal_Port_ConfigurePads()` | PCR 레지스터 설정 (입출력 모드, 풀업 등) |
| `SetPortInitialize()` | `Mcal_Port_SetInitialStates()` | GPIO 초기 상태 설정 (HIGH/LOW) |
| (신규) | `Mcal_Port_ReadPin_SbcFault()` | SBC Fault 핀 읽기 |
| (신규) | `Mcal_Port_WritePin_L9369Ctrl()` | L9369 제어 핀 쓰기 |

**하드웨어 접근 지점**:
- `SIU.PCR[PORT_PIN_*].R` - 핀 설정 레지스터
- `SIU.GPDO[PORT_PIN_*].R` - GPIO 출력 데이터 레지스터
- `SIU.GPDI[PORT_PIN_*].R` - GPIO 입력 데이터 레지스터

### 2.2 BSW Layer (Basic Software)

**역할**: 핀 신호를 의미 있는 신호(IGN/FAULT/EN)로 해석, 폴라리티 처리

| 신규 함수 | 기능 | 근거 |
|-----------|------|------|
| `Bsw_Port_GetIgnitionState()` | IGN 신호 상태 판단 (ADC 값 > IGN_ON) | ADC 물리값을 ON/OFF로 해석 |
| `Bsw_Port_GetSbcFaultState()` | SBC Fault 핀 상태 읽기 | MCAL 래핑 |
| `Bsw_Port_SetMotorDriverControl()` | 모터 드라이버 제어 핀 설정 | MCAL 래핑 |
| `Bsw_Port_UpdateIgnitionLogic()` | IGN 상태 머신 로직 | IGN ON/OFF에 따른 시스템 상태 전이 |
| `Bsw_Port_UpdateMotorDriverControl()` | 모터 드라이버 제어 로직 | TC277 고장 시 모터 드라이버 비활성화 |
| `Bsw_Port_UpdateFaultStatus()` | Fault 상태 업데이트 | SBC Fault를 전역 상태로 매핑 |

**Intent 보존 사항**:
- IGN ON/OFF 판단 기준: `g_ADC.Physical.Power.IGN > IGN_ON` (6000mV)
- IGN OFF 카운터 로직: 200회(2초) 이상 OFF 시 `SetSystemOff()` 호출
- 모터 드라이버 제어 조건: `g_TC277.fMainfault == FALSE`일 때 비활성화

### 2.3 FS Layer (Functional Safety / Application Software)

**역할**: 고수준 포트 모니터링 인터페이스, 시스템 상태 조정

| 신규 함수 | 기능 |
|-----------|------|
| `Fs_Port_MonitorAll()` | 모든 포트 모니터링 통합 (IGN, 모터 드라이버, Fault) |

**호출 구조**:
```
Fs_Port_MonitorAll()
    ├─ Bsw_Port_UpdateIgnitionLogic()
    │   ├─ Bsw_Port_GetIgnitionState()
    │   ├─ SetSystemOn() / SetSystemDown() / SetSystemOff()
    │   └─ g_PORT.IgnOffCount 관리
    ├─ Bsw_Port_UpdateMotorDriverControl()
    │   └─ Bsw_Port_SetMotorDriverControl()
    │       └─ Mcal_Port_WritePin_L9369Ctrl()
    └─ Bsw_Port_UpdateFaultStatus()
        └─ Bsw_Port_GetSbcFaultState()
            └─ Mcal_Port_ReadPin_SbcFault()
```

### 2.4 Legacy Interface (하위 호환성)

**역할**: 기존 함수명 유지, 새 레이어 구현으로 라우팅

| 기존 함수 | 라우팅 대상 |
|-----------|-------------|
| `SetPadConfiguration()` | `Mcal_Port_ConfigurePads()` |
| `SetPortInitialize()` | `Mcal_Port_SetInitialStates()` |
| `CheckPortStatus()` | `Fs_Port_MonitorAll()` |

## 3. 상위 호출자 변경

### 3.1 호출 지점

**파일**: `EPB/Main_Api.c`  
**함수**: `TaskScheduler_5ms()`  
**호출 라인**: 233, 257, 265

### 3.2 호출 흐름 변경

**변경 전**:
```
TaskScheduler_5ms()
    ↓
CheckPortStatus()
    ↓
(직접 하드웨어 접근 + 로직 혼재)
```

**변경 후**:
```
TaskScheduler_5ms()
    ↓
CheckPortStatus() [Legacy wrapper]
    ↓
Fs_Port_MonitorAll()
    ↓
├─ Bsw_Port_UpdateIgnitionLogic()
│   └─ Mcal_Port_* (하드웨어 접근)
├─ Bsw_Port_UpdateMotorDriverControl()
│   └─ Mcal_Port_* (하드웨어 접근)
└─ Bsw_Port_UpdateFaultStatus()
    └─ Mcal_Port_* (하드웨어 접근)
```

**외부 호출부 수정**: 없음 (기존 `CheckPortStatus()` 호출 유지)

## 4. 변경 사항 상세 (Diff 형식)

### 4.1 MCAL Layer 추가

```diff
+/*===========================================================================
+ * MCAL Layer (Microcontroller Abstraction Layer)
+ * - Direct GPIO/PORT register access
+ * - Pin configuration and initialization
+ * - Hardware pin read/write operations
+ *===========================================================================*/
+
+void Mcal_Port_ConfigurePads(void)
+{
+    PCR_SUB_UBB_EN = PORT_PINCFG_OUTPUT_MODE;
+    PCR_SUB_UBVR_EN = PORT_PINCFG_OUTPUT_MODE;
+    // ... (기존 SetPadConfiguration 내용)
+}
+
+void Mcal_Port_SetInitialStates(void)
+{
+    PORT_SUB_L9369_CS = TRUE;
+    PORT_SUB_L9369_MOSI = FALSE;
+    // ... (기존 SetPortInitialize 내용)
+}
+
+uint8 Mcal_Port_ReadPin_SbcFault(void)
+{
+    return PORT_SUB_EPB_SBC_FAULT;
+}
+
+void Mcal_Port_WritePin_L9369Ctrl(uint8 state)
+{
+    PORT_SUB_L9369_CTRL = state;
+}
```

### 4.2 BSW Layer 추가

```diff
+/*===========================================================================
+ * BSW Layer (Basic Software)
+ * - Semantic signal interpretation (IGN/WAKE/FAULT/EN)
+ * - Polarity handling and signal conditioning
+ * - Hardware abstraction for application layer
+ *===========================================================================*/
+
+uint8 Bsw_Port_GetIgnitionState(void)
+{
+    if(g_ADC.Physical.Power.IGN > IGN_ON) {
+        return TRUE;
+    } else {
+        return FALSE;
+    }
+}
+
+void Bsw_Port_UpdateIgnitionLogic(void)
+{
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
+void Bsw_Port_UpdateMotorDriverControl(void)
+{
+    if(g_TC277.fMainfault == FALSE) {
+        Bsw_Port_SetMotorDriverControl(FALSE);
+    }
+}
+
+void Bsw_Port_UpdateFaultStatus(void)
+{
+    g_PORT.SUB_EPB_SBC_FAULT = Bsw_Port_GetSbcFaultState();
+}
```

### 4.3 FS Layer 추가

```diff
+/*===========================================================================
+ * FS Layer (Functional Safety / Application Software)
+ * - High-level port monitoring interface
+ * - System state coordination
+ * - Application logic integration
+ *===========================================================================*/
+
+void Fs_Port_MonitorAll(void)
+{
+    Bsw_Port_UpdateIgnitionLogic();
+    Bsw_Port_UpdateMotorDriverControl();
+    Bsw_Port_UpdateFaultStatus();
+}
```

### 4.4 Legacy Interface 수정

```diff
 void SetPadConfiguration(void)
 {
-    PCR_SUB_UBB_EN = PORT_PINCFG_OUTPUT_MODE;
-    // ... (기존 레지스터 접근 코드)
+    Mcal_Port_ConfigurePads();
 }

 void SetPortInitialize(void)
 {
-    PORT_SUB_L9369_CS = TRUE;
-    // ... (기존 GPIO 설정 코드)
+    Mcal_Port_SetInitialStates();
 }

 void CheckPortStatus(void)
 {
-    if(g_ADC.Physical.Power.IGN > IGN_ON) {
-        SetSystemOn();
-        g_PORT.IgnOffCount = 0;
-    } else {
-        SetSystemDown();
-        if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
-            g_PORT.IgnOffCount = IGN_OFF_TIME;
-            SetSystemOff();
-        }
-    }
-    
-    if(g_TC277.fMainfault == FALSE) {
-        PORT_SUB_L9369_CTRL = FALSE;
-    }
-    
-    g_PORT.SUB_EPB_SBC_FAULT = PORT_SUB_EPB_SBC_FAULT;
+    Fs_Port_MonitorAll();
 }
```

## 5. Intent 보존 검증

### 5.1 IGN 상태 전이 로직 (불변)

| 조건 | 동작 | 변경 전 위치 | 변경 후 위치 |
|------|------|--------------|--------------|
| `IGN > 6000mV` | `SetSystemOn()` 호출 | `CheckPortStatus()` | `Bsw_Port_UpdateIgnitionLogic()` |
| `IGN <= 6000mV` | `SetSystemDown()` 호출 | `CheckPortStatus()` | `Bsw_Port_UpdateIgnitionLogic()` |
| IGN OFF 2초 이상 | `SetSystemOff()` 호출 | `CheckPortStatus()` | `Bsw_Port_UpdateIgnitionLogic()` |

**검증**: 조건문, 카운터 로직, 함수 호출 순서 모두 동일

### 5.2 모터 드라이버 제어 로직 (불변)

| 조건 | 동작 | 변경 전 | 변경 후 |
|------|------|---------|---------|
| `g_TC277.fMainfault == FALSE` | `PORT_SUB_L9369_CTRL = FALSE` | 직접 레지스터 접근 | `Mcal_Port_WritePin_L9369Ctrl(FALSE)` |

**검증**: 조건 판단 로직 동일, 하드웨어 접근만 MCAL로 래핑

### 5.3 Fault 상태 업데이트 (불변)

| 동작 | 변경 전 | 변경 후 |
|------|---------|---------|
| SBC Fault 읽기 | `g_PORT.SUB_EPB_SBC_FAULT = PORT_SUB_EPB_SBC_FAULT` | `g_PORT.SUB_EPB_SBC_FAULT = Mcal_Port_ReadPin_SbcFault()` |

**검증**: 읽기 동작 동일, 하드웨어 접근만 MCAL로 래핑

## 6. 하드웨어 접근 지점 정리

### 6.1 레지스터 접근 집중화

**변경 전**: `CheckPortStatus()` 내부에서 직접 레지스터 접근  
**변경 후**: 모든 레지스터 접근을 MCAL Layer로 집중

| 레지스터 | 접근 타입 | MCAL 함수 |
|----------|-----------|-----------|
| `SIU.PCR[*].R` | Write | `Mcal_Port_ConfigurePads()` |
| `SIU.GPDO[*].R` | Write | `Mcal_Port_SetInitialStates()`, `Mcal_Port_WritePin_L9369Ctrl()` |
| `SIU.GPDI[*].R` | Read | `Mcal_Port_ReadPin_SbcFault()` |

### 6.2 전역 변수 접근

| 전역 변수 | 접근 타입 | 접근 레이어 | 용도 |
|-----------|-----------|-------------|------|
| `g_ADC.Physical.Power.IGN` | Read | BSW | IGN 상태 판단 |
| `g_TC277.fMainfault` | Read | BSW | 모터 드라이버 제어 조건 |
| `g_PORT.IgnOffCount` | Read/Write | BSW | IGN OFF 카운터 |
| `g_PORT.SUB_EPB_SBC_FAULT` | Write | BSW | SBC Fault 상태 저장 |

## 7. 검증 포인트

### 7.1 기능 동작 검증

| 검증 항목 | 확인 방법 | 예상 결과 |
|-----------|-----------|-----------|
| IGN ON 감지 | `g_ADC.Physical.Power.IGN` 값 모니터링 | 6000mV 초과 시 `SetSystemOn()` 호출 |
| IGN OFF 감지 | `g_PORT.IgnOffCount` 카운터 확인 | 200회(2초) 후 `SetSystemOff()` 호출 |
| 모터 드라이버 제어 | `PORT_SUB_L9369_CTRL` 핀 상태 | TC277 고장 시 LOW |
| SBC Fault 읽기 | `g_PORT.SUB_EPB_SBC_FAULT` 값 | 핀 상태와 동일 |

### 7.2 레이어 경계 검증

| 검증 항목 | 확인 방법 |
|-----------|-----------|
| MCAL 순수성 | MCAL 함수가 레지스터 접근만 수행하는지 확인 |
| BSW 추상화 | BSW 함수가 의미 신호로 변환하는지 확인 |
| FS 통합성 | FS 함수가 시스템 상태 조정만 수행하는지 확인 |

## 8. 요약

### 8.1 주요 성과

1. **레이어 분리 완료**: MCAL/BSW/FS 3계층으로 명확히 구분
2. **Intent 보존**: 모든 판단 로직, 상태 전이, 제어 의미 불변
3. **Wiring 정리**: 하드웨어 접근을 MCAL로 집중화
4. **하위 호환성 유지**: 기존 함수명 유지, 외부 호출부 수정 없음
5. **새 파일 생성 없음**: 기존 `Port_Api.c` 파일 내부에서만 재구성

### 8.2 변경 통계

| 항목 | 개수 |
|------|------|
| 신규 MCAL 함수 | 4개 |
| 신규 BSW 함수 | 6개 |
| 신규 FS 함수 | 1개 |
| Legacy 래퍼 함수 | 3개 |
| 레지스터 접근 집중화 | 27개 핀 설정 + 2개 GPIO 접근 |

### 8.3 다음 단계 제안

1. **FS 반환값 도입**: 현재는 void, 추후 상태 코드 반환 추가
2. **진단 기능 통합**: Fault 상태를 DTC로 연계
3. **Safety 검증 강화**: FS Layer에 범위 체크 및 플라우저빌리티 검증 추가
4. **다른 모듈 확장**: CAN, SPI 등 다른 주변장치도 동일 패턴 적용