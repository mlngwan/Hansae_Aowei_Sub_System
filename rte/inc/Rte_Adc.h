#ifndef RTE_ADC_H
#define RTE_ADC_H

#include "Rte_Type.h"

/* RTE Internal Functions (SWC에서 호출 금지) */
void Rte_Adc_Publish_5ms(void);
void Rte_Adc_Publish_10ms(void);

/* SWC Interface - Rte_Read API */
Std_ReturnType Rte_Read_Adc_BatteryMotor(uint16_t* value);
Std_ReturnType Rte_Read_Adc_BatteryValve(uint16_t* value);
Std_ReturnType Rte_Read_Adc_Ignition(uint16_t* value);
Std_ReturnType Rte_Read_Adc_MotorA_Voltage(uint16_t* value);
Std_ReturnType Rte_Read_Adc_MotorA_Current(int16_t* value);
Std_ReturnType Rte_Read_Adc_MotorB_Voltage(uint16_t* value);
Std_ReturnType Rte_Read_Adc_MotorB_Current(int16_t* value);

#endif