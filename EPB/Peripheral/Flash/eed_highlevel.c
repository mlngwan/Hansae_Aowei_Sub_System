/*******************************************************************
* PROJECT : EEPROM Emulation Driver (EED) For C90 flash
* FILE    : eed_highlevel.c
*
* DESCRIPTION : This file contains high (user) level EED API
*               implementation 
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

/* Flag to keep track of invalid data*/
UINT8 recordStatusInvalid_Flag = FALSE;
/* Flag to keep track of ECC Error Status*/
UINT8 volatile eccErrorStatus_Flag = FALSE;
/* Flag to keep track of ECC Error Module*/
volatile EE_ModuleType eccErrorModule_Flag = NONE;
/* Flag to keep track of Erase State*/
EE_EraseType eraseStatus_Flag = ERASE_NOTSTARTED;
/* structure to store block configuration during erase*/
BLOCK_CONFIG* blockConfInternal;
/* Flag to keep track of Block Swapping*/
UINT8 blockSwap_Flag = FALSE;
/*Variable to store C90FL Register Base*/
UINT32 c90flRegBaseInternal;
/*Variable to store erasing cycles*/
UINT64 erasingCycleInternal;
/*Variable to store source*/
UINT64 sourceInternal;
/*Variable to store size of the data record*/
UINT32 tempInternal;
/*Variable to store DataID*/
UINT16 dataIDInternal;
/*Variable to store Data Size*/
UINT16 dataSizeInternal;
/*Variable to store source address while swap*/
UINT32 sourceInternalSwap;


/*******************************************************************
| global variable definitions (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| function prototypes (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| function implementations (scope: module-local)
|------------------------------------------------------------------*/

/*******************************************************************
| function implementations (scope: module-exported)
|------------------------------------------------------------------*/

UINT32 FSL_InitEeprom (EEPROM_CONFIG* eepromConfig, void(*CallBack)(void))
{
    UINT16          index;          // loop index
    UINT32          returnCode;     // return code
    UINT32          temp;           // temporary variable
    BLOCK_CONFIG*   blockConf;      // block configuration pointer
    CACHE_TABLE*    cTable;         // local cache table pointer
   	UINT32		        returntemp;


	returntemp = EE_OK;

	/* check the write lock of the FlashEE*/
	if (eepromConfig->blockWriteFlag)
	{
        /*a write operation is already in progress*/
        /* no write operation is permitted*/
        return (EE_ERROR_WRITE_IN_PROGRESS);
    }
    else
    {
        /* set the write lock flag*/
        eepromConfig->blockWriteFlag = TRUE;
    }

    /* search the active block*/
    if (!FSL_SearchActiveBlock (eepromConfig, &(eepromConfig->activeBlockIndex), CallBack))
    {

        /* no active block found*/
        /* need reset the blocks*/
        /* select block 0 as active block*/
        blockConf = eepromConfig->flashBlocks[0];

        /* Initialize block 0*/
        returnCode = FSL_InitEEBlock(blockConf, eepromConfig->c90flRegBase, CallBack);
        if (returnCode != EE_OK)
        {
            return (returnCode);
        }

        /* change the block 0 status to active*/
        returnCode = FSL_ProgramDoubleWord (eepromConfig->c90flRegBase, blockConf->blockStartAddr, BLOCK_STATUS_ACTIVE, CallBack);
        if (returnCode != EE_OK)
        {
            return (returnCode);
        }

        /* set the active block in block configuration*/
        eepromConfig->activeBlockIndex = 0;

		returntemp = EE_FIRST_TIME_INITIALIZATION;

    }
    else
    {
    	/* eepromConfig->activeBlockIndex*/
	    if(TRUE == recordStatusInvalid_Flag)
	    {
	         /* perform block swapping*/
	         /* call block swapping function*/
	         returnCode = FSL_BlockSwapping(eepromConfig, CallBack);
	         if (returnCode != EE_OK)
	         {
	             /* unlock the write lock*/
	             eepromConfig->blockWriteFlag = FALSE;
	             return (returnCode);
	         }
	           /*Finished swapping;reset flag*/
	   		   recordStatusInvalid_Flag = FALSE;
	    }
	    else
	    {
	        /*do nothing*/
	    }
	 }

    /* initialize the alternative blocks*/
    for (temp = 0; temp < eepromConfig->numberOfBlock; temp++)
    {
        /* skip the active block*/
        if (temp == eepromConfig->activeBlockIndex)
        {
            continue;
        }

        /* initialize one Flash block*/
        blockConf = eepromConfig->flashBlocks[temp];
        returnCode = FSL_InitEEBlock(blockConf, eepromConfig->c90flRegBase, CallBack);
        if (returnCode != EE_OK)
        {
            return (returnCode);
        }
    }

    /* initialize the cache table*/
    if (eepromConfig->cacheEnable)
    {
        /* initialize the cache table pointer*/
        cTable = eepromConfig->cTable;
        for (temp = 0; temp <((cTable->tableSize)/4); temp++)
        {
           /* Fill all cache table entries with 0xFFFFFFFF*/
           FSL_UpdateCacheTable(eepromConfig->cTable, temp, 0xFFFFFFFF);
        }
        /* get the active block*/
        blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

        /* initialize index*/
        index = 0;

        /* search through the active block to find the eligible data records address*/
        FSL_RecordBuffering(blockConf, eepromConfig->c90flRegBase, cTable->tableStartAddress, cTable->tableSize, &index, CallBack);
    }

    /* unlock the write lock*/
    eepromConfig->blockWriteFlag = FALSE;

    return (returntemp);
}


