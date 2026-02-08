#ifndef CDD_L9369ADC_H
#define CDD_L9369ADC_H

#include <stdint.h>

/* CDD L9369 ADC 초기화 */
void Cdd_L9369Adc_Init(void);

/* 모터 A 전압/전류 읽기 */
void Cdd_L9369Adc_ReadMotorA(uint16_t* voltage_mV, int16_t* current_mA);

/* 모터 B 전압/전류 읽기 */
void Cdd_L9369Adc_ReadMotorB(uint16_t* voltage_mV, int16_t* current_mA);

#endif