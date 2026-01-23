/*******************************************************************
* PROJECT : EEPROM Emulation Driver (EED) For C90 flash
* FILE    : ee_emulation.h
*
* DESCRIPTION : This the header file to include EED  
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

#ifndef _EE_EMULATION_H_
#define _EE_EMULATION_H_

/* macros for scheduling*/
#define NUMBER_OF_SEARCHING_RECORD_IN_BLOCK     0x2000
#define DATA_VERIFY_NUMBER                      0x5000
#define DATA_READ_NUMBER                        0x5000

/* macros for CallBack period controlling*/
#define CALLBACK_READ_BYTES_NUMBER              0x0020
#define CALLBACK_SEARCHING_BYTES_NUMBER         0x001A
#define CALLBACK_VERIFY_BYTES_NUMBER            0x0020
#define CALLBACK_FILL_BUFFER                    0x0060

/* NULL callback */
#define NULL_CALLBACK             ((void *) 0xFFFFFFFF)

/* structure declaration*/
typedef struct
{
    UINT64  dataStatus;                 /* the data record status*/
    UINT16  dataID;                     /* the unique data ID*/
    UINT16  dataSize;                   /* the data size*/
    UINT32  wordData;                   /* the first 4 bytes data*/
}DATA_RECORD_HEAD;


typedef struct
{
    UINT32  enabledBlock;               /* the block bit map in specific space*/
    UINT32  blockStartAddr;             /* the block start address*/
    UINT32  blockSize;                  /* the block size*/
    UINT32  blankSpace;                 /* the address pointer to the blank space*/
    UINT8   blockSpace;                 /* the space (low, middle or high) for the block*/
}BLOCK_CONFIG;


typedef struct
{
    UINT32  tableStartAddress;          /* the start address of cache table*/
    UINT32  tableSize;                  /* the size of cache table in byte*/
}CACHE_TABLE;


typedef struct
{
    UINT32          numberOfBlock;      /* the number of blocks used for emulation*/
    UINT32          activeBlockIndex;   /* the active block index*/
    volatile UINT32 blockWriteFlag;     /* the writing flag of the blocks*/
    UINT32          c90flRegBase;       /* the register base address for C90FL*/
    BOOL            cacheEnable;        /* the flag to enable/disable the cache table*/
    CACHE_TABLE*    cTable;             /* the cache table structure*/
    BLOCK_CONFIG**  flashBlocks;        /* the block configuration array pointer*/
}EEPROM_CONFIG;

typedef enum
{
  NONE = 0,
  EE_MODULE = 1,
  OTHER_MODULE = 2
}EE_ModuleType;

typedef enum
{
  ERASE_NOTSTARTED=0,
  ERASE_DONE = 1,
  ERASE_FAIL = 2,
  ERASE_INPROGRESS = 3,
  ERASE_SWAPERROR = 4
}EE_EraseType;

/* Flag to keep track of invalid data*/
extern UINT8 recordStatusInvalid_Flag;
/* Flag to keep track of ECC Error Status*/
extern UINT8 volatile eccErrorStatus_Flag;
/* Flag to keep track of ECC Error Module*/
extern volatile EE_ModuleType eccErrorModule_Flag;
/* structure to store block configuration during erase*/
extern BLOCK_CONFIG* blockConfInternal;
/* Flag to keep track of Block Swapping*/
extern UINT8 blockSwap_Flag;
/*Variable to store C90FL Register Base*/
extern UINT32 c90flRegBaseInternal;
/*Variable to store erasing cycles */
extern UINT64 erasingCycleInternal;
/*Variable to store source*/
extern UINT64 sourceInternal;
/* Flag to keep track of Erase State*/
extern EE_EraseType eraseStatus_Flag;
/*Variable to store size of the data record*/
extern UINT32 tempInternal;
/*Variable to store DataID*/
extern UINT16 dataIDInternal;
/*Variable to store Data Size*/
extern UINT16 dataSizeInternal;
/*Variable to store source address while swap*/
extern UINT32 sourceInternalSwap;

