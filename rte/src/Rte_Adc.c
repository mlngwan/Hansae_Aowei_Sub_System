#include "Rte_Adc.h"
#include "Bsw_Adc.h"
#include "Std_Types.h"   /* SchM_Enter/Exit_Exclusive_Area */

/* ===== Raw SR Buffer (5ms 갱신) ===== */
static uint16_t s_RteRaw_BatteryMotor = 0;
static uint16_t s_RteRaw_BatteryValve = 0;
static uint16_t s_RteRaw_Ignition = 0;
static uint16_t s_RteRaw_MotorA_Voltage = 0;
static int16_t  s_RteRaw_MotorA_Current = 0;
static uint16_t s_RteRaw_MotorB_Voltage = 0;
static int16_t  s_RteRaw_MotorB_Current = 0;
static uint8_t  s_RteRaw_Valid = 0;

/* ===== Avg SR Buffer (10ms 갱신) ===== */
static uint16_t s_RteAvg_BatteryMotor = 0;
static uint16_t s_RteAvg_BatteryValve = 0;
static uint16_t s_RteAvg_Ignition = 0;
static uint16_t s_RteAvg_MotorA_Voltage = 0;
static int16_t  s_RteAvg_MotorA_Current = 0;
static uint16_t s_RteAvg_MotorB_Voltage = 0;
static int16_t  s_RteAvg_MotorB_Current = 0;
static uint8_t  s_RteAvg_Valid = 0;

/* --- 내부 헬퍼: Raw Buffer에 Snapshot 복사 (임계구역 보호) --- */
static void Rte_Adc_CopyToRawBuffer(const Bsw_Adc_Snapshot_t *snapshot)
{
    SchM_Enter_Exclusive_Area();
    s_RteRaw_BatteryMotor  = snapshot->BatteryMotor_mV;
    s_RteRaw_BatteryValve  = snapshot->BatteryValve_mV;
    s_RteRaw_Ignition      = snapshot->Ignition_mV;
    s_RteRaw_MotorA_Voltage = snapshot->MotorA_Voltage_mV;
    s_RteRaw_MotorA_Current = snapshot->MotorA_Current_mA;
    s_RteRaw_MotorB_Voltage = snapshot->MotorB_Voltage_mV;
    s_RteRaw_MotorB_Current = snapshot->MotorB_Current_mA;
    s_RteRaw_Valid = 1;
    SchM_Exit_Exclusive_Area();
}

/* --- 내부 헬퍼: Avg Buffer에 Snapshot 복사 (임계구역 보호) --- */
static void Rte_Adc_CopyToAvgBuffer(const Bsw_Adc_Snapshot_t *snapshot)
{
    SchM_Enter_Exclusive_Area();
    s_RteAvg_BatteryMotor  = snapshot->BatteryMotor_mV;
    s_RteAvg_BatteryValve  = snapshot->BatteryValve_mV;
    s_RteAvg_Ignition      = snapshot->Ignition_mV;
    s_RteAvg_MotorA_Voltage = snapshot->MotorA_Voltage_mV;
    s_RteAvg_MotorA_Current = snapshot->MotorA_Current_mA;
    s_RteAvg_MotorB_Voltage = snapshot->MotorB_Voltage_mV;
    s_RteAvg_MotorB_Current = snapshot->MotorB_Current_mA;
    s_RteAvg_Valid = 1;
    SchM_Exit_Exclusive_Area();
}

/* RTE Internal: 5ms Publish (Raw Snapshot → Raw Buffer) */
void Rte_Adc_Publish_5ms(void)
{
    Bsw_Adc_Snapshot_t snapshot;

    if (Bsw_Adc_GetSnapshot(&snapshot) == E_OK) {
        Rte_Adc_CopyToRawBuffer(&snapshot);
    }
}

/* RTE Internal: 10ms Publish (Avg Snapshot → Avg Buffer) */
void Rte_Adc_Publish_10ms(void)
{
    Bsw_Adc_Snapshot_t processed;

    if (Bsw_Adc_GetProcessedSnapshot(&processed) == E_OK) {
        Rte_Adc_CopyToAvgBuffer(&processed);
    }
}

/* ===================================================================
 * SWC Interface: _Raw APIs (5ms Raw 값 읽기)
 * =================================================================== */

Std_ReturnType Rte_Read_Adc_BatteryMotor_Raw(uint16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_BatteryMotor;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_BatteryValve_Raw(uint16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_BatteryValve;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_Ignition_Raw(uint16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_Ignition;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Voltage_Raw(uint16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_MotorA_Voltage;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Current_Raw(int16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_MotorA_Current;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Voltage_Raw(uint16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_MotorB_Voltage;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Current_Raw(int16_t* value)
{
    if (value == 0 || s_RteRaw_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteRaw_MotorB_Current;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

/* ===================================================================
 * SWC Interface: _Avg APIs (10ms 평균값 읽기)
 * =================================================================== */

Std_ReturnType Rte_Read_Adc_BatteryMotor_Avg(uint16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_BatteryMotor;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_BatteryValve_Avg(uint16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_BatteryValve;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_Ignition_Avg(uint16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_Ignition;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Voltage_Avg(uint16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_MotorA_Voltage;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Current_Avg(int16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_MotorA_Current;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Voltage_Avg(uint16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_MotorB_Voltage;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Current_Avg(int16_t* value)
{
    if (value == 0 || s_RteAvg_Valid == 0) { return E_NOT_OK; }
    SchM_Enter_Exclusive_Area();
    *value = s_RteAvg_MotorB_Current;
    SchM_Exit_Exclusive_Area();
    return E_OK;
}

/* ===================================================================
 * Legacy APIs (호환성: Avg valid → Avg, 아니면 Raw 폴백)
 * =================================================================== */

Std_ReturnType Rte_Read_Adc_BatteryMotor(uint16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_BatteryMotor;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_BatteryMotor;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_Adc_BatteryValve(uint16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_BatteryValve;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_BatteryValve;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_Adc_Ignition(uint16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_Ignition;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_Ignition;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Voltage(uint16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_MotorA_Voltage;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_MotorA_Voltage;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_Adc_MotorA_Current(int16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_MotorA_Current;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_MotorA_Current;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Voltage(uint16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_MotorB_Voltage;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_MotorB_Voltage;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_Adc_MotorB_Current(int16_t* value)
{
    if (value == 0) { return E_NOT_OK; }
    if (s_RteAvg_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteAvg_MotorB_Current;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    if (s_RteRaw_Valid != 0) {
        SchM_Enter_Exclusive_Area();
        *value = s_RteRaw_MotorB_Current;
        SchM_Exit_Exclusive_Area();
        return E_OK;
    }
    return E_NOT_OK;
}