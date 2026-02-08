#include "Watchdog_Api.h"
#include "Me_Api.h"
#include "Cgm_Api.h"
#include "Port_Api.h"
#include "Stm_Api.h"
#include "Adc_Api.h"
#include "Dspi_Api.h"
#include "Can_Api.h"
#include "UDS_Api.h"
#include "Logic_Api.h"
#include "Main_Api.h"
#include "ccp.h"
#include "Interface.h"
#include "board.h"
#include "linflex_uart.h"
#include "Calibration.h"
#include "LookupTable.h"
#include "DEM_Priv.h"
#include "Spi_L9369Data_Types.h"
#include "Spi_TLE9461_Data_Types.h"
#include "Spi_TC277_Api.h"
#include "Spi_ExtEEPROM_Api.h"
#include "Diagnostics_CAN.h"
#include "Common.h"
#include "ElectronicParkBrake.h"
#include "FailSafe_Api.h"
#include "Os_Task.h"

static TaskControl_t g_TaskControl;
Main_t g_MAIN;
extern TC277_t g_TC277;


void SetSystemOn(void)
{
	SSMInput.SSMInIgnition = TRUE;
	g_TaskControl.Flag.SystemDown = FALSE;
	g_TaskControl.Flag.SystemOff = FALSE;
	g_TaskControl.PowerOffCount = 0;
}
void SetSystemOff(void)
{
	if (g_TaskControl.Flag.SystemDown == TRUE) {
		g_TaskControl.Flag.SystemOff = TRUE;
	}
}

void SetSystemDown(void)
{
	SSMInput.SSMInIgnition = FALSE;
	g_TaskControl.Flag.SystemDown = TRUE;
}

uint8 PowerOff(uint32 count)
{
	if(g_MAIN.PowerOffOk == FALSE){
		/* disable CAN BUS */
		PORT_SUB_UBVR_EN = FALSE;
		PORT_SUB_UBB_EN = FALSE;
		COM_ModeModify(COM_MODE_BUS_OFF);
		CAN_BusOff();
		Delayms(5);
		/* L9369 GO SLEEP */
		L9369PowerOff();
		/* MCU Interrupt disable */
		APP_InterruptDisable();
		/* TLF9461 GO SLEEP */
		CheckCANSBCRegStatus(SPI_CANSBC_DOWN);
		g_MAIN.PowerOffOk = TRUE;
	}
	else
	{
		/*nop*/
	}
	return g_MAIN.PowerOffOk;
}

static void TaskScheduler_Initialization(uint16 maxTask)
{
	g_TaskControl.TickCount = 0U;
	g_TaskControl.Tick = SYSTEM_TICK_5MS;
	g_TaskControl.TaskRun = TASK_none;
	g_TaskControl.TaskMax = maxTask;
	g_TaskControl.StepCount = 0U;
	g_TaskControl.PowerOffCount = 0U;
	g_TaskControl.Flag.Enable = FALSE;
	g_TaskControl.Flag.SystemDown = TRUE;
	g_TaskControl.Flag.SystemOff = FALSE;
	g_TaskControl.Flag.PowerOff = FALSE;
	g_TaskControl.Flag.EmergencyMode = FALSE;
	g_TaskControl.Flag.StartDownload = FALSE;
	g_TaskControl.Flag.SystemRunning = TRUE;
	g_TaskControl.Flag.ControlStep = 0U;
	g_TaskControl.Flag.RunMode = RUN_TYPE;
	g_TaskControl.Flag.OperationMode = EPB_OP_MODE;
	g_TaskControl.Flag.SystemSafety = FALSE;
	g_TaskControl.Flag.SystemSafetyCount = 0U;
	g_TaskControl.WakeupStatus = 0xFFU;
	g_TaskControl.WakeupStep = WAKEUP_INIT;
	g_MAIN.SystemStatus = SYSTEM_STATUS_INIT;
	g_MAIN.MtDrvStatus = MTDRV_STATUS_INIT;
	g_MAIN.Debug = DEBUG_MAIN_ALIVE;

	APP_Init();
}
static void ExecuteTask(void (*TaskFunc)(void), uint32_t* Cnt, uint32_t* Elapsed, uint32_t* ElapsedMax)
{
    uint32_t startTick = g_TaskControl.TickCount;
    
    (*TaskFunc)();
    (*Cnt)++;
    
    *Elapsed = g_TaskControl.TickCount - startTick;
    if (*ElapsedMax < *Elapsed) {
        *ElapsedMax = *Elapsed;
    }
}

