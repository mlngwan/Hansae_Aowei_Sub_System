# ADC 기능 패키지 As-Is 사실 확정 및 단위 분석

## 1. ADC 관련 출력 값의 현행 단위 확정

### 1.1 ADC 기본 단위 및 스케일링 팩터

| 항목 | 값 | 근거 (파일/변수/라인) | 설명 |
|------|----|--------------------|------|
| **ADC_REF** | `1000` | `EPB/Peripheral/ADC/Adc_Api.h:28` | 기본 스케일링 팩터 |
| **ADC 해상도** | `10bit (0~1023)` | `EPB/Peripheral/ADC/Adc_Api.c:61` `& 0x000003FF` | ADC 레지스터 마스킹 |
| **기준 전압** | `3.3V` | `EPB/Peripheral/ADC/Adc_Api.h:35` `ref_3v3` | ADC 기준 전압 |

### 1.2 물리값 변환 단위 (현행 프로젝트 기준)

| 물리값 타입 | 데이터 타입 | 단위 | 근거 (파일/함수/라인) | 계산 공식 |
|-------------|-------------|------|---------------------|-----------|
| **전원 전압** | `uint16` | **mV** | `EPB/Peripheral/ADC/Adc_Api.c:112-115` `*ADC_REF` | `ConvertToPhysicalBatVolt() * 1000` |
| **모터 전압** | `uint16` | **mV** | `EPB/Peripheral/ADC/Adc_Api.c:117-118` `*ADC_REF` | `EpbMotorVoltage() * 1000` |
| **모터 전류** | `uint16` | **mA** | `EPB/Peripheral/ADC/Adc_Api.c:119-120` `*ADC_REF` | `EpbMotorCurrent() * 1000` |

### 1.3 CAN 출력에서의 단위 변환 확인

| CAN 신호 | 스케일링 | 최종 단위 | 근거 (파일/함수/라인) | 설명 |
|----------|----------|-----------|---------------------|------|
| `EPBBoardvoltage` | `/100.0` | **0.1V** | `EPB/Application/Diagnostics/Can_Api.c:65` | mV → 0.1V 변환 |
| `EPBMotorAcurrent` | `/100.0` | **0.1A** | `EPB/Application/Diagnostics/Can_Api.c:66` | mA → 0.1A 변환 |
| `EPBMotorAvoltage` | `/100.0` | **0.1V** | `EPB/Application/Diagnostics/Can_Api.c:67` | mV → 0.1V 변환 |
| `EPBMotorBcurrent` | `/100.0` | **0.1A** | `EPB/Application/Diagnostics/Can_Api.c:68` | mA → 0.1A 변환 |
| `EPBMotorBvoltage` | `/100.0` | **0.1V** | `EPB/Application/Diagnostics/Can_Api.c:69` | mV → 0.1V 변환 |

**결론**: 현행 프로젝트에서 ADC 물리값은 **mV(전압), mA(전류)** 단위를 사용하며, CAN 출력 시 100으로 나누어 0.1V, 0.1A 단위로 전송

## 2. ADC 처리 흐름을 기능 단위로 분해 (Intent vs Wiring)

### 2.1 처리 단계별 함수 매핑

| 처리 단계 | 함수명 | 파일 위치 | 라인 범위 | 분류 | 근거 |
|-----------|--------|-----------|-----------|------|------|
| **1. Raw Start/Read** | `GetADCdata()` | `EPB/Peripheral/ADC/Adc_Api.c` | 28-64 | **Wiring** | ADC 레지스터 직접 접근 |
| **2. Averaging/Filtering** | `GetAdcAverage()` | `EPB/Peripheral/ADC/Adc_Api.c` | 66-85 | **Intent** | 6회 샘플링 후 최대/최소 제거 평균 |
| **3. Scaling/Convert** | `ConvertToPhysicalBatVolt()` | `EPB/Peripheral/ADC/Adc_Api.c` | 87-95 | **Intent** | 수학적 변환 공식 |
| **4. Store/Return** | `GetRawData()` | `EPB/Peripheral/ADC/Adc_Api.c` | 104-122 | **Mixed** | Raw 저장(Wiring) + 물리값 계산(Intent) |
| **5. Data Transfer** | `SetLogicData()` | `EPB/Peripheral/ADC/Adc_Api.c` | 124-132 | **Intent** | 전역 구조체 간 데이터 복사 |

### 2.2 Intent vs Wiring 상세 분류

#### Intent (알고리즘/로직) - ASW 영역
| 함수 | 알고리즘 내용 | 근거 |
|------|---------------|------|
| `GetAdcAverage()` | 6회 샘플링 → 최대/최소 제거 → 4개 평균 계산 | 통계적 노이즈 제거 알고리즘 |
| `ConvertToPhysicalBatVolt()` | `(ADC값/1023) * Vref * ((R1+R2)/R2)` | 전압 분배 회로 수학 공식 |
| `SetLogicData()` | 구조체 간 데이터 복사 및 매핑 | 데이터 인터페이스 로직 |

