#ifndef BSW_ADC_H
#define BSW_ADC_H

#include "Std_Types.h" /* 프로젝트 공통 표준 타입 (config/Std_Types.h) */

/* Snapshot 구조체 정의 */
typedef struct
{
    uint16_t BatteryMotor_mV;   /* UBB: 모터용 배터리 전압 */
    uint16_t BatteryValve_mV;   /* UBVR: 밸브용 배터리 전압 */
    uint16_t Ignition_mV;       /* IGN: 이그니션 전압 */
    uint16_t MotorA_Voltage_mV; /* 모터 A 전압 */
    uint16_t MotorB_Voltage_mV; /* 모터 B 전압 */
    int16_t MotorA_Current_mA;  /* 모터 A 전류 */
    int16_t MotorB_Current_mA;  /* 모터 B 전류 */
    uint8_t DataValid;          /* 데이터 유효성 플래그 */
} Bsw_Adc_Snapshot_t;

/* BSW ADC 초기화 */
void Bsw_Adc_Init(void);

/* 5ms 주기 샘플링 함수 */
void Bsw_Adc_MainFunction_5ms(void);

/* 10ms 주기 가공 함수 */
void Bsw_Adc_MainFunction_10ms(void);

/* Raw Snapshot 읽기 (5ms 즉시값) */
Std_ReturnType Bsw_Adc_GetSnapshot(Bsw_Adc_Snapshot_t *snapshot);

/* Avg Snapshot 읽기 (10ms 평균값) */
Std_ReturnType Bsw_Adc_GetProcessedSnapshot(Bsw_Adc_Snapshot_t *snapshot);

#endif