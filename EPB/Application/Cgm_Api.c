#include "CGM_Api.h"
#include "spc560bsp.h"

/*===========================================================================
 * MCAL Layer
 *===========================================================================*/
void Mcal_Cgm_Initialize(void)
{
  ME.RUN[0].B.FXOSC0ON = 1;				/* Enable external osc */ 
	ME.RUN[0].B.FMPLLON  = 1;				/* Enable PLL */
	ME.RUN[0].B.SYSCLK   = 0x4;			/* System clock is PLL */

	// System clock = (Clk input * NDIV) / (IDF * ODF)
	// 48Mhz = (8Mhz * 48) / (2 * 4)
	// 64mhz = (8mhz* 32) / (2 * 2)
	//Clock Sources Configurations    
	CGM.SC_DC[0].R = 0x80;									// Enable the sysclock (div=1) for all pheripherals in PS0
	CGM.SC_DC[1].R = 0x80;									// Enable the sysclock (div=1) for all pheripherals in PS2
	CGM.SC_DC[2].R = 0x80;									// Enable the sysclock (div=1) for all pheripherals in PS3

	CGM.FMPLL_CR.B.IDF  = 0x1;			/* Input Divider  = 2   */
	CGM.FMPLL_CR.B.NDIV = 32;			/* Loop Divider   = 32  */
	CGM.FMPLL_CR.B.ODF  = 0x0;			/* Output Divider = 2   */ 
	

	ME.MCTL.R = 0x40005AF0;					/* Mode & Key */
	ME.MCTL.R = 0x4000A50F;					/* Mode & Key inverted */
	while(ME.GS.B.S_MTRANS==1) {};	/* Wait for mode entry to complete */
}

/*===========================================================================
 * BSW Layer
 *===========================================================================*/
void Bsw_Cgm_Initialize(void)
{
	Mcal_Cgm_Initialize();
}

/*===========================================================================
 * FS Layer
 *===========================================================================*/
void Fs_Cgm_Initialize(void)
{
	Bsw_Cgm_Initialize();
}

/*===========================================================================
 * ASW Layer
 *===========================================================================*/
void Asw_Cgm_Initialize(void)
{
	Fs_Cgm_Initialize();
}

 /*===========================================================================
 * Test용 기존 코드
 *===========================================================================*/
void CGMInitialize(void)
{
	Mcal_Cgm_Initialize();
}