static void TaskScheduler_TaskCalculation(void)
{
	if ((g_TaskControl.TickCount % (uint16)TASK_20ms) == 0U) {
		g_TaskControl.TaskRun = (uint16)TASK_20ms;
		ExecuteTask(OsTask_5ms, &g_TaskControl.TaskCount_5ms_Cnt, &g_TaskControl.TaskCount_5ms_TimeElapsed, &g_TaskControl.TaskCount_5ms_TimeElapsed_Max);
		ExecuteTask(OsTask_10ms, &g_TaskControl.TaskCount_10ms_Cnt, &g_TaskControl.TaskCount_10ms_TimeElapsed, &g_TaskControl.TaskCount_10ms_TimeElapsed_Max);
		ExecuteTask(OsTask_20ms, &g_TaskControl.TaskCount_20ms_Cnt, &g_TaskControl.TaskCount_20ms_TimeElapsed, &g_TaskControl.TaskCount_20ms_TimeElapsed_Max);
	}	
	else if ((g_TaskControl.TickCount % (uint16)TASK_10ms) == 0U) {
		g_TaskControl.TaskRun = (uint16)TASK_10ms;
		ExecuteTask(OsTask_5ms, &g_TaskControl.TaskCount_5ms_Cnt, &g_TaskControl.TaskCount_5ms_TimeElapsed, &g_TaskControl.TaskCount_5ms_TimeElapsed_Max);
		ExecuteTask(OsTask_10ms, &g_TaskControl.TaskCount_10ms_Cnt, &g_TaskControl.TaskCount_10ms_TimeElapsed, &g_TaskControl.TaskCount_10ms_TimeElapsed_Max);
	}

	else if ((g_TaskControl.TickCount % (uint16)TASK_5ms) == 0U) {
		g_TaskControl.TaskRun = (uint16)TASK_5ms;
		ExecuteTask(OsTask_5ms, &g_TaskControl.TaskCount_5ms_Cnt, &g_TaskControl.TaskCount_5ms_TimeElapsed, &g_TaskControl.TaskCount_5ms_TimeElapsed_Max);
	}
	else
	{
		/*NOP*/
	}
	Task_Diagnostic();
}

static void CheckWakeupStatus(uint8 mode)
{
	g_TaskControl.WakeupStep = mode;
	g_TaskControl.WakeupStatusTemp = (uint8)((GetEpbWakeupStatus() << 4U)/* | SpiGetWakeupStatus()*/);
	if (g_TaskControl.WakeupStep == WAKEUP_START_S0) {
		g_TaskControl.WakeupStatus = (uint8)(g_TaskControl.WakeupStatusTemp & 0x0FU);
	}
	else {
		if (g_TaskControl.WakeupStep == WAKEUP_OFF) {
			if (g_TaskControl.WakeupStatus == 1U) { /* EPB Wakeup */
				g_TaskControl.WakeupStatus = 3U;
			}
			else {
				/* nop */
			}
		}
		else {
			if (g_TaskControl.WakeupStep == WAKEUP_START_S3) {
				if (g_TaskControl.WakeupStatus == 3U) {
					g_TaskControl.WakeupStatus = 5U;
				}
				else {
					/* nop */
				}
			}
			else {
				/* nop */
			}
		}
	}
	g_MAIN.Session = UDS_GetCurrentSession();
}

void Task_Diagnostic(void)
{
	if(g_TaskControl.TaskCount_5ms_Cnt>1000000)
	{	
		g_TaskControl.TaskCount_10ms_Cnt= 0;
		g_TaskControl.TaskCount_20ms_Cnt= 0;
	}	
}

