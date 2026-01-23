/**
 * @file Bsw_Adc.c
 * @brief BSW Layer - ADC Basic Software Implementation
 *
 * Refactoring Note:
 * - 기존: GetAdcAverage(), ConvertToPhysicalBatVolt() from Adc_Api.c
 * - 변경: Intent 로직 보존, MCAL 호출로 Wiring 분리
 */

#include "Bsw_Adc.h"
#include "Mcal_Adc.h"

/*===========================================================================
 * BSW ADC Channel Mapping (Logical → Physical)
 *===========================================================================*/
static const uint8_t s_channelMap[BSW_ADC_CH_MAX] = {
    MCAL_ADC_CH_SUB_IGN_MON, /* BSW_ADC_CH_IGN_VOLTAGE */
    MCAL_ADC_CH_SUB_UBB_MON, /* BSW_ADC_CH_MOTOR_BATTERY */
    MCAL_ADC_CH_SUB_UBVR_MON /* BSW_ADC_CH_VALVE_BATTERY */
};

/*===========================================================================
 * BSW ADC Conversion Parameters (Predefined)
 *===========================================================================*/
const Bsw_Adc_ConversionParams BSW_ADC_PARAMS_IGN = {
    .vref_mv = 3300, /* ref_3v3 */
    .r_upper = 300,  /* r300 */
    .r_lower = 51    /* r51 */
};

const Bsw_Adc_ConversionParams BSW_ADC_PARAMS_UBB = {
    .vref_mv = 3300,
    .r_upper = 300,
    .r_lower = 51};

const Bsw_Adc_ConversionParams BSW_ADC_PARAMS_UBVR = {
    .vref_mv = 3300,
    .r_upper = 300,
    .r_lower = 51};

/*===========================================================================
 * BSW ADC Initialization
 *===========================================================================*/
void Bsw_Adc_Init(void)
{
  Mcal_Adc_Init();
}

/*===========================================================================
 * BSW ADC Averaged Read (Intent: Filtering Algorithm)
 *===========================================================================*/
Bsw_Adc_StatusType Bsw_Adc_ReadAveraged(
    Bsw_Adc_ChannelType channel,
    uint8_t numSamples,
    uint16_t *avgRawValue)
{
  uint16_t samples[10]; /* Max 10 samples */
  uint16_t max_val = 0;
  uint16_t min_val = 0xFFFF;
  uint32_t sum = 0;
  uint8_t i;
  uint8_t validSamples;
  Mcal_Adc_StatusType mcalStatus;

  if (avgRawValue == NULL || numSamples == 0 || numSamples > 10)
  {
    return BSW_ADC_INVALID_PARAM;
  }

  if (channel >= BSW_ADC_CH_MAX)
  {
    return BSW_ADC_INVALID_PARAM;
  }

  /* 기존: GetAdcAverage() from Adc_Api.c:66-85 */

  /* 1. Collect multiple samples */
  for (i = 0; i < numSamples; i++)
  {
    mcalStatus = Mcal_Adc_ReadChannel(s_channelMap[channel], &samples[i]);
    if (mcalStatus != MCAL_ADC_OK)
    {
      return BSW_ADC_ERROR;
    }
  }

  /* 2. Find max and min values */
  for (i = 0; i < numSamples; i++)
  {
    if (samples[i] > max_val)
    {
      max_val = samples[i];
    }
    if (samples[i] < min_val)
    {
      min_val = samples[i];
    }
  }

  /* 3. Sum all samples except max/min (if enough samples) */
  validSamples = numSamples;
  for (i = 0; i < numSamples; i++)
  {
    if ((numSamples > 2) && (samples[i] == max_val || samples[i] == min_val))
    {
      validSamples--;
      continue; /* Skip outliers */
    }
    sum += samples[i];
  }

  /* 4. Calculate average */
  if (validSamples > 0)
  {
    *avgRawValue = (uint16_t)(sum / validSamples);
  }
  else
  {
    *avgRawValue = 0;
  }

  return BSW_ADC_OK;
}

/*===========================================================================
 * BSW ADC Physical Conversion (Intent: Voltage Scaling)
 *===========================================================================*/
Bsw_Adc_StatusType Bsw_Adc_ConvertToVoltage(
    uint16_t rawValue,
    const Bsw_Adc_ConversionParams *params,
    uint32_t *voltage_mv)
{
  uint32_t temp;

  if (voltage_mv == NULL || params == NULL)
  {
    return BSW_ADC_INVALID_PARAM;
  }

  /* 기존: ConvertToPhysicalBatVolt() from Adc_Api.c:87-95 */
  /* Formula: V_real = (ADC / ADC_REF) * Vref * ((R1 + R2) / R2) */

  temp = (uint32_t)rawValue * params->vref_mv;
  temp = temp / 1023u; /* ADC_REF = 1023 (10-bit) */
  temp = temp * (params->r_upper + params->r_lower);
  temp = temp / params->r_lower;

  *voltage_mv = temp;

  return BSW_ADC_OK;
}

