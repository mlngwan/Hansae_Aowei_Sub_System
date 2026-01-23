/*******************************************************************
* PROJECT : EEPROM Emulation Driver (EED) For C90 flash
* FILE    : eed_middlelevel.c
*
* DESCRIPTION : This file contains low level EED API implementation 
*
* COPYRIGHT :(c) 2009, Freescale & STMicroelectronics
*
* VERSION   : BETA 0.9
* DATE      : 05.28.2009
* AUTHOR    : Leonardo Colombo
*
* HISTORY :
* 2008.01.15    Arvind Awasthi    Initial Version for JPC563M target
* 2009.05.28    Leonardo Colombo  Porting for SPC560B64/MPC5607B devices 
*******************************************************************/
#include "common.h"
#include "ee_types.h"
#include "ee_emulation.h"

/*******************************************************************
| external declarations
|------------------------------------------------------------------*/

/*******************************************************************
| defines and macros (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| typedefs and structures (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| global variable definitions (scope: module-exported)
|------------------------------------------------------------------*/

/*******************************************************************
| global variable definitions (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| function prototypes (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| function implementations (scope: module-local)
|------------------------------------------------------------------*/

asm UINT32 read_MSR(void)
{
  mfmsr	r3
  wrteei 0
}

asm void write_MSR(UINT32 value)
{
% reg value
  mtmsr	value
}

/*******************************************************************
| function implementations (scope: module-exported)
|------------------------------------------------------------------*/

void FSL_ReadRecordHead(UINT32 address, DATA_RECORD_HEAD *pLocRecHead)
{
  DATA_RECORD_HEAD* pRH =  (DATA_RECORD_HEAD*)(address);
  pLocRecHead->dataStatus = pRH->dataStatus;
  pLocRecHead->dataID = pRH->dataID;
  pLocRecHead->dataSize = pRH->dataSize;
  pLocRecHead->wordData = pRH->wordData;
}


