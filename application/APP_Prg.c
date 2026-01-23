/*********************************************************************************/
/*                                                                               */
/*                                  CAN Stack                                    */
/*                                                                               */
/* ----------------------------------------------------------------------------- */
/*                                                                               */
/*                                APP                                            */
/*                                                                               */
/*********************************************************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*%%   _____________________________   %%  \file APP_Prg.c                       */
/*%%  |                             |  %%                                        */
/*%%  |   &&&&&   &&&&&             |  %%  Module version: 3.6.4                 */
/*%%  |   &       &    &            |  %%                                        */
/*%%  |   &&&&    &&&&&             |  %%                                        */
/*%%  |   &       &    &            |  %%                                        */
/*%%  |   &&&&&   &&&&&             |  %%                                        */
/*%%  |_____________________________|  %%                                        */
/*%%                                   %%                                        */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*********************************************************************************/
/* Copyright 2010 by Elektrobit Automotive GmbH                                  */
/* All rights exclusively reserved for Elektrobit Automotive GmbH,               */
/* unless expressly agreed to otherwise.                                         */
/*********************************************************************************/

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 16.7 (advisory)
 *   A pointer parameter in a function prototype should be declared as pointer to const
 *   if the pointer is not used to modify the addressed object.
 *
 *   Reason:
 *   The MISRA rule is not violated. This pointer will be used once the function is completed by customer.
 */

/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/

/* - Header Files: EB                                                            */
#include "EB_Prj.h"
#include "Board.h"
#include "CCP.h"
#include "common.h"
#include "Adc_Api.h"				
#include "DEM_Priv.h"
#include "Diagnostics_CAN.h"
#include "Dspi_Api.h"
#include "Spi_ExtEEPROM_Api.h"
#include "Can_Api.h"
#include "ElectronicParkBrake.h"

/*                                                                               */
/*********************************************************************************/

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */
//NCS_PRI_VAR __attribute__((section(".noinit"))) u32 eBootFlag;
static uint8 Cnt_InvalidDLC[COM_RX_FRAME_MAX] = {3,3,3,3};

extern Status_of_CAN_DTC DTC_CAN_Status[DTC_CAN_NUM];
extern Status_of_DTC DTC_Status[DEM_DTC_NUMBER];
extern DiagFlagBus DiagInput;
extern SSMInputBus SSMInput;   
extern Can_t g_CAN;
extern Uds_t g_UDS;
extern SaveDtcType stSaveDTC;
extern ExtY_ElectronicParkBrake_T ElectronicParkBrake_Y;

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

/*                                                                               */
/*********************************************************************************/

/***************************** PRIVATE DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Private Constants                                                           */


#define DOWNLOAD_REQUESTED                  0xA55AU

#define NO_OPERATION           					0U   /* no program/erase operation */
#define SA_NOERR 								0U
#define SA_ERR_UNKNOWN							1U
#define SA_ERR_KGRE_UnspecifiedError			2U
#define SA_ERR_BufferToSmall 					3U


#define PROG_SA_NOT_STARTED         		    0x00U
#define PROG_SA_SEED_SENT            			0x01U

#define PROG_SECURITY_STATE_LOCK    			0U
#define PROG_SECURITY_STATE_UNLOCK 				1U

#define PROG_SECURITY_INITIALE_STATE   PROG_SECURITY_STATE_LOCK

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/*-------------------------------------------------------------------------------*/
/* - Private Variables  */

/* Interrupt context */
NCS_PRI_VAR u8 m_ubItContext;

/* Value of the Ecu key calculated
   Range : (0 .. 2^32)                 */
NCS_PRI_VAR u32 m_ulEcuKey;

typedef u8 tProgSecurityAccessState;
typedef u8 tSecurityState;

/* Security access management state */
tProgSecurityAccessState m_ubSecurityAccessState = 0;
/* security state flag */
tSecurityState m_eSecurityState = 0;

extern void Delayms(uint32_t time);

/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

/*                                                                               */
/*********************************************************************************/

/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */

#include <board.h>

/*--------------------------{NAMI GenerateKeyEx}--------------------------------------*/
u32 GenerateKeyEx(const unsigned char * ipSeedArray, unsigned int iSeedArraySize,
   								unsigned char * iopKeyArray, unsigned int * oActualKeyArraySize)
{	
	//Algorithm coefficients
	#define A_COEFF_BIT_LOCATION 30
	#define B_COEFF_BIT_LOCATION 3
	#define C_COEFF_BIT_LOCATION 7
	#define D_COEFF_BIT_LOCATION 6
	#define E_COEFF_BIT_LOCATION 9
	#define F_COEFF_BIT_LOCATION 11
	#define G_COEFF_BIT_LOCATION 16
	#define H_COEFF_BIT_LOCATION 18
	#define J_COEFF_BIT_LOCATION 4

	//Bit inversion operation specification
	#define	BIT_INV_A_1	6
	#define	BIT_INV_A_2	1
	#define	BIT_INV_A_3	31
	#define	BIT_INV_B_1	8
	#define	BIT_INV_B_2	3
	#define	BIT_INV_B_3	5
	#define	BIT_INV_C_1	28
	#define	BIT_INV_C_2	18
	#define	BIT_INV_C_3	24
	
	//Bit inversion selection
	#define NO_INVERSION	0
	#define BIT_INVERSION_A	1
	#define BIT_INVERSION_B	2
	#define BIT_INVERSION_C	3

	//Bitwise operation
	#define SEED_AND_RESULT	1
	#define SEED_XOR_RESULT	2
	#define SEED_OR_RESULT	3

	unsigned char a_coeff = 0;	//Bit-shift direction
	unsigned char b_coeff = 0;	//Number of overlapping bit-shifts MSb
	unsigned char c_coeff = 0;	//Number of overlapping bit-shifts
	unsigned char d_coeff = 0;	//Number of overlapping bit-shifts
	unsigned char e_coeff = 0;	//Number of overlapping bit-shifts LSb
	unsigned char f_coeff = 0;	//Bit inversion selection MSb
	unsigned char g_coeff = 0;	//Bit inversion selection LSb
	unsigned char h_coeff = 0;	//Bitwise operation MSb
	unsigned char j_coeff = 0;	//Bitwise operation LSb

	unsigned char numOverlappingBitShifts = 0;
	unsigned char bitInversionRule = 0;
	unsigned char bitwiseOperation = 0;

	unsigned int Seed = 0;
	unsigned int Result = 0;
	unsigned int Key = 0;
	unsigned char i = 0;

	//Expect a seed of 4 bytes
	if ( !(iSeedArraySize==4) )
	{
		return SA_ERR_BufferToSmall;
	}
	else
	{		
		Seed = (unsigned int)((ipSeedArray[0] << 24) | (ipSeedArray[1] << 16) | (ipSeedArray[2] << 8) | (ipSeedArray[3]));
		Result = Seed;

		//Extract the coefficients
		a_coeff = (unsigned char)((Seed >> A_COEFF_BIT_LOCATION) & 0x01);
		b_coeff = (unsigned char)((Seed >> B_COEFF_BIT_LOCATION) & 0x01);
		c_coeff = (unsigned char)((Seed >> C_COEFF_BIT_LOCATION) & 0x01);
		d_coeff = (unsigned char)((Seed >> D_COEFF_BIT_LOCATION) & 0x01);
		e_coeff = (unsigned char)((Seed >> E_COEFF_BIT_LOCATION) & 0x01);
		f_coeff = (unsigned char)((Seed >> F_COEFF_BIT_LOCATION) & 0x01);
		g_coeff = (unsigned char)((Seed >> G_COEFF_BIT_LOCATION) & 0x01);
		h_coeff = (unsigned char)((Seed >> H_COEFF_BIT_LOCATION) & 0x01);
		j_coeff = (unsigned char)((Seed >> J_COEFF_BIT_LOCATION) & 0x01);

		numOverlappingBitShifts = (unsigned char)((b_coeff << 3) | (c_coeff << 2) | (d_coeff << 1) | e_coeff);
		bitInversionRule = (unsigned char)((f_coeff << 1) | g_coeff);
		bitwiseOperation = (unsigned char)((h_coeff << 1) | j_coeff);

		//a coefficient controls the shift direction
		if (!a_coeff)
		{
			//Shift direction = left
			for (i = 0; i < numOverlappingBitShifts; i++)
			{
				Result = (unsigned int)((Result << 1) | (Result >> 31));
			}
		}
		else
		{
			//Shift direction = right
			for (i = 0; i < numOverlappingBitShifts; i++)
			{
				Result = (unsigned int)((Result << 31) | (Result >> 1));
			}
		}
		
		switch (bitInversionRule)
		{
			case NO_INVERSION:
				//Do nothing
				break;

			case BIT_INVERSION_A:
				Result ^= (unsigned int)(1 << BIT_INV_A_1);
				Result ^= (unsigned int)(1 << BIT_INV_A_2);
				Result ^= (unsigned int)(1 << BIT_INV_A_3);
				break;

			case BIT_INVERSION_B:
				Result ^= (unsigned int)(1 << BIT_INV_B_1);
				Result ^= (unsigned int)(1 << BIT_INV_B_2);
				Result ^= (unsigned int)(1 << BIT_INV_B_3);
				break;

			case BIT_INVERSION_C:
				Result ^= (unsigned int)(1 << BIT_INV_C_1);
				Result ^= (unsigned int)(1 << BIT_INV_C_2);
				Result ^= (unsigned int)(1 << BIT_INV_C_3);
				break;

			default:
				return SA_ERR_KGRE_UnspecifiedError;
		}

		switch(bitwiseOperation)
		{
			case NO_OPERATION:
				Key = Result;
				break;
			case SEED_AND_RESULT:
				Key = (unsigned int)(Seed & Result);
				break;
			case SEED_XOR_RESULT:
				Key = (unsigned int)(Seed ^ Result);
				break;
			case SEED_OR_RESULT:
				Key = (unsigned int)(Seed | Result);
				break;			

			default:
				return SA_ERR_KGRE_UnspecifiedError;
		}
	}

	iopKeyArray[0] = (unsigned char)(Key >> 24);
	iopKeyArray[1] = (unsigned char)(Key >> 16);
	iopKeyArray[2] = (unsigned char)(Key >> 8);
	iopKeyArray[3] = (unsigned char)Key;
	*oActualKeyArraySize = 4;

	
	return SA_NOERR;
}
/*--------------------------{NAMI GenerateKeyEx}--------------------------------------*/

