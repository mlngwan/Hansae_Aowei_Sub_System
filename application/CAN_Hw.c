/*===============================================================================*/
/*                                                                               */
/*                                  CAN Stack                                    */
/*                                                                               */
/* ------------------------------------------------------------------------------*/
/*                                                                               */
/*                                 CAN Driver for XPC560XB                       */
/*                                                                               */
/*===============================================================================*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*%%   _____________________________   %%  \file CAN_Hw.c                       */
/*%%  |                             |  %%  \brief CAN layer implementation       */
/*%%  |   &&&&&   &&&&&             |  %%  Module version: 4.6.14                 */
/*%%  |   &       &    &            |  %%  Variant: OsekCore                  */
/*%%  |   &&&&    &&&&&             |  %%                                        */
/*%%  |   &       &    &            |  %%                                        */
/*%%  |   &&&&&   &&&&&             |  %%                                        */
/*%%  |_____________________________|  %%                                        */
/*%%                                   %%                                        */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*===============================================================================*/
/* Copyright 2015 by Elektrobit Automotive GmbH                                  */
/* All rights exclusively reserved for Elektrobit Automotive GmbH,               */
/* unless expressly agreed to otherwise.                                         */
/*===============================================================================*/



/*=============================== FILE INCLUSION *===============================*/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files: CAN                                                           */

#include "EB_Prj.h"
#include "CAN_Lld.h"

/*                                                                               */
/*===============================================================================*/
/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1)  Deviated Rule: 11.3
 *   A cast should not be performed between a pointer type and an integral type.
 *
 *   Reason:
 *   A cast is mandatory for memory access in all CAN_LLD APIs.
 *
 */


/* Deviation MISRA-1 <START> */


/*============================== PUBLIC DEFINITIONS =============================*/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */
u8 m_ubNerrBitForTest = 0U;

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

/*                                                                               */
/*===============================================================================*/




/*============================ PRIVATE DEFINITIONS ==============================*/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Private Constants                                                           */

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

/*                                                                               */
/*===============================================================================*/




/*================================ IMPLEMENTATION ===============================*/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */



/*----------------------------------{CAN_HW_TransceiverPinCfg}-------------------------------*/
void CAN_HW_TransceiverPinCfg (CTRLIDX_VOID)
{
    CAN_SIU.PCR[9U].R = 0x200U; /* STB pin as output */
}

/*----------------------------------{end CAN_HW_TransceiverPinCfg}-------------------------------*/


/*----------------------------------{CAN_HW_SetTransceiverMode}-------------------------------*/
void CAN_HW_SetTransceiverMode (CTRLIDX u8 ubEn, u8 ubStb)
{
    /* Dummy code to avoid warning : remark #826-D: parameter was never referenced */
    OSC_PARAM_UNUSED(ubEn);
    OSC_PARAM_UNUSED(ubStb);
}
/*----------------------------------{end CAN_SetTransceiverMode}-------------------------------*/


#ifndef BOOTLOADER
    #if (CAN_TYPE == CAN_TYPE_LS)
/*----------------------------------{CAN_HW_GetNerrBit}-------------------------------*/
void CAN_HW_GetNerrBit (CTRLIDX u8 *pbNerr)
{
    if (NULL != pbNerr)
    {
        /*                                                                  *
         * The bit shall be read from hardware.                             *
         * Here, we set it to zero to comply with Misra-C:2004 16.7.        *
         * A pointer parameter in a function prototype should be declared   *
         * as pointer to const if the pointer is not used                   *
         * to modify the addressed object.                                  *
         */
        *pbNerr = m_ubNerrBitForTest;
    }
}
/*----------------------------------{end CAN_HW_GetNerrBit}-------------------------------*/
    #endif
#endif


