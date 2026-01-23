/*********************************************************************************/
/*                                                                               */
/*                                 CAN AE2010 Layers                             */
/*                                                                               */
/* ----------------------------------------------------------------------------- */
/*                                                                               */
/*                                       DEM                                     */
/*                                                                               */
/*********************************************************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                        */
/*%%   _____________________________   %%  \file DEM_App.c                       */
/*%%  |                             |  %%                                        */
/*%%  |   &&&&&   &&&&&             |  %%  Module version: 2.4.2  */
/*%%  |   &       &    &            |  %%  Variant: RSA_CAN_HS                  */
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
/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 14.2 (required)
 *   "All non-null statements shall either: 
 *      a) have at least one side effect however executed, or 
 *      b) cause control flow to change."
 *
 *   Reason:
 *   The MISRA rule is not violated. This statement is used to avoid a compile error.
 *
 *  MISRA-2) Deviated Rule: 16.7 (advisory)
 *   A pointer parameter in a function prototype should be declared as pointer to const
 *   if the pointer is not used to modify the addressed object.
 *
 *   Reason:
 *   The MISRA rule is not violated. This pointer will be used once the function is completed by customer.
 */

/******************************** FILE INCLUSION *********************************/
/*                                                                               */
#include "EB_Prj.h"
#include "DEM_Priv.h"
#include "DEM_App.h"
/*                                                                               */
/*********************************************************************************/




/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */


/*                                                                               */
/*********************************************************************************/




/***************************** PRIVATE DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Private Constants                                                           */



/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */


/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */
/* Flag to know if an operating cycle is started or not */



/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

NCS_PRI_FCT void DEM_FillSnapshotRecord(u8 *aubUdsData, tDtcIdx ubDtcIdx);
/*                                                                               */
/*********************************************************************************/


/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */

/*----------------------------------{DEM_ReportDTCSnapshotRecordByDTCNumber}--------------------------------*/
tUdsStatus DEM_ReportDTCSnapshotRecordByDTCNumber(u16 *puwLen, u8 *aubUdsData)
{   
    tUdsStatus eUdsStatus;
    tDtcConfig ulDtcCode;
    tDtcIdx ubDtcIdx;
    tDemBoolean ubDtcFound;

    /* Initialize the internal variable */
    eUdsStatus = UDS_ACK;
    ubDtcFound = DEM_FALSE;
    
    /* input pointers are always checked in case of no EB UDS module */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {    
        /* Check if the DTCSnapshotRecordNumber is 0xFF */
        if (0xFFU == aubUdsData[5])
        {
            /* Retrieve the DTC code from UDS buffer */
            ulDtcCode =  (u32)(((u32)aubUdsData[2] << 16U) & 0x00FF0000U);
            ulDtcCode |= (u32)(((u32)aubUdsData[3] << 8U) & 0x0000FF00U);
            ulDtcCode |= (u32)((u32)aubUdsData[4] & 0x000000FFU);

            ubDtcIdx = 0U;
            /* Check if the DTC asked is configured */
            do
            {
                if(DEM_DTC_CODE(ubDtcIdx) == ulDtcCode)
                {
                    ubDtcFound = DEM_TRUE;        
                }
                else
                {
                    ubDtcIdx ++;
                }
            }
            while((ubDtcIdx < DEM_DTC_NUMBER) && (ubDtcFound == DEM_FALSE));

            if(DEM_TRUE == ubDtcFound)
            {
            
                /* HERE set the length to return the DID value required */
                /* exemple: */
                *puwLen = 20U; 
                
                /* Check if there is enough space in buffer */
                if (*puwLen <= DEM_DIAG_BUFFER_SIZE)
                {
                
                    /* Initialize the common part between confirm and not confirm DTC */      
                    aubUdsData[5] = m_astDtcLocalStatus[ubDtcIdx].ubDtcStatusMask; /* DTC status mask */
                    aubUdsData[6] = 0x01U; 
                                    
                    /* HERE fill the DID value required */
                    
                    /* Fill the snapshot record value */
                    DEM_FillSnapshotRecord(aubUdsData, ubDtcIdx);
                }            
                else
                {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
                    eUdsStatus = UDS_NRC_14;
#else
                    /* No NRC_14 in KWP spec >> the most appropritate is NRC_22 (condition not correct) */
                    eUdsStatus = UDS_NRC_22;
#endif                    
                }         
            }
            else
            {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
                eUdsStatus = UDS_NRC_31;
#else
                eUdsStatus = UDS_NRC_12;
#endif                    
            }
        }
        else
        {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
            eUdsStatus = UDS_NRC_31;
#else
            eUdsStatus = UDS_NRC_12;
#endif                    
        }
    }
    else
    {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
        eUdsStatus = UDS_NRC_31;
#else
        eUdsStatus = UDS_NRC_12;
#endif                    
    }

    return eUdsStatus;
}
/*----------------------------------{end DEM_ReportDTCSnapshotRecordByDTCNumber}----------------------------*/