/*--------------------------{PROG_SeedGeneration}--------------------------------------*/
u32 PROG_SeedGeneration(u16 uwTime)
{
    OSC_PARAM_UNUSED(uwTime);
	uwTime = BoardGetRandom();
    return uwTime;
}
/*--------------------------{end PROG_SeedGeneration}----------------------------------*/

/*--------------------------{PROG_CalculateUceKey}--------------------------------------*/
u32 PROG_CalculateUceKey(u32 ulSeedGenerated)
{
	u32 ulCalculatedKey = 0;
	u32 ulKeySize = 0;
	u8 uRet = SA_ERR_UNKNOWN;
	u32 iSeedArraySize = 4;
	uRet = GenerateKeyEx((const unsigned char *)&ulSeedGenerated,(unsigned int)iSeedArraySize,(unsigned char *)&ulCalculatedKey,(unsigned int *)&ulKeySize);
	if(uRet != SA_NOERR)
	{
		ulCalculatedKey = 0;
	}
    return (ulCalculatedKey);
}
/*--------------------------{end PROG_CalculateUceKey}----------------------------------*/

/*----------------------------------{PROG_Sa1Managment}--------------------------------*/
tUdsStatus PROG_Sa1Managment (u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eStatus;
    u32 ulSeedGenerated;

    eStatus = UDS_ACK;

    /* Implements : 000-07-D028-D-91503-1
       Implements : 000-07-D028-D-91504-1
       Generate the SEED value */
    ulSeedGenerated = PROG_SeedGeneration(0U);

    /* Implements : 000-07-D028-D-91505-1
       Calculate the UCE key */
    m_ulEcuKey = PROG_CalculateUceKey(ulSeedGenerated);

    /* Fill the answer */
    aubUdsData[2] = (u8) (ulSeedGenerated >> 24);
    aubUdsData[3] = (u8) (ulSeedGenerated >> 16);
    aubUdsData[4] = (u8) (ulSeedGenerated >> 8);
    aubUdsData[5] = (u8) ulSeedGenerated;
    *puwLen = 6;

    return eStatus;
}
/*----------------------------------{end PROG_Sa1Managment}----------------------------*/


/*----------------------------------{PROG_Sa2Managment}--------------------------------*/
tUdsStatus PROG_Sa2Managment (u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eStatus;
    u32 ulKeyReceived;

    eStatus = UDS_ACK;

    /* Recover the Key value received */
    ulKeyReceived = (u32)(((u32)(aubUdsData[2]) << 24U)
                     + ((u32)(aubUdsData[3]) << 16U)
                     + ((u32)(aubUdsData[4]) << 8U)
                     + (u32)(aubUdsData[5]));

    if(ulKeyReceived == m_ulEcuKey)
    {
        eStatus = UDS_ACK;
        *puwLen = 2U;
    }
    else
    {
        eStatus = UDS_NRC_35;
    }

    return eStatus;
}
/*----------------------------------{end PROG_Sa2Managment}----------------------------*/

/*----------------------------------{PROG_OpenProgrammingSession}----------------*/
void PROG_OpenProgrammingSession(void)
{
  /* Initialize the different flags of the PROG layer */
  m_ubSecurityAccessState = PROG_SA_NOT_STARTED;
  m_eSecurityState = PROG_SECURITY_STATE_LOCK;
	g_UDS.fAppInit_Mutex = 1;			
}
/*----------------------------------{end PROG_OpenProgrammingSession}------------*/

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */

/*--------------------------{APP_InterruptEnable}--------------------------------------*/
void APP_InterruptEnable (u8 ebStatus)
{

    /* Implement here a treatment for a critical section exit. */
			
//		CAN_InterruptDisable();			
    /* Interrupts must be resumed. */
    m_ubItContext = ebStatus;
}
/*--------------------------{end APP_InterruptEnable}----------------------------------*/

/*--------------------------{APP_InterruptDisable}--------------------------------------*/
u8 APP_InterruptDisable (void)
{

    /* Implement here a treatment for a critical section entry. */
    /* Interrupts must be suspended. */
    return (m_ubItContext);
}
/*--------------------------{end APP_InterruptDisable}----------------------------------*/
#define NM_COUNTER_MAX 170U
#define NM_COUNTER_MIN 0U

ubyte	ubMissMessageTimer[COM_MON_NODE_MAX];
/*--------------------------{APP_Init}--------------------------------------*/
void APP_Init (void)
{
	u8 i;
	EB_Init();

	/* Activation of periodic frames to be send */
	/* Start ABSENT frames monitoring */
	(void)COM_Periodic(COM_PERIODIC_START);

	for (i = 0; i < COM_MON_NODE_MAX; i++)
	{
		ubMissMessageTimer[i] = NM_COUNTER_MAX;
	}
	
	DEM_StartOperatingCycle();

}
/*--------------------------{end APP_Init}----------------------------------*/

/*--------------------------{APP_RestoreContext}--------------------------------------*/
void APP_RestoreContext (void)
{
    /* After asynchronous reload from the non volatile memory, */
    /* reactivate the context into the related stack layers. */
}
/*--------------------------{end APP_RestoreContext}----------------------------------*/




/*--------------------------{APP_Manage}--------------------------------------*/
void APP_Manage (void)
{
}
/*--------------------------{end APP_Manage}----------------------------------*/




/*--------------------------{APP_ComNAsInd}--------------------------------------*/
void APP_ComNAsInd (tComFrameIdx uFrameIdx)
{
    OSC_PARAM_UNUSED(uFrameIdx);
}
/*--------------------------{end APP_ComNAsInd}----------------------------------*/

/*--------------------------{APP_ComTxTimeoutInd}--------------------------------------*/
tComStatus APP_ComTxTimeoutInd (tComFrameIdx uFrameIdx)
{
    OSC_PARAM_UNUSED(uFrameIdx);

    return COM_ERR_OK;
}
/*--------------------------{end APP_ComTxTimeoutInd}----------------------------------*/

/*--------------------------{APP_ComTxConf}--------------------------------------*/
void APP_ComTxConf (tComFrameIdx uFrameIdx)
{
#if 0
 	if (uFrameIdx == COM_FRAME_IDX_CCP_DTO)
	{
		ccpSendCallBack();
	}
 	else if(uFrameIdx == COM_FRAME_IDX_EPB1R_01_Info)
	{
		if(++EPB_RC > 0x0F) EPB_RC = 0;
	//	m_aaubComFrameData[COM_FRAME_IDX_EPB1L_01_Info][3] = (u8)((u8)(EPB_RC<<4U)|0x0fU) 
//  & (m_aaubComFrameData[COM_FRAME_IDX_EPB1L_01_Info][3] | (u8)((u8)(EPB_RC<<4U)&0xf0U));
	}
#endif
	//OSC_PARAM_UNUSED(uFrameIdx);
}
/*--------------------------{end APP_ComTxConf}----------------------------------*/
 Flag_Type	 ubCanRxFrameInd1,ubCanRxFrameInd2; 

u8 OnRxFrameiEHB(tComFrameIdx uFrameIdx)
{
	g_CAN.fbRxInd_IEHB = TRUE;
	return COM_RX_IND_CHECK_OK;

}
u8 OnRxFrameVCUCMD(tComFrameIdx uFrameIdx)
{
	 return COM_RX_IND_CHECK_OK;
}

/*--------------------------{APP_ComModeModifyExtension}--------------------------------------*/
void APP_ComModeModifyExtension (tComMode eCurMode)
{
    OSC_PARAM_UNUSED(eCurMode);
}
/*--------------------------{end APP_ComModeModifyExtension}----------------------------------*/

