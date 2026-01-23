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
/* Board.c file */
/* 
 * !LINKSTO OsekCore.Platforms.Board.001, 1 
 */
/* 
 * !LINKSTO OsekCore.Platforms.Board.012, 1
 */

#ifndef I_KNOW_THAT_THIS_CODE_IS_NOT_FOR_PRODUCTION
#error "the program is not qualified for series production"
#endif

#include "TSAutosar.h"
#include "board.h"
#include "common.h"
#include "Can_Api.h"

/*******************************************************************************
 * check Board Scheduling pre-conditions
 ******************************************************************************/
#if ( (BOARD_SCHEDULER * BOARD_SYSTEM_CLOCK_MHZ) > 4294967295)
#error "Board Scheduling is impossible to apply, it overflows Timer configuration, please reduce BOARD_SCHEDULER"
#endif

#if(BOARD_SCHEDULER < BOARD_TIMER_MINIMAL )
#error "Board Scheduling is inferior to minimal possible scheduling, please adapt BOARD_SCHEDULER"
#endif

#if(BOARD_SYSTEM_CLOCK_MHZ == 0)
#error "Board Frequency is not configured correctly, please adapt BOARD_SYSTEM_CLOCK_MHZ"
#endif

#if !defined(BOARD_SYSTEM_CLOCK_MHZ)
#error "Board Frequency is not configured correctly, please adapt BOARD_SYSTEM_CLOCK_MHZ"
#endif


/*==================[internal function declarations]========================*/
#ifndef PLATFORMS_STUBS_ENABLED
NCS_PRI_FCT uint8 Board_PAMagicCarpetEnterMode(u32 newMode);
NCS_PRI_FCT Platforms_ResetType BoardGetResetTypeBeforeClear(void);
#endif

/*==================[external function definitions]=========================*/
extern Board_t g_BOARD;
uint8 m_ubBoardIntLockNestingCounter = 0U;
#ifndef PLATFORMS_STUBS_ENABLED
static Platforms_ResetType m_uwResetType = 0U;
#endif
#ifdef BOARD_ISR_TEST
volatile u32 isrCounter = 0;
#endif

/*==================[external function definitions]=========================*/

/*----------------------------------{BoardInit}---------------------------------*/
/*
** BoardInit()
**
** BoardInit() is called by the startup code (cstartup.s) after data and bss
** sections have been initialized (global variables may now be used).
** The BoardInit() can be used to start up drivers etc.
*/
/*
 * !LINKSTO OsekCore.Platforms.Board.013, 1
 */
void BoardInit(void)
{

#if(0) /* trueonot : use EPB STM init */
  BOARD_STM0_CR = BOARD_STM0_CR_TEN;
#endif /* trueonot : use EPB STM init */

#ifdef PLATFORMS_STUBS_ENABLED
  /* Call stub function for test */
  BoardStubInit();
#else
#if (BOARD_LED_USED == BOARD_TRUE)
    /*Init LED so that an Invalid Interrupt/Exception can be seen earlier*/
    BoardInitLed();
#endif 

#endif
}
/*----------------------------------{end of BoardInit}---------------------------------*/

/*----------------------------------{BoardDisableInterrupts}---------------------------------*/
/* DisableInterrupt is linked to PLatforms plugin file
 * !LINKSTO OsekCore.Platforms.Board.002, 1
 */
void BoardDisableInterrupts(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
  /* Call stub function for test */
  BoardStubDisableInterrupts();
#else
  if (m_ubBoardIntLockNestingCounter < 255U)
  {
      if (m_ubBoardIntLockNestingCounter == 0U)
      {
          PLATFORMS_DISABLE();
      }
      /* Manage nesting
       * !LINKSTO OsekCore.Platforms.Board.102, 1
       */
      m_ubBoardIntLockNestingCounter++;
  }
#endif /* ifdef PLATFORMS_STUBS_ENABLED  */
}
/*----------------------------------{end of BoardDisableInterrupts}---------------------------------*/

/*----------------------------------{BoardEnableInterrupts}---------------------------------*/
/* EnableInterrupt is linked to PLatforms plugin file
 * !LINKSTO OsekCore.Platforms.Board.002, 1
 */
