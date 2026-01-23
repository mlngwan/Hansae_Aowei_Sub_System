# ADC 레이어 분리 리팩토링 - 이동 목록 + 의존성 팩트

## 1. 각 함수가 참조하는 타입/매크로/전역 심볼 목록

### 1.1 MCAL Layer 함수 의존성

#### `Mcal_Adc_Initialize()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **레지스터** | `ADC.MCR.R` | `EPB/Peripheral/ADC/Adc_Api.c:13` | ADC 제어 레지스터 |
| **레지스터** | `ADC.MCR.B.OWREN` | `EPB/Peripheral/ADC/Adc_Api.c:14` | 오버라이트 비트 필드 |
| **레지스터** | `ADC.MCR.B.ADCLKSEL` | `EPB/Peripheral/ADC/Adc_Api.c:15` | 클럭 선택 비트 필드 |
| **레지스터** | `ADC.CTR[0].B.INPLATCH` | `EPB/Peripheral/ADC/Adc_Api.c:17` | 입력 래치 비트 필드 |
| **레지스터** | `ADC.CTR[0].B.INPCMP` | `EPB/Peripheral/ADC/Adc_Api.c:18` | 비교 클럭 비트 필드 |
| **레지스터** | `ADC.CTR[0].B.INPSAMP` | `EPB/Peripheral/ADC/Adc_Api.c:19` | 샘플링 클럭 비트 필드 |

#### `Mcal_Adc_ReadChannel()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **타입** | `uword` | `EPB/Peripheral/typedefs.h:75` | unsigned int 타입 |
| **타입** | `uint8_t` | `EPB/Peripheral/typedefs.h:64` | unsigned char 타입 |
| **타입** | `vuint32_t` | `EPB/Peripheral/typedefs.h:78` | volatile unsigned int 타입 |
| **레지스터** | `ADC.NCMR[0/1/2].R` | `EPB/Peripheral/ADC/Adc_Api.c:38,43,48` | 채널 선택 레지스터 |
| **레지스터** | `ADC.MCR.B.NSTART` | `EPB/Peripheral/ADC/Adc_Api.c:53` | 변환 시작 비트 |
| **레지스터** | `ADC.MSR.B.NSTART` | `EPB/Peripheral/ADC/Adc_Api.c:55` | 변환 상태 비트 |
| **레지스터** | `ADC.CDR[adc_ch].R` | `EPB/Peripheral/ADC/Adc_Api.c:61` | 변환 결과 레지스터 |
| **전역 변수** | `g_ADC.Flag.Timeout` | `EPB/Peripheral/ADC/Adc_Api.h:58` | 타임아웃 플래그 |

#### `Mcal_Adc_ReadL9369Motor()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **타입** | `uint8_t` | `EPB/Peripheral/typedefs.h:64` | unsigned char 타입 |
| **타입** | `float` | C 표준 | 부동소수점 타입 |
| **매크로** | `L9369_LEFT` | `EPB/Application/MotorControl/Spi_L9369Data_Types.h` | 좌측 모터 ID |
| **매크로** | `L9369_ADC_AVR` | `EPB/Application/MotorControl/Spi_L9369Data_Types.h` | L9369 ADC 평균 인덱스 |
| **전역 변수** | `g_L9369_Adc_Value.VSBRIDGE_A` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1706` | 좌측 모터 브리지 전압 |
| **전역 변수** | `g_L9369_Adc_Value.VSBRIDGE_B` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1710` | 우측 모터 브리지 전압 |
| **전역 변수** | `g_L9369_Adc_Value.CS1_A` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1688` | 좌측 모터 전류 센싱 |
| **전역 변수** | `g_L9369_Adc_Value.CS1_B` | `EPB/Application/MotorControl/Spi_L9369_Api.c:1691` | 우측 모터 전류 센싱 |

### 1.2 BSW Layer 함수 의존성

#### `Bsw_Adc_GetAveraged()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **타입** | `ubyte` | `EPB/Peripheral/typedefs.h:63` | unsigned char 타입 |
| **타입** | `uword` | `EPB/Peripheral/typedefs.h:75` | unsigned int 타입 |
| **함수 호출** | `Mcal_Adc_ReadChannel()` | 새로 정의된 MCAL 함수 | MCAL 레이어 의존성 |