/*--------------------------{APP_ComTxFrameInd}--------------------------------------*/
void APP_ComTxFrameInd (tComFrameIdx uFrameIdx)
{
    OSC_PARAM_UNUSED(uFrameIdx);
}
/*--------------------------{end APP_ComTxFrameInd}----------------------------------*/



/*--------------------------{APP_ComBackInd}--------------------------------------*/
void APP_ComBackInd (tComMonNodeIdx uNodeIdx)
{
	ubMissMessageTimer[uNodeIdx] = NM_COUNTER_MAX;
}
/*--------------------------{end APP_ComBackInd}----------------------------------*/

/*--------------------------{APP_ComLossInd}--------------------------------------*/
void APP_ComLossInd (tComMonNodeIdx uNodeIdx)
{
	if(ubMissMessageTimer[uNodeIdx] > 1)
	{
		ubMissMessageTimer[uNodeIdx]--;
	}
}
/*--------------------------{end APP_ComLossInd}----------------------------------*/

/*--------------------------{APP_COM_MonNodeStatusInd}--------------------------------------*/
void APP_COM_MonNodeStatusInd(tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus)
{
	u8 ubIdx;
	u8 ubDtcIdx = 0xFFU;
	//u8 aubContext[DEM_CONTEXT_SIZE];
	switch (uNodeIdx)
	{
		case COM_NODE_IDX_MissingMessageVCU:
			ubDtcIdx = DEM_DTC_IDX_DTC_MISSINGMESSAGEVCU;
		break;
		
		default:break;
	}

}
/*--------------------------{end APP_COM_MonNodeStatusInd}----------------------------------*/

/*--------------------------{UDS_Default_Session}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus UDS_Default_Session(u16 *puwLen, u8 *aubUdsData)
{

    OSC_PARAM_UNUSED(puwLen);
    OSC_PARAM_UNUSED(aubUdsData);

    return UDS_ACK;
}
/*--------------------------{end UDS_Default_Session}----------------------------------*/

/*--------------------------{UDS_ProgrammingSession}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus UDS_ProgrammingSession(u16 *puwLen, u8 *aubUdsData)
{
  /* RQIM-UDS-002-1 */
  /* Firstly, check specific condition to jump in bootloader (if condition NOK, return NRC_22) */
  /* Then, set BootFlag to DOWNLOAD_REQUESTED */
  /* Request a reset */
  /* If needed (eg reset need time), deactivate response since it will be done in bootloader after jump */
  	OSC_PARAM_UNUSED(puwLen);
  	OSC_PARAM_UNUSED(aubUdsData);
	PROG_OpenProgrammingSession();
	if(aubUdsData[1] & 0x80)
	{
		g_UDS.SPR_Req = 0x80;
	}						   
	return UDS_ACK;
}
/*--------------------------{end UDS_ProgrammingSession}----------------------------------*/

/*--------------------------{UDS_Extended_Session}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus UDS_Extended_Session(u16 *puwLen, u8 *aubUdsData)
{
  u8 ubsession;

  OSC_PARAM_UNUSED(puwLen);
  OSC_PARAM_UNUSED(aubUdsData);
  ubsession = UDS_GetCurrentSession();
  
  if(ubsession == 2) // Programming Session
  {
	  return UDS_NRC_7E;
  }
  else
  {
	  PROG_OpenProgrammingSession();
	  return UDS_ACK;
  }

}
/*--------------------------{end UDS_Extended_Session}----------------------------------*/

/*--------------------------{UDS_ReportDtcByStatusMask}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus UDS_ReportDtcByStatusMask(u16 *puwLen, u8 *aubUdsData)
{
	u8	i,eUdsStatus,ubUdsData[150],udsRxData;
	u8 ubDtcIdx;

	
	/* Check the status mask of all DTC */
	for (ubDtcIdx = 0U; ubDtcIdx < DEM_DTC_MAX; ubDtcIdx++)
	{
		m_astDtcLocalStatus[ubDtcIdx].ubDtcStatusMask = DTC_Status[ubDtcIdx].Flag.Data;
		m_astDtcLocalStatus[ubDtcIdx].ubDtcStatusMask = m_astDtcLocalStatus[ubDtcIdx].ubDtcStatusMask & 0x09;

	}
	eUdsStatus = DEM_ReportDtcByStatusMask(puwLen,aubUdsData);
	*(aubUdsData+2) = 0x09;

  return eUdsStatus;
}

/*--------------------------{end UDS_ReportDtcByStatusMask}----------------------------------*/

/*--------------------------{EPB_TestMode}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus EPB_TestMode(u16 *puwLen, u8 *aubUdsData)
{

    OSC_PARAM_UNUSED(puwLen);
    OSC_PARAM_UNUSED(aubUdsData);

	if(!g_UDS.TestModeEnable_Flag)
	{
		g_UDS.TestModeEnable_Flag = 0xA5;
	}				 
    return UDS_ACK;
}
/*--------------------------{end EPB_TestMode}----------------------------------*/



/*--------------------------{CAN_BusOffInd}--------------------------------------*/
void CAN_BusOffInd (void)
{
    /* Set CAN HW to BusOff and unlock upper layer (TP) with CAN_TxFail function */
#if (CAN_CTRL_NB>1)
    CAN_BusOff(NM_CAN_BUS_IDX);

#else
    CAN_BusOff();
#endif

    /* Set COM to BUS_OFF: CAN is deactivated */
    (void) COM_ModeModify(COM_MODE_BUS_OFF);

    /* Set COM to BUS_ON: CAN is activated */
    (void) COM_ModeModify(COM_MODE_BUS_ON);

		if(g_CAN.CANBUS_firstcheck == 0)
		{
			g_CAN.CANBUS_firstcheck = 1;
			g_CAN.Cnt_CANBusOff = 0;
		}
		
		if(g_CAN.Cnt_CANBusOff > 199)
		{
			g_CAN.Cnt_CANBusOff = 200;
			g_CAN.Cnt_BusOn = 0;
			DiagInput.D21_CAN_BusOff = 1;
			/* Signal Defect confirmation */
			//DEM_DtcTestStatus(DEM_DTC_IDX_DTC_CAN_BUS_OFF, DEM_TEST_FAIL);
			g_CAN.CANBUS_fail = 1;
		}

		
    /* Set CAN HW to BusOn */
#if (CAN_CTRL_NB>1)
    CAN_BusOn(NM_CAN_BUS_IDX);
#else
    CAN_BusOn();
#endif
}
/*--------------------------{end CAN_BusOffInd}----------------------------------*/

/*--------------------------{UDS_P2AboutToExpireInd}--------------------------------------*/
void UDS_P2AboutToExpireInd(void)
{

}
/*--------------------------{end UDS_P2AboutToExpireInd}----------------------------------*/

/*--------------------------{APP_UdsSessionStatusInd}--------------------------------------*/
void APP_UdsSessionStatusInd (tUdsSessionType eUdsNewSessType,tUdsSessionType eUdsOldSessType,tUdsChangeReason eUdsChangingCause)
{
    OSC_PARAM_UNUSED(eUdsOldSessType);
    OSC_PARAM_UNUSED(eUdsChangingCause);

    OSC_PARAM_UNUSED(eUdsNewSessType);

}
/*--------------------------{end APP_UdsSessionStatusInd}----------------------------------*/

/*--------------------------{APP_TpDataBufferEmptyInd}--------------------------------------*/
void APP_TpDataBufferEmptyInd (tTpMsgIdx uMsgIdx)
{
    OSC_PARAM_UNUSED(uMsgIdx);
}
/*--------------------------{end APP_TpDataBufferEmptyInd}----------------------------------*/

/*--------------------------{APP_TpTxConf}--------------------------------------*/
void APP_TpTxConf (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
    OSC_PARAM_UNUSED(uMsgIdx);
    OSC_PARAM_UNUSED(ebStatus);
}
/*--------------------------{end APP_TpTxConf}----------------------------------*/

/*--------------------------{APP_TpRxInd}--------------------------------------*/
void APP_TpRxInd (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
    OSC_PARAM_UNUSED(uMsgIdx);
    OSC_PARAM_UNUSED(ebStatus);
}
/*--------------------------{end APP_TpRxInd}----------------------------------*/

/*--------------------------{APP_TpRxEnableInd}--------------------------------------*/
void APP_TpRxEnableInd (tTpMsgIdx uMsgIdx, tTpRxEnable uRxEnable)
{
    OSC_PARAM_UNUSED(uMsgIdx);
    OSC_PARAM_UNUSED(uRxEnable);
}
/*--------------------------{end APP_TpRxEnableInd}----------------------------------*/

/*--------------------------{APP_GetUdsDataBufferInd}--------------------------------------*/
/* Deviation MISRA-1 */
void APP_GetUdsDataBufferInd(u16 *uwLen, u8 *paubUdsData, tTpMsgIdx uMsgIdx, tUdsStatus eStatusRxReq)
{
    OSC_PARAM_UNUSED(uMsgIdx);
    OSC_PARAM_UNUSED(eStatusRxReq);
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((uwLen != NULL) && (paubUdsData != NULL))
    {
        OSC_PARAM_UNUSED(uwLen);
        OSC_PARAM_UNUSED(paubUdsData);
    }
}

