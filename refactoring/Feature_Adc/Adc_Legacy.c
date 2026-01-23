/**
 * @file Adc_Legacy.c
 * @brief ADC Legacy API Implementation (호환성 계층)
 *
 * Purpose: 기존 API 호환성 유지 및 새 레이어로 라우팅
 * Scope: 기존 EPB/Peripheral/ADC/Adc_Api.c의 공용 함수 래핑
 * Intent: 100% Pass-through (의미 불변, 레이어만 변경)
 *
 * 근거: ADC_LAYER_SPLIT_FACTS.md 섹션 3.1, 3.2, 3.3
 */

#include "Adc_Legacy.h"
#include "Mcal_Adc.h"
#include "Bsw_Adc.h"
#include "Fs_Adc.h"

extern ADC_t g_ADC;
extern SSMInputBus SSMInput;
extern PbcInDriverBus PbcInDriver;

/*===========================================================================
 * Legacy ADC Public API Implementation
 *===========================================================================*/

/**
 * @brief ADC Initialization
 * 라우팅: ADCInitialize() → Mcal_Adc_Init()
 */
void ADCInitialize(void)
{
  Mcal_Adc_Init();
}

/**
 * @brief Read raw ADC value
 * 라우팅: GetADCdata(ch) → Mcal_Adc_ReadChannel(ch)
 * 단위 불변: 0-1023 (10-bit)
 */
uword GetADCdata(uint8_t adc_ch)
{
  Adc_RawValueType rawValue = 0;

  Mcal_Adc_ReadChannel(adc_ch, &rawValue);

  return (uword)rawValue;
}

/**
 * @brief Read averaged ADC value
 * 라우팅: GetAdcAverage(ch) → Bsw_Adc_ReadAveraged()
 * 의미 불변: 6-sample averaging with max/min exclusion
 * 단위 불변: 0-1023 (10-bit)
 */
uword GetAdcAverage(ubyte ADC_Channel)
{
  Adc_RawValueType avgValue = 0;

  Bsw_Adc_ReadAveraged(ADC_Channel, 6, &avgValue);

  return (uword)avgValue;
}

/**
 * @brief Convert ADC value to physical voltage
 * 라우팅: ConvertToPhysicalBatVolt() → Bsw_Adc_ConvertToVoltage()
 * 의미 불변: (adc/1023)*vref*((r1+r2)/r2) = volts
 * 단위 불변: output in volts (호출자가 *ADC_REF로 mV 변환)
 */
float ConvertToPhysicalBatVolt(uint16_t adcvalue, float v_ref, uint16_t r1, uint16_t r2)
{
  Adc_ConversionParamsType params;
  Adc_VoltageType voltage_mv = 0;
  float voltage_v;

  /* Set up conversion parameters */
  params.vref_mv = (uint16)v_ref;
  params.r_upper = r1;
  params.r_lower = r2;

  /* Convert to voltage in mV */
  Bsw_Adc_ConvertToVoltage(adcvalue, &params, &voltage_mv);

  /* Convert mV to V (float) for legacy compatibility */
  voltage_v = (float)voltage_mv / 1000.0f;

  return voltage_v;
}

/**
 * @brief Read all ADC channels and update g_ADC structure
 * 라우팅: GetRawData() → Bsw_Adc_GetPowerVoltages() + Bsw_Adc_GetMotorMeasurements()
 * 의미 불변: 모든 채널 읽기 + 물리값 계산 + g_ADC 구조체 업데이트
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:104-120
 */
void GetRawData(void)
{
  Adc_VoltageType ign_mv, ubb_mv, ubvr_mv;
  Adc_VoltageType motorA_volt, motorB_volt;
  Adc_CurrentType motorA_cur, motorB_cur;

  /* Read power voltages (배터리, IGN, UBVR) */
  Bsw_Adc_GetPowerVoltages(&ign_mv, &ubb_mv, &ubvr_mv);

  /* Read motor measurements (L9369 IC) */
  Bsw_Adc_GetMotorMeasurements(&motorA_volt, &motorB_volt, &motorA_cur, &motorB_cur);

  /* Update global ADC structure (의미 불변) */
  g_ADC.Physical.Power.UBB = ubb_mv;
  g_ADC.Physical.Power.IGN = ign_mv;
  g_ADC.Physical.Power.UBVR = ubvr_mv;

  g_ADC.Physical.Motor.MOTORA = motorA_volt;
  g_ADC.Physical.Motor.MOTORB = motorB_volt;
  g_ADC.Physical.Motor.MOTORA_CUR = motorA_cur;
  g_ADC.Physical.Motor.MOTORB_CUR = motorB_cur;
}

/**
 * @brief Update logic data (SSMInput, PbcInDriver)
 * 라우팅: SetLogicData() → Fs_Adc_UpdateData()
 * 의미 불변: 물리값을 상위 버스 구조체로 전파
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:124-130
 */
void SetLogicData(void)
{
  /* Route to FS layer for safety wrapper */
  Fs_Adc_UpdateData();
}

/**
 * @brief Main ADC processing (상위 호출자 치환 포인트)
 * 호출 경로: TaskScheduler_5ms() → CheckAdcStatus()
 *
 * 라우팅: CheckAdcStatus() → Fs_Adc_ProcessAll()
 * 의미 불변: ReadAllChannels() + UpdateData() 조합 수행
 *
 * 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 2.2
 * - 상위 호출자: CheckAdcStatus() @ EPB/Peripheral/ADC/Adc_Api.c:134
 * - 호출 주기: 5ms (TaskScheduler_5ms 내)
 * - 호출 경로: 변경 없음 (함수명 동일)
 */
void CheckAdcStatus(void)
{
  /* Route to FS layer for full ADC processing */
  Fs_Adc_ProcessAll();
}
