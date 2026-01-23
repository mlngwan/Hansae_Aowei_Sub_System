//-------------------------------------------------------------------------
// FILE NAME  	: SPC560x_xcptn.c
// DEPARTMENT  : STMicroelectronics APG Group, Power Train & Safety
// AUTHOR         	: Rosario MARTORANA
// AUTHOR'S EMAIL : rosario.martorana@st.com
//-------------------------------------------------------------------------
// REVIEW(S) : 
// -----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION	DATE      	AUTHOR            		DESCRIPTION
// 0.1		                   		Rosario Martorana	First release
// -----------------------------------------------------------------------------
// KEYWORDS: 
// -----------------------------------------------------------------------------
// PURPOSE:  Monaco environment set up for SOFTWARE interrupt
// -----------------------------------------------------------------------------

#include "common.h"
 
 
#if(0) /* trueonot: use EB's vector. 2017.01.03 ++ */
/*========================================================================*/
/*				 		GLOBAL VARIABLES								  */
/*========================================================================*/
extern vuint32_t __IV_ADDR;     		// Defined in the linker file */
extern uint32_t ISRVectorTable_SW_Table[];	// Reference the Vector Table

/*========================================================================*/
/*			 			EXTERNAL PROTOTYPES								  */
/*========================================================================*/
extern void IVOR4Handler (void);
extern void IVORnTable (void);

/*========================================================================*/
/*		 					FUNCTIONS									  */
/*========================================================================*/
//!  This function intializes the IVPR and IVOR4 registers for exceptions
/*!
For debuf purpose all the exception are enabled.
*/
void initIrqVectors(void) {
	//	IVPR = address base used with IVOR's
	//	asm ("e_lis r5, __IV_ADDR@h");	 
	//	asm ("e_or2i r5, __IV_ADDR@l");
#if defined(__ghs__)
		asm ("e_lis r5, __ghsbegin_xcptn@h");
		asm ("e_or2i r5, __ghsbegin_xcptn@l");
#else
	//	asm ("e_lis r5, %startof(.xcptn)@h");
	//	asm ("e_or2i r5, %startof(.xcptn)@l");
#if(0) /* delete by trueonot : not used, 2017.01.03 	++ */
		asm ("e_lis r5, __XCPTN_ADDR@h");
		asm ("e_or2i r5, __XCPTN_ADDR@l");
#endif /* delete by trueonot : not used, 2017.01.03 	-- */
#endif
	//	asm ("e_lis r5, 0x0000");
	//	asm ("e_or2i r5, 0x1000");
	
		asm ("mtIVPR r5 ");

	
}



//!  This function intializes the INTC for software vector  mode.
/*!
Set up INTC vector table base address to the  address defined in the linker. <br>
Satellite transmission interrupt has priority 2. <br>
Diagnostic transmission interrupt has priority 3.
*/
static void initINTC(void) {	
  	
	// Initialize INTC for software vector mode
	INTC.MCR.B.HVEN = 0;
  	
	// Use the default vector table entry offsets of 4 bytes
	INTC.MCR.B.VTES = 0;	
	
	// Set INTC ISR vector table base addr.
	INTC.IACKR.R = (uint32_t) &ISRVectorTable_SW_Table[0];
}
//!  This function enables the interrupt
/*!
 It sets INTC's current priority to 0. 
External interrupts to the core are enabled.
*/
void enableIrq(void) {
  	
	// Ensure INTC's current priority is 0
	INTC.CPR.B.PRI = 0;
	
	// Enable external interrupts
	asm(" wrteei 1");
	
	//enable external interrupt and exceptions
//	asm ("e_lis r3,  0x0002");
//	asm ("e_or2i r3, 0x9000"); 
	
//	asm ("mfmsr    	r4"); 					//1 Check later
//	asm ("se_or   	r4, r3"); 
//	asm ("mtmsr    	r4");
}


//!  This function set up the environment to raise interrupt
void xcptn_xmpl(void) {
    
	// Initialise IVPR in Z1
	initIrqVectors();       
	
	// Initialize INTC for software vector mode
	initINTC();
}


#endif  /* trueonot: use EB's vector. 2017.01.03 ++ */