/*--------------------------{end APP_GetUdsDataBufferInd}----------------------------------*/


#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
/*--------------------------{APP_CanWakeUpInd}--------------------------------------*/
void APP_CanWakeUpInd (void)
{
}
/*--------------------------{end APP_CanWakeUpInd}----------------------------------*/
#endif

/*--------------------------{APP_CanCtrlModeErrorInd}--------------------------------------*/
void APP_CanCtrlModeErrorInd (tCanErrors uErrorID)
{
    OSC_PARAM_UNUSED(uErrorID);
}
/*--------------------------{end APP_CanCtrlModeErrorInd}----------------------------------*/



/*--------------------------{NCS_SdfVariableCoherencyError}--------------------------------------*/
void NCS_SdfVariableCoherencyError(u16 uwParam)
{
    OSC_PARAM_UNUSED(uwParam);
}
/*--------------------------{end NCS_SdfVariableCoherencyError}----------------------------------*/

/*--------------------------{NCS_SdfVariableRangeError}--------------------------------------*/
void NCS_SdfVariableRangeError(u16 uwParam)
{
    OSC_PARAM_UNUSED(uwParam);
}
/*--------------------------{end NCS_SdfVariableRangeError}----------------------------------*/

/*--------------------------{App_CCP_CRO}--------------------------------------*/
void App_CCP_CRO(tComFrameIdx uFrameIdx)
{
    u8 aubData[8]={0};
    COM_GetFrameData (uFrameIdx, aubData);

    ccpCommand(&(aubData[0]));
}
/*--------------------------{end App_CCP_CRO}----------------------------------*/

/*--------------------------{RxInvalidMessage_DLC}--------------------------------------*/
void RxInvalidMessage_DLC(tComFrameIdx uFrameIdx)
{
	u8 ubDtcIdx = 0xFFU;
	if(NCS_TST_BIT_RESET(m_astDtcLocalStatus[DEM_DTC_IDX_DTC_CAN_BUS_OFF].ubDtcStatusMask, DEM_SM_TF, u8))			// Not Bus-Off failure.
	{
		if((SSMInput.SSMInIgnition) &&							// When T15 is on and
			(!DiagInput.D2_Battery))									// Battery voltage is in operating range.
		{
			
			if(uFrameIdx < 14)
			{
				//if(Cnt_InvalidDLC[uFrameIdx] > 0)
				//{
				//	--Cnt_InvalidDLC[uFrameIdx];
				//}
				//else
				{
					/* Signal clearance confirmation */
//					DEM_DtcTestStatus(ubDtcIdx, DEM_TEST_FAIL);
				}
			}
		}
	}

}

/*--------------------------{end RxInvalidMessage_DLC}----------------------------------*/


/*--------------------------{DEM_CriticalSectionExit}--------------------------------------*/
void DEM_CriticalSectionExit (u8 ebStatus)
{
    OSC_PARAM_UNUSED(ebStatus);
}
/*--------------------------{end DEM_CriticalSectionExit}----------------------------------*/

/*--------------------------{DEM_CriticalSectionEntry}--------------------------------------*/
u8 DEM_CriticalSectionEntry (void)
{
    return 1U;
}
/*--------------------------{end DEM_CriticalSectionEntry}----------------------------------*/


/*--------------------------{DEM_StoreDtcCbk}--------------------------------------*/
/* Deviation MISRA-1 */
void DEM_StoreDtcCbk(tDtcLocalStatus *stDtcLocalStatus, tDtcIdx ubDtcIdx)
{
	ubyte temp;
	/* Check inputs parameters */
	if (NULL != stDtcLocalStatus)
	{
		if(ubDtcIdx == 0xFFU)								// Erase all command
		{
			g_UDS.fDTC_EraseAllRequest = 1;
		}
		else if(ubDtcIdx < DEM_DTC_NUMBER)	// Write one by one
		{			
			/* A5 */
			stSaveDTC.ubStoreReq_DTC[ubDtcIdx] = 0xA5;	
		}
		m_astDtcLocalStatus[ubDtcIdx].ubDtcStatusMask = DTC_Status[ubDtcIdx].Flag.Data;
	}
}

/*--------------------------{end DEM_StoreDtcCbk}----------------------------------*/

ubyte pass_stack[5][14] =
{
	0,
};


/*--------------------------{DEM_IsConditionCorrect}--------------------------------------*/
/* Deviation MISRA-1 */
void DTC_BIT_SET(ubyte IDX, ubyte fail_confirm, ubyte set)
{  
	if(set == 0)//reset
	{
		if(NCS_TST_BIT_SET(m_astDtcLocalStatus[IDX].ubDtcStatusMask, DEM_SM_TF, u8))
		{
			//NCS_RESET_BIT(m_astDtcLocalStatus[IDX].ubDtcStatusMask, DEM_SM_TF, u8);
			DEM_StoreDtcCbk(m_astDtcLocalStatus, IDX);
		}
	}
	else if(set == 1)//set
	{
		if(NCS_TST_BIT_RESET(m_astDtcLocalStatus[IDX].ubDtcStatusMask, DEM_SM_TF, u8))
		{
			//NCS_SET_BIT(m_astDtcLocalStatus[IDX].ubDtcStatusMask, DEM_SM_TF, u8);
			DEM_StoreDtcCbk(m_astDtcLocalStatus, IDX);
		}
	} 
}

void DTC_SAVE_SET(ubyte IDX, ubyte set)
{  
	if(set == 0)//reset
	{
		if(NCS_TST_BIT_SET(m_astDtcLocalStatus[IDX].ubDtcStatusMask, DEM_SM_TF, u8))
		{
			//DEM_StoreDtcCbk(m_astDtcLocalStatus, IDX);
		}
	}
	else if(set == 1)//set
	{
		if(NCS_TST_BIT_RESET(m_astDtcLocalStatus[IDX].ubDtcStatusMask, DEM_SM_TF, u8))
		{
			//DEM_StoreDtcCbk(m_astDtcLocalStatus, IDX);
		}
	} 
}