void BoardEnableInterrupts(void)
{
  #ifdef PLATFORMS_STUBS_ENABLED
      /* Call stub function for test */
      BoardStubEnableInterrupts();
  #else
      if (m_ubBoardIntLockNestingCounter > 0U)
      {
      /* Manage nesting
       * !LINKSTO OsekCore.Platforms.Board.102, 1
       */
          m_ubBoardIntLockNestingCounter--;

          if (m_ubBoardIntLockNestingCounter == 0U)
          {
            /* wrapping to compiler specific ISR disable */
            PLATFORMS_ENABLE();
          }
      }
#endif /* ifdef PLATFORMS_STUBS_ENABLED  */
}
/*----------------------------------{end of BoardEnableInterrupts}---------------------------------*/

/*----------------------------------{BoardWatchdogDeactivation}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.003, 1
 */
void BoardWatchdogDeactivation(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
  /* Call stub function for test */
  BoardStubWatchdogDeactivation();
#else
#if (BOARD_WDG_USED == BOARD_TRUE)
    /* disable watchdog */
    BOARD_SWT0_SR_REG = BOARD_SWT0_SR_KEY_1;
    BOARD_SWT0_SR_REG = BOARD_SWT0_SR_KEY_2;
    BOARD_SWT0_CR_REG = (BOARD_SWT0_CR_MAP0|BOARD_SWT0_CR_RIA|BOARD_SWT0_CR_1|BOARD_SWT0_CR_FRZ);
#endif
#endif
}
/*----------------------------------{end of BoardWatchdogDeactivation}---------------------------------*/

/*----------------------------------{BoardStartupClocks}---------------------------------*/
void BoardStartupClocks(void)
{
/* Clocks setup is under preprocessing conditions
 * !LINKSTO OsekCore.Platforms.Board.004, 1
 */
#ifdef PLATFORMS_STUBS_ENABLED
  /* Call stub function for test */
    BoardStubStartupClocks();
#else
#if (BOARD_TRUE == BOARD_STARTUP_CLOCK_ENABLE)
    /* Default parameters after reset are used. */
    /* No register modification needed */
    /* Set Run Peripheral Configuration Registers 0 */
    BOARD_ME_RUN_PC0 = 0xFF;

    /* set CMU to avoid reset when XOSC is enabled (Frc/2^div < Fosc)*/
    BOARD_CMU0_CSR_REG |= (3 << 1);

    /* Consistent initial point */
    if (!Board_PAMagicCarpetEnterMode(BOARD_MC_MODE_SAFE))
    {
      while(1);
    }

    /* Init magic carpet module */
    BOARD_MC_ME_ME_REG = 0x0000053D; /* enable all modes, except Test,Run2+Run3 */
    BOARD_MC_ME_MODE_MC_REG(BOARD_MC_MODE_SAFE) = 0x001F0030; /* SAFE:  Flash+XTAL on, IRC is sys.clock */
    BOARD_MC_ME_MODE_MC_REG(BOARD_MC_MODE_DRUN) = 0x001F0030; /* DRUN:  Flash+XTAL on, IRC is sys.clock */

    if (!Board_PAMagicCarpetEnterMode(BOARD_MC_MODE_DRUN))
    {
      while(1);
    }

    /* Set FMPLL Control Register */
    BOARD_FMPLL0_CR_REG = FMPLL_VALUE | 0x1 ;

    /* Wait for PLL */
    while(BOARD_MC_ME_GS_REG & (1 << 6)){};

    BOARD_MC_ME_MODE_MC_REG(BOARD_MC_MODE_RUN0) = 0x001F0074; /* RUN0:  Flash+XTAL on, PLL0 is sys.clock */

    if (!Board_PAMagicCarpetEnterMode(BOARD_MC_MODE_RUN0))
    {
      while(1);
    }

    /* Wait for SIUL becomes active. Must not be checked directly after the assignment
     * to BOARD_ME_RUN_PC0 (see above), because otherwise this loop would be endless.
     */
    while((BOARD_ME_PS2_REG & (1 << 4)) == 0){}
#endif /*#if (BOARD_TRUE == BOARD_STARTUP_CLOCK_ENABLE)*/
#endif
}
/*----------------------------------{end of BoardStartupClocks}---------------------------------*/

/*----------------------------------{BoardPortsSettings}---------------------------------*/
/* So far no Port are used in the Board
 * !LINKSTO OsekCore.Platforms.Board.007, 1
 */
