
#include "Spi_TLE9461_Data_Types.h"
#include "Port_Api.h"
#include "Main_Api.h"


extern CANSBC_REG_MOSI g_CANSBC_REG_MOSI;
extern CANSBC_REG_MISO g_CANSBC_REG_MISO;
extern CANSBC_RegVar_t g_CANSBC_RegRead;
extern CANSBC_RegWrite_t g_CANSBC_RegWrite;
static CANSBC_RegWrite_t *g_pCANSBC_RegWrite;
extern CANSBC_FaultStatus_t g_CANSBC_FaultStatus;
static CANSBC_FaultStatus_t *g_pCANSBC_FaultStatus;
extern CANSBC_Status_t g_CANSBC_Status;
static CANSBC_Status_t *g_pCANSBC_Status;
CANSBC_t g_CANSBC;
extern Main_t g_MAIN;

void CheckCANSBCRegStatus(uint8 step)
{
	

	if (g_CANSBC_RegWrite.Step == 0x00U) {
		CANSBC_ReadWrite(CANSBCGetMsg(0x43U));	/*	WD SPI state*/
		CANSBC_ReadWrite(CANSBCGetMsg(0x01U)); /*	VCC		*/
		CANSBC_ReadWrite(CANSBCGetMsg(0x02U));	/*	FO,CP	*/
		CANSBC_ReadWrite(CANSBCGetMsg(0x41U));	/*		*/
	}
	else if (g_CANSBC_RegWrite.Step == 0x01U) {
		CANSBC_ReadWrite(CANSBCGetMsg(0x17U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x18U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x1CU));
		CANSBC_ReadWrite(CANSBCGetMsg(0x1DU));
		
	}
	else if (g_CANSBC_RegWrite.Step == 0x02U) {
		CANSBC_ReadWrite(CANSBCGetMsg(0x40U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x41U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x42U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x44U));
	
	}
	else if (g_CANSBC_RegWrite.Step == 0x03U) {
		CANSBC_ReadWrite(CANSBCGetMsg(0x08U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x0BU));
		CANSBC_ReadWrite(CANSBCGetMsg(0x0CU));
		CANSBC_ReadWrite(CANSBCGetMsg(0x0EU));
	
	}
	else if (g_CANSBC_RegWrite.Step == 0x04U) {
		CANSBC_ReadWrite(CANSBCGetMsg(0x48U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x54U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x55U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x07U));
	}
	else if (g_CANSBC_RegWrite.Step == 0x05U) {
		
		CANSBC_ReadWrite(CANSBCGetMsg(0x1EU));
		CANSBC_ReadWrite(CANSBCGetMsg(0x46U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x0FU));
		CANSBC_ReadWrite(CANSBCGetMsg(0x47U));
	}
	else if (g_CANSBC_RegWrite.Step == 0x06U) {
		CANSBC_ReadWrite(CANSBCGetMsg(0x1FU));
		CANSBC_ReadWrite(CANSBCGetMsg(0x04U)); /* CAN Control */
		CANSBC_ReadWrite(CANSBCGetMsg(0x06U));
	}
	else
	{
		//
	}
	
	if (g_CANSBC_RegWrite.Step < 0x06U) {
		g_CANSBC_RegWrite.Step++;
	}
	else {
		g_CANSBC_RegWrite.Step = 0x00U;
	}
	
	if(step == SPI_CANSBC_DOWN)
	{
		CANSBC_ReadWrite(CANSBCSetMsg_0x46());
		CANSBC_ReadWrite(CANSBCGetMsg(0x46U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x43U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x44U));
		CANSBC_ReadWrite(CANSBCGetMsg(0x47U));
		CANSBC_ReadWrite(CANSBCSetMsg_0x02(CP_DISABLE));	
		CANSBC_ReadWrite(CANSBCSetMsg_0x01(MODE_GO_SLEEP));	
	}
	else{ /* nop */
	}
}


void CANSBCDisableWatchdog(void)
{
	//the watchdog disabled in stop mode
		g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep = GO_STEP1;
}