#### Wiring (하드웨어 접근) - BSW/MCAL 영역
| 함수 | 하드웨어 접근 내용 | 근거 |
|------|-------------------|------|
| `GetADCdata()` | ADC 레지스터 읽기/쓰기, 변환 시작/대기 | `ADC.NCMR`, `ADC.MCR`, `ADC.CDR` 직접 접근 |
| `EpbMotorVoltage()` | L9369 IC 내부 ADC 데이터 읽기 | SPI 통신을 통한 외부 IC 접근 |
| `EpbMotorCurrent()` | L9369 IC 내부 ADC 데이터 읽기 | SPI 통신을 통한 외부 IC 접근 |

## 3. Fs_Adc_* 포트(함수) 목록 제안

### 3.1 현재 ASW가 필요로 하는 입력/출력 기준

| 포트 함수명 | 반환 타입 | 단위 | 설명 | 현재 대응 함수 |
|-------------|-----------|------|------|----------------|
| `Fs_Adc_GetBatteryVoltage()` | `uint16` | **mV** | 메인 배터리 전압 | `g_ADC.Physical.Power.UBB` |
| `Fs_Adc_GetIgnitionVoltage()` | `uint16` | **mV** | IGN 스위치 전압 | `g_ADC.Physical.Power.IGN` |
| `Fs_Adc_GetValveBatteryVoltage()` | `uint16` | **mV** | 밸브 배터리 전압 | `g_ADC.Physical.Power.UBVR` |
| `Fs_Adc_GetMotorVoltageLeft()` | `uint16` | **mV** | 좌측 모터 전압 | `g_ADC.Physical.Motor.MOTORA` |
| `Fs_Adc_GetMotorVoltageRight()` | `uint16` | **mV** | 우측 모터 전압 | `g_ADC.Physical.Motor.MOTORB` |
| `Fs_Adc_GetMotorCurrentLeft()` | `uint16` | **mA** | 좌측 모터 전류 | `g_ADC.Physical.Motor.MOTORA_CUR` |
| `Fs_Adc_GetMotorCurrentRight()` | `uint16` | **mA** | 우측 모터 전류 | `g_ADC.Physical.Motor.MOTORB_CUR` |
| `Fs_Adc_GetAdcStatus()` | `boolean` | - | ADC 시스템 상태 | `g_ADC.Flag.Timeout` 기반 |

### 3.2 포트 함수 호출 주기 및 컨텍스트

| 포트 함수 | 호출 주기 | 호출자 | 근거 |
|-----------|-----------|--------|------|
| 모든 `Fs_Adc_*` 함수 | **5ms** | EPB 메인 로직 | 현재 `CheckAdcStatus()` 호출 주기 |

## 4. 하드웨어 접근 지점(HW Touch) 정리

### 4.1 MCAL 레이어 - 직접 레지스터 접근

| 레지스터 그룹 | 접근 레지스터 | 접근 타입 | 현재 위치 | 용도 |
|---------------|---------------|-----------|-----------|------|
| **ADC 제어** | `ADC.MCR.R` | Write | `ADCInitialize()` | ADC 모듈 초기화 |
| **ADC 제어** | `ADC.CTR[0].B.*` | Write | `ADCInitialize()` | 변환 타이밍 설정 |
| **채널 선택** | `ADC.NCMR[0/1/2].R` | Write | `GetADCdata()` | 변환 채널 선택 |
| **변환 제어** | `ADC.MCR.B.NSTART` | Write | `GetADCdata()` | 변환 시작 명령 |
| **상태 확인** | `ADC.MSR.B.NSTART` | Read | `GetADCdata()` | 변환 완료 대기 |
| **결과 읽기** | `ADC.CDR[ch].R` | Read | `GetADCdata()` | 변환 결과 획득 |

### 4.2 BSW 레이어 - 외부 IC 통신

| 외부 IC | 통신 방식 | 접근 데이터 | 현재 위치 | 용도 |
|---------|-----------|-------------|-----------|------|
| **L9369** | SPI | `g_L9369_Adc_Value.VSBRIDGE_A/B` | `EpbMotorVoltage()` | 모터 브리지 전압 |
| **L9369** | SPI | `g_L9369_Adc_Value.CS1_A/B` | `EpbMotorCurrent()` | 모터 전류 센싱 |

## 5. 전역 상태 필드 R/W 영향 정리

### 5.1 g_ADC 구조체 필드별 접근

