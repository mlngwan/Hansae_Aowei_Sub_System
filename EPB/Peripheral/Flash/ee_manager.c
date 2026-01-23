/*******************************************************************
* PROJECT : EEPROM Emulation Driver (EED) for C90 flash demo
* FILE    : ee_demo.c
*
* DESCRIPTION : This file contains source code for EED demo.               
*
* COPYRIGHT :(c) 2009, Freescale & STMicroelectronics
*
* VERSION   : BETA 0.9
* DATE      : 05.28.2009
* AUTHOR    : Leonardo Colombo
*
* HISTORY :
* 2008.01.15    Arvind Awasthi    Initial Version for JPC563M target
* 2009.05.28    Leonardo Colombo  Porting for SPC560B64/MPC5607B devices 
*                                 Added Data storage exception routine
*                                 example to manage ECC error.
*                                 ECC error could be caused by the ECC_Error
*                                 routine.
* 2010.01.22    Leonardo Colombo  Added example usage of Abort Function
*******************************************************************/
#include "ee_types.h"
#include "typedefs.h"
#include "ee_emulation.h"
#include "common.h"
//#define  _C90FL_TYPES_H_   /* Just to avoid compiler warnings */
#undef   NULL_CALLBACK
#include "ssd_c90fl.h"
#include "ee_manager.h"
#include "Can_Api.h"
extern Board_t g_BOARD;

/* DFlash */
/* Data flash blocks are used for EEprom Emulation */ 
/* block 0 configuration*/
BLOCK_CONFIG blockConf0 =
{
    LOW_DATA_BLOCK0_MASK,                           /* enabledBlock*/
    MAIN_DATA_ARRAY_BASE + LOW_DATA_BLOCK0_OFFSET,  /* start*/
    LOW_DATA_BLOCK0_SIZE,                           /* size*/
    0x00000000,                                     /* blank pointer*/
    LOW_ADDRESS_SPACE                               /* address space*/
};

/* block 1 configuration*/
BLOCK_CONFIG blockConf1 =
{
    LOW_DATA_BLOCK1_MASK,                           /* enabledBlock*/
    MAIN_DATA_ARRAY_BASE + LOW_DATA_BLOCK1_OFFSET,  /* start*/
    LOW_DATA_BLOCK1_SIZE,                           /* size*/
    0x00000000,                                     /* blank pointer*/
    LOW_ADDRESS_SPACE                               /* address space*/
};

/* block 2 configuration*/
BLOCK_CONFIG blockConf2 =
{
    LOW_DATA_BLOCK2_MASK,                           /* enabledBlock*/
    MAIN_DATA_ARRAY_BASE + LOW_DATA_BLOCK2_OFFSET,  /* start*/
    LOW_DATA_BLOCK2_SIZE,                           /* size*/
    0x00000000,                                     /* blank pointer*/
    LOW_ADDRESS_SPACE                               /* address space*/
};

/* block 3 configuration*/
BLOCK_CONFIG blockConf3 =
{
    LOW_DATA_BLOCK3_MASK,                           /* enabledBlock*/
    MAIN_DATA_ARRAY_BASE + LOW_DATA_BLOCK3_OFFSET,  /* start*/
    LOW_DATA_BLOCK3_SIZE,                           /* size*/
    0x00000000,                                     /* blank pointer*/
    LOW_ADDRESS_SPACE                               /* address space*/
};

/* block configuration array*/
BLOCK_CONFIG* blockConfig[4] =
{
    &blockConf0,				/* block 0 structure address*/
    &blockConf1,				/* block 1 structure address*/
    &blockConf2,				/* block 2 structure address*/
    &blockConf3					/* block 3 structure address*/
};

/* EEPROM configuration*/
EEPROM_CONFIG eepromConf_Base =
{
    4,										/* number of blocks*/
    0,										/* active block index*/
    0,										/* write lock flag*/
    C90FL_DATA_REG_BASE,	/* C90 Data Flash Configuration base address*/
    FALSE,								/* do not use cache table*/
    0x00000000,						/* cache table - blank pointer */
    &blockConfig[0]				/* block configuration array*/
};

void FlashEEPROM_Manager(uint32_t Length);
void FlashWriteForDtc(UINT64* WriteData,uint32_t Length);
void Unprotect_EEPROM( void );
void EmptyCallBack(void);
static FlashEEPROM_Write(uint8_t BlockNum,UINT64* WriteData, uint32_t Length);

uint8_t EEPROM_Manager_State;
UINT32 Manager_Error_Code;
uint8_t EE_Status;
uint8_t EE_Init_Attempt_Count;

#define MAX_ATTEMPTS 3

UINT64 RxData;