uint16_t CANSBCWatchdog(void)
{
	uint8_t MODE = 0,MODE_SLEEP = 0;
	g_CANSBC_RegWrite.SpiTLE9461WDTime++;
	if((g_CANSBC_RegWrite.SpiTLE9461WDTime % 2U)== 0U ){ /* 40ms */
		g_CANSBC_RegWrite.SpiTLE9461WDTime = 0U;
		if(g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep == GO_STEP1){
			if(!g_CANSBC.fStart_Mutex){
				g_CANSBC.fStart_Mutex = 1;
			g_CANSBC_RegWrite.WD_STM_EN_1 = 0x01U;
			CANSBC_ReadWrite(CANSBCSetMsg_0x06());
			g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep = GO_STEP2;
			}
		}
		else if (g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep == GO_STEP3){
			CANSBC_ReadWrite(CANSBCSetMsg_0x01(MODE_GO_STOP));
			 g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep = 0;
			 CANSBC_ReadWrite(CANSBCGetMsg(0x01U));
			 CANSBC_ReadWrite(CANSBCGetMsg(0x43U));
		} else { /* nop */
		}
		MODE = g_CANSBC_RegRead._0x01.field.MODE;
		MODE_SLEEP = g_CANSBC_RegRead._0x43.field._DEV_STAT;
		
		if(MODE == MODE_GO_STOP || MODE_SLEEP == 0x02U){
			//NOP
		} else {
			if(g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep == GO_STEP2){
				g_CANSBC_RegWrite.WD_STM_EN_0 = 0x01U;
				g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep = GO_STEP3;
				g_CANSBC.fStart_Mutex = 0;
			}
		CANSBC_ReadWrite(CANSBCSetMsg_0x03()); //Watchdog triggering
		//CANSBC_ReadWrite(CANSBCGetMsg(0x03U)); //get watchdog register
		}
	}
	
}

void CANSBCInitialize(void)
{
		CANSBC_ReadWrite(CANSBCSetMsg_0x01(MODE_GO_NORMAL));
		CANSBC_ReadWrite(CANSBCSetMsg_0x02(CP_ENABLE));
		CANSBC_ReadWrite(CANSBCSetMsg_0x04());
		CANSBC_ReadWrite(CANSBCSetMsg_0x06());
		CANSBC_ReadWrite(CANSBCSetMsg_0x07());
		CANSBC_ReadWrite(CANSBCSetMsg_0x08());
		CANSBC_ReadWrite(CANSBCSetMsg_0x0B());
		CANSBC_ReadWrite(CANSBCSetMsg_0x0C());
		CANSBC_ReadWrite(CANSBCSetMsg_0x0E());
		CANSBC_ReadWrite(CANSBCSetMsg_0x0F());
		CANSBC_ReadWrite(CANSBCSetMsg_0x1D());
		CANSBC_ReadWrite(CANSBCSetMsg_0x41());
}

void CANSBCInit(void)
	
{
	g_pCANSBC_RegWrite = CANSBCRegInit();
	CANSBCInitialize();
	g_CANSBC_RegWrite.SpiTLE9461WDTime = 2U;
}

