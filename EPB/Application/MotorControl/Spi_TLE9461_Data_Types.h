
#include "Typedefs.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define SPI_CANSBC_BOOT	(0x0U)
#define SPI_CANSBC_INIT	(0x1U)
#define SPI_CANSBC_RUN	(0x2U)
#define SPI_CANSBC_DOWN	(0x3U)

#define	MODE_GO_NORMAL	(0x0U)  
#define MODE_GO_SLEEP	(0x1U)
#define MODE_GO_STOP	(0x2U)
#define MODE_GO_RESET	(0x3U)



#define	CP_ENABLE		(0x1U)  
#define	CP_DISABLE		(0x0U)  

#define GO_STEP1			(0x1U)
#define GO_STEP2			(0x2U)
#define GO_STEP3			(0x3U)

#define GO_STEP4			(0x4U)


#define WRITE			 ((uint8)0x1U)
#define READ  			 ((uint8)0x0U)
#define INIT    		 ((uint8)0x01U)
#define NOMAL    		 ((uint8)0x02U)
#define SLEEP   		 ((uint8)0x03U)
#define STANDBY 		 ((uint8)0x04U)
#define WAKE    		 ((uint8)0x05U)
#define GO_NORMAL 		 (0x3A) 
#define GO_INIT			 (0x29)
#define GO_SLEEP 		 (0x33)
#define GO_STANDBY 		 (0x04)

#define CANSBC_DATA_SIZE   (2U)

typedef union {
	uint8 data[CANSBC_DATA_SIZE];
	uint16 data16;
	struct {
		#if 1
		unsigned int data	: 8;
		unsigned int r_w 	: 1;
		unsigned int addr	: 7;
		#else
		unsigned int addr : 7;
		unsigned int r_w 	: 1;
		unsigned int data	: 8;
		#endif
	} field;
} CANSBC_REG_MOSI;

typedef union {
	uint8 data[CANSBC_DATA_SIZE];
	uint16 data16;
	struct {
		#if 0
		unsigned int status	: 8;
		unsigned int data 	: 8;
		#else
		unsigned int data	: 8;
		unsigned int status	: 8;
		#endif
	} field;	
} CANSBC_REG_MISO;

typedef union {
	uint8 data;
	struct {
		unsigned int MODE 				: 2; 
/*
		SBC Mode Control
		00B , SBC Normal Mode
		01B , SBC Sleep Mode
		10B , SBC Stop Mode
		11B , SBC Reset: Soft Reset is executed (configuration of RSTN
		triggering in bit SOFT_RESET_RST)

*/
		unsigned int Reserved 			: 1;
		unsigned int VCC2_ON 			: 2; 
/*
		VCC2 Mode Control
		00B , VCC2 Off
		01B , VCC2 On in Normal Mode
		10B , VCC2 On in Normal and Stop Mode
		11B , VCC2 always On (except in SBC Init - if not in SBC
		Development Mode, SBC Restart and Fail-Safe Mode)

*/
		unsigned int VCC1_OV_RST 		: 1;
/*
		VCC1 Over Voltage leading to Restart / Fail-Safe Mode enable
		0B , VCC1_ OV is set in case of VCC1_OV; no SBC Restart or Fail-
		Safe is entered for VCC1_OV
		1B , VCC1_ OV is set in case of VCC1_OV; depending on the
		device configuration SBC Restart or SBC Fail-Safe Mode is
		entered (see Chapter 5.1.1);
*/
		unsigned int VCC1_RT		: 2;
/*
		VCC1 Reset Threshold Control
		00B , Vrt1 selected (highest threshold)
		01B , Vrt2 selected
		10B , Vrt3 selected
		11B , Vrt4 selected
*/
	} field;
} CANSBC_REG_DATA_0x01_M_S_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 1; 
		unsigned int SOFT_RESET_RST	: 1;
/*
		Soft Reset Configuration
		0B , RST is triggered (pulled Low) during a Soft Reset
		1B , no RST trigger during a Soft Reset
*/
		unsigned int FO_ON 			: 1; 
