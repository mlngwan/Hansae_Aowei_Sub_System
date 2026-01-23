#include "Adc_Api.h"
#include "Can_Api.h"
#include "ElectronicParkBrake.h"
#include "Spi_ExtEEPROM_Api.h"
#include "Dspi_Api.h"
#include "board.h"
#include "EB_Prj.h"
#include "Spi_L9369Data_Types.h"

Can_t g_CAN;
Uds_t g_UDS;
extern ADC_t g_ADC;
Board_t g_BOARD;
SaveDtcType	stSaveDTC;
Status_of_CAN_DTC DTC_CAN_Status[DTC_CAN_NUM];
Status_of_DTC DTC_Status[DEM_DTC_NUMBER];
static Data_Packet RxBuffer[10];
extern ExtY_ElectronicParkBrake_T ElectronicParkBrake_Y;
extern tDtcLocalStatus m_astDtcLocalStatus[DEM_DTC_NUMBER];
extern ubyte pass_stack[5][14];
extern DiagFlagBus DiagInput;
extern SSMInputBus SSMInput;
extern L9369_Status_t g_L9369_Status;


void iEHB_Message(void)
{
	if(g_CAN.fbRxInd_IEHB)
	{
		g_CAN.Cnt_Fail_IEHB = 0U;
		g_CAN.fbRxInd_IEHB = FALSE;
		//DTC_CAN_Missing(DTC_IEHB, DTC_PASS,DEM_DTC_IDX_DTC_MISSINGMESSAGEVCU);

		//g_CAN.Func_Result = IL_u8Rd_VCUMessageCounter((u8 *)&g_CAN.Rx_U8);
		//SSMInput.SSMInSwitch= (uint8_T)g_CAN.Rx_U8;
		
	}
	else
	{
		if(g_CAN.Cnt_Fail_IEHB > 9)	//11 -> 16
		{
			//g_CAN.Cnt_Fail_IEHB = 0;
			//DTC_CAN_Missing(DTC_IEHB, DTC_FAIL, DEM_DTC_IDX_DTC_MISSINGMESSAGEVCU);
		}
	}

}

void Tx_Can_100ms(void)
{
	//IL_BoolWr_EPBMotorAEcho();
	//IL_BoolWr_EPBMotorBEcho(g_EPB_St.Can_Tx.Can_EchoB);
	
	IL_u8Wr_EPBMotorAstate(g_L9369_Status.EpbStatusLeft);
	IL_u8Wr_EPBMotorAindication((g_L9369_Status.fMaxApplied_Left  == 0x01U)? 0x1U:0x0U);
	IL_u8Wr_EPBMotorBstate(g_L9369_Status.EpbStatusRight);
	IL_u8Wr_EPBMotorBindication((g_L9369_Status.fMaxApplied_Right  == 0x01U)? 0x1U:0x0U);
	IL_BoolWr_EPBMotorAovercurrent(DiagInput.D11_MtrCurOvA);
	IL_BoolWr_EPBMotorAundercurrent(DiagInput.D11_MtrCurUvA);
	
	IL_BoolWr_EPBMotorBovercurrent(DiagInput.D11_MtrCurOvB);
	IL_BoolWr_EPBMotorBundercurrent(DiagInput.D11_MtrCurUvB);
	
	IL_BoolWr_EPBRxError(ElectronicParkBrake_Y.SSMOutputCAN.EPB_FailureSts);

	IL_u8Wr_EPBBoardvoltage((uint8)(g_ADC.Physical.Power.UBB/100.0));
	IL_u16Wr_EPBMotorAcurrent((uint16)(g_ADC.Physical.Motor.MOTORA_CUR/100.0));
	IL_u8Wr_EPBMotorAvoltage((uint8)(g_ADC.Physical.Motor.MOTORA/100.0));
	IL_u16Wr_EPBMotorBcurrent((uint16)(g_ADC.Physical.Motor.MOTORB_CUR/100.0));
	IL_u8Wr_EPBMotorBvoltage((uint8)(g_ADC.Physical.Motor.MOTORB/100.0));
}

void CanInitialize(void)
{
	APP_Init();
	g_UDS.gnRxFrameFlag = COM_TRUE;
}
/*	20msec	*/
void EcuResetCheck(void)
{
	uint32_t ReadData = 0,WriteData = 0;

	if(g_UDS.fECU_Reset_Bit)
	{
		g_UDS.Cnt_ResetDelay++;
		if(g_UDS.Cnt_ResetDelay > 250) //5s 
		{
			g_UDS.Cnt_ResetDelay = 1000;

			if(!g_UDS.fFingerPrintWrote)
			{		
				APP_InterruptDisable();
				BoardPerformReset(PLATFORMS_SW_RESET);
			}
		}
		else
		{
			switch(g_UDS.BackupStep)
			{
				case 0:
					if(g_UDS.fBootWriteRequest)
					{
						WriteData = g_UDS.BootWriteAdd[0];
						WriteData = WriteData << 8;
						WriteData |= g_UDS.BootWriteAdd[1];
						SPI_EEP_Send_X16(EEP_BootWriteAdd,WriteData,EWEN_CMD);
						g_UDS.BackupStep = 1;
					}
					else
					{
						g_UDS.BackupStep = 2;
					}
				break;
				case 1:
					if(g_UDS.fBootWriteRequest)
					{
						WriteData = g_UDS.BootWriteAdd[2];
						WriteData = WriteData << 8;
						WriteData |= g_UDS.BootWriteAdd[3];
						SPI_EEP_Send_X16((EEP_BootWriteAdd+1),WriteData,EWEN_CMD);
					}
					g_UDS.BackupStep = 2;
				break;
				case 2:
					if(g_UDS.fECU_Reset)
					{
						g_UDS.fECU_Reset = 0;
						SPI_EEP_Send_X16(UDS_ResetECU_CheckAdd,(0x5A00U | g_UDS.SPR_Req),EWEN_CMD);
					}
					g_UDS.BackupStep = 3;
				break;
				default:
					g_UDS.Cnt_ResetDelay = 1000;
				break;
			}
		
		}
	}
	else
	{
		g_UDS.Cnt_ResetDelay = 0;
		g_UDS.BackupStep = 0;
		if(!g_UDS.fECU_ResetErase)
		{
			g_UDS.fECU_ResetErase = 1;
			ReadData = SPI_EEP_Read_X16(UDS_ResetECU_CheckAdd);
			if(ReadData & 0x5A5AU)
			{
				g_UDS.DiagTxData[0] = 0x02;
				g_UDS.DiagTxData[1] = 0x51;
				g_UDS.DiagTxData[2] = 0x01;
				
				IL_u32Wr_Diag_From_EPB1R_Sig(g_UDS.DiagTxData);
				COM_SendFrameDirect(COM_FRAME_IDX_Diag_From_EPB1R);
				SPI_EEP_Send_X16(UDS_ResetECU_CheckAdd,0x00U,EWEN_CMD);
			}
		}
	}
}
void CAN_Task(void)
{
	CAN_AllPolling();
	APP_Manage();
	EB_Manage();
	ccpDaq(1);
}

void TxCanMessage(void)
{
	CAN_Task();
	Tx_Can_100ms();

}

