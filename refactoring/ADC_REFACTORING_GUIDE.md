# ADC 센서 처리 리팩토링 가이드

## 1. 개요

본 문서는 ADC 센서 처리 기능을 FS/BSW/MCAL 레이어로 분리하는 리팩토링 가이드입니다.

**리팩토링 원칙:**

- **Intent 불변**: 평균/필터링/스케일링 알고리즘은 변경하지 않음
- **Wiring 분리**: 하드웨어 접근 코드만 MCAL로 이동
- **Safety 추가**: FS 레이어에서 범위 검증 및 진단 기능 제공
- **최소 변경**: ASW는 함수 호출만 치환 (로직 수정 없음)

---

## 2. 레이어 구조 및 역할

```
┌─────────────────────────────────────────────────────────┐
│ ASW (Application Software)                              │
│ - CheckAdcStatus(), GetRawData(), SetLogicData()       │
│ - 기존 로직 유지, FS API 호출로만 변경                    │
└────────────────┬────────────────────────────────────────┘
                 │ Fs_Adc_GetPowerVoltages_Safe()
                 │ Fs_Adc_GetMotorMeasurements_Safe()
┌────────────────▼────────────────────────────────────────┐
│ FS (Functional Safety Layer)                            │
│ - 범위 검증 (6V-18V automotive range)                    │
│ - Timeout 감지 및 DTC 생성                               │
│ - Safe fallback values 제공                             │
│ - 진단 카운터 관리                                        │
└────────────────┬────────────────────────────────────────┘
                 │ Bsw_Adc_GetPowerVoltages()
                 │ Bsw_Adc_ConvertToVoltage()
┌────────────────▼────────────────────────────────────────┐
│ BSW (Basic Software)                                     │
│ - GetAdcAverage() 알고리즘 보존 (6샘플, 최대/최소 제거)    │
│ - ConvertToPhysicalBatVolt() 공식 보존                   │
│ - 논리 채널 → 물리 채널 매핑                              │
└────────────────┬────────────────────────────────────────┘
                 │ Mcal_Adc_ReadChannel()
                 │ Mcal_Adc_ReadL9369Adc()
┌────────────────▼────────────────────────────────────────┐
│ MCAL (Microcontroller Abstraction Layer)                │
│ - ADC 레지스터 직접 접근 (ADC.MCR, ADC.CDR)               │
│ - L9369 글로벌 변수 접근                                  │
│ - Timeout 플래그 관리                                     │
└─────────────────────────────────────────────────────────┘
```

---

## 3. 호출 흐름 예시

### 3.1 전원 전압 읽기 (IGN, UBB, UBVR)

**기존 (As-Is):**

```c
/* EPB/Peripheral/ADC/Adc_Api.c:134 */
void CheckAdcStatus(void)
{
    GetRawData();  /* 내부에서 GetADCdata(), GetAdcAverage() 호출 */
    SetLogicData();
}

/* EPB/Peripheral/ADC/Adc_Api.c:104 */
void GetRawData(void)
{
    g_ADC.Raw.UBB = GetAdcAverage(ADC_CH_SUB_UBB_MON);
    g_ADC.Raw.IGN = GetAdcAverage(ADC_CH_SUB_IGN_MON);
    g_ADC.Raw.UBVR = GetAdcAverage(ADC_CH_SUB_UBVR_MON);

    g_ADC.Physical.Power.UBB = ConvertToPhysicalBatVolt(g_ADC.Raw.UBB);
    g_ADC.Physical.Power.IGN = ConvertToPhysicalBatVolt(g_ADC.Raw.IGN);
    g_ADC.Physical.Power.UBVR = ConvertToPhysicalBatVolt(g_ADC.Raw.UBVR);
}
```

**리팩토링 후 (To-Be):**