UINT32 FSL_ReadEeprom(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT32 source, void(*CallBack)(void))
{
    BOOL            foundFlag;      /* data found flag*/
    UINT32          size;           /* reading size*/
    UINT32          temp;           /* temporary local variable*/
    UINT32          buffer;         /* reading buffer*/
    UINT32          returnCode;     /* return code*/
    UINT32          recordAddr;     /* data record address*/
    UINT32          c90flRegBase;    /* local c90fl register base address*/
    BLOCK_CONFIG*   blockConf;      /* local block configuration pointer*/

    /* init the found flag*/
    foundFlag = FALSE;

    /* get c90fl register base address*/
    c90flRegBase = eepromConfig->c90flRegBase;

    /* check status and end operation*/
    if(ERASE_INPROGRESS == eraseStatus_Flag || ERASE_DONE == eraseStatus_Flag)
    {
       /* Erase in progress RWW in same partition not supported*/
       return(EE_INFO_HVOP_INPROGRESS);
    }

    /* check if cache table is enabled*/
    if (eepromConfig->cacheEnable)
    {
        /* search the cache table first*/
        returnCode = FSL_SearchInTable (eepromConfig->cTable, dataID, &recordAddr);

        if (returnCode == EE_OK)
        {
            /* data record addres is in the cache table*/
            foundFlag = TRUE;
        }
        else if(returnCode == EE_ERROR_DATA_NOT_FOUND)
        {
            /* data record does not exist*/
            return (EE_ERROR_DATA_NOT_FOUND);
        }
    }

    /* data is not found in enabled cache table or cache is disabled*/

    /* search data in block*/
    if (!foundFlag)
    {
        /* have to search the data record in the active block*/
        /* find the active block*/
        blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

        returnCode = FSL_SearchInBlock (blockConf, eepromConfig->c90flRegBase, dataID, &recordAddr, CallBack);
        if (returnCode != EE_OK)
        {
            return (returnCode);
        }
    }

    /* already get the data record address and fetch it*/
    /* get the data ID and size*/
    recordAddr += 8;
    temp = WORD_SIZE;
    buffer = (UINT32)(&size);
    FSL_FlashRead(c90flRegBase, &recordAddr, &temp, &buffer, CallBack);

    /* get data size*/
    size = size & 0xFFFF;

    /* read the data to buffer*/
    FSL_FlashRead(c90flRegBase, &recordAddr, &size, &source, CallBack);

    return (EE_OK);
}


