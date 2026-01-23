#include "Spi_ExtEEPROM_Api.h"
#include "Dspi_Api.h"
#include "ElectronicParkBrake.h"
#include "Spi_L9369Data_Types.h"
#include "APP_Pub.h"
#include "Can_Api.h"
#include "DEM_Priv.h"
#include "Spi_TLE9461_Data_Types.h"



extern DW_ElectronicParkBrake_T ElectronicParkBrake_DW;
extern L9369_Status_t g_L9369_Status;
extern Status_of_DTC DTC_Status[DEM_DTC_NUMBER];
extern tDtcLocalStatus m_astDtcLocalStatus[DEM_DTC_NUMBER];
extern CANSBC_RegVar_t g_CANSBC_RegRead;
extern CANSBC_FaultStatus_t g_CANSBC_FaultStatus;

ExtEEPROM_t g_ExtEEPROM;
void Delayms(uint32_t time)
{
	uint32_t ms = 0U;
	
	ms = time * 6400 * 2;
	while(ms){
		--ms;
	}
}
uint16_t g_liy[5] = {0,};
void ApplicationStoredDataRead(void)
{
	uint16_t temp;
	//1 OpMode value read from EEPROM
	temp = SPI_EEP_Read_X16(RxOpMode_Add);
	if(temp > 1)
	{
		SSMInput.SSMInEPB_OpMode = 0;						// Default Full mode setting
	}
	else
	{
		SSMInput.SSMInEPB_OpMode = temp;
	}
	Delayms(1);
	temp = SPI_EEP_Read_X16(EpbStatusLeft_Add);
	/* 
	PbcInDataStorageEpbStatus
	0 : Unknown
	1 : Applied
	2 : Reserved
	3 : Released

	TestEpbStatus
	0x00 : neutral, 0x01: released, 0x02: applied,
	0x03: releasing, 0x04: applying
	*/

	if(temp == 1)
	{
		PbcInDataStorageEpbStatusLeft = 0x01U;
		g_L9369_Status.EpbStatusLeft = 0x02U;
		//Default : Park Applied 
		//Extended : Applied
	}
	else if(temp == 2)
	{
		PbcInDataStorageEpbStatusLeft = 0x02U;
		g_L9369_Status.EpbStatusLeft = 0x02U;
		//Default : Emergency Applied 
		//Extended : Applied
	}
	else 
	{
		PbcInDataStorageEpbStatusLeft = 0x03U;
		g_L9369_Status.EpbStatusLeft = 0x01U;
		//Default : Released 
		//Extended : Released
	}
	Delayms(1);
	temp = SPI_EEP_Read_X16(EpbStatusRight_Add);
	g_liy[3] = temp;

	if(temp == 1)
	{
		PbcInDataStorageEpbStatusRight= 0x01U;
		g_L9369_Status.EpbStatusRight = 0x02U;
		//Default : Park Applied 
		//Extended : Applied
	}
	else if (temp == 2)
	{
		PbcInDataStorageEpbStatusRight= 0x02U;
		g_L9369_Status.EpbStatusRight = 0x02U;
		//Default : Emergency Applied 
		//Extended : Applied
	}
	else 
	{
		PbcInDataStorageEpbStatusRight = 0x03U;
		g_L9369_Status.EpbStatusRight = 0x01U;
		//Default : Released 
		//Extended : Released
	}
	Delayms(1);
	g_liy[0] = SPI_EEP_Read_X16(EEPROM_TEST_1);

	Delayms(1);
	g_liy[1] = SPI_EEP_Read_X16(EEPROM_TEST_2);

	

}

void EepromMemorySizeCheck(void)
{										
	SPI_EEP_Send_X16(TatooEeprom_Add,0x55aa,EWEN_CMD);
	Delayms(1);
	g_ExtEEPROM.EEP_ReadData[0]= SPI_EEP_Read_X16(TatooEeprom_Add);
	Delayms(1);
	if(g_ExtEEPROM.EEP_ReadData[0]!= 0x55aa){
		g_ExtEEPROM.Eeprom_BitSize = EEPROM_BITSIZE_9;
	}
}

/********************************************************************************
* Function Name		: eeprom read write
* Description			:    config
* input						: None
* return					: None
********************************************************************************/

