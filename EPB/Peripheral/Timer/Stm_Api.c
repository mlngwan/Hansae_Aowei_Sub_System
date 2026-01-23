#include "common.h"
#include "Port_Api.h"
#include "Stm_Api.h"
#include "Main_Api.h"
#include "Adc_Api.h"
#include "Spi_L9369Data_Types.h"
#include "Spi_TC277_Api.h"
/*===========================================================================
 * CDD Layer
 *===========================================================================*/
Stm_t g_STM;
extern ADC_t g_ADC;
extern Main_t g_MAIN;
void STMInitialize(void)
{
	STM.CH[0].CMP.R = (uint32_t)(Time3 * 10);

	// 1msec period,	Match on System Timer Mode channel 0
	STM.CH[0].CIR.B.CIF = 1; // clear interrupt flag if set
	STM.CH[0].CCR.B.CEN = 1; // enable CCR0 interrupt

	STM.CNT.R = 0;				 // initialize STM counter
	STM.CR.R = 0x00000003; // Set prescaler and enable STM

	INTC.PSR[30].R = 15; // set interrupt priority
}

void STM_CH0_ISR(void)
{
	STM.CH[0].CIR.B.CIF = 1; // clear interrupt flag
	STM.CNT.R = 0;

	TaskScheduler_TaskCount();

	if (g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
	{
		L9369_TASK_1ms();
	}
}
