# ADC 센서 데이터 처리 As-Is 팩트 패킷

## 1. Entry & Call Context

### 1.1 최초 호출 지점 (상위 호출자)

| 호출자 | 파일 위치 | 함수명 | 라인 범위 | 호출 조건 |
|--------|-----------|--------|-----------|-----------|
| `TaskScheduler_5ms()` | `EPB/Main_Api.c` | `CheckAdcStatus()` | 231, 257, 265 | 모든 시스템 상태 (SystemDown/INIT/NORMAL) |

### 1.2 호출 주기 및 경로 (3 depth)

```
STM_CH0_ISR() [1ms ISR]
    ↓
TaskScheduler_TaskCount() [EPB/Main_Api.c:185]
    ↓
TaskScheduler_ActivateTask() [EPB/Main_Api.c:289] (메인 루프)
    ↓
TaskScheduler_TaskCalculation() [EPB/Main_Api.c:118]
    ↓
TaskScheduler_5ms() [EPB/Main_Api.c:225]
    ↓
CheckAdcStatus() [EPB/Peripheral/ADC/Adc_Api.c:134] (5ms 주기)
```

**근거**: `EPB/Main_Api.c:231, 257, 265` 라인에서 `CheckAdcStatus()` 호출 확인

## 2. Function Boundary (핵심)

### 2.1 ADC 처리 함수 분해 (가장 작은 단위)

| 함수명 | 파일 위치 | 라인 범위 | 분류 | 근거 |
|--------|-----------|-----------|------|------|
| `CheckAdcStatus()` | `EPB/Peripheral/ADC/Adc_Api.c` | 134-137 | **Intent** | 단순 호출 시퀀스 제어 |
| `GetRawData()` | `EPB/Peripheral/ADC/Adc_Api.c` | 104-122 | **Mixed** | ADC 읽기(Wiring) + 물리값 변환(Intent) |
| `GetAdcAverage()` | `EPB/Peripheral/ADC/Adc_Api.c` | 66-85 | **Intent** | 평균/필터링 알고리즘 |
| `GetADCdata()` | `EPB/Peripheral/ADC/Adc_Api.c` | 28-64 | **Wiring** | ADC 레지스터 직접 접근 |
| `ConvertToPhysicalBatVolt()` | `EPB/Peripheral/ADC/Adc_Api.c` | 87-95 | **Intent** | 수학적 변환 공식 |
| `EpbMotorVoltage()` | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 1701-1713 | **Wiring** | L9369 ADC 데이터 읽기 |
| `EpbMotorCurrent()` | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 1683-1700 | **Wiring** | L9369 ADC 데이터 읽기 |
| `SetLogicData()` | `EPB/Peripheral/ADC/Adc_Api.c` | 124-132 | **Intent** | 전역 변수 데이터 전달 |

### 2.2 Intent vs Wiring 분류 상세

**Intent (알고리즘/로직)**:
- `GetAdcAverage()`: 6회 샘플링 후 최대/최소 제거하여 4개 평균 계산
- `ConvertToPhysicalBatVolt()`: `(ADC값/1023) * Vref * ((R1+R2)/R2)` 수식
- `SetLogicData()`: 계산된 물리값을 SSMInput, PbcInDriver 구조체에 복사

**Wiring (하드웨어 접근)**:
- `GetADCdata()`: ADC 레지스터 직접 읽기/쓰기
- `EpbMotorVoltage/Current()`: L9369 IC 내부 ADC 데이터 접근

## 3. Hardware Touch Points

### 3.1 레지스터 직접 접근

| 레지스터 | 접근 위치 | 파일/함수 | 라인 | 용도 |
|----------|-----------|-----------|------|------|
| `ADC.MCR.R` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | 13 | ADC 제어 레지스터 초기화 |
| `ADC.MCR.B.OWREN` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | 14 | 오버라이트 활성화 |
| `ADC.MCR.B.ADCLKSEL` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | 15 | 클럭 선택 |
| `ADC.CTR[0].B.*` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `ADCInitialize()` | 17-19 | 변환 타이밍 설정 |
| `ADC.NCMR[0/1/2].R` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | 38, 43, 48 | 채널 선택 레지스터 |
| `ADC.MCR.B.NSTART` | Write | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | 53 | 변환 시작 |
| `ADC.MSR.B.NSTART` | Read | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | 55 | 변환 완료 대기 |
| `ADC.CDR[adc_ch].R` | Read | `EPB/Peripheral/ADC/Adc_Api.c` / `GetADCdata()` | 61 | 변환 결과 읽기 |

