/**
 * @file Bsw_Adc.h
 * @brief BSW Layer - ADC Basic Software Services
 *
 * Purpose: Hardware-independent ADC processing services
 * Scope: Intent 알고리즘 보존 (평균/필터링, 물리값 변환)
 * Intent: PRESERVED - 기존 계산식/알고리즘 그대로 유지
 *
 * Refactoring Note:
 * - 기존: GetAdcAverage(), ConvertToPhysicalBatVolt()
 * - 이동 대상: Intent 로직(계산식/필터링)
 * - 하드웨어 접근은 MCAL 호출로 대체
 */

#ifndef BSW_ADC_H
#define BSW_ADC_H

#include "Adc_Types.h"
#include "Mcal_Adc.h"

/*===========================================================================
 * BSW ADC API - Intent Algorithm Services
 *===========================================================================*/

/**
 * @brief Initialize BSW ADC layer
 * @return ADC_STATUS_OK or error code
 *
 * TODO: Mcal_Adc_Init() 호출
 */
Adc_StatusType Bsw_Adc_Init(void);

/**
 * @brief Read averaged ADC value with outlier filtering
 * @param[in] channelId Logical channel ID (ADC_CH_*)
 * @param[in] numSamples Number of samples to average (e.g., 6)
 * @param[out] avgRawValue Pointer to store averaged raw value
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 기존 GetAdcAverage() Intent 알고리즘 이동
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:66-85
 *
 * Intent 알고리즘 (불변):
 * 1. numSamples 횟수만큼 샘플링
 * 2. 최대값/최소값 찾기
 * 3. 최대값/최소값 제외하고 나머지 평균 계산
 *
 * Wiring 변경:
 * - 기존: GetADCdata() 직접 호출
 * - 변경: Mcal_Adc_ReadChannel() 호출
 */
Adc_StatusType Bsw_Adc_ReadAveraged(
    Adc_ChannelIdType channelId,
    uint8 numSamples,
    Adc_RawValueType *avgRawValue);

/**
 * @brief Convert raw ADC value to physical voltage (mV)
 * @param[in] rawValue 10-bit ADC raw value (0-1023)
 * @param[in] params Conversion parameters (Vref, R divider)
 * @param[out] voltage_mv Pointer to store voltage in millivolts
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 기존 ConvertToPhysicalBatVolt() Intent 공식 이동
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:87-95
 *
 * Intent 공식 (불변):
 * temp = rawValue * vref_mv / ADC_RESOLUTION_10BIT
 * temp = temp * (r_upper + r_lower) / r_lower
 * *voltage_mv = temp
 *
 * 단위: mV (기존 프로젝트 단위 유지)
 */
Adc_StatusType Bsw_Adc_ConvertToVoltage(
    Adc_RawValueType rawValue,
    const Adc_ConversionParamsType *params,
    Adc_VoltageType *voltage_mv);

/**
 * @brief Read motor ADC value from L9369 driver IC
 * @param[in] channelId Motor channel ID (ADC_CH_MOTOR_*)
 * @param[out] rawValue Pointer to store motor ADC value
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 논리 채널을 L9369 ADC 타입으로 매핑 후 MCAL 호출
 * 근거: EPB/Application/MotorControl/Spi_L9369_Api.c:1701, 1683
 *
 * 매핑:
 * - ADC_CH_MOTOR_VOLTAGE_LEFT → ADC_L9369_VSBRIDGE_A
 * - ADC_CH_MOTOR_VOLTAGE_RIGHT → ADC_L9369_VSBRIDGE_B
 * - ADC_CH_MOTOR_CURRENT_LEFT → ADC_L9369_CS1_A
 * - ADC_CH_MOTOR_CURRENT_RIGHT → ADC_L9369_CS1_B
 */
Adc_StatusType Bsw_Adc_ReadMotorChannel(
    Adc_ChannelIdType channelId,
    uint16 *rawValue);

/**
 * @brief Get all power supply voltages (composed operation)
 * @param[out] ign_mv IGN voltage in mV
 * @param[out] ubb_mv Motor battery voltage in mV
 * @param[out] ubvr_mv Valve battery voltage in mV
 * @return ADC_STATUS_OK or error code
 *
 * TODO: ReadAveraged + ConvertToVoltage 조합
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:104-115
 *
 * 동작:
 * 1. Bsw_Adc_ReadAveraged(ADC_CH_IGNITION_VOLTAGE, 6, &raw)
 * 2. Bsw_Adc_ConvertToVoltage(raw, &params, &ign_mv)
 * 3. UBB, UBVR 반복
 */
Adc_StatusType Bsw_Adc_GetPowerVoltages(
    Adc_VoltageType *ign_mv,
    Adc_VoltageType *ubb_mv,
    Adc_VoltageType *ubvr_mv);

/**
 * @brief Get all motor driver measurements (voltage + current)
 * @param[out] motorA_volt_mv Left motor voltage (mV)
 * @param[out] motorB_volt_mv Right motor voltage (mV)
 * @param[out] motorA_cur_ma Left motor current (mA)
 * @param[out] motorB_cur_ma Right motor current (mA)
 * @return ADC_STATUS_OK or error code
 *
 * TODO: ReadMotorChannel 조합
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:117-120
 */
Adc_StatusType Bsw_Adc_GetMotorMeasurements(
    Adc_VoltageType *motorA_volt_mv,
    Adc_VoltageType *motorB_volt_mv,
    Adc_CurrentType *motorA_cur_ma,
    Adc_CurrentType *motorB_cur_ma);

/*===========================================================================
 * BSW ADC Predefined Conversion Parameters
 *===========================================================================*/
/* TODO: 기존 프로젝트 값으로 초기화
 * 근거: EPB/Peripheral/ADC/Adc_Api.h:35-37
 *
 * ref_3v3 = 3300mV
 * r300 = 300kΩ
 * r51 = 51kΩ
 */
extern const Adc_ConversionParamsType BSW_ADC_PARAMS_POWER; /* IGN/UBB/UBVR 공용 */

#endif /* BSW_ADC_H */