void EepromReadAll(void)
{
	uword	Temp_Read;
	uint16_t *WriteEnd_Address;
	ubyte	x,i,temp;
	
	/* EEPROM Memory Size check */
	EepromMemorySizeCheck();

	/* Read the stored Data depend on application */
	ApplicationStoredDataRead();

	
	//1 Stored DTC data read from external EEPROM
	for(i=0;i < (DEM_DTC_NUMBER/2);i+=2)
	{
		Temp_Read = SPI_EEP_Read_X16(DTC_StoreAddOffset+i);
		Delayms(1);
		temp = (ubyte)Temp_Read;
		if(temp != 0xFF)
		{
			DTC_Status[i].Flag.Bit.Indicator = ((temp)>>1);
			DTC_Status[i].Healing_counter = ((temp)>>2);
		}

		temp = (ubyte)(Temp_Read >>8);
		if(temp != 0xFF)
		{ 
		
			DTC_Status[i+1].Flag.Bit.Indicator = ((temp)>>1);
			DTC_Status[i+1].Healing_counter = ((temp)>>2);
		}

	}

	//1  DTC_Status Initialize
	for(i=0;i < DEM_DTC_NUMBER;i++)
	{
		DTC_Status[i].Flag.Bit.NotCompleted = 1;

		if(Temporary_Failure_Check(i)==1) //Temporary
		{
			DTC_Status[i].Flag.Bit.Failed = 0;
			DTC_Status[i].Flag.Bit.FailedCycle = 0;
		}
		else //Permanent
		{
			if(DTC_Status[i].Flag.Bit.Indicator == 1)
			{
				
				DTC_Status[i].Flag.Bit.Failed = 1;
				DTC_Status[i].Flag.Bit.Confirmed = 1;
				DTC_Status[i].Flag.Bit.FailedCycle = 1;
				DTC_Status[i].Flag.Bit.Indicator = 1;
				DTC_Status[i].Healing_counter = 40;
			}
		}
		if(DTC_Status[i].Flag.Bit.Indicator == 1)
		{
			//IL_BoolWr_EPB2_FailureStatus(0x01); 
			
			switch(i)
			{	
			
				#if 0
				case DEM_DTC_IDX_DTC_ACTUATORSTUCK: 	
					DiagInput.D20_EPB_Stuck= 1;
				//case DEM_DTC_IDX_DTC_NOLOADCABLDISCON:	
				break;
				
				case DEM_DTC_IDX_DTC_MTROVERCURRENTL:	//4 D11 ( 1time = temporary, 2time = permanent) 
					DiagInput.D11_MtrCurOvA= 1;
				break;
				case DEM_DTC_IDX_DTC_MTRUNDERCURRENTL:	
					DiagInput.D11_MtrCurUvA= 1;
				break;
				case DEM_DTC_IDX_DTC_MTROVERCURRENTR:	//4 D11 ( 1time = temporary, 2time = permanent) 
					DiagInput.D11_MtrCurOvB= 1;
				break;
				case DEM_DTC_IDX_DTC_MTRUNDERCURRENTR:						
					DiagInput.D11_MtrCurUvB= 1;
				break;
				#endif
				
				default:
				break;
			}
			
		}
		if(DTC_Status[i].Healing_counter > 0)
		{
			DTC_Status[i].Flag.Bit.Confirmed =1;
		}

	
		m_astDtcLocalStatus[i].ubDtcStatusMask = DTC_Status[i].Flag.Data;

	}

}

void SaveParamData(void)
{
	g_ExtEEPROM.SaveParamCount++;
	if ((g_ExtEEPROM.SaveParamCount % 2U)== 0U) { /*10ms*/ 
		if(++g_ExtEEPROM.SaveWritingStep > 8){
			g_ExtEEPROM.SaveWritingStep = 10U;
		}
		else{
		switch(g_ExtEEPROM.SaveWritingStep)
		{
			case 0:
				SPI_EEP_Send_X16(EpbStatusLeft_Add,g_L9369_Status.EpbStatusLeft,EWEN_CMD);
				break;
			case 1:
				SPI_EEP_Send_X16(EpbStatusRight_Add,g_L9369_Status.EpbStatusRight,EWEN_CMD);
				break;
			case 2:
				SPI_EEP_Send_X16(EEPROM_TEST_1,g_CANSBC_RegRead._0x40.field.VCC1_WARN,EWEN_CMD);
				break;
			case 3:
				SPI_EEP_Send_X16(EEPROM_TEST_2,g_CANSBC_FaultStatus.SPI_Status.data,EWEN_CMD);
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			default:
				g_ExtEEPROM.SaveWritingStep = 10U;
				break;
			Delayms(1);
		}
		}
	}
}

void ExtEEPROMInit(void)
{
	g_ExtEEPROM.Eeprom_BitSize = EEPROM_BITSIZE_11;	
}