ubyte Temporary_Failure_Check(ubyte dtcIdx)
{
	ubyte result;
	switch (dtcIdx)
	{
		case DEM_DTC_IDX_DTC_MISSINGMESSAGEVCU:  
		case DEM_DTC_IDX_DTC_CAN_BUS_OFF 	  : 
		case DEM_DTC_IDX_DTC_MESSAGEFAILUREVCU : 
		case DEM_DTC_IDX_DTC_INVALIDALIVECOUNTERVCU: 
		case DEM_DTC_IDX_DTC_BATUNDERVOLTAGE   : 
		case DEM_DTC_IDX_DTC_BATOVERVOLTAGE    : 
		//case DEM_DTC_IDX_DTC_NEUTRALSWSHOTBAT  : 
		//case DEM_DTC_IDX_DTC_NEUTRALSWSHOTGND  : 
		case DEM_DTC_IDX_DTC_APPLYSWSHOTBAT    : 
		case DEM_DTC_IDX_DTC_APPLYSWSHOTGND    : 
		case DEM_DTC_IDX_DTC_RLSSWSHOTBAT      : 
		case DEM_DTC_IDX_DTC_RLSSWSHOTGND      : 
		case DEM_DTC_IDX_DTC_OVERTEMPERATURE   : 
		case DEM_DTC_IDX_DTC_UNDERTEMPERATURE  : 
		case DEM_DTC_IDX_DTC_MTRDRIVERSCGL	   :
		case DEM_DTC_IDX_DTC_MTRDRIVERSCBL 	   :      
		case DEM_DTC_IDX_DTC_MTRDRIVERSCGR	   :  
		case DEM_DTC_IDX_DTC_MTRDRIVERSCBR 	   : 
		case DEM_DTC_IDX_DTC_NOLOADCABLDISCON  :	//<<<<<<<<<<<---------- l9369 internal failure
			result = TRUE; //Temporary
		break;
		case DEM_DTC_IDX_DTC_ACTUATORSTUCK	  : 	
		case DEM_DTC_IDX_DTC_MTROVERCURRENTL    : 
		case DEM_DTC_IDX_DTC_MTRUNDERCURRENTL  : 
		case DEM_DTC_IDX_DTC_MTROVERCURRENTR    : 
		case DEM_DTC_IDX_DTC_MTRUNDERCURRENTR  : 
			result = TRUE; //Temporary                    <--------------          //  result = FALSE; //Permanet
		break;		
	}
	return result;
}
ubyte DTC_Status_Cycle(ubyte dtcIdx, ubyte test_pass, ubyte cycle)
{

	DTC_Status[dtcIdx].Flag.Bit.NotCompleted = 0;	// 호출 마다 NotCompleted 초기화 ,  진단 테스트가 수행됨 
	if(test_pass == TRUE)
	{
		if(DTC_Status[dtcIdx].Failure_counter > 0)
		{
			--DTC_Status[dtcIdx].Failure_counter;	// 이전 실패로 인해 증가한 카운트 감소 
			if(Temporary_Failure_Check(dtcIdx) == TRUE) //Temperary or permernent check
			{
				DTC_Status[dtcIdx].Flag.Bit.Failed = 0; 
				
			}
		}
	}
	else // test 실패 
	{
		if(DTC_Status[dtcIdx].Failure_counter < cycle)
		{
			/* first step counter increase */
			++DTC_Status[dtcIdx].Failure_counter; //실패 카운트 증가 시켜 오류 추적 
		}
		else	// DTC 확정 및 저장 , 플래그 업데이트 
		{
			if(stSaveDTC.ubStoreReq_Mutex[dtcIdx]==0)
			{
				DEM_StoreDtcCbk(m_astDtcLocalStatus,dtcIdx);
				stSaveDTC.ubStoreReq_Mutex[dtcIdx] = 1;	
			}
			DTC_Status[dtcIdx].Flag.Bit.Failed = 1;
			DTC_Status[dtcIdx].Flag.Bit.FailedCycle = 1;
			DTC_Status[dtcIdx].Healing_counter = 40;
			DTC_Status[dtcIdx].Flag.Bit.Confirmed = 1; 
			DTC_Status[dtcIdx].Flag.Bit.Indicator = 1;
			
		}
	}
}		
void EEPROM_DTC_Save(void)
{
#if 0
	ubyte Data_Buffer =0, WriteAdd = 0, DTC_StoreIndex = 0;
	uint16_t WriteData;
	WriteDelay=30;
	
	for(WriteAdd=0; WriteAdd<(DEM_DTC_NUMBER/2)+1;)
	{
		if(WriteDelay > 20) 							// 20ms delay
		{
			WriteDelay = 0;

			if((DTC_StoreIndex % 2))
			{

				Data_Buffer = ((DTC_Status[DTC_StoreIndex-1].Healing_counter)<<2);
				Data_Buffer |= ((DTC_Status[DTC_StoreIndex-1].Flag.Bit.Indicator)<<1); //Indication bit
				//Data_Buffer |= DTC_Status[DTC_StoreIndex+1].Flag.Bit.FailedCycle>>1;	//testFailCycle bit
				WriteData = Data_Buffer;
					
				Data_Buffer = ((DTC_Status[DTC_StoreIndex].Healing_counter)<<2);
				Data_Buffer |= ((DTC_Status[DTC_StoreIndex].Flag.Bit.Indicator)<<1);	//Indication bit
				//Data_Buffer |= DTC_Status[DTC_StoreIndex].Flag.Bit.FailedCycle>>1;	//testFailCycle bit
				WriteData |= (Data_Buffer << 8);
			}
			else
			{
				Data_Buffer = ((DTC_Status[DTC_StoreIndex].Healing_counter)<<2);
				Data_Buffer |= ((DTC_Status[DTC_StoreIndex].Flag.Bit.Indicator)<<1); //Indication bit
				//Data_Buffer |= DTC_Status[DTC_StoreIndex+1].Flag.Bit.FailedCycle>>1;	//testFailCycle bit
				WriteData = Data_Buffer;
					
				Data_Buffer = ((DTC_Status[DTC_StoreIndex+1].Healing_counter)<<2);
				Data_Buffer |= ((DTC_Status[DTC_StoreIndex+1].Flag.Bit.Indicator)<<1);	//Indication bit
				//Data_Buffer |= DTC_Status[DTC_StoreIndex].Flag.Bit.FailedCycle>>1;	//testFailCycle bit
				WriteData |= (Data_Buffer << 8);
			}

			
			
		//	if(DTC_StoreAddOffset+WriteAdd == 0x3D)
			{
		//		WriteData = EEPROM_0x3D.Flag.Data;
			}
		//	SPI_EEP_Send_X16((ubyte)(DTC_StoreAddOffset+WriteAdd),(uint16_t)WriteData,EWEN_CMD);
			
			WriteAdd++;
			DTC_StoreIndex += 2;
		}		
	}	


#endif
}

void DTC_Poweroff(void)
{
	ubyte DTC_StoreIndex = 0;
	
	// Indicator, Healing counter Decrease Procedure
	for(DTC_StoreIndex=0;DTC_StoreIndex<(DEM_DTC_NUMBER);DTC_StoreIndex++)
	{
		if(Temporary_Failure_Check(DTC_StoreIndex) == TRUE)	//Temporary
		{
			
				if(DTC_Status[DTC_StoreIndex].Healing_counter > 1)
				{
					 --DTC_Status[DTC_StoreIndex].Healing_counter;
				}
				else if (DTC_Status[DTC_StoreIndex].Healing_counter == 1)
				{
					if(DTC_Status[DTC_StoreIndex].Flag.Bit.Indicator == 0)
					{
						--DTC_Status[DTC_StoreIndex].Healing_counter;
					}
				}					
				
				if(DTC_Status[DTC_StoreIndex].Flag.Bit.NotCompleted == 0 && DTC_Status[DTC_StoreIndex].Flag.Bit.FailedCycle == 0)
				{
					DTC_Status[DTC_StoreIndex].Flag.Bit.Indicator = 0;
				}
			
		}
	}

	// EEPROM Save Procedure
	EEPROM_DTC_Save();
}

tUdsStatus DEM_IsConditionCorrect(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    { 
        OSC_PARAM_UNUSED(puwLen);
        OSC_PARAM_UNUSED(aubUdsData);
        eStatus = UDS_ACK;
    }
    else
    {
        eStatus = UDS_NRC_22;
    }

    return eStatus;
}
/*--------------------------{end DEM_IsConditionCorrect}----------------------------------*/


/*--------------------------{ERAE_ClearDTC}--------------------------------------*/
/* Deviation MISRA-1 */
void ERAE_DEGRADE_CLEAR(void)
{
	//3 Specific Situcation Check Parameter
	//fActuator_Stuck = 0;
	//fCableDisconnect = 0;
	//
	//fEPBActive_check = 0;
	//UNDER_CUT_FAULT = 0;
	//OVER_CUT_FAULT = 0;
	DiagInput.D1_Actuator = 0; // always set from fActuator_stuck & fCableDisconnect
	DiagInput.D3_MotorDriver = 0;	// in Applying & Releasing
	DiagInput.D5_SwitchApply = 0;	// First Diag
	DiagInput.D6_SwitchRelease = 0;	// First Diag
	DiagInput.D7_SwitchNeutral = 0;	// First Diag
	DiagInput.D8_SwitchSignalCross = 0;
	//Diag_Force = NoInit;
	DiagInput.D11_MtrCurOvA = 0;
	DiagInput.D11_MtrCurUvA = 0;
	DiagInput.D11_MtrCurOvB= 0;	// 수정 필요 
	DiagInput.D11_MtrCurUvB = 0;

	
	//DiagInput.D20_VIN_Mismatch = 0;
	DiagInput.D21_CAN_BusOff = 0;
	
	//3 Always Check Parameter
	DiagInput.D2_Battery = 0;
	DiagInput.D4_Temperature = 0;

	

/*---------------------------------------------------------------------------
	DTC_Off($85) 기능으로 인해 Always Check Parameter 가 아님 
	DTC_Off($85) 시키면 CAN Message를 Check 하지 않기에 Clear 항목 추가 
	*/
	DiagInput.C1_EMS_3 = 0;
	DiagInput.C2_ECU_14 = 0;
	DiagInput.C3_ECU_Veh_02 = 0;
	DiagInput.C4_VCU = 0;
	DiagInput.C5_ESP_speed = 0;
	DiagInput.C6_CDPDecelAvailable = 0;
	DiagInput.C8_ESP_09 = 0;
	DiagInput.C9_ESP_YRS_02 = 0;
	DiagInput.C10_ACU_01 = 0;
	DiagInput.C11_BCM = 0;
	DiagInput.C12_BCM_LVBatteryVoltage = 0;
	DiagInput.C13_LowVoltage = 0;
	DiagInput.C14_ratAccPedSts = 0;
	DiagInput.C15_VehicleSpeed_ValueError = 0;
/*---------------------------------------------------------------------------- 2021.11.01 Power Cow */
	//3 Not Used
	DiagInput.D13_FIU_VM = 0;
	//DiagInput.D14 = 0;
	//DiagInput.D15_EMS = 0;
	DiagInput.D16_TCU = 0;
	DiagInput.D17_ESP = 0;
	//DiagInput.D18_YRS = 0;
	//DiagInput.D19_ACU = 0;
	//DiagInput.D22_ForceAndHallSensor = 0;
}


