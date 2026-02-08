#ifndef MCAL_ADC_H
#define MCAL_ADC_H

#include <stdint.h>

/* ADC 채널 ID 정의 */
#define MCAL_ADC_CH_IGN         32U
#define MCAL_ADC_CH_UBVR        9U
#define MCAL_ADC_CH_UBB         10U

/* MCAL ADC 초기화 */
void Mcal_Adc_Init(void);

/* ADC Raw 값 읽기 (10-bit, 0-1023) */
uint16_t Mcal_Adc_ReadRaw(uint8_t channelId);

#endif