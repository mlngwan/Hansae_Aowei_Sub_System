# ADC 기능 패키지 리팩토링 가이드 (Skeleton 버전)

## 1. 개요

본 문서는 ADC 기능 패키지의 FS/BSW/MCAL Skeleton 코드 사용 가이드입니다.

**중요: 이것은 리팩토링 시작 전 단계입니다**

- 현재는 뼈대(Skeleton) 코드만 제공
- 실제 구현은 TODO 주석으로 표시
- 기존 ASW 코드는 수정하지 않음
- Intent(알고리즘) 불변 원칙 준수

---

## 2. Feature_Adc 폴더 구조

```
refactoring/
└── Feature_Adc/
    ├── Adc_Types.h        # 공용 타입 정의 (레이어 간 공유)
    ├── Fs_Adc.h / .c     # FS Layer (Safety wrapper)
    ├── Bsw_Adc.h / .c    # BSW Layer (Intent 알고리즘)
    └── Mcal_Adc.h / .c   # MCAL Layer (Hardware wiring)
```

### 파일별 역할

| 파일              | 역할             | 현재 대응 함수                                                 | TODO 구현 항목            |
| ----------------- | ---------------- | -------------------------------------------------------------- | ------------------------- |
| **Adc_Types.h**   | 공용 타입/상수   | -                                                              | 완료 (타입 정의만)        |
| **Mcal_Adc.c/.h** | HW 레지스터 접근 | `GetADCdata()`, `ADCInitialize()`, `EpbMotorVoltage/Current()` | 레지스터 접근 코드 이동   |
| **Bsw_Adc.c/.h**  | Intent 알고리즘  | `GetAdcAverage()`, `ConvertToPhysicalBatVolt()`                | 계산식/필터링 로직 이동   |
| **Fs_Adc.c/.h**   | Safety wrapper   | (신규)                                                         | 범위 검증, 진단 로직 구현 |

---

## 3. 레이어 간 호출 흐름

```
┌─────────────────────────────────────────────────────────┐
│ ASW (Application Software)                              │
│ 현재: CheckAdcStatus() → GetRawData() → SetLogicData() │
│                                                          │
│ [호출 치환 포인트]                                        │
│ - GetRawData() 내부에서 Fs_Adc_* 호출로 변경            │
│ - 기존 g_ADC.Physical.* 접근은 유지                      │
└────────────────┬────────────────────────────────────────┘
                 │ Fs_Adc_GetPowerVoltages_Safe()
                 │ Fs_Adc_GetMotorMeasurements_Safe()
┌────────────────▼────────────────────────────────────────┐
│ FS Layer (Feature_Adc/Fs_Adc.c)                         │
│ - 범위 검증 (6V-18V, 0-30A)                              │
│ - Timeout 감지                                           │
│ - Safe fallback values 제공                             │
│ - 진단 카운터 관리                                        │
└────────────────┬────────────────────────────────────────┘
                 │ Bsw_Adc_GetPowerVoltages()
                 │ Bsw_Adc_ReadAveraged()
                 │ Bsw_Adc_ConvertToVoltage()
┌────────────────▼────────────────────────────────────────┐
│ BSW Layer (Feature_Adc/Bsw_Adc.c)                       │
│ - GetAdcAverage() 알고리즘 보존 (6샘플, 최대/최소 제거)   │
│ - ConvertToPhysicalBatVolt() 공식 보존                   │
│ - 논리 채널 → 물리 채널 매핑                              │
└────────────────┬────────────────────────────────────────┘
                 │ Mcal_Adc_ReadChannel()
                 │ Mcal_Adc_ReadL9369Adc()
┌────────────────▼────────────────────────────────────────┐
│ MCAL Layer (Feature_Adc/Mcal_Adc.c)                     │
│ - ADC 레지스터 직접 접근 (ADC.MCR, ADC.CDR)              │
│ - L9369 글로벌 변수 접근                                  │
│ - Timeout 플래그 관리                                     │
└─────────────────────────────────────────────────────────┘
```

---

## 4. ASW 호출 치환 포인트 (최소 변경)

### 4.1 변경 대상 파일

**파일:** `EPB/Peripheral/ADC/Adc_Api.c`
**함수:** `GetRawData()` (라인 104-122)

### 4.2 호출 치환 예시 (Skeleton 기준)

