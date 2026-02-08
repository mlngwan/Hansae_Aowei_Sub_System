#include "Mcal_Adc.h"

/* 기존 레거시 ADC 함수 참조 (임시) */
extern void Mcal_Adc_Initialize(void);
extern uint16_t Mcal_Adc_ReadChannel(uint8_t adc_ch);

/* MCAL ADC 초기화 */
void Mcal_Adc_Init(void)
{
    /* 기존 레거시 초기화 함수 호출 */
    Mcal_Adc_Initialize();
}

/* ADC Raw 값 읽기 */
uint16_t Mcal_Adc_ReadRaw(uint8_t channelId)
{
    /* 기존 레거시 읽기 함수 호출 */
    return Mcal_Adc_ReadChannel(channelId);
}