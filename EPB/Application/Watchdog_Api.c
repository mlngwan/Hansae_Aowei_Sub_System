#include "spc560bsp.h"
#include "Watchdog_Api.h"
/*===========================================================================
 * MCAL Layer
 *===========================================================================*/
void Mcal_Watchdog_Disable(void)
{ 
//	CFLASH.NVUSRO.B.WATCHDOG_EN = 0;
  SWT.SR.R = 0xc520;
  SWT.SR.R = 0xd928;
  SWT.CR.R = 0x8000010A;
}

/********************************************************************************
* Function Name		: Enable Watchdog
* Description			: Enable Watchdog function
* input						: None
* return					: None
********************************************************************************/
void Mcal_Watchdog_Enable(void)
{ 
//	CFLASH.NVUSRO.B.WATCHDOG_EN = 1;
	SWT.SR.R = 0xA602;
	SWT.SR.R = 0xB480;
	SWT.CR.R = 0x80000103;
	SWT.TO.R = 0x1F400;						// 0x1F400 is 1 sec when clock is 128KHz.
}

/********************************************************************************
* Function Name		: ClearWatchdog Timer
* Description			: Clear Watchdog Timer
* input						: None
* return					: None
********************************************************************************/
void Mcal_Watchdog_Clear_Timer(void)
{
	SWT.SR.R = 0xA602;
	SWT.SR.R = 0xB480;
	SWT.TO.R = 0x1F400;						// 0x1F400 is 1 sec when clock is 128KHz.
}

/*===========================================================================
 * BSW Layer
 *===========================================================================*/
void Bsw_Watchdog_Disable(void)
{
	Mcal_Watchdog_Disable();
}

void Bsw_Watchdog_Enable(void)
{
	Mcal_Watchdog_Enable();
}

void Bsw_Watchdog_Clear_Timer(void)
{
	Mcal_Watchdog_Clear_Timer();
}
/*===========================================================================
 * FS Layer
 *===========================================================================*/
 void Fs_Watchdog_Disable(void)
 {
	Bsw_Watchdog_Disable();
 }

 void Fs_Watchdog_Enable(void)
 {
	Bsw_Watchdog_Enable();
 }
 
 void Fs_Watchdog_Clear_Timer(void)
 {
	Bsw_Watchdog_Clear_Timer();
 }
 
/*===========================================================================
 * ASW Layer
 *===========================================================================*/
 
 /*===========================================================================
 * Test용 기존 코드
 *===========================================================================*/
 void DisableWatchdog(void)
{ 
	Mcal_Watchdog_Disable();
}

void EnableWatchdog(void)
{ 
	Mcal_Watchdog_Enable();
}

void ClearWatchdogTimer(void)
{
	Mcal_Watchdog_Clear_Timer();
}