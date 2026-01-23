 /*******************************************************************
* PROJECT : EEPROM Emulation Driver (EED) For C90 flash demo
* FILE    : ee_demo.h
*
* DESCRIPTION : This file contains macro definition for EED demo.               
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

#ifndef _EE_DEMO_H_
#define _EE_DEMO_H_

// C90 Data Flash Control Register
#define C90FL_DATA_REG_BASE             0xC3F8C000
#define MAIN_DATA_ARRAY_BASE            0x00800000


// Unlock macro
#define C90FL_DATA_LML_SLOCK             0x00100000   /* Shadow Lock */
#define C90FL_DATA_SLL_SSLOCK            0x00100000   /* Secondary Shadow Lock */

/* Low space Data block 0 */
#define LOW_DATA_BLOCK0_OFFSET          0x0000  /* Offset of low block 0 */
#define LOW_DATA_BLOCK0_SIZE            0x4000  /* 16KB size */
#define LOW_DATA_BLOCK0_MASK            0x00000001

/* Low space Data block 1 */
#define LOW_DATA_BLOCK1_OFFSET          0x4000  /* Offset of low block 0 */
#define LOW_DATA_BLOCK1_SIZE            0x4000  /* 16KB size */
#define LOW_DATA_BLOCK1_MASK            0x00000002

/* Low space Data block 2 */
#define LOW_DATA_BLOCK2_OFFSET          0x8000  /* Offset of low block 0 */
#define LOW_DATA_BLOCK2_SIZE            0x4000  /* 16KB size */
#define LOW_DATA_BLOCK2_MASK            0x00000004

/* Low space Data block 3 */
#define LOW_DATA_BLOCK3_OFFSET          0xC000  /* Offset of low block 0 */
#define LOW_DATA_BLOCK3_SIZE            0x4000  /* 16KB size */
#define LOW_DATA_BLOCK3_MASK            0x00000008

/* data record ID*/
#define DATA_ID_0       0
#define DATA_ID_1       1
#define DATA_ID_2       2
#define DATA_ID_3       3
#define DATA_ID_4       4
#define DATA_ID_5       5
#define DATA_ID_6       6
#define DATA_ID_7       7

extern uint8_t EE_Status;
#define WRITE_REQ BIT31
#define EE_BUSY   BIT30
#define EE_ERROR  BIT29

typedef struct{
   uint16_t  data_id;
   uint32_t  ee_data;
} s_EE_Buffer;

#define MAX_EE_BUFFER_SIZE 2

typedef struct{
   uint16_t record_id;
   uint16_t* data_address;
} s_EE_Data;


#define TEST_COUNTER1_INDEX 0
#define TEST_COUNTER2_INDEX 1

#define NUMBER_EE_DATA_VARIABLES 2

#define EE_MANAGER_INIT     0x00
#define EE_MANAGER_SHADOW   0x01
#define EE_MANAGER_IDLE     0x02
#define EE_MANAGER_WRITE    0x03
#define EE_MANAGER_VERIFY   0x04
#define EE_MANAGER_ERROR    0x05

/* Private variables ---------------------------------------------------------*/
extern uint8_t EEPROM_Manager_State;
extern UINT64 RxData;

/* Private function prototypes -----------------------------------------------*/
extern void FlashEEPROM_Manager(uint32_t Length);
extern void FlashWriteForDtc(UINT64* WriteData,uint32_t Length);
#endif /*_EE_DEMO_H_*/

