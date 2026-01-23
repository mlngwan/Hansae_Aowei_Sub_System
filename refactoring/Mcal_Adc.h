/**
 * @file Mcal_Adc.h
 * @brief MCAL Layer - ADC Hardware Abstraction (Wiring Only)
 *
 * Purpose: Direct hardware register access and device control
 * Scope: Minimal wrapper over ADC register operations
 * Intent: NONE - Pure hardware access only
 */

#ifndef MCAL_ADC_H
#define MCAL_ADC_H

#include "typedefs.h"

/*===========================================================================
 * MCAL ADC Return Types
 *===========================================================================*/
typedef enum
{
  MCAL_ADC_OK = 0,
  MCAL_ADC_TIMEOUT,
  MCAL_ADC_INVALID_CHANNEL,
  MCAL_ADC_HW_ERROR
} Mcal_Adc_StatusType;

/*===========================================================================
 * MCAL ADC Channel Definitions (직접 하드웨어 매핑)
 *===========================================================================*/
#define MCAL_ADC_CH_SUB_IGN_MON 32u /* IGN switch voltage */
#define MCAL_ADC_CH_SUB_UBVR_MON 9u /* Valve battery voltage */
#define MCAL_ADC_CH_SUB_UBB_MON 10u /* Motor battery voltage */

/*===========================================================================
 * MCAL ADC API - Hardware Operations
 *===========================================================================*/

/**
 * @brief Initialize ADC hardware registers
 * @note 레지스터 직접 초기화 (ADC.MCR, CTR, NCMR)
 */
void Mcal_Adc_Init(void);

/**
 * @brief Read raw ADC value from single channel
 * @param[in] channel Hardware channel number (0-63)
 * @param[out] rawValue Pointer to store 10-bit raw value (0-1023)
 * @return MCAL_ADC_OK or error code
 *
 * @note Direct register access: ADC.NCMR, ADC.MCR.NSTART, ADC.CDR
 * @note Blocking call with timeout protection
 */
Mcal_Adc_StatusType Mcal_Adc_ReadChannel(uint8_t channel, uint16_t *rawValue);

/**
 * @brief Read L9369 motor driver internal ADC value
 * @param[in] adcType L9369 ADC type (VSBRIDGE_A/B, CS1_A/B)
 * @param[out] rawValue Pointer to store L9369 ADC value
 * @return MCAL_ADC_OK or error code
 *
 * @note Access g_L9369_Adc_Value global structure
 */
Mcal_Adc_StatusType Mcal_Adc_ReadL9369Adc(uint8_t adcType, uint16_t *rawValue);

/**
 * @brief Get ADC conversion timeout flag
 * @return 1 if timeout occurred, 0 otherwise
 *
 * @note Access g_ADC.Flag.Timeout
 */
uint8_t Mcal_Adc_GetTimeoutFlag(void);

/**
 * @brief Clear ADC conversion timeout flag
 */
void Mcal_Adc_ClearTimeoutFlag(void);

/*===========================================================================
 * L9369 ADC Type Definitions
 *===========================================================================*/
#define MCAL_L9369_ADC_VSBRIDGE_A 0u
#define MCAL_L9369_ADC_VSBRIDGE_B 1u
#define MCAL_L9369_ADC_CS1_A 2u
#define MCAL_L9369_ADC_CS1_B 3u

#endif /* MCAL_ADC_H */
