/**
 * @file Bsw_Adc.h
 * @brief BSW Layer - ADC Basic Software Services
 *
 * Purpose: Hardware-independent ADC data acquisition services
 * Scope: Channel management, multi-sample reads, physical conversion
 * Intent: Physical unit conversion (Raw → mV/mA) - PRESERVED from ASW
 */

#ifndef BSW_ADC_H
#define BSW_ADC_H

#include "typedefs.h"
#include "Mcal_Adc.h"

/*===========================================================================
 * BSW ADC Return Types
 *===========================================================================*/
typedef enum
{
  BSW_ADC_OK = 0,
  BSW_ADC_ERROR,
  BSW_ADC_TIMEOUT,
  BSW_ADC_INVALID_PARAM
} Bsw_Adc_StatusType;

/*===========================================================================
 * BSW ADC Logical Channel IDs (Hardware-independent)
 *===========================================================================*/
typedef enum
{
  BSW_ADC_CH_IGN_VOLTAGE = 0, /* IGN switch voltage */
  BSW_ADC_CH_MOTOR_BATTERY,   /* Motor battery (UBB) */
  BSW_ADC_CH_VALVE_BATTERY,   /* Valve battery (UBVR) */
  BSW_ADC_CH_MAX
} Bsw_Adc_ChannelType;

/*===========================================================================
 * BSW ADC Motor Channel IDs (L9369-specific)
 *===========================================================================*/
typedef enum
{
  BSW_ADC_MOTOR_VOLTAGE_LEFT = 0,
  BSW_ADC_MOTOR_VOLTAGE_RIGHT,
  BSW_ADC_MOTOR_CURRENT_LEFT,
  BSW_ADC_MOTOR_CURRENT_RIGHT,
  BSW_ADC_MOTOR_CH_MAX
} Bsw_Adc_MotorChannelType;

/*===========================================================================
 * BSW ADC Physical Conversion Parameters
 *===========================================================================*/
typedef struct
{
  uint16_t vref_mv; /* Reference voltage in mV (e.g., 3300) */
  uint16_t r_upper; /* Upper resistor in voltage divider */
  uint16_t r_lower; /* Lower resistor in voltage divider */
} Bsw_Adc_ConversionParams;

/*===========================================================================
 * BSW ADC API - Basic Services
 *===========================================================================*/

/**
 * @brief Initialize BSW ADC layer
 * @note Calls MCAL_Adc_Init() internally
 */
void Bsw_Adc_Init(void);

/**
 * @brief Read averaged ADC value (multi-sample filtering)
 * @param[in] channel Logical channel ID
 * @param[in] numSamples Number of samples to average (e.g., 6)
 * @param[out] avgRawValue Pointer to store averaged raw value
 * @return BSW_ADC_OK or error code
 *
 * @note Intent: GetAdcAverage() from Adc_Api.c:66-85
 *       - Takes numSamples samples
 *       - Removes max/min outliers
 *       - Returns average of remaining samples
 */
Bsw_Adc_StatusType Bsw_Adc_ReadAveraged(
    Bsw_Adc_ChannelType channel,
    uint8_t numSamples,
    uint16_t *avgRawValue);

/**
 * @brief Convert raw ADC value to physical voltage (mV)
 * @param[in] rawValue 10-bit ADC raw value (0-1023)
 * @param[in] params Conversion parameters (Vref, R divider)
 * @param[out] voltage_mv Pointer to store voltage in millivolts
 * @return BSW_ADC_OK or error code
 *
 * @note Intent: ConvertToPhysicalBatVolt() from Adc_Api.c:87-95
 *       Formula: V_real = (ADC/1023) * Vref * ((R1+R2)/R2)
 */
Bsw_Adc_StatusType Bsw_Adc_ConvertToVoltage(
    uint16_t rawValue,
    const Bsw_Adc_ConversionParams *params,
    uint32_t *voltage_mv);

/**
 * @brief Read motor ADC values from L9369 driver IC
 * @param[in] motorChannel Motor channel type (voltage/current, left/right)
 * @param[out] rawValue Pointer to store motor ADC raw value
 * @return BSW_ADC_OK or error code
 *
 * @note Intent: EpbMotorVoltage(), EpbMotorCurrent() from Spi_L9369_Api.c
 */
Bsw_Adc_StatusType Bsw_Adc_ReadMotorChannel(
    Bsw_Adc_MotorChannelType motorChannel,
    uint16_t *rawValue);

/**
 * @brief Get all power supply voltages (preprocessed)
 * @param[out] ign_mv IGN voltage in mV
 * @param[out] ubb_mv Motor battery voltage in mV
 * @param[out] ubvr_mv Valve battery voltage in mV
 * @return BSW_ADC_OK or error code
 *
 * @note Combines ReadAveraged + ConvertToVoltage for power channels
 */
Bsw_Adc_StatusType Bsw_Adc_GetPowerVoltages(
    uint32_t *ign_mv,
    uint32_t *ubb_mv,
    uint32_t *ubvr_mv);

/**
 * @brief Get all motor driver measurements (voltage + current)
 * @param[out] motorA_volt_mv Left motor voltage in mV
 * @param[out] motorB_volt_mv Right motor voltage in mV
 * @param[out] motorA_cur_ma Left motor current in mA
 * @param[out] motorB_cur_ma Right motor current in mA
 * @return BSW_ADC_OK or error code
 */
Bsw_Adc_StatusType Bsw_Adc_GetMotorMeasurements(
    uint32_t *motorA_volt_mv,
    uint32_t *motorB_volt_mv,
    uint32_t *motorA_cur_ma,
    uint32_t *motorB_cur_ma);

/*===========================================================================
 * BSW ADC Predefined Conversion Parameters
 *===========================================================================*/
extern const Bsw_Adc_ConversionParams BSW_ADC_PARAMS_IGN; /* 3.3V, 300kΩ, 51kΩ */
extern const Bsw_Adc_ConversionParams BSW_ADC_PARAMS_UBB;
extern const Bsw_Adc_ConversionParams BSW_ADC_PARAMS_UBVR;

#endif /* BSW_ADC_H */