UINT32 FSL_SearchRecordInBlock (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT32 bufferAddress, UINT32 bufferSize, UINT16 startID, UINT32* startAddrInBlock, UINT16* nextStartID, void(*CallBack)(void))
{
    UINT32          suspendState;       /* suspend state*/
    UINT32          returnCode;         /* return code*/
    UINT32          endID;              /* define the data ID range*/
    UINT32          index;              /* loop index*/
    UINT32          temp;               /* temporary variable*/
    UINT32          recordLength;       /* the length of the record*/
    UINT64          status;             /* record status*/
    DATA_RECORD_HEAD   record;          /* local data record head structure*/
   	UINT32			       temp_MSR,enable_MSR;/* temporary space for MSR*/

    /* initialize the return code*/
    returnCode = EE_OK;

    /* calculate the boundary of the data ID range*/
    endID = startID + bufferSize / WORD_SIZE;

    /* calculate the end address of the specific block*/
    temp = blockConf->blockStartAddr + blockConf->blockSize;

    /* check high voltage operation*/
    suspendState = FSL_FlashSuspend(c90flRegBase);

    for (index = 0; index < NUMBER_OF_SEARCHING_RECORD_IN_BLOCK; index++)
    {
        /* callback service*/
        if ((index % CALLBACK_SEARCHING_BYTES_NUMBER) == 0)
        {
            if(NULL_CALLBACK != CallBack)
            {
               CallBack();
            }
        }

        /* check the record address is within the block range*/
        if (*startAddrInBlock >= temp)
        {
            /* set the blank space pointer*/
            blockConf->blankSpace = temp;

            /* finish searching block*/
            break;
        }

	       /*save old MSR*/
	       temp_MSR = read_MSR();
	       enable_MSR = 0x00208200;
	       write_MSR(enable_MSR);

        /*Set ECC Error Module flag*/
        eccErrorModule_Flag = EE_MODULE;

        /* assign data record pointer*/
		      FSL_ReadRecordHead(*startAddrInBlock, &record);

        /*Reset ECC Error Module flag*/
        eccErrorModule_Flag = NONE;

        /*Enable interrupts*/
        /*restore MSR*/
		      write_MSR(temp_MSR);

        /* check the record status type*/
        status = record.dataStatus;
        if ((status == RECORD_STATUS_PROGRAM_COMPLETE) && (FALSE == eccErrorStatus_Flag))
        {
           /* this is valid record and check the ID*/
           if ((record.dataID >= startID) && (record.dataID < endID))
           {
               if ((record.dataID == dataIDInternal && TRUE == blockSwap_Flag ))
               {
                  /*do nothing for this record when block swapping is being done*/
               }
               else
               {
                  /* it is within the data ID range then save it to buffer*/
                  WRITE32((bufferAddress + (record.dataID - startID) * WORD_SIZE), *startAddrInBlock);
               }
            }
            else if ((record.dataID >= endID) && (record.dataID < *nextStartID))
            {
                /* the start ID for next call*/
                *nextStartID = record.dataID;
            }
        }
        else if ((status == RECORD_STATUS_PROGRAM_START) && (FALSE == eccErrorStatus_Flag))
	       {
	          /*that means this data record is not valid and
           ID&SIZE field is unavailable, skip this record*/
           recordLength = PAGE_SIZE;
       }
       else if ((status == RECORD_STATUS_PROGRAM_IDSIZE) && (FALSE == eccErrorStatus_Flag))
	      {
	   	       // that means this data record is not valid and
            // ID&SIZE field is valid, skip this record
       }
       else if ((status == RECORD_STATUS_DELETE) && (FALSE == eccErrorStatus_Flag))
       {
            /* check the ID*/
           if ((record.dataID >= startID) && (record.dataID < endID))
           {
              /* delete found result*/
             WRITE32((bufferAddress + (record.dataID - startID) * WORD_SIZE), 0xFFFFFFFF);
           }
       }
       else if ((status == RECORD_STATUS_ERASED) && (FALSE == eccErrorStatus_Flag))
       {
            /* while encounter the erased state data record*/
            /* that means no data available in remained space*/

            /* set the blank space pointer*/
            blockConf->blankSpace = *startAddrInBlock;

            /* end of searching operation*/
            *startAddrInBlock = temp;

            /* check if it needs resume*/
            FSL_FlashResume(c90flRegBase, suspendState);

            return (returnCode);
       }
       else
       {
            /*reset ECC Error Status Flag*/
	           eccErrorStatus_Flag = FALSE;

            /*Set record invalid flag*/
            recordStatusInvalid_Flag = TRUE;

            /* check if it needs resume*/
            FSL_FlashResume(c90flRegBase, suspendState);

            /* end of searching operation*/
            *startAddrInBlock = temp;
            return (returnCode);

        }

        if(status == RECORD_STATUS_PROGRAM_START)
        {
		      /*do nothing*/
	       }
        /* get the data length including status, ID and size field*/
        else if (record.dataSize <= (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE)))
        {
           /* less than or equal to (Flash Pagesize-(Status Size + Record Size+ID size)) bytes data*/
           recordLength = PAGE_SIZE;
        }
        else
        {
           /* increment by pages if record length is greater than (Flash Pagesize-(Status Size + Record Size+ID size)) bytes*/
           if (( (record.dataSize - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) % PAGE_SIZE) )
              recordLength = PAGE_SIZE + PAGE_SIZE * ( (record.dataSize - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) / PAGE_SIZE + 1);
           else
              recordLength = PAGE_SIZE + PAGE_SIZE * ( (record.dataSize - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) / PAGE_SIZE);
        }

        /* prepare for next data record*/
        *startAddrInBlock = *startAddrInBlock + recordLength;
    }

    /* check if it needs resume*/
    FSL_FlashResume(c90flRegBase, suspendState);

    return (returnCode);
}


