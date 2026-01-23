/**
 * @file Adc_Types.h
 * @brief ADC Feature Package - Common Type Definitions
 *
 * Purpose: 레이어 간 공용 타입/상수 정의 (Intent 불변)
 * Scope: FS/BSW/MCAL 레이어가 공유하는 타입만 포함
 *
 * Refactoring Note:
 * - 기존 프로젝트의 타입/단위를 그대로 사용
 * - 새로운 타입 정의 금지
 * - 현재 단위: mV(전압), mA(전류), uint16 타입 유지
 */

#ifndef ADC_TYPES_H
#define ADC_TYPES_H

#include "typedefs.h"

/*===========================================================================
 * ADC 공용 상수 정의 (현행 프로젝트 단위)
 *===========================================================================*/
/* 근거: EPB/Peripheral/ADC/Adc_Api.h:28, ADC_SKELETON_VERIFICATION_FACTS.md 섹션 1.1 */
#define ADC_REF 1000u /* V → mV 변환 스케일링 팩터 (ConvertToPhysicalBatVolt() * ADC_REF) */

/* 근거: EPB/Peripheral/ADC/Adc_Api.h:35-37, ADC_SKELETON_VERIFICATION_FACTS.md 섹션 1.1 */
#define ADC_VREF_MV 3300u /* ADC 기준 전압 3.3V (mV 단위) */
#define ADC_R_UPPER 300u  /* 전압 분배 회로 상단 저항값 (kΩ) */
#define ADC_R_LOWER 51u   /* 전압 분배 회로 하단 저항값 (kΩ) */

/* ADC 해상도 (10bit) */
#define ADC_RESOLUTION_10BIT 1023u /* 10bit ADC 최대값 */
#define ADC_RAW_MASK 0x03FFu       /* 10bit 마스크 (0~1023) */

/*===========================================================================
 * ADC 채널 논리 ID (Hardware-independent)
 *===========================================================================*/
/* 근거: ADC_FEATURE_FACT_PACKET_v2.md 섹션 3.1 */
typedef enum
{
  ADC_CH_BATTERY_VOLTAGE = 0,   /* 메인 배터리 전압 (UBB) */
  ADC_CH_IGNITION_VOLTAGE,      /* IGN 스위치 전압 */
  ADC_CH_VALVE_BATTERY_VOLTAGE, /* 밸브 배터리 전압 (UBVR) */
  ADC_CH_MOTOR_VOLTAGE_LEFT,    /* 좌측 모터 전압 */
  ADC_CH_MOTOR_VOLTAGE_RIGHT,   /* 우측 모터 전압 */
  ADC_CH_MOTOR_CURRENT_LEFT,    /* 좌측 모터 전류 */
  ADC_CH_MOTOR_CURRENT_RIGHT,   /* 우측 모터 전류 */
  ADC_CH_MAX
} Adc_ChannelIdType;

/*===========================================================================
 * ADC 물리 채널 매핑 (Hardware-specific)
 *===========================================================================*/
/* 근거: EPB/Peripheral/ADC/Adc_Api.h:30-32 */
typedef enum
{
  ADC_HW_CH_IGN_MON = 32, /* IGN 전압 모니터링 채널 */
  ADC_HW_CH_UBVR_MON = 9, /* UBVR 전압 모니터링 채널 */
  ADC_HW_CH_UBB_MON = 10  /* UBB 전압 모니터링 채널 */
} Adc_HwChannelType;

/*===========================================================================
 * ADC 반환 상태 타입
 *===========================================================================*/
typedef enum
{
  ADC_STATUS_OK = 0,        /* 정상 동작 */
  ADC_STATUS_ERROR,         /* 일반 에러 */
  ADC_STATUS_TIMEOUT,       /* 변환 타임아웃 */
  ADC_STATUS_OUT_OF_RANGE,  /* 범위 초과 (Safety) */
  ADC_STATUS_INVALID_PARAM, /* 잘못된 파라미터 */
  ADC_STATUS_SAFE_STATE     /* Safe State 진입 */
} Adc_StatusType;

/*===========================================================================
 * ADC 데이터 타입 (현행 프로젝트 단위 확정)
 *===========================================================================*/
/* 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 1.1
 *
 * 물리값 변환 흐름 및 단위:
 * 1. Raw ADC: 0~1023 (10bit) → GetADCdata() 결과
 * 2. 전압 계산: (adc/1023) * 3.3V * (351/51) = V (볼트)
 * 3. 최종 저장: ConvertToPhysicalBatVolt() * ADC_REF = mV (밀리볼트)
 * 4. CAN 출력: mV / 100 = 0.1V (십분의 일 볼트)
 *
 * 전류는 L9369에서 직접 mA 단위로 제공됨
 */
typedef uint16 Adc_RawValueType; /* Raw ADC 값 (0~1023, 10bit) */
typedef uint16 Adc_VoltageType;  /* 물리 전압값 (mV 단위) */
typedef uint16 Adc_CurrentType;  /* 물리 전류값 (mA 단위) */

/*===========================================================================
 * ADC 물리값 변환 파라미터
 *===========================================================================*/
/* 근거: ADC_FEATURE_FACT_PACKET_v2.md 섹션 1.1 */
typedef struct
{
  uint16 vref_mv; /* 기준 전압 (mV) */
  uint16 r_upper; /* 상단 저항 (kΩ) */
  uint16 r_lower; /* 하단 저항 (kΩ) */
} Adc_ConversionParamsType;

/*===========================================================================
 * ADC 진단 카운터 (Safety Monitoring)
 *===========================================================================*/
typedef struct
{
  uint16 timeoutCount;      /* 타임아웃 발생 횟수 */
  uint16 rangeErrorCount;   /* 범위 초과 발생 횟수 */
  uint16 consecutiveErrors; /* 연속 에러 카운트 */
} Adc_DiagCountersType;

/*===========================================================================
 * ADC Safety Range 설정
 *===========================================================================*/
typedef struct
{
  uint32 min_valid_mv;  /* 최소 유효 전압 (mV) */
  uint32 max_valid_mv;  /* 최대 유효 전압 (mV) */
  uint32 safe_value_mv; /* Safe fallback 값 (mV) */
} Adc_RangeConfigType;

/*===========================================================================
 * L9369 모터 드라이버 ADC 타입 (BSW 레이어용)
 *===========================================================================*/
/* 근거: Spi_L9369_Api.c */
typedef enum
{
  ADC_L9369_VSBRIDGE_A = 0, /* 좌측 모터 브리지 전압 */
  ADC_L9369_VSBRIDGE_B,     /* 우측 모터 브리지 전압 */
  ADC_L9369_CS1_A,          /* 좌측 모터 전류 센싱 */
  ADC_L9369_CS1_B,          /* 우측 모터 전류 센싱 */
  ADC_L9369_MAX
} Adc_L9369AdcType;

#endif /* ADC_TYPES_H */
