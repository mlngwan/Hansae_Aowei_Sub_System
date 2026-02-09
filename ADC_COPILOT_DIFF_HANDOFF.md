# ADC 리팩토링 Diff 정리 — Copilot → ChatGPT 핸드오프 (v2)

> **날짜**: 2026-02-09  
> **작업자**: GitHub Copilot  
> **대상**: Kiro가 생성한 AUTOSAR-like ADC 계층 구조 (MCAL → CDD → BSW → RTE → SWC)  
> **맥락**: Kiro의 초기 스캐폴딩을 평가한 후, 1차 이슈 수정 + Raw/Avg 이중 버퍼 분리까지 완료

---

## 변경 요약 (Kiro 원본 대비 전체 diff)

| # | 영역 | 파일 | 변경 핵심 |
|---|------|------|-----------|
| 1 | config | `config/Std_Types.h` **(신규)** | `Std_ReturnType`, `E_OK/E_NOT_OK`, `SchM` 매크로 단일 출처 |
| 2 | BSW | `bsw/inc/Bsw_Adc.h` | 인라인 타입 제거 → `Std_Types.h` include, `GetProcessedSnapshot` 선언 추가 |
| 3 | BSW | `bsw/src/Bsw_Adc.c` | `s_AdcSnapshotRaw`/`s_AdcSnapshotAvg` 이중 버퍼, 1024→1023 수정, `GetProcessedSnapshot()` 구현 |
| 4 | RTE | `rte/inc/Rte_Type.h` | 인라인 타입 전부 제거 → `Std_Types.h` include |
| 5 | RTE | `rte/inc/Rte_Adc.h` | `_Raw`(7개) + `_Avg`(7개) = 14개 API 선언 추가, Legacy 7개 유지 |
| 6 | RTE | `rte/src/Rte_Adc.c` | Raw/Avg 이중 SR Buffer, 헬퍼 2개, 21개 Read API, SchM 임계구역 |
| 7 | SWC | `swc/SwcAdcMon/src/SwcAdcMon.c` | 진단 읽기를 `_Avg` API로 변경 (5곳) |

---

## 수정 이슈 히스토리

| # | 심각도 | 이슈 | 상태 |
|---|--------|------|------|
| 1 | **Critical** | `Std_ReturnType` 이중 정의 (Bsw_Adc.h + Rte_Type.h) | ✅ `config/Std_Types.h` 신규 생성 |
| 2 | **Important** | ADC 변환 분모 1024 vs 1023 불일치 | ✅ `1023.0f`로 통일 |
| 3 | **Important** | 5ms Raw가 10ms Avg 결과를 덮어씀 | ✅ `s_AdcSnapshotRaw`/`s_AdcSnapshotAvg` 분리 |
| 4 | **Minor** | SR Buffer 임계구역 보호 없음 | ✅ `SchM_Enter/Exit_Exclusive_Area()` 적용 |
| 5 | **Known** | 신규 경로 + 레거시 경로 이중 ADC 샘플링 | ⏳ Phase 2/3 대상 |
| 6 | **Enhancement** | RTE에서 Raw/Avg 구분 불가 | ✅ `_Raw`/`_Avg`/Legacy 21개 API 체계 |

---

## 1. [신규] config/Std_Types.h

**목적**: `Std_ReturnType`, `E_OK`, `E_NOT_OK`, `TRUE`, `FALSE`, `SchM` 매크로를 프로젝트 전체에서 하나의 파일로 관리

```c
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

typedef uint8_t Std_ReturnType;
#define E_OK     ((Std_ReturnType)0u)
#define E_NOT_OK ((Std_ReturnType)1u)

#ifndef TRUE
#define TRUE  ((uint8_t)1u)
#endif
#ifndef FALSE
#define FALSE ((uint8_t)0u)
#endif

/* 임계구역: 기존 프로젝트 API 활용 */
extern void APP_InterruptDisable(void);
extern void APP_InterruptEnable(void);

#define SchM_Enter_Exclusive_Area()  APP_InterruptDisable()
#define SchM_Exit_Exclusive_Area()   APP_InterruptEnable()

#endif /* STD_TYPES_H */
```

---

## 2. [수정] bsw/inc/Bsw_Adc.h

