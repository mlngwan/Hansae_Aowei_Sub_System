#include "common.h"
/*===========================================================================
 * MCAL Layer
 *===========================================================================*/
 void Mcal_PIT0_Init(vint32_t LDVAL)
 {
	   PIT.CH[0].LDVAL.R         = LDVAL;
	   PIT.CH[0].TCTRL.B.TIE     = 1;
	   PIT.CH[0].TCTRL.B.TEN     = 1;
	   PIT.PITMCR.B.MDIS         = 0;
  
    INTC.PSR[59].R = 0x01;  /* PIT0 interrupt priority level */
 }
 
 void Mcal_PIT0_Enable(void)
 {
    PIT.PITMCR.B.MDIS              = 0;         // enable PIT
    PIT.PITMCR.B.FRZ               = 1;         // stop timer during debug
 }

 void Mcal_PIT0_Disable(void)
 {
	 PIT.CH[0].TCTRL.B.TEN     = 0;
 }

 void Mcal_PIT_CH0_ISR(void)
 {
   PIT.CH[0].TFLG.B.TIF    = 1;  // clear flag  
   //	SIU.GPDO[PORT_PIN_A1].R = ~SIU.GPDO[PORT_PIN_A1].R;     // toggle pin PE4
 }

/*===========================================================================
 * BSW Layer
 *===========================================================================*/

 void Bsw_PIT0_Init()
 {

 }

 void Bsw_PIT0_Enable(void)
 {
	
 }

 void Bsw_PIT0_Disable(void)
 {
	
 }

 void Bsw_PIT_CH0_ISR(void)
 {
	
 }
 /*===========================================================================
 * FS Layer
 *===========================================================================*/
 /*===========================================================================
 * ASW Layer
 *===========================================================================*/
 /*===========================================================================
 * Test용 기존 코드
 *===========================================================================*/
 // ****************************************************************************
//! @file             pit.c
//! @version          $Revision: $
//! @date             $Date:$
//! @author           ted.lim@st.com
//! @brief            Template for C source file
//
// Company:             <fill in comapny name>
//                      <fill in street>
//                      <fill in zip> <fill in city> <fill in country>
//
// Processor:           <PPC>
// Compiler:            <GHS>
//
// ***************************** Revision History *****************************
//
// Date           Name         Description
// 8/5/2013     ted lim        PIT
//
// ****************************************************************************




// ****************************************************************************
// ************************ Internal Constants and Variables ******************
// ****************************************************************************

   
// ****************************************************************************
// ***************************** Functions ************************************
// ****************************************************************************


/*******************************************************************************
 * Function Name  : PIT0_INIT
 * Description    : PIT0 initialisation routine
 * Input          : LDVAL
 * Output         : None
 * Return         : None
 *******************************************************************************/
void PIT0_INIT(vint32_t LDVAL)
{
	 PIT.CH[0].LDVAL.R         = LDVAL;
	 PIT.CH[0].TCTRL.B.TIE     = 1;
	 PIT.CH[0].TCTRL.B.TEN     = 1;
	 PIT.PITMCR.B.MDIS         = 0;
  
	INTC.PSR[59].R = 0x01;  /* PIT0 interrupt priority level */
}

/*******************************************************************************
 * Function Name  : PIT0_ENABLE
 * Description    : PIT0 initialisation routine
 * Input          : void
 * Output         : None
 * Return         : None
 *******************************************************************************/
void PIT0_ENABLE(void)
{
    PIT.PITMCR.B.MDIS              = 0;         // enable PIT
    PIT.PITMCR.B.FRZ               = 1;         // stop timer during debug
}

/*******************************************************************************
 * Function Name  : PIT0_DISABLE
 * Description    : PIT0 initialisation routine
 * Input          : void
 * Output         : None
 * Return         : None
 *******************************************************************************/
void PIT0_DISABLE(void)
{
	 PIT.CH[0].TCTRL.B.TEN     = 0;
}


void PIT_CH0_ISR(void)
{
	PIT.CH[0].TFLG.B.TIF    = 1;  // clear flag  
//	SIU.GPDO[PORT_PIN_A1].R = ~SIU.GPDO[PORT_PIN_A1].R;     // toggle pin PE4
}