/*----------------------------------{DEM_FillSnapshotRecord}----------------------------*/
/* Deviation MISRA-2 */
NCS_PRI_FCT void DEM_FillSnapshotRecord(u8 *aubUdsData, tDtcIdx ubDtcIdx)
{
   
    /* Deviation MISRA-1 */
    OSC_PARAM_UNUSED(aubUdsData);
    OSC_PARAM_UNUSED(ubDtcIdx);

    /* HERE fill the snapshot record data */

}
/*----------------------------------{end DEM_FillSnapshotRecord}----------------------------*/


/*----------------------------------{DEM_ReportDTCFaultDetectionCounter}----------------------------*/
tUdsStatus DEM_ReportDTCFaultDetectionCounter(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

    /* input pointers are always checked in case of no EB UDS module */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        /* For details about the implementation of this request, have a look on ISO 14229-1:2012(E) : */
        /* Table 269 explain the details concerning the ReportDTCFaultDetectionCounter sub-service behaviour */
        /* Table 262 explain the structure of the ReadDTCInformation request message based on ReportDTCFaultDetectionCounter sub-service (RDTCFDC) */
        /* Following Table 262, the structure of request message is length = 0x02, Request SID = 0x19 with sub-service = 0x14 */
        /* Table 278 define the response message of ReadDTCInformation request message */
        eUdsStatus = UDS_ACK;
    }
    else
    {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
            eUdsStatus = UDS_NRC_31;
#else
            eUdsStatus = UDS_NRC_12;
#endif                    
    }

    return eUdsStatus;
}
/*----------------------------------{end DEM_ReportDTCFaultDetectionCounter}----------------------------*/