/*
		Failure Output Activation
		0B , FO not activated by software, FO is activated by specified
		failures (see Chapter 11.1.1)
		1B , FO activated by software (via SPI), only if configured as FO

*/
		unsigned int Reserved2 		: 2;
		unsigned int CP_EN				: 1;
/*
		Charge Pump Output Enable
		0B , Charge Pump is Off
		1B , Charge Pump Output is enabled (see Chapter 5)
*/
		unsigned int Reserved3			: 1;
		unsigned int CFG1				: 1;
/*
		Configuration Select 1 (see also Table 5)
		0B , Depending on hardware configuration, SBC Restart or Fail-
		Safe Mode is reached after the 2. watchdog trigger failure
		(=default) - Config 3/4
		1B , Depending on hardware configuration, SBC Restart or Fail-
		Safe Mode is reached after the 1. watchdog trigger failure -
		Config 1/2
*/
	} field;
} CANSBC_REG_DATA_0x02_HW_CTRL_0;

typedef union {
	uint8 data;
	struct {
		unsigned int CHECKSUM 			: 1; 
/*
		Watchdog Setting Check Sum Bit
		The sum of bits 7:0 needs to have even parity (see Chapter 12.2.3)
		0B , Counts as 0 for checksum calculation
		1B , Counts as 1 for checksum calculation
*/
		unsigned int WD_STM_EN_0		: 1;
/*
		Watchdog Deactivation during Stop Mode, bit 0
		(Chapter 12.2.4)
		0B , Watchdog is active in Stop Mode
		1B , Watchdog is deactivated in Stop Mode

*/
		unsigned int WD_WIN 			: 1; 
/*
		Watchdog Type Selection
		0B , Watchdog works as a Time-Out watchdog
		1B , Watchdog works as a Window watchdSog

*/
		unsigned int WD_EN_WK_BUS 		: 1;
/*
		Watchdog Enable after Bus (CAN) Wake-up in SBC Stop Mode
		0B , Watchdog will not start after a CAN wake-up
		1B , Watchdog starts with a long open window after CAN Wake
*/
		unsigned int Reserved			: 1;
		unsigned int WD_TIMER			: 3;
/*
		Watchdog Timer Period
		000B , 10ms
		001B , 20ms
		010B , 50ms
		011B , 100ms
		100B , 200ms
		101B , 500ms
		110B , 1000ms
		111B , 10000ms
*/
	} field;
} CANSBC_REG_DATA_0x03_WD_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 6; 
		unsigned int CAN				: 2;
/*
		HS-CAN Module Modes
		00B , CAN Off
		01B , CAN is Wake Capable
		10B , CAN Receive Only Mode
		11B , CAN Normal Mode
*/
	} field;
} CANSBC_REG_DATA_0x04_BUS_CTRL_0;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 1; 
		unsigned int TIMER_WK_EN		: 1;
/*
		Timer Wake Source Control (for Cyclic Wake)
		0B , Timer wake-up disabled
		1B , Timer is enabled as a wake source
*/
		unsigned int Reserved2			: 3;
		unsigned int WD_STM_EN_1		: 1;
/*
		Watchdog Deactivation during Stop Mode, bit 1
		(Chapter 12.2.4)
		0B , Watchdog is active in Stop Mode
		1B , Watchdog is deactivated in Stop Mode
*/
		unsigned int Reserved3			: 2;
	} field;
} CANSBC_REG_DATA_0x06_WK_CTRL_0;

typedef union {
	uint8 data;
	struct {
		unsigned int INT_GLOBAL 		: 1; 
/*
		Global Interrupt Configuration (see also Chapter 10.1)
		0B , Only wake sources trigger INTN (default)
		1B , All status information register bits will trigger INTN
		(including all wake sources)
*/
		unsigned int Reserved			: 1;
		unsigned int WK_MEAS			: 1;
/*
		Wake / Voltage Sensing Selection (see also Chapter 9.2.4)
		0B , Wake-up functionality enabled for WK
		1B , Voltage sensing functionality enabled, no wake-up events
		are generated
*/
		unsigned int Reserved2			: 4;
		unsigned int WK_EN				: 1;
/*
		WK Wake Source Control
		0B , WK wake-up disabled
		1B , WK is enabled as a wake source

*/	
	} field;
} CANSBC_REG_DATA_0x07_WK_CTRL_1;

