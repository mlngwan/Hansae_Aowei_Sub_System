# ADC Skeleton 검증을 위한 As-Is 팩트 확정

## 1. 전압 단위 확정

### 1.1 ADC 변환 공식 및 단위 (코드 기준)

| 단계 | 함수 | 공식 | 결과 단위 | 근거 (파일/라인) |
|------|------|------|-----------|------------------|
| **1. Raw ADC** | `GetADCdata()` | `ADC.CDR[ch].R & 0x000003FF` | **0~1023 (10bit)** | `EPB/Peripheral/ADC/Adc_Api.c:61` |
| **2. 물리 전압** | `ConvertToPhysicalBatVolt()` | `(adc/1023.0) * 3.3 * ((300+51)/51)` | **V (볼트)** | `EPB/Peripheral/ADC/Adc_Api.c:95-99` |
| **3. 최종 저장** | `GetRawData()` | `ConvertToPhysicalBatVolt() * ADC_REF` | **mV (밀리볼트)** | `EPB/Peripheral/ADC/Adc_Api.c:112-115` |

### 1.2 단위 확정 근거

| 항목 | 값 | 근거 (파일/변수/주석/사용처) | 설명 |
|------|----|-----------------------------|------|
| **ADC_REF** | `1000` | `EPB/Peripheral/ADC/Adc_Api.h:28` | 스케일링 팩터 (V → mV 변환) |
| **기준 전압** | `3.3V` | `EPB/Peripheral/ADC/Adc_Api.h:35` `ref_3v3` | ADC 기준 전압 |
| **분압 저항** | `r300=300, r51=51` | `EPB/Peripheral/ADC/Adc_Api.h:30-31` | 전압 분배 회로 |
| **분압 비율** | `(300+51)/51 = 6.88` | `ConvertToPhysicalBatVolt()` 공식 | 실제 전압 = ADC전압 × 6.88 |

### 1.3 CAN 출력 단위 검증

| CAN 신호 | 변환 공식 | 최종 단위 | 근거 (파일/라인) |
|----------|-----------|-----------|------------------|
| `EPBBoardvoltage` | `g_ADC.Physical.Power.UBB/100.0` | **0.1V** | `EPB/Application/Diagnostics/Can_Api.c:65` |
| `EPBMotorAvoltage` | `g_ADC.Physical.Motor.MOTORA/100.0` | **0.1V** | `EPB/Application/Diagnostics/Can_Api.c:67` |
| `EPBMotorAcurrent` | `g_ADC.Physical.Motor.MOTORA_CUR/100.0` | **0.1A** | `EPB/Application/Diagnostics/Can_Api.c:66` |

**결론**: 현재 프로젝트에서 ADC 물리값은 **mV(전압), mA(전류)** 단위를 사용하며, CAN 출력 시 100으로 나누어 0.1V, 0.1A 단위로 전송

## 2. Fs_Adc로 치환해야 할 상위 호출자 위치 1곳 선정

### 2.1 호출 그래프 (2 depth)

```
TaskScheduler_5ms() [EPB/Main_Api.c:225]
    ↓
CheckAdcStatus() [EPB/Peripheral/ADC/Adc_Api.c:134] ← **치환 대상**
    ↓
GetRawData() [EPB/Peripheral/ADC/Adc_Api.c:104]
    ↓
SetLogicData() [EPB/Peripheral/ADC/Adc_Api.c:124]
```

### 2.2 선정된 치환 위치

| 치환 대상 함수 | 파일 위치 | 라인 | 호출자 | 호출 조건 |
|----------------|-----------|------|--------|-----------|
| **`CheckAdcStatus()`** | `EPB/Peripheral/ADC/Adc_Api.c` | **134** | `TaskScheduler_5ms()` | 5ms 주기, 모든 시스템 상태 |

### 2.3 호출 위치 상세 (TaskScheduler_5ms 내)

| 시스템 상태 | 호출 라인 | 근거 (파일/라인) | 호출 조건 |
|-------------|-----------|------------------|-----------|
| **SystemDown == TRUE** | **231** | `EPB/Main_Api.c:231` | IGN Off 상태 |
| **SYSTEM_STATUS_INIT** | **257** | `EPB/Main_Api.c:257` | 시스템 초기화 중 |
| **SYSTEM_STATUS_NORMAL** | **265** | `EPB/Main_Api.c:265` | 정상 동작 중 |

**선정 이유**: `CheckAdcStatus()`는 ADC 기능의 최상위 진입점으로, 내부에서 모든 ADC 처리(`GetRawData()` + `SetLogicData()`)를 수행하므로 Fs_Adc API로 치환하기에 가장 적합

## 3. HW Touch 지점 목록과 위치

### 3.1 직접 레지스터 접근 (MCAL 레이어)

