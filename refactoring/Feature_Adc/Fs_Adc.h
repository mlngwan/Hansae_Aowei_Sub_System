/**
 * @file Fs_Adc.h
 * @brief Functional Safety Layer - ADC Wrapper (Pass-through Base)
 *
 * Purpose: BSW 레이어 래핑 (향후 Safety 기능 추가 가능)
 * Scope: 현재는 BSW 호출을 통과만 함 (Safety 검증 OFF 기본)
 * Intent: NONE - Pure pass-through wrapper over BSW
 *
 * Refactoring Note:
 * - 현재 버전: Pass-through 기본 구조 (Safety 기능 #define OFF)
 * - 상위 호출자 치환: CheckAdcStatus() → Fs_Adc_* 호출로 대체
 * - 향후 확장: Safety 기능을 활성화하려면 #define 수정
 * - Skeleton 목적: 레이어 분리 구조 명확화
 */

#ifndef FS_ADC_H
#define FS_ADC_H

#include "Adc_Types.h"
#include "Bsw_Adc.h"

/*===========================================================================
 * FS ADC Safety Features (Default: OFF - Pass-through mode)
 *===========================================================================*/
/* 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 2.2
 * 상위 호출자: CheckAdcStatus() @ EPB/Peripheral/ADC/Adc_Api.c:134
 * 호출 주기: 5ms (TaskScheduler_5ms 내)
 */

/* Safety 기능 활성화 플래그 (현재: OFF) */
#define FS_ADC_SAFETY_ENABLED 0

/* Range validation (OFF 시 수행하지 않음) */
#if FS_ADC_SAFETY_ENABLED
#define FS_ADC_RANGE_CHECK_ENABLED 1
#define FS_ADC_TIMEOUT_CHECK_ENABLED 1
#define FS_ADC_DTC_GENERATION_ENABLED 1
#else
#define FS_ADC_RANGE_CHECK_ENABLED 0    /* 범위 검증 OFF */
#define FS_ADC_TIMEOUT_CHECK_ENABLED 0  /* Timeout 검증 OFF */
#define FS_ADC_DTC_GENERATION_ENABLED 0 /* DTC 생성 OFF */
#endif

/*===========================================================================
 * FS ADC API - Wrapper Functions
 *===========================================================================*/

/**
 * @brief Initialize FS ADC layer
 * @return ADC_STATUS_OK or error code
 *
 * [Pass-through 기본 동작]
 * - Bsw_Adc_Init() 호출
 * - Safety 기능은 OFF 상태 (FS_ADC_SAFETY_ENABLED=0)
 */
Adc_StatusType Fs_Adc_Init(void);

/**
 * @brief Read power voltages (Pass-through to BSW)
 * @param[out] ign_mv IGN voltage in mV
 * @param[out] ubb_mv Motor battery voltage in mV
 * @param[out] ubvr_mv Valve battery voltage in mV
 * @return ADC_STATUS_OK or error code
 *
 * [Pass-through 기본 동작]
 * - Bsw_Adc_GetPowerVoltages() 직접 호출
 * - Safety 기능 OFF: 범위 검증 없음, Timeout 체크 없음
 * - 단순 데이터 전달만 수행
 *
 * [상위 호출자 치환 포인트]
 * 기존: CheckAdcStatus() @ EPB/Peripheral/ADC/Adc_Api.c:134
 * 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 2.2
 */
Adc_StatusType Fs_Adc_GetPowerVoltages_Safe(
    Adc_VoltageType *ign_mv,
    Adc_VoltageType *ubb_mv,
    Adc_VoltageType *ubvr_mv);

/**
 * @brief Read motor measurements (Pass-through to BSW)
 * @param[out] motorA_volt_mv Left motor voltage (mV)
 * @param[out] motorB_volt_mv Right motor voltage (mV)
 * @param[out] motorA_cur_ma Left motor current (mA)
 * @param[out] motorB_cur_ma Right motor current (mA)
 * @return ADC_STATUS_OK or error code
 *
 * [Pass-through 기본 동작]
 * - Bsw_Adc_GetMotorMeasurements() 직접 호출
 * - Safety 기능 OFF: 범위 검증 없음
 * - 단순 데이터 전달만 수행
 */
Adc_StatusType Fs_Adc_GetMotorMeasurements_Safe(
    Adc_VoltageType *motorA_volt_mv,
    Adc_VoltageType *motorB_volt_mv,
    Adc_CurrentType *motorA_cur_ma,
    Adc_CurrentType *motorB_cur_ma);