| 필드 경로 | 데이터 타입 | R/W | 접근 함수 | 라인 | 용도 |
|-----------|-------------|-----|-----------|------|------|
| `g_ADC.Flag.Timeout` | `unsigned int:1` | **W** | `GetADCdata()` | 54 | ADC 변환 타임아웃 플래그 |
| `g_ADC.Raw.UBB` | `uint16` | **W** | `GetRawData()` | 108 | 배터리 전압 Raw 값 |
| `g_ADC.Raw.IGN` | `uint16` | **W** | `GetRawData()` | 109 | IGN 전압 Raw 값 |
| `g_ADC.Raw.UBVR` | `uint16` | **W** | `GetRawData()` | 110 | UBVR 전압 Raw 값 |
| `g_ADC.Physical.Power.UBB` | `uint16` | **W** | `GetRawData()` | 112 | 배터리 전압 물리값 (mV) |
| `g_ADC.Physical.Power.IGN` | `uint16` | **W** | `GetRawData()` | 114 | IGN 전압 물리값 (mV) |
| `g_ADC.Physical.Power.UBVR` | `uint16` | **W** | `GetRawData()` | 115 | UBVR 전압 물리값 (mV) |
| `g_ADC.Physical.Motor.MOTORA` | `uint16` | **W** | `GetRawData()` | 117 | 좌측 모터 전압 (mV) |
| `g_ADC.Physical.Motor.MOTORB` | `uint16` | **W** | `GetRawData()` | 118 | 우측 모터 전압 (mV) |
| `g_ADC.Physical.Motor.MOTORA_CUR` | `uint16` | **W** | `GetRawData()` | 119 | 좌측 모터 전류 (mA) |
| `g_ADC.Physical.Motor.MOTORB_CUR` | `uint16` | **W** | `GetRawData()` | 120 | 우측 모터 전류 (mA) |

### 5.2 외부 인터페이스 구조체 필드별 접근

| 구조체.필드 | 데이터 타입 | R/W | 접근 함수 | 라인 | 용도 |
|-------------|-------------|-----|-----------|------|------|
| `SSMInput.SSMInVoltage` | `real32_T` | **W** | `SetLogicData()` | 125 | EPB 로직 배터리 전압 입력 |
| `PbcInDriver.MotorDriverSupplyVoltage` | `uint16_T` | **W** | `SetLogicData()` | 127 | 모터 드라이버 공급 전압 |
| `PbcInDriver.MotorVoltageLeft` | `real32_T` | **W** | `SetLogicData()` | 128 | 좌측 모터 전압 |
| `PbcInDriver.MotorVoltageRight` | `real32_T` | **W** | `SetLogicData()` | 129 | 우측 모터 전압 |
| `PbcInDriver.MotorCurrentLeft` | `real32_T` | **W** | `SetLogicData()` | 130 | 좌측 모터 전류 |
| `PbcInDriver.MotorCurrentRight` | `real32_T` | **W** | `SetLogicData()` | 131 | 우측 모터 전류 |

### 5.3 g_MAIN / g_TaskControl 접근 여부

| 전역 변수 | 접근 여부 | 근거 |
|-----------|-----------|------|
| `g_MAIN` | **접근 안함** | ADC 관련 모든 함수에서 g_MAIN 참조 없음 |
| `g_TaskControl` | **접근 안함** | ADC 관련 모든 함수에서 g_TaskControl 참조 없음 |

**결론**: ADC 기능은 독립적인 전역 구조체(`g_ADC`)를 사용하며, 시스템 전역 상태와 직접적인 의존성이 없음

## 6. 기능 패키지 구조 제안 요약

### 6.1 파일 구조 (Feature_Adc 폴더 기준)

```
Feature_Adc/
├── Adc_Types.h          # 공용 타입 정의
├── Fs_Adc.c/.h         # ASW 레이어 (Intent)
├── Bsw_Adc.c/.h        # BSW 레이어 (외부 IC 통신)
└── Mcal_Adc.c/.h       # MCAL 레이어 (레지스터 접근)
```

### 6.2 레이어별 책임 분담

| 레이어 | 파일 | 담당 기능 | 현재 대응 함수 |
|--------|------|-----------|----------------|
| **ASW** | `Fs_Adc.*` | 평균/필터링, 물리값 변환, 데이터 전달 | `GetAdcAverage()`, `ConvertToPhysicalBatVolt()`, `SetLogicData()` |
| **BSW** | `Bsw_Adc.*` | L9369 통신, 상위 레이어 인터페이스 | `EpbMotorVoltage()`, `EpbMotorCurrent()` |
| **MCAL** | `Mcal_Adc.*` | ADC 레지스터 직접 접근, 초기화 | `ADCInitialize()`, `GetADCdata()` |

### 6.3 단위 유지 원칙

- **반환 단위**: 현행 프로젝트 기준 **mV(전압), mA(전류)** 유지
- **데이터 타입**: `uint16` 유지 (기존 구조체 호환성)
- **스케일링**: `ADC_REF = 1000` 팩터 유지
- **CAN 출력**: 기존 `/100.0` 스케일링 유지 (0.1V, 0.1A 단위)

**근거**: 모든 분석 내용은 `EPB/Peripheral/ADC/Adc_Api.c/h`, `EPB/Application/Diagnostics/Can_Api.c`, `EPB/AppLogic/ElectronicParkBrake_types.h` 파일의 실제 코드 분석 결과