UINT32 FSL_WriteEeprom (EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT16 dataSize, UINT32 source, void(*CallBack)(void))
{
    UINT32          recordAddr;
    UINT32           temp;
    UINT32          returnCode;
    UINT8           swapFlag = FALSE;
    BLOCK_CONFIG*   blockConf;

    /* check status and end operation*/
    if(ERASE_INPROGRESS == eraseStatus_Flag || ERASE_DONE == eraseStatus_Flag)
    {
       /* Erase in progress RWW in same partition not supported*/
       return(EE_INFO_HVOP_INPROGRESS);
    }

    /* Check if an active block exist */
    if (FALSE == FSL_SearchActiveBlock (eepromConfig, &(eepromConfig->activeBlockIndex), CallBack))
    {
       /* Active block not found return error */
       return(EE_ERROR_BLOCK_STATUS);
    }
    
    /* check the write lock of the FlashEE*/
    if (eepromConfig->blockWriteFlag)
    {
        /* a write operation is already in progress*/
        /* no write operation is permitted*/
        return (EE_ERROR_WRITE_IN_PROGRESS);
    }
    else
    {
        /* set the write lock flag*/
        eepromConfig->blockWriteFlag = TRUE;
    }

    /* check whether there is enough blank space for data record*/
    blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

    /* get the data length including status, ID, size and data field*/
    if (dataSize <= (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE)))
    {
        /* less than or equal to 4 bytes data*/
        temp = PAGE_SIZE;
    }
    else
    {
           /* increment by pages*/
        if (( (dataSize - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) % PAGE_SIZE) )
            temp = PAGE_SIZE + PAGE_SIZE * ( (dataSize - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) / PAGE_SIZE + 1);
        else
            temp = PAGE_SIZE + PAGE_SIZE * ( (dataSize - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) / PAGE_SIZE);

    }

    /* compare against the available space*/
    if ((blockConf->blockStartAddr + blockConf->blockSize - blockConf->blankSpace) < temp)
    {
        /* no enough space for data record*/
        /* perform block swapping*/
        /* call block swapping function*/
        swapFlag = TRUE;
        sourceInternalSwap = source;
        dataIDInternal = dataID;
        dataSizeInternal = dataSize;
        tempInternal = temp;
        blockSwap_Flag = TRUE;
        returnCode = FSL_BlockSwapping(eepromConfig, CallBack);
        blockSwap_Flag = FALSE;
        if (returnCode != EE_OK)
        {
            /* unlock the write lock*/
            eepromConfig->blockWriteFlag = FALSE;

            return (returnCode);

        }

        /* set the new active block to local variable*/
        blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

        /* check the free space again*/
        if ((blockConf->blockStartAddr + blockConf->blockSize - blockConf->blankSpace) < temp)
        {
            /* no enough space for this record*/
            /* unlock the write lock*/
            eepromConfig->blockWriteFlag = FALSE;

            return (EE_ERROR_NO_ENOUGH_SPACE);
        }
    }
    if (FALSE == swapFlag)
    {
       /* have enough space for this data*/
       /* save current blank pointer*/
       recordAddr = blockConf->blankSpace;

       /* copy one data record to free space in the active block*/
       returnCode = FSL_CopyDataRecord(eepromConfig->c90flRegBase, dataID, dataSize, source, recordAddr, CallBack);
       if (returnCode != EE_OK)
       {
           /* update the blank pointer to the end of block*/
           blockConf->blankSpace = blockConf->blockStartAddr + blockConf->blockSize;

           /* unlock the write lock*/
           eepromConfig->blockWriteFlag = FALSE;

           return (EE_ERROR_PE_OPT);
        }

       /* check if the cache table is enabled*/
       if (eepromConfig->cacheEnable)
       {
           /* check if it is needed to update the cache table*/
           FSL_UpdateCacheTable(eepromConfig->cTable, dataID, recordAddr);
       }

       /* update the blank pointer first (add the data size)*/
       blockConf->blankSpace += temp;
    }

      /* unlock the write lock*/
      eepromConfig->blockWriteFlag = FALSE;

    return (returnCode);
}