```diff
-#include <stdint.h>
+#include "Std_Types.h" /* 프로젝트 공통 표준 타입 */

-/* 표준 리턴 타입 */
-typedef uint8_t Std_ReturnType;
-#define E_OK     ((Std_ReturnType)0u)
-#define E_NOT_OK ((Std_ReturnType)1u)
-(삭제 — Std_Types.h로 통합)

-/* Snapshot 읽기 */
-Std_ReturnType Bsw_Adc_GetSnapshot(Bsw_Adc_Snapshot_t* snapshot);
+/* Raw Snapshot 읽기 (5ms 즉시값) */
+Std_ReturnType Bsw_Adc_GetSnapshot(Bsw_Adc_Snapshot_t *snapshot);
+
+/* Avg Snapshot 읽기 (10ms 평균값) */
+Std_ReturnType Bsw_Adc_GetProcessedSnapshot(Bsw_Adc_Snapshot_t *snapshot);
```

---

## 3. [수정] bsw/src/Bsw_Adc.c — 핵심 변경

### 3-1. 이중 Snapshot 버퍼

```diff
-static Bsw_Adc_Snapshot_t s_AdcSnapshot = {0};
+static Bsw_Adc_Snapshot_t s_AdcSnapshotRaw = {0}; /* Raw: 5ms 즉시 갱신 */
+static Bsw_Adc_Snapshot_t s_AdcSnapshotAvg = {0}; /* Avg: 10ms 평균 결과 */
```

### 3-2. ADC 변환 분모 수정

```diff
-float adc_mV = ((float)raw * vref * 1000.0f) / 1024.0f;
+/* 주의: 10-bit ADC 최대값 = 1023 (레거시 Adc_Api.c 동일) */
+float adc_mV = ((float)raw * vref * 1000.0f) / 1023.0f;
```

### 3-3. Init에서 양쪽 초기화

```diff
-s_AdcSnapshot.DataValid = 0;
+s_AdcSnapshotRaw.DataValid = 0;
+s_AdcSnapshotAvg.DataValid = 0;
```

### 3-4. 5ms → Raw만 갱신

```diff
-s_AdcSnapshot.BatteryMotor_mV = Bsw_Adc_ConvertRawToPhysical(raw_ubb, ...);
-Cdd_L9369Adc_ReadMotorA(&s_AdcSnapshot.MotorA_Voltage_mV, ...);
-s_AdcSnapshot.DataValid = 1;
+s_AdcSnapshotRaw.BatteryMotor_mV = Bsw_Adc_ConvertRawToPhysical(raw_ubb, ...);
+Cdd_L9369Adc_ReadMotorA(&s_AdcSnapshotRaw.MotorA_Voltage_mV, ...);
+s_AdcSnapshotRaw.DataValid = 1;
```

### 3-5. 10ms → Avg만 갱신 (Raw 덮어쓰기 제거)

```diff
-/* 가공된 값으로 Snapshot 업데이트 */
-s_AdcSnapshot.BatteryMotor_mV = Bsw_Adc_ConvertRawToPhysical(avg_ubb, ...);
+/* 가공 결과를 Avg Snapshot에 저장 (Raw Snapshot 덮어쓰지 않음) */
+s_AdcSnapshotAvg.BatteryMotor_mV = Bsw_Adc_ConvertRawToPhysical(avg_ubb, ...);
+s_AdcSnapshotAvg.BatteryValve_mV = ...;
+s_AdcSnapshotAvg.Ignition_mV = ...;
+/* 모터 값은 5ms에서 이미 가공되므로 최신 Raw 복사 */
+s_AdcSnapshotAvg.MotorA_Voltage_mV = s_AdcSnapshotRaw.MotorA_Voltage_mV;
+s_AdcSnapshotAvg.MotorB_Voltage_mV = s_AdcSnapshotRaw.MotorB_Voltage_mV;
+s_AdcSnapshotAvg.MotorA_Current_mA = s_AdcSnapshotRaw.MotorA_Current_mA;
+s_AdcSnapshotAvg.MotorB_Current_mA = s_AdcSnapshotRaw.MotorB_Current_mA;
+s_AdcSnapshotAvg.DataValid = s_AdcSnapshotRaw.DataValid;
```

### 3-6. GetSnapshot → Raw, GetProcessedSnapshot → Avg