#### 현재 코드 (As-Is)

```c
/* EPB/Peripheral/ADC/Adc_Api.c:104-122 */
void GetRawData(void)
{
    // 전원 전압 읽기 (6회 샘플링 평균)
    g_ADC.Raw.UBB = GetAdcAverage(ADC_CH_SUB_UBB_MON);
    g_ADC.Raw.IGN = GetAdcAverage(ADC_CH_SUB_IGN_MON);
    g_ADC.Raw.UBVR = GetAdcAverage(ADC_CH_SUB_UBVR_MON);

    // 물리값 변환 (mV 단위)
    g_ADC.Physical.Power.UBB = ConvertToPhysicalBatVolt(g_ADC.Raw.UBB) * ADC_REF;
    g_ADC.Physical.Power.IGN = ConvertToPhysicalBatVolt(g_ADC.Raw.IGN) * ADC_REF;
    g_ADC.Physical.Power.UBVR = ConvertToPhysicalBatVolt(g_ADC.Raw.UBVR) * ADC_REF;

    // 모터 측정값 읽기
    g_ADC.Physical.Motor.MOTORA = EpbMotorVoltage(0) * ADC_REF;
    g_ADC.Physical.Motor.MOTORB = EpbMotorVoltage(1) * ADC_REF;
    g_ADC.Physical.Motor.MOTORA_CUR = EpbMotorCurrent(0) * ADC_REF;
    g_ADC.Physical.Motor.MOTORB_CUR = EpbMotorCurrent(1) * ADC_REF;
}
```

#### 리팩토링 후 (To-Be)

```c
/* EPB/Peripheral/ADC/Adc_Api.c:104 (수정) */
#include "Feature_Adc/Fs_Adc.h"  /* 추가 */

void GetRawData(void)
{
    Adc_StatusType status;

    /* [호출 치환 포인트 1] 전원 전압 읽기 → FS API 호출 */
    status = Fs_Adc_GetPowerVoltages_Safe(
        &g_ADC.Physical.Power.IGN,    /* mV 단위로 직접 저장 */
        &g_ADC.Physical.Power.UBB,
        &g_ADC.Physical.Power.UBVR
    );

    /* [호출 치환 포인트 2] 모터 측정값 읽기 → FS API 호출 */
    status = Fs_Adc_GetMotorMeasurements_Safe(
        &g_ADC.Physical.Motor.MOTORA,      /* mV 단위 */
        &g_ADC.Physical.Motor.MOTORB,      /* mV 단위 */
        &g_ADC.Physical.Motor.MOTORA_CUR,  /* mA 단위 */
        &g_ADC.Physical.Motor.MOTORB_CUR   /* mA 단위 */
    );

    /* [호출 치환 포인트 3] Timeout 플래그 업데이트 (backward compatibility) */
    g_ADC.Flag.Timeout = Fs_Adc_IsInSafeState();
}
```

### 4.3 변경 요약

| 항목             | 변경 전                                         | 변경 후                 | 효과        |
| ---------------- | ----------------------------------------------- | ----------------------- | ----------- |
| **코드 라인 수** | 15줄                                            | 12줄                    | 간소화      |
| **함수 호출**    | 7개 (GetAdcAverage x3, Convert x3, EpbMotor x4) | 2개 (Fs_Adc_Get x2)     | 단순화      |
| **단위 변환**    | `* ADC_REF` 수동 곱셈                           | FS/BSW에서 자동 처리    | 에러 감소   |
| **Safety 기능**  | 없음                                            | 범위 검증, Timeout 감지 | 안전성 향상 |

---

## 5. 개별 포트 함수 사용 (Optional)

ADC_FEATURE_FACT_PACKET_v2.md 섹션 3.1의 포트 함수 제안에 따라 개별 값 접근도 가능합니다.

### 5.1 개별 값 접근 예시

```c
/* 기존: 구조체 직접 접근 */
uint16 battery_volt = g_ADC.Physical.Power.UBB;

/* 리팩토링 후: 포트 함수 사용 */
#include "Feature_Adc/Fs_Adc.h"
uint16 battery_volt = Fs_Adc_GetBatteryVoltage();  /* mV 단위, Safety 검증됨 */
```

### 5.2 사용 가능한 포트 함수 목록

