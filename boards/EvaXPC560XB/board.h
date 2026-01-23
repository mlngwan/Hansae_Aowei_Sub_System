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
*%%   _____________________________   %%  \file Board.h                         *
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
* Copyright 2013 by Elektrobit Automotive GmbH                                  *
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
#ifndef __BOARD_H__
#define __BOARD_H__


#ifndef BOARD_ASM

#include "TSAutosar.h"
#include <Platforms_asm_PA.h>
#include <ncs.h>
#include <board_Pub.h>
#ifdef PLATFORMS_STUBS_ENABLED
#include <BoardStub.h>
#endif

#ifdef BOARD_ISR_TEST
extern volatile u32 isrCounter;
#endif /* BOARD_ISR_TEST */

extern u32 IntcVectorTableHw[];

extern uint8 m_ubBoardIntLockNestingCounter;

#endif /* BOARD_ASM */


/* ********************************************
 * General configuration
 * ********************************************
*/
#define BOARD_TRUE                      1U
#define BOARD_FALSE                     0U

#define BOARD_OSC_FREQ_MHZ  8

#define BOARD_TRK_MPC5604B  1U

#define BOARD_NB_VECTORS    154U
#define BOARD_NB_EXCEPTIONS  16U
/* TODO : fix the ISR nesting bug (OSCPLATFORMS-124) */
#define BOARD_NESTINGBUG_NOTFIXED

/* ******************
 * Startup features *
 * ******************
*/

/* start address of SRAM to initialize */
#define INIT_SRAM_START                 0x40000000

/* size of SRAM to initialize (total size minus RamNoInit) */
#define INIT_SRAM_SIZE                  (0x8000)

/* SRAM NoInit address */
#define INIT_SRAM_START_NOINIT          (INIT_SRAM_START+INIT_SRAM_SIZE)
#define INIT_SRAM_SIZE_NOINIT           0x100


/* Preprocessing symbols for startup
 * !LINKSTO OsekCore.Platforms.Board.020,1
 */

/* Derivate always runs in lock step mode. An initialisation
** for all register is needed to avoid unexpected system behavior.*/
#ifndef BOARD_INIT_CPU_REG
#define BOARD_INIT_CPU_REG              BOARD_TRUE
#endif


/* Initialize SRAM. If enabled, cstartup.s will initialize the SRAM according
 * to the following settings. This is needed for ECC enabled systems where
 * initializing the C bss and data sections is not enough.
 * set to 1 to enable initialization, to 0 to disable. */
#ifndef BOARD_INIT_SRAM
#define BOARD_INIT_SRAM                 BOARD_TRUE
#endif

#ifndef BOARD_STARTUP_CLOCK_ENABLE
#define BOARD_STARTUP_CLOCK_ENABLE      BOARD_FALSE
#endif

#ifndef BOARD_WDG_USED
#define BOARD_WDG_USED                  BOARD_TRUE
#endif

#ifndef BOARD_INIT_IT
#define BOARD_INIT_IT                   BOARD_TRUE
#endif

#ifndef BOARD_INTERRUPT_ENABLE
#define BOARD_INTERRUPT_ENABLE          BOARD_TRUE
#endif

/* This feature is developped for demo boards, but in some project specific 
 * cases the board is a real ECU.
 * In this case it can be risky to pilote external port that might be link 
 * to a specific hardware component.
 */
#ifndef BOARD_LED_USED 
#define BOARD_LED_USED                  BOARD_FALSE
#endif

#ifndef BOARD_CHECK_SECTION_BASEADDR 
#define BOARD_CHECK_SECTION_BASEADDR    BOARD_TRUE
#endif

#define EXCEPTION_TABLE_AGLINMENT       0xFFF
#define INTC_TABLE_OFFSET               0x800
/* **********************
 * Output configuration *
 * **********************
*/

/* Program PLL to 50Mhz */

/* Note: The following 3 values are the values set directly to the register.
         Not the logical values ldf,idf and odf !
*/
/* Output clock = (CLKin*NDIV) / (IDF*ODF) */
#define FMPLL_IDF			0x0  /* => idf=1  */
#define FMPLL_ODF		  0x2  /* => odf=8  */
#define FMPLL_NDIV		0x32 /* => ldf=50 */
#define FMPLL_VALUE	  ( (FMPLL_IDF << 26) | (FMPLL_ODF << 24) | (FMPLL_NDIV << 16) )


/* Used by timer units: Board runs with 50Mhz */
#define BOARD_SYSTEM_CLOCK_MHZ  50L
#define BOARD_SYSTEM_CLOCK_HZ   50000000L

/*1µS is the minimal timer available*/
#define BOARD_TIMER_MINIMAL 1L


