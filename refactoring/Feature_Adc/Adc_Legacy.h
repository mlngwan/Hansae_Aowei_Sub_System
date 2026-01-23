/**
 * @file Adc_Legacy.h
 * @brief ADC Legacy API Compatibility Layer
 *
 * Purpose: 기존 API 호환성 유지 (호출 치환 포인트)
 * Scope: 기존 EPB/Peripheral/ADC/Adc_Api.h의 공용 함수 래핑
 * Intent: 100% Pass-through to new layered architecture
 *
 * Refactoring Strategy:
 * - 기존 함수명 그대로 유지 (호출자 코드 변경 불필요)
 * - 내부에서 새 레이어(MCAL/BSW/FS)를 호출
 * - 호출 치환 포인트: CheckAdcStatus() 호출 경로만 변경 필요
 *
 * 근거: ADC_LAYER_SPLIT_FACTS.md 섹션 3.1
 */

#ifndef ADC_LEGACY_H
#define ADC_LEGACY_H

#include "Adc_Types.h"

/*===========================================================================
 * Legacy ADC Public API (기존 함수명 유지)
 *===========================================================================*/

/**
 * @brief Initialize ADC subsystem (기존 ADCInitialize 호환)
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:9
 *
 * 라우팅:
 * - ADCInitialize() → Mcal_Adc_Init()
 */
void ADCInitialize(void);

/**
 * @brief Read raw ADC value from hardware channel (기존 GetADCdata 호환)
 * @param adc_ch Physical ADC channel number
 * @return 10-bit raw ADC value (0-1023)
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:28
 *
 * 라우팅:
 * - GetADCdata(adc_ch) → Mcal_Adc_ReadChannel(adc_ch)
 */
uword GetADCdata(uint8_t adc_ch);

/**
 * @brief Read averaged ADC value (기존 GetAdcAverage 호환)
 * @param ADC_Channel Physical channel number
 * @return Averaged raw value (max/min excluded)
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:66
 *
 * 라우팅:
 * - GetAdcAverage(ch) → Bsw_Adc_ReadAveraged()
 */
uword GetAdcAverage(ubyte ADC_Channel);

/**
 * @brief Convert ADC value to physical voltage (기존 ConvertToPhysicalBatVolt 호환)
 * @param adcvalue 10-bit raw ADC value
 * @param v_ref Reference voltage (3.3V typically)
 * @param r1 Upper resistor value (kΩ)
 * @param r2 Lower resistor value (kΩ)
 * @return Physical voltage in volts
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:87
 *
 * 라우팅:
 * - ConvertToPhysicalBatVolt(adc, vref, r1, r2) → Bsw_Adc_ConvertToVoltage()
 */
float ConvertToPhysicalBatVolt(uint16_t adcvalue, float v_ref, uint16_t r1, uint16_t r2);

/**
 * @brief Read all ADC channels and update physical values (기존 GetRawData 호환)
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:104
 *
 * 라우팅:
 * - GetRawData() → Bsw_Adc_ReadAllChannels()
 * - 결과는 전역 g_ADC 구조체에 저장
 */
void GetRawData(void);

/**
 * @brief Update logic data structures from physical values (기존 SetLogicData 호환)
 *
 * 근거: EPB/Peripheral/ADC/Adc_Api.c:124
 *
 * 라우팅:
 * - SetLogicData() → Fs_Adc_UpdateData()
 * - SSMInput, PbcInDriver 버스 업데이트
 */
void SetLogicData(void);

/**
 * @brief Main ADC processing function (상위 호출자 치환 포인트)
 * 호출 경로: TaskScheduler_5ms() → CheckAdcStatus()
 *
 * 근거: ADC_SKELETON_VERIFICATION_FACTS.md 섹션 2.2
 *
 * 기존: GetRawData() → SetLogicData() 순서 호출
 * 변경: Fs_Adc_ProcessAll() 단일 호출
 *
 * 라우팅:
 * - CheckAdcStatus() → Fs_Adc_ProcessAll()
 */
void CheckAdcStatus(void);

#endif /* ADC_LEGACY_H */
