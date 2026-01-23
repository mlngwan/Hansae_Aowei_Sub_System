/**
 * @file Mcal_Adc.c
 * @brief MCAL Layer - ADC Hardware Abstraction Implementation
 *
 * Purpose: Direct hardware register access (Wiring Only - No Intent Logic)
 * Scope: ADC 레지스터 직접 접근, L9369 글로벌 변수 접근
 * Intent: NONE - Pure Wiring/Hardware Abstraction
 *
 * Refactoring Note:
 * - 기존 EPB/Peripheral/ADC/Adc_Api.c의 HW 접근 코드 이동
 * - 모든 하드웨어 의존성을 이 레이어로 격리
 * - 상위 레이어(BSW/FS)는 이 인터페이스만 호출
 *
 * 근거: ADC_LAYER_SPLIT_FACTS.md 섹션 1.1, 2.1
 */

#include "Mcal_Adc.h"

extern ADC_t g_ADC;                          /* g_ADC.Flag.Timeout 접근용 */
extern ST_L9369_ADC_VALUE g_L9369_Adc_Value; /* L9369 ADC 데이터 */

/*===========================================================================
 * MCAL ADC Initialization
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:9-21
 *===========================================================================*/
Adc_StatusType Mcal_Adc_Init(void)
{
  /* HW 레지스터 초기화 (기존 ADCInitialize() 코드 이동) */
  ADC.MCR.R = 0x00000000; /* reset register and get out of power-down mode */
  ADC.MCR.B.OWREN = 1;    /* overwrite enable */
  ADC.MCR.B.ADCLKSEL = 1; /* ADC clock selection */

  ADC.CTR[0].B.INPLATCH = 1; /* set to 1 when INPCMP>=2 */
  ADC.CTR[0].B.INPCMP = 2;   /* input pre-charge time */
  ADC.CTR[0].B.INPSAMP = 17; /* input sampling time */

  /* Timing calculations (at fADC=48MHz/2=24MHz):
   * Teval = 10*INPCMP*Tck = 20Tck = 833ns
   * Tsamp = INPSAMP*Tck = 17Tck = 708ns
   * Tconv = 624ns
   */

  return ADC_STATUS_OK;
}

/*===========================================================================
 * MCAL ADC Channel Read (Blocking)
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:28-64
 *===========================================================================*/
Adc_StatusType Mcal_Adc_ReadChannel(Adc_HwChannelType hwChannel, Adc_RawValueType *rawValue)
{
  uint8_t shift_ch = 0;
  vuint32_t *adc_reg = (void *)0; /* Null Pointer initialize */
  uint32_t timeout_cnt = 0;

  if (rawValue == NULL)
  {
    return ADC_STATUS_INVALID_PARAM;
  }

  /* Channel select based on channel number */
  if (hwChannel < 32)
  {
    shift_ch = 0;
    adc_reg = &ADC.NCMR[0].R;
  }
  else if (hwChannel < 64)
  {
    shift_ch = 32;
    adc_reg = &ADC.NCMR[1].R;
  }
  else
  {
    shift_ch = 64;
    adc_reg = &ADC.NCMR[2].R;
  }

  /* Enable channel for normal conversion */
  *adc_reg = (uint32_t)0x00000001 << (hwChannel - shift_ch);

  /* Start normal conversion */
  ADC.MCR.B.NSTART = 1;
  g_ADC.Flag.Timeout = 0;

  /* Wait until conversion is finished (with timeout protection) */
  timeout_cnt = 0;
  while (ADC.MSR.B.NSTART != 0)
  {
    /* timeout check */
    if (g_ADC.Flag.Timeout || (timeout_cnt++ > 1000000))
    {
      return ADC_STATUS_TIMEOUT;
    }
  }

  /* Read ADC result from data register (10-bit masked) */
  *rawValue = (uint16)((uint32_t)ADC.CDR[hwChannel].R & ADC_RAW_MASK);

  return ADC_STATUS_OK;
}

/*===========================================================================
 * MCAL L9369 ADC Read
 * 근거: EPB/Application/MotorControl/Spi_L9369_Api.c:1701, 1683
 *===========================================================================*/
Adc_StatusType Mcal_Adc_ReadL9369Adc(Adc_L9369AdcType l9369AdcType, uint16 *rawValue)
{
  if (rawValue == NULL)
  {
    return ADC_STATUS_INVALID_PARAM;
  }

  /* L9369 IC 내부 ADC 값 읽기 (글로벌 변수 접근) */
  switch (l9369AdcType)
  {
  case ADC_L9369_VSBRIDGE_A:
    *rawValue = (uint16)g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR];
    break;

  case ADC_L9369_VSBRIDGE_B:
    *rawValue = (uint16)g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR];
    break;

  case ADC_L9369_CS1_A:
    *rawValue = (uint16)g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR];
    break;

  case ADC_L9369_CS1_B:
    *rawValue = (uint16)g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR];
    break;

  default:
    return ADC_STATUS_INVALID_PARAM;
  }

  return ADC_STATUS_OK;
}

/*===========================================================================
 * MCAL ADC Timeout Flag Management
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:54
 *===========================================================================*/
uint8 Mcal_Adc_GetTimeoutFlag(void)
{
  return g_ADC.Flag.Timeout;
}

void Mcal_Adc_ClearTimeoutFlag(void)
{
  g_ADC.Flag.Timeout = 0;
}