| 포트 함수                         | 반환 타입 | 단위 | 현재 대응                         |
| --------------------------------- | --------- | ---- | --------------------------------- |
| `Fs_Adc_GetBatteryVoltage()`      | `uint16`  | mV   | `g_ADC.Physical.Power.UBB`        |
| `Fs_Adc_GetIgnitionVoltage()`     | `uint16`  | mV   | `g_ADC.Physical.Power.IGN`        |
| `Fs_Adc_GetValveBatteryVoltage()` | `uint16`  | mV   | `g_ADC.Physical.Power.UBVR`       |
| `Fs_Adc_GetMotorVoltageLeft()`    | `uint16`  | mV   | `g_ADC.Physical.Motor.MOTORA`     |
| `Fs_Adc_GetMotorVoltageRight()`   | `uint16`  | mV   | `g_ADC.Physical.Motor.MOTORB`     |
| `Fs_Adc_GetMotorCurrentLeft()`    | `uint16`  | mA   | `g_ADC.Physical.Motor.MOTORA_CUR` |
| `Fs_Adc_GetMotorCurrentRight()`   | `uint16`  | mA   | `g_ADC.Physical.Motor.MOTORB_CUR` |
| `Fs_Adc_GetAdcStatus()`           | `uint8`   | -    | `g_ADC.Flag.Timeout` 기반         |

---

## 6. 실제 리팩토링 시 TODO 구현 순서

### Phase 1: MCAL 레이어 구현

1. `Mcal_Adc.c`의 TODO 주석 확인
2. 기존 `GetADCdata()` 코드를 `Mcal_Adc_ReadChannel()`로 이동
3. 레지스터 접근 코드만 이동 (Intent 로직 제외)

### Phase 2: BSW 레이어 구현

1. `Bsw_Adc.c`의 TODO 주석 확인
2. 기존 `GetAdcAverage()` 알고리즘을 `Bsw_Adc_ReadAveraged()`로 이동
3. 기존 `ConvertToPhysicalBatVolt()` 공식을 `Bsw_Adc_ConvertToVoltage()`로 이동
4. **Intent 알고리즘은 수정하지 않고 그대로 복사**

### Phase 3: FS 레이어 구현

1. `Fs_Adc.c`의 TODO 주석 확인
2. Safety 범위 검증 로직 구현
3. 진단 카운터 업데이트 로직 구현

### Phase 4: ASW 호출 치환

1. `EPB/Peripheral/ADC/Adc_Api.c`의 `GetRawData()` 수정
2. 섹션 4.2의 치환 예시 적용
3. 빌드 및 테스트

---

## 7. 단위 및 타입 확정 사항

### 7.1 현재 프로젝트 단위 (불변)

| 물리량      | 단위             | 데이터 타입 | 근거                                   |
| ----------- | ---------------- | ----------- | -------------------------------------- |
| **전압**    | mV (millivolt)   | `uint16`    | ADC_FEATURE_FACT_PACKET_v2.md 섹션 1.2 |
| **전류**    | mA (milliampere) | `uint16`    | ADC_FEATURE_FACT_PACKET_v2.md 섹션 1.2 |
| **Raw ADC** | 0-1023 (10bit)   | `uint16`    | ADC_FEATURE_FACT_PACKET_v2.md 섹션 1.1 |

### 7.2 CAN 출력 스케일링 (기존 유지)

```c
/* EPB/Application/Diagnostics/Can_Api.c:65-69 */
EPBBoardvoltage = g_ADC.Physical.Power.UBB / 100.0;  /* mV → 0.1V */
EPBMotorAcurrent = g_ADC.Physical.Motor.MOTORA_CUR / 100.0;  /* mA → 0.1A */
```

**리팩토링 후에도 동일하게 유지** (CAN 출력 코드 변경 없음)

---

## 8. 빌드 설정 (TODO)

### 8.1 헤더 검색 경로 추가

```makefile
# Makefile 또는 CMakeLists.txt
INCLUDE_DIRS += -I$(PROJECT_ROOT)/source/refactoring
```

### 8.2 소스 파일 추가

```makefile
SRC_FILES += refactoring/Feature_Adc/Mcal_Adc.c
SRC_FILES += refactoring/Feature_Adc/Bsw_Adc.c
SRC_FILES += refactoring/Feature_Adc/Fs_Adc.c
```