### 3.2 디바이스 연계 (L9369)

| 함수 | 접근 대상 | 파일 위치 | 라인 | 용도 |
|------|-----------|-----------|------|------|
| `EpbMotorVoltage()` | `g_L9369_Adc_Value.VSBRIDGE_A/B[L9369_ADC_AVR]` | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 1706, 1710 | 모터 브리지 전압 |
| `EpbMotorCurrent()` | `g_L9369_Adc_Value.CS1_A/B[L9369_ADC_AVR]` | `EPB/Application/MotorControl/Spi_L9369_Api.c` | 1688, 1691 | 모터 전류 센싱 |

## 4. Global State Impact

### 4.1 g_MAIN / g_TaskControl 접근

| 전역 변수 | 필드 | Read/Write | 접근 위치 | 파일/함수 | 용도 |
|-----------|------|------------|-----------|-----------|------|
| **없음** | - | - | - | - | ADC 처리는 g_MAIN, g_TaskControl 접근 안함 |

### 4.2 기타 전역 변수 접근

| 전역 변수 | 필드 | Read/Write | 접근 위치 | 파일/함수 | 용도 |
|-----------|------|------------|-----------|-----------|------|
| `g_ADC` | `Flag.Timeout` | Write | `EPB/Peripheral/ADC/Adc_Api.c:54` | `GetADCdata()` | ADC 변환 타임아웃 플래그 |
| `g_ADC` | `Raw.UBB` | Write | `EPB/Peripheral/ADC/Adc_Api.c:108` | `GetRawData()` | 배터리 전압 Raw 값 |
| `g_ADC` | `Raw.IGN` | Write | `EPB/Peripheral/ADC/Adc_Api.c:109` | `GetRawData()` | IGN 전압 Raw 값 |
| `g_ADC` | `Raw.UBVR` | Write | `EPB/Peripheral/ADC/Adc_Api.c:110` | `GetRawData()` | UBVR 전압 Raw 값 |
| `g_ADC` | `Physical.Power.*` | Write | `EPB/Peripheral/ADC/Adc_Api.c:112-115` | `GetRawData()` | 전원 전압 물리값 |
| `g_ADC` | `Physical.Motor.*` | Write | `EPB/Peripheral/ADC/Adc_Api.c:117-120` | `GetRawData()` | 모터 전압/전류 물리값 |
| `SSMInput` | `SSMInVoltage` | Write | `EPB/Peripheral/ADC/Adc_Api.c:125` | `SetLogicData()` | EPB 로직으로 전압 전달 |
| `PbcInDriver` | `MotorDriverSupplyVoltage` | Write | `EPB/Peripheral/ADC/Adc_Api.c:127` | `SetLogicData()` | 모터 드라이버 공급 전압 |
| `PbcInDriver` | `MotorVoltageLeft/Right` | Write | `EPB/Peripheral/ADC/Adc_Api.c:128-129` | `SetLogicData()` | 좌/우 모터 전압 |
| `PbcInDriver` | `MotorCurrentLeft/Right` | Write | `EPB/Peripheral/ADC/Adc_Api.c:130-131` | `SetLogicData()` | 좌/우 모터 전류 |

## 5. Inputs/Outputs

### 5.1 입력 (Inputs)

| 입력 타입 | 데이터 소스 | 접근 위치 | 파일/함수 | 설명 |
|-----------|-------------|-----------|-----------|------|
| **하드웨어** | ADC 채널 32 (IGN) | `ADC_CH_SUB_IGN_MON` | `GetRawData()` | IGN 스위치 전압 |
| **하드웨어** | ADC 채널 9 (UBVR) | `ADC_CH_SUB_UBVR_MON` | `GetRawData()` | 밸브 배터리 전압 |
| **하드웨어** | ADC 채널 10 (UBB) | `ADC_CH_SUB_UBB_MON` | `GetRawData()` | 모터 배터리 전압 |
| **L9369 IC** | `g_L9369_Adc_Value.*` | L9369 SPI 통신 결과 | `EpbMotorVoltage/Current()` | 모터 전압/전류 센싱 |
| **상수** | `ref_3v3`, `r300`, `r51`, `ADC_REF` | 헤더 파일 정의 | `ConvertToPhysicalBatVolt()` | 변환 계수 |

### 5.2 출력 (Outputs)

