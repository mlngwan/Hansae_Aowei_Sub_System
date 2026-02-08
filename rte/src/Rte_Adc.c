#include "Rte_Adc.h"
#include "Bsw_Adc.h"

/* Static SR Buffer (외부 노출 금지) */
static uint16_t s_RteBuffer_BatteryMotor = 0;
static uint16_t s_RteBuffer_BatteryValve = 0;
static uint16_t s_RteBuffer_Ignition = 0;
static uint16_t s_RteBuffer_MotorA_Voltage = 0;
static int16_t  s_RteBuffer_MotorA_Current = 0;
static uint16_t s_RteBuffer_MotorB_Voltage = 0;
static int16_t  s_RteBuffer_MotorB_Current = 0;
static uint8_t  s_RteBuffer_Valid = 0;

/* RTE Internal: 5ms Publish */
void Rte_Adc_Publish_5ms(void)
{
    Bsw_Adc_Snapshot_t snapshot;
    
    if (Bsw_Adc_GetSnapshot(&snapshot) == E_OK) {
        /* BSW Snapshot → RTE SR Buffer */
        s_RteBuffer_BatteryMotor = snapshot.BatteryMotor_mV;
        s_RteBuffer_BatteryValve = snapshot.BatteryValve_mV;
        s_RteBuffer_Ignition = snapshot.Ignition_mV;
        s_RteBuffer_MotorA_Voltage = snapshot.MotorA_Voltage_mV;
        s_RteBuffer_MotorA_Current = snapshot.MotorA_Current_mA;
        s_RteBuffer_MotorB_Voltage = snapshot.MotorB_Voltage_mV;
        s_RteBuffer_MotorB_Current = snapshot.MotorB_Current_mA;
        s_RteBuffer_Valid = 1;
    }
}

/* RTE Internal: 10ms Publish */
void Rte_Adc_Publish_10ms(void)
{
    /* 10ms 가공 결과 publish (현재는 5ms와 동일) */
    Rte_Adc_Publish_5ms();
}

/* SWC Interface: Rte_Read API */
Std_ReturnType Rte_Read_Adc_BatteryMotor(uint16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_BatteryMotor;
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_BatteryValve(uint16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_BatteryValve;
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_Ignition(uint16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_Ignition;
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Voltage(uint16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_MotorA_Voltage;
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Current(int16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_MotorA_Current;
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Voltage(uint16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_MotorB_Voltage;
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Current(int16_t* value)
{
    if (value == 0 || s_RteBuffer_Valid == 0) {
        return E_NOT_OK;
    }
    *value = s_RteBuffer_MotorB_Current;
    return E_OK;
}