/*----------------------------------{CAN_HW_PortCfg}-------------------------------*/
void CAN_HW_PortCfg (CTRLIDX_VOID)
{
#if (CAN_NODE_SELECTED == CAN_NODE_0)
    /* Config RX Port B1*/

    /* ------------------------ */
    /* SIUL : Configure port B1 as CAN0RX */
    /* ------------------------ */
    /* SMC:1 = 0 = output buffer disabled in SAFE mode */
    /* APC:1 = 0 = analog input cannot be used */
    /* PA:2 = 0 = AlternativeMode 0 = GPIO */
    /* OBE:1 = 0 = output buffer disabled */
    /* IBE:1 = 1 = input buffer enabled = CAN0RX */
    /* ODE:1 = 0 = open drain disabled */
    /* SRC:1 = 0 = pad configured as slow */
    /* WPE:1 = 0 = weak pull down disabled */
    /* WPS:1 = 0 = weak pull up diasbled */
   CAN_SIU.PCR[CAN_PORT_CAN0RX_PB1].R = 0x0100U;

    /* Config TX Port PB0*/

    /* ------------------------ */
    /* SIUL : Configure port B0 as CAN0TX */
    /* ------------------------ */
    /* SMC:1 = 0 = output buffer disabled in SAFE mode */
    /* APC:1 = 0 = analog input cannot be used */
    /* PA:2 = 01 = AlternativeMode 1 = CAN1TX */
    /* OBE:1 = 1 = output buffer enabled */
    /* IBE:1 = 0 = input buffer disabled */
    /* ODE:1 = 0 = open drain disabled */
    /* SRC:1 = 1 = pad configured as medium or fast */
    /* WPE:1 = 0 = weak pull down disabled */
    /* WPS:1 = 0 = weak pull up diasbled */
    CAN_SIU.PCR[CAN_PORT_CAN0TX_PB0].R = 0x0604U;

#elif (CAN_NODE_SELECTED == CAN_NODE_1)
    /* Config RX Port C11*/

   /* ------------------------ */
   /* SIUL : Configure port C11 as CAN1RX */
   /* ------------------------ */
   /* SMC:1 = 0 = output buffer disabled in SAFE mode */
   /* APC:1 = 0 = analog input cannot be used */
   /* PA:2 = 0 = AlternativeMode 0 = GPIO */
   /* OBE:1 = 0 = output buffer disabled */
   /* IBE:1 = 1 = input buffer enabled = CAN0RX */
   /* ODE:1 = 0 = open drain disabled */
   /* SRC:1 = 0 = pad configured as slow */
   /* WPE:1 = 0 = weak pull down disabled */
   /* WPS:1 = 0 = weak pull up diasbled */
   CAN_SIU.PCR[CAN_PORT_CAN1RX_PC11].R = 0x0100U;

   /* MPC56xxB: Select PCR 43 for CAN1RX Input */
   CAN_SIU.PSMI[0U].R = 0x01U;

    /* Config TX Port C10*/

   /* ------------------------ */
   /* SIUL : Configure port C10 as CAN1TX */
   /* ------------------------ */
   /* SMC:1 = 0 = output buffer disabled in SAFE mode */
   /* APC:1 = 0 = analog input cannot be used */
   /* PA:2 = 01 = AlternativeMode 1 = CAN1TX */
   /* OBE:1 = 1 = output buffer enabled */
   /* IBE:1 = 0 = input buffer disabled */
   /* ODE:1 = 0 = open drain disabled */
   /* SRC:1 = 1 = pad configured as medium or fast */
   /* WPE:1 = 0 = weak pull down disabled */
   /* WPS:1 = 0 = weak pull up diasbled */
   CAN_SIU.PCR[CAN_PORT_CAN1TX_PC10].R = 0x0604U;

#elif (CAN_NODE_SELECTED == CAN_NODE_2)
    /* Config RX Port E9*/

   /* ------------------------ */
   /* SIUL : Configure port E9 as CAN2RX */
   /* ------------------------ */
   /* SMC:1 = 0 = output buffer disabled in SAFE mode */
   /* APC:1 = 0 = analog input cannot be used */
   /* PA:2 = 0 = AlternativeMode 0 = GPIO */
   /* OBE:1 = 0 = output buffer disabled */
   /* IBE:1 = 1 = input buffer enabled = CAN0RX */
   /* ODE:1 = 0 = open drain disabled */
   /* SRC:1 = 0 = pad configured as slow */
   /* WPE:1 = 0 = weak pull down disabled */
   /* WPS:1 = 0 = weak pull up diasbled */
   CAN_SIU.PCR[CAN_PORT_CAN2RX_PE9].R = 0x0100U;

   /* MPC56xxB: Select PCR 73 for CAN1RX Input */
   CAN_SIU.PSMI[1U].R = 0x00U;

    /* Config TX Port E8*/

   /* ------------------------ */
   /* SIUL : Configure port E8 as CAN2TX */
   /* ------------------------ */
   /* SMC:1 = 0 = output buffer disabled in SAFE mode */
   /* APC:1 = 0 = analog input cannot be used */
   /* PA:2 = 01 = AlternativeMode 1 = CAN1TX */
   /* OBE:1 = 1 = output buffer enabled */
   /* IBE:1 = 0 = input buffer disabled */
   /* ODE:1 = 0 = open drain disabled */
   /* SRC:1 = 1 = pad configured as medium or fast */
   /* WPE:1 = 0 = weak pull down disabled */
   /* WPS:1 = 0 = weak pull up diasbled */
   CAN_SIU.PCR[CAN_PORT_CAN2TX_PE8].R = 0x0604U;

#elif (CAN_NODE_SELECTED == CAN_NODE_3)
    /* to do */
#elif (CAN_NODE_SELECTED == CAN_NODE_4)
    /* to do */
#else /* (CAN_NODE_SELECTED == CAN_NODE_5) */
    /* to do */
#endif
}
/*----------------------------------{end CAN_HW_PortCfg}-------------------------------*/


/*----------------------------------{CAN_HW_PreInit}-------------------------------*/
void CAN_HW_PreInit (CTRLIDX_VOID)
{
   /* To be filled by customer for required actions before CAN activation 
    * Example: RAM check, etc..
    */
}
/*----------------------------------{end CAN_HWPreInit}-------------------------------*/


/*-------------------------------------------------------------------------------*/
/* - Private Functions                                                           */

/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */

/* Deviation MISRA-1 <STOP> */
/*                                                                               */
/*===============================================================================*/