tUdsStatus ERAE_ClearDTC(u16 *puwLen, u8 *aubUdsData)
{
	
	ubyte FAIL_IDX, DTC_IDX;
	tUdsStatus eUdsStatus;
	eUdsStatus = UDS_ACK;
	/* input pointers are always checked in case of function takes NULL parameters */
	if ((puwLen != NULL) && (aubUdsData != NULL))
	{
   		eUdsStatus = DEM_ClearDiagnosticInformation(puwLen, aubUdsData);

		for(FAIL_IDX=0 ; FAIL_IDX<DTC_CAN_NUM; FAIL_IDX++)
		{
			for(DTC_IDX=0 ; DTC_IDX < 5; DTC_IDX++)
			{
				DTC_CAN_Status[FAIL_IDX].Failure_is[DTC_IDX].Data=0;
			}
		}
		for(FAIL_IDX=0 ; FAIL_IDX<DEM_DTC_NUMBER; FAIL_IDX++)
		{
			DTC_Status[FAIL_IDX].Flag.Data=0;
			DTC_Status[FAIL_IDX].Flag.Bit.NotCompleted=1;
			DTC_Status[FAIL_IDX].Healing_counter=0;
			DTC_Status[FAIL_IDX].Failure_counter=0;
			stSaveDTC.ubStoreReq_Mutex[FAIL_IDX] = 0;
		}
		g_CAN.CANBUS_firstcheck = 0;
		g_CAN.CANBUS_fail = 0;
		g_CAN.CANBUS_confirmed = 0;
		ERAE_DEGRADE_CLEAR();
		
	}
	else 
	{
		eUdsStatus = UDS_NRC_22;
	}
   return eUdsStatus;
}

/*--------------------------{end ERAE_ClearDTC}----------------------------------*/


/*--------------------------{COM_CBK_RXFRAME}--------------------------------------*/
u8 COM_CBK_RXFRAME(tComFrameIdx uFrameIdx)
{
    u8 nComStatus = COM_RX_IND_CHECK_OK;
    OSC_PARAM_UNUSED(uFrameIdx);
    if(g_UDS.gnRxFrameFlag == COM_TRUE)
    {
	    nComStatus = COM_RX_IND_CHECK_OK;
    }
    else
    {
	    nComStatus = COM_RX_IND_CHECK_REJECT;
    }
    return nComStatus;
}
/*--------------------------{end COM_CBK_RXFRAME}----------------------------------*/

/*--------------------------{ERAE_CommunicationControl}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus ERAE_CommunicationControl(u16 *puwLen, u8 *aubUdsData)
{
	tUdsStatus eUdsStatus = UDS_NRC_7F;
	OSC_PARAM_UNUSED(puwLen);
	OSC_PARAM_UNUSED(aubUdsData);

	if ((puwLen != NULL) && (aubUdsData != NULL))
	{
		eUdsStatus = UDS_ACK;
		if(!aubUdsData[1])
		{
			if((aubUdsData[2] == 0x01) || (aubUdsData[2] == 0x03))
			{
				IL_EnableEvtFrame(IL_TRUE); 					// For Tx
				g_UDS.gnRxFrameFlag = COM_TRUE;							// For Rx
				COM_Periodic(COM_PERIODIC_START);
			}
			else
			{
				eUdsStatus = UDS_NRC_31;
			}
		}
		
		else
		{
			if((aubUdsData[2] == 0x01) || (aubUdsData[2] == 0x03))		
			{
				IL_EnableEvtFrame(IL_FALSE);					// For Tx
				g_UDS.gnRxFrameFlag = COM_TRUE;							// For Rx
				COM_Periodic(COM_PERIODIC_STOP);
			}
			else
			{	
				eUdsStatus = UDS_NRC_31;
			}
		}
			*puwLen = 2;
	}

	return eUdsStatus;
}


/*--------------------------{end ERAE_CommunicationControl}----------------------------------*/



/*--------------------------{ERAE_TesterPresent}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus ERAE_TesterPresent(u16 *puwLen, u8 *aubUdsData)
{
	tUdsStatus eUdsStatus;


	/* input pointers are always checked in case of function takes NULL parameters */
	if ((puwLen != NULL) && (aubUdsData != NULL))
	{
		if(*puwLen == 2)
		{
			/* If all the DTC shall be locked */
			if((0U == aubUdsData[1])|| (0x80 == aubUdsData[1]))
			{
				eUdsStatus = UDS_ACK;
			}
		}
		else
		{
			eUdsStatus = UDS_NRC_13;
		}
	}
	else
	{
		eUdsStatus = UDS_NRC_22;
	}
	return eUdsStatus;
}

/*--------------------------{end ERAE_TesterPresent}----------------------------------*/

/*--------------------------{UDS_ControlDTCSetting}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus UDS_ControlDTCSetting(u16 *puwLen, u8 *aubUdsData)
{
	tUdsStatus eUdsStatus;


	eUdsStatus = UDS_ACK;


	/* input pointers are always checked in case of function takes NULL parameters */
	if ((puwLen != NULL) && (aubUdsData != NULL))
	{
		/* Check if all condition to execute the request are correct*/
		eUdsStatus = DEM_IsConditionCorrect(puwLen,aubUdsData);

		if(UDS_ACK == eUdsStatus)
		{
			/* If all the DTC shall be locked */
			if((1U == aubUdsData[1]) || (0x81 == aubUdsData[1]))
			{
				/* start DTC detect */
				g_UDS.m_aubDtcActivation = DEM_MODE_ON;
				DEM_StartOperatingCycle();
			}
			/* If all the DTC shall be unlocked */
			else if((2U == aubUdsData[1]) || (0x82 == aubUdsData[1]))
			{
				/* stop DTC detect */
				g_UDS.m_aubDtcActivation = DEM_MODE_OFF;
				DEM_StopOperatingCycle();
			}
			else
			{
				/* Request Out of Range */
				eUdsStatus = UDS_NRC_31;
			}
		}
	}
	else
	{
		eUdsStatus = UDS_NRC_22;
	}
	return eUdsStatus;
}

/*--------------------------{end UDS_ControlDTCSetting}----------------------------------*/

/*--------------------------{PROG_EcuReset}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PROG_EcuReset(u16 *puwLen, u8 *aubUdsData)
{
  OSC_PARAM_UNUSED(puwLen);
  OSC_PARAM_UNUSED(aubUdsData);

	g_UDS.Cnt_ResetDelay = 0;
	g_UDS.fECU_Reset = 1;
	g_UDS.fECU_Reset_Bit = 1;
	return UDS_NRC_78;
}
/*--------------------------{end PROG_EcuReset}----------------------------------*/

/*--------------------------{PROG_Sa1Request}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PROG_Sa1Request(u16 *puwLen, u8 *aubUdsData)
{
  tUdsStatus eStatus;

  eStatus = UDS_ACK;

	if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
	{
    /* Fill the answer */
    aubUdsData[2] = (u8) 0x00;					// For requirement chapter 4.1.1 of Diagnostic Test case with CANoeDiVa(Gabriel)
    aubUdsData[3] = (u8) 0x00;
    aubUdsData[4] = (u8) 0x00;
    aubUdsData[5] = (u8) 0x00;
    *puwLen = 6;
  }
  else
  {
      (void) PROG_Sa1Managment(puwLen, aubUdsData);
      m_ubSecurityAccessState = PROG_SA_SEED_SENT;
  }

  return eStatus;
}
/*--------------------------{end PROG_Sa1Request}----------------------------------*/

/*--------------------------{PROG_Sa2Request}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PROG_Sa2Request(u16 *puwLen, u8 *aubUdsData)
{
	tUdsStatus eStatus;
	
	eStatus = UDS_ACK;

	/* Implements : 000-07-D028-D-91601-1
	 if the SeedSent flag is true, start the AS tempo else rturn a NACK_24,
	 recover the key received */
	if (PROG_SA_SEED_SENT == m_ubSecurityAccessState)
	{
			eStatus = PROG_Sa2Managment(puwLen, aubUdsData);

			if (UDS_ACK == eStatus)
			{
					/* Unlock ECU if key is correct */
					m_eSecurityState = PROG_SECURITY_STATE_UNLOCK;
			}
	}
	else
	{
			eStatus = UDS_NRC_24;
	}

	return eStatus;
}
/*--------------------------{end PROG_Sa2Request}----------------------------------*/

/*--------------------------{RDBI_ADMON_0}--------------------------------------*/
#define	VoltageScaleFactor		0.1
/* Deviation MISRA-1 */
tUdsStatus RDBI_ADMON_0(u16 *puwLen, u8 *aubUdsData)
{
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 5U;
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_ADMON_0}----------------------------------*/

/*--------------------------{RDBI_ADMON_1}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_ADMON_1(u16 *puwLen, u8 *aubUdsData)
{
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 5U;
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_ADMON_1}----------------------------------*/

/*--------------------------{RDBI_ADMON_2}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_ADMON_2(u16 *puwLen, u8 *aubUdsData)
{
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 5U;
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_ADMON_2}----------------------------------*/

/*--------------------------{RDBI_ADMON_3}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_ADMON_3(u16 *puwLen, u8 *aubUdsData)
{
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 5U;
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_ADMON_3}----------------------------------*/

/*--------------------------{RDBI_EpbSwitchTest}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EpbSwitchTest(u16 *puwLen, u8 *aubUdsData)
{
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 4U;
        
		eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_EpbSwitchTest}----------------------------------*/