void TaskScheduler_TaskCount(void)
{
	g_TaskControl.Flag.Enable = TRUE;
	g_TaskControl.TickCount++;
}

static void TaskScheduler_20ms(void)
{

	if (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
	{	
		if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
		{
			CheckL9369Watchdog();
		}
		CANSBCWatchdog();
		EcuResetCheck();
		CheckTC277Status();
	}
	else{
	}
}

void TaskScheduler_10ms(void)
{	
	if (g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
	{
		if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
		{
			L9369AdcAvr();
			EpbMotorTestLogic();
			EpbMotorDriveProcess();
		}
		Diagnostic_Task_10ms();
		
	}
	else {
		/* nop */
	}
}
static void TaskScheduler_5ms(void)
{
	if (g_TaskControl.Flag.SystemDown == TRUE) 
	{ /* IGN Off�� ���� */
		//CheckL9369RegStatus(SPI_L9369_RUN);
		TxCanMessage();
		CheckAdcStatus();
		CheckPortStatus();
		CheckCANSBCRegStatus(SPI_CANSBC_RUN);
		/* IGN Off�� 1�� �̻� ���ӵ� ������� Ȯ�� */
		if (g_TaskControl.Flag.SystemOff == TRUE) 
		{ /*	5�� �� off	*/
			if (++g_TaskControl.PowerOffCount > SYSTEM_POWER_OFF_TIME) 
			{ /* 5 sec */
				//if (GetEpbPowerLatchRequest() == 0U)
				//{
					g_TaskControl.Flag.PowerOff = PowerOff(g_TaskControl.PowerOffCount);
				//} 
				//else{ /* no latchdown */
				//}
			} 
			else{ 
				SaveParamData();
			}
	 	}
		else{
		}
	}
	else { 
		if(g_MAIN.SystemStatus == SYSTEM_STATUS_INIT){
			CheckWakeupStatus(WAKEUP_START);
			CheckPortStatus();
			CheckAdcStatus();
			g_MAIN.SystemStatus = TLE9461Init(FALSE);
			 
		}
		else if(g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL) 
		{
			TxCanMessage();
			CheckPortStatus();
			CheckAdcStatus();
			CheckCANSBCRegStatus(SPI_CANSBC_RUN);
			if(g_TC277.fMainfault == TRUE)
			{
				if(g_MAIN.MtDrvStatus == MTDRV_STATUS_NORMAL)
				{
					CheckL9369RegStatus(SPI_L9369_RUN);
				}
				else
				{
					g_MAIN.MtDrvStatus = L9369Init(FALSE);
				}
			}
			else
			{
				g_MAIN.MtDrvStatus = MTDRV_STATUS_INIT;
				g_MAIN.L9369_InitStep = MTDRV_STATUS_INIT;
			}
		}
		else{
		}
	}
}

static void TaskScheduler_ActivateTask(void)
{
	uint32_t TickCount_5ms = 0, TickCount_10ms = 0, TickCount_20ms = 0;
	
	if (g_TaskControl.TickCount > g_TaskControl.TaskMax) {
		g_TaskControl.TickCount = 1U;
	}
	else {
		/* nop */
	}

	if (g_TaskControl.Flag.Enable == TRUE) {
		g_TaskControl.Flag.Enable = FALSE;			/* Stop task scheduler */
		
		TaskScheduler_TaskCalculation();
	}
	else {
		/* nop */
	}
}


static void SubEpbInitialize(void)
{
	BoardDisableInterrupts();
	DisableWatchdog();
	MEInitialize();
	CGMInitialize();
	PORTInitialize();
	STMInitialize();
	ADCInitialize();
	SpiInitialize();
	CanInitialize();
	BoardEnableInterrupts();
}


void main(void)
{	
	SubEpbInitialize();
	
	ElectronicParkBrake_initialize();
	
	TaskScheduler_Initialization(TASK_5000ms);

	EepromReadAll();

	while(1)
	{
		TaskScheduler_ActivateTask();
	}
}