UINT32 FSL_DeleteRecord(EEPROM_CONFIG* eepromConfig, UINT16 dataID, void(*CallBack)(void))
{
    BOOL            searchFlag;     /* search in block flag*/
    UINT32          recordAddr;     /* record address*/
    UINT32          returnCode;     /* return code*/

    /* initialize the return code*/
    returnCode = EE_OK;
    searchFlag = TRUE;

    /* check status and end operation*/
    if(ERASE_INPROGRESS == eraseStatus_Flag || ERASE_DONE == eraseStatus_Flag)
    {
       /*Erase in progress RWW in same partition not supported*/
       return(EE_INFO_HVOP_INPROGRESS);
    }
    /* check the write lock of the FlashEE*/
    if (eepromConfig->blockWriteFlag)
    {
        /* a write operation is already in progress*/
        /* no write operation is permitted*/
        return (EE_ERROR_WRITE_IN_PROGRESS);
    }
    else
    {
        /* set the write lock flag*/
        eepromConfig->blockWriteFlag = TRUE;
    }

    /* try to delete the data in cache table firstly*/
    if (eepromConfig->cacheEnable)
    {
        /* find it in cache table*/
        returnCode = FSL_SearchInTable(eepromConfig->cTable, dataID, &recordAddr);
        if (returnCode == EE_ERROR_DATA_NOT_FOUND)
        {
            /* release the write lock*/
            eepromConfig->blockWriteFlag = FALSE;

            /* data does not exist*/
            return (EE_ERROR_DATA_NOT_FOUND);
        }
        else if (returnCode == EE_OK)
        {
            /* data location can fetch from cache*/
            /* need not search in block*/
            searchFlag = FALSE;

            /* update the data location in cache*/
            FSL_UpdateCacheTable(eepromConfig->cTable, dataID, 0xFFFFFFFF);
        }
    }

    if (searchFlag == TRUE)
    {
        /* find it in active block*/
        if(FSL_SearchInBlock (eepromConfig->flashBlocks[eepromConfig->activeBlockIndex], eepromConfig->c90flRegBase, dataID, &recordAddr, CallBack) == EE_OK)
        {
            /* found in the active block*/
        }
        else
        {
            /* not found specific data record*/
            /* unlock the write lock*/
            eepromConfig->blockWriteFlag = FALSE;

            return (EE_ERROR_DATA_NOT_FOUND);
        }
    }

    /* update the record status to DELETED*/
    returnCode = FSL_ProgramDoubleWord (eepromConfig->c90flRegBase, recordAddr, RECORD_STATUS_DELETE, CallBack);

    /* release the write lock*/
    eepromConfig->blockWriteFlag = FALSE;

    return (returnCode);
}