### 8.3 헤더 include 경로

```c
/* ASW 코드에서 */
#include "Feature_Adc/Fs_Adc.h"  /* FS 레이어만 include */

/* FS 내부에서 */
#include "Adc_Types.h"
#include "Bsw_Adc.h"

/* BSW 내부에서 */
#include "Adc_Types.h"
#include "Mcal_Adc.h"
```

---

## 9. 검증 항목 (리팩토링 완료 후)

| 검증 항목         | 확인 방법                      | 예상 결과                   |
| ----------------- | ------------------------------ | --------------------------- |
| **동작 동일성**   | `g_ADC.Physical.*` 디버거 확인 | 기존과 동일한 mV/mA 값      |
| **단위 일치**     | CAN 메시지 모니터링            | 기존과 동일한 0.1V/0.1A 값  |
| **타임아웃 동작** | ADC 선 분리 테스트             | `g_ADC.Flag.Timeout` 설정됨 |
| **범위 검증**     | 비정상 전압 입력 (25V)         | Safe value(12V)로 대체      |
| **EPB 로직**      | `SSMInput.SSMInVoltage` 확인   | 정상 전달                   |

---

## 10. 주의 사항

### 10.1 Intent 불변 원칙

- **GetAdcAverage()의 필터링 알고리즘은 수정 금지**
  - 6회 샘플링
  - 최대/최소 제거
  - 나머지 4개 평균
- **ConvertToPhysicalBatVolt()의 변환 공식은 수정 금지**
  - `(ADC/1023) * Vref * ((R1+R2)/R2)`
  - 상수 값: `ref_3v3=3300, r300=300, r51=51`

### 10.2 기존 코드 보존

- 기존 `EPB/Peripheral/ADC/Adc_Api.c`는 삭제하지 않음
- 리팩토링 실패 시 롤백 가능하도록 백업 유지
- 점진적 전환 전략 권장 (Phase 1 → 2 → 3 → 4)

### 10.3 테스트 전략

1. **Phase별 단위 테스트**
   - MCAL만 구현 후 Raw 값 확인
   - BSW 추가 후 물리값 확인
   - FS 추가 후 Safety 기능 확인
2. **통합 테스트**
   - ASW 호출 치환 후 전체 동작 확인
   - CAN 메시지 비교 (기존 vs 리팩토링)

---

## 11. TODO 체크리스트

### MCAL 레이어

- [ ] `Mcal_Adc_Init()` 구현 (레지스터 초기화)
- [ ] `Mcal_Adc_ReadChannel()` 구현 (레지스터 읽기)
- [ ] `Mcal_Adc_ReadL9369Adc()` 구현 (L9369 접근)
- [ ] `Mcal_Adc_GetTimeoutFlag()` 구현

### BSW 레이어

- [ ] `Bsw_Adc_ReadAveraged()` 구현 (필터링 알고리즘)
- [ ] `Bsw_Adc_ConvertToVoltage()` 구현 (변환 공식)
- [ ] `Bsw_Adc_GetPowerVoltages()` 구현 (조합)
- [ ] `Bsw_Adc_GetMotorMeasurements()` 구현 (조합)

### FS 레이어

- [ ] `Fs_Adc_GetPowerVoltages_Safe()` 구현 (Safety wrapper)
- [ ] `Fs_Adc_GetMotorMeasurements_Safe()` 구현 (Safety wrapper)
- [ ] 범위 검증 로직 구현
- [ ] 진단 카운터 관리 구현

### ASW 호출 치환

- [ ] `EPB/Peripheral/ADC/Adc_Api.c` 헤더 추가
- [ ] `GetRawData()` 함수 수정
- [ ] 빌드 설정 업데이트

### 검증

- [ ] Raw 값 동일성 확인
- [ ] 물리값 동일성 확인
- [ ] CAN 메시지 비교
- [ ] Timeout 동작 확인
- [ ] Safety 기능 확인

---

## 12. 참고 문서

- 입력 분석: `ADC_FEATURE_FACT_PACKET_v2.md`
- Skeleton 코드: `refactoring/Feature_Adc/` 디렉토리
- 기존 코드: `EPB/Peripheral/ADC/Adc_Api.c`

**문의 사항:** Skeleton 코드의 TODO 주석을 따라 순차적으로 구현하세요.
