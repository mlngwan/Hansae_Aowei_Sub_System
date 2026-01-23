/*********************************************************************************/
/*                                                                               */
/*                                  CAN Stack                                    */
/*                                                                               */
/* ----------------------------------------------------------------------------- */
/*                                                                               */
/*                                APP                                            */
/*                                                                               */
/*********************************************************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*%%   _____________________________   %%  \file APP_Pub.h                       */
/*%%  |                             |  %%                                        */
/*%%  |   &&&&&   &&&&&             |  %%  Module version: 3.6.4                 */
/*%%  |   &       &    &            |  %%                                        */
/*%%  |   &&&&    &&&&&             |  %%                                        */
/*%%  |   &       &    &            |  %%                                        */
/*%%  |   &&&&&   &&&&&             |  %%                                        */
/*%%  |_____________________________|  %%                                        */
/*%%                                   %%                                        */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*********************************************************************************/
/* Copyright 2010 by Elektrobit Automotive GmbH                                  */
/* All rights exclusively reserved for Elektrobit Automotive GmbH,               */
/* unless expressly agreed to otherwise.                                         */
/*********************************************************************************/

#ifndef APP_PUB_H
#define APP_PUB_H

#include "COM_Cbk.h"

extern void ERAE_DEGRADE_CLEAR(void);
extern void EEPROM_DTC_Save(void);
extern u8 DTC_Status_Cycle(u8 dtcIdx, u8 test_pass, u8 cycle);
extern u8 Temporary_Failure_Check(u8 dtcIdx);
extern void DTC_Poweroff(void);
extern u8 DTC_HW_Cycle(u8 dtcIdx, u8 fault_bit, u8 cycle);
// extern u8 DTC_CAN_Check(u8 FailIdx, u8 dtcIdx, u8 fault_bit);
// extern u8 CAN_Degrade_Control(u8 ECU_Name);
// extern u8 DTC_Group_Control(u8 FailIdx, u8 dtcIdx);

extern void DTC_BIT_SET(u8 IDX, u8 fail_confirm, u8 set);
/** \brief Initialize layer
 **
 ** This function initializes all layers of the complete STACK.
 ** EB_Init is called in this API.
 ** It shall be called only once at ECU startup.
 **
 **/

extern void APP_Init (void);

/** \brief Regular tick of the layer
 **
 ** Ensure cyclic tasks of the layer.
 **
 **/
extern void APP_Manage (void);

/** \brief Restore context at startup
 **
 ** This function shall be called by customer at startup to restore context from non volatile memory.
 ** This API shall be called after APP_Init.
 ** It may deal with monitoring status, specific frame data content...
 **
 ** Only available for CANSTACK
 **
 **/
extern void APP_RestoreContext (void);

/** \brief Report DTC by stuatus Mask for read DTC
 **
 ** This function shall be called by customer at RDTCI.
 **
 **/
extern tUdsStatus DEM_ReportDtcByStatusMask (u16 *puwLen, u8 *aubUdsData);

/** \brief Open PROGRAMMING session
 **
 ** This function is called when programming session is opened.
 ** ::PROG_Init is called to initialize PROG layer
 **
 **/
extern void PROG_OpenProgrammingSession (void);

/* Callback function prototype : CbkOnRxFrame --> OnRxFrameESP02 */
u8 OnRxFrameESP02(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameESP03 */
u8 OnRxFrameESP03(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameECU14 */
u8 OnRxFrameECU14(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameEMS3 */
u8 OnRxFrameEMS3(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameTCU */
u8 OnRxFrameTCU(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameESP04 */
u8 OnRxFrameESP04(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameESP09 */
u8 OnRxFrameESP09(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameYRS02 */
u8 OnRxFrameYRS02(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameVeh02 */
u8 OnRxFrameVeh02(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameACU01 */
u8 OnRxFrameACU01(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameState */
u8 OnRxFrameState(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameLock */
u8 OnRxFrameLock(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameESP05 */
u8 OnRxFrameESP05(tComFrameIdx uFrameIdx);
/* Callback function prototype : CbkOnRxFrame --> OnRxFrameFIU */
u8 OnRxFrameFIU(tComFrameIdx uFrameIdx);



#endif      /* APP_PUB_H */

