#ifndef BSW_ADC_H
#define BSW_ADC_H

#include <stdint.h>

/* Snapshot 구조체 정의 */
typedef struct {
    uint16_t BatteryMotor_mV;      /* UBB: 모터용 배터리 전압 */
    uint16_t BatteryValve_mV;      /* UBVR: 밸브용 배터리 전압 */
    uint16_t Ignition_mV;          /* IGN: 이그니션 전압 */
    uint16_t MotorA_Voltage_mV;    /* 모터 A 전압 */
    uint16_t MotorB_Voltage_mV;    /* 모터 B 전압 */
    int16_t  MotorA_Current_mA;    /* 모터 A 전류 */
    int16_t  MotorB_Current_mA;    /* 모터 B 전류 */
    uint8_t  DataValid;            /* 데이터 유효성 플래그 */
} Bsw_Adc_Snapshot_t;

/* 표준 리턴 타입 */
typedef uint8_t Std_ReturnType;
#define E_OK     ((Std_ReturnType)0u)
#define E_NOT_OK ((Std_ReturnType)1u)

/* BSW ADC 초기화 */
void Bsw_Adc_Init(void);

/* 5ms 주기 샘플링 함수 */
void Bsw_Adc_MainFunction_5ms(void);

/* 10ms 주기 가공 함수 */
void Bsw_Adc_MainFunction_10ms(void);

/* Snapshot 읽기 */
Std_ReturnType Bsw_Adc_GetSnapshot(Bsw_Adc_Snapshot_t* snapshot);

#endif