```c
/* EPB/Peripheral/ADC/Adc_Api.c:134 (수정) */
void CheckAdcStatus(void)
{
    GetRawData_Refactored();  /* FS API 호출로 변경 */
    SetLogicData();  /* 변경 없음 */
}

/* EPB/Peripheral/ADC/Adc_Api.c:104 (수정) */
void GetRawData_Refactored(void)
{
    Fs_Adc_StatusType fsStatus;

    /* FS 레이어 호출 (Safety-wrapped) */
    fsStatus = Fs_Adc_GetPowerVoltages_Safe(
        &g_ADC.Physical.Power.IGN,
        &g_ADC.Physical.Power.UBB,
        &g_ADC.Physical.Power.UBVR
    );

    /* 에러 처리 (기존 타임아웃 플래그와 동일) */
    if (fsStatus == FS_ADC_TIMEOUT || fsStatus == FS_ADC_SAFE_STATE) {
        g_ADC.Flag.Timeout = 1;
    } else {
        g_ADC.Flag.Timeout = 0;
    }
}
```

**내부 호출 체인:**

```
ASW: GetRawData_Refactored()
  ↓
FS: Fs_Adc_GetPowerVoltages_Safe()
  ├─ 1. Bsw_Adc_GetPowerVoltages()
  │    ├─ Bsw_Adc_ReadAveraged(BSW_ADC_CH_IGN_VOLTAGE, 6, &raw)
  │    │    ├─ [6회 루프] Mcal_Adc_ReadChannel(MCAL_ADC_CH_SUB_IGN_MON, &sample)
  │    │    │    └─ ADC.NCMR[x].R = ... (레지스터 접근)
  │    │    │    └─ ADC.MCR.B.NSTART = 1
  │    │    │    └─ while(ADC.MSR.B.NSTART) { timeout check }
  │    │    │    └─ ADC.CDR[channel].R 읽기
  │    │    └─ [필터링] max/min 제거 후 평균 계산
  │    └─ Bsw_Adc_ConvertToVoltage(raw, &BSW_ADC_PARAMS_IGN, &voltage_mv)
  │         └─ V_real = (ADC/1023) * Vref * ((R1+R2)/R2)
  ├─ 2. Mcal_Adc_GetTimeoutFlag() 체크
  └─ 3. Fs_Adc_ValidateVoltageRange() (6V-18V 검증)
       └─ 범위 초과 시 safe_value_mv=12000 적용
```

### 3.2 모터 전압/전류 읽기 (L9369)

**기존 (As-Is):**

```c
/* EPB/Peripheral/ADC/Adc_Api.c:117-120 */
void GetRawData(void)
{
    /* ... 전원 전압 처리 ... */

    g_ADC.Physical.Motor.MOTORA = EpbMotorVoltage(0);  /* Left motor */
    g_ADC.Physical.Motor.MOTORB = EpbMotorVoltage(1);  /* Right motor */
    g_ADC.Physical.Motor.MOTORA_CUR = EpbMotorCurrent(0);
    g_ADC.Physical.Motor.MOTORB_CUR = EpbMotorCurrent(1);
}

/* EPB/Application/MotorControl/Spi_L9369_Api.c:1701 */
uint16_t EpbMotorVoltage(uint8_t motor)
{
    if (motor == 0)
        return g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR];
    else
        return g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR];
}
```

**리팩토링 후 (To-Be):**

```c
/* EPB/Peripheral/ADC/Adc_Api.c:117 (수정) */
void GetRawData_Refactored(void)
{
    Fs_Adc_StatusType fsStatus;

    /* 전원 전압 읽기 */
    fsStatus = Fs_Adc_GetPowerVoltages_Safe(/* ... */);

    /* 모터 측정값 읽기 (Safety-wrapped) */
    fsStatus = Fs_Adc_GetMotorMeasurements_Safe(
        &g_ADC.Physical.Motor.MOTORA,      /* Left voltage */
        &g_ADC.Physical.Motor.MOTORB,      /* Right voltage */
        &g_ADC.Physical.Motor.MOTORA_CUR,  /* Left current */
        &g_ADC.Physical.Motor.MOTORB_CUR   /* Right current */
    );

    /* 에러 처리 */
    if (fsStatus != FS_ADC_OK) {
        /* DTC 생성 또는 로깅 */
    }
}
```

**내부 호출 체인:**

