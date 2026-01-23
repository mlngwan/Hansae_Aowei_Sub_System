/**
 * @file Fs_Adc.c
 * @brief Functional Safety Layer - ADC Pass-Through Implementation
 *
 * Refactoring Note:
 * - Pass-through 기본 구조: Safety 기능은 #define OFF 상태
 * - BSW 레이어를 직접 호출하는 래퍼
 * - 향후 #define 수정 시 Safety 기능 추가 가능
 * 
 * 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 2.2
 * - 상위 호출자 치환: CheckAdcStatus() @ EPB/Peripheral/ADC/Adc_Api.c:134
 * - 호출 주기: 5ms (TaskScheduler_5ms 내)
 */

#include "Fs_Adc.h"

/*===========================================================================
 * FS ADC Predefined Safety Ranges (Safety 활성화 시 사용)
 *===========================================================================*/
const Adc_RangeConfigType FS_ADC_RANGE_POWER_VOLTAGE = {
    .min_valid_mv = 6000,   /* 6V minimum for automotive */
    .max_valid_mv = 18000,  /* 18V maximum for 12V system */
    .safe_value_mv = 12000  /* 12V nominal safe value */
};

const Adc_RangeConfigType FS_ADC_RANGE_MOTOR_VOLTAGE = {
    .min_valid_mv = 0,      /* 0V minimum */
    .max_valid_mv = 20000,  /* 20V maximum */
    .safe_value_mv = 0      /* 0V safe (motor off) */
};

const Adc_RangeConfigType FS_ADC_RANGE_MOTOR_CURRENT = {
    .min_valid_mv = 0,      /* 0A minimum */
    .max_valid_mv = 30000,  /* 30A maximum */
    .safe_value_mv = 0      /* 0A safe (motor off) */
};

/*===========================================================================
 * FS ADC Initialization (Pass-through)
 *===========================================================================*/
Adc_StatusType Fs_Adc_Init(void)
{
  /* [Pass-through 기본 동작] */
  /* Safety OFF (FS_ADC_SAFETY_ENABLED=0): BSW 초기화만 수행 */
  return Bsw_Adc_Init();
}

/*===========================================================================
 * FS ADC Power Voltages (Pass-through to BSW)
 *===========================================================================*/
Adc_StatusType Fs_Adc_GetPowerVoltages_Safe(
    Adc_VoltageType *ign_mv,
    Adc_VoltageType *ubb_mv,
    Adc_VoltageType *ubvr_mv)
{
  /* [Pass-through 기본 동작] */
  /* 
   * Safety OFF: 단순 BSW 호출만 수행
   * - 범위 검증 없음
   * - Timeout 체크 없음
   * - DTC 생성 없음
   * 
   * 향후 Safety 활성화 시 추가될 기능:
   * #if FS_ADC_SAFETY_ENABLED
   *   - Timeout 감지 (Mcal_Adc_GetTimeoutFlag())
   *   - Range validation (6V-18V)
   *   - DTC 생성
   * #endif
   */
  
  if (ign_mv == NULL || ubb_mv == NULL || ubvr_mv == NULL) {
    return ADC_STATUS_INVALID_PARAM;
  }

  return Bsw_Adc_GetPowerVoltages(ign_mv, ubb_mv, ubvr_mv);
}

/*===========================================================================
 * FS ADC Motor Measurements (Pass-through to BSW)
 *===========================================================================*/
Adc_StatusType Fs_Adc_GetMotorMeasurements_Safe(
    Adc_VoltageType *motorA_volt_mv,
    Adc_VoltageType *motorB_volt_mv,
    Adc_CurrentType *motorA_cur_ma,
    Adc_CurrentType *motorB_cur_ma)
{
  /* [Pass-through 기본 동작] */
  /* 
   * Safety OFF: 단순 BSW 호출만 수행
   * - 전압 범위 검증 없음 (0V-20V)
   * - 전류 범위 검증 없음 (0A-30A)
   * - DTC 생성 없음
   */
  
  if (motorA_volt_mv == NULL || motorB_volt_mv == NULL ||
      motorA_cur_ma == NULL || motorB_cur_ma == NULL) {
    return ADC_STATUS_INVALID_PARAM;
  }

  return Bsw_Adc_GetMotorMeasurements(
      motorA_volt_mv, motorB_volt_mv,
      motorA_cur_ma, motorB_cur_ma);
}