typedef union {
	uint8 data;
	struct {
		unsigned int GPIO_WK_PUPD 		: 2; 
/*
		GPIO WK Pull-Up / Pull-Down Configuration (only if GPIO
		configured as WK)
		00B , No pull-up / pull-down selected
		01B , Pull-down resistor selected
		10B , Pull-up resistor selected
		11B , Automatic switching to pull-up or pull-down
*/
		unsigned int Reserved			: 4;
		unsigned int WK_PUPD			: 2;
/*
		WK Pull-Up / Pull-Down Configuration
		00B , No pull-up / pull-down selected
		01B , Pull-down resistor selected
		10B , Pull-up resistor selected
		11B , Automatic switching to pull-up or pull-down
*/
	} field;
} CANSBC_REG_DATA_0x08_WK_PUPD_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 3; 
		unsigned int CAN_Flash			: 1;
/*
		HS-CAN Flash Mode Activation
		0B , Flash Mode disabled: CAN communication up to 5MBaud
		1B , Flash Mode enabled: CAN communication for higher than
		5MBaud (higher emission on CAN bus - no slew rate control)
*/
		unsigned int Reserved2			: 4;
	} field;
} CANSBC_REG_DATA_0x0B_BUS_CTRL_3;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 1; 
		unsigned int TIMER_ON			: 3;
/*
		Timer On-Time Configuration
		000B , Off / Low (timer not running, HSx output is Low)
		001B , 0.1ms On-time
		010B , 0.3ms On-time
		011B , 1.0ms On-time
		100B , 10ms On-time
		101B , 20ms On-time
		110B , Off / High (timer not running, HSx output is High)
		111B , reserved
*/		
		unsigned int TIMER_PER			: 4;
/*
		Timer Period Configuration
		0000B, 10ms
		0001B, 20ms
		0010B, 50ms
		0011B, 100ms
		0100B, 200ms
		0101B, 500ms
		0110B, 1s
		0111B, 2s
		1000B, 5s
		1001B, 10s
		1010B, 20s
		1011B, 50s
		1100B, 100s
		1101B, 200s
		1110B, 500s
		1111B, 1000s
*/
	} field;
} CANSBC_REG_DATA_0x0C_TIMER_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int RSTN_HYS 			: 1;
/*
		VCC1 Undervoltage Reset Hysteresis Selection (see also
		Chapter 12.5.1 for more information)
		0B , default hysteresis applies as specified in the electrical
		characteristics table
		1B , the highest rising threshold (Vrt1,r) is always used for the
		release of the undervoltage reset

*/		
		unsigned int Reserved			: 1;
		unsigned int TSD2_DEL			: 1;
/*
		TSD2 Minimum Waiting Time Selection
		0B , Minimum waiting time until TSD2 is released again is always
		1s
		1B , Minimum waiting time until TSD2 is released again is 1s,
		after >16 consecutive TSD2 events, it is extended to x64
*/
		unsigned int RSTN_DEL			: 1;
/*
		Reset Delay Time Selection
		0B , The extended reset delay time tRD1is selected (default)
		1B , The reduced tRD2 reset delay time is selected
*/
		unsigned int CFG_LOCK_0		: 1;
/*
		Configuration Lock Bit - Level 0
		0B , CP_EN and GPIO can be modified
		1B , CP_EN and GPIO is locked and cannot be modified
*/
		unsigned int Reserved2			: 3;
	} field;
} CANSBC_REG_DATA_0x0E_HW_CTRL_1;