#### `Bsw_Adc_ConvertToPhysical()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **타입** | `uint16_t` | `EPB/Peripheral/typedefs.h:69` | unsigned short 타입 |
| **타입** | `float` | C 표준 | 부동소수점 타입 |

#### `Bsw_Adc_ReadAllChannels()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **매크로** | `ADC_CH_SUB_UBB_MON` | `EPB/Peripheral/ADC/Adc_Api.h:12` | UBB 채널 번호 (10) |
| **매크로** | `ADC_CH_SUB_IGN_MON` | `EPB/Peripheral/ADC/Adc_Api.h:11` | IGN 채널 번호 (32) |
| **매크로** | `ADC_CH_SUB_UBVR_MON` | `EPB/Peripheral/ADC/Adc_Api.h:13` | UBVR 채널 번호 (9) |
| **매크로** | `ref_3v3` | `EPB/Peripheral/ADC/Adc_Api.h:35` | 기준 전압 (3.3) |
| **매크로** | `r300` | `EPB/Peripheral/ADC/Adc_Api.h:30` | 저항값 (300) |
| **매크로** | `r51` | `EPB/Peripheral/ADC/Adc_Api.h:31` | 저항값 (51) |
| **매크로** | `ADC_REF` | `EPB/Peripheral/ADC/Adc_Api.h:28` | 스케일링 팩터 (1000) |
| **매크로** | `L9369_LEFT` | `EPB/Application/MotorControl/Spi_L9369Data_Types.h` | 좌측 모터 ID |
| **매크로** | `L9369_RIGHT` | `EPB/Application/MotorControl/Spi_L9369Data_Types.h` | 우측 모터 ID |
| **전역 변수** | `g_ADC.Raw.*` | `EPB/Peripheral/ADC/Adc_Api.h:48-52` | Raw ADC 값 구조체 |
| **전역 변수** | `g_ADC.Physical.*` | `EPB/Peripheral/ADC/Adc_Api.h:39-47` | 물리값 구조체 |
| **함수 호출** | `Bsw_Adc_GetAveraged()` | BSW 레이어 내부 | BSW 내부 의존성 |
| **함수 호출** | `Bsw_Adc_ConvertToPhysical()` | BSW 레이어 내부 | BSW 내부 의존성 |
| **함수 호출** | `Mcal_Adc_ReadL9369Motor()` | MCAL 레이어 | MCAL 레이어 의존성 |

### 1.3 FS Layer 함수 의존성

#### `Fs_Adc_UpdateData()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **타입** | `SSMInputBus` | `EPB/AppLogic/ElectronicParkBrake_types.h:52` | SSM 입력 버스 타입 |
| **타입** | `PbcInDriverBus` | `EPB/AppLogic/ElectronicParkBrake_types.h:95` | PBC 드라이버 입력 버스 타입 |
| **전역 변수** | `SSMInput` | `EPB/Peripheral/ADC/Adc_Api.c:4` | SSM 입력 버스 인스턴스 |
| **전역 변수** | `PbcInDriver` | `EPB/Peripheral/ADC/Adc_Api.c:5` | PBC 드라이버 입력 버스 인스턴스 |
| **전역 변수** | `g_ADC.Physical.*` | `EPB/Peripheral/ADC/Adc_Api.h:39-47` | 물리값 구조체 |

#### `Fs_Adc_ProcessAll()`
| 심볼 타입 | 심볼명 | 근거 (파일/라인) | 설명 |
|-----------|--------|------------------|------|
| **함수 호출** | `Bsw_Adc_ReadAllChannels()` | BSW 레이어 | BSW 레이어 의존성 |
| **함수 호출** | `Fs_Adc_UpdateData()` | FS 레이어 내부 | FS 내부 의존성 |