/* Timer Initialisation - schedule to 1ms = 1000µS. */
#define BOARD_SCHEDULER 1000
/* *******************************
 * Periodic Timer  configuration *
 * ******************************/
#define BOARD_PIT_BASE_ADR      0xC3FF0000UL
#define BOARD_PIT_MCR           (*(volatile u32 *)(BOARD_PIT_BASE_ADR + 0x00U))
#define BOARD_PIT_CHAN_LDVAL(x) (*(volatile u32 *)(BOARD_PIT_BASE_ADR + 0x100u + (x)*(0x10U) + 0x00U))
#define BOARD_PIT_CHAN_CVAL(x)  (*(volatile u32 *)(BOARD_PIT_BASE_ADR + 0x100u + (x)*(0x10U) + 0x04U))
#define BOARD_PIT_CHAN_TCTRL(x) (*(volatile u32 *)(BOARD_PIT_BASE_ADR + 0x100u + (x)*(0x10U) + 0x08U))
#define BOARD_PIT_CHAN_TFLG(x)  (*(volatile u32 *)(BOARD_PIT_BASE_ADR + 0x100u + (x)*(0x10U) + 0x0CU))

/* *******************************
 * System Timer  configuration *
 * ******************************/
#define BOARD_STM0_BASE_ADR     0xFFF3C000UL /*table 2.2 of Leopard RM Rev9 */
#define BOARD_STM0_CR           (*(volatile u32 *)((BOARD_STM0_BASE_ADR + 0x0U)))
#define BOARD_STM0_CR_TEN       0x00000001U
#define BOARD_STM0_CNT          (*(volatile u32 *)((BOARD_STM0_BASE_ADR + 0x4U)))

/* **************************
 * Interrupt configuration *
 * *************************/
#define BOARD_INTC_BASE_ADR   0xFFF48000UL
#define BOARD_INTC_MCR        (*(volatile u32 *)(BOARD_INTC_BASE_ADR + 0x00U))
#define BOARD_INTC_MCR_HVEN   (0x00000001UL)
#define BOARD_INTC_MCR_VTES   (0x00000000UL)
#define BOARD_INTC_CPR        (*(volatile u32 *)(BOARD_INTC_BASE_ADR + 0x08U))
#define BOARD_INTC_IACKR      (*(volatile u32 *)(BOARD_INTC_BASE_ADR + 0x10U))
#define BOARD_INTC_EOIR       (*(volatile u32 *)(BOARD_INTC_BASE_ADR + 0x18U))
#define BOARD_INTC_PSR(x)     (*(volatile u8 *)(BOARD_INTC_BASE_ADR + 0x40U + (x)))
#define BOARD_INTC_NB_PSR     147U
#define BOARD_INTC_PSR_PRIO_4 4U
#define BOARD_INTC_PSR_PRIO   0U
#define BOARD_INTC_CPU_PRIO   0U
#define BOARD_INTC_CHOOSE_CPU0          BOARD_INTC_CPU_PRIO

#define BOARD_INTC_SSCIR_0  (*(volatile u32 *)(BOARD_INTC_BASE_ADR + 0x20U))
#define BOARD_INTC_SSCIR_0_SET3 (0x02U) /* write SET0 to '1'*/
#define BOARD_INTC_SSCIR_0_CLR3 (0x01U) /* write CLR0 to '1'*/

#define EXIT_BOARD_INTERRUPT_HANDLER    (BOARD_INTC_EOIR = 0U)

#define BOARD_INTC_IACKR_INTVEC   (0x000007FCUL)

#ifdef BOARD_ISR_CAN0
#define INTC_FLEXCAN0_ESR_ERR            65U
#define INTC_FLEXCAN0_ESR_BOFF           66U
#define INTC_FLEXCAN0_BUF_00_03          68U
#define INTC_FLEXCAN0_BUF_04_07          69U
#define INTC_FLEXCAN0_BUF_08_11          70U
#define INTC_FLEXCAN0_BUF_12_15          71U
#define INTC_FLEXCAN0_BUF_16_31          72U
#define INTC_FLEXCAN0_BUF_32_63          73U


#endif


#define INTC_STM_0         				 30U
#define INTC_UART0         				 79U

#define INTC_DSPI1_SR_RFDF          	 98U

#ifdef BOARD_ISR_TEST
#define INTC_TEST                       3U
#endif

#define TRIGGER_INT() do { \
    volatile int i; \
    BOARD_INTC_SSCIR_0 |= BOARD_INTC_SSCIR_0_SET3; \
    for(i=40; i > 0; i--) { } \
  } while(0)

/* In Board.h, the BOARD_GET_ISR_SRC sould be defined in order to have the index
 * Vector of the peripheral or software-settable interrupt request that caused the interrupt
 * request to the processor.
 * !LINKSTO OsekCore.Platforms.Board.023, 1
 */