typedef union {
	uint8 data;
	struct {
		unsigned int _2MHZ_FREQ 		: 3; 
/*
		Charge Pump Switching Frequency Setting
		000B , 1.8MHz
		001B , 2.0MHz
		010B , 2.2MHz (default value)
		011B , 2.4MHz
		100B , Reserved
		101B , Reserved
		110B , Reserved
		111B , Reserved
*/
		unsigned int I_PEAK_TH			: 1;
/*
		VCC1 Active Peak Threshold Selection
		0B , low VCC1 active peak threshold selected (ICC1,peak_1)
		1B , high VCC1 active peak threshold selected (ICC1,peak_2).
*/
		unsigned int SS_MOD_FR			: 2;
/*
		Spread Spectrum Modulation Frequency Setting of integrated
		2MHz oscillator for charge pump
		00B , Spread Spectrum disabled
		01B , 15.625kHz Modulation Frequency
		10B , 31.250kHz Modulation Frequency
		11B , 62.500kHz Modulation Frequency
*/
		unsigned int Reserved			: 1;
		unsigned int CFG_LOCK_1		: 1;
/*
		Configuration Lock Bit - Level 1
		0B , Bits with bit type ¡®rwl¡¯ (except CP_EN and GPIO) can be
		modified
		1B , Bits with bit type ¡®rwl¡¯ (except CP_EN and GPIO) are locked
		and cannot be modified anymore until next device power-up.
*/
	} field;
} CANSBC_REG_DATA_0x0F_HW_CTRL_2;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 5; 
		unsigned int GPIO				: 3;
/*
		GPIO Configuration
		000B , FO selected (default)
		001B , FO selected
		010B , FO selected
		011B , High-Side controlled by TIMER (Cyclic Sense)
		100B , Off
		101B , Wake input enabled (16us static filter)
		110B , Low-Side Switch controlled by PWM
		111B , High-Side Switch controlled by PWM
*/
	} field;
} CANSBC_REG_DATA_0x17_GPIO_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int PWM_DC 			: 8; 
/*
		PWM Duty Cycle Setting (bit0 = LSB; bit7 = MSB)
		0000 0000B, 100% Off, i.e. HS/LS = Off
		xxxx xxxxB, On with duty cycle fraction of 255
		1111 1111B, 100% On, i.e. HS/LS always On
*/
	} field;
} CANSBC_REG_DATA_0x18_PWM_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 6; 
		unsigned int PWM_FREQ 			: 2; 
/*
		Spread Spectrum Modulation Frequency Setting
		00B , 100Hz configuration
		01B , 200Hz configuration
		10B , 325Hz configuration
		11B , 400Hz configuration
*/
	} field;
} CANSBC_REG_DATA_0x1C_PWM_FREQ_CTRL;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 			: 5; 
		unsigned int TSD_THR 			: 1; 
/*
		Thermal Shutdown Threshold (TSD1 & TSD2) Configuration
		0B , Default shutdown threshold selected
		1B , higher shutdown threshold selected
*/
		unsigned int ICC1_LIM_ADJ 		: 2; 
/*
		Configuration of ICC1 current limitation
		00B , 1 step down from default value (-25% of typ. default)
		01B , default value (typ. 1000mA)
		10B , 1 step up form default value (+20% of default), setting not
		recommended
		11B , 2 steps up from default value (+50% of default), setting not
		recommended
*/
	} field;
} CANSBC_REG_DATA_0x1D_HW_CTRL_3;

typedef union {
	uint8 data;
	struct {
		unsigned int SYS_STAT_L 		: 8; 
/*
		System Status Control Low Byte (bit0=LSB; bit7=MSB)
		Dedicated byte for system configuration, access only by
		microcontroller. Cleared after power up
*/
	} field;
} CANSBC_REG_DATA_0x1E_SYS_STATUS_CTRL_0;

typedef union {
	uint8 data;
	struct {
		unsigned int SYS_STAT_H 		: 8; 
/*
		System Status Control High Byte (bit8=LSB; bit15=MSB)
		Dedicated byte for system configuration, access only by
		microcontroller. Cleared after power up
*/
	} field;
} CANSBC_REG_DATA_0x1F_SYS_STATUS_CTRL_1;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 		: 1; 
		unsigned int VS_UV 		: 1; 