```c
Std_ReturnType Bsw_Adc_GetSnapshot(...)        { *snapshot = s_AdcSnapshotRaw; }
Std_ReturnType Bsw_Adc_GetProcessedSnapshot(...){ *snapshot = s_AdcSnapshotAvg; }
```

---

## 4. [수정] rte/inc/Rte_Type.h

```diff
-#include <stdint.h>
-typedef uint8_t Std_ReturnType;
-#define E_OK / E_NOT_OK ...
+#include "Std_Types.h" /* 프로젝트 공통 표준 타입 */
```

---

## 5. [수정] rte/inc/Rte_Adc.h — API 21개 체계

```diff
-/* SWC Interface - Rte_Read API */
-Std_ReturnType Rte_Read_Adc_BatteryMotor(uint16_t* value);
-...(7개)
+/* Legacy (Avg valid → Avg, 아니면 Raw 폴백) */
+Std_ReturnType Rte_Read_Adc_BatteryMotor(uint16_t* value);      // 포함 7개
+
+/* Raw (5ms 즉시값) */
+Std_ReturnType Rte_Read_Adc_BatteryMotor_Raw(uint16_t* value);  // 신규 7개
+
+/* Avg (10ms 평균값) */
+Std_ReturnType Rte_Read_Adc_BatteryMotor_Avg(uint16_t* value);  // 신규 7개
```

**전체 API 목록 (21개)**:

| 접미사 | 대상 | 개수 | 소스 |
|--------|------|------|------|
| (없음) | Legacy 호환 | 7 | Avg valid ? Avg : Raw 폴백 |
| `_Raw` | 5ms 즉시값 | 7 | `s_RteRaw_*` 버퍼 |
| `_Avg` | 10ms 평균값 | 7 | `s_RteAvg_*` 버퍼 |

---

## 6. [수정] rte/src/Rte_Adc.c — 핵심 변경 (101행 → 337행)

### 6-1. 이중 SR Buffer

```diff
-static uint16_t s_RteBuffer_BatteryMotor = 0;
-...(변수 8개 + Valid 1개)
+/* ===== Raw SR Buffer (5ms 갱신) ===== */
+static uint16_t s_RteRaw_BatteryMotor = 0;
+...(7개 + s_RteRaw_Valid)
+
+/* ===== Avg SR Buffer (10ms 갱신) ===== */
+static uint16_t s_RteAvg_BatteryMotor = 0;
+...(7개 + s_RteAvg_Valid)
```

### 6-2. 헬퍼 함수 2개 (SchM 보호)

```c
static void Rte_Adc_CopyToRawBuffer(const Bsw_Adc_Snapshot_t *snapshot)
{
    SchM_Enter_Exclusive_Area();
    s_RteRaw_BatteryMotor = snapshot->BatteryMotor_mV;
    /* ... 7개 필드 복사 ... */
    s_RteRaw_Valid = 1;
    SchM_Exit_Exclusive_Area();
}

static void Rte_Adc_CopyToAvgBuffer(const Bsw_Adc_Snapshot_t *snapshot)
{
    /* 동일 패턴, s_RteAvg_* 대상 */
}
```

### 6-3. Publish 분리

```diff
-/* 10ms Publish */
-void Rte_Adc_Publish_10ms(void) {
-    Rte_Adc_Publish_5ms();  // 단순 재호출이었음
-}
+/* 5ms → Raw Buffer */
+void Rte_Adc_Publish_5ms(void) {
+    if (Bsw_Adc_GetSnapshot(&snapshot) == E_OK) {
+        Rte_Adc_CopyToRawBuffer(&snapshot);
+    }
+}
+/* 10ms → Avg Buffer */
+void Rte_Adc_Publish_10ms(void) {
+    if (Bsw_Adc_GetProcessedSnapshot(&processed) == E_OK) {
+        Rte_Adc_CopyToAvgBuffer(&processed);
+    }
+}
```

### 6-4. Legacy API 폴백 정책

```c
Std_ReturnType Rte_Read_Adc_BatteryMotor(uint16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {          // Avg 우선
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_BatteryMotor;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {          // Raw 폴백
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_BatteryMotor;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;                    // 둘 다 없으면 실패
}
```

---

## 7. [수정] swc/SwcAdcMon/src/SwcAdcMon.c

