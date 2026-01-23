// ****************************************************************************
//! @file             pit.h
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

#ifndef PIT_H
#define PIT_H


// ****************************************************************************
// ************************** Configuration Constants *************************
// ****************************************************************************
    //put the defines below to appcfg.h file to turn particular features on.

    
// ****************************************************************************
// ********************** External Constants and Variables ********************
// ****************************************************************************

   

// ****************************************************************************
// *********************** Function Prototypes ********************************
// ****************************************************************************
void PIT0_INIT(vint32_t LDVAL);
void PIT0_ENABLE(void);
void PIT0_DISABLE(void);
void PIT_CH0_ISR(void);

#endif

// ***************************** End of pit.h *****************************
