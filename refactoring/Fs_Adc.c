/**
 * @file Fs_Adc.c
 * @brief Functional Safety Layer - ADC Safety Implementation
 *
 * Refactoring Note:
 * - 기존: ASW에서 직접 g_ADC.Flag.Timeout 체크
 * - 변경: FS 레이어에서 통합 safety monitoring 제공
 */

#include "Fs_Adc.h"
#include "Bsw_Adc.h"
#include "Mcal_Adc.h"

/*===========================================================================
 * FS ADC Safety Range Configurations (Predefined)
 *===========================================================================*/
const Fs_Adc_RangeConfig FS_ADC_RANGE_POWER_VOLTAGE = {
    .min_valid_mv = 6000,  /* 6V minimum for automotive */
    .max_valid_mv = 18000, /* 18V maximum for 12V system */
    .safe_value_mv = 12000 /* 12V nominal safe value */
};

const Fs_Adc_RangeConfig FS_ADC_RANGE_MOTOR_VOLTAGE = {
    .min_valid_mv = 0,
    .max_valid_mv = 20000, /* 20V maximum motor voltage */
    .safe_value_mv = 0     /* 0V safe (motor off) */
};

const Fs_Adc_RangeConfig FS_ADC_RANGE_MOTOR_CURRENT = {
    .min_valid_mv = 0,     /* Using mV field for mA (0-30000mA) */
    .max_valid_mv = 30000, /* 30A maximum current */
    .safe_value_mv = 0     /* 0A safe (motor off) */
};

/*===========================================================================
 * FS ADC Local Variables (Safety State)
 *===========================================================================*/
static Fs_Adc_DiagCounters s_diagCounters = {0, 0, 0};
static uint8_t s_safeStateActive = 0u;

/* Error thresholds for safety state activation */
#define FS_ADC_ERROR_THRESHOLD_CONSECUTIVE 3u
#define FS_ADC_ERROR_THRESHOLD_TOTAL 10u

/*===========================================================================
 * FS ADC Initialization
 *===========================================================================*/
void Fs_Adc_Init(void)
{
  Bsw_Adc_Init();

  /* Reset safety state */
  s_diagCounters.timeoutCount = 0;
  s_diagCounters.rangeErrorCount = 0;
  s_diagCounters.consecutiveErrors = 0;
  s_safeStateActive = 0u;
}

/*===========================================================================
 * FS ADC Range Validation (Internal Helper)
 *===========================================================================*/
Fs_Adc_StatusType Fs_Adc_ValidateVoltageRange(
    uint32_t *voltage_mv,
    const Fs_Adc_RangeConfig *config)
{
  if (voltage_mv == NULL || config == NULL)
  {
    return FS_ADC_ERROR;
  }

  /* Check if value is within valid range */
  if (*voltage_mv < config->min_valid_mv || *voltage_mv > config->max_valid_mv)
  {
    /* Apply safe fallback value */
    *voltage_mv = config->safe_value_mv;

    /* Update diagnostic counters */
    s_diagCounters.rangeErrorCount++;
    s_diagCounters.consecutiveErrors++;

    return FS_ADC_OUT_OF_RANGE;
  }

  /* Valid value - reset consecutive error counter */
  s_diagCounters.consecutiveErrors = 0;

  return FS_ADC_OK;
}

/*===========================================================================
 * FS ADC Safety State Management
 *===========================================================================*/
static void Fs_Adc_UpdateSafetyState(void)
{
  /* Activate safe state if error thresholds exceeded */
  if (s_diagCounters.consecutiveErrors >= FS_ADC_ERROR_THRESHOLD_CONSECUTIVE ||
      s_diagCounters.timeoutCount >= FS_ADC_ERROR_THRESHOLD_TOTAL)
  {
    s_safeStateActive = 1u;
  }
}

/*===========================================================================
 * FS ADC Power Voltages (Safety-wrapped)
 *===========================================================================*/
