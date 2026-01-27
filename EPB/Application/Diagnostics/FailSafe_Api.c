#include "FailSafe_Api.h"
#include "Adc_Api.h"
#include "UDS_Api.h"
#include "EB_Prj.h"

extern ADC_t g_ADC;

/* Battery voltage thresholds (mV) */
#define BAT_UNDERVOLTAGE_THRESHOLD    9000U   /* 9.0V */
#define BAT_OVERVOLTAGE_THRESHOLD     17000U  /* 17.0V */

/* DTC test cycles */
#define DTC_FAIL_CYCLE_COUNT          100U    /* 1 sec (10ms * 100) */
#define DTC_PASS_CYCLE_COUNT          300U    /* 3 sec (10ms * 300) */

/*===========================================================================
 * MCAL Layer
 *===========================================================================*/

/*===========================================================================
 * BSW Layer - Data Access Services
 *===========================================================================*/
static uint16_t Bsw_FailSafe_GetBatteryVoltage(void)
{
    return g_ADC.Physical.Power.UBB;
}

/*===========================================================================
 * FS Layer
 *===========================================================================*/
/* Battery voltage status enumeration */
typedef enum {
    BAT_VOLTAGE_UNDER = 0,    /* < 9000mV */
    BAT_VOLTAGE_NORMAL = 1,   /* 9000~17000mV */  
    BAT_VOLTAGE_OVER = 2      /* >= 17000mV */
} BatVoltageStatus_t;

static BatVoltageStatus_t Fs_BatDiag_DetermineVoltageStatusFromAdc(void)
{
    /* BSW Layer: Get battery voltage data */
    uint16_t voltage_mv = Bsw_FailSafe_GetBatteryVoltage();
    
    /* FS Layer: Determine voltage status based on thresholds */
    if (voltage_mv < BAT_UNDERVOLTAGE_THRESHOLD)
    {
        return BAT_VOLTAGE_UNDER;
    }
    else if ((voltage_mv >= BAT_UNDERVOLTAGE_THRESHOLD) && (voltage_mv < BAT_OVERVOLTAGE_THRESHOLD))
    {
        return BAT_VOLTAGE_NORMAL;
    }
    else /* voltage_mv >= BAT_OVERVOLTAGE_THRESHOLD */
    {
        return BAT_VOLTAGE_OVER;
    }
}

/*===========================================================================
 * ASW Layer
 *===========================================================================*/
static void Asw_BatDiag_ExecuteByStatus(BatVoltageStatus_t voltage_status)
{
    switch (voltage_status)
    {
        case BAT_VOLTAGE_UNDER:
            DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATUNDERVOLTAGE, DEM_TEST_FAIL, DTC_FAIL_CYCLE_COUNT);
            break;
            
        case BAT_VOLTAGE_NORMAL:
            DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATUNDERVOLTAGE, DEM_TEST_PASS, DTC_PASS_CYCLE_COUNT);
            DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATOVERVOLTAGE, DEM_TEST_PASS, DTC_PASS_CYCLE_COUNT);
            break;
            
        case BAT_VOLTAGE_OVER:
            DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATOVERVOLTAGE, DEM_TEST_FAIL, DTC_FAIL_CYCLE_COUNT);
            break;
            
        default:
            /* Invalid status - no action */
            break;
    }
}

static void Asw_BatDiag_MainFunction(void)
{
    BatVoltageStatus_t voltage_status;
    voltage_status = Fs_BatDiag_DetermineVoltageStatusFromAdc();
    Asw_BatDiag_ExecuteByStatus(voltage_status);
}

/*===========================================================================
 * Test를 위한 기존 Legacy 코드
 *===========================================================================*/
void BAT_Diag(void)
{
    Asw_BatDiag_MainFunction();
}

void Power_Diag(void)
{
    Asw_BatDiag_MainFunction();
}

void Diagnostic_Task_10ms(void)
{
    Asw_BatDiag_MainFunction();
}