| 레지스터 | 접근 타입 | 파일/함수 | 라인 | 용도 | 접근 코드 |
|----------|-----------|-----------|------|------|-----------|
| **`ADC.MCR.R`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | **13** | ADC 제어 레지스터 초기화 | `ADC.MCR.R = 0x00000000;` |
| **`ADC.MCR.B.OWREN`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | **14** | 오버라이트 활성화 | `ADC.MCR.B.OWREN = 1;` |
| **`ADC.MCR.B.ADCLKSEL`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | **15** | 클럭 선택 | `ADC.MCR.B.ADCLKSEL = 1;` |
| **`ADC.CTR[0].B.INPLATCH`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | **17** | 입력 래치 설정 | `ADC.CTR[0].B.INPLATCH = 1;` |
| **`ADC.CTR[0].B.INPCMP`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | **18** | 비교 클럭 설정 | `ADC.CTR[0].B.INPCMP = 2;` |
| **`ADC.CTR[0].B.INPSAMP`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | **19** | 샘플링 클럭 설정 | `ADC.CTR[0].B.INPSAMP = 17;` |
| **`ADC.NCMR[0].R`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **38** | 채널 0~31 선택 | `*adc_reg = (uint32_t)0x00000001<<(adc_ch-shift_ch);` |
| **`ADC.NCMR[1].R`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **43** | 채널 32~63 선택 | `*adc_reg = (uint32_t)0x00000001<<(adc_ch-shift_ch);` |
| **`ADC.NCMR[2].R`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **48** | 채널 64~95 선택 | `*adc_reg = (uint32_t)0x00000001<<(adc_ch-shift_ch);` |
| **`ADC.MCR.B.NSTART`** | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **53** | 변환 시작 | `ADC.MCR.B.NSTART = 1;` |
| **`ADC.MSR.B.NSTART`** | Read | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **55** | 변환 완료 대기 | `while(ADC.MSR.B.NSTART!=0)` |
| **`ADC.CDR[adc_ch].R`** | Read | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **61** | 변환 결과 읽기 | `ADC.CDR[adc_ch].R & 0x000003FF` |

### 3.2 외부 IC 통신 (BSW 레이어)

| 외부 IC | 접근 데이터 | 파일/함수 | 라인 | 용도 | 접근 코드 |
|---------|-------------|-----------|------|------|-----------|
| **L9369** | `g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR]` | `EPB/Application/MotorControl/Spi_L9369_Api.c` / `EpbMotorVoltage()` | **1706** | 좌측 모터 브리지 전압 | `value = (float)(g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR]);` |
| **L9369** | `g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR]` | `EPB/Application/MotorControl/Spi_L9369_Api.c` / `EpbMotorVoltage()` | **1710** | 우측 모터 브리지 전압 | `value = (float)(g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR]);` |
| **L9369** | `g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR]` | `EPB/Application/MotorControl/Spi_L9369_Api.c` / `EpbMotorCurrent()` | **1688** | 좌측 모터 전류 센싱 | `value = (float)(g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR]);` |
| **L9369** | `g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR]` | `EPB/Application/MotorControl/Spi_L9369_Api.c` / `EpbMotorCurrent()` | **1691** | 우측 모터 전류 센싱 | `value = (float)(g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR]);` |

### 3.3 전역 변수 접근 (상태 관리)

| 전역 변수 | 필드 | 접근 타입 | 파일/함수 | 라인 | 용도 |
|-----------|------|-----------|-----------|------|------|
| **`g_ADC`** | `Flag.Timeout` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | **54** | ADC 변환 타임아웃 플래그 |
| **`g_ADC`** | `Raw.UBB` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetRawData()` | **108** | 배터리 전압 Raw 값 |
| **`g_ADC`** | `Raw.IGN` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetRawData()` | **109** | IGN 전압 Raw 값 |
| **`g_ADC`** | `Raw.UBVR` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetRawData()` | **110** | UBVR 전압 Raw 값 |
| **`g_ADC`** | `Physical.Power.*` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetRawData()` | **112-115** | 전원 전압 물리값 (mV) |
| **`g_ADC`** | `Physical.Motor.*` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetRawData()` | **117-120** | 모터 전압/전류 물리값 (mV/mA) |

### 3.4 HW Touch 지점 요약

| 레이어 | HW Touch 개수 | 주요 접근 대상 | 파일 위치 |
|--------|---------------|----------------|-----------|
| **MCAL** | **12개** | ADC 레지스터 직접 접근 | `EPB/Peripheral/ADC/Adc_Api.c` |
| **BSW** | **4개** | L9369 IC SPI 통신 데이터 | `EPB/Application/MotorControl/Spi_L9369_Api.c` |
| **전역 상태** | **11개** | g_ADC 구조체 필드 | `EPB/Peripheral/ADC/Adc_Api.c` |

**총 HW Touch 지점**: **27개** (레지스터 12개 + L9369 통신 4개 + 전역 상태 11개)

## 4. 검증 요약

### 4.1 단위 확정 결과
- **ADC Raw**: 0~1023 (10bit)
- **물리값**: mV(전압), mA(전류) - ADC_REF=1000 스케일링 적용
- **CAN 출력**: 0.1V, 0.1A - /100.0 스케일링 적용

### 4.2 치환 대상 확정
- **함수**: `CheckAdcStatus()` (EPB/Peripheral/ADC/Adc_Api.c:134)
- **호출자**: `TaskScheduler_5ms()` (3곳에서 호출: 라인 231, 257, 265)
- **주기**: 5ms

### 4.3 HW Touch 지점 확정
- **MCAL**: ADC 레지스터 12개 접근점
- **BSW**: L9369 IC 4개 데이터 접근점  
- **전역 상태**: g_ADC 구조체 11개 필드 접근점

**근거**: 모든 분석 내용은 실제 소스 코드 파일의 라인별 분석 결과