/*----------------------------------{DEM_ReportDTCExtDataRecordByDTCNumber}----------------------------*/
tUdsStatus DEM_ReportDTCExtDataRecordByDTCNumber(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tDtcConfig ulDtcCode;
    tDtcIdx ubDtcIdx;
    tDemBoolean ubDtcFound;
    
    /* Initialize the internal variable */
    eUdsStatus = UDS_ACK;
    ubDtcFound = DEM_FALSE;
    
    /* input pointers are always checked in case of no EB UDS module */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {    
            /* Retrieve the DTC code from UDS buffer */
            ulDtcCode =  (u32)(((u32)aubUdsData[2] << 16U) & 0x00FF0000U);
            ulDtcCode |= (u32)(((u32)aubUdsData[3] << 8U) & 0x0000FF00U);
            ulDtcCode |= (u32)((u32)aubUdsData[4] & 0x000000FFU);

            ubDtcIdx = 0U;
            /* Check if the DTC asked is configured */
            do
            {
                if(DEM_DTC_CODE(ubDtcIdx) == ulDtcCode)
                {
                    ubDtcFound = DEM_TRUE;        
                }
                else
                {
                    ubDtcIdx ++;
                }
            }
            while((ubDtcIdx < DEM_DTC_NUMBER) && (ubDtcFound == DEM_FALSE));

            if(DEM_TRUE == ubDtcFound)
            {
                /* Here, check the DTCExtDataRecordNumber */
                switch(aubUdsData[5])
                {
                    /* Example based on Table 311 from ISO 14229-1:2012(E) : */
                    /* DTCExtDataRecordNumber = 0xFFU */
                    case 0xFFU:
                    {
                        /* Here set the length for RDTCI response based on example #7 from ISO 14229-1:2012(E) */
                        *puwLen = 10U; 
                        
                        /* Check if there is enough space in buffer */
                        if (*puwLen <= DEM_DIAG_BUFFER_SIZE)
                        {
                            /* Initialize the common part between confirm and not confirm DTC */      
                            aubUdsData[5] = m_astDtcLocalStatus[ubDtcIdx].ubDtcStatusMask; /* DTC status mask */

                            /* Here customer shall fulfil the response with DTCExtDataRecordNumber and DTCExtDataRecord */
                            /* Below example implementation based on example #7 from ISO 14229-1:2012(E) */
                               aubUdsData [6] = 0x05U; /* DTCExtDataRecordNumber 0x05   */
                               aubUdsData [7] = 0x17U; /* DTCExtDataRecord              */
                               aubUdsData [8] = 0x10U; /* DTCExtDataRecordNumber 0x10   */
                               aubUdsData [9] = 0x79U; /* DTCExtDataRecord              */
                            /* End of example */
                        }            
                        else
                        {
    #if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
                            eUdsStatus = UDS_NRC_14;
    #else
                            /* No NRC_14 in KWP spec >> the most appropriate is NRC_22 (condition not correct) */
                            eUdsStatus = UDS_NRC_22;
    #endif                    
                        }
                    }
                    break;
                    
                    default:
                        /* here customer shall manage other DTCExtDataRecordNumber */
                    break;
                }
            }
            else
            {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
                eUdsStatus = UDS_NRC_31;
#else
                eUdsStatus = UDS_NRC_12;
#endif                    
            }
    }
    else
    {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
        eUdsStatus = UDS_NRC_31;
#else
        eUdsStatus = UDS_NRC_12;
#endif                    
    }

    return eUdsStatus;
}
/*----------------------------------{end DEM_ReportDTCExtDataRecordByDTCNumber}----------------------------*/


/*----------------------------------{DEM_ReportDTCSnapshotIdentification}----------------------------*/
tUdsStatus DEM_ReportDTCSnapshotIdentification(u16 *puwLen, u8 *aubUdsData)
{
    /* Define Internal Data */
    tUdsStatus eUdsStatus;
        
    /* input pointers are always checked in case of no EB UDS module */
    if ((puwLen != NULL) && (aubUdsData != NULL))
    {
        eUdsStatus = UDS_ACK;
        /* Based on ISO 14229-1:2012(E) example#4, table 301, set the length of request message */
        *puwLen = 2U;

        /* Here return DTC number and DTCSnapshot record number */
        /* Below example of implementation based on example #4 on ISO 14229-1:2012(E) */
        /* DTCSnapshotRecordNumber#1 for DTC 0x123456 */
        aubUdsData[2] = 0x12U;
        aubUdsData[3] = 0x34U;
        aubUdsData[4] = 0x56U;
        aubUdsData[5] = 0x01U;
        *puwLen += 4U;
        /* DTCSnapshotRecordNumber#2 for DTC 0x123456 */
        aubUdsData[6] = 0x12U;
        aubUdsData[7] = 0x34U;
        aubUdsData[8] = 0x56U;
        aubUdsData[9] = 0x02U;
        *puwLen += 4U;
        /* DTCSnapshotRecordNumber#1 for DTC 0x789ABC */
        aubUdsData[10] = 0x78U;
        aubUdsData[11] = 0x9AU;
        aubUdsData[12] = 0xBCU;
        aubUdsData[13] = 0x01U;
        *puwLen += 4U;
        /* end of example */   
    }
    else
    {
#if (DEM_RSA_CAN_PROTOCOL == DEM_CAN_PROTOCOL_UDS)
        eUdsStatus = UDS_NRC_31;
#else
        eUdsStatus = UDS_NRC_12;
#endif                    
    }
    
    return eUdsStatus;
}
/*----------------------------------{end DEM_ReportDTCSnapshotIdentification}----------------------------*/