/* return code definition*/
#define EE_OK                        0x00000000
#define EE_FIRST_TIME_INITIALIZATION 0x00000001
#define EE_INFO_HVOP_INPROGRESS      0x00000002
#define EE_INFO_PROGRAM_SUSPEND      0x00000004
#define EE_INFO_ERASE_SUSPEND        0x00000010
#define EE_ERROR_WRITE_IN_PROGRESS   0x00000020
#define EE_ERROR_PE_OPT              0x00000040
#define EE_ERROR_MISMATCH            0x00000080
#define EE_ERROR_BLOCK_STATUS        0x00000100
#define EE_ERROR_RECORD_STATUS       0x00000200
#define EE_ERROR_BLOCK_CONFIG        0x00000400
#define EE_ERROR_DATA_NOT_FOUND      0x00000800
#define EE_ERROR_NOT_IN_CACHE        0x00001000
#define EE_ERROR_NO_ENOUGH_SPACE     0x00002000


/* MCU ID Register*/
#define SIU_MIDR                    0xC3F90004

/* shadow row base address*/
#define SHADOW_BASE_ADDR            0x00FFFC00

/* address space macro*/
#define LOW_ADDRESS_SPACE           0x01
#define MID_ADDRESS_SPACE           0x02
#define HIGH_ADDRESS_SPACE          0x03

/* Offsets of C90FL Control Registers*/
#define C90FL_MCR                    0x0000        /* Module Configuration Register */
#define C90FL_LML                    0x0004        /* Low/Mid Address Sapce Block Locking Register */
#define C90FL_HBL                    0x0008        /* High Address Sapce Block Locking Register */
#define C90FL_SLL                    0x000C        /* Secondary Low/Mid Address Space Block Locking Register */
#define C90FL_LMS                    0x0010        /* Low/Mid Address Space Block Select Register */
#define C90FL_HBS                    0x0014        /* High Address Space Block Select Register */

/* Module Configuration Register */
#define C90FL_MCR_EER                0x00008000   /* ECC Event Error */
#define C90FL_MCR_RWE                0x00004000   /* Read While Write Event Error */
#define C90FL_MCR_DONE               0x00000400   /* State Machine Status */
#define C90FL_MCR_PEG                0x00000200   /* Program/Erase Good */
#define C90FL_MCR_PGM                0x00000010   /* Program */
#define C90FL_MCR_PSUS               0x00000008   /* Program Suspend */
#define C90FL_MCR_ERS                0x00000004   /* Erase */
#define C90FL_MCR_ESUS               0x00000002   /* Erase Suspend */
#define C90FL_MCR_EHV                0x00000001   /* Enable High Voltage */

/* read and write macros*/
#define WRITE8(address, value)      (*(VUINT8*)(address) = (value))
#define READ8(address)              ((UINT8)(*(VUINT8*)(address)))
#define SET8(address, value)        (*(VUINT8*)(address) |= (value))
#define CLEAR8(address, value)      (*(VUINT8*)(address) &= ~(value))

#define WRITE16(address, value)     (*(VUINT16*)(address) = (value))
#define READ16(address)             ((UINT16)(*(VUINT16*)(address)))
#define SET16(address, value)       (*(VUINT16*)(address) |= (value))
#define CLEAR16(address, value)     (*(VUINT16*)(address) &= ~(value))

#define WRITE32(address, value)     (*(VUINT32*)(address) = (value))
#define READ32(address)             ((UINT32)(*(VUINT32*)(address)))
#define SET32(address, value)       (*(VUINT32*)(address) |= (value))
#define CLEAR32(address, value)     (*(VUINT32*)(address) &= ~(value))

#define WRITE64(address, value)     (*(VUINT64*)(address) = (value))
#define READ64(address)             ((UINT64)(*(VUINT64*)(address)))
#define SET64(address, value)       (*(VUINT64*)(address) |= (value))
#define CLEAR64(address, value)     (*(VUINT64*)(address) &= ~(value))