#define BOARD_GET_ISR_SRC()             (u32)((BOARD_INTC_IACKR & BOARD_INTC_IACKR_INTVEC) >> 2U);

/* **************************
 * Mode Entry configuration *
 * *************************/
#define BOARD_MC_MODE_RESET      0
#define BOARD_MC_MODE_TEST       1
#define BOARD_MC_MODE_SAFE       2
#define BOARD_MC_MODE_DRUN       3
#define BOARD_MC_MODE_RUN0       4
#define BOARD_MC_MODE_RUN1       5
#define BOARD_MC_MODE_RUN2       6
#define BOARD_MC_MODE_RUN3       7
#define BOARD_MC_MODE_HALT0      8
#define BOARD_MC_MODE_STOP0      10
#define BOARD_MC_MODE_MASK       0x0FU

#define BOARD_MC_BASE_ADR           0xC3FDC000UL
#define BOARD_MC_MODE_MASK          0x0FU
#define BOARD_MC_ME_MCTL_TM_OFS     28
  /* Mode control register */
#define BOARD_MC_ME_MCTL                (*(volatile u32 *)(BOARD_MC_BASE_ADR + 4U))
#define BOARD_MC_ME_MCTL_MODE_DRUN      (0x30000000UL)
#define BOARD_MC_ME_MCTL_MODE_RUN0      (0x40000000UL)
#define BOARD_MC_ME_MCTL_MODE_RST       (0x00000000UL)
#define BOARD_MC_ME_MCTL_KEY            (0x00005AF0UL)
#define BOARD_MC_ME_MCTL_KEY_INV        (0x0000A50FUL)

/* Transition */
#define BOARD_MC_ME_IS_REG             (*(volatile u32 *)(BOARD_MC_BASE_ADR + 0xCU))
#define BOARD_MC_ME_IS_MTC_SET         ((uint32)(((uint32)1)<<0))

  /* Global status register */
#define BOARD_MC_ME_GS_REG            (*(volatile u32 *)(BOARD_MC_BASE_ADR))
#define BOARD_MC_ME_GS_S_MTRANS       (0x08000000UL)
#define BOARD_MC_ME_GS_S_CURRENT_MODE (0xF0000000UL)
#define BOARD_MC_ME_GS_DRUN           (0x30000000UL)
#define BOARD_MC_ME_GS_RUN0           (0x40000000UL)
#define BOARD_MC_ME_GS_HALT0          (0x80000000UL)
#define BOARD_MC_ME_GS_STOP0          (0xA0000000UL)
  /* Mode enable register */
#define BOARD_MC_ME_ME_REG          (*(volatile u32 *)(BOARD_MC_BASE_ADR + 8U))
  /* Mode configuration register */
#define BOARD_MC_ME_MODE_MC_REG(mode) \
    (((u32 *)(BOARD_MC_BASE_ADR + 0x20U ))[(mode)])

/*******************************************************************************
 * MC_RGM (mode control reset generation module) registers, bits used for configuration
 ******************************************************************************/
#define BOARD_MC_RGM_BASE     (0xC3FE4000)
#define BOARD_MC_RGM_FES      (*(volatile u16 *)(BOARD_MC_RGM_BASE+0x0))
#define BOARD_MC_RGM_DES      (*(volatile u16 *)(BOARD_MC_RGM_BASE+0x2))

#define BOARD_MC_RGM_DES_ADD            (BOARD_MC_RGM_BASE+0x02)
#define BOARD_MC_RGM_FES_ADD            (BOARD_MC_RGM_BASE+0x0)

#define BOARD_MC_RGM_DES_SOFT           0x0004 /* Software Generated Destructive Reset */
#define BOARD_MC_RGM_DES_POR            0x8000 /* Power-On Reset */

#define BOARD_MC_RGM_FES_SOFT           0x0004 /* software 'functional' reset */
#define BOARD_MC_RGM_FES_EXR            0x8000 /* External Reset */


/* Initialize the RAM_NO_INIT if one these flags is set */
#define BOARD_MC_RGM_DES_NOINIT         ( BOARD_MC_RGM_DES_POR)
#define BOARD_MC_RGM_FES_NOINIT         ( BOARD_MC_RGM_FES_EXR)

#define PLATFORMS_RGM_DES_FES_REG_MASK 0xFFFFFFFF
/* arbitratry CPU cycle number*/
#define BOARD_REG_TIMEOUT 10000

/* **********************
 * FMPLL configuration *
 * **********************/
