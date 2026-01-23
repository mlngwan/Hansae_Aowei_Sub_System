/*===============================================================================*/
/*                                                                               */
/*                                CCP_UserCustomize.c                            */
/*                       CCP functionality for user implementation               */
/*                                                                               */
/*===============================================================================*/

/*===============================================================================*/
/* All functions need user implementation for CCP should be put in this file,    */
/* user should add here specific function described in CCP Driver if some        */
/* feature are enabled.                                                          */
/*===============================================================================*/


/*==================[inclusions]=================================================*/
#include "CCP.h"
#include "EB_Prj.h"
//------------------------------------------------------------------------------
// Global

// Current Calibration page
CCP_BYTE ccpCalPage = 0; // Default ROM

/*----------------------------------------------------------------------------*/
/* Transmit the CCP message */

void ccpSend( CCP_BYTEPTR msg )
{

    COM_SetFrameData(COM_FRAME_IDX_EPB1R_01_Info, msg);
    COM_SendFrameDirect(COM_FRAME_IDX_EPB1R_01_Info);
}


/*----------------------------------------------------------------------------*/
/* Convert a memory address from CCP 8/32bit into a C pointer */

CCP_MTABYTEPTR ccpGetPointer( CCP_BYTE addr_ext, CCP_DWORD addr )
{
    return (CCP_MTABYTEPTR)addr;
}


/*----------------------------------------------------------------------------*/
// CCP Callbacks

void ccpUserBackground( void )
{
}

CCP_BYTE ccpDisableNormalOperation( CCP_MTABYTEPTR a, CCP_WORD s )
{
    return 1;
}


/*----------------------------------------------------------------------------*/
/* Calibration RAM/ROM Selection */


CCP_DWORD ccpGetCalPage( void )
{
    return (CCP_DWORD)ccpCalPage;
}

void ccpSetCalPage( CCP_DWORD a )
{

}

void ccpInitCalPage( void )
{

}