void BoardPortsSettings(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
  /* Call stub function for test */
  BoardStubPortsSettings();
#else
  /* Leds (SIUL2) shall be initialized after clocks */
  BoardInitLed();
#endif
}
/*----------------------------------{end of BoardPortsSettings}---------------------------------*/

/*----------------------------------{BoardInitializationTimers}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.005, 1
 */
u8 BoardInitializationTimers(u16 uwTimer)
{

    u8 RetVal = 0u;
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubInitializationTimers(uwTimer);
#else

  u32 ulTimerValue = 0u;

  /* check Timer validity and Board frequency available */
  if ((uwTimer >= BOARD_TIMER_MINIMAL) && (BOARD_SYSTEM_CLOCK_MHZ > 0))
  {
    /* Turn on Periodic Interrupt Timer */
    /* FRZ = 1 : Timer freezed in debug mode */
    BOARD_PIT_MCR = 0x01U;

    /* Timer for scheduling: */

    /* Input clock is x MHz = y ns
     * Use Timer with uwTimer in 킪 period
     * x MHz => Load value must be uwTimer/y ns cycles
     */

    ulTimerValue = uwTimer * BOARD_SYSTEM_CLOCK_MHZ;

    BOARD_PIT_CHAN_LDVAL(1) = ulTimerValue;

    /* Disable Timer 1 interrupts + Start timer 1 */
    BOARD_PIT_CHAN_TCTRL(1) =  0x01U;

    /* Timer configured OK*/
    RetVal = 1u;
  }
  else
  {
    RetVal = 0u;
  }

#endif

  return RetVal;
}
/*----------------------------------{end of BoardInitializationTimers}---------------------------------*/

/*----------------------------------{BoardSchedulingTimer}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.010, 1
 */
u8 BoardSchedulingTimer (void)
{
  u8 ubFlag = BOARD_FALSE;

#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
  ubFlag = BoardStubSchedulingTimer();
#else
    /* Timer is scheduled at 1ms */
    if (0x01U == ((u8) BOARD_PIT_CHAN_TFLG(1) & 0x01U))
    {
        /* reset timer */
        BOARD_PIT_CHAN_TFLG(1) &= 0xFFU;

        /* Set return flag to TRUE */
        ubFlag = BOARD_TRUE;
    }
#endif
    return ubFlag;

}
/*----------------------------------{end of BoardSchedulingTimer}---------------------------------*/

/*----------------------------------{BoardStart}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.011, 1
 */
void BoardStart(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubStart();
#else

    /* Clear reset event status for later mode switching    */
    /* Note: Run mode switch into safe mode is not possible */
    /*       while safe mode interrupt pending flaf is set  */
    /*
     * !LINKSTO OsekCore.Platforms.Board.021, 1
     */
    m_uwResetType = BoardGetResetTypeBeforeClear();
    /* clear destructive event reset reason (w1c) */
    BOARD_MC_RGM_DES = PLATFORMS_RGM_DES_REG_MASK;
    /* clear functional event reset reason (w1c) */
    BOARD_MC_RGM_FES = PLATFORMS_RGM_FES_REG_MASK;

  /* Deactivation of watchdog */
  BoardWatchdogDeactivation();

  /* Suspend all interrupts */
  BoardDisableInterrupts();

  /* Setup clocks*/
  BoardStartupClocks();


  if(1u != BoardInitializationTimers(BOARD_SCHEDULER))
  {
  /*in case timer cannot be initialized, while loop for debugging
   * !LINKSTO OsekCore.Platforms.Board.011, 1
   */
    for (;;) { }
  }

  /* P1 Initialisation */
  BoardPortsSettings();

#if (BOARD_TRUE == BOARD_INIT_IT)
  /* Initialization ISRs */
  BoardInitInterrupts();
#endif

#if (BOARD_TRUE == BOARD_INTERRUPT_ENABLE)
  /* Enable all interrupts */
  BoardEnableInterrupts();
#endif

#endif
}
/*----------------------------------{end of BoardStart}---------------------------------*/

/*----------------------------------{BoardInitLed}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.008, 1
 */
void BoardInitLed(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubInitLed();
#else
  LEDS_INIT();
  LEDS_SET(0); /* check the LEDS */
  LEDS_SET(1);
  LEDS_SET(0);
#endif
}
/*----------------------------------{end of BoardInitLed}---------------------------------*/
#if 0									//1 Commented by Gabriel because unintended PC jump.
/*----------------------------------{BoardToggleLed}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.009, 1
 */