Fs_Adc_StatusType Fs_Adc_GetPowerVoltages_Safe(
    uint32_t *ign_mv,
    uint32_t *ubb_mv,
    uint32_t *ubvr_mv)
{
  Bsw_Adc_StatusType bswStatus;
  Fs_Adc_StatusType fsStatus = FS_ADC_OK;
  uint8_t hasError = 0;

  if (ign_mv == NULL || ubb_mv == NULL || ubvr_mv == NULL)
  {
    return FS_ADC_ERROR;
  }

  /* 1. Call BSW layer to get raw measurements */
  bswStatus = Bsw_Adc_GetPowerVoltages(ign_mv, ubb_mv, ubvr_mv);

  /* 2. Check for timeout errors */
  if (Mcal_Adc_GetTimeoutFlag())
  {
    s_diagCounters.timeoutCount++;
    s_diagCounters.consecutiveErrors++;
    hasError = 1;
    fsStatus = FS_ADC_TIMEOUT;

    /* Apply safe values on timeout */
    *ign_mv = FS_ADC_RANGE_POWER_VOLTAGE.safe_value_mv;
    *ubb_mv = FS_ADC_RANGE_POWER_VOLTAGE.safe_value_mv;
    *ubvr_mv = FS_ADC_RANGE_POWER_VOLTAGE.safe_value_mv;
  }

  /* 3. Validate ranges even if BSW call succeeded */
  if (bswStatus == BSW_ADC_OK && !hasError)
  {
    if (Fs_Adc_ValidateVoltageRange(ign_mv, &FS_ADC_RANGE_POWER_VOLTAGE) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }
    if (Fs_Adc_ValidateVoltageRange(ubb_mv, &FS_ADC_RANGE_POWER_VOLTAGE) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }
    if (Fs_Adc_ValidateVoltageRange(ubvr_mv, &FS_ADC_RANGE_POWER_VOLTAGE) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }
  }

  /* 4. Update safety state */
  Fs_Adc_UpdateSafetyState();

  /* 5. Return safe state status if activated */
  if (s_safeStateActive)
  {
    return FS_ADC_SAFE_STATE;
  }

  return (bswStatus == BSW_ADC_OK && !hasError) ? FS_ADC_OK : fsStatus;
}

/*===========================================================================
 * FS ADC Motor Measurements (Safety-wrapped)
 *===========================================================================*/
Fs_Adc_StatusType Fs_Adc_GetMotorMeasurements_Safe(
    uint32_t *motorA_volt_mv,
    uint32_t *motorB_volt_mv,
    uint32_t *motorA_cur_ma,
    uint32_t *motorB_cur_ma)
{
  Bsw_Adc_StatusType bswStatus;
  Fs_Adc_StatusType fsStatus = FS_ADC_OK;
  uint8_t hasError = 0;

  if (motorA_volt_mv == NULL || motorB_volt_mv == NULL ||
      motorA_cur_ma == NULL || motorB_cur_ma == NULL)
  {
    return FS_ADC_ERROR;
  }

  /* 1. Call BSW layer to get motor measurements */
  bswStatus = Bsw_Adc_GetMotorMeasurements(
      motorA_volt_mv, motorB_volt_mv,
      motorA_cur_ma, motorB_cur_ma);

  /* 2. Validate voltage ranges */
  if (bswStatus == BSW_ADC_OK)
  {
    if (Fs_Adc_ValidateVoltageRange(motorA_volt_mv, &FS_ADC_RANGE_MOTOR_VOLTAGE) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }
    if (Fs_Adc_ValidateVoltageRange(motorB_volt_mv, &FS_ADC_RANGE_MOTOR_VOLTAGE) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }

    /* 3. Validate current ranges (reuse voltage validation logic) */
    if (Fs_Adc_ValidateVoltageRange(motorA_cur_ma, &FS_ADC_RANGE_MOTOR_CURRENT) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }
    if (Fs_Adc_ValidateVoltageRange(motorB_cur_ma, &FS_ADC_RANGE_MOTOR_CURRENT) != FS_ADC_OK)
    {
      fsStatus = FS_ADC_OUT_OF_RANGE;
      hasError = 1;
    }
  }
  else
  {
    /* BSW error - apply safe values */
    *motorA_volt_mv = FS_ADC_RANGE_MOTOR_VOLTAGE.safe_value_mv;
    *motorB_volt_mv = FS_ADC_RANGE_MOTOR_VOLTAGE.safe_value_mv;
    *motorA_cur_ma = FS_ADC_RANGE_MOTOR_CURRENT.safe_value_mv;
    *motorB_cur_ma = FS_ADC_RANGE_MOTOR_CURRENT.safe_value_mv;
    fsStatus = FS_ADC_ERROR;
  }

  /* 4. Update safety state */
  Fs_Adc_UpdateSafetyState();

  /* 5. Return safe state status if activated */
  if (s_safeStateActive)
  {
    return FS_ADC_SAFE_STATE;
  }

  return (bswStatus == BSW_ADC_OK && !hasError) ? FS_ADC_OK : fsStatus;
}

/*===========================================================================
 * FS ADC Diagnostics API
 *===========================================================================*/
Fs_Adc_StatusType Fs_Adc_GetDiagnostics(Fs_Adc_DiagCounters *diagCounters)
{
  if (diagCounters == NULL)
  {
    return FS_ADC_ERROR;
  }

  *diagCounters = s_diagCounters;
  return FS_ADC_OK;
}

void Fs_Adc_ClearDiagnostics(void)
{
  s_diagCounters.timeoutCount = 0;
  s_diagCounters.rangeErrorCount = 0;
  s_diagCounters.consecutiveErrors = 0;
  s_safeStateActive = 0u;

  Mcal_Adc_ClearTimeoutFlag();
}

uint8_t Fs_Adc_IsInSafeState(void)
{
  return s_safeStateActive;
}