## 2. 파일 분리 시 필요한 include / extern 선언 위치

### 2.1 Mcal_Adc.c/.h 파일

#### Mcal_Adc.h (헤더)
```c
#ifndef MCAL_ADC_H
#define MCAL_ADC_H

/* 기본 타입 정의 */
#include "typedefs.h"  /* uword, uint8_t, vuint32_t */

/* 함수 선언 */
void Mcal_Adc_Initialize(void);
uword Mcal_Adc_ReadChannel(uint8_t adc_ch);
float Mcal_Adc_ReadL9369Motor(uint8_t motor_id, uint8_t data_type);

#endif
```

#### Mcal_Adc.c (구현)
```c
#include "Mcal_Adc.h"
#include "Spi_L9369Data_Types.h"  /* L9369_LEFT, L9369_ADC_AVR, g_L9369_Adc_Value */

/* 외부 전역 변수 선언 */
extern ADC_t g_ADC;  /* g_ADC.Flag.Timeout 접근용 */

/* 하드웨어 레지스터 접근 - 컴파일러/MCU 의존적 */
/* ADC.MCR, ADC.CTR, ADC.NCMR, ADC.MSR, ADC.CDR 레지스터 정의 필요 */
```

### 2.2 Bsw_Adc.c/.h 파일

#### Bsw_Adc.h (헤더)
```c
#ifndef BSW_ADC_H
#define BSW_ADC_H

/* 기본 타입 정의 */
#include "typedefs.h"  /* ubyte, uword, uint16_t */
#include "Mcal_Adc.h"  /* MCAL 레이어 의존성 */

/* 함수 선언 */
uword Bsw_Adc_GetAveraged(ubyte ADC_Channel);
float Bsw_Adc_ConvertToPhysical(uint16_t adcvalue, float v_ref, uint16_t r1, uint16_t r2);
void Bsw_Adc_ReadAllChannels(void);

#endif
```

#### Bsw_Adc.c (구현)
```c
#include "Bsw_Adc.h"
#include "Adc_Api.h"  /* ADC_CH_SUB_*, ref_3v3, r300, r51, ADC_REF, ADC_t */
#include "Spi_L9369Data_Types.h"  /* L9369_LEFT, L9369_RIGHT */

/* 외부 전역 변수 선언 */
extern ADC_t g_ADC;  /* g_ADC.Raw.*, g_ADC.Physical.* 접근용 */
```

### 2.3 Fs_Adc.c/.h 파일

#### Fs_Adc.h (헤더)
```c
#ifndef FS_ADC_H
#define FS_ADC_H

#include "Bsw_Adc.h"  /* BSW 레이어 의존성 */

/* FS 레이어 반환 타입 (프로젝트 기준) */
typedef enum {
    FS_ADC_OK = 0,
    FS_ADC_ERROR = 1,
    FS_ADC_TIMEOUT = 2,
    FS_ADC_SAFE_STATE = 3
} Fs_Adc_StatusType;

/* 함수 선언 */
void Fs_Adc_UpdateData(void);
Fs_Adc_StatusType Fs_Adc_ProcessAll(void);

#endif
```

#### Fs_Adc.c (구현)
```c
#include "Fs_Adc.h"
#include "ElectronicParkBrake_types.h"  /* SSMInputBus, PbcInDriverBus */
#include "Adc_Api.h"  /* ADC_t */

/* 외부 전역 변수 선언 */
extern SSMInputBus SSMInput;
extern PbcInDriverBus PbcInDriver;
extern ADC_t g_ADC;
```

## 3. 프로젝트에서 사용 가능한 반환 타입 근거 확인

### 3.1 기존 프로젝트 반환 타입 분석