| 출력 타입 | 데이터 목적지 | 저장 위치 | 파일/함수 | 설명 |
|-----------|---------------|-----------|-----------|------|
| **전역 저장** | `g_ADC.Raw.*` | ADC Raw 값 | `GetRawData()` | 10bit ADC 원시 데이터 |
| **전역 저장** | `g_ADC.Physical.*` | 물리값 | `GetRawData()` | 전압/전류 물리값 (mV, mA) |
| **EPB 로직** | `SSMInput.SSMInVoltage` | EPB 알고리즘 입력 | `SetLogicData()` | 배터리 전압 정보 |
| **모터 제어** | `PbcInDriver.*` | 모터 제어 입력 | `SetLogicData()` | 모터 전압/전류 정보 |
| **반환값** | 함수 리턴 | 지역 변수 | `GetAdcAverage()`, `ConvertToPhysicalBatVolt()` | 계산 결과 |

## 6. Verification Hooks (확인 항목)

### 6.1 ADC Raw 데이터 확인

| 확인 항목 | 변수명 | 파일 위치 | 예상 범위 | 설명 |
|-----------|--------|-----------|-----------|------|
| IGN Raw 값 | `g_ADC.Raw.IGN` | `EPB/Peripheral/ADC/Adc_Api.c:109` | 0~1023 | 10bit ADC 값 |
| UBB Raw 값 | `g_ADC.Raw.UBB` | `EPB/Peripheral/ADC/Adc_Api.c:108` | 0~1023 | 10bit ADC 값 |
| UBVR Raw 값 | `g_ADC.Raw.UBVR` | `EPB/Peripheral/ADC/Adc_Api.c:110` | 0~1023 | 10bit ADC 값 |

### 6.2 물리값 변환 확인

| 확인 항목 | 변수명 | 파일 위치 | 예상 범위 | 설명 |
|-----------|--------|-----------|-----------|------|
| IGN 물리값 | `g_ADC.Physical.Power.IGN` | `EPB/Peripheral/ADC/Adc_Api.c:114` | 0~20000 (mV) | IGN 전압 |
| UBB 물리값 | `g_ADC.Physical.Power.UBB` | `EPB/Peripheral/ADC/Adc_Api.c:112` | 0~20000 (mV) | 배터리 전압 |
| 모터A 전압 | `g_ADC.Physical.Motor.MOTORA` | `EPB/Peripheral/ADC/Adc_Api.c:117` | 0~20000 (mV) | 좌측 모터 전압 |
| 모터A 전류 | `g_ADC.Physical.Motor.MOTORA_CUR` | `EPB/Peripheral/ADC/Adc_Api.c:119` | 0~30000 (mA) | 좌측 모터 전류 |

### 6.3 EPB 로직 전달 확인

| 확인 항목 | 변수명 | 파일 위치 | 설명 |
|-----------|--------|-----------|------|
| EPB 입력 전압 | `SSMInput.SSMInVoltage` | `EPB/Peripheral/ADC/Adc_Api.c:125` | EPB 알고리즘으로 전달되는 배터리 전압 |
| 모터 드라이버 전압 | `PbcInDriver.MotorDriverSupplyVoltage` | `EPB/Peripheral/ADC/Adc_Api.c:127` | 모터 제어로 전달되는 공급 전압 |

### 6.4 CAN 출력 확인 (간접)

| 확인 항목 | 추적 경로 | 설명 |
|-----------|-----------|------|
| CAN 전압 정보 | `SSMInput.SSMInVoltage` → EPB 로직 → CAN 메시지 | EPB 상태 CAN 메시지에 포함되는 전압 정보 |
| 진단 정보 | ADC 타임아웃 → DTC 생성 → CAN 진단 메시지 | ADC 고장 시 진단 코드 생성 |

### 6.5 디버그 출력 위치

| 디버그 항목 | 접근 방법 | 설명 |
|-------------|-----------|------|
| ADC 변환 타임아웃 | `g_ADC.Flag.Timeout` | ADC 변환 실패 감지 |
| L9369 ADC 값 | `g_L9369_Adc_Value.*` | 모터 드라이버 내부 ADC 값 |
| 변환 공식 계수 | `ref_3v3`, `r300`, `r51` | 물리값 변환에 사용되는 상수 |

**근거**: 모든 항목은 `EPB/Peripheral/ADC/Adc_Api.c` 및 관련 파일의 코드 분석 결과