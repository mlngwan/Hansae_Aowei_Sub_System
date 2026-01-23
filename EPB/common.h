// ****************************************************************************
// Project Name:      PPCframework
//
//! @file             common.h
//! @version          $Revision: 182 $
//! @date             $Date: 2004-10-05 15:59:58 +0200 (Tue, 05 Oct 2004) $
//! @author           Tom Rejhon
//! @brief            General constant definition
//
// Company:             STMicroelectronics
//                      Pobrezni 3
//                      186 00 Praha 8, Czech Republic
//
// Processor:           PowerPC
// Compiler:            GHS
//
// ***************************** Revision History *****************************
//
// Date           Name         Description
// 20.07.2011     Felix Min    Modified for Bolero
// 13.03.2008     Tom Rejhon   Preliminary support for SPC560B/S/P
// 18.02.2008     Tom Rejhon   Added macros ORDERING_LITTLE2BIG_32() and 
//                             ORDERING_LITTLE2BIG_16() for endiannes conversion
// 14.02.2008     Tom Rejhon   Modified to support SPC563M
// 11.05.2007     Tom Rejhon   Original Issue - based on common.h for ST10 from 14.11.2006
//
// ****************************************************************************

#ifndef COMMON
#define COMMON

#include "EB_Prj.h"
#include "typedefs.h"

#define _nop()	asm("nop")
//#define msync()	asm("msync")

typedef struct
{
    /* Deviation MISRA-2 */
    u8 ubBit0 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit1 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit2 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit3 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit4 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit5 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit6 : 1U;
    /* Deviation MISRA-2 */
    u8 ubBit7 : 1U;
} Flag_Type;
extern Flag_Type ubCanRxFrameInd1, ubCanRxFrameInd2; 
extern Flag_Type ubFlag_Fault1;
extern Flag_Type ubFlag_Fault2;
extern Flag_Type ubFlag_Buf1;
extern Flag_Type ubFlag_Buf2;
extern Flag_Type ubFlag_Buf3;
extern Flag_Type ubFlag_Buf4;
extern Flag_Type ubFlag_Buf5;
extern Flag_Type ubFlag_Buf6;
extern Flag_Type ubEpbSwitch_Flag;
extern Flag_Type ubUartRxTx_Flag;
extern Flag_Type ubIOCBI_Flag;
extern Flag_Type ubMutex_Flag1;
extern Flag_Type ubMutex_Flag2;
extern Flag_Type ubMutex_Flag3;
extern Flag_Type ubMutex_Flag4;
extern Flag_Type ubMutex_Flag5;
extern Flag_Type ubMutex_Flag6;
extern Flag_Type ubMutex_Flag7;
#endif
// ***************************** End of common.h ******************************

