/**
 * @file Mcal_Adc.h
 * @brief MCAL Layer - ADC Hardware Abstraction (Wiring Only)
 *
 * Purpose: 하드웨어 레지스터 직접 접근 래핑
 * Scope: ADC 레지스터, L9369 글로벌 변수 접근
 * Intent: NONE - Pure Wiring Only
 *
 * Refactoring Note:
 * - 기존: EPB/Peripheral/ADC/Adc_Api.c의 GetADCdata(), ADCInitialize()
 * - 이동 대상: 하드웨어 레지스터 접근 코드만
 * - Intent 로직(평균/필터/변환)은 BSW/FS로
 */

#ifndef MCAL_ADC_H
#define MCAL_ADC_H

#include "Adc_Types.h"

/*===========================================================================
 * MCAL ADC API - Hardware Operations
 *===========================================================================*/

/**
 * @brief Initialize ADC hardware registers
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 기존 ADCInitialize() 내용 이동
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:9-21
 *
 * 이동 대상 코드:
 * - ADC.MCR.R = 0x80000000;
 * - ADC.MCR.B.OWREN = 1;
 * - ADC.MCR.B.ADCLKSEL = 1;
 * - ADC.CTR[0].B.* 설정
 */
Adc_StatusType Mcal_Adc_Init(void);

/**
 * @brief Read raw ADC value from single hardware channel
 * @param[in] hwChannel Physical ADC channel number (0-63)
 * @param[out] rawValue Pointer to store 10-bit raw value (0-1023)
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 기존 GetADCdata() 내용 이동
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:28-64
 *
 * 이동 대상 코드:
 * - ADC.NCMR[x].R 채널 선택
 * - ADC.MCR.B.NSTART = 1 변환 시작
 * - while(ADC.MSR.B.NSTART) 변환 완료 대기
 * - ADC.CDR[channel].R 결과 읽기
 *
 * @note Blocking call with timeout protection
 */
Adc_StatusType Mcal_Adc_ReadChannel(Adc_HwChannelType hwChannel, Adc_RawValueType *rawValue);

/**
 * @brief Read L9369 motor driver internal ADC value
 * @param[in] l9369AdcType L9369 ADC type (VSBRIDGE/CS1)
 * @param[out] rawValue Pointer to store L9369 ADC value
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 기존 EpbMotorVoltage(), EpbMotorCurrent() 래핑
 * 근거: EPB/Application/MotorControl/Spi_L9369_Api.c:1701, 1683
 *
 * 접근 대상:
 * - g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR]
 * - g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR]
 * - g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR]
 * - g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR]
 */
Adc_StatusType Mcal_Adc_ReadL9369Adc(Adc_L9369AdcType l9369AdcType, uint16 *rawValue);

/**
 * @brief Get ADC conversion timeout flag
 * @return 1 if timeout occurred, 0 otherwise
 *
 * TODO: g_ADC.Flag.Timeout 래핑
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:54
 */
uint8 Mcal_Adc_GetTimeoutFlag(void);

/**
 * @brief Clear ADC conversion timeout flag
 *
 * TODO: g_ADC.Flag.Timeout = 0 래핑
 */
void Mcal_Adc_ClearTimeoutFlag(void);

#endif /* MCAL_ADC_H */