```
ASW: GetRawData_Refactored()
  ↓
FS: Fs_Adc_GetMotorMeasurements_Safe()
  ├─ 1. Bsw_Adc_GetMotorMeasurements()
  │    ├─ Bsw_Adc_ReadMotorChannel(BSW_ADC_MOTOR_VOLTAGE_LEFT, &raw)
  │    │    └─ Mcal_Adc_ReadL9369Adc(MCAL_L9369_ADC_VSBRIDGE_A, &raw)
  │    │         └─ g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR] 접근
  │    └─ [반복] Right voltage, Left/Right current
  ├─ 2. Fs_Adc_ValidateVoltageRange() (0V-20V, 0A-30A 검증)
  └─ 3. 범위 초과 시 safe_value=0 적용
```

---

## 4. ASW 수정 가이드 (최소 변경)

### 4.1 변경 대상 파일

| 파일                           | 함수               | 변경 내용                        |
| ------------------------------ | ------------------ | -------------------------------- |
| `EPB/Peripheral/ADC/Adc_Api.c` | `CheckAdcStatus()` | 함수명 변경 없음, 내부 호출 치환 |
| `EPB/Peripheral/ADC/Adc_Api.c` | `GetRawData()`     | FS API 호출로 치환               |
| `EPB/Peripheral/ADC/Adc_Api.c` | `SetLogicData()`   | **변경 없음** (그대로 유지)      |
| `EPB/Peripheral/ADC/Adc_Api.h` | 헤더 추가          | `#include "Fs_Adc.h"` 추가       |

### 4.2 코드 수정 예시 (Diff 스타일)

#### 4.2.1 GetRawData() 함수 수정

```diff
--- EPB/Peripheral/ADC/Adc_Api.c (원본)
+++ EPB/Peripheral/ADC/Adc_Api.c (수정)
@@ -104,17 +104,25 @@
 void GetRawData(void)
 {
-    g_ADC.Raw.UBB = GetAdcAverage(ADC_CH_SUB_UBB_MON);
-    g_ADC.Raw.IGN = GetAdcAverage(ADC_CH_SUB_IGN_MON);
-    g_ADC.Raw.UBVR = GetAdcAverage(ADC_CH_SUB_UBVR_MON);
+    Fs_Adc_StatusType fsStatus;

-    g_ADC.Physical.Power.UBB = ConvertToPhysicalBatVolt(g_ADC.Raw.UBB);
-    g_ADC.Physical.Power.IGN = ConvertToPhysicalBatVolt(g_ADC.Raw.IGN);
-    g_ADC.Physical.Power.UBVR = ConvertToPhysicalBatVolt(g_ADC.Raw.UBVR);
+    /* Read power voltages (IGN, UBB, UBVR) */
+    fsStatus = Fs_Adc_GetPowerVoltages_Safe(
+        &g_ADC.Physical.Power.IGN,
+        &g_ADC.Physical.Power.UBB,
+        &g_ADC.Physical.Power.UBVR
+    );

-    g_ADC.Physical.Motor.MOTORA = EpbMotorVoltage(0);
-    g_ADC.Physical.Motor.MOTORB = EpbMotorVoltage(1);
-    g_ADC.Physical.Motor.MOTORA_CUR = EpbMotorCurrent(0);
-    g_ADC.Physical.Motor.MOTORB_CUR = EpbMotorCurrent(1);
+    /* Read motor measurements */
+    fsStatus = Fs_Adc_GetMotorMeasurements_Safe(
+        &g_ADC.Physical.Motor.MOTORA,
+        &g_ADC.Physical.Motor.MOTORB,
+        &g_ADC.Physical.Motor.MOTORA_CUR,
+        &g_ADC.Physical.Motor.MOTORB_CUR
+    );
+
+    /* Update timeout flag for backward compatibility */
+    g_ADC.Flag.Timeout = Fs_Adc_IsInSafeState();
 }
```

#### 4.2.2 헤더 파일 수정

```diff
--- EPB/Peripheral/ADC/Adc_Api.h (원본)
+++ EPB/Peripheral/ADC/Adc_Api.h (수정)
@@ -5,6 +5,11 @@
 #ifndef ADC_API_H
 #define ADC_API_H

+/* Refactored FS/BSW/MCAL headers */
+#include "Fs_Adc.h"
+#include "Bsw_Adc.h"
+#include "Mcal_Adc.h"
+
 /* Function prototypes */
 void ADCInitialize(void);
 void CheckAdcStatus(void);
```

### 4.3 삭제 가능한 함수 (Optional - 단계적 제거)