UINT32 FSL_FlashProgramStart (UINT32 c90flRegBase, UINT32* dest, UINT16* size, UINT32* source)
{
    UINT32 counter;                 /* loop counter*/
    UINT32 sourceIndex;             /* source address index*/
    UINT32 temp;                    /* temporary variable*/
    UINT64 buffer[4];               /* internal source data buffer*/

    /* Anything to program?*/
    if ( *size == 0 )
    {
        return (EE_OK);
    }

    /* check the high voltage operation*/
    if (READ32(c90flRegBase) & (C90FL_MCR_ERS | C90FL_MCR_PGM))
    {
        /* if any P/E operation in progress, return error*/
        return (EE_INFO_HVOP_INPROGRESS);
    }

    /* calculate the size to be programmed within the page boundary*/
    temp = *dest;
    temp = ((temp / PAGE_SIZE_BPS) + 1) * PAGE_SIZE_BPS - temp;

    /* there are remained data need programming*/
    /* padding for not alignment data*/
    for ( counter = 0; counter < (sizeof(buffer) / DWORD_SIZE); counter++)
    {
        buffer[counter] = CREATE_WORD64(ERASED_HIGH, ERASED_LOW);
    }

    /* copy one page to page buffer*/
    for (sourceIndex = 0; (sourceIndex < temp) && (*size > 0); sourceIndex++)
    {
        /* copy data*/
        WRITE8(((UINT32)buffer + sourceIndex), FSL_FlashRead8(*source));

        /* update source data pointer*/
        *source += 1;

        *size -= 1;
    }

    /* program this page*/
    temp = ((sourceIndex + DWORD_SIZE - 1) / DWORD_SIZE);

    /* Set MCR PGM bit*/
    SET32(c90flRegBase, C90FL_MCR_PGM);

    /* Program data within one page*/
    for (sourceIndex = 0; sourceIndex < temp; sourceIndex++)
    {
#if defined(C90_DFO)
        /* Programming interlock write*/
        WRITE32(*dest, (UINT32)(buffer[sourceIndex]>>32));

        *dest += DWORD_SIZE/2;
#else
        /* Programming interlock write*/
        WRITE64(*dest, buffer[sourceIndex]);

        *dest += DWORD_SIZE;
#endif        
    }

    /* Set MCR EHV bit*/
    SET32(c90flRegBase, C90FL_MCR_EHV);

    return (EE_OK);
}

UINT32 FSL_FlashEraseStart (UINT32 c90flRegBase, UINT32 interlockWriteAddress, UINT32 lowEnabledBlock, UINT32 midEnabledBlock, UINT32 highEnabledBlock)
{

    /* check the high voltage operation*/
    if (READ32(c90flRegBase) & (C90FL_MCR_ERS | C90FL_MCR_PGM))
    {
        /* if any P/E operation in progress, return error*/
        return (EE_INFO_HVOP_INPROGRESS);
    }

    /* Set MCR ERS bit*/
    SET32(c90flRegBase, C90FL_MCR_ERS);

    /* prepare low enabled blocks*/
    lowEnabledBlock &= 0x0000FFFF;

    /* prepare middle enabled blocks*/
    midEnabledBlock &= 0xF;
    midEnabledBlock = midEnabledBlock << 16;

    /* prepare high enabled blocks*/
    highEnabledBlock &= 0x0FFFFFFF;

    /* write the block selection registers*/
    WRITE32 ((c90flRegBase + C90FL_LMS), (lowEnabledBlock | midEnabledBlock));
    WRITE32 ((c90flRegBase + C90FL_HBS), highEnabledBlock);

    /* Interlock write*/
    WRITE32(interlockWriteAddress, 0xFFFFFFFF);

    /* Set MCR EHV bit*/
    SET32(c90flRegBase, C90FL_MCR_EHV);

    return (EE_OK);
}


UINT32 FSL_FlashCheckStatus (UINT32 c90flRegBase)
{
    UINT32 returnCode;          /* return code*/

    /* initialize return code*/
    returnCode = EE_OK;

    /* Check if MCR DONE is set*/
    if (READ32(c90flRegBase) & C90FL_MCR_DONE)
    {
        /* check the operation status*/
        if(!(READ32(c90flRegBase) & C90FL_MCR_PEG))
        {
            /* high voltage operation failed*/
            returnCode = EE_ERROR_PE_OPT;
        }

        /* end the high voltage operation*/
        CLEAR32(c90flRegBase, C90FL_MCR_EHV);

        /* check for program operation*/
        if (READ32(c90flRegBase) & C90FL_MCR_PGM)
        {
            /* finish the program operation*/
            CLEAR32(c90flRegBase, C90FL_MCR_PGM);
        }
        else
        {
            /* finish the erase operation*/
            CLEAR32(c90flRegBase, C90FL_MCR_ERS);
        }

        return (returnCode);
    }

    return (EE_INFO_HVOP_INPROGRESS);
}


UINT32 FSL_DataVerify (UINT32 c90flRegBase, BOOL blankCheck, UINT32* dest, UINT32* size, UINT32* source, UINT32 *compareAddress, UINT64 *compareData, void(*CallBack)(void))
{
    UINT32  destIndex;          /* destination address index*/
    UINT32  counter;            /* loop counter*/
    UINT32  suspendState;       /* suspend state*/
    UINT32  returnCode;         /* return code*/
    UINT64  destData;           /* dest data (double word type)*/
    UINT64  sourceData;         /* source data (byte type)*/

    /* initialize return code*/
    returnCode = EE_OK;

    /* Clear comapreAddress and compareData arguments*/
    *compareAddress = 0;
    *compareData = 0;

    /* check high voltage operation and try to suspend it*/
    suspendState = FSL_FlashSuspend(c90flRegBase);

    /* init sourceData, default is for blank checking*/
    sourceData = CREATE_WORD64(ERASED_HIGH, ERASED_LOW);

    /* Verify data word by word*/
    for (destIndex = 0; (destIndex < DATA_VERIFY_NUMBER) && (*size > 0); destIndex += DWORD_SIZE)
    {
        /* callback service*/
        if ((destIndex % CALLBACK_VERIFY_BYTES_NUMBER) == 0)
        {
            if(NULL_CALLBACK != CallBack)
            {
               CallBack();
            }
        }

        /* get the data first*/
        destData = FSL_FlashRead64(*dest);

        /* check if it is blank check or data verfication*/
        if (!blankCheck)
        {
            /* data verification*/
            /* read source data*/
            sourceData = FSL_FlashRead64(*source);

            /* only updated source while performing data verify*/
            *source += DWORD_SIZE;
        }

        /* check if the size is not double word length*/
        if (*size < 8 )
        {
            /* fetch the correct data depending on the actual size,*/
            for (counter = 0; counter < *size; counter++)
            {
                if (*(UINT8*)((UINT32)(&destData) + counter) != *(UINT8*)((UINT32)(&sourceData) + counter))
                {
                    /* if not match, set error return code*/
                    returnCode = EE_ERROR_MISMATCH;
                    break;
                }
            }

            /* for end of the loop only*/
            *size = DWORD_SIZE;
        }
        else
        {
            /* compare the data against the Flash content*/
            if (destData != sourceData)
            {
                /* if not match, set error return code*/
                returnCode = EE_ERROR_MISMATCH;
            }
        }

        /* check return code*/
        if (returnCode == EE_ERROR_MISMATCH)
        {
            /* data mismatch*/
            *compareAddress = *dest;
            *compareData = destData;
            break;
        }

        /* update the dest*/
        *dest += DWORD_SIZE;

        /* update the size*/
        *size -= DWORD_SIZE;
    }

    /* check if it needs resume*/
    FSL_FlashResume(c90flRegBase, suspendState);

    return (returnCode);
}

UINT32 FSL_FlashSuspend (UINT32 c90flRegBase)
{
    UINT32 returnCode;          /* return code*/
    UINT32 MCRValue;            /* MCR register value*/

    /* initialize the return code*/
    returnCode = EE_OK;

    /* get the current MCR value*/
    MCRValue = READ32(c90flRegBase);

    /* no P/E sequence: PGM=0 and ERS=0*/

    /* possible stages for program sequece:*/
    /*    a. interlock write;               (PGM=1;EHV=0;  PSUS=0;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    b. high voltage active;           (PGM=1;EHV=1;  PSUS=0;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    c. entering suspend state;        (PGM=1;EHV=1;  PSUS=1;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    d. in suspend state;              (PGM=1;EHV=1/0;PSUS=1;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    e. resuming from suspend state;   (PGM=1;EHV=1;  PSUS=0;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    f. high voltage stopped;          (PGM=1;EHV=1;  PSUS=0;DONE=1; valid  PEG)  OR (ERS=1;ESUS=1)*/
    /*    g. abort period.                  (PGM=1;EHV=0;  PSUS=0;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)*/

    /* possible stages for erase sequece:*/
    /*    a. interlock write;               (ERS=1;EHV=0;  ESUS=0;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    b. high voltage active;           (ERS=1;EHV=1;  ESUS=0;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    c. entering suspend state;        (ERS=1;EHV=1;  ESUS=1;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    d. in suspend state;              (ERS=1;EHV=1/0;ESUS=1;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    e. resuming from suspend state;   (ERS=1;EHV=1;  ESUS=0;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    f. high voltage stopped;          (ERS=1;EHV=1;  ESUS=0;DONE=1; valid  PEG)  AND (PGM=0;PSUS=0)*/
    /*    g. abort period.                  (ERS=1;EHV=0;  ESUS=0;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)*/

    /* b. high voltage active*/
    /* e. resuming from suspend state*/
    /* f. high voltage stopped*/
    if ( (MCRValue & C90FL_MCR_EHV) &&
         ( ((MCRValue & C90FL_MCR_PGM) && !(MCRValue & C90FL_MCR_PSUS)) ||
           ((MCRValue & C90FL_MCR_ERS) && !(MCRValue & C90FL_MCR_ESUS)) ) )
    {
        if (MCRValue & C90FL_MCR_PGM)
        {
            /* program suspend*/
            C90FLMCR_EED_BIT_SET (c90flRegBase, C90FL_MCR_PSUS);   /* set MCR-PSUS bit*/
            returnCode = EE_INFO_PROGRAM_SUSPEND;
        }
        else
        {
            /* erase suspend*/
            C90FLMCR_EED_BIT_SET (c90flRegBase, C90FL_MCR_ESUS);   /* set MCR-ESUS bit*/
            returnCode = EE_INFO_ERASE_SUSPEND;
        }
    }

    /* Wait until MCR-DONE = 1*/
    while (!(READ32(c90flRegBase) & C90FL_MCR_DONE))
    {
    }

    /* Set MCR-EHV to 0*/
    C90FLMCR_EED_BIT_CLEAR (c90flRegBase, C90FL_MCR_EHV);

    return (returnCode);
}

UINT32 FSL_FlashResume (UINT32 c90flRegBase, UINT32 suspendStatus)
{
    UINT32 mask;            /* bit mask value*/
    UINT32 count;           /* loop counter*/

    /* initialize the loop counter*/
    count = FLASH_RESUME_WAIT;

    /* check the suspend state*/
    if (suspendStatus == EE_INFO_PROGRAM_SUSPEND)
    {
        /* need resume programming*/
        mask = C90FL_MCR_PSUS;
    }
    else if (suspendStatus == EE_INFO_ERASE_SUSPEND)
    {
        /* need resume erasing*/
        mask = C90FL_MCR_ESUS;
    }
    else
    {
        /* nothing to resume*/
        return (EE_OK);
    }

    /* Set MCR-EHV bit*/
    C90FLMCR_EED_BIT_SET (c90flRegBase, C90FL_MCR_EHV);

    /* clear the MCR-ESUS bit or MCR-PSUS bit*/
    C90FLMCR_EED_BIT_CLEAR (c90flRegBase, mask);

    /* wait the MCR-DONE bit goes low*/
    while ( (READ32(c90flRegBase) & C90FL_MCR_DONE) && (count--) > 0)
    {
    }

    return (EE_OK);
}

UINT32 FSL_FlashEraseAbort (UINT32 c90flRegBase)
{
    UINT32 returnCode = EE_OK;   /* return code*/

    if(READ32(c90flRegBase) & C90FL_MCR_ERS)
    {
       /* Set MCR-EHV to 0*/
       C90FLMCR_EED_BIT_CLEAR (c90flRegBase, C90FL_MCR_EHV);

       /* Wait until MCR-DONE = 1*/
       while (!(READ32(c90flRegBase) & C90FL_MCR_DONE))
       {
       }

       /* finish the erase operation*/
       CLEAR32(c90flRegBase, C90FL_MCR_ERS);
    }
    return (returnCode);
}

UINT32 FSL_FlashRead (UINT32 c90flRegBase, UINT32* dest, UINT32* size, UINT32* buffer, void(*CallBack)(void))
{
    UINT32      suspendState;   /* suspend state*/
    UINT32      counter;        /* loop counter*/

    /* check high voltage operation*/
    suspendState = FSL_FlashSuspend(c90flRegBase);

    /* read data*/
    for (counter = 0; (*size > 0) && (counter < DATA_READ_NUMBER); counter++)
    {
        /* callback service*/
        if ((counter % CALLBACK_READ_BYTES_NUMBER) == 0)
        {
            if(NULL_CALLBACK != CallBack)
            {
               CallBack();
            }
        }

        WRITE8(*buffer, FSL_FlashRead8(*dest));
        (*dest) += 1;
        (*buffer) += 1;
        (*size) -= 1;
    }

    /* check if it needs resume*/
    FSL_FlashResume(c90flRegBase, suspendState);

    return (EE_OK);
}
//put Flash read functions in non-VLE sections
//#pragma section data_type ".text"

/*exception handler*/
void EER_exception_handler(void)
{
	if (eccErrorModule_Flag == EE_MODULE)
	{
		eccErrorStatus_Flag = TRUE;
	}
	else
	{
	  /*do nothing*/
	}
	return;
}


/*put Flash read functions in non-VLE sections*/
/*#pragma section data_type ".text"*/

UINT8 FSL_FlashRead8(UINT32 address)
{
  return((UINT8)(*(VUINT8*)(address)));
}

UINT64 FSL_FlashRead64(UINT32 address)
{
  return((UINT64)(*(VUINT64*)(address)));
}