/*
		VS Undervoltage Detection (VS,UV)
		0B , No VS undervoltage detected
		1B , VS undervoltage detected (detection is only active when
		VCC1 is enabled - see also note below)
*/
		unsigned int VS_OV 		: 1; 
/*
		VS Overvoltage Detection (VS,UV)
		0B , No VS overvoltage detected
		1B , VS overvoltage detected (detection is only active when VCC1
		is enabled - see also note below)
*/
		unsigned int Reserved2 		: 3; 
		unsigned int VCC1_OV 		: 1; 
/*
		VCC1 Overvoltage Detection (VCC1,OV,r)
		0B , No VCC1 overvoltage warning
		1B , VCC1 overvoltage detected
*/
		unsigned int VCC1_WARN 	: 1;
/*
		VCC1 Undervoltage Prewarning (VPW,f)
		0B , No VCC1 undervoltage prewarning
		1B , VCC1 undervoltage prewarning detected
*/
	} field;
} CANSBC_REG_DATA_0x40_SUP_STAT_1;

typedef union {
	uint8 data;
	struct {
		unsigned int POR 			: 1; 
/*
		Power-On Reset Detection
		0B , No POR
		1B , POR occurred
*/
		unsigned int Reserved 		: 2; 
		unsigned int VCC2_OT 		: 1; 
/*
		VCC2 Over Temperature Detection
		0B , No over temperature
		1B , VCC2 over temperature detected
*/
		unsigned int VCC2_UV 		: 1;
/*
		VCC2 Under Voltage Detection (VCC2,UV,f)
		0B , No VCC2 Under voltage
		1B , VCC2 under voltage detected
*/
		unsigned int VCC1_SC 		: 1; 
/*
		VCC1 Short to GND Detection (<Vrtx for t>2ms after switch On)
		0B , No short
		1B , VCC1 short to GND detected
*/
		unsigned int Reserved2	 	: 1; 
		unsigned int VCC1_UV 		: 1;
/*
		VCC1 UV-Detection (due to Vrtx reset)
		0B , No VCC1_UV detection
		1B , VCC1 UV-Fail detected
*/
	} field;
} CANSBC_REG_DATA_0x41_SUP_STAT_0;


typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 		: 4; 
		unsigned int TSD2_SAFE 	: 1; 
/*
		TSD2 Thermal Shut-Down Safe State Detection
		0B , No TSD2 safe state detected
		1B , TSD2 safe state detected: >16 consecutive TSD2 events
		occurred, next TSD2 waiting time is 60s
*/
		unsigned int TSD2 			: 1; 
/*
		TSD2 Thermal Shut-Down Detection
		0B , No TSD2 event
		1B , TSD2 OT detected - leading to SBC Fail-Safe Mode
*/
		unsigned int TSD1 			: 1; 
/*
		TSD1 Thermal Shut-Down Detection
		0B , No TSD1 fail
		1B , TSD1 OT detected (affected module is disabled)
*/
		unsigned int TPW 			: 1; 
/*
		Thermal Pre Warning
		0B , No Thermal Pre warning
		1B , Thermal Pre warning detected
*/
	} field;
} CANSBC_REG_DATA_0x42_THERM_STAT;

typedef union {
	uint8 data;
	struct {
		
		unsigned int _DEV_STAT : 2;
/*
		Device Status before Restart Mode
		00B , Cleared (Register must be actively cleared)
		01B , Restart due to failure (WD fail, TSD2, VCC1_UV, trial to access
		SLEEP MODE without any wake source activated); also after a
		wake-up from Fail-Safe Mode
		10B , Sleep Mode
		11B , Reserved
*/
		unsigned int Reserved 			: 2; 
		unsigned int WD_FAIL 			: 2; 
/*
		Number of WD-Failure Events (1/2 WD failures depending on
		CFG1)
		00B , No WD Fail
		01B , 1x WD Fail, FO activation - Config 2 selected
		10B , 2x WD Fail, FO activation - Config 1 / 3 / 4 selected
		11B , Reserved (never reached)
*/
		unsigned int SPI_FAIL 			: 1;
/*
		SPI Fail Information
		0B , No SPI fail
		1B , Invalid SPI command detected
*/
		unsigned int FAILURE 			: 1; 
/*
		Activation of Fail Output FO
		0B , No Failure
		1B , Failure occurred
*/
	} field;
} CANSBC_REG_DATA_0x43_DEV_STAT;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 				: 1; 
		unsigned int Reserved2 			: 2; 
		unsigned int Reserved3 			: 2; 
		unsigned int CAN_FAIL 				: 2; 
