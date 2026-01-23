/********************************************************************************
 *                                                                               *
 *                                 EB Layers                                     *
 *                                                                               *
 * ----------------------------------------------------------------------------- *
 *                                                                               *
 *                                   Board                                       *
 *                                                                               *
 *********************************************************************************
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        *
 *%%   _____________________________   %%  \file Board.c                         *
 *%%  |                             |  %%                                        *
 *%%  |   &&&&&   &&&&&             |  %%  Module version: 1.0.0                 *
 *%%  |   &       &    &            |  %%                                        *
 *%%  |   &&&&    &&&&&             |  %%                                        *
 *%%  |   &       &    &            |  %%                                        *
 *%%  |   &&&&&   &&&&&             |  %%                                        *
 *%%  |_____________________________|  %%                                        *
 *%%                                   %%                                        *
 *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        *
 *********************************************************************************
 * Copyright 2015 by Elektrobit Automotive GmbH                                  *
 * All rights exclusively reserved for Elektrobit Automotive GmbH,               *
 * unless expressly agreed to otherwise.                                         *
 ********************************************************************************/
/* Board_irq.c file
 * !LINKSTO OsekCore.Platforms.Board.001, 1
 *          OsekCore.Platforms.Board.101, 1
 *          OsekCore.Platforms.Board.012, 1
 */

#ifndef I_KNOW_THAT_THIS_CODE_IS_NOT_FOR_PRODUCTION
#error "the program is not qualified for series production"
#endif

#include "TSAutosar.h"
#include "board.h"
#include "Stm_Api.h"
#include "linflex_uart.h"
#include "common.h"


u32 offset_read = 0;
#ifdef BOARD_ISR_CAN0
#include "EB_Prj.h"
#include "CAN_Lld.h"
void BoardFlexCanErrorInterrupt(void);
void BoardFlexCanMailboxInterrupt(void);
void FlexCanESRClear(void);
#endif


#if(PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
INTERRUPT_FUNC(BoardIsrHandler)
#else
INTERRUPT_FUNC void BoardIsrHandler (void);
#endif

#ifdef BOARD_ISR_TEST
/*volatile u32 isrCounter;*/
void TestIsr (void);
#endif



#if(PLATFORMS_TOOLCHAIN == PLATFORMS_gnu)
INTERRUPT_FUNC(BoardIsrHandler)
#else
INTERRUPT_FUNC void BoardIsrHandler (void)
#endif
{

    u32 offset = BOARD_GET_ISR_SRC();
	offset_read = offset;
    /* Interrupt function shall call BoardEnableInterrupts/ BoardDisableInterrupts
     * !LINKSTO OsekCore.Platforms.Board.022, 1
     */
    m_ubBoardIntLockNestingCounter++;
	// 외부 인터럽트 Falling Edge 비활성화
	//SIU.ISR.R |= (1<<1); // SIUL_IFEER 레지스터에서 비트 1 클리어

	
    /*offset = BoardGetIsrIndex();*/
    switch(offset)
    {
#ifdef BOARD_ISR_TEST 
        case INTC_TEST:
            TestIsr();
            break;
#endif
#ifdef BOARD_ISR_CAN0
/* Board_isr_can0 활성화 확인 완료 */
       case INTC_FLEXCAN0_ESR_ERR  :
       case INTC_FLEXCAN0_ESR_BOFF :
			//BoardFlexCanErrorInterrupt();
			//FlexCanESRClear();
			
            break;
			
        case INTC_FLEXCAN0_BUF_00_03:
        case INTC_FLEXCAN0_BUF_04_07:
        case INTC_FLEXCAN0_BUF_08_11:
        case INTC_FLEXCAN0_BUF_12_15:
        case INTC_FLEXCAN0_BUF_16_31:
        case INTC_FLEXCAN0_BUF_32_63:
        	BoardFlexCanMailboxInterrupt();
        	break;
#endif
        case INTC_STM_0:
            STM_CH0_ISR();
            break;
		case INTC_UART0:
			LinFlex_0_RX_ISR();
			break;
		case INTC_DSPI1_SR_RFDF:
			//DSPI1_ReadWrite();

			break;
    }

    m_ubBoardIntLockNestingCounter--;

    EXIT_BOARD_INTERRUPT_HANDLER;
	//FlexCanReset();
}

#ifdef BOARD_ISR_TEST
void TestIsr (void)
{
    isrCounter++;
    BOARD_INTC_SSCIR_0 = BOARD_INTC_SSCIR_0_CLR3;

    /* In order to verify that ISR handler will not be lost
     * if during the interrupt routine, another ISR was triggered
     */
    if(isrCounter == 3U)
    {
        TRIGGER_INT();
    }
}
#endif

#ifdef BOARD_ISR_CAN0
/*
 * BoardFlexCanErrorInterrupt(u32 id)
 *
 * The BoardFlexCanErrorInterrupt() routine is called on CAN0 error event
 */
void BoardFlexCanErrorInterrupt(void)
{
   CAN_ErrorStatusInterrupt();

}

/*
 * BoardFlexCanMailboxInterrupt(u32 id)
 *
 * The BoardFlexCanMailboxInterrupt() routine is called on CAN0 mailbox event
 */
void BoardFlexCanMailboxInterrupt(void)
{
    /* get ISR Vector from INTC IACKR/INTVEC bit 30/31 are 0 and needs to be removed*/
    u16 uwISRVector = (u16)((BOARD_INTC_IACKR & BOARD_INTC_IACKR_INTVEC) >> 2);
    switch(uwISRVector)
    {
        case INTC_FLEXCAN0_BUF_00_03: CAN_LLD_MailboxInterrupt (0U, 3U); break;
        case INTC_FLEXCAN0_BUF_04_07: CAN_LLD_MailboxInterrupt (4U, 7U); break;
        case INTC_FLEXCAN0_BUF_08_11: CAN_LLD_MailboxInterrupt (8U, 11U); break;
        case INTC_FLEXCAN0_BUF_12_15: CAN_LLD_MailboxInterrupt (12U, 15U); break;
        case INTC_FLEXCAN0_BUF_16_31: CAN_LLD_MailboxInterrupt (16U, 31U); break;
        case INTC_FLEXCAN0_BUF_32_63: CAN_LLD_MailboxInterrupt (32U, 39U); break;
        //default : for(;;);    /* undefined event has same effect than Invalid Interrupt */
    }
    EXIT_BOARD_INTERRUPT_HANDLER;
}
void FlexCanESRClear(void)
{

	CAN_0.MCR.B.FRZ =1;
	CAN_0.MCR.B.HALT =1;
	while(CAN_0.MCR.B.FRZACK != 1){
		}

	
	CAN_0.ESR.B.TWRNINT=1;
	CAN_0.ESR.B.RWRNINT=1;
	CAN_0.ESR.B.BOFFINT=1;
	CAN_0.ESR.B.ERRINT=1;


	CAN_0.MCR.B.FRZ =0;
	CAN_0.MCR.B.HALT =0;
	while(CAN_0.MCR.B.NOTRDY != 0){
		}


}

#endif