| 반환 타입 | 사용 위치 | 근거 (파일/라인) | 설명 |
|-----------|-----------|------------------|------|
| **EE_OK** | Flash 모듈 | `EPB/Peripheral/Flash/eed_highlevel.c:92` | Flash EEPROM 성공 |
| **EE_ERROR_*** | Flash 모듈 | `EPB/Peripheral/Flash/eed_highlevel.c:447` | Flash EEPROM 에러 코드 |
| **TRUE/FALSE** | 전역 사용 | `EPB/Defines.h:4-9` | 불린 값 |
| **boolean_T** | Simulink 생성 | `EPB/AppLogic/rtwtypes.h:59` | Simulink 불린 타입 |

### 3.2 Std_ReturnType 사용 여부

| 항목 | 결과 | 근거 |
|------|------|------|
| **Std_ReturnType** | **사용 안함** | 검색 결과 없음 |
| **E_OK/E_NOT_OK** | **사용 안함** | 검색 결과 없음 |
| **AUTOSAR 표준 타입** | **사용 안함** | 프로젝트가 AUTOSAR 표준을 따르지 않음 |

### 3.3 FS 레이어 반환 타입 제안

**프로젝트 기준 권장 반환 타입**:
```c
typedef enum {
    FS_ADC_OK = 0,        /* 정상 처리 */
    FS_ADC_ERROR = 1,     /* 일반 에러 */
    FS_ADC_TIMEOUT = 2,   /* ADC 변환 타임아웃 */
    FS_ADC_SAFE_STATE = 3 /* 안전 상태 진입 */
} Fs_Adc_StatusType;
```

**근거**:
- 기존 프로젝트에서 `EE_OK`, `EE_ERROR_*` 패턴 사용 (Flash 모듈)
- `TRUE/FALSE` 불린 타입 광범위 사용
- AUTOSAR 표준 타입 미사용으로 자체 정의 필요

## 4. 빌드/단위/동작 불변 보장 방안

### 4.1 레거시 인터페이스 유지

| 기존 함수 | 새 구현 | 근거 |
|-----------|---------|------|
| `CheckAdcStatus()` | `Fs_Adc_ProcessAll()` 호출 | 상위 호출자 변경 없음 |
| `ADCInitialize()` | `Mcal_Adc_Initialize()` 호출 | 초기화 함수 호환성 |
| `GetADCdata()` | `Mcal_Adc_ReadChannel()` 호출 | 직접 ADC 읽기 호환성 |
| `GetAdcAverage()` | `Bsw_Adc_GetAveraged()` 호출 | 평균 계산 호환성 |
| `ConvertToPhysicalBatVolt()` | `Bsw_Adc_ConvertToPhysical()` 호출 | 물리값 변환 호환성 |

### 4.2 전역 변수 접근 유지

| 전역 변수 | 접근 레이어 | 변경 사항 |
|-----------|-------------|-----------|
| `g_ADC` | 모든 레이어 | **변경 없음** - 기존 구조체 그대로 사용 |
| `SSMInput` | FS 레이어만 | **변경 없음** - 기존 접근 방식 유지 |
| `PbcInDriver` | FS 레이어만 | **변경 없음** - 기존 접근 방식 유지 |

### 4.3 계산 의미 불변 보장

| 계산 로직 | 변경 여부 | 근거 |
|-----------|-----------|------|
| **6회 샘플링 평균** | **불변** | `Bsw_Adc_GetAveraged()` 동일 알고리즘 |
| **물리값 변환 공식** | **불변** | `Bsw_Adc_ConvertToPhysical()` 동일 수식 |
| **ADC_REF 스케일링** | **불변** | 기존 `*ADC_REF` 그대로 적용 |
| **L9369 데이터 접근** | **불변** | `Mcal_Adc_ReadL9369Motor()` 동일 접근 |

**결론**: 레이어 분리는 함수 호출 경로만 변경하고, 모든 계산 로직과 데이터 접근 방식은 기존과 동일하게 유지하여 빌드/단위/동작 불변을 보장함