/*
		CAN Failure Status
		00B , No error
		01B , CAN TSD
		10B , CAN_TXD_DOM: TXD dominant time out detected (P_9.3.39)
		11B , CAN_BUS_DOM: BUS dominant time out detected (P_9.3.40)
*/
		unsigned int VCAN_UV 					: 1; 
/*
		Under Voltage CAN Bus Supply
		0B , Normal operation
		1B , CAN Supply under voltage detected. Transmitter disabled
*/
	} field;
} CANSBC_REG_DATA_0x44_BUS_STAT;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 					: 2; 
		unsigned int CAN_WU 					: 1; 
/*
		Wake-up via CAN Bus
		0B , No Wake-up
		1B , Wake-up
*/
		unsigned int TIMER_WU 					: 1; 
/*
		Wake-up via TimerX
		0B , No Wake-up
		1B , Wake-up
*/
		unsigned int Reserved2 					: 3; 
		unsigned int WK_WU 					: 1;
/*
		Wake-up via WK
		0B , No Wake-up
		1B , Wake-up
*/
	} field;
} CANSBC_REG_DATA_0x46_WK_STAT_0;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 				: 3; 
		unsigned int GPIO_WK_WU			: 1; 
/*
		Wake-up via GPIO if configured as WK
		0B , No Wake-up
		1B , Wake-up
*/
		unsigned int Reserved2 			: 4; 
	} field;
} CANSBC_REG_DATA_0x47_WK_STAT_1;

typedef union {
	uint8 data;
	struct {
		unsigned int SBC_DEV_LVL 			: 1; 
/*
		Status of SBC Operating Mode at TEST Pin
		0B , User Mode activated
		1B , SBC Development Mode activated
*/
		unsigned int CFG0_STATE			: 1; 
/*
		Device Configuration Status on pin INTN
		0B , No external pull-up resistor connected on INTN (Config 2/4)
		1B , External pull-up resistor connected on INTN (Config 1/3)
*/
		unsigned int Reserved 				: 1; 
		unsigned int GPIO_LVL 				: 1; 
/*
		Status of GPIO if configured as GPIO (WK, LS or HS function)
		0B , Low Level (=0)
		1B , High Level (=1)
*/
		unsigned int Reserved2 			: 3; 
		unsigned int WK_LVL 				: 1; 
/*
	Status of WK
	0B , Low Level (=0)
	1B , High Level (=1)
*/	
	} field;
} CANSBC_REG_DATA_0x48_WK_LVL_STAT;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 				: 1; 
		unsigned int GPIO_OC				: 1; 
/*
		Overcurrent Detection on GPIO (if configured as LS or HS)
		0B , No OC
		1B , OC detected
*/
		unsigned int Reserved2 				: 6; 
	} field;
} CANSBC_REG_DATA_0x54_GPIO_OC_STAT;

typedef union {
	uint8 data;
	struct {
		unsigned int Reserved 				: 1; 
		unsigned int GPIO_OL				: 1; 
/*
		Open-Load Detection on GPIO (if configured as HS)
		0B , No OL
		1B , OL detected
*/
		unsigned int Reserved2 				: 6; 
	} field;
} CANSBC_REG_DATA_0x55_GPIO_OL_STAT;