/*===========================================================================
 * BSW ADC Motor Channel Read (L9369-specific)
 *===========================================================================*/
Bsw_Adc_StatusType Bsw_Adc_ReadMotorChannel(
    Bsw_Adc_MotorChannelType motorChannel,
    uint16_t *rawValue)
{
  uint8_t mcalMotorType;
  Mcal_Adc_StatusType mcalStatus;

  if (rawValue == NULL)
  {
    return BSW_ADC_INVALID_PARAM;
  }

  /* Map BSW motor channel to MCAL L9369 ADC type */
  switch (motorChannel)
  {
  case BSW_ADC_MOTOR_VOLTAGE_LEFT:
    mcalMotorType = MCAL_L9369_ADC_VSBRIDGE_A;
    break;
  case BSW_ADC_MOTOR_VOLTAGE_RIGHT:
    mcalMotorType = MCAL_L9369_ADC_VSBRIDGE_B;
    break;
  case BSW_ADC_MOTOR_CURRENT_LEFT:
    mcalMotorType = MCAL_L9369_ADC_CS1_A;
    break;
  case BSW_ADC_MOTOR_CURRENT_RIGHT:
    mcalMotorType = MCAL_L9369_ADC_CS1_B;
    break;
  default:
    return BSW_ADC_INVALID_PARAM;
  }

  mcalStatus = Mcal_Adc_ReadL9369Adc(mcalMotorType, rawValue);

  return (mcalStatus == MCAL_ADC_OK) ? BSW_ADC_OK : BSW_ADC_ERROR;
}

/*===========================================================================
 * BSW ADC High-Level Services (Composed Operations)
 *===========================================================================*/

Bsw_Adc_StatusType Bsw_Adc_GetPowerVoltages(
    uint32_t *ign_mv,
    uint32_t *ubb_mv,
    uint32_t *ubvr_mv)
{
  uint16_t rawValue;
  Bsw_Adc_StatusType status;

  if (ign_mv == NULL || ubb_mv == NULL || ubvr_mv == NULL)
  {
    return BSW_ADC_INVALID_PARAM;
  }

  /* Read and convert IGN voltage */
  status = Bsw_Adc_ReadAveraged(BSW_ADC_CH_IGN_VOLTAGE, 6, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  status = Bsw_Adc_ConvertToVoltage(rawValue, &BSW_ADC_PARAMS_IGN, ign_mv);
  if (status != BSW_ADC_OK)
    return status;

  /* Read and convert UBB voltage */
  status = Bsw_Adc_ReadAveraged(BSW_ADC_CH_MOTOR_BATTERY, 6, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  status = Bsw_Adc_ConvertToVoltage(rawValue, &BSW_ADC_PARAMS_UBB, ubb_mv);
  if (status != BSW_ADC_OK)
    return status;

  /* Read and convert UBVR voltage */
  status = Bsw_Adc_ReadAveraged(BSW_ADC_CH_VALVE_BATTERY, 6, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  status = Bsw_Adc_ConvertToVoltage(rawValue, &BSW_ADC_PARAMS_UBVR, ubvr_mv);
  if (status != BSW_ADC_OK)
    return status;

  return BSW_ADC_OK;
}

Bsw_Adc_StatusType Bsw_Adc_GetMotorMeasurements(
    uint32_t *motorA_volt_mv,
    uint32_t *motorB_volt_mv,
    uint32_t *motorA_cur_ma,
    uint32_t *motorB_cur_ma)
{
  uint16_t rawValue;
  Bsw_Adc_StatusType status;

  if (motorA_volt_mv == NULL || motorB_volt_mv == NULL ||
      motorA_cur_ma == NULL || motorB_cur_ma == NULL)
  {
    return BSW_ADC_INVALID_PARAM;
  }

  /* Read motor voltages (already in physical units from L9369) */
  status = Bsw_Adc_ReadMotorChannel(BSW_ADC_MOTOR_VOLTAGE_LEFT, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  *motorA_volt_mv = rawValue; /* L9369 provides physical value */

  status = Bsw_Adc_ReadMotorChannel(BSW_ADC_MOTOR_VOLTAGE_RIGHT, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  *motorB_volt_mv = rawValue;

  /* Read motor currents */
  status = Bsw_Adc_ReadMotorChannel(BSW_ADC_MOTOR_CURRENT_LEFT, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  *motorA_cur_ma = rawValue;

  status = Bsw_Adc_ReadMotorChannel(BSW_ADC_MOTOR_CURRENT_RIGHT, &rawValue);
  if (status != BSW_ADC_OK)
    return status;
  *motorB_cur_ma = rawValue;

  return BSW_ADC_OK;
}