UINT32 FSL_RemoveEeprom (EEPROM_CONFIG* eepromConfig, void(*CallBack)(void))
{
    UINT32          counter;        /* loop counter*/
    UINT32          returnCode;     /* return code*/
    UINT32          ebHigh;         /* enabled blocks for high space Flash block*/
    UINT32          ebMiddle;       /* enabled blocks for middle space Flash block*/
    UINT32          ebLow;          /* enabled blocks for low space Flash block*/
    UINT32          dest;           /* blank checking destination address*/
    UINT32          size;           /* blank checking size*/
    UINT32          source;         /* dummy variable*/
    UINT32          temp;           /* local temporary variable*/
    UINT32          faildAddr;      /* failed address*/
    UINT64          faildData;      /* failed data*/
    BLOCK_CONFIG*   blockConf;      /* local block configuration pointer*/

    /* check status and end operation*/
    if(ERASE_INPROGRESS == eraseStatus_Flag || ERASE_DONE == eraseStatus_Flag)
    {
       /* Erase in progress RWW in same partition not supported*/
       return(EE_INFO_HVOP_INPROGRESS);
    }

    /* check the write lock of the FlashEE*/
    if (eepromConfig->blockWriteFlag)
    {
        /* a write operation is already in progress*/
        /* no write operation is permitted*/
        return (EE_ERROR_WRITE_IN_PROGRESS);
    }
    else
    {
        /* set the write lock flag*/
        eepromConfig->blockWriteFlag = TRUE;
    }

    /* initialize enabled block*/
    ebLow = ebMiddle = ebHigh = 0;

    /* erase all blocks except actived block*/
    for (counter = 0; counter < eepromConfig->numberOfBlock; counter++)
    {
        /* get block configuration*/
        blockConf = eepromConfig->flashBlocks[counter];

        /* check the address space*/
        switch (blockConf->blockSpace)
        {
            case LOW_ADDRESS_SPACE:
                ebLow |= blockConf->enabledBlock;
                break;
            case MID_ADDRESS_SPACE:
                ebMiddle |= blockConf->enabledBlock;
                break;
            case HIGH_ADDRESS_SPACE:
                ebHigh |= blockConf->enabledBlock;
                break;
            default:
                /* unlock the write lock*/
                eepromConfig->blockWriteFlag = FALSE;
                return (EE_ERROR_BLOCK_CONFIG);
        }
    }

    /* erase blocks*/
    returnCode = FSL_FlashEraseStart(eepromConfig->c90flRegBase, blockConf->blockStartAddr, ebLow, ebMiddle, ebHigh);
    if (returnCode != EE_OK)
    {
        /* unlock the write lock*/
        eepromConfig->blockWriteFlag = FALSE;
        return (EE_ERROR_PE_OPT);
    }

    /* check status and end operation*/
    while (1)
    {
        if(NULL_CALLBACK != CallBack)
        {
           CallBack();
        }

        returnCode = FSL_FlashCheckStatus(eepromConfig->c90flRegBase);
        if (returnCode == EE_OK)
        {
            break;
        }
        else if (returnCode == EE_ERROR_PE_OPT)
        {
            /* unlock the write lock*/
            eepromConfig->blockWriteFlag = FALSE;
            return (returnCode);
        }
    }


    /* blank check*/
    for (counter = 0; counter < eepromConfig->numberOfBlock; counter++)
    {
        /* get the block configuration*/
        blockConf = eepromConfig->flashBlocks[counter];

        /* get dest and size*/
        dest = blockConf->blockStartAddr;
        size = blockConf->blockSize;

        /* calculate the end address*/
        temp = dest + size;

        /* blank check*/
        while (dest < temp)
        {
            returnCode = FSL_DataVerify(eepromConfig->c90flRegBase, TRUE, &dest, &size, &source, &faildAddr, &faildData, CallBack);
            if (returnCode != EE_OK)
            {
                /* unlock the write lock*/
                eepromConfig->blockWriteFlag = FALSE;
                return (EE_ERROR_PE_OPT);
            }
        }
    }

    /* unlock the write lock*/
    eepromConfig->blockWriteFlag = FALSE;
    return (returnCode);
}

UINT32 FSL_ReportEepromStatus(EEPROM_CONFIG* eepromConfig, UINT64* erasingCycles, UINT32* failedAddress, void(*CallBack)(void))
{
    UINT32          suspendState;       /* suspend state*/
    UINT32          returnCode;         /* return code*/
    UINT32          counter;            /* loop counter*/
    UINT32          number;             /* number of the Flash blocks*/
    UINT64          blockStatus;        /* block status*/
    UINT32          c90flRegBase;        /* local c90fl register base address*/
    BLOCK_CONFIG*   blockConf;          /* local block ocnfiguration pointer*/

    /* check status and end operation*/
    if(ERASE_INPROGRESS == eraseStatus_Flag || ERASE_DONE == eraseStatus_Flag)
    {
       /* Erase in progress RWW in same partition not supported*/
       return(EE_INFO_HVOP_INPROGRESS);
    }

    /* get the c90fl registier base address*/
    c90flRegBase = eepromConfig->c90flRegBase;

    /* check high voltage operation*/
    suspendState = FSL_FlashSuspend(c90flRegBase);

    /* check the block status*/
    /* get the total number of Flash blocks used for emulation*/
    number = eepromConfig->numberOfBlock;

    for (counter = 0; counter < number; counter++)
    {
        /* get block config*/
        blockConf = eepromConfig->flashBlocks[counter];

        /* read the status*/
        blockStatus = FSL_FlashRead64(blockConf->blockStartAddr);

        /* check the block status*/
        if ((blockStatus == BLOCK_STATUS_BLANKED) ||
            (blockStatus == BLOCK_STATUS_COPY_START) ||
            (blockStatus == BLOCK_STATUS_ACTIVE) ||
            (blockStatus == BLOCK_STATUS_ERASED))
        {
            ;
        }
        else
        {
            /* error status*/
            /* check if it needs resume*/
            FSL_FlashResume(c90flRegBase, suspendState);

            return (EE_ERROR_BLOCK_STATUS);
        }
    }

    /* get the active block*/
    blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

    /* get the erasing cycles*/
    *erasingCycles = FSL_FlashRead64(blockConf->blockStartAddr + PROGRAMMABLE_SIZE);

    /* check the active block data records*/
    /* go throuch the block to check the data record status with reserved data ID (0xFFFF)*/
    returnCode = FSL_SearchInBlock (blockConf, c90flRegBase, 0xFFFF, failedAddress, CallBack);

    /* check if it needs resume*/
    FSL_FlashResume(c90flRegBase, suspendState);

    if (returnCode == EE_ERROR_RECORD_STATUS)
    {
        return (EE_ERROR_RECORD_STATUS);
    }
    else
    {
        return (EE_OK);
    }
}