/*===========================================================================
 * FS ADC Diagnostics API (Pass-through / Stub)
 *===========================================================================*/
Adc_StatusType Fs_Adc_GetDiagnostics(Adc_DiagCountersType *diagCounters)
{
  /* [Pass-through 기본 동작] */
  /* Safety OFF: 진단 정보 없음 (또는 0으로 초기화) */
  
  if (diagCounters == NULL) {
    return ADC_STATUS_INVALID_PARAM;
  }

#if FS_ADC_SAFETY_ENABLED
  /* Safety 활성화 시 진단 카운터 반환 */
  /* TODO: 실제 진단 정보 제공 */
#else
  /* Safety OFF: 진단 정보 0으로 초기화 */
  diagCounters->timeoutCount = 0;
  diagCounters->rangeErrorCount = 0;
  diagCounters->consecutiveErrors = 0;
#endif

  return ADC_STATUS_OK;
}

void Fs_Adc_ClearDiagnostics(void)
{
  /* [Pass-through 기본 동작] */
  /* Safety OFF: 아무 동작 없음 */
#if FS_ADC_SAFETY_ENABLED
  /* Safety 활성화 시 진단 카운터 초기화 */
  /* TODO: 진단 카운터 초기화 */
#endif
}

uint8 Fs_Adc_IsInSafeState(void)
{
  /* [Pass-through 기본 동작] */
  /* Safety OFF: 항상 정상 상태 반환 */
#if FS_ADC_SAFETY_ENABLED
  /* Safety 활성화 시 Safe state 플래그 반환 */
  /* TODO: Safe state 체크 */
  return 0;  /* Placeholder */
#else
  return 0;  /* 정상 상태 (Safe state 미진입) */
#endif
}

/*===========================================================================
 * FS ADC Individual Port Functions (Pass-through)
 *===========================================================================*/
/* 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 2.2 */

Adc_VoltageType Fs_Adc_GetBatteryVoltage(void)
{
  Adc_VoltageType ign, ubb, ubvr;
  Fs_Adc_GetPowerVoltages_Safe(&ign, &ubb, &ubvr);
  return ubb;
}

Adc_VoltageType Fs_Adc_GetIgnitionVoltage(void)
{
  Adc_VoltageType ign, ubb, ubvr;
  Fs_Adc_GetPowerVoltages_Safe(&ign, &ubb, &ubvr);
  return ign;
}

Adc_VoltageType Fs_Adc_GetValveBatteryVoltage(void)
{
  Adc_VoltageType ign, ubb, ubvr;
  Fs_Adc_GetPowerVoltages_Safe(&ign, &ubb, &ubvr);
  return ubvr;
}

Adc_VoltageType Fs_Adc_GetMotorVoltageLeft(void)
{
  Adc_VoltageType voltA, voltB;
  Adc_CurrentType curA, curB;
  Fs_Adc_GetMotorMeasurements_Safe(&voltA, &voltB, &curA, &curB);
  return voltA;
}

Adc_VoltageType Fs_Adc_GetMotorVoltageRight(void)
{
  Adc_VoltageType voltA, voltB;
  Adc_CurrentType curA, curB;
  Fs_Adc_GetMotorMeasurements_Safe(&voltA, &voltB, &curA, &curB);
  return voltB;
}

Adc_CurrentType Fs_Adc_GetMotorCurrentLeft(void)
{
  Adc_VoltageType voltA, voltB;
  Adc_CurrentType curA, curB;
  Fs_Adc_GetMotorMeasurements_Safe(&voltA, &voltB, &curA, &curB);
  return curA;
}

Adc_CurrentType Fs_Adc_GetMotorCurrentRight(void)
{
  Adc_VoltageType voltA, voltB;
  Adc_CurrentType curA, curB;
  Fs_Adc_GetMotorMeasurements_Safe(&voltA, &voltB, &curA, &curB);
  return curB;
}

uint8 Fs_Adc_GetAdcStatus(void)
{
  /* [Pass-through 기본 동작] */
  /* Safety OFF: 항상 OK 반환 */
#if FS_ADC_SAFETY_ENABLED
  /* Safety 활성화 시 Timeout/Safe state 체크 */
  /* TODO: 상태 체크 */
  return 1;
#else
  return 1;  /* 정상 상태 */
#endif
}
