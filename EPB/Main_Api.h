#include "Defines.h"
#include "Typedefs.h"

#define SYSTEM_POWER_DOWN_TIME				(5000U /5U)	/* 5sec */

#define	SYSTEM_TICK_1MS						(1000U)		/* [usec] */
#define	SYSTEM_TICK_5MS						(5000U)		/* [usec] */


#define SYSTEM_TIME_A						(1U)
#define SYSTEM_TIME_B 						(0U)
#define SYSTEM_TIME 						(SYSTEM_TIME_B)

#if SYSTEM_TIME
#define SYSTEM_BOOT_INIT_TIME	(50U)		/* 50ms */
#define SYSTEM_BOOT_TIME		(350U)		/* 50ms + 300ms */
#define SYSTEM_IDLE_TIME		(380U)		/* 50ms + 300ms + 30ms */
#define SYSTEM_INITIAL_TIME		(2080U)		/* 50ms + 300ms + 30ms + 1700ms */
#define SYSTEM_WARN_LAMP_TIME	(2580U)		/* 50ms + 300ms + 30ms + 1700ms + 500ms */
#else
#define SYSTEM_BOOT_INIT_TIME	(50U / 5U)		/* 50ms */
#define SYSTEM_INITIAL_TIME		(500U)		/* 50ms + 450ms */
#define SYSTEM_WARN_LAMP_TIME	(1000U)		/* 50ms + 450ms + 500ms */

#endif

#define SYSTEM_LOOP_5MS			(5U)		/* 5ms */
#define SYSTEM_LOOP_1SEC		(1000U)		/* 1000ms */
#define SYSTEM_LOOP_CNT_MAX		(6000U /5U)		/* 6000ms */
#define SYSTEM_POWER_OFF_TIME	(4000U /5U)		/* 5 - (1: ign off detection time) sec */ 
#define SYSTEM_SAFETY_TIME		(30000U)	/* 30sec */

#define SYSTEM_STATUS_INIT			(0U)
#define SYSTEM_STATUS_NORMAL		(1U) 
#define MTDRV_STATUS_INIT			(0U) 
#define MTDRV_STATUS_NORMAL			(1U) 
#define SYSTEM_STATUS_POWER_DOWN	(2U)
#define SYSTEM_STATUS_EMERGENCY		(3U) 


#define WAKEUP_INIT		(0U)
#define WAKEUP_START_S0	(1U)
#define WAKEUP_START	(2U)
#define WAKEUP_START_S1	(3U)
#define WAKEUP_START_S2	(4U)
#define WAKEUP_START_S3	(5U)
#define WAKEUP_DOWN		(6U)
#define WAKEUP_OFF		(7U)

#define EPB_DEBUG_ON	(0U)
#define DEBUG_MAIN_ALIVE	(1U)

typedef struct {
	uint16 TickCount;	/* Software counter for system tick */ /* 2 */
	uint16 Tick;		/* Selected System tick */ /* 4 */
	uint16 TaskRun;		/* Current running task */ /* 6 */
	uint16 TaskMax;		/* Maximum repeated time task */ /* 8 */
	uint16 StepCount;	/* Operation Step Count */ /* 10 */
	uint8 EepromReadIndex;
	uint8 EepromClearIndex; /* 12 */
	struct {
		unsigned int Enable			: 1; /* Task scheduler enable bit */
		unsigned int SystemDown		: 1; /* IGN Switch off */
		unsigned int SystemOff		: 1; /* IGN Switch off hold 1 sec */
		unsigned int PowerOff		: 1; /* System Power off */
		unsigned int EmergencyMode	: 1; /* not Match Board ID */
		unsigned int SystemRunning	: 1; /* System Running */
		unsigned int StartDownload	: 1; /* CAN Download Start */
		unsigned int ControlStep	: 3; /* Control Step */
		unsigned int RunMode		: 3; /* System Run Mode */
		unsigned int OperationMode	: 3; /* System Operation Mode */
		unsigned int SystemSafety	: 1;
		unsigned int SystemSafetyCount	: 15;
	} Flag; /* 16 */
	uint32_t PowerOffCount; /* 20 */
	uint8 WakeupStatusTemp;
	uint8 WakeupStatus;
	uint8 WakeupStep;
	uint8 Reserved;  /* 24 */

	uint32_t TaskCount_5ms_Cnt;	/* 1ms Task Scheduler Count */
	uint32_t TaskCount_10ms_Cnt;	/* 5ms Task Scheduler Count */
	uint32_t TaskCount_20ms_Cnt;	/* 10ms Task Scheduler Count */
	uint32_t TaskCount_5ms_TimeElapsed;	/* 1ms Task Scheduler TimeElapsed */
	uint32_t TaskCount_10ms_TimeElapsed;	/* 5ms Task Scheduler TimeElapsed */
	uint32_t TaskCount_20ms_TimeElapsed;	/* 10ms Task Scheduler TimeElapsed */
	uint32_t TaskCount_5ms_TimeElapsed_Max;	/* 1ms Task Scheduler Max TimeElapsed */
	uint32_t TaskCount_10ms_TimeElapsed_Max;	/* 5ms Task Scheduler Max TimeElapsed */
	uint32_t TaskCount_20ms_TimeElapsed_Max;	/* 10ms Task Scheduler Max TimeElapsed */
	uint8 TaskCount_5ms_Error; /* 1ms Task Scheduler Count */
	uint8 TaskCount_10ms_Error; /* 5ms Task Scheduler Count */
	uint8 TaskCount_20ms_Error; /* 10ms Task Scheduler Count */

	
} TaskControl_t;

typedef enum {
	TASK_5ms	= 5,
	TASK_10ms 	= 10,
	TASK_20ms	= 20,
	TASK_1000ms = 1000,
	TASK_5000ms = 5000,
	TASK_none 	= 0xFFFF
} NO_OF_TICK_COUNT_FOR_TASK;

typedef struct{
	uint8 SystemStatus;
	uint8 MtDrvStatus;
	uint8 PowerOffOk;
	uint8 Session;
	uint8 L9369_InitStep;
	uint8 TLE9461_InitStep;
	unsigned int Debug : 1;
}Main_t;
void SetSystemDown(void);
void SetSystemOn(void);
void SetSystemOff(void);
void SetSyatemSafetyOn(void);
void SetSyatemSafetyOff(void);
void TaskScheduler_TaskCount(void);
void TaskScheduler_TaskCheck_1ms_Process(void);
void TaskScheduler_TaskCheck_5ms_Control(void);
void TaskScheduler_TaskCheck_5ms_Can(void);
void DemoApp(void);
uint8 GetDbsOpMode(void); 
void SetDbsOpMode(uint8 mode);
uint8 GetRunType(void); 
void SetRunType(uint8 mode);
uint8 GetWakeupStatus(void);
void SetWakeupStatus(uint8 status);
static void TaskScheduler_5ms(void);
static void TaskScheduler_10ms(void);
static void TaskScheduler_20ms(void);
void Task_Diagnostic(void);