/********************************************************************************
* Function Name		: FlashWriteForDtc
* Description			: Flash write for DTC
* input						: None
* return					: None
********************************************************************************/
void FlashWriteForDtc(UINT64* WriteData,uint32_t Length)
{
	VUINT32 returnCode; 				 /* return code*/
	uint32_t* data_ptr;
	uint8_t index;
	UINT16	ReadData16;
	UINT64 TxData;
	UINT64 *pTxData;
	UINT32	CCP_StartAdd;
	BLOCK_CONFIG*	blockConf;		// block configuration pointer
	
	/* Added for marginal written by Gabriel */
	blockConf = eepromConf_Base.flashBlocks[2];
	RxData = FSL_FlashRead64(blockConf->blockStartAddr);

	if(g_BOARD.fEEP_ECC_Err)
	{
		g_BOARD.fEEP_ECC_Err = 0;
		blockConf = eepromConf_Base.flashBlocks[3];										// Flash Backup Area.
		RxData = FSL_FlashRead64(blockConf->blockStartAddr);
		if(g_BOARD.fEEP_ECC_Err)
		{
			Unprotect_EEPROM();
			CCP_StartAdd = FSL_RemoveEeprom(&eepromConf_Base, EmptyCallBack);
		}
	}

	Unprotect_EEPROM();
	FlashEEPROM_Write(2,WriteData,Length);
	FlashEEPROM_Write(3,WriteData,Length);
}

/********************************************************************************
* Function Name		: FlashEEPROM_Manager
* Description			: Flash writing function like EEPROM
* input						: None
* return					: None
********************************************************************************/
void FlashEEPROM_Manager(uint32_t Length)
{
VUINT32 returnCode;          /* return code*/
uint32_t* data_ptr;
uint8_t index;
UINT16  ReadData16;
UINT64 TxData;
UINT64 *pTxData;
UINT32	CCP_StartAdd;

BLOCK_CONFIG*	blockConf;		// block configuration pointer

	switch( EEPROM_Manager_State )
	{
		default:
		case EE_MANAGER_INIT:
			/* Added for marginal written by Gabriel */
			blockConf = eepromConf_Base.flashBlocks[0];
			RxData = FSL_FlashRead64(blockConf->blockStartAddr);

			if(g_BOARD.fEEP_ECC_Err)
			{
				g_BOARD.fEEP_ECC_Err = 0;
				blockConf = eepromConf_Base.flashBlocks[1];										// Flash Backup Area.
				RxData = FSL_FlashRead64(blockConf->blockStartAddr);
				if(g_BOARD.fEEP_ECC_Err)
				{
					Unprotect_EEPROM();
					CCP_StartAdd = FSL_RemoveEeprom(&eepromConf_Base, EmptyCallBack);
				}
			}
			
			g_BOARD.fEEP_ECC_Err = 0;
			if(RxData == 0xA5A5AA55)									// Flash Tatoo
			{				
				CCP_StartAdd = blockConf->blockStartAddr+8;

//				pTxData = (UINT64 *)&arg_G_CalPramSets;
				for(index = 0; index <= 48; index++)
				{
					RxData = FSL_FlashRead64(CCP_StartAdd);
					*pTxData = RxData;
					CCP_StartAdd += 8;
					++pTxData;
				}
			}

//			FSL_AbortFunction(&eepromConf);
		break;

		case EE_MANAGER_WRITE:
			Unprotect_EEPROM();
//			FlashEEPROM_Write(0,(UINT64 *)&arg_G_CalPramSets,Length);
//			FlashEEPROM_Write(1,(UINT64 *)&arg_G_CalPramSets,Length);								// CCP Data Backup
		break;
	}
}

/********************************************************************************
* Function Name	: FlashEEPROM_Write
* Description			: Flash writing function like EEPROM
* input						: None
* return					: None
********************************************************************************/
static FlashEEPROM_Write(uint8_t BlockNum,UINT64 * WriteData, uint32_t Length)
{
	UINT64 TxData;
	UINT32	CCP_StartAdd;
	uint8_t index;
	UINT64 *pTxData;
	BLOCK_CONFIG*	blockConf;

	blockConf = eepromConf_Base.flashBlocks[BlockNum];
	TxData = 0xA5A5AA55;											// Flash Tatoo
	if(FSL_ProgramDoubleWord(eepromConf_Base.c90flRegBase, blockConf->blockStartAddr,TxData, EmptyCallBack) == 0x40)
	{
		CCP_StartAdd = FSL_RemoveEeprom(&eepromConf_Base, EmptyCallBack);
	}
		
	pTxData = WriteData;
	CCP_StartAdd = blockConf->blockStartAddr+8;

	for(index = 0; index <= Length; index++)
	{
		TxData = *pTxData;
		if(FSL_ProgramDoubleWord(eepromConf_Base.c90flRegBase, CCP_StartAdd,TxData, EmptyCallBack) == 0x40)
		{
			CCP_StartAdd = FSL_RemoveEeprom(&eepromConf_Base, EmptyCallBack);
		}
		++pTxData;
		CCP_StartAdd += 8;
	}
}

void Unprotect_EEPROM( void )
{
    /* unprotect low and middle blocks*/
    /* write the passwords*/
    WRITE32(C90FL_DATA_REG_BASE + C90FL_LML, 0xA1A11111);
    WRITE32(C90FL_DATA_REG_BASE + C90FL_SLL, 0xC3C33333);

    /* set lock registers for unlocking all low and middle blocks except Shadow*/
    WRITE32(C90FL_DATA_REG_BASE + C90FL_LML, C90FL_LML_SLOCK);
    WRITE32(C90FL_DATA_REG_BASE + C90FL_SLL, C90FL_SLL_SSLOCK);

}

void EmptyCallBack(void)
{
}