void FSL_MainFunction(void(*CallBack)(void))
{

    UINT32      returnCode;
    UINT32	  faildAddr;
    UINT64	  faildData;
    UINT32      dest;           /* blank checking destination address*/
    UINT32      size;           /* blank checking size*/

    /* check status and end operation*/
    if(ERASE_INPROGRESS == eraseStatus_Flag)
    {

      returnCode = FSL_FlashCheckStatus(c90flRegBaseInternal);
      if (returnCode == EE_OK)
      {
          eraseStatus_Flag = ERASE_DONE;
      }
      else if (returnCode == EE_ERROR_PE_OPT)
      {
          eraseStatus_Flag = ERASE_FAIL;
      }
      else
      {
          eraseStatus_Flag = ERASE_INPROGRESS;
          if(NULL_CALLBACK != CallBack)
          {
             CallBack();
          } 
      }

      if (ERASE_DONE == eraseStatus_Flag)
      {

        /* blank check and update block status*/
        /* blank check*/
        dest = blockConfInternal->blockStartAddr;
        size = blockConfInternal->blockSize;

        while (size)
        {
            /* perform blank check*/
            returnCode = FSL_DataVerify(c90flRegBaseInternal, TRUE, &dest, &size, (UINT32*)&sourceInternal, &faildAddr, &faildData, CallBack);
            if (returnCode != EE_OK)
            {
                /* error during data verify*/
                eraseStatus_Flag = ERASE_SWAPERROR;
            }
        }

        /* program the increased erasing cycles back*/
        returnCode = FSL_ProgramDoubleWord(c90flRegBaseInternal, blockConfInternal->blockStartAddr + PROGRAMMABLE_SIZE, (erasingCycleInternal + 1), CallBack);
        if (returnCode != EE_OK)
        {
            /* error during programming erase cycles*/
            eraseStatus_Flag = ERASE_SWAPERROR;
        }

        /* update the block status to BLANKED*/
        returnCode = FSL_ProgramDoubleWord(c90flRegBaseInternal, blockConfInternal->blockStartAddr,BLOCK_STATUS_BLANKED, CallBack);
        if (returnCode != EE_OK)
        {
            /* error during programming block status*/
            eraseStatus_Flag = ERASE_SWAPERROR;
        }

        if(ERASE_SWAPERROR != eraseStatus_Flag)
        {
            /* Reset flag*/
            eraseStatus_Flag = ERASE_NOTSTARTED;
            /* set blank space pointer*/
            blockConfInternal->blankSpace = blockConfInternal->blockStartAddr + PAGE_SIZE;
        }
      }

    }
}

UINT32 FSL_AbortFunction(EEPROM_CONFIG* eepromConfig)
{
    UINT32 returnCode = EE_OK;
    UINT32 c90flRegBase;       /* local c90fl register base address*/

    /* get the c90fl register base address*/
    c90flRegBase = eepromConfig->c90flRegBase;

    returnCode = FSL_FlashEraseAbort(c90flRegBase);
    if(EE_OK == returnCode)
    {
       eraseStatus_Flag = ERASE_FAIL;
    }
    return(returnCode);    
}
