#include "Bsw_Adc.h"
#include "Mcal_Adc.h"
#include "Cdd_L9369Adc.h"

/* Static Snapshot 버퍼 (외부 노출 금지) */
static Bsw_Adc_Snapshot_t s_AdcSnapshot = {0};

/* 평균 계산용 버퍼 (6샘플) */
#define BSW_ADC_AVG_SAMPLES 6U
static uint16_t s_AvgBuffer_UBB[BSW_ADC_AVG_SAMPLES] = {0};
static uint16_t s_AvgBuffer_UBVR[BSW_ADC_AVG_SAMPLES] = {0};
static uint16_t s_AvgBuffer_IGN[BSW_ADC_AVG_SAMPLES] = {0};
static uint8_t s_AvgIndex = 0;

/* Raw → Physical 변환 함수 (내부 사용) */
static uint16_t Bsw_Adc_ConvertRawToPhysical(uint16_t raw, float vref, uint16_t r1, uint16_t r2)
{
    /* ADC Raw (10-bit) → mV 변환 */
    float adc_mV = ((float)raw * vref * 1000.0f) / 1024.0f;
    
    /* 전압 분배 회로 적용 */
    float physical_mV = adc_mV * ((float)(r1 + r2) / (float)r2);
    
    return (uint16_t)physical_mV;
}

/* 평균 계산 (Min/Max 제거) */
static uint16_t Bsw_Adc_CalculateAverage(uint16_t* buffer, uint8_t size)
{
    uint32_t sum = 0;
    uint16_t min = 0xFFFF;
    uint16_t max = 0;
    
    /* Min/Max 찾기 */
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] < min) min = buffer[i];
        if (buffer[i] > max) max = buffer[i];
        sum += buffer[i];
    }
    
    /* Min/Max 제거 후 평균 */
    if (size > 2) {
        sum = sum - min - max;
        return (uint16_t)(sum / (size - 2));
    } else {
        return (uint16_t)(sum / size);
    }
}

/* BSW ADC 초기화 */
void Bsw_Adc_Init(void)
{
    Mcal_Adc_Init();
    Cdd_L9369Adc_Init();
    
    s_AdcSnapshot.DataValid = 0;
    s_AvgIndex = 0;
}

/* 5ms 주기 샘플링 함수 */
void Bsw_Adc_MainFunction_5ms(void)
{
    uint16_t raw_ubb, raw_ubvr, raw_ign;
    
    /* MCAL ADC 읽기 */
    raw_ubb = Mcal_Adc_ReadRaw(MCAL_ADC_CH_UBB);
    raw_ubvr = Mcal_Adc_ReadRaw(MCAL_ADC_CH_UBVR);
    raw_ign = Mcal_Adc_ReadRaw(MCAL_ADC_CH_IGN);
    
    /* 평균 버퍼에 저장 */
    s_AvgBuffer_UBB[s_AvgIndex] = raw_ubb;
    s_AvgBuffer_UBVR[s_AvgIndex] = raw_ubvr;
    s_AvgBuffer_IGN[s_AvgIndex] = raw_ign;
    s_AvgIndex = (s_AvgIndex + 1) % BSW_ADC_AVG_SAMPLES;
    
    /* Raw → Physical 변환 (전압 분배: R1=300, R2=51) */
    s_AdcSnapshot.BatteryMotor_mV = Bsw_Adc_ConvertRawToPhysical(raw_ubb, 3.3f, 300, 51);
    s_AdcSnapshot.BatteryValve_mV = Bsw_Adc_ConvertRawToPhysical(raw_ubvr, 3.3f, 300, 51);
    s_AdcSnapshot.Ignition_mV = Bsw_Adc_ConvertRawToPhysical(raw_ign, 3.3f, 300, 51);
    
    /* CDD L9369 읽기 */
    Cdd_L9369Adc_ReadMotorA(&s_AdcSnapshot.MotorA_Voltage_mV, &s_AdcSnapshot.MotorA_Current_mA);
    Cdd_L9369Adc_ReadMotorB(&s_AdcSnapshot.MotorB_Voltage_mV, &s_AdcSnapshot.MotorB_Current_mA);
    
    s_AdcSnapshot.DataValid = 1;
}

/* 10ms 주기 가공 함수 */
void Bsw_Adc_MainFunction_10ms(void)
{
    /* 평균 계산 (Min/Max 제거) */
    uint16_t avg_ubb = Bsw_Adc_CalculateAverage(s_AvgBuffer_UBB, BSW_ADC_AVG_SAMPLES);
    uint16_t avg_ubvr = Bsw_Adc_CalculateAverage(s_AvgBuffer_UBVR, BSW_ADC_AVG_SAMPLES);
    uint16_t avg_ign = Bsw_Adc_CalculateAverage(s_AvgBuffer_IGN, BSW_ADC_AVG_SAMPLES);
    
    /* 가공된 값으로 Snapshot 업데이트 */
    s_AdcSnapshot.BatteryMotor_mV = Bsw_Adc_ConvertRawToPhysical(avg_ubb, 3.3f, 300, 51);
    s_AdcSnapshot.BatteryValve_mV = Bsw_Adc_ConvertRawToPhysical(avg_ubvr, 3.3f, 300, 51);
    s_AdcSnapshot.Ignition_mV = Bsw_Adc_ConvertRawToPhysical(avg_ign, 3.3f, 300, 51);
}

/* Snapshot 읽기 */
Std_ReturnType Bsw_Adc_GetSnapshot(Bsw_Adc_Snapshot_t* snapshot)
{
    if (snapshot == 0) {
        return E_NOT_OK;
    }
    
    if (s_AdcSnapshot.DataValid == 0) {
        return E_NOT_OK;
    }
    
    /* Snapshot 복사 */
    *snapshot = s_AdcSnapshot;
    
    return E_OK;
}