/*--------------------------{RDBI_EepromRead}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EepromRead(u16 *puwLen, u8 *aubUdsData)
{
	/* Define pointer to length in callback */
	/* If nothing is implemented in callback, length returned is 0 and no response is sent */
	/* By default the callback length shall be 3 (service 1 and DID 2 ) */
	
	tUdsStatus eUdsStatus;
		uword AD_Value;
	uint16_t Read_Buf_1 = 0x00U, Read_Buf_2 = 0x00U;
	/* input pointers are always checked in case of function takes NULL parameters */
	if ((puwLen != NULL) && (aubUdsData != NULL))
	{
		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
			*puwLen = 7U;
			Read_Buf_1	= SPI_EEP_Read_X16(EEPROM_TEST_1);
			Read_Buf_2	= SPI_EEP_Read_X16(EEPROM_TEST_2);
		
			aubUdsData[3] = (u8)(Read_Buf_1>>8);
			aubUdsData[4] = (u8)(Read_Buf_1);
			aubUdsData[5] = (u8)(Read_Buf_2>>8);
			aubUdsData[6] = (u8)(Read_Buf_2);
			eUdsStatus = UDS_ACK;
			
		}
		
		else
		{
			eUdsStatus = UDS_NRC_7F;
		}

	}
	else
	{
		eUdsStatus = UDS_NRC_13;
	}

	return eUdsStatus;
}

/*--------------------------{end RDBI_EepromRead}----------------------------------*/



/*--------------------------{RDBI_EpbHardwareNum}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EpbHardwareNum(u16 *puwLen, u8 *aubUdsData)
{
ubyte	i;
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 10U;

		eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_EpbHardwareNum}----------------------------------*/


/*--------------------------{RDBI_EpbSoftwareNum}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EpbSoftwareNum(u16 *puwLen, u8 *aubUdsData)
{
ubyte i;
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 23U;
		
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_EpbSoftwareNum}----------------------------------*/


/*--------------------------{RDBI_EpbBootSoftware_ID}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EpbBootSoftware_ID(u16 *puwLen, u8 *aubUdsData)
{
ubyte i;
uint16_t current_A=0U, current_B=0U;


    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
   
        *puwLen = 7U;
		eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_EpbBootSoftware_ID}----------------------------------*/

/*--------------------------{RDBI_EpbApplicationData_ID}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EpbApplicationData_ID(u16 *puwLen, u8 *aubUdsData)
{
ubyte i;
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 10U;
		eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_EpbApplicationData_ID}----------------------------------*/


/*--------------------------{RDBI_FingerPrint}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_Fingerprint(u16 *puwLen, u8 *aubUdsData)
{
	ubyte i,x,temp;
	uword	WriteData;
  /* Define pointer to length in callback */
  /* If nothing is implemented in callback, length returned is 0 and no response is sent */
  /* By default the callback length shall be 3 (service 1 and DID 2 ) */
  
  tUdsStatus eUdsStatus;
  
  /* input pointers are always checked in case of function takes NULL parameters */
  if ((puwLen != NULL) && (aubUdsData != NULL))
  {
    *puwLen = 15U;
		for(x=0,i=0;i<12;i++)
		{
			temp = i % 2;
			if(!temp)
			{
				WriteData = SPI_EEP_Read_X16((Fingerprint_AddOffset + x));
				g_UDS.ubFingerprint[i] = (ubyte)(WriteData >> 8);
				g_UDS.ubFingerprint[i+1] = (ubyte)WriteData;
				++x;
			}

			aubUdsData[3+i] = g_UDS.ubFingerprint[i];
		}
		eUdsStatus = UDS_ACK;
  }
  else
  {
      eUdsStatus = UDS_NRC_13;					// Incorrect Message Length or Invalid Format
  }

    return eUdsStatus;
}
/*--------------------------{end RDBI_FingerPrint}----------------------------------*/

/*--------------------------{WDBI_WriteFingerPrint}--------------------------------------*/

/* Deviation MISRA-1 */
tUdsStatus WDBI_Fingerprint(u16 *puwLen, u8 *aubUdsData)
{
	ubyte i;
	u8 ubsession;

  /* Define pointer to length in callback */
  /* If nothing is implemented in callback, length returned is 0 and no response is sent */
  /* By default the callback length shall be 3 (service 1 and DID 2 ) */
  
  tUdsStatus eUdsStatus = UDS_ACK;

  ubsession = UDS_GetCurrentSession();
  
  /* input pointers are always checked in case of function takes NULL parameters */
  if ((puwLen != NULL) && (aubUdsData != NULL))
  {
	 if(ubsession == 2)
		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
		  *puwLen = 5U;
				
				for(i=0;i<12;i++)
				{
					g_UDS.ubFingerprint[i] = aubUdsData[3+i];
				}

				g_UDS.fFingerPrintWrote = 1;
				eUdsStatus = UDS_NRC_78;
				//if(aubUdsData[15] != NULL || aubUdsData[15] == 0x0)
				//{
				//	eUdsStatus = UDS_NRC_13;
				//}
		}
		else
		{
			eUdsStatus = UDS_NRC_7F;					// Security Acces Denied
		}
	else
	{
		eUdsStatus = UDS_NRC_33;					// Security Acces Denied
	}
	
  }
  
  else
  {
	  eUdsStatus = UDS_NRC_13;					// Incorrect Message Length or Invalid Format
  }
	
	return eUdsStatus;
}

/*--------------------------{end WDBI_WriteFingerPrint}----------------------------------*/


/*--------------------------{WDBI_EpbTestMethod}--------------------------------------*/

/* Deviation MISRA-1 */
tUdsStatus WDBI_EpbTestMethod(u16 *puwLen, u8 *aubUdsData)
{
	ubyte i,TestMotorMethod;
	ubyte Read_th1 = 0U ,Read_th2 = 0U;
	uint16_t limit;

  /* Define pointer to length in callback */
  /* If nothing is implemented in callback, length returned is 0 and no response is sent */
  /* By default the callback length shall be 3 (service 1 and DID 2 ) */
  
  tUdsStatus eUdsStatus = UDS_ACK;
  
  /* input pointers are always checked in case of function takes NULL parameters */
  if ((puwLen != NULL) && (aubUdsData != NULL))
  {
 
	if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
	{
		*puwLen = 6U;
		eUdsStatus = UDS_ACK;

	}
	else
	{
		eUdsStatus = UDS_NRC_7F;					// Security Acces Denied
	}
	
  }
  
  else
  {
	  eUdsStatus = UDS_NRC_13;					// Incorrect Message Length or Invalid Format
  }
	
	return eUdsStatus;
}

/*--------------------------{end WDBI_EpbTestMethod}----------------------------------*/
	

/*--------------------------{RDBI_EpbTestState}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus RDBI_EpbTestState(u16 *puwLen, u8 *aubUdsData)
{
	ubyte i;
    /* Define pointer to length in callback */
    /* If nothing is implemented in callback, length returned is 0 and no response is sent */
    /* By default the callback length shall be 3 (service 1 and DID 2 ) */
    
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        *puwLen = 5U;		
//       OSC_PARAM_UNUSED(aubUdsData);


        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }

    return eUdsStatus;
}
/*--------------------------{end RDBI_EpbTestState}----------------------------------*/




/*--------------------------{IOCBI_EpbTest_CAN}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus IOCBI_EpbTest_CAN(u16 *puwLen, u8 *aubUdsData)
{
	ubyte i;
	ubyte Read_Cmd= 0x0U;


  /* Define pointer to length in callback */
  /* If nothing is implemented in callback, length returned is 0 and no response is sent */
  /* By default the callback length shall be 3 (service 1 and DID 2 ) */
  
  tUdsStatus eUdsStatus = UDS_ACK;
  /* input pointers are always checked in case of function takes NULL parameters */
  if ((puwLen != NULL) && (aubUdsData != NULL))
  {
	
	if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
	{

		*puwLen = 4U;
		eUdsStatus = UDS_ACK;
	}
	else
	{
		eUdsStatus = UDS_NRC_7F;					// Security Acces Denied
	}

  	}
  
  else
  {
	  eUdsStatus = UDS_NRC_13;					// Incorrect Message Length or Invalid Format
  }
	
	return eUdsStatus;
}

/*--------------------------{end IOCBI_EpbTest_CAN}----------------------------------*/


/*--------------------------{WDBI_EepromWrite}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus WDBI_EepromWrite(u16 *puwLen, u8 *aubUdsData)
{
	ubyte i;
	

  /* Define pointer to length in callback */
  /* If nothing is implemented in callback, length returned is 0 and no response is sent */
  /* By default the callback length shall be 3 (service 1 and DID 2 ) */
  
  tUdsStatus eUdsStatus = UDS_ACK;
  uword Write_Buf = 0x0U;
  /* input pointers are always checked in case of function takes NULL parameters */
  if ((puwLen != NULL) && (aubUdsData != NULL))
  {

	if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
	{
		*puwLen = 7U;
		eUdsStatus = UDS_ACK;
	}
	else
	{
		eUdsStatus = UDS_NRC_7F;					// Security Acces Denied
	}

  }
  
  else
  {
	  eUdsStatus = UDS_NRC_13;					// Incorrect Message Length or Invalid Format
  }
	
	return eUdsStatus;
}


