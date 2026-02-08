#include "SwcAdcMon.h"
#include "Rte_Adc.h"

/* 진단 임계값 (데모용) */
#define BATTERY_MIN_MV  9000U   /* 9V */
#define BATTERY_MAX_MV  16000U  /* 16V */
#define MOTOR_CURRENT_MAX_MA  20000  /* 20A */

/* Static 진단 카운터 */
static uint8_t s_BatteryLowCount = 0;
static uint8_t s_BatteryHighCount = 0;
static uint8_t s_MotorOverCurrentCount = 0;

/* SWC ADC Monitor 초기화 */
void SwcAdcMon_Init(void)
{
    s_BatteryLowCount = 0;
    s_BatteryHighCount = 0;
    s_MotorOverCurrentCount = 0;
}

/* 10ms Runnable */
void SwcAdcMon_Runnable_10ms(void)
{
    uint16_t battery_mV = 0;
    uint16_t motorA_voltage = 0;
    int16_t motorA_current = 0;
    uint16_t motorB_voltage = 0;
    int16_t motorB_current = 0;
    
    /* RTE를 통해 ADC 값 읽기 */
    if (Rte_Read_Adc_BatteryMotor(&battery_mV) == E_OK) {
        /* 배터리 전압 범위 체크 */
        if (battery_mV < BATTERY_MIN_MV) {
            s_BatteryLowCount++;
            /* TODO: DTC 설정 또는 경고 */
        } else {
            s_BatteryLowCount = 0;
        }
        
        if (battery_mV > BATTERY_MAX_MV) {
            s_BatteryHighCount++;
            /* TODO: DTC 설정 또는 경고 */
        } else {
            s_BatteryHighCount = 0;
        }
    }
    
    /* 모터 A 전류 체크 */
    if (Rte_Read_Adc_MotorA_Voltage(&motorA_voltage) == E_OK &&
        Rte_Read_Adc_MotorA_Current(&motorA_current) == E_OK) {
        
        if (motorA_current > MOTOR_CURRENT_MAX_MA || motorA_current < -MOTOR_CURRENT_MAX_MA) {
            s_MotorOverCurrentCount++;
            /* TODO: 과전류 보호 로직 */
        } else {
            s_MotorOverCurrentCount = 0;
        }
    }
    
    /* 모터 B 전류 체크 */
    if (Rte_Read_Adc_MotorB_Voltage(&motorB_voltage) == E_OK &&
        Rte_Read_Adc_MotorB_Current(&motorB_current) == E_OK) {
        
        if (motorB_current > MOTOR_CURRENT_MAX_MA || motorB_current < -MOTOR_CURRENT_MAX_MA) {
            s_MotorOverCurrentCount++;
            /* TODO: 과전류 보호 로직 */
        }
    }
    
    /* 데모: 진단 결과 출력 (실제로는 DTC 설정 등) */
    /* printf("Battery: %d mV, MotorA: %d mV / %d mA\n", battery_mV, motorA_voltage, motorA_current); */
}