void CANSBCCheckStatus(uint8 addr, uint16 data)
{

	CANSBC_REG_MISO miso;

	miso.data16 = data;
	switch (addr) {
		case 0x01U:
			g_CANSBC_RegRead._0x01.data = miso.field.data;
			g_CANSBC_FaultStatus.SPI_Status.data = miso.field.status;
			break;
		case 0x02U:
			g_CANSBC_RegRead._0x02.data = miso.field.data;
			break;
		case 0x03U:
			g_CANSBC_RegRead._0x03.data = miso.field.data;
			break;
		case 0x04U:
			g_CANSBC_RegRead._0x04.data = miso.field.data;
			break;
		case 0x06U:
			g_CANSBC_RegRead._0x06.data = miso.field.data;
			break;
		case 0x07U:
			g_CANSBC_RegRead._0x07.data = miso.field.data;
			break;
		case 0x08U:
			g_CANSBC_RegRead._0x08.data = miso.field.data;
			break;
		case 0x0BU:
			g_CANSBC_RegRead._0x0B.data = miso.field.data;
			break;
		case 0x0CU:
			g_CANSBC_RegRead._0x0C.data = miso.field.data;
			break;
		case 0x0EU:
			g_CANSBC_RegRead._0x0E.data = miso.field.data;
			break;
		case 0x0FU:
			g_CANSBC_RegRead._0x0F.data = miso.field.data;
			break;
		case 0x17U:
			g_CANSBC_RegRead._0x17.data = miso.field.data;
			break;
		case 0x18U:
			g_CANSBC_RegRead._0x18.data = miso.field.data;
			break;
		case 0x1CU:
			g_CANSBC_RegRead._0x1C.data = miso.field.data;
			break;
		case 0x1DU:
			g_CANSBC_RegRead._0x1D.data = miso.field.data;
			break;
		case 0x1EU:
			g_CANSBC_RegRead._0x1E.data = miso.field.data;
			break;
		case 0x1FU:
			g_CANSBC_RegRead._0x1F.data = miso.field.data;
			break;
		case 0x40U:
			g_CANSBC_RegRead._0x40.data = miso.field.data;
			if (g_CANSBC_FaultStatus.VS_UV == 0x00U) {
				g_CANSBC_FaultStatus.VS_UV = g_CANSBC_RegRead._0x40.field.VS_UV;
			}
			if (g_CANSBC_FaultStatus.VS_OV == 0x00U) {
				g_CANSBC_FaultStatus.VS_OV = g_CANSBC_RegRead._0x40.field.VS_OV;
			}
			if (g_CANSBC_FaultStatus.VCC1_OV == 0x00U) {
				g_CANSBC_FaultStatus.VCC1_OV = g_CANSBC_RegRead._0x40.field.VCC1_OV;
			}
			if (g_CANSBC_FaultStatus.VCC1_WARN == 0x00U) {
				g_CANSBC_FaultStatus.VCC1_WARN = g_CANSBC_RegRead._0x40.field.VCC1_WARN;
			}
			break;
		
		case 0x41U:
			g_CANSBC_RegRead._0x41.data = miso.field.data;
			if (g_CANSBC_FaultStatus.POR == 0x00U) {
				g_CANSBC_FaultStatus.POR = g_CANSBC_RegRead._0x41.field.POR;
			}
			if (g_CANSBC_FaultStatus.VCC2_OT == 0x00U) {
				g_CANSBC_FaultStatus.VCC2_OT = g_CANSBC_RegRead._0x41.field.VCC2_OT;
			}
			if (g_CANSBC_FaultStatus.VCC2_UV == 0x00U) {
				g_CANSBC_FaultStatus.VCC2_UV = g_CANSBC_RegRead._0x41.field.VCC2_UV;
			}
			if (g_CANSBC_FaultStatus.VCC1_SC == 0x00U) {
				g_CANSBC_FaultStatus.VCC1_SC = g_CANSBC_RegRead._0x41.field.VCC1_SC;
			}
			if (g_CANSBC_FaultStatus.VCC1_UV == 0x00U) {
				g_CANSBC_FaultStatus.VCC1_UV = g_CANSBC_RegRead._0x41.field.VCC1_UV;
			}
			break;
		
		case 0x42U:
			g_CANSBC_RegRead._0x42.data = miso.field.data;
			if (g_CANSBC_FaultStatus.TSD2_SAFE == 0x00U) {
				g_CANSBC_FaultStatus.TSD2_SAFE = g_CANSBC_RegRead._0x42.field.TSD2_SAFE;
			}
			if (g_CANSBC_FaultStatus.TSD2 == 0x00U) {
				g_CANSBC_FaultStatus.TSD2 = g_CANSBC_RegRead._0x42.field.TSD2;
			}
			if (g_CANSBC_FaultStatus.TSD1 == 0x00U) {
				g_CANSBC_FaultStatus.TSD1 = g_CANSBC_RegRead._0x42.field.TSD1;
			}
			if (g_CANSBC_FaultStatus.TPW == 0x00U) {
				g_CANSBC_FaultStatus.TPW = g_CANSBC_RegRead._0x42.field.TPW;
			}
			break;
		
		case 0x43U:
			g_CANSBC_RegRead._0x43.data = miso.field.data;
			if (g_CANSBC_FaultStatus.DEV_STAT == 0x00U) {
				g_CANSBC_FaultStatus.DEV_STAT = g_CANSBC_RegRead._0x43.field._DEV_STAT;
			}
			if (g_CANSBC_FaultStatus.WD_FAIL == 0x00U) {
				g_CANSBC_FaultStatus.WD_FAIL = g_CANSBC_RegRead._0x43.field.WD_FAIL;
			}
			if (g_CANSBC_FaultStatus.SPI_FAIL == 0x00U) {
				g_CANSBC_FaultStatus.SPI_FAIL = g_CANSBC_RegRead._0x43.field.SPI_FAIL;
			}
			if (g_CANSBC_FaultStatus.FAILURE == 0x00U) {
				g_CANSBC_FaultStatus.FAILURE = g_CANSBC_RegRead._0x43.field.FAILURE;
			}
			break;


		case 0x44U:
			g_CANSBC_RegRead._0x44.data = miso.field.data;
			if (g_CANSBC_FaultStatus.CAN_FAIL == 0x00U) {
				g_CANSBC_FaultStatus.CAN_FAIL = g_CANSBC_RegRead._0x44.field.CAN_FAIL;
			}
			if (g_CANSBC_FaultStatus.VCAN_UV == 0x00U) {
				g_CANSBC_FaultStatus.VCAN_UV = g_CANSBC_RegRead._0x44.field.VCAN_UV;
			}
			break;
		
		case 0x46U:
			g_CANSBC_RegRead._0x46.data = miso.field.data;
			if (g_CANSBC_Status.CAN_WU == 0x00U) {
				g_CANSBC_Status.CAN_WU = g_CANSBC_RegRead._0x46.field.CAN_WU;
			}
			if (g_CANSBC_Status.TIMER_WU == 0x00U) {
				g_CANSBC_Status.TIMER_WU = g_CANSBC_RegRead._0x46.field.TIMER_WU;
			}
			if (g_CANSBC_Status.WK_WU == 0x00U) {
				g_CANSBC_Status.WK_WU = g_CANSBC_RegRead._0x46.field.WK_WU;
			}
			break;
		
		case 0x47U:
			g_CANSBC_RegRead._0x47.data = miso.field.data;
			if (g_CANSBC_Status.GPIO_WK_WU == 0x00U) {
				g_CANSBC_Status.GPIO_WK_WU = g_CANSBC_RegRead._0x47.field.GPIO_WK_WU;
			}
			break;
		
		case 0x48U:
			g_CANSBC_RegRead._0x48.data = miso.field.data;
			if (g_CANSBC_Status.SBC_DEV_LVL == 0x00U) {
				g_CANSBC_Status.SBC_DEV_LVL = g_CANSBC_RegRead._0x48.field.SBC_DEV_LVL;
			}
			if (g_CANSBC_Status.CFG0_STATE == 0x00U) {
				g_CANSBC_Status.CFG0_STATE = g_CANSBC_RegRead._0x48.field.CFG0_STATE;
			}
			if (g_CANSBC_Status.GPIO_LVL == 0x00U) {
				g_CANSBC_Status.GPIO_LVL = g_CANSBC_RegRead._0x48.field.GPIO_LVL;
			}
			if (g_CANSBC_Status.WK_LVL == 0x00U) {
				g_CANSBC_Status.WK_LVL = g_CANSBC_RegRead._0x48.field.WK_LVL;
			}
			break;
		
		case 0x54U:
			g_CANSBC_RegRead._0x54.data = miso.field.data;
			if (g_CANSBC_FaultStatus.GPIO_OC == 0x00U) {
				g_CANSBC_FaultStatus.GPIO_OC = g_CANSBC_RegRead._0x54.field.GPIO_OC;
			}
			break;
		
		case 0x55U:
			g_CANSBC_RegRead._0x55.data = miso.field.data;
			if (g_CANSBC_FaultStatus.GPIO_OL == 0x00U) {
				g_CANSBC_FaultStatus.GPIO_OL = g_CANSBC_RegRead._0x55.field.GPIO_OL;
			}
			break;
		case 0x7EU:
			g_CANSBC_RegRead._0x7E.data = miso.field.data;
			break;

	}

}


