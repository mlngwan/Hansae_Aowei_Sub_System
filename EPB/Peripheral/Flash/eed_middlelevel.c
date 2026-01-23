/*******************************************************************
* PROJECT : EEPROM Emulation Driver (EED) For C90 flash
* FILE    : eed_middlelevel.c
*
* DESCRIPTION : This file contains middle level EED API implementation 
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

/*******************************************************************
| function implementations (scope: module-exported)
|------------------------------------------------------------------*/

UINT32 FSL_BlockSwapping(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void))
{
    BOOL                cacheFlag;          /* used to save the original cache table status*/
    UINT16              dataID;             /* the maximum ID found in last time and used as the ID range*/
    UINT32              index;              /* loop index*/
    UINT32              bufferStart;        /* buffer start address for data copy*/
    UINT32              bufferSize;         /* buffer size in byte*/
    UINT32              u32PrevBuff;        /* Previous time buffer start address for data copy*/
    UINT32              destAddr;           /* the destination address to be copied in alternative block*/
    UINT32              counter;            /* the loop counter*/
    UINT32              alternativeBlock;   /* the alternative block index*/
    UINT32              returnCode;         /* the return code*/
    UINT32              buffer[1];          /* the local data buffer*/
    UINT32              dest;               /* the destination address*/
    UINT32              size;               /* the size for reading*/
    UINT32              readBuff;           /* the buffer for reading*/
    UINT32              temp;               /* the temporary variable*/
    UINT64              blockStatus;        /* block status*/
    UINT32              recordAddr;
    CACHE_TABLE*        cTable;             /* the local cache table pointer*/
    DATA_RECORD_HEAD*   record;             /* the local data record pointer*/
    BLOCK_CONFIG*       sourceBlock;        /* the source (active) block configuration pointer*/
    BLOCK_CONFIG*       destBlock;          /* the destination (alternative) block configuration pointer*/

    /* find the active block*/
    counter = eepromConfig->activeBlockIndex;

    /* the next block can be used as alternative block*/
    /* if active block is the last one, then the first one*/
    /* is the alternative block*/
    alternativeBlock = (counter + 1) % (eepromConfig->numberOfBlock);

    /* assign source and dest blocks*/
    sourceBlock = eepromConfig->flashBlocks[counter];
    destBlock = eepromConfig->flashBlocks[alternativeBlock];

    /* check block status*/
    /* check if source block is the active block*/
    dest = sourceBlock->blockStartAddr;
    size = DWORD_SIZE;
    readBuff = (UINT32)(&blockStatus);
    FSL_FlashRead(eepromConfig->c90flRegBase, &dest, &size, &readBuff, CallBack);

    if (blockStatus != BLOCK_STATUS_ACTIVE)
    {
        /* source block is not actived*/
        return (EE_ERROR_BLOCK_STATUS);
    }

    /* check if destination block is verified blank*/
    dest = destBlock->blockStartAddr;
    size = DWORD_SIZE;
    readBuff = (UINT32)(&blockStatus);
    FSL_FlashRead(eepromConfig->c90flRegBase, &dest, &size, &readBuff, CallBack);

    if (blockStatus != BLOCK_STATUS_BLANKED)
    {
        /* dest block is not BLANKED*/
        return (EE_ERROR_BLOCK_STATUS);
    }

    /* assign the free space address to new block*/
    destAddr = destBlock->blockStartAddr;

    /* before copy data, change dest block status to COPY_START*/
    returnCode = FSL_ProgramDoubleWord(eepromConfig->c90flRegBase, destAddr, BLOCK_STATUS_COPY_START, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    /* update dest address to data record region*/
    destAddr += PAGE_SIZE;

    /* get the cache table status*/
    cacheFlag = eepromConfig->cacheEnable;

    if (cacheFlag)
    {
        /* using cache table as the data buffer*/
        cTable = eepromConfig->cTable;

        /* get the cache table address and size*/
        bufferStart = cTable->tableStartAddress;
        bufferSize = cTable->tableSize;
    }
    else
    {
        /* use local variable as data buffer*/
        bufferStart = (UINT32)(buffer);
        bufferSize = sizeof(buffer);
    }

    if( FALSE == recordStatusInvalid_Flag )
    {
      recordAddr = destAddr;
      returnCode = FSL_CopyDataRecord(eepromConfig->c90flRegBase, dataIDInternal, dataSizeInternal, sourceInternalSwap, recordAddr, CallBack);
      if (returnCode != EE_OK)
      {
        /* update the blank pointer to the end of block*/
        destBlock->blankSpace = destBlock->blockStartAddr + destBlock->blockSize;

        /* unlock the write lock*/
        eepromConfig->blockWriteFlag = FALSE;

        return (EE_ERROR_PE_OPT);
      }

      /* check if the cache table is enabled*/
      if (cacheFlag)
      {
         /* check if it is needed to update the cache table*/
         FSL_UpdateCacheTable(eepromConfig->cTable, dataIDInternal, recordAddr);

      }

      /* update the blank pointer first (add the data size)*/
      destBlock->blankSpace += tempInternal;
      destAddr += tempInternal;
    }

    /* start copy data from source to dest*/
    for(dataID = 0; dataID < 0xFFFF;)
    {
        if (cacheFlag && dataID <= (((cTable->tableSize)/4)-1))
        {
          /* till this ID all records are present in Cache*/
          /*So do not use FSL_RecordBuffering API for buffering data */
          dataID = ((cTable->tableSize)/4);
        }
        else
        {

          /* use local variable as data buffer*/
          bufferStart = (UINT32)(buffer);
          bufferSize = sizeof(buffer);
        
          /* find the data record by its ID*/
          u32PrevBuff = *(UINT32*)bufferStart;
          returnCode = FSL_RecordBuffering(sourceBlock, eepromConfig->c90flRegBase, bufferStart, bufferSize, &dataID, CallBack);
          if (returnCode != EE_OK)
          {
              /* at least one record status error*/
              /* end the swapping*/
              return (returnCode);
          }
        }
        index = 0;

        /* Record is copied only if new source address is different from the previous one */
        if(u32PrevBuff != *(UINT32*)bufferStart)
        {    
          for (counter = 0; counter < (bufferSize / WORD_SIZE); counter++)
          {
            /* copy this data to new block*/
            record = (DATA_RECORD_HEAD*) ((UINT32*)bufferStart)[counter];

            if (index++ == 0)
            {
                if(NULL_CALLBACK != CallBack)
                {
                   CallBack();
                }
            }

            if (index == CALLBACK_FILL_BUFFER)
            {
                /* reset count */
                index = 0;
            }

            if(TRUE == blockSwap_Flag && (dataIDInternal== record->dataID))
            {
                continue;
            }
            if ((UINT32)record == 0xFFFFFFFF || (UINT32) record == 0x00000000 )
            {
                /* empty data reocrd (data does not exist)*/
                continue;
            }

            /* get the data ID and size*/
            dest = (UINT32)record + PROGRAMMABLE_SIZE;
            size = WORD_SIZE;
            readBuff = (UINT32)(&temp);
            FSL_FlashRead(eepromConfig->c90flRegBase, &dest, &size, &readBuff, CallBack);

						      /*reuse dest to store the destAddr*/
					       dest = destAddr;
					       if ((UINT16)temp <= (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE)))
            {
                /* less than or equal to (Flash Pagesize-(Status Size + Record Size+ID size)) bytes data*/
                /*increment dest to have the blank address of the UPDATE block*/
                /*after copying the data record*/
                dest += PAGE_SIZE;
            }
            else
            {
                /* increment by pages */
                /* increment dest to have the blank address of the UPDATE block */
                /* after copying the data record */
                if (( (temp - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) % PAGE_SIZE) )
                    dest += PAGE_SIZE + PAGE_SIZE * ( ((UINT16)temp - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) / PAGE_SIZE + 1);
                else
                    dest += PAGE_SIZE + PAGE_SIZE * ( ((UINT16)temp - (PAGE_SIZE-(PROGRAMMABLE_SIZE + WORD_SIZE))) / PAGE_SIZE);

            }

					      	/*check if address in dest exceeds the capacity of the UPDATE block*/
            if(dest >= (destBlock->blockStartAddr + destBlock->blockSize))
            {
               return(EE_ERROR_NO_ENOUGH_SPACE);
            }

            /* copy it to alternative block*/
            returnCode = FSL_CopyDataRecord(eepromConfig->c90flRegBase, (UINT16)(temp >> 16), (UINT16)temp, ((UINT32)record + 12), destAddr, CallBack);
            if (returnCode != EE_OK)
            {
                return (EE_ERROR_PE_OPT);
            }
			         /* update the free space address;*/
            /* less than or equal to (Flash Pagesize-(Status Size + Record Size+ID size)) bytes data*/
            destAddr = dest;
          }
        }  
    }

    /* update the new block blank space*/
    destBlock->blankSpace = destAddr;

    /* after copy data, change dest block status to ACTIVE*/
    returnCode = FSL_ProgramDoubleWord(eepromConfig->c90flRegBase, destBlock->blockStartAddr, BLOCK_STATUS_ACTIVE, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    /* after copy data, change source block status to DELETE*/
    returnCode = FSL_ProgramDoubleWord(eepromConfig->c90flRegBase, sourceBlock->blockStartAddr, BLOCK_STATUS_DELETE, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    /* set the active block in block configuration*/
    eepromConfig->activeBlockIndex = alternativeBlock;

    /* reset cache table if it is enabled*/
    if(cacheFlag)
    {
       UINT8 TempBlkSwap = blockSwap_Flag;
        dataID = 0;

        /* Assume swapping done because otherwise dataIDInternal cache entry is not updated */
        blockSwap_Flag = FALSE;

        /* search through the active block to find the eligible data records address*/
        FSL_RecordBuffering(destBlock, eepromConfig->c90flRegBase, cTable->tableStartAddress, cTable->tableSize, &dataID, CallBack);

        /* Restore blockSwap_Flag value */
        blockSwap_Flag = TempBlkSwap; 
            
        /* re-enable cache table*/
        eepromConfig->cacheEnable = TRUE;
    }

    /* erase the source block for next time*/
    returnCode = FSL_InitEEBlock(sourceBlock, eepromConfig->c90flRegBase, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    return (EE_OK);
}


UINT32 FSL_SearchInBlock (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT16 dataID, UINT32* expDataInBlock, void(*CallBack)(void))
{
    UINT16      nextStartID;        /* dummy variable*/
    UINT32      temp;               /* temporary variable*/
    UINT32      returnCode;         /* the return code*/
    UINT32      startAddrInBlock;   /* the data record address*/

    /* search the data from the block start to end*/
    /* the first double word (8 bytes) is block status*/
    /* the second double word is the block erasing cycle*/
    /* address will point to the record status field*/
    startAddrInBlock = blockConf->blockStartAddr + PAGE_SIZE;

    /* calculate the end address of the specific block*/
    temp = blockConf->blockStartAddr + blockConf->blockSize;

    /* initialize the expected data address*/
    *expDataInBlock = 0xFFFFFFFF;

    while (startAddrInBlock < temp)
    {
        /* need go through entire block*/
        returnCode = FSL_SearchRecordInBlock(blockConf, c90flRegBase, (UINT32)expDataInBlock, WORD_SIZE, dataID, &startAddrInBlock, &nextStartID, CallBack);
        if (returnCode != EE_OK)
        {
            /* error record status*/
            /* save the error record address*/
            *expDataInBlock = startAddrInBlock;
            return (returnCode);
        }
    }

    /* check if the expected data is ever found*/
    if ((*expDataInBlock == 0xFFFFFFFF) || (TRUE == recordStatusInvalid_Flag))
    {
        /* never found in this block*/
        return (EE_ERROR_DATA_NOT_FOUND);
    }
    else
    {
        return (EE_OK);
    }
}


UINT32 FSL_RecordBuffering (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT32 bufferAddress, UINT32 bufferSize, UINT16* startID, void(*CallBack)(void))
{
    UINT32      returnCode;         /* return code*/
    UINT32      temp;               /* temporary variable*/
    //UINT32      count;              /* loop counter*/
    UINT32      startAddrInBlock;   /* the local data record pointer*/
    UINT16      nextStartID;        /* the start ID for next time*/

    /* clear the cache table first*/
    //count = 0;

    /* the first double word (8 bytes) is block status*/
    /* address will point to the record status field*/
    startAddrInBlock = blockConf->blockStartAddr + PAGE_SIZE;
    /* calculate the end address of the specific block*/
    temp = blockConf->blockStartAddr + blockConf->blockSize;

    /* initialize the next ID*/
    nextStartID = 0xFFFF;

    while (startAddrInBlock < temp)
    {
        /* need go through entire block*/
        returnCode = FSL_SearchRecordInBlock(blockConf, c90flRegBase, bufferAddress, bufferSize, *startID, &startAddrInBlock, &nextStartID, CallBack);
        if (returnCode != EE_OK)
        {
            /* error record status*/
            return (returnCode);
        }
    }

    /* assign start ID for next time*/
    *startID = nextStartID;

    return (EE_OK);
}


BOOL FSL_SearchActiveBlock (EEPROM_CONFIG* eepromConfig, UINT32* activeBlockIndex, void(*CallBack)(void))
{
    BOOL            foundFlag;      /* found flag*/
    UINT32          counter;        /* loop counter*/
    UINT32          freeSpace;      /* the free space*/
    UINT32          temp;           /* temporary variable*/
	   UINT32	temp_MSR,enable_MSR;		   /* temporary space for MSR*/
    UINT64          blockStatusTemp;/* temporary block status*/
    BLOCK_CONFIG*   blockConf;      /* local block config pointer*/

    /* initialize the free space to minimum value*/
    temp = 0;

    /* initialize the found flag*/
    foundFlag = FALSE;

    /* check each block*/
    for (counter = 0; counter < eepromConfig->numberOfBlock; counter++)
    {
        /* get block configuration instant*/
        blockConf = eepromConfig->flashBlocks[counter];

	       /*save old MSR*/
	       temp_MSR = readAndClearEei();
	       enable_MSR = 0x00208200;
	       restoreEei(enable_MSR);

        eccErrorModule_Flag = EE_MODULE;

        /* assign data record pointer*/
	       blockStatusTemp = FSL_FlashRead64(blockConf->blockStartAddr);

        /*Reset ECC Error Module flag*/
        eccErrorModule_Flag = NONE;

        /*restore MSR*/
	       restoreEei(temp_MSR);

		      if (FALSE == eccErrorStatus_Flag)
		      {
          if (blockStatusTemp == BLOCK_STATUS_ACTIVE)
          {
            /* this is the actived block*/
            foundFlag = TRUE;

            /* check free space*/
            if (!FSL_CheckFreeSpace (blockConf, eepromConfig->c90flRegBase, &freeSpace, CallBack))
            {
                /* cannot get free space*/
                foundFlag = FALSE;
                continue;
            }

            /* compare the blank address against other blocks*/
            if (freeSpace >= temp)
            {
                /* this is the newer block*/
                /* update the free space size for comparison*/
                temp = freeSpace;

                /* save this block as active one*/
                *activeBlockIndex = counter;
            }
          }
		      }
		      else
		      {
		        eccErrorStatus_Flag = FALSE;
		      }
    }

    return (foundFlag);
}


UINT32 FSL_InitEEBlock (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, void(*CallBack)(void))
{
	   UINT32	     temp_MSR;
    UINT32      enable_MSR;	    /* temporary space for MSR*/
    UINT32      ebHigh;         /* enabled blocks for high space Flash block*/
    UINT32      ebMiddle;       /* enabled blocks for middle space Flash block*/
    UINT32      ebLow;          /* enabled blocks for low space Flash block*/
    UINT32      faildAddr;      /* failed address for blank check*/
    UINT32      returnCode;     /* the return code*/
    UINT32      dest;           /* blank checking destination address*/
    UINT32      size;           /* blank checking size*/
    UINT32      readBuff;       /* the buffer for reading*/
    UINT64      faildData;      /* failed data for blank check*/
    UINT64      source;         /* the source data buffer for change block status field*/
    UINT64      erasingCycle;   /* save the block erasing cycles*/

    /* initialize the enabled block variables*/
    ebHigh = ebMiddle = ebLow = 0;
    /*store configuration pointer for the block to be erased*/
    blockConfInternal = blockConf;
    /*store H&FA Register base*/
    c90flRegBaseInternal = c90flRegBase;


    /* judge the block space*/
    switch (blockConf->blockSpace)
    {
        case LOW_ADDRESS_SPACE:
            ebLow = blockConf->enabledBlock;
            break;

        case MID_ADDRESS_SPACE:
            ebMiddle = blockConf->enabledBlock;
            break;

        case HIGH_ADDRESS_SPACE:
            ebHigh = blockConf->enabledBlock;
            break;

        default:
            return (EE_ERROR_BLOCK_CONFIG);
    }

    /* get the block status*/
    dest = blockConf->blockStartAddr;
    size = DWORD_SIZE;
    readBuff = (UINT32)(&source);

	/*save old MSR*/
	temp_MSR = readAndClearEei();
	enable_MSR = 0x00208200;
	restoreEei(enable_MSR);
	/*Set ECC Error Module flag*/
    eccErrorModule_Flag = EE_MODULE;

    source = FSL_FlashRead64(blockConf->blockStartAddr);

    /*Reset ECC Error Module flag*/
    eccErrorModule_Flag = NONE;
    /*restore MSR*/
    restoreEei(temp_MSR);
    /*reuse temp_MSR to store the erasing cycles of the block*/
    temp_MSR = dest + PROGRAMMABLE_SIZE;

    erasingCycle = 0;
   	if (eccErrorStatus_Flag == FALSE)
	{
	  /* check block status*/
	  if (source == BLOCK_STATUS_BLANKED)
	  {
	      /* no need to erase again*/
	      return(EE_OK);
	  }
	  else if ((source == BLOCK_STATUS_COPY_START) ||
	           (source == BLOCK_STATUS_ACTIVE)	  ||
	           (source == BLOCK_STATUS_DELETE))
	  {
	      /* get the old erasing cycles*/
	      /* dest is updated in previous FSL_FlashRead*/
	      size = DWORD_SIZE;
	      readBuff = (UINT32)(&erasingCycle);
	      FSL_FlashRead(c90flRegBase, &temp_MSR, &size, &readBuff, CallBack);
	  }
	  else
	  {
	      /* reset the erasing cycles*/
	      erasingCycle = 0;
	  }
	}
    else /* Just reset flag if ECC error has been found */ 
    {
         eccErrorStatus_Flag = FALSE;
    }
    
    erasingCycleInternal = erasingCycle;

    sourceInternal = source;
    /* start the erase operation on the block*/
    returnCode = FSL_FlashEraseStart(c90flRegBase, blockConf->blockStartAddr, ebLow, ebMiddle, ebHigh);
    if (returnCode != EE_OK)
    {
        return (EE_ERROR_PE_OPT);
    }

    /*erase synchronously for initialization*/
    if( FALSE == blockSwap_Flag)
    {

       /* check status and end operation*/
       while (1)
       {
           if(NULL_CALLBACK != CallBack)
           {
              CallBack();
           }

           returnCode = FSL_FlashCheckStatus(c90flRegBase);
           if (returnCode == EE_OK)
           {
               break;
            }
           else if (returnCode == EE_ERROR_PE_OPT)
           {
              return (returnCode);
           }
       }

       /* blank check and update block status*/
       /* blank check */
       dest = blockConf->blockStartAddr;
       size = blockConf->blockSize;

       while (size)
       {
           /* perform blank check*/
           returnCode = FSL_DataVerify(c90flRegBase, TRUE, &dest, &size, (UINT32*)&source, &faildAddr, &faildData, CallBack);
           if (returnCode != EE_OK)
           {
              return (EE_ERROR_PE_OPT);
           }
       }

       /* program the increased erasing cycles back*/
       returnCode = FSL_ProgramDoubleWord(c90flRegBase, blockConf->blockStartAddr + PROGRAMMABLE_SIZE, (erasingCycle + 1), CallBack);
       if (returnCode != EE_OK)
       {
          return (returnCode);
       }

       /* update the block status to BLANKED*/
       returnCode = FSL_ProgramDoubleWord(c90flRegBase, blockConf->blockStartAddr,BLOCK_STATUS_BLANKED, CallBack);
       if (returnCode != EE_OK)
       {
          return (returnCode);
       }

       /* set blank space pointer*/
       blockConf->blankSpace = blockConf->blockStartAddr + 2 * PROGRAMMABLE_SIZE;
    }
    /*erase asynchronously for normal block swapping*/
    else
    {
      eraseStatus_Flag = ERASE_INPROGRESS;
    }

    return (EE_OK);
}

UINT32 FSL_ProgramDoubleWord (UINT32 c90flRegBase, UINT32 dest, UINT64 data, void(*CallBack)(void))
{
    UINT32 returnCode;      /* return code*/
    UINT16 size=0;            /* data size*/
    UINT32 address;         /* data buffer address*/

    /* set the programmed data size*/
    size = DWORD_SIZE;

    /* assign the data buffer address*/
    address = (UINT32)(&data);

    while(size > 0)
    {
          /* program the status field*/
          returnCode = FSL_FlashProgramStart(c90flRegBase, &dest, &size, &address);
          if (returnCode != EE_OK)
          {
              return (EE_ERROR_PE_OPT);
          }

          /* check high voltage status*/
          while (1)
          {
              if(NULL_CALLBACK != CallBack)
              {
                 CallBack();
              }

              returnCode = FSL_FlashCheckStatus(c90flRegBase);
              if (returnCode == EE_OK)
              {
                  break;
              }
              else if (returnCode == EE_ERROR_PE_OPT)
              {
                 return (returnCode);
              }
          }
    }
    
    /* check the programmed data against the source data*/
    if (FSL_FlashRead64(dest - DWORD_SIZE) != data)
    {
        return (EE_ERROR_PE_OPT);
    }

    return (EE_OK);
}

UINT32* address;
UINT32 ProgramCCP_DoubleWord (UINT32 c90flRegBase, UINT32 dest, UINT32* source, void(*CallBack)(void))
{
    UINT32 returnCode;      /* return code*/
    UINT16 size=0;            /* data size*/         /* data buffer address*/

    /* set the programmed data size*/
    size = DWORD_SIZE;
	address = source;
    while(size > 0)
    {
          /* program the status field*/
          returnCode = FSL_FlashProgramStart(c90flRegBase, &dest, &size, source);
          if (returnCode != EE_OK)
          {
              return (EE_ERROR_PE_OPT);
          }

          /* check high voltage status*/
          while (1)
          {
              if(NULL_CALLBACK != CallBack)
              {
                 CallBack();
              }

              returnCode = FSL_FlashCheckStatus(c90flRegBase);
              if (returnCode == EE_OK)
              {
                  break;
              }
              else if (returnCode == EE_ERROR_PE_OPT)
              {
                 return (returnCode);
              }
          }
    }
    
    /* check the programmed data against the source data*/
    if (FSL_FlashRead64(dest - DWORD_SIZE) != (UINT64)source)
    {
        return (EE_ERROR_PE_OPT);
    }

    return (EE_OK);
}

UINT32 FSL_CopyDataRecord (UINT32 c90flRegBase, UINT16 dataID, UINT16 dataSize, UINT32 source, UINT32 dest, void(*CallBack)(void))
{
    UINT32          counter;        /* loop counter*/
    UINT32          returnCode;     /* return code*/
    UINT64          buffer;         /* data buffer*/
    UINT32          failAddr;       /* used for program verification*/
    UINT64          failData;       /* used for program verification*/
    UINT32          destBak;        /* backup of dest*/
    UINT32          sourceBak;      /* backup of source*/
    UINT32          dataSizeBak;    /* backup of data size*/

    /* save record start address*/
    destBak = dest;

    /* update the record status to PROGRAM_START*/
    returnCode = FSL_ProgramDoubleWord(c90flRegBase, dest, RECORD_STATUS_PROGRAM_START, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    /* prepare ID, size and some data field*/
    buffer = CREATE_WORD64(0x00000000, 0xFFFFFFFF);

    /* add ID and size field to buffer*/
    buffer = ((UINT64)dataID << 48) | ((UINT64)dataSize << 32) | buffer;

    /* fill data into last word (4 bytes)*/
    for(counter = 4; (dataSize > 0) && (counter < 8); dataSize--, counter++)
    {
        /* copy the first 1 to 4 bytes data*/
        WRITE8(((UINT32)(&buffer) + counter), FSL_FlashRead8(source));

        /* update source data pointer*/
        source++;
    }

    /* program ID, size and some data field*/
    returnCode = FSL_ProgramDoubleWord(c90flRegBase, (dest + PROGRAMMABLE_SIZE), buffer, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    /* update the record status to PROGRAM_IDSIZE*/
    returnCode = FSL_ProgramDoubleWord(c90flRegBase, dest, RECORD_STATUS_PROGRAM_IDSIZE, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    /* program data to Flash block*/
    /* update the pointer to data field (skip status field and ID, size and word data field)*/
    dest += 2 * PROGRAMMABLE_SIZE;

    /* save remained data size*/
    sourceBak = source;
    dataSizeBak = dataSize;

    while (dataSize > 0)
    {
        /* start program*/
        returnCode = FSL_FlashProgramStart(c90flRegBase, &dest, &dataSize, &source);
        if (returnCode != EE_OK)
        {
            return (EE_ERROR_PE_OPT);
        }

        /* check status and end operation*/
        while (1)
        {
            if(NULL_CALLBACK != CallBack)
            {
               CallBack();
            }

            returnCode = FSL_FlashCheckStatus(c90flRegBase);
            if (returnCode == EE_OK)
            {
                break;
            }
            else if (returnCode == EE_ERROR_PE_OPT)
            {
                return (returnCode);
            }
        }
    }

    /* verify the programmed data*/
    while (dataSizeBak > 0)
    {
        /* restore word data address*/
        dest = destBak + 2 * PROGRAMMABLE_SIZE;

        returnCode = FSL_DataVerify(c90flRegBase, FALSE, &dest, &dataSizeBak, &sourceBak, &failAddr, &failData, CallBack);
        if (returnCode != EE_OK)
        {
            return (EE_ERROR_PE_OPT);
        }
    }

    /* data programmed complete and update the status field to PROGRAM_COMPLETE*/
    returnCode = FSL_ProgramDoubleWord(c90flRegBase, destBak, RECORD_STATUS_PROGRAM_COMPLETE, CallBack);
    if (returnCode != EE_OK)
    {
        return (returnCode);
    }

    return (EE_OK);
}


BOOL FSL_CheckFreeSpace (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT32* freeSpaceSize, void(*CallBack)(void))
{
    /* go through the entire block to get the blank space address with reserved data ID (0xFFFF)*/
    if (FSL_SearchInBlock (blockConf, c90flRegBase, 0xFFFF, freeSpaceSize, CallBack) == EE_ERROR_RECORD_STATUS)
    {
        /* data record status error*/
        return (FALSE);
    }

    /* calculate the free space*/
    *freeSpaceSize = blockConf->blockStartAddr + blockConf->blockSize - blockConf->blankSpace;

    return (TRUE);
}


BOOL FSL_UpdateCacheTable(CACHE_TABLE* cTable, UINT16 dataID, UINT32 newValue)
{
    UINT32 temp;

    /* get the maximum data ID number in cache table*/
    temp = cTable->tableSize / CTABLE_ITEM_SIZE;

    /* check if it is in the cache table*/
    if ( dataID >= temp )
    {
        /* data should not be in the cache table*/
        return (FALSE);
    }

    /* insert the data ID into the cache table*/
    temp = cTable->tableStartAddress;

    /* update the cache table item with new value*/
    WRITE32((temp + CTABLE_ITEM_SIZE * dataID), newValue);

    return (TRUE);
}


UINT32 FSL_SearchInTable (CACHE_TABLE* cTable, UINT16 dataID, UINT32* expDataAddress)
{
    UINT32      counter;        /* loop counter*/

    /* check if the data ID is in the cache table ID range*/
    counter = cTable->tableSize / CTABLE_ITEM_SIZE;

    if (dataID >= counter)
    {
        /* data ID is out of range*/
        return (EE_ERROR_NOT_IN_CACHE);
    }

    /* fetch the required data record address*/
    *expDataAddress = READ32(cTable->tableStartAddress + dataID * CTABLE_ITEM_SIZE);

    if (*expDataAddress == 0xFFFFFFFF)
    {
        /* no data found*/
        return (EE_ERROR_DATA_NOT_FOUND);
    }
    else
    {
        /* data found*/
        return (EE_OK);
    }
}

#if defined(__ghs__)
/* Use GHS intrinsic */
static UINT32 readAndClearEei(void)
{
  UINT32 u32Temp = __GETSR();
   __DI();
   return(u32Temp);   
}
#else
asm UINT32 readAndClearEei(void)
{
  mfmsr	r3
  wrteei 0
}
#endif

#if defined(__ghs__)
/* Use GHS intrinsic */
static void restoreEei(UINT32 value)
{
   __SETSR(value);
}
#else
asm void restoreEei(UINT32 value)
{
% reg value
  mtmsr	value
}
#endif