/* FMPLL */
#define BOARD_FMPLL0_BASE_ADR       0xC3FE00A0U
#define BOARD_FMPLL0_CR_REG         (*(volatile u32 *)(BOARD_FMPLL0_BASE_ADR ))       /* Control Register  */
#define BOARD_FMPLL0_MR_REG         (*(volatile u32 *)(BOARD_FMPLL0_BASE_ADR + 0x04)) /* Modulation register */

/* Control Status Register     */
#define BOARD_CMU0_CSR_REG    (*(volatile u32 *)(0xC3FE0100U))

/* **********************
 * SWT configuration *
 * **********************
*/
/* Software Watchdog */
#define BOARD_SWT0_BASE_ADR 0xFFF38000U
#define BOARD_SWT0_CR_REG  (*(volatile u32 *)(BOARD_SWT0_BASE_ADR ))           /* SWT0 Control Register   */
#define BOARD_SWT0_SR_REG  (*(volatile u32 *)(BOARD_SWT0_BASE_ADR + 0x10 ))    /* SWT0 Service Register   */

#define BOARD_SWT0_CR_MAP0  (0x80000000UL)
#define BOARD_SWT0_CR_RIA (0x00000100UL)
#define BOARD_SWT0_CR_1   (0x00000008UL)
#define BOARD_SWT0_CR_FRZ (0x00000002UL)

#define BOARD_SWT0_SR_KEY_1 0xc520
#define BOARD_SWT0_SR_KEY_2 0xd928

/* *****************************
 * SIUL configuration for LEDs *
 * *****************************
*/
#define BOARD_SIUL_BASE_ADR	0xC3F90000

#define BOARD_SIUL_PSMI_REG(x)  (*(volatile u8 *)(BOARD_SIUL_BASE_ADR + 0x0500 + (x)))

#define BOARD_SIUL_GPDO_REG(x)	(*(volatile u8 *)(BOARD_SIUL_BASE_ADR + 0x0600 + (x)))
#define BOARD_SIUL_GPDO_68_REG	BOARD_SIUL_GPDO_REG(68)/* GPIO pin data output registers 68 */
#define BOARD_SIUL_GPDO_69_REG	BOARD_SIUL_GPDO_REG(69)	/* GPIO pin data output registers 69 */
#define BOARD_SIUL_GPDO_70_REG	BOARD_SIUL_GPDO_REG(70)/* GPIO pin data output registers 70 */
#define BOARD_SIUL_GPDO_71_REG	BOARD_SIUL_GPDO_REG(71)/* GPIO pin data output registers 71 */


#define BOARD_SIUL_PCR_REG(x)	(*(volatile u16 *)(BOARD_SIUL_BASE_ADR + 0x0040 + (x)*2))
#define BOARD_SIUL_PCR_68_REG	BOARD_SIUL_PCR_REG(68)		/* Pad configuration registers 68 */
#define BOARD_SIUL_PCR_69_REG	BOARD_SIUL_PCR_REG(69)		/* Pad configuration registers 69 */
#define BOARD_SIUL_PCR_70_REG	BOARD_SIUL_PCR_REG(70)		/* Pad configuration registers 70 */
#define BOARD_SIUL_PCR_71_REG	BOARD_SIUL_PCR_REG(71)		/* Pad configuration registers 71 */

#define BOARD_ME_RUN_PC0    (*(volatile u32 *)(0xC3FDC080))   /* Peripheral Configuration Register PC0  */
#define BOARD_PCTL068       (*(volatile u16 *)(0xC3FDC104))   /* Peripheral Control Register 68 (SIUL)*/
#define BOARD_ME_PS2_REG    (*(volatile u32 *)(0xC3FDC068))   /* Peripheral Status Register 2 */


#define LEDS_SET(x) \
do \
{ \
  BOARD_SIUL_GPDO_68_REG = ((((x) & 0x8) == 0) ? 1 : 0); \
  BOARD_SIUL_GPDO_69_REG = ((((x) & 0x4) == 0) ? 1 : 0); \
  BOARD_SIUL_GPDO_70_REG = ((((x) & 0x2) == 0) ? 1 : 0); \
  BOARD_SIUL_GPDO_71_REG = ((((x) & 0x1) == 0) ? 1 : 0); \
} \
while ( 0 )

#define LEDS_INIT() \
do {\
	BOARD_SIUL_GPDO_68_REG |= 0x01;\
	BOARD_SIUL_GPDO_69_REG |= 0x01;\
	BOARD_SIUL_GPDO_70_REG |= 0x01;\
	BOARD_SIUL_GPDO_71_REG |= 0x01;\
	BOARD_SIUL_PCR_68_REG = 0x202;	\
	BOARD_SIUL_PCR_69_REG = 0x202;	\
	BOARD_SIUL_PCR_70_REG = 0x202;	\
  BOARD_SIUL_PCR_71_REG = 0x202;	\
} while (0)



#endif
