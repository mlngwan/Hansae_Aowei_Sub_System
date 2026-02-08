#include "Cdd_L9369Adc.h"

/* 기존 레거시 L9369 함수 참조 */
extern float Mcal_Adc_ReadL9369Motor(uint8_t motor_id, uint8_t data_type);

#define L9369_LEFT   0U
#define L9369_RIGHT  1U
#define ADC_REF      1000

/* CDD L9369 ADC 초기화 */
void Cdd_L9369Adc_Init(void)
{
    /* L9369 초기화는 기존 시스템에서 수행됨 */
    /* 필요시 추가 초기화 로직 */
}

/* 모터 A 전압/전류 읽기 */
void Cdd_L9369Adc_ReadMotorA(uint16_t* voltage_mV, int16_t* current_mA)
{
    if (voltage_mV != 0) {
        float voltage = Mcal_Adc_ReadL9369Motor(L9369_LEFT, 0);
        *voltage_mV = (uint16_t)(voltage * ADC_REF);
    }
    
    if (current_mA != 0) {
        float current = Mcal_Adc_ReadL9369Motor(L9369_LEFT, 1);
        *current_mA = (int16_t)(current * ADC_REF);
    }
}

/* 모터 B 전압/전류 읽기 */
void Cdd_L9369Adc_ReadMotorB(uint16_t* voltage_mV, int16_t* current_mA)
{
    if (voltage_mV != 0) {
        float voltage = Mcal_Adc_ReadL9369Motor(L9369_RIGHT, 0);
        *voltage_mV = (uint16_t)(voltage * ADC_REF);
    }
    
    if (current_mA != 0) {
        float current = Mcal_Adc_ReadL9369Motor(L9369_RIGHT, 1);
        *current_mA = (int16_t)(current * ADC_REF);
    }
}