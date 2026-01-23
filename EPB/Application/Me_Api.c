#include "ME_Api.h"
#include "spc560bsp.h"

void MEInitialize(void)
{

	//Modes Configurations 
	ME.MER.R = 0x000025FF;										// enable all modes

	/* constants for ME.PCTL.R register configuration (peripheral clock gating) */
	ME.PCTL[92].B.RUN_CFG=0;
	ME.PCTL[92].B.LP_CFG=0;	
	    
	//Configuring Periphs
	ME.RUNPC[0].R = 0x000000FE;								// Enable Periphs in all running modes
	ME.LPPC[0].R= 0x00002500;									// Enable Periphs in low power modes
	//ME_PCTL[0..143]=0 by default


	// Re-enter in DRUN mode to update the configuration	
	ME.MCTL.R = 0x30005AF0;										// DRUN Mode & Key
	ME.MCTL.R = 0x3000A50F;										// DRUN Mode & Key

	// ME.MCTL.R = 0xD0005AF0;										 // STANDBY Mode & Key
	//ME.MCTL.R = 0xD000A50F;										 // STANDBY Mode & Key

	while (ME.IS.B.I_MTC != 1) {}							// Wait Until transition completed
	ME.IS.B.I_MTC = 1;												// Clear bit

}