/**
 * @brief Get diagnostic information for ADC
 * @param[out] diagCounters Pointer to diagnostic counters structure
 * @return ADC_STATUS_OK or error code
 *
 * TODO: 진단 카운터 복사 구현
 */
Adc_StatusType Fs_Adc_GetDiagnostics(Adc_DiagCountersType *diagCounters);

/**
 * @brief Clear diagnostic counters (for service reset)
 *
 * TODO: 진단 카운터 0으로 초기화
 */
void Fs_Adc_ClearDiagnostics(void);

/**
 * @brief Check if ADC is in safe state
 * @return 1 if in safe state, 0 otherwise
 *
 * TODO: Safe state 플래그 반환
 */
uint8 Fs_Adc_IsInSafeState(void);

/*===========================================================================
 * FS ADC Predefined Safety Ranges
 *===========================================================================*/
/* TODO: 실제 값으로 초기화
 *
 * 전원 전압 범위 (Automotive 12V system):
 * - min: 6000 mV (6V)
 * - max: 18000 mV (18V)
 * - safe: 12000 mV (12V)
 *
 * 모터 전압 범위:
 * - min: 0 mV
 * - max: 20000 mV (20V)
 * - safe: 0 mV
 *
 * 모터 전류 범위:
 * - min: 0 mA
 * - max: 30000 mA (30A)
 * - safe: 0 mA
 */
extern const Adc_RangeConfigType FS_ADC_RANGE_POWER_VOLTAGE;
extern const Adc_RangeConfigType FS_ADC_RANGE_MOTOR_VOLTAGE;
extern const Adc_RangeConfigType FS_ADC_RANGE_MOTOR_CURRENT;

/*===========================================================================
 * FS ADC Individual Port Functions (ASW 호출용)
 *===========================================================================*/
/* 근거: ADC_FEATURE_FACT_PACKET_v2.md 섹션 3.1 */

/**
 * @brief Get battery voltage with safety check
 * @return Voltage in mV (validated)
 *
 * TODO: Fs_Adc_GetPowerVoltages_Safe() 호출 후 UBB 값만 반환
 */
Adc_VoltageType Fs_Adc_GetBatteryVoltage(void);

/**
 * @brief Get ignition voltage with safety check
 * @return Voltage in mV (validated)
 *
 * TODO: Fs_Adc_GetPowerVoltages_Safe() 호출 후 IGN 값만 반환
 */
Adc_VoltageType Fs_Adc_GetIgnitionVoltage(void);

/**
 * @brief Get valve battery voltage with safety check
 * @return Voltage in mV (validated)
 *
 * TODO: Fs_Adc_GetPowerVoltages_Safe() 호출 후 UBVR 값만 반환
 */
Adc_VoltageType Fs_Adc_GetValveBatteryVoltage(void);

/**
 * @brief Get left motor voltage with safety check
 * @return Voltage in mV (validated)
 *
 * TODO: Fs_Adc_GetMotorMeasurements_Safe() 호출 후 MotorA 전압만 반환
 */
Adc_VoltageType Fs_Adc_GetMotorVoltageLeft(void);

/**
 * @brief Get right motor voltage with safety check
 * @return Voltage in mV (validated)
 *
 * TODO: Fs_Adc_GetMotorMeasurements_Safe() 호출 후 MotorB 전압만 반환
 */
Adc_VoltageType Fs_Adc_GetMotorVoltageRight(void);

/**
 * @brief Get left motor current with safety check
 * @return Current in mA (validated)
 *
 * TODO: Fs_Adc_GetMotorMeasurements_Safe() 호출 후 MotorA 전류만 반환
 */
Adc_CurrentType Fs_Adc_GetMotorCurrentLeft(void);

/**
 * @brief Get right motor current with safety check
 * @return Current in mA (validated)
 *
 * TODO: Fs_Adc_GetMotorMeasurements_Safe() 호출 후 MotorB 전류만 반환
 */
Adc_CurrentType Fs_Adc_GetMotorCurrentRight(void);

/**
 * @brief Get ADC system status
 * @return 1 if ADC system OK, 0 if error/timeout
 *
 * TODO: Timeout flag 및 Safe state 조합하여 반환
 */
uint8 Fs_Adc_GetAdcStatus(void);

#endif /* FS_ADC_H */