리팩토링 후 다음 함수들은 더 이상 직접 호출되지 않으므로 제거 가능합니다:

- `GetADCdata()` → `Mcal_Adc_ReadChannel()`로 대체
- `GetAdcAverage()` → `Bsw_Adc_ReadAveraged()`로 대체
- `ConvertToPhysicalBatVolt()` → `Bsw_Adc_ConvertToVoltage()`로 대체
- `EpbMotorVoltage()` → `Bsw_Adc_ReadMotorChannel()`로 대체
- `EpbMotorCurrent()` → `Bsw_Adc_ReadMotorChannel()`로 대체

**주의:** 다른 곳에서 호출되는지 확인 후 제거하세요.

---

## 5. 빌드 및 테스트 절차

### 5.1 빌드 설정

#### 5.1.1 헤더 검색 경로 추가 (Make/CMake)

```makefile
# Makefile 또는 CMakeLists.txt
INCLUDE_DIRS += -I$(PROJECT_ROOT)/source/refactoring
```

#### 5.1.2 소스 파일 추가

```makefile
# 새로 추가된 소스 파일
SRC_FILES += refactoring/Mcal_Adc.c
SRC_FILES += refactoring/Bsw_Adc.c
SRC_FILES += refactoring/Fs_Adc.c
```

### 5.2 점진적 통합 전략

#### Phase 1: MCAL 레이어만 통합

```c
/* GetADCdata() 호출을 Mcal_Adc_ReadChannel()로 치환 */
// 기존: uint16_t raw = GetADCdata(ADC_CH_SUB_IGN_MON);
uint16_t raw;
Mcal_Adc_ReadChannel(MCAL_ADC_CH_SUB_IGN_MON, &raw);
```

#### Phase 2: BSW 레이어 추가

```c
/* GetAdcAverage() 호출을 Bsw_Adc_ReadAveraged()로 치환 */
uint16_t avgRaw;
Bsw_Adc_ReadAveraged(BSW_ADC_CH_IGN_VOLTAGE, 6, &avgRaw);
```

#### Phase 3: FS 레이어 완전 통합 (최종)

```c
/* 전체 로직을 FS API로 치환 (본 문서의 최종 형태) */
Fs_Adc_GetPowerVoltages_Safe(&ign, &ubb, &ubvr);
```

### 5.3 검증 항목

| 검증 항목         | 확인 방법                      | 예상 결과                      |
| ----------------- | ------------------------------ | ------------------------------ |
| **Raw 값 동일성** | `g_ADC.Raw.*` 디버거 확인      | 기존과 동일한 10bit 값         |
| **물리값 동일성** | `g_ADC.Physical.*` 디버거 확인 | 기존과 동일한 mV/mA 값         |
| **타임아웃 동작** | ADC 선 분리 후 동작            | `g_ADC.Flag.Timeout` 설정 확인 |
| **범위 검증**     | 비정상 전압(25V) 입력          | Safe value(12V)로 대체 확인    |
| **EPB 로직 정상** | `SSMInput.SSMInVoltage` 확인   | 기존과 동일한 값 전달          |
| **CAN 메시지**    | CAN 모니터링                   | 전압 정보 정상 송신 확인       |

---

## 6. 트러블슈팅

### 6.1 컴파일 에러

**에러:** `'Fs_Adc_GetPowerVoltages_Safe' undeclared`
**해결:** `Adc_Api.c`에 `#include "Fs_Adc.h"` 추가

**에러:** `'MCAL_ADC_CH_SUB_IGN_MON' undeclared`
**해결:** `Mcal_Adc.h`에서 기존 ADC 채널 매크로 확인 (이름 다를 수 있음)

### 6.2 링크 에러

**에러:** `undefined reference to 'Bsw_Adc_ReadAveraged'`
**해결:** `Bsw_Adc.c`를 빌드 시스템에 추가 (`Makefile` 또는 `CMakeLists.txt` 수정)

### 6.3 런타임 에러

**증상:** ADC 값이 항상 0
**원인:** `Fs_Adc_Init()` 호출 누락
**해결:** `EPB/Main_Api.c`의 초기화 코드에 `Fs_Adc_Init()` 추가