```diff
-if (Rte_Read_Adc_BatteryMotor(&battery_mV) == E_OK) {
+if (Rte_Read_Adc_BatteryMotor_Avg(&battery_mV) == E_OK) {

-if (Rte_Read_Adc_MotorA_Voltage(&motorA_voltage) == E_OK &&
-    Rte_Read_Adc_MotorA_Current(&motorA_current) == E_OK) {
+if (Rte_Read_Adc_MotorA_Voltage_Avg(&motorA_voltage) == E_OK &&
+    Rte_Read_Adc_MotorA_Current_Avg(&motorA_current) == E_OK) {

-if (Rte_Read_Adc_MotorB_Voltage(&motorB_voltage) == E_OK &&
-    Rte_Read_Adc_MotorB_Current(&motorB_current) == E_OK) {
+if (Rte_Read_Adc_MotorB_Voltage_Avg(&motorB_voltage) == E_OK &&
+    Rte_Read_Adc_MotorB_Current_Avg(&motorB_current) == E_OK) {
```

**이유**: 진단 SWC는 노이즈 제거된 평균값(Avg)으로 판정해야 함

---

## 미수정 사항 (이슈 #5 — Phase 2/3 대상)

**이중 ADC 샘플링**: 현재 같은 5ms 틱에서 두 경로가 모두 실행됨

```
경로 A (신규): OsTask_5ms → Rte_Task_5ms → Bsw_Adc_MainFunction_5ms → Mcal_Adc_ReadRaw
경로 B (레거시): SwcA_Runnable_5ms → TaskScheduler_5ms → CheckAdcStatus → Bsw_Adc_ReadAllChannels
```

Phase 1에서는 의도적 공존. Phase 2에서 소비 모듈(SSMInput, PbcInDriver, CAN Tx 등)을 `Rte_Read_Adc_*` API로 전환 시 `경로 B` 제거 가능.

---

## 아키텍처 데이터 흐름 (현재 상태)

```
[5ms Tick]
  MCAL_Adc_ReadRaw ──→ Bsw_Adc_MainFunction_5ms ──→ s_AdcSnapshotRaw
  Cdd_L9369Adc_Read ─↗                               │
                                                       └─→ Rte_Adc_Publish_5ms
                                                              └─→ s_RteRaw_* (SchM 보호)

[10ms Tick]
  Bsw_Adc_MainFunction_10ms ──→ s_AdcSnapshotAvg
    (Min/Max 제거 6샘플 평균)      │ (모터값은 Raw에서 copy)
                                    └─→ Rte_Adc_Publish_10ms
                                          └─→ s_RteAvg_* (SchM 보호)

[SWC 읽기 — 3가지 방법]
  _Raw API  ──→ s_RteRaw_*  (5ms 즉시값, SchM 보호)
  _Avg API  ──→ s_RteAvg_*  (10ms 평균값, SchM 보호)
  Legacy API ─→ Avg valid ? s_RteAvg_* : s_RteRaw_* (폴백)

[SwcAdcMon 진단]
  SwcAdcMon_10ms ──→ Rte_Read_Adc_BatteryMotor_Avg()   ← Avg 사용
                  ──→ Rte_Read_Adc_MotorA_Current_Avg()  ← Avg 사용
```

---

## ChatGPT 검토 요청 사항

다음 관점으로 리뷰를 요청:

1. **Raw/Avg 이중 버퍼 설계**: BSW에서 `s_AdcSnapshotRaw`(5ms) / `s_AdcSnapshotAvg`(10ms) 분리, RTE에서도 `s_RteRaw_*` / `s_RteAvg_*` 이중 버퍼 — 메모리 비용 대비 설계 적절성
2. **21개 API 체계**: `_Raw`(7) + `_Avg`(7) + Legacy(7) = 21개 — API 수 과다 여부, 매크로/인라인 최적화 가능성
3. **Legacy 폴백 정책**: `Avg valid → Avg, else Raw` — 부팅 직후 Avg 미준비 시 Raw 반환이 안전한지
4. **SchM 매핑**: `SchM_Enter/Exit_Exclusive_Area()` → `APP_InterruptDisable/Enable()` 매핑 — AUTOSAR 호환성
5. **모터 값 Avg 처리**: L9369 모터 전압/전류는 평균을 안 하고 Raw에서 copy하는 설계 — CDD 레벨에서 평균 필요 여부
6. **Phase 2 전환 전략**: 이중 샘플링 해소를 위한 레거시 제거 순서 제안