typedef union {
	uint8 data;
	struct {
		unsigned int FAM 					: 4; 
/*
		SBC Family Identifier (bit4=LSB; bit7=MSB)
		0 0 01B, Driver SBC Family
		0 0 10B, DC/DC-SBC Family
		0 0 11B, Mid-Range SBC Family
		0 100B, Multi-CAN SBC Family
		0 101B, LITE SBC Family
		0 111B, Mid-Range+ SBC Family
		x x x xB, reserved for future products
*/
		unsigned int PROD					: 4;  
/*
		SBC Product Identifier (bit0=LSB; bit3=MSB)
		0 1 10B, TLE9461ES (VCC1 = 5V, no SWK) / TLE9461-3ES (VCC1 = 5V,
		SWK)
		0 1 1 1B, TLE9461ESV33 (VCC1 = 3.3V, no SWK) / TLE9461-3ESV33
		(VCC1 = 3.3V, SWK)
		1 1 1 0B, TLE9471ES (VCC1 = 5V, no SWK) /
		TLE9471-3ES(VCC1 = 5V, SWK)
		1 1 1 1B, TLE9471ESV33 (VCC1 = 3.3V, no SWK) /
		TLE9471-3ESV33 (VCC1 = 3.3V, SWK)
*/
	} field;
} CANSBC_REG_DATA_0x7E_FAM_PROD_STAT;

typedef struct {
	CANSBC_REG_DATA_0x01_M_S_CTRL _0x01;
	CANSBC_REG_DATA_0x02_HW_CTRL_0 _0x02;
	CANSBC_REG_DATA_0x03_WD_CTRL _0x03;
	CANSBC_REG_DATA_0x04_BUS_CTRL_0 _0x04;
	CANSBC_REG_DATA_0x06_WK_CTRL_0 _0x06;
	CANSBC_REG_DATA_0x07_WK_CTRL_1 _0x07;
	CANSBC_REG_DATA_0x08_WK_PUPD_CTRL _0x08;
	CANSBC_REG_DATA_0x0B_BUS_CTRL_3 _0x0B;
	CANSBC_REG_DATA_0x0C_TIMER_CTRL _0x0C;
	CANSBC_REG_DATA_0x0E_HW_CTRL_1 _0x0E;
	CANSBC_REG_DATA_0x0F_HW_CTRL_2 _0x0F;
	CANSBC_REG_DATA_0x17_GPIO_CTRL _0x17;
	CANSBC_REG_DATA_0x18_PWM_CTRL _0x18;
	CANSBC_REG_DATA_0x1C_PWM_FREQ_CTRL _0x1C;
	CANSBC_REG_DATA_0x1D_HW_CTRL_3 _0x1D;
	CANSBC_REG_DATA_0x1E_SYS_STATUS_CTRL_0 _0x1E;
	CANSBC_REG_DATA_0x1F_SYS_STATUS_CTRL_1 _0x1F;
	CANSBC_REG_DATA_0x40_SUP_STAT_1 _0x40;
	CANSBC_REG_DATA_0x41_SUP_STAT_0 _0x41;
	CANSBC_REG_DATA_0x42_THERM_STAT _0x42;
	CANSBC_REG_DATA_0x43_DEV_STAT _0x43;
	CANSBC_REG_DATA_0x44_BUS_STAT _0x44;
	CANSBC_REG_DATA_0x46_WK_STAT_0 _0x46;
	CANSBC_REG_DATA_0x47_WK_STAT_1 _0x47;
	CANSBC_REG_DATA_0x48_WK_LVL_STAT _0x48;
	CANSBC_REG_DATA_0x54_GPIO_OC_STAT _0x54;
	CANSBC_REG_DATA_0x55_GPIO_OL_STAT _0x55;
	CANSBC_REG_DATA_0x7E_FAM_PROD_STAT _0x7E;
} CANSBC_RegVar_t;

typedef struct {
	unsigned int Step							: 4;
	unsigned int CheckSum						: 1;
	unsigned int TestFlag						: 3;
	unsigned int VCC2_ON						: 1;
	unsigned int WD_STM_EN_0					: 1; 	
	unsigned int WD_STM_EN_1					: 1; 
	unsigned int fCANSBCDisableWatchdogStep		: 3;
	unsigned int fFaultClear					: 1;
	unsigned int CAN 							: 2;
	uint16 SpiTLE9461WDTime;
} CANSBC_RegWrite_t;