```c
void SystemInitialize(void) {
    /* ... 기존 초기화 ... */
    Fs_Adc_Init();  /* FS/BSW/MCAL ADC 초기화 */
}
```

**증상:** 타임아웃이 계속 발생
**원인:** MCAL 레이어의 타임아웃 카운터 값이 너무 작음
**해결:** `Mcal_Adc.c:60` 라인의 `timeout_cnt > 1000000u`를 조정

### 6.4 Safety 기능 오동작

**증상:** 정상 전압(13V)인데 Safe State 진입
**원인:** 범위 설정이 너무 좁음
**해결:** `Fs_Adc.c`의 `FS_ADC_RANGE_POWER_VOLTAGE` 범위 조정

```c
const Fs_Adc_RangeConfig FS_ADC_RANGE_POWER_VOLTAGE = {
    .min_valid_mv = 5000,    /* 6V → 5V로 완화 */
    .max_valid_mv = 19000,   /* 18V → 19V로 확대 */
    .safe_value_mv = 12000
};
```

---

## 7. 고봉밥 스타일 파일 통합 (Optional)

파일을 분리하지 않고 하나의 파일에 모든 레이어를 넣고 싶다면:

```c
/* EPB/Peripheral/ADC/Adc_Api_Refactored.c */

/*===========================================================================
 * Section 1: MCAL Layer (Hardware Wiring)
 *===========================================================================*/
void Mcal_Adc_Init(void) {
    /* ... MCAL 구현 ... */
}

Mcal_Adc_StatusType Mcal_Adc_ReadChannel(uint8_t channel, uint16_t* raw) {
    /* ... MCAL 구현 ... */
}

/*===========================================================================
 * Section 2: BSW Layer (Intent Algorithms)
 *===========================================================================*/
Bsw_Adc_StatusType Bsw_Adc_ReadAveraged(/* ... */) {
    /* ... BSW 구현 ... */
}

Bsw_Adc_StatusType Bsw_Adc_ConvertToVoltage(/* ... */) {
    /* ... BSW 구현 ... */
}

/*===========================================================================
 * Section 3: FS Layer (Safety Wrapper)
 *===========================================================================*/
Fs_Adc_StatusType Fs_Adc_GetPowerVoltages_Safe(/* ... */) {
    /* ... FS 구현 ... */
}

/*===========================================================================
 * Section 4: ASW Compatibility Layer (기존 인터페이스 유지)
 *===========================================================================*/
void CheckAdcStatus(void) {
    GetRawData();
    SetLogicData();
}

void GetRawData(void) {
    /* FS API 호출 (본 문서 섹션 4.2.1 참고) */
}
```

---

## 8. 요약

### 8.1 핵심 변경 사항

1. **ASW 수정**: `GetRawData()` 함수에서 FS API 호출로 치환 (7줄 → 15줄)
2. **헤더 추가**: `Adc_Api.h`에 `#include "Fs_Adc.h"` 추가
3. **빌드 설정**: `refactoring/*.c` 파일들을 빌드에 추가

### 8.2 동작 보장

- **필터링 알고리즘**: `Bsw_Adc_ReadAveraged()`가 기존 `GetAdcAverage()` 로직 그대로 구현
- **물리값 변환**: `Bsw_Adc_ConvertToVoltage()`가 기존 `ConvertToPhysicalBatVolt()` 공식 그대로 구현
- **타임아웃 처리**: `g_ADC.Flag.Timeout`은 여전히 사용 가능 (backward compatibility)

### 8.3 추가 이득

- **Safety**: 범위 검증 및 안전 값 제공 (기존에 없던 기능)
- **Diagnostics**: DTC 생성 및 에러 카운팅 가능
- **Portability**: MCAL 레이어만 교체하면 다른 MCU로 이식 가능

---

## 9. 참고 자료

- 입력 문서: `ADC_ASIS_FACT_PACKET.md`
- 생성 파일 위치: `refactoring/` 디렉토리
  - `Mcal_Adc.h`, `Mcal_Adc.c`
  - `Bsw_Adc.h`, `Bsw_Adc.c`
  - `Fs_Adc.h`, `Fs_Adc.c`

**문의 사항:** ASW 수정이 어렵거나 테스트 결과가 기대와 다르면 본 가이드를 다시 참고하세요.