void BoardToggleLed (u8 ubLed)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubToggleLed(ubLed);
#else
    LEDS_SET(ubLed);
#endif
}
/*----------------------------------{end of BoardToggleLed}---------------------------------*/
#endif
/*----------------------------------{BoardInitInterrupts}---------------------------------*/
uint8_t GetInterruptPriority(uint8_t input)
{    return BOARD_INTC_PSR(input);  // IRQ30 (STM0)의 우선순위 읽기
}

uint8_t Priority = 0;

void BoardInitInterrupts(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubInitInterrupts();
#else
  u16 i = 0u;

  /* Initialize INTC for hardware vector mode */
  BOARD_INTC_MCR = (BOARD_INTC_MCR_HVEN | BOARD_INTC_MCR_VTES);

  /* Set IACKR to Interrupt Table*/
  BOARD_INTC_IACKR = (u32)((u32)(&IntcVectorTableHw[0]) << 11U);


  /* set priorities of all interrupts */
  for (i = 0U; i < BOARD_INTC_NB_PSR ; i++)
  {
    /* Set all PSR priority to lowest*/
    BOARD_INTC_PSR(i) = BOARD_INTC_PSR_PRIO;
  }
  

#ifdef BOARD_ISR_TEST
  /* Set TestISr priority above the other priorities*/
  BOARD_INTC_PSR(INTC_TEST) = BOARD_INTC_PSR_PRIO_4;
#endif
 

 // Priority =GetInterruptPriority(65U);

  /* Ensure INTC's current priority is 0 */
  BOARD_INTC_CPR = BOARD_INTC_CPU_PRIO;
#endif
}
/*----------------------------------{end of BoardInitInterrupts}---------------------------------*/

/*----------------------------------{BoardInvalidInterrupt}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.014, 1
 */
void BoardInvalidInterrupt(u32 id)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubInvalidInterrupt(id);
#else

  /* remove unused parameter warning */
  (void) id;

#if 0			//1 Infinite Loop disable by Gabriel
  	for (;;) { }
#else
		eBootFlag = 0;
		BoardPerformReset(PLATFORMS_SW_RESET);
#endif

#endif
}
/*----------------------------------{end of BoardInvalidInterrupt}---------------------------------*/

/*----------------------------------{BoardInvalidException}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.015,1
 */
u32 id_Temp, sp_Temp;
void BoardInvalidException(u32 id, u32 sp)
{
	id_Temp = id;
	sp_Temp = sp;
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubInvalidException(id,sp);
#else
  /* CHECK: RULE 404 ON */

  /* remove unused parameter warning */
  (void) id;
  (void) sp;
#if (BOARD_LED_USED == BOARD_TRUE)
  LEDS_INIT();
  LEDS_SET(0xF);
#endif
  if((id == 0x02) && ((sp > 0x7FFFFF) && (sp < 0x810000)))
  {
  	g_BOARD.fEEP_ECC_Err = 1;
  }
  else
  {
#if 0			//1 Infinite Loop disable by Gabriel
				for (;;) { }
#else
				eBootFlag = 0;
				BoardPerformReset(PLATFORMS_SW_RESET);
#endif
  }
#endif
}
/*----------------------------------{end of BoardInvalidException}---------------------------------*/


/*----------------------------------{BoardPerformReset}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.016,1
 */
void BoardPerformReset(Platforms_ResetType ubResetType)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubPerformReset(ubResetType);
#else
    switch(ubResetType)
    {
      /* Perform a Destructive Reset no matter what */
      case PLATFORMS_SW_RESET:
      case PLATFORMS_SOFT_DEST_RESET:
      default:
        BOARD_MC_ME_MCTL = (BOARD_MC_ME_MCTL_MODE_RST) |
                           (BOARD_MC_ME_MCTL_KEY);
        BOARD_MC_ME_MCTL = (BOARD_MC_ME_MCTL_MODE_RST) |
                           (BOARD_MC_ME_MCTL_KEY_INV);

      break;
    }

    /* Infinite loop is intended since ECU shall reset,
     * in worst case, if WDG is on, it shall reset the ECU
     */
    /* Wait for Reset*/
    for (;;)
    {

    }

#endif /* ifdef PLATFORMS_STUBS_ENABLED  */

}
/*----------------------------------{end of BoardPerformReset}---------------------------------*/

