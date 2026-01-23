/**
 * @file Mcal_Adc.c
 * @brief MCAL Layer - ADC Hardware Implementation
 *
 * Refactoring Note:
 * - 기존: EPB/Peripheral/ADC/Adc_Api.c의 GetADCdata(), ADCInitialize()
 * - 변경: 레지스터 접근만 래핑, Intent 로직은 제거
 */

#include "Mcal_Adc.h"
#include "spc560bsp.h" /* ADC register definitions */

/* External dependency - L9369 global data */
extern ST_L9369_ADC_VALUE g_L9369_Adc_Value;

/* Local timeout counter */
static uint8_t s_timeoutFlag = 0u;

/*===========================================================================
 * MCAL ADC Initialization
 *===========================================================================*/
void Mcal_Adc_Init(void)
{
  /* 기존: ADCInitialize() from Adc_Api.c:9-21 */
  ADC.MCR.R = 0x80000000; /* ADC control register reset */
  ADC.MCR.B.OWREN = 1;    /* Overwrite enable */
  ADC.MCR.B.ADCLKSEL = 1; /* Clock selection */

  /* Conversion timing registers */
  ADC.CTR[0].B.INPLATCH = 1;
  ADC.CTR[0].B.INPCMP = 0;
  ADC.CTR[0].B.INPSAMP = 0x30;

  s_timeoutFlag = 0u;
}

/*===========================================================================
 * MCAL ADC Channel Read (Direct Register Access)
 *===========================================================================*/
Mcal_Adc_StatusType Mcal_Adc_ReadChannel(uint8_t channel, uint16_t *rawValue)
{
  uint32_t timeout_cnt = 0u;

  if (rawValue == NULL)
  {
    return MCAL_ADC_INVALID_CHANNEL;
  }

  /* 기존: GetADCdata() from Adc_Api.c:28-64 */

  /* 1. Select ADC channel based on range */
  if (channel <= 31u)
  {
    ADC.NCMR[0].R = (uint32_t)(1u << channel);
    ADC.NCMR[1].R = 0x0;
    ADC.NCMR[2].R = 0x0;
  }
  else if (channel <= 39u)
  {
    ADC.NCMR[0].R = 0x0;
    ADC.NCMR[1].R = (uint32_t)(1u << (channel - 32u));
    ADC.NCMR[2].R = 0x0;
  }
  else if (channel <= 63u)
  {
    ADC.NCMR[0].R = 0x0;
    ADC.NCMR[1].R = 0x0;
    ADC.NCMR[2].R = (uint32_t)(1u << (channel - 40u));
  }
  else
  {
    return MCAL_ADC_INVALID_CHANNEL;
  }

  /* 2. Start ADC conversion */
  ADC.MCR.B.NSTART = 1;

  /* 3. Wait for conversion complete (with timeout) */
  while (ADC.MSR.B.NSTART == 1)
  {
    timeout_cnt++;
    if (timeout_cnt > 1000000u)
    {
      s_timeoutFlag = 1u;
      return MCAL_ADC_TIMEOUT;
    }
  }

  /* 4. Read conversion result */
  *rawValue = (uint16_t)ADC.CDR[channel].R;

  s_timeoutFlag = 0u;
  return MCAL_ADC_OK;
}

/*===========================================================================
 * MCAL L9369 ADC Read (Device-specific)
 *===========================================================================*/
Mcal_Adc_StatusType Mcal_Adc_ReadL9369Adc(uint8_t adcType, uint16_t *rawValue)
{
  if (rawValue == NULL)
  {
    return MCAL_ADC_INVALID_CHANNEL;
  }

  /* 기존: EpbMotorVoltage(), EpbMotorCurrent() from Spi_L9369_Api.c */
  switch (adcType)
  {
  case MCAL_L9369_ADC_VSBRIDGE_A:
    *rawValue = g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR];
    break;
  case MCAL_L9369_ADC_VSBRIDGE_B:
    *rawValue = g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR];
    break;
  case MCAL_L9369_ADC_CS1_A:
    *rawValue = g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR];
    break;
  case MCAL_L9369_ADC_CS1_B:
    *rawValue = g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR];
    break;
  default:
    return MCAL_ADC_INVALID_CHANNEL;
  }

  return MCAL_ADC_OK;
}

/*===========================================================================
 * MCAL ADC Timeout Management
 *===========================================================================*/
uint8_t Mcal_Adc_GetTimeoutFlag(void)
{
  return s_timeoutFlag;
}

void Mcal_Adc_ClearTimeoutFlag(void)
{
  s_timeoutFlag = 0u;
}