uint8 TLE9461Init(uint8 flag)
{
	uint8 step = 0, state = 0;
	
	step = g_MAIN.TLE9461_InitStep;

	if(flag == TRUE){
		state = flag;
	}
	else{
		switch(step)
		{
			case 0:
				CheckCANSBCRegStatus(SPI_CANSBC_INIT);
				step++;
				break;
			case 1:
				if(g_CANSBC_RegRead._0x01.field.MODE == 0x00U)
				{
					step++;
					state = TRUE;
				}
				break;
			default:
				step = 0;
				break;
		}
		
	g_MAIN.TLE9461_InitStep = step;
	return state;
	}
}
void EpbCANSBCTest(void)
{
	switch (g_CANSBC_RegWrite.TestFlag) {
		case 0:
			break;
		case 1:
			CANSBC_ReadWrite(CANSBCSetMsg_0x01(MODE_GO_NORMAL));
			g_CANSBC_RegWrite.TestFlag= 0x0U;
			break;
		case 2:

			CANSBC_ReadWrite(CANSBCSetMsg_0x01(MODE_GO_SLEEP));
			g_CANSBC_RegWrite.TestFlag= 0x0U;
			break;
		case 3:
			CANSBC_ReadWrite(CANSBCSetMsg_0x01(MODE_GO_STOP));
			g_CANSBC_RegWrite.TestFlag= 0x0U;
			break;
		case 4:
		//	PORT_SUB_CAN1_MODE = TRUE;
			g_CANSBC_RegWrite.TestFlag= 0x0U;
			break;
		case 5:
		//	PORT_SUB_CAN1_MODE = FALSE;
			g_CANSBC_RegWrite.TestFlag= 0x0U;
			break;
		default:

			break;
	}
}