/*----------------------------------{BoardGetResetType}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.017,1
 */
Platforms_ResetType BoardGetResetType(void)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    return BoardStubGetResetType();
#else
    return m_uwResetType;
#endif
}
/*----------------------------------{end of BoardGetResetType}---------------------------------*/

#ifndef PLATFORMS_STUBS_ENABLED
NCS_PRI_FCT Platforms_ResetType BoardGetResetTypeBeforeClear(void)
{
    volatile Platforms_ResetType BoardResetReturn;

    volatile uint32 BoardResetRegister_des = (uint32) 0U;
    volatile uint32 BoardResetRegister_fes = (uint32) 0U;
    volatile uint16 BoardTmpReg = (uint16) 0U;
    uint32 BoardResetBitMaskDes = (uint32) 0U;
    uint32 BoardResetBitMaskFes = (uint32) 0U;
    uint32 Local_TimeOut = (uint32) 0U;

    BoardResetReturn = PLATFORMS_RESET_UNDEFINED;
    BoardResetRegister_des = PLATFORMS_RESET_UNDEFINED;
    BoardResetRegister_fes = PLATFORMS_RESET_UNDEFINED;

    /* build ResetReson from Functionnal & Desctructive reset 16 bits registers */

    BoardResetRegister_des = (uint32) BOARD_MC_RGM_DES;
    BoardResetRegister_fes = (uint32) BOARD_MC_RGM_FES;

    if (BOARD_MC_RGM_DES_POR == (BoardResetRegister_des & (BOARD_MC_RGM_DES_POR)))
    {
        BoardResetReturn = PLATFORMS_POWER_ON_RESET;
        BoardResetBitMaskDes = BOARD_MC_RGM_DES_POR;
    }
    else if (BOARD_MC_RGM_FES_SOFT == (BoardResetRegister_fes & (BOARD_MC_RGM_FES_SOFT)))
    {
        BoardResetReturn = PLATFORMS_SW_RESET;
        BoardResetBitMaskFes = BOARD_MC_RGM_FES_SOFT;
    }
    else if (BOARD_MC_RGM_DES_SOFT == (BoardResetRegister_des & (BOARD_MC_RGM_DES_SOFT)))
    {
        BoardResetReturn = PLATFORMS_SW_RESET;
        BoardResetBitMaskDes = BOARD_MC_RGM_DES_SOFT;
    }
    else
    {
        /* all other reset reasons are unmanaged*/
        BoardResetReturn = PLATFORMS_RESET_UNDEFINED;
    }

    if ((BoardResetBitMaskDes & PLATFORMS_RGM_DES_FES_REG_MASK) != 0U)
    {
        /* dest reset */
        BOARD_MC_RGM_DES = (uint16) BoardResetBitMaskDes;
    }
    else
    {
        /* func reset */

        /* the following loop is required according to the note in Reference Manual FES register*/
        do
        {
            /* Clear FES */
            BOARD_MC_RGM_FES = BoardResetBitMaskFes;

            /* Read FES */
            BoardTmpReg = BOARD_MC_RGM_FES;
            Local_TimeOut++;
        } while (((BoardTmpReg & BoardResetBitMaskFes) != (0U))
                && (Local_TimeOut < BOARD_REG_TIMEOUT));
    }


    return BoardResetReturn;
}
#endif /* ifdef PLATFORMS_STUBS_ENABLED  */


/*----------------------------------{BoardSetState}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.018,1
 */