/* macros for Flash suspend and resume*/
#define C90FLMCR_EED_BIT_SET(MCRAddress, mask)      \
    WRITE32(MCRAddress, ((mask | READ32(MCRAddress)) & (~(C90FL_MCR_EER | C90FL_MCR_RWE))))

#define C90FLMCR_EED_BIT_CLEAR(MCRAddress, mask)    \
    WRITE32(MCRAddress, (((~mask) & READ32(MCRAddress)) & (~(C90FL_MCR_EER | C90FL_MCR_RWE))))

#define FLASH_RESUME_WAIT           15

/* macro for 64-bit data comparison*/
#define CREATE_WORD64(high, low)    (VUINT64)((VUINT64)(high) << 32 | (low))


/* data record status state*/
#define ERASED_HIGH                     0xFFFFFFFF
#define ERASED_LOW                      0xFFFFFFFF
#define RECORD_STATUS_ERASED            CREATE_WORD64(ERASED_HIGH, ERASED_LOW)

#define PROGRAM_START_HIGH              0xFFFFFFFF
#define PROGRAM_START_LOW               0xFFFF0000
#define RECORD_STATUS_PROGRAM_START     CREATE_WORD64(PROGRAM_START_HIGH, PROGRAM_START_LOW)

#define PROGRAM_IDSIZE_HIGH             0xFFFF0000
#define PROGRAM_IDSIZE_LOW              0xFFFF0000
#define RECORD_STATUS_PROGRAM_IDSIZE    CREATE_WORD64(PROGRAM_IDSIZE_HIGH, PROGRAM_IDSIZE_LOW)

#define PROGRAM_COMPLETE_HIGH           0xFFFF0000
#define PROGRAM_COMPLETE_LOW            0x00000000
#define RECORD_STATUS_PROGRAM_COMPLETE  CREATE_WORD64(PROGRAM_COMPLETE_HIGH, PROGRAM_COMPLETE_LOW)

#define DELETE_HIGH                     0x00000000
#define DELETE_LOW                      0x00000000
#define RECORD_STATUS_DELETE            CREATE_WORD64(DELETE_HIGH, DELETE_LOW)


/* block status state*/
#define BLOCK_STATUS_ERASED             CREATE_WORD64(ERASED_HIGH, ERASED_LOW)

#define BLANKED_HIGH                    0x0000FFFF
#define BLANKED_LOW                     0xFFFFFFFF
#define BLOCK_STATUS_BLANKED            CREATE_WORD64(BLANKED_HIGH, BLANKED_LOW)

#define COPY_START_HIGH                 0x00000000
#define COPY_START_LOW                  0xFFFFFFFF
#define BLOCK_STATUS_COPY_START         CREATE_WORD64(COPY_START_HIGH, COPY_START_LOW)

#define ACTIVE_HIGH                     0x00000000
#define ACTIVE_LOW                      0x0000FFFF
#define BLOCK_STATUS_ACTIVE             CREATE_WORD64(ACTIVE_HIGH, ACTIVE_LOW)

#define BLOCK_DELETE_HIGH                     0x00000000
#define BLOCK_DELETE_LOW                      0x00000000
#define BLOCK_STATUS_DELETE             CREATE_WORD64(BLOCK_DELETE_HIGH, BLOCK_DELETE_LOW)

#define BLOCK_INVALID_HIGH                     0xFFFFFFFF
#define BLOCK_INVALID_LOW                      0xFFFFFFFF
#define BLOCK_STATUS_INVALID             CREATE_WORD64(BLOCK_INVALID_HIGH, BLOCK_INVALID_LOW)


/* const definition*/
#define DATAID_SIZE                 2
#define CTABLE_ITEM_SIZE            4
#define PROGRAMMABLE_SIZE           8
#define WORD_SIZE                   4
#define DWORD_SIZE                  8
#define PAGE_SIZE                   16
#if defined(C90_DFO)
  #define PAGE_SIZE_BPS             4   /* Page buffer size for C90DFO flash */
#else
  #define PAGE_SIZE_BPS             8   /* Page buffer size for C90LC flash*/
#endif