typedef union {
	uint8 data;
	struct {
	unsigned int ST7_GPIO_OL_STAT	: 1; 
	unsigned int ST6_GPIO_OC_STAT	: 1; 
	unsigned int ST5_SUP_STAT_1 	: 1; 
	unsigned int ST4_WK_STAT		: 1; 
	unsigned int ST3_BUS_STAT		: 1; 
	unsigned int ST2_DEV_STAT		: 1; 
	unsigned int ST1_THERM_STAT 	: 1; 
	unsigned int ST0_SUP_STAT_0 	: 1; 
	}field;
} CANSBC_SPI_Status;

typedef struct {
	CANSBC_SPI_Status SPI_Status;
	unsigned int VS_UV			: 1; 
	unsigned int VS_OV			: 1; 
	unsigned int VCC1_OV		: 1; 
	unsigned int VCC1_WARN		: 1; 
	unsigned int POR			: 1; 
	unsigned int VCC2_OT		: 1; 
	unsigned int VCC2_UV		: 1; 
	unsigned int VCC1_SC		: 1; 
	unsigned int VCC1_UV		: 1; 
	unsigned int TSD2_SAFE		: 1; 
	unsigned int TSD2			: 1; 
	unsigned int TSD1			: 1; 
	unsigned int TPW			: 1; 
	unsigned int DEV_STAT		: 1; 
	unsigned int WD_FAIL		: 1; 
	unsigned int SPI_FAIL		: 1; 
	unsigned int FAILURE		: 1; 
	unsigned int CAN_FAIL		: 1; 
	unsigned int VCAN_UV		: 1; 
	unsigned int GPIO_OC		: 1; 
	unsigned int GPIO_OL		: 1; 
	
} CANSBC_FaultStatus_t;


typedef struct {
	unsigned int CAN_WU			: 1; 
	unsigned int TIMER_WU		: 1; 
	unsigned int WK_WU			: 1; 
	unsigned int GPIO_WK_WU		: 1; 
	unsigned int SBC_DEV_LVL	: 1; 
	unsigned int CFG0_STATE		: 1; 
	unsigned int GPIO_LVL		: 1; 
	unsigned int WK_LVL			: 1; 									
} CANSBC_Status_t;

typedef struct {
	unsigned int fStart_Mutex			: 1; 
} CANSBC_t;

extern uint8_t fCANSBCModeNormal;
extern CANSBC_RegWrite_t *CANSBCRegInit(void);
extern CANSBC_FaultStatus_t *CANSBCFaultRegInit(void);
extern CANSBC_Status_t *CANSBCStatusRegInit(void);

extern void CANSBC_Communication(uint16 Mode, uint8 Address, uint8 Data);

uint16_t CANSBCSetMsg_0x01(uint8 Mode);
uint16_t CANSBCSetMsg_0x02(uint8 Mode);	
uint16_t CANSBCSetMsg_0x03(void);	
uint16_t CANSBCSetMsg_0x04(void);	
uint16_t CANSBCSetMsg_0x06(void);	
uint16_t CANSBCSetMsg_0x07(void);	
uint16_t CANSBCSetMsg_0x08(void);	
uint16_t CANSBCSetMsg_0x0B(void);	
uint16_t CANSBCSetMsg_0x0C(void);	
uint16_t CANSBCSetMsg_0x0E(void);	
uint16_t CANSBCSetMsg_0x0F(void);	

uint16_t CANSBCSetMsg_0x1D(void);	


uint16_t CANSBCSetMsg_0x41(void);	
uint16_t CANSBCSetMsg_0x46(void);

uint16_t CANSBCGetMsg(uint8 addr);
void CANSBC_ReadWrite(uint16_t RxData);
uint16_t CANSBCWatchdog(void);

void CANSBCInit(void);
extern void CANSBCDisableWatchdog(void);

extern void CheckCANSBCRegStatus(uint8 step);
extern void CANSBCCheckStatus(uint8 addr, uint16 data);
extern void CANSBCFaultClear(void);

extern void EpbCANSBCTest(void);
extern uint8 TLE9461Init(uint8 flag);