void BoardSetState(Platforms_PowerType ubState)
{
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    BoardStubSetState(ubState);
#else
    uint32 BoardStatusReg = (uint32) 0u;
    /* Clear mode transition complete status */
    BOARD_MC_ME_IS_REG = BOARD_MC_ME_IS_MTC_SET;

    /* Current status */
    BoardStatusReg = ((BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_CURRENT_MODE) >> (uint32) 28UL);

    /* Check input States in acceptable range */
    switch (ubState)
    {
        case PLATFORMS_MCU_IDLE:
          if ((BoardStatusReg == BOARD_MC_MODE_HALT0) || (BoardStatusReg == BOARD_MC_MODE_STOP0))
            {
                BOARD_MC_ME_MCTL = ((uint32) (BOARD_MC_MODE_RUN0 << 28U) | (uint32) (BOARD_MC_ME_MCTL_KEY));
                BOARD_MC_ME_MCTL = ((uint32) (BOARD_MC_MODE_RUN0 << 28U) | (uint32) (BOARD_MC_ME_MCTL_KEY_INV));

                while(BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_MTRANS);    /* wait until mode transition is complete */
                while((BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_CURRENT_MODE) != BOARD_MC_ME_GS_RUN0);   /* check mode */
            }
            break;

        case PLATFORMS_MCU_SLEEP:
            /* equal HALT_MODE on PA*/
            /* check invalid transition */
            if ((BoardStatusReg >= BOARD_MC_MODE_RUN0) && (BoardStatusReg <= BOARD_MC_MODE_RUN3))
            {
                BOARD_MC_ME_MCTL = ((uint32) (BOARD_MC_MODE_HALT0 << 28U) | (uint32) (BOARD_MC_ME_MCTL_KEY));
                BOARD_MC_ME_MCTL = ((uint32) (BOARD_MC_MODE_HALT0 << 28U) | (uint32) (BOARD_MC_ME_MCTL_KEY_INV));

                while(BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_MTRANS);    /* wait until mode transition is complete */
                while((BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_CURRENT_MODE) != BOARD_MC_ME_GS_HALT0);   /* check mode */
            }

            break;

        case PLATFORMS_MCU_STANDBY:
            /* equal STOP_MODE on PA*/
            /* check invalid transition */
            if ((BoardStatusReg >= BOARD_MC_MODE_RUN0) && (BoardStatusReg <= BOARD_MC_MODE_RUN3))
            {
                BOARD_MC_ME_MCTL = ((uint32) (BOARD_MC_MODE_STOP0 << 28U) | (uint32) (BOARD_MC_ME_MCTL_KEY));
                BOARD_MC_ME_MCTL = ((uint32) (BOARD_MC_MODE_STOP0 << 28U) | (uint32) (BOARD_MC_ME_MCTL_KEY_INV));

                while(BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_MTRANS);    /* wait until mode transition is complete */
                while((BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_CURRENT_MODE) != BOARD_MC_ME_GS_STOP0);   /* check mode */
            }

         break;
        default :
            /* Do not change anything*/
          break;

    }

#endif
}
/*----------------------------------{end of BoardSetState}---------------------------------*/

/*----------------------------------{BoardGetRandom}---------------------------------*/
/*
 * !LINKSTO OsekCore.Platforms.Board.019,1
 */
uint32 BoardGetRandom(void)
{
    uint32 ulBoardRandom;
#ifdef PLATFORMS_STUBS_ENABLED
    /* Call stub function for test */
    ulBoardRandom=BoardStubGetRandom();
#else
    /* Get 32bits STM0 */
    ulBoardRandom = BOARD_STM0_CNT;

    /* Bit inverted */
    ulBoardRandom = ~(ulBoardRandom);

    /* Manipulate the Random value so it is not that easy to find !*/
    ulBoardRandom =
            (uint32) ((uint32) ulBoardRandom + (uint32) (ulBoardRandom << 16U)
                    + (uint32) (ulBoardRandom >> 24U));

#endif /* ifdef PLATFORMS_STUBS_ENABLED  */
    return ulBoardRandom;
}
/*----------------------------------{end of BoardGetRandom}---------------------------------*/
#ifndef PLATFORMS_STUBS_ENABLED
NCS_PRI_FCT uint8 Board_PAMagicCarpetEnterMode(u32 newMode)
{
  uint8 modeSwitchOkay = BOARD_FALSE;
  u32 modeValue;

  /* request transition to new mode */
  modeValue = (newMode & BOARD_MC_MODE_MASK) << BOARD_MC_ME_MCTL_TM_OFS;
  BOARD_MC_ME_MCTL = (modeValue | BOARD_MC_ME_MCTL_KEY);
  BOARD_MC_ME_MCTL = (modeValue | BOARD_MC_ME_MCTL_KEY_INV);

  /* wait until switching is finished */
  while ((BOARD_MC_ME_GS_REG & BOARD_MC_ME_GS_S_MTRANS) != 0)
  {
    /* wait */
  }

  /* check that new mode was accepted */
  if ( ((BOARD_MC_ME_GS_REG >> BOARD_MC_ME_MCTL_TM_OFS) & BOARD_MC_MODE_MASK) == newMode )
  {
    modeSwitchOkay = BOARD_TRUE;
  }

  return modeSwitchOkay;
}
#endif
