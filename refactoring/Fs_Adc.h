/**
 * @file Fs_Adc.h
 * @brief Functional Safety Layer - ADC Safety Wrapper
 *
 * Purpose: Safety mechanisms and diagnostic checks
 * Scope: Error detection, range validation, fail-safe handling
 * Intent: NONE - Pure safety wrapper over BSW
 *
 * Safety Requirements:
 * - ADC value range validation (plausibility check)
 * - Timeout detection and reporting
 * - Diagnostic Trouble Code (DTC) generation
 * - Safe fallback values on error
 */

#ifndef FS_ADC_H
#define FS_ADC_H

#include "typedefs.h"
#include "Bsw_Adc.h"

/*===========================================================================
 * FS ADC Return Types (Safety-aware)
 *===========================================================================*/
typedef enum
{
  FS_ADC_OK = 0,
  FS_ADC_ERROR,
  FS_ADC_OUT_OF_RANGE,
  FS_ADC_TIMEOUT,
  FS_ADC_SAFE_STATE
} Fs_Adc_StatusType;

/*===========================================================================
 * FS ADC Safety Configuration
 *===========================================================================*/
typedef struct
{
  uint32_t min_valid_mv;  /* Minimum valid voltage (mV) */
  uint32_t max_valid_mv;  /* Maximum valid voltage (mV) */
  uint32_t safe_value_mv; /* Safe fallback value (mV) */
} Fs_Adc_RangeConfig;

/*===========================================================================
 * FS ADC Diagnostic Counters (Safety Monitoring)
 *===========================================================================*/
typedef struct
{
  uint16_t timeoutCount;      /* ADC timeout occurrences */
  uint16_t rangeErrorCount;   /* Out-of-range detections */
  uint16_t consecutiveErrors; /* Consecutive error count */
} Fs_Adc_DiagCounters;

/*===========================================================================
 * FS ADC API - Safety Services
 *===========================================================================*/

/**
 * @brief Initialize FS ADC layer with safety features
 * @note Calls Bsw_Adc_Init() and resets diagnostic counters
 */
void Fs_Adc_Init(void);

/**
 * @brief Read power voltages with safety checks
 * @param[out] ign_mv IGN voltage in mV (validated)
 * @param[out] ubb_mv Motor battery voltage in mV (validated)
 * @param[out] ubvr_mv Valve battery voltage in mV (validated)
 * @return FS_ADC_OK or error code with DTC
 *
 * @note Safety Features:
 *       - Range validation (6V-18V for automotive)
 *       - Timeout detection
 *       - Safe fallback values on error
 *       - Diagnostic counter update
 */
Fs_Adc_StatusType Fs_Adc_GetPowerVoltages_Safe(
    uint32_t *ign_mv,
    uint32_t *ubb_mv,
    uint32_t *ubvr_mv);

/**
 * @brief Read motor measurements with safety checks
 * @param[out] motorA_volt_mv Left motor voltage (validated)
 * @param[out] motorB_volt_mv Right motor voltage (validated)
 * @param[out] motorA_cur_ma Left motor current (validated)
 * @param[out] motorB_cur_ma Right motor current (validated)
 * @return FS_ADC_OK or error code with DTC
 *
 * @note Safety Features:
 *       - Current range validation (0-30A)
 *       - Voltage range validation (0-20V)
 *       - Over-current detection
 */
Fs_Adc_StatusType Fs_Adc_GetMotorMeasurements_Safe(
    uint32_t *motorA_volt_mv,
    uint32_t *motorB_volt_mv,
    uint32_t *motorA_cur_ma,
    uint32_t *motorB_cur_ma);

/**
 * @brief Get diagnostic information for ADC
 * @param[out] diagCounters Pointer to diagnostic counters structure
 * @return FS_ADC_OK or error code
 */
Fs_Adc_StatusType Fs_Adc_GetDiagnostics(Fs_Adc_DiagCounters *diagCounters);

/**
 * @brief Clear diagnostic counters (for service reset)
 */
void Fs_Adc_ClearDiagnostics(void);

/**
 * @brief Check if ADC is in safe state (fail-safe activated)
 * @return 1 if in safe state, 0 otherwise
 */
uint8_t Fs_Adc_IsInSafeState(void);

/**
 * @brief Validate voltage range and apply safe value if needed
 * @param[in,out] voltage_mv Voltage to validate (modified to safe value on error)
 * @param[in] config Range configuration
 * @return FS_ADC_OK if valid, FS_ADC_OUT_OF_RANGE if corrected
 *
 * @note Internal helper for range validation
 */
Fs_Adc_StatusType Fs_Adc_ValidateVoltageRange(
    uint32_t *voltage_mv,
    const Fs_Adc_RangeConfig *config);

/*===========================================================================
 * FS ADC Predefined Safety Ranges
 *===========================================================================*/
extern const Fs_Adc_RangeConfig FS_ADC_RANGE_POWER_VOLTAGE; /* 6V-18V, safe=12V */
extern const Fs_Adc_RangeConfig FS_ADC_RANGE_MOTOR_VOLTAGE; /* 0V-20V, safe=0V */
extern const Fs_Adc_RangeConfig FS_ADC_RANGE_MOTOR_CURRENT; /* 0A-30A, safe=0A */

#endif /* FS_ADC_H */
