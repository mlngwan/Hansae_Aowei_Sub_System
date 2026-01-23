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
/*%%   _____________________________   %%  \file CAN_Int.c                       */
/*%%  |                             |  %%  \brief CAN layer mplemtation          */
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



/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files: CAN                                                           */

#include "EB_Prj.h"
#include "CAN_Lld.h"

#include "Board.h"




/*                                                                               */
/*********************************************************************************/

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



/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

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

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

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



/*----------------------------------{CAN_InterruptLevelCfg}-------------------------------*/

void CAN_InterruptLevelCfg (CTRLIDX_VOID)
{
#ifdef BOARD_ISR_CAN0
    BOARD_INTC_PSR(INTC_FLEXCAN0_ESR_ERR)   = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
    BOARD_INTC_PSR(INTC_FLEXCAN0_ESR_BOFF)  = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
    BOARD_INTC_PSR(INTC_FLEXCAN0_BUF_00_03) = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
    BOARD_INTC_PSR(INTC_FLEXCAN0_BUF_04_07) = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
    BOARD_INTC_PSR(INTC_FLEXCAN0_BUF_08_11) = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
    BOARD_INTC_PSR(INTC_FLEXCAN0_BUF_12_15) = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
    BOARD_INTC_PSR(INTC_FLEXCAN0_BUF_16_31) = (BOARD_INTC_CHOOSE_CPU0 | BOARD_INTC_PSR_PRIO_4);
#endif
}
/*----------------------------------{end CAN_InterruptLevelCfg}-------------------------------*/


/*----------------------------------{CAN_InterruptEnable}-------------------------------*/
void CAN_InterruptEnable (CTRLIDX_VOID)
{
    /* This function should be written by the client */

#if (CAN_ERROR_STATUS_MODE == CAN_INTERRUPT_MODE)
    /* Control Register :
     * - BUSOFF IT enabled
     */
    NCS_SET_BIT(CAN_CTRL, (CAN_CTRL_BOFFMSK), u32);

    /* Error and Status Register : clear the IT flags */
    /* Caution : write1toclear register type => do not use OR operation */
    CAN_ESR = CAN_ESR_BOFFINT;
#endif

#if ((CAN_TX_MODE == CAN_INTERRUPT_MODE) || (CAN_RX_MODE == CAN_INTERRUPT_MODE))
    /* Interrupt Mask Register - enable all the mailbox IT */
    /* Since they were all put in INACTIVE mode in CAN_Init, */
    /* those which are not used will not generate IT. */
    CAN_IMASK1 = CAN_IR_ALL;

    /* Interrupt Flag Register - clear all the IT */
    CAN_IFLAG1 = CAN_IR_ALL;
#endif
}
/*----------------------------------{end CAN_InterruptEnable}-------------------------------*/


/*----------------------------------{CAN_InterruptDisable}-------------------------------*/
void CAN_InterruptDisable (CTRLIDX_VOID)
{
    /* This function should be written by the client */
    
    /* Control Register :
     * - BUSOFF IT disabled
     */
    NCS_RESET_BIT(CAN_CTRL, (CAN_CTRL_BOFFMSK), u32);

    /* Interrupt Mask Register */
    CAN_IMASK1 = 0U;
}
/*----------------------------------{end CAN_InterruptDisable}-------------------------------*/


/*-------------------------------------------------------------------------------*/
/* - Private Functions                                                           */

/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */

/* Deviation MISRA-1 <STOP> */
/*                                                                               */
/*********************************************************************************/