/*--------------------------{end WDBI_EepromWrite}----------------------------------*/



/*--------------------------{IOCBI_BreakLightReq}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus IOCBI_EepromClear(u16 *puwLen, u8 *aubUdsData)
{
	/* Define pointer to length in callback */
	/* If nothing is implemented in callback, length returned is 0 and no response is sent */
	/* By default the callback length shall be 3 (service 1 and DID 2 ) */
	
	tUdsStatus eUdsStatus;
	uint16_t Read_Buf_1 = 0x00U, Read_Buf_2 = 0x00U;
	/* input pointers are always checked in case of function takes NULL parameters */
	if ((puwLen != NULL) && (aubUdsData != NULL))
	{

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
			*puwLen = 7U;
			eUdsStatus = UDS_ACK;
			
		}
		else
		{
			eUdsStatus = UDS_NRC_7F;					// Security Acces Denied
		}

	}
	else
	{
		eUdsStatus = UDS_NRC_13;					// Incorrect Message Length or Invalid Format
	}

	return eUdsStatus;
}

/*--------------------------{end IOCBI_BreakLightReq}----------------------------------*/



/*--------------------------{PostProgMemoryCheck}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PostProgMemoryCheck(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        OSC_PARAM_UNUSED(puwLen);
        OSC_PARAM_UNUSED(aubUdsData);
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }
    return eUdsStatus;
}
/*--------------------------{end PostProgMemoryCheck}----------------------------------*/

/*--------------------------{VinLearn}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus VinLearn(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
			g_UDS.fUDS_VIN_Lean = 1;
			eUdsStatus = UDS_NRC_78;
//					VIN_udsData = aubUdsData;
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_22;
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;					// Security Acces Denied
		}
    return eUdsStatus;
}
/*--------------------------{end VinLearn}----------------------------------*/

/*--------------------------{PFLT_RollerBenchStart}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PFLT_RollerBenchStart(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    /* input pointers are always checked in case of function takes NULL parameters */
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {	    	
	    	if((ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchPhase < 0x03) &&
					(ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchActive == 0x00))
				{
					g_UDS.DiagTxData[0] = 0x03;
					g_UDS.DiagTxData[1] = 0x7F;
					g_UDS.DiagTxData[2] = 0x31;
					g_UDS.DiagTxData[3] = 0x78;
					UDS_LongRequestEnd(0, g_UDS.DiagTxData, UDS_ACK);
//					IL_u32Wr_Diag_From_EPB_Sig(g_UDS.DiagTxData);
					COM_SendFrameDirect(COM_FRAME_IDX_Diag_From_EPB1R);
				//	StepRollerbench = 1;
	    		    SSMInput.SSMInRollerbenchRoutine = 0x4E;
						
	    	}
			else
			{
				eUdsStatus = UDS_NRC_24;		/* RequestS Sequence Error - The routine is already actived */
			}
	    }
	    else
	    {
					eUdsStatus = UDS_NRC_13;	/* Incorrect message length or invalid format */
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;			/* Sequrity access denied */
	  }
		
    return eUdsStatus;
}
/*--------------------------{end PFLT_RollerBenchStart}----------------------------------*/

/*--------------------------{PFLT_RollerBenchStop}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PFLT_RollerBenchStop(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    /* input pointers are always checked in case of function takes NULL parameters */
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
	    	if((ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchPhase > 0x02) &&
					(ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchPhase < 0x06))
	    	{
	    	//	StepRollerbench = 0;
		        SSMInput.SSMInRollerbenchRoutine = 2;
		        eUdsStatus = UDS_ACK;
	    	}
			else
			{
				eUdsStatus = UDS_NRC_24;		//Request Sequence Error - EPB is already stopped 
			}
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_13;	/* Incorrect message length or invalid format */
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;			/* Sequrity access denied */
	  }
		
    return eUdsStatus;
}
/*--------------------------{end PFLT_RollerBenchStop}----------------------------------*/

/*--------------------------{PFLT_RollerBenchResult}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PFLT_RollerBenchResult(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    /* input pointers are always checked in case of function takes NULL parameters */
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
	    	if((ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchRoutineResults < 0x04) &&
					(ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchRoutineID == 0x4E))
	    	{
	        SSMInput.SSMInRollerbenchRoutine = 0;

			*puwLen = 5;
			aubUdsData[2] = 0x18;
			aubUdsData[3] = 0x4E;
			aubUdsData[4] = (u8)ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchRoutineResults;
	        eUdsStatus = UDS_ACK;
	    	}
				else
				{
					eUdsStatus = UDS_NRC_24;		/* Request Sequence Error - The toutine has not been started */
				}
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_13;	/* Incorrect message length or invalid format */
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;			/* Sequrity access denied */
	  }
    return eUdsStatus;
}
/*--------------------------{end PFLT_RollerBenchResult}----------------------------------*/

/*--------------------------{PFLT_BeddingStart}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PFLT_BeddingStart(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    /* input pointers are always checked in case of function takes NULL parameters */
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
	        OSC_PARAM_UNUSED(puwLen);
	        OSC_PARAM_UNUSED(aubUdsData);
	        eUdsStatus = UDS_ACK;
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_22;
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;			/* Sequrity access denied */
	  }
		
    return eUdsStatus;
}
/*--------------------------{end PFLT_BeddingStart}----------------------------------*/

/*--------------------------{PFLT_BeddingStop}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PFLT_BeddingStop(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    /* input pointers are always checked in case of function takes NULL parameters */
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
	        OSC_PARAM_UNUSED(puwLen);
	        OSC_PARAM_UNUSED(aubUdsData);
	        eUdsStatus = UDS_ACK;
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_22;
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;			/* Sequrity access denied */
	  }
		
    return eUdsStatus;
}
/*--------------------------{end PFLT_BeddingStop}----------------------------------*/

/*--------------------------{PFLT_BeddingResult}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus PFLT_BeddingResult(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    /* input pointers are always checked in case of function takes NULL parameters */
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
	        //OSC_PARAM_UNUSED(puwLen);
	        //OSC_PARAM_UNUSED(aubUdsData);
			*puwLen = 5;
			aubUdsData[2] = 0x18;
			aubUdsData[3] = 0x4F;
			aubUdsData[4] = 0x00;
	        eUdsStatus = UDS_ACK;
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_22;
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;			/* Sequrity access denied */
	  }
		
    return eUdsStatus;
}
/*--------------------------{end PFLT_BeddingResult}----------------------------------*/

/*--------------------------{ProductForceLevelTest}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus ProductForceLevelTest(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
		if(m_eSecurityState == PROG_SECURITY_STATE_UNLOCK)
		{
	    if ((puwLen != NULL) && (aubUdsData != NULL))
	    {
//				SSMInput.SSMInRollerbenchRoutine = (uint16_T)aubUdsData[2];

				*puwLen = 0x04U;
	      eUdsStatus = UDS_ACK;
	    }
	    else
	    {
	        eUdsStatus = UDS_NRC_22;
	    }
		}
		else
		{
			eUdsStatus = UDS_NRC_33;					// Security Acces Denied
		}
    return eUdsStatus;
}
/*--------------------------{end ProductForceLevelTest}----------------------------------*/


/*--------------------------{EraeMemoryTest}--------------------------------------*/
/* Deviation MISRA-1 */


tUdsStatus EraeMemoryTest(u16 *puwLen, u8 *aubUdsData)
{
	tUdsStatus eUdsStatus = UDS_ACK;

	g_UDS.BootWriteAdd[0] = aubUdsData[9];
	g_UDS.BootWriteAdd[1] = aubUdsData[10];
	g_UDS.BootWriteAdd[2] = aubUdsData[11];
	g_UDS.BootWriteAdd[3] = aubUdsData[12];

	eUdsStatus = UDS_NRC_78;

	if(((aubUdsData[5] == 0x00) && (aubUdsData[6] == 0x01) && (aubUdsData[7] == 0x80) && (aubUdsData[8] == 0x00)))
	{
		eBootFlag = DOWNLOAD_REQUESTED;
		g_UDS.Cnt_ResetDelay = 0;
		g_UDS.fECU_Reset_Bit = 1;
		g_UDS.fBootWriteRequest = 1;
	}
	
	else
	{
		eUdsStatus = UDS_NRC_13;
	}
	
	return eUdsStatus;
}


/*--------------------------{end EraeMemoryTest}----------------------------------*/

/*--------------------------{CheckProgramingDependendcy}--------------------------------------*/
/* Deviation MISRA-1 */
tUdsStatus CheckProgramingDependency(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    
    /* input pointers are always checked in case of function takes NULL parameters */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        OSC_PARAM_UNUSED(puwLen);
        OSC_PARAM_UNUSED(aubUdsData);
		
        eUdsStatus = UDS_ACK;
    }
    else
    {
        eUdsStatus = UDS_NRC_22;
    }
    return eUdsStatus;
}
/*--------------------------{end CheckProgramingDependendcy}----------------------------------*/

