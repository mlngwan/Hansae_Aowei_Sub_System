
#include "FailSafe_Api.h"
#include "Adc_Api.h"
#include "UDS_Api.h"
#include "EB_Prj.h"

extern ADC_t g_ADC;


void BAT_Diag(void)
{
	/*BAT*/
	if(g_ADC.Physical.Power.UBB<9000)
	{
		DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATUNDERVOLTAGE,DEM_TEST_FAIL,100);//1Sec
	}
	else if(g_ADC.Physical.Power.UBB>=9000 && g_ADC.Physical.Power.UBB<17000)
	{
		DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATUNDERVOLTAGE,DEM_TEST_PASS,300);//3Sec
		DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATOVERVOLTAGE,DEM_TEST_PASS,300);//3Sec
	}
	else if(g_ADC.Physical.Power.UBB>=17000)
	{
		DTC_Status_Cycle(DEM_DTC_IDX_DTC_BATOVERVOLTAGE,DEM_TEST_FAIL,100);//1Sec
	}
}

void Power_Diag(void)//10ms
{		
	BAT_Diag();
}
/* 10ms period */
void Diagnostic_Task_10ms(void)
{
	Power_Diag();
	/*ASIC_Diag는 각 ASIC 함수 에서 진행  */
}