/* function declarations*/
/* flash driver functions*/
UINT32 FSL_FlashProgramStart (UINT32 c90flRegBase, UINT32* dest, UINT16* size, UINT32* source);
UINT32 FSL_FlashEraseStart (UINT32 c90flRegBase, UINT32 interlockWriteAddress, UINT32 lowEnabledBlock, UINT32 midEnabledBlock, UINT32 highEnabledBlock);
UINT32 FSL_FlashCheckStatus (UINT32 c90flRegBase);
UINT32 FSL_DataVerify (UINT32 c90flRegBase, BOOL blankCheck, UINT32* dest, UINT32* size, UINT32* source, UINT32 *compareAddress, UINT64 *compareData, void(*CallBack)(void));
UINT32 FSL_FlashSuspend (UINT32 c90flRegBase);
UINT32 FSL_FlashResume (UINT32 c90flRegBase, UINT32 resumeType);
UINT32 FSL_FlashRead (UINT32 c90flRegBase, UINT32* dest, UINT32* size, UINT32* buffer, void(*CallBack)(void));
UINT32 FSL_FlashEraseAbort (UINT32 c90flRegBase);

/* low level functions*/
UINT32 FSL_SearchRecordInBlock (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT32 bufferAddress, UINT32 bufferSize, UINT16 startID, UINT32* startAddrInBlock, UINT16* nextStartID, void(*CallBack)(void));
UINT8  FSL_FlashRead8(UINT32 address);
UINT64 FSL_FlashRead64(UINT32 address);
void   FSL_ReadRecordHead(UINT32 address, DATA_RECORD_HEAD *pLocRecHead);
UINT32 readAndClearEei(void);
void   restoreEei(UINT32 value);
void   EER_exception_handler(void);

/* middle level functions*/
BOOL   FSL_CheckFreeSpace(BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT32* freeSpaceSize, void(*CallBack)(void));
UINT32 FSL_SearchInTable (CACHE_TABLE* cTable, UINT16 dataID, UINT32* expDataAddress);
UINT32 FSL_SearchInBlock (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT16 dataID, UINT32* expDataInBlock, void(*CallBack)(void));
BOOL   FSL_SearchActiveBlock (EEPROM_CONFIG * eepromConfig, UINT32* activeBlockIndex, void(*CallBack)(void));
UINT32 FSL_RecordBuffering (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, UINT32 bufferAddress, UINT32 bufferSize, UINT16* startID, void(*CallBack)(void));
UINT32 FSL_InitEEBlock (BLOCK_CONFIG* blockConf, UINT32 c90flRegBase, void(*CallBack)(void));
UINT32 FSL_ProgramDoubleWord (UINT32 c90flRegBase, UINT32 address, UINT64 data, void(*CallBack)(void));
UINT32 ProgramCCP_DoubleWord (UINT32 c90flRegBase, UINT32 address, UINT32* source, void(*CallBack)(void));
UINT32 FSL_CopyDataRecord (UINT32 c90flRegBase, UINT16 dataID, UINT16 dataSize, UINT32 source, UINT32 dest, void(*CallBack)(void));
BOOL   FSL_UpdateCacheTable(CACHE_TABLE* cTable, UINT16 dataID, UINT32 newValue);
UINT32 FSL_BlockSwapping(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void));


/* high level functions*/
UINT32 FSL_InitEeprom(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void));
UINT32 FSL_ReadEeprom(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT32 source, void(*CallBack)(void));
UINT32 FSL_WriteEeprom (EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT16 dataSize, UINT32 source, void(*CallBack)(void));
UINT32 FSL_DeleteRecord(EEPROM_CONFIG* eepromConfig, UINT16 dataID, void(*CallBack)(void));
UINT32 FSL_RemoveEeprom(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void));
UINT32 FSL_ReportEepromStatus(EEPROM_CONFIG* eepromConfig, UINT64* erasingCycles, UINT32* failedAddress, void(*CallBack)(void));
void   FSL_MainFunction(void(*CallBack)(void));
UINT32 FSL_AbortFunction(EEPROM_CONFIG* eepromConfig);

#endif /*_EE_EMULATION_H_*/
