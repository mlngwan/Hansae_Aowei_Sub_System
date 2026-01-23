#include "Typedefs.h"
#include "CCP.h"
#include "EB_Prj.h"
NCS_PRI_VAR __attribute__((section(".noinit"))) u32 eBootFlag;

#define CAN_INIT_OFF			(0x00U)
#define CAN_INIT_IDLE			(0x01U)
#define CAN_INIT_PAS			(0x02U)
#define CAN_INIT_LAMP			(0x03U)
#define CAN_INIT_OK				(0x04U)
#define CAN_INIT_DOWN			(0x05U)
#define CAN_INIT_INIT			(0x06U)
#define CAN_INIT_NONE			(0xFFU)
#define CAN_INIT_IDLE_CLEAR		(0x10U)
#define CAN_INIT_DOWN_OFF		(0x55U)

#define DTC_IEHB     	  		0U
#define DTC_CAN_NUM				15U
#define DTC_PASS 				0U
#define DTC_FAIL 				1U
#define Confirmed_DTC			1U
#define Fail_DTC				0U
#define DTC_RESET 				0U
#define DTC_SET					1U
#define MissingMessage			0U
#define MessageFailure			1U 
#define InvalidData				2U //rolling counter
#define InvalidChecksum			3U
#define InvalidAlive			4U
#define InvalidData2			5U
#define InvalidData3			6U
#define InvalidData4			7U

typedef struct
{
	uint8_t Data[8];
}Data_Packet;

typedef struct
{
	union 
	{
		ubyte Data; // 1byte data
		struct 
		{
			ubyte Indicator			: 1U;	//bit7 warningIndicatorRequested
			ubyte NotCompleted		: 1U;	//bit6 testNotCompletedThisOperationCycle 
			ubyte tFSLC				: 1U;	//bit5 testFailedSinceLastClear
			ubyte tNCSLC			: 1U;	//bit4 testNotCompletedSinceLastClear
			ubyte Confirmed			: 1U;	//bit3 confirmedDTC	
			ubyte pDTC				: 1U;	//bit2 pendingDTC 
			ubyte FailedCycle		: 1U;	//bit1 testFailedThisOperationCycle
			ubyte Failed			: 1U;	//bit0 testFailed	
		}Bit;
	}Flag;
	ubyte Healing_counter;
	ubyte Failure_counter;
}Status_of_DTC;

typedef struct
{
	union 
	{
		ubyte Data; // 1byte data
		struct 
		{
			ubyte Failure_counter_CAN	: 7U;	
			ubyte Failed			: 1U;
		}Bit;
	}Failure_is[5];
}Status_of_CAN_DTC;

typedef struct{
	unsigned int fbRxInd_IEHB  : 1U;
	uint8 Cnt_BusOn;
	uint8 Cnt_BusOff;
	uint8 Cnt_CANBusOff;
	uint8 Cnt_CANBusOn;
	uint8 CANBUS_fail;
	uint8 CANBUS_confirmed;
	uint8 CANBUS_firstcheck;
	uint8 Cnt_Fail_IEHB;
	uint8 Func_Result;
	uint8 Rx_U8;
	uint16 Rx_U16;

}Can_t;

typedef struct
{
	ubyte ubStoreReq_DTC[DEM_DTC_NUMBER];
	ubyte ubStoreReq_Mutex[DEM_DTC_NUMBER];
}SaveDtcType;

typedef struct
{
	uint32 Cnt_ResetDelay;
	uint8 m_aubDtcActivation;
	uint8 gnRxFrameFlag;
	uint8 SPR_Req;
	uint8 BackupStep;
	uint8 TestModeEnable_Flag;
	unsigned int fUDS_VIN_Lean : 1;
	unsigned int fECU_Reset : 1;
	unsigned int fBootWriteRequest : 1;
	unsigned int fDTC_EraseAllRequest : 1;
	unsigned int fFingerPrintWrote : 1;
	unsigned int fAppInit_Mutex : 1;
	unsigned int fECU_Reset_Bit : 1;
	unsigned int fECU_ResetErase : 1;
	uint8 BootWriteAdd[4];
	uint8 ubFingerprint[12];
	uint8 DiagRxData[8];
	uint8 DiagTxData[8];
}Uds_t;

typedef struct
{
	unsigned int fEEP_ECC_Err : 1;
}Board_t;
void CanInitialize(void);
void CAN_Task(void);
void TxCanMessage(void);
void CAN_init(void);

uint8 DTC_CAN_Missing(uint8 can_num, uint8 fault_bit, uint8 dtcIdx);
uint8 DTC_CAN_MessageFailure(uint8 can_num, uint8 fault_bit, uint8 dtcIdx);
uint8 DTC_CAN_InvalidData(uint8 can_num, uint8 fault_bit, uint8 dtcIdx);
uint8 DTC_CAN_Checksum(uint8 can_num, uint8 fault_bit, uint8 dtcIdx);
uint8 DTC_CAN_AliveCounter(uint8 can_num, uint8 fault_bit, uint8 dtcIdx);
uint8 RollingCounter_Valuecheck(uint8 *RxRC,uint8 *IntRC);

extern void CAN_AllPolling(void);
extern void APP_Manage(void);
extern void EB_Manage(void);
extern void ccpDaq( CCP_BYTE eventChannel );
void EcuResetCheck(void);

