#include "Typedefs.h"
#include "Common.h"

#define WRITE ((uint8)0x1U)
#define READ  ((uint8)0x0U)
#define CRC_CAL_SIZE 26
#define BIT0 			(0x01)
#define BIT1 			(0x02)
#define BIT2 			(0x04)
#define BIT3 			(0x08)
#define BIT4 			(0x10)
#define GIO1_MOD 		FALSE
#define GIO4_MOD 		FALSE
#define GIO6_MOD 		FALSE
#define GIO7_MOD 		FALSE
#define GIO8_MOD 		FALSE
#define SPI_L9369_BOOT	(0x0U)
#define SPI_L9369_INIT	(0x1U)
#define SPI_L9369_RUN	(0x2U)
#define SPI_L9369_DOWN	(0x3U)
#define L9369_DATA_SIZE	(4U)
#define L9369_APPLY		(0x01U)
#define L9369_RELEASE	(0x00U)
#define L9369_NEUTRAL	(0x00U)
#define DefaultMode  	(0x01U)
#define ExtendedMode 	(0x03U)
#define IgnitionON 		(0x01U)
#define IgnitionOFF 	(0x00U)
#define	Period			(0x7FU)


#define NUM_ADDRS			3U

#define TIME_3S					300U
#define TIME_5S					500U

#define TIME_1S					100U
#define TIME_1500MS				150U

#define TIME_500MS				50U

/* EPB Motor Type */
#if (MOTOR_TYPE == MOTOR_TYPE_INFAC) 
#define L9369_EPB_APPLY			(1U)
#define L9369_EPB_RELEASE		(0U)
#else
#define L9369_EPB_APPLY			(0U)
#define L9369_EPB_RELEASE		(1U)
#endif

/* EPB Button Type */
#define EPB_BUTTON_4			(0U)
#define EPB_BUTTON_6			(1U)
#define EPB_BUTTON_NOT			(2U)
#define EPB_BUTTON_TYPE			(EPB_BUTTON_NOT)


#define CS2_ENABLE				(1U) 
#define L9369_TEST				(0U)


#define MarurationTime_CC	(20U) //1Sec

#define SWITCH_APPLY 		  (0x02U)
#define SWITCH_RELEASE 		  (0x01U)
#define SWITCH_NEUTRAL 		  (0x00U)
#define SWITCH_FAULT 		  (0x03U)
#define SWITCH_READY		  (0x00U)
#define SWITCH_OPEN			  (0x03U)
#define CAN_APPLY 		  	  (0x01U)
#define CAN_RELEASE 		  (0x00U)
#define CAN_READY 		 	  (0x02U)
#define L9369_LEFT		(0x01U)
#define L9369_RIGHT		(0x00U)
#define GIO_HIGH_ACTIVE		(0U)
#define GIO_LOW_ACTIVE		(1U)
#define GIO2_MODE			GIO_LOW_ACTIVE /* AVH_SW : GIO_HIGH_ACTIVE */
#define GIO3_MODE			GIO_LOW_ACTIVE /* HDC_SW : GIO_HIGH_ACTIVE */
#define GIO7_MODE			GIO_LOW_ACTIVE /* ESC_OFF_SW : GIO_HIGH_ACTIVE */
#define HSOUT_HS_MODE		(0U)
#define HSOUT_LS_MODE		(1U)
#define HSOUT0_MODE			HSOUT_HS_MODE /* HSOUT_LS_MODE */
#define HSOUT1_MODE			HSOUT_HS_MODE /* HSOUT_HS_MODE */
#define MOTOR_CUR_CUT 			(10U)
#define L9369_ADC_COUNT				(7U)
#define L9369_ADC_SUM				(6U)
#define L9369_ADC_AVR				(5U)
#define L9369_ADC_PARKING_DETECT	(9.0F) /* 9A */
#define L9369_ADC_RELEASE_COUNT		(240U) /* 1200ms */
#define L9369_Rsense_Ohms			(2.0F)
#define L9369_Rsense_Ohms2			(3.0F)
typedef struct {
    uint32_t step;
    const uint32_t *addresses;
    uint32_t count;
} L9369StepConfig;

typedef struct {
	float VSBRIDGE_A[L9369_ADC_COUNT];
	float VSBRIDGE_B[L9369_ADC_COUNT];
	float SH1_A[L9369_ADC_COUNT];
	float SH1_B[L9369_ADC_COUNT];
	float SH2_A[L9369_ADC_COUNT];
	float SH2_B[L9369_ADC_COUNT];
	float CS1_A[L9369_ADC_COUNT];
	float CS1_B[L9369_ADC_COUNT];
	float CS2_A[L9369_ADC_COUNT];
	float CS2_B[L9369_ADC_COUNT];
	float tCS1_A;
	float tCS1_B;
} L9369_Adc_Value_t;



typedef union {
	uint8 data[L9369_DATA_SIZE];
	uint32_t data32;
	struct {
		unsigned int r_w	: 1;
		unsigned int addr 	: 8;
		unsigned int tm 	: 1;
		unsigned int cnt	: 1;
		unsigned int data 	: 16;
		unsigned int crc 	: 5;	
	} field;
} L9369_REG_MOSI;

typedef union {
	uint8 data[L9369_DATA_SIZE];
	uint32_t data32;
	struct {
		unsigned int spi_err: 1;
		unsigned int ierr 	: 2;
		unsigned int addr 	: 8;
		unsigned int data	: 16;
		unsigned int crc 	: 5;
	} field;
} L9369_REG_MISO;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int Init_busy_SPI 	: 1; /* Initialization test is running after leaving Sleep Mode to : 1 / CoR */	
		unsigned int Init_fail_SPI 	: 1; /* Initialized test failed during start-up phase : 0 / R ('1' = test failed) */	
		unsigned int SPI_RES_HW 	: 2; /* Software reset : 01 /R/W */
		unsigned int Revision_ID_SPI: 8; /* Revision ID : As programmed in the silicon / R */
		
	} field;
} L9369_REG_DATA_0x02;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int Device_ID_SPI	: 8; /* Fixed L9369 device ID : 01010111 / R */
		unsigned int SPICLK_fail_SPI: 1; /* SPI clock failure : 0 / CoR('1' = failure detected) */
		unsigned int CRC_fail_SPI	: 1; /* SPI CRC failure detected : 0 / CoR('1' = failure detected) */
		unsigned int ADR_fail_SPI	: 1; /* SPI address failure (WR/R) : 0 / CoR('1' = failure detected) */
		unsigned int ADW_fail_SPI	: 1; /* SPI address write failure : 0 / CoR('1' = failure detected in writing access), ('0' = failure detected in reading access) */

	} field;
} L9369_REG_DATA_0x03;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_A_result1_SPI : 12; /* 1st Measurement result for VSBRIDGE on stage A : undefined/ CoR */
	} field;
} L9369_REG_DATA_0x04;

typedef union {
	uint16 data;
	struct {
		
		
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_A_res1_ACT_SPI	: 1; /* H-bridge actuation status 1st measurement result for VSBRIDGE on stage A : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2					: 1; /* 1 */
		unsigned int VSBRIDGE_A_cnt1_SPI		: 6; /* ADC counter status for VSBRIDGE on stage A : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x05;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_A_result2_SPI : 12; /* 2nd Measurement result status for VSBRIDGE on stage A : undefined/ CoR */
	} field;
} L9369_REG_DATA_0x06;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_A_res2_ACT_SPI	: 1; /* H-bridge actuation status 2nd measurement result for VSBRIDGE on stage A : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_A_cnt2_SPI		: 6; /* ADC counter status for 2nd measurement result VSBRIDGE on stage A : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
					
	} field;
} L9369_REG_DATA_0x07;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_A_result3_SPI : 12; /* 3rd Measurement result status for VSBRIDGE on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x08;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_A_res3_ACT_SPI	: 1; /* H-bridge actuation status 3rd measurement result for VSBRIDGE on stage A : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_A_cnt3_SPI		: 6; /* ADC counter status for 3rd measurement result VSBRIDGE on stage A : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x09;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_A_result4_SPI : 12; /* 4th Measurement result status for VSBRIDGE on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x0A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_A_res4_ACT_SPI	: 1; /* H-bridge actuation status 4th measurement result for VSBRIDGE on stage A : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_A_cnt4_SPI		: 6; /* ADC counter status for 4th measurement result VSBRIDGE on stage A : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x0B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_A_result5_SPI : 12; /* 5th Measurement result status for VSBRIDGE on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x0C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_A_res5_ACT_SPI	: 1; /* H-bridge actuation status 5th measurement result for VSBRIDGE on stage A : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_A_cnt5_SPI		: 6; /* ADC counter status for 5th measurement result VSBRIDGE on stage A : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x0D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_B_result1_SPI : 12; /* 1st Measurement result status for VSBRIDGE on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x0E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_B_res1_ACT_SPI	: 1; /* H-bridge actuation status 1st measurement result for VSBRIDGE on stage B : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_B_cnt1_SPI		: 6; /* ADC counter status for 1st measurement result VSBRIDGE on stage B : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x0F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_B_result2_SPI : 12; /* 2nd Measurement result status for VSBRIDGE on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x10;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_B_res2_ACT_SPI	: 1; /* H-bridge actuation status 2nd measurement result for VSBRIDGE on stage B : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_B_cnt2_SPI		: 6; /* ADC counter status for 2nd measurement result VSBRIDGE on stage B : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x11;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_B_result3_SPI : 12; /* 3rd Measurement result status for VSBRIDGE on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x12;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_B_res3_ACT_SPI	: 1; /* H-bridge actuation status 3rd measurement result for VSBRIDGE on stage B : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_B_cnt3_SPI		: 6; /* ADC counter status for 3rd measurement result VSBRIDGE on stage B : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x13;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_B_result4_SPI : 12; /* 4th Measurement result status for VSBRIDGE on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x14;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_B_res4_ACT_SPI	: 1; /* H-bridge actuation status 4th measurement result for VSBRIDGE on stage B : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_B_cnt4_SPI		: 6; /* ADC counter status for 4th measurement result VSBRIDGE on stage B : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x15;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int VSBRIDGE_B_result5_SPI : 12; /* 5th Measurement result status for VSBRIDGE on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x16;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 					: 4; /* 0000 */
		unsigned int VSBRIDGE_B_res5_ACT_SPI	: 1; /* H-bridge actuation status 5th measurement result for VSBRIDGE on stage B : 0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 					: 1; /* 1 */
		unsigned int VSBRIDGE_B_cnt5_SPI		: 6; /* ADC counter status for 5th measurement result VSBRIDGE on stage B : 000000 /CoR */
		unsigned int Not_used1 					: 4; /* 1111 */
				

	} field;
} L9369_REG_DATA_0x17;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_A_result1_SPI	: 13; /* 1st Measurement result for SH1 on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x18;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_A_res1_ACT_SPI : 1; /* H-bridge actuation status for 1st Measurement result for SH1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_A_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result SH1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x19;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_A_result2_SPI	: 13; /* 2nd Measurement result for SH1 on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x1A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_A_res2_ACT_SPI : 1; /* H-bridge actuation status for 2nd Measurement result for SH1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_A_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result SH1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x1B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_A_result3_SPI	: 13; /* 3rd Measurement result for SH1 on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x1C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_A_res3_ACT_SPI : 1; /* H-bridge actuation status for 3rd Measurement result for SH1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
				
		unsigned int Not_used2			: 1; /* 1 */
		unsigned int SH1_A_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result SH1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
						



	} field;
} L9369_REG_DATA_0x1D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_A_result4_SPI	: 13; /* 4th Measurement result for SH1 on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x1E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_A_res4_ACT_SPI : 1; /* H-bridge actuation status for 4th Measurement result for SH1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_A_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result SH1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x1F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_A_result5_SPI	: 13; /* 5th Measurement result for SH1 on stage A : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x20;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_A_res5_ACT_SPI : 1; /* H-bridge actuation status for 5th Measurement result for SH1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_A_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result SH1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x21;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_B_result1_SPI	: 13; /* 1st Measurement result for SH1 on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x22;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_B_res1_ACT_SPI : 1; /* H-bridge actuation status for 1st Measurement result for SH1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_B_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result SH1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x23;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_B_result2_SPI	: 13; /* 2nd Measurement result for SH1 on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x24;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_B_res2_ACT_SPI : 1; /* H-bridge actuation status for 2nd Measurement result for SH1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_B_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result SH1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x25;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_B_result3_SPI	: 13; /* 3rd Measurement result for SH1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x26;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_B_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for SH1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
			
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_B_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result SH1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
			
	} field;
} L9369_REG_DATA_0x27;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_B_result4_SPI	: 13; /* 4th Measurement result for SH1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x28;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_B_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for SH1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_B_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result SH1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x29;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH1_B_result5_SPI	: 13; /* 5th Measurement result for SH1 on stage B : undefined/ CoR */
				

	} field;
} L9369_REG_DATA_0x2A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH1_B_res5_ACT_SPI : 1; /* H-bridge actuation status for 5th Measurement result for SH1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH1_B_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result SH1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x2B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_A_result1_SPI	: 13; /* 1st Measurement result for SH2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x2C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_A_res1_ACT_SPI	: 1; /* H-bridge actuation status for 1st Measurement result for SH2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_A_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result SH2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x2D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_A_result2_SPI	: 13; /* 2nd Measurement result for SH2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x2E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_A_res2_ACT_SPI	: 1; /* H-bridge actuation status for 2nd Measurement result for SH2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_A_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result SH2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x2F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_A_result3_SPI	: 13; /* 3rd Measurement result for SH2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x30;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_A_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for SH2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_A_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result SH2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x31;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_A_result4_SPI	: 13; /* 4th Measurement result for SH2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x32;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_A_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for SH2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_A_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result SH2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x33;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_A_result5_SPI	: 13; /* 5th Measurement result for SH2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x34;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_A_res5_ACT_SPI	: 1; /* H-bridge actuation status for 5th Measurement result for SH2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_A_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result SH2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x35;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_B_result1_SPI	: 13; /* 1st Measurement result for SH2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x36;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_B_res1_ACT_SPI : 1; /* H-bridge actuation status for 1st Measurement result for SH2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_B_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result SH2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
				

	} field;
} L9369_REG_DATA_0x37;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_B_result2_SPI	: 13; /* 2nd Measurement result for SH2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x38;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_B_res2_ACT_SPI	: 1; /* H-bridge actuation status for 2nd Measurement result for SH2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_B_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result SH2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x39;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_B_result3_SPI	: 13; /* 3rd Measurement result for SH2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x3A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_B_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for SH2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_B_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result SH2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x3B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_B_result4_SPI	: 13; /* 4th Measurement result for SH2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x3C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_B_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for SH2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_B_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result SH2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x3D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int SH2_B_result5_SPI	: 13; /* 5th Measurement result for SH2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x3E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_B_res5_ACT_SPI	: 1; /* H-bridge actuation status for 5th Measurement result for SH2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int SH2_B_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result SH2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x3F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_A_result1_SPI	: 13; /* 1st Measurement result for CSA1 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x40;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_A_res1_ACT_SPI	: 1; /* H-bridge actuation status for 1st Measurement result for CSA1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_A_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result CSA1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x41;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_A_result2_SPI	: 13; /* 2nd Measurement result for CSA1 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x42;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_A_res2_ACT_SPI	: 1; /* H-bridge actuation status for 2nd Measurement result for CSA1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_A_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result CSA1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x43;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_A_result3_SPI	: 13; /* 3rd Measurement result for CSA1 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x44;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_A_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for CSA1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_A_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result CSA1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x45;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_A_result4_SPI	: 13; /* 4th Measurement result for CSA1 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x46;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_A_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for CSA1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_A_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result CSA1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x47;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_A_result5_SPI	: 13; /* 5th Measurement result for CSA1 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x48;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_A_res5_ACT_SPI	: 1; /* H-bridge actuation status for 5th Measurement result for CSA1 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_A_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result CSA1 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x49;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_B_result1_SPI	: 13; /* 1st Measurement result for CSA1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x4A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_B_res1_ACT_SPI	: 1; /* H-bridge actuation status for 1st Measurement result for CSA1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_B_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result CSA1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x4B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_B_result2_SPI	: 13; /* 2nd Measurement result for CSA1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x4C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_B_res2_ACT_SPI	: 1; /* H-bridge actuation status for 2nd Measurement result for CSA1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_B_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result CSA1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x4D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_B_result3_SPI	: 13; /* 3rd Measurement result for CSA1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x4E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_B_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for CSA1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_B_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result CSA1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x4F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_B_result4_SPI	: 13; /* 4th Measurement result for CSA1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x50;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_B_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for CSA1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_B_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result CSA1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x51;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS1_B_result5_SPI	: 13; /* 5th Measurement result for CSA1 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x52;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS1_B_res5_ACT_SPI	: 1; /* H-bridge actuation status for 5th Measurement result for CSA1 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS1_B_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result CSA1 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x53;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_A_result1_SPI	: 13; /* 1st Measurement result for CSA2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x54;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_A_res1_ACT_SPI	: 1; /* H-bridge actuation status for 1st Measurement result for CSA2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_A_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result CSA2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x55;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_A_result2_SPI	: 13; /* 2nd Measurement result for CSA2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x56;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_A_res2_ACT_SPI	: 1; /* H-bridge actuation status for 2nd Measurement result for CSA2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_A_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result CSA2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x57;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_A_result3_SPI	: 13; /* 3rd Measurement result for CSA2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x58;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_A_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for CSA2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_A_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result CSA2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x59;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_A_result4_SPI	: 13; /* 4th Measurement result for CSA2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x5A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_A_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for CSA2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_A_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result CSA2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x5B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_A_result5_SPI	: 13; /* 5th Measurement result for CSA2 on stage A : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x5C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_A_res5_ACT_SPI	: 1; /* H-bridge actuation status for 5th Measurement result for CSA2 on stage A :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_A_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result CSA2 on stage A : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x5D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_B_result1_SPI	: 13; /* 1st Measurement result for CSA2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x5E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_B_res1_ACT_SPI	: 1; /* H-bridge actuation status for 1st Measurement result for CSA2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_B_cnt1_SPI		: 6; /* ADC counter status for 1st for 1st Measurement result CSA2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x5F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_B_result2_SPI	: 13; /* 2nd Measurement result for CSA2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x60;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int SH2_B_res2_ACT_SPI	: 1; /* H-bridge actuation status for 2nd Measurement result for CSA2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_B_cnt2_SPI		: 6; /* ADC counter status for 2nd for 1st Measurement result CSA2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x61;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_B_result3_SPI	: 13; /* 3rd Measurement result for CSA2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x62;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_B_res3_ACT_SPI	: 1; /* H-bridge actuation status for 3rd Measurement result for CSA2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_B_cnt3_SPI		: 6; /* ADC counter status for 3rd for 1st Measurement result CSA2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x63;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_B_result4_SPI	: 13; /* 4th Measurement result for CSA2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x64;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_B_res4_ACT_SPI	: 1; /* H-bridge actuation status for 4th Measurement result for CSA2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_B_cnt4_SPI		: 6; /* ADC counter status for 4th for 1st Measurement result CSA2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x65;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 3; /* 000 */
		unsigned int CS2_B_result5_SPI	: 13; /* 5th Measurement result for CSA2 on stage B : undefined/ CoR */
		
	} field;
} L9369_REG_DATA_0x66;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used3 			: 4; /* 0000 */
		unsigned int CS2_B_res5_ACT_SPI	: 1; /* H-bridge actuation status for 5th Measurement result for CSA2 on stage B :  0 /CoR('0' = voltage acquisition when H-bridge is turned-off), ('1' = voltage acquisition when H-bridge is turned on) */
		unsigned int Not_used2 			: 1; /* 1 */
		unsigned int CS2_B_cnt5_SPI		: 6; /* ADC counter status for 5th for 1st Measurement result CSA2 on stage B : 000000 /CoR */
		unsigned int Not_used1 			: 4; /* 1110 */
		
	} field;
} L9369_REG_DATA_0x67;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_DS_B_res	: 1; /* DS monitoring reset for Stage B : 0 / R/W('1' = reset diagnostics failure) */
		unsigned int SPI_DS_A_res	: 1; /* DS monitoring reset for Stage A : 0 / R/W('1' = reset diagnostics failure) */
		unsigned int reserved3 		: 4; /* 0011 */
		unsigned int SPI_OSCtest	: 1; /* Trigger command oscillator test : 0/ R/W */
		unsigned int reserved2 		: 2; /* 01 */
		unsigned int reserved1 		: 3; /* 010 */
		
		
	} field;
} L9369_REG_DATA_0x68;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_ACT_OFF_HS2_B	: 2; /* Redundant safety switch-off command for HS stage2 stage B : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
		unsigned int SPI_ACT_OFF_HS1_B	: 2; /* Redundant safety switch-off command for HS1 stage B : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
		unsigned int SPI_ACT_OFF_HS2_A	: 2; /* Redundant safety switch-off command for HS2 stage A : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
		unsigned int SPI_ACT_OFF_HS1_A	: 2; /* Redundant safety switch-off command for HS1 stage A : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
		unsigned int SPI_res_WD 		: 2; /* Integrated WD reset command : 01 / R/W(01 default, 10 reset) */
		unsigned int SPI_set_LPF 		: 1; /* Low Pass Filter Cut off Freq configuration (155Hz/465Hz) : 0 */
		unsigned int SPI_OC_A_res 		: 1; /* Overcurrent Reset for stage A : 0 */
		
	} field;
} L9369_REG_DATA_0x69;

typedef union {
	uint16 data;
	struct {

		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int SPI_set_WDfail_limit	: 2; /* WD fail counter setting : 11 */
		unsigned int SPI_set_PWM			: 2; /* Select the PWM frequency for H-bridge : 00 / R/W */
		/* SPI_set_PWM  H-bridge PWM frequency(Hz)
			00(default) 610
			01          1220
			10          4880
			11          19520 
		*/
		unsigned int SPI_PWM_A				: 7; /* PWM mode enabled on H-bridge A : 1111111 / R/W(please review pre-driver stage for detailed configuration table) */
			/* PWM Duty 
				 0 : 0.00,	16 : 12.60, 32 : 25.20, 48 : 37.80, 64 : 50.39, 80 : 62.99,  96 : 75.59, 112 : 88.19
				 1 : 0.79,	17 : 13.39, 33 : 25.98, 49 : 38.58, 65 : 51.18, 81 : 63.78,  97 : 76.38, 113 : 88.98
				 2 : 1.57,	18 : 14.17, 34 : 26.77, 50 : 39.37, 66 : 51.97, 82 : 64.57,  98 : 77.17, 114 : 89.76
				 3 : 2.36,	19 : 14.96, 35 : 27.56, 51 : 40.16, 67 : 52.76, 83 : 65.35,  99 : 77.95, 115 : 90.55
				 4 : 3.15,	20 : 15.75, 36 : 28.35, 52 : 40.94, 68 : 53.54, 84 : 66.14, 100 : 78.74, 116 : 91.34
				 5 : 3.94,	21 : 16.54, 37 : 29.13, 53 : 41.73, 69 : 54.33, 85 : 66.93, 101 : 79.53, 117 : 92.13
				 6 : 4.72,	22 : 17.32, 38 : 29.92, 54 : 42.52, 70 : 55.12, 86 : 67.72, 102 : 80.31, 118 : 92.91
				 7 : 5.51,	23 : 18.11, 39 : 30.71, 55 : 43.31, 71 : 55.91, 87 : 68.50, 103 : 81.10, 119 : 93.70
				 8 : 6.30,	24 : 18.90, 40 : 31.50, 56 : 44.09, 72 : 56.69, 88 : 69.29, 104 : 81.89, 120 : 94.49
				 9 : 7.09,	25 : 19.69, 41 : 32.28, 57 : 44.88, 73 : 57.48, 89 : 70.08, 105 : 82.68, 121 : 95.28
				10 : 7.87,	26 : 20.47, 42 : 33.07, 58 : 45.67, 74 : 58.27, 90 : 70.87, 106 : 83.46, 122 : 96.06
				11 : 8.66,	27 : 21.26, 43 : 33.86, 59 : 46.46, 75 : 59.06, 91 : 71.65, 107 : 84.25, 123 : 96.85
				12 : 9.45,	28 : 22.05, 44 : 34.65, 60 : 47.24, 76 : 59.84, 92 : 72.44, 108 : 85.04, 124 : 97.64
				13 : 10.24, 29 : 22.83, 45 : 35.43, 61 : 48.03, 77 : 60.63, 93 : 73.23, 109 : 85.83, 125 : 98.43
				14 : 11.02, 30 : 23.62, 46 : 36.22, 62 : 48.82, 78 : 61.42, 94 : 74.02, 110 : 86.61, 126 : 99.21
				15 : 11.81, 31 : 24.41, 47 : 37.01, 63 : 49.61, 79 : 62.20, 95 : 74.80, 111 : 87.40, 127 : 100.00
			*/

		unsigned int SPI_on_RVPG			: 1; /* RVPG gate driver enabled : 0 / R/W('1'= RVPG enabled) */

	} field;
} L9369_REG_DATA_0x6A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_on_HS1_B	: 1; /* Enable for HS1 gate pre-driver, Stage B : 0 / R/W('1' = HS1 pre-driver stage B turned on) */
		unsigned int SPI_on_HS2_B	: 1; /* Enable for HS2 gate pre-driver, Stage B : 0 / R/W('1' = HS2 pre-driver stage B turned on) */
		unsigned int SPI_on_HS1_A	: 1; /* Enable for HS1 gate pre-driver, Stage A : 0 / R/W('1' = HS1 pre-driver stage A turned on) */
		unsigned int SPI_on_HS2_A	: 1; /* Enable for HS2 gate pre-driver, Stage A : 0 / R/W('1' = HS2 pre-driver stage A turned on) */
		unsigned int SPI_PWM_B		: 7; /* PWM mode enabled on H-bridge B : 1111111 / R/W(please review pre-driver stage for detailed configuration table) */
		/* PWM Duty 
			 0 : 0.00,	16 : 12.60, 32 : 25.20, 48 : 37.80, 64 : 50.39, 80 : 62.99,  96 : 75.59, 112 : 88.19
			 1 : 0.79,	17 : 13.39, 33 : 25.98, 49 : 38.58, 65 : 51.18, 81 : 63.78,  97 : 76.38, 113 : 88.98
			 2 : 1.57,	18 : 14.17, 34 : 26.77, 50 : 39.37, 66 : 51.97, 82 : 64.57,  98 : 77.17, 114 : 89.76
			 3 : 2.36,	19 : 14.96, 35 : 27.56, 51 : 40.16, 67 : 52.76, 83 : 65.35,  99 : 77.95, 115 : 90.55
			 4 : 3.15,	20 : 15.75, 36 : 28.35, 52 : 40.94, 68 : 53.54, 84 : 66.14, 100 : 78.74, 116 : 91.34
			 5 : 3.94,	21 : 16.54, 37 : 29.13, 53 : 41.73, 69 : 54.33, 85 : 66.93, 101 : 79.53, 117 : 92.13
			 6 : 4.72,	22 : 17.32, 38 : 29.92, 54 : 42.52, 70 : 55.12, 86 : 67.72, 102 : 80.31, 118 : 92.91
			 7 : 5.51,	23 : 18.11, 39 : 30.71, 55 : 43.31, 71 : 55.91, 87 : 68.50, 103 : 81.10, 119 : 93.70
			 8 : 6.30,	24 : 18.90, 40 : 31.50, 56 : 44.09, 72 : 56.69, 88 : 69.29, 104 : 81.89, 120 : 94.49
			 9 : 7.09,	25 : 19.69, 41 : 32.28, 57 : 44.88, 73 : 57.48, 89 : 70.08, 105 : 82.68, 121 : 95.28
			10 : 7.87,	26 : 20.47, 42 : 33.07, 58 : 45.67, 74 : 58.27, 90 : 70.87, 106 : 83.46, 122 : 96.06
			11 : 8.66,	27 : 21.26, 43 : 33.86, 59 : 46.46, 75 : 59.06, 91 : 71.65, 107 : 84.25, 123 : 96.85
			12 : 9.45,	28 : 22.05, 44 : 34.65, 60 : 47.24, 76 : 59.84, 92 : 72.44, 108 : 85.04, 124 : 97.64
			13 : 10.24, 29 : 22.83, 45 : 35.43, 61 : 48.03, 77 : 60.63, 93 : 73.23, 109 : 85.83, 125 : 98.43
			14 : 11.02, 30 : 23.62, 46 : 36.22, 62 : 48.82, 78 : 61.42, 94 : 74.02, 110 : 86.61, 126 : 99.21
			15 : 11.81, 31 : 24.41, 47 : 37.01, 63 : 49.61, 79 : 62.20, 95 : 74.80, 111 : 87.40, 127 : 100.00
		*/
		unsigned int SPI_RefA_on	: 1; /* Reference Voltage enable on H-bridge A : 1 / R/W('0' = voltage reference disabled) */
		} field;
} L9369_REG_DATA_0x6B;

typedef union {
	uint16 data;
	struct {

		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_MTPth		: 3; /* Configuration of Motor test Threshold : 011 / R/W(please refer to pre-driver stage for detailed configuration table) */
		/* VSBRIDGE_x = 13.5 V
			SPI_MTPth OVC threshold(3 m目 shunt) OVC threshold(2 m目 shunt)
			000 		 ▽8.41 				 ▽12.61
			001 		 ▽9.48 				 ▽14.21
			010 		 ▽10.54				 ▽15.82
			011(default) ▽11.61				 ▽17.42
			100 		 ▽12.68				 ▽19.02
			101 		 ▽13.75				 ▽20.62
			110 		 ▽14.81				 ▽22.22
			111 		 ▽15.88				 ▽23.82
		*/
		unsigned int SPI_RefB_on	: 1; /* H-bridge stage B reference voltage enabled : 1 / R/W('0' = voltage reference disabled) */
		unsigned int SPI_DSthHS		: 4; /* DS monitoring thresholds for HS stage (A,B) : 1111 / R/W(please refer to pre-driver stage for detailed configuration table) */
		/* 0000 : 0.75, 0001 : 0.90, 0010 : 1.05, 0011 : 1.20, 0100 : 1.35, 0101 : 1.50, 0110 : 1.65, 0111 : 1.80, 1000 : 0.3, 1001 : 0.45, 1010 : 0.6, 1011 : 0.75, 1100 : 0.90, 1101 : 1.05, 1110 : 1.20, 1111(default) : 1.35 */
		unsigned int SPI_DSthLS		: 4; /* DS monitoring thresholds for LS stage (A,B) : 1111 / R/W(please refer to pre-driver stage for detailed configuration table) */
		/* 0000 : 0.765, 0001 : 0.915, 0010 : 1.065, 0011 : 1.215, 0100 : 1.365, 0101 : 1.515, 0110 : 1.665, 0111 : 1.815, 1000 : 0.315, 1001 : 0.465, 1010 : 0.615, 1011 : 0.765, 1100 : 0.915, 1101 : 1.065, 1110 : 1.215, 1111(default) : 1.365 */
	} field;
} L9369_REG_DATA_0x6C;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_on_T2_B	: 1; /* T2 NFET enable in MSS stage B : 0 /R/W('1' = T2 FET stage B enabled) */
		unsigned int SPI_on_T2_A	: 1; /* T2 NFET enable in MSS stage A : 0 /R/W('1' = T2 FET stage A enabled) */
		unsigned int SPI_on_T1_B	: 1; /* T1 NFET enable in MSS stage B : 0 /R/W('1' = T1 FET stage B enabled) */
		unsigned int SPI_on_T1_A	: 1; /* T1 NFET enable in MSS stage A : 0 /R/W('1' = T1 FET stage A enabled) */
		unsigned int SPI_test_CSA1_A: 1; /* Test enable for CSA1 stage A : 0 / R/W('1' = test enabled on CSA1 stage A) */
		unsigned int SPI_test_CSA2_A: 1; /* Test enable for CSA2 stage A : 0 / R/W('1' = test enabled on CSA2 stage A) */
		unsigned int SPI_test_CSA1_B: 1; /* Test enable for CSA1 stage B : 0 / R/W('1' = test enabled on CSA1 stage B) */
		unsigned int SPI_test_CSA2_B: 1; /* Test enable for CSA2 stage B : 0 / R/W('1' = test enabled on CSA2 stage B) */

		unsigned int SPI_set_WDTime : 2; /* Watchdog : 01 / R/W */
		unsigned int SPI_MTPtime	: 2; /* Motor test time configuration : 01 / R/W */

	} field;
} L9369_REG_DATA_0x6D;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_on_LS1_B	: 1; /* LS1 turn-on command for H-bridge stage B : 0 / R/W('1' = LS1 stage B turned on) */
		unsigned int SPI_on_LS2_B	: 1; /* LS2 turn-on command for H-bridge stage B : 0 / R/W('1' = LS2 stage B turned on) */
		unsigned int SPI_on_LS1_A	: 1; /* LS1 turn-on command for H-bridge stage A : 0 / R/W('1' = LS1 stage A turned on) */
		unsigned int SPI_on_LS2_A	: 1; /* LS2 turn-on command for H-bridge stage A : 0 / R/W('1' = LS2 stage A turned on) */
		unsigned int SPI_OCth_B		: 4; /* OVC thresholds configuration for Hbridge stage B : 1010 / R/W */
		/* SPI_OCth_X Overcurrent Threshold [A] Overcurrent Threshold [A]
			(x=A,B)      (3 m目 shunt)                 (2 m目 shunt)
			0000          ▽ 36.17                      ▽ 54. 25
			0001          ▽ 38.30                      ▽ 57.46
			0010          ▽ 40.44                      ▽ 60.66
			0011          ▽ 42.57                      ▽ 63.86
			0100          ▽ 44.71                      ▽ 67.07
			0101          ▽ 46.85                      ▽ 70.27
			0110          ▽ 48.98                      ▽ 73.47
			0111          ▽ 51.12                      ▽ 76.67
			1000          ▽ 53.25                      ▽ 79.88
			1001          ▽ 55.39                      ▽ 83.08
			1010(default) ▽ 57.52                      ▽ 86.28
			1011          ▽ 59.66                      ▽ 89.49
			1100          ▽ 61.75                      ▽ 92.69
			1101          ▽ 63.93                      ▽ 95.89
			1110          ▽ 66.06                      ▽ 99.10
			1111          ▽ 68.20                      ▽ 102.30
		*/
		unsigned int SPI_OCth_A		: 4; /* OVC thresholds configuration for Hbridge stage A : 1010 / R/W */
		} field;
} L9369_REG_DATA_0x6E;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_com_WD			: 4; /* WD command : 0000 / R/W */
		unsigned int SPI_MSSon_B		: 1; /* Enable command bit for MSS interface stage B : 0 / R/W(1 enabled bit) */
		unsigned int SPI_MSSon_A		: 1; /* Enable command bit for MSS interface stage A : 0 / R/W(1 enabled bit) */
		unsigned int SPI_on_HSOUT1		: 1; /* HSOUT1 lamp driver enable command bit : 0 / R/W(1 enabled) */
		unsigned int SPI_on_HSOUT0		: 1; /* HSOUT0 lamp driver enable command bit : 0 / R/W(1 enabled) */
		unsigned int SPI_OLTest_HSOUT1	: 1; /* OL test enable for lamp driver HSOUT1 : 0 / R/W('1' = Open-load test enabled on HSOUT1) (to be set to '0' by app sw when test is finished) */
		unsigned int SPI_OLTest_HSOUT0	: 1; /* OL test enable for lamp driver HSOUT0 : 0 / R/W('1' = Open-load test enabled on HSOUT0) (to be set to '0' by app sw when test is finished) */
		unsigned int SPI_HSoff_high		: 1; /* Low ohmic path switch off disable bit for HS pre-driver stage A and B : 0 / R/W (default)('1' = low ohmic path enabled) */
		unsigned int SPI_LSoff_high		: 1; /* Low ohmic path switch off disable bit for LS pre-driver stage A and B : 0 / R/W (default)('1' = low ohmic path enabled) */
	} field;
} L9369_REG_DATA_0x6F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_voltage_HSOUT0: 12; /* Programmable Lamp driver output voltage in PWM mode for HSOUT0 : 000000000000 / R/W(please refer to lamp driver chapter for detailed configuration table) */
	} field;
} L9369_REG_DATA_0x70;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_voltage_HSOUT1: 12; /* Programmable Lamp driver output voltage in PWM mode for HSOUT1 : 000000000000 / R/W(please refer to lamp driver chapter for detailed configuration table) */
	} field;
} L9369_REG_DATA_0x71;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_GIO3_MuxCtrl	: 4; /* Mux Control command bit for GIO3 : 1111 / R/W('1111'= GIO3 not configured as digital input controlled output) NAWD '1001' and '1110' */
		/* SPI_GIOx_MuxCtrl
			0000 : GIO0
			0001 : GIO1
			0010 : GIO2
			0011 : GIO3
			0100 : GIO4
			0101 : GIO5
			0110 : GIO6
			0111 : GIO7
			1000 : GIO8
			1111 : no
			1001 to 1110 : Discarded by the logic
		*/
		unsigned int SPI_GIO4_MuxCtrl	: 4; /* Mux Control command bit for GIO4 : 1111 / R/W('1111'= GIO4 not configured as digital input controlled output) NAWD '1001' and '1110' */
		unsigned int SPI_GIO5_MuxCtrl	: 4; /* Mux Control command bit for GIO5 : 1111 / R/W('1111'= GIO5 not configured as digital input controlled output) NAWD '1001' and '1110' */
		} field;
} L9369_REG_DATA_0x72;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_GIO0_MuxCtrl	: 4; /* Mux Control command bit for GIO0 : 1111 / R/W('1111'= GIO0 not configured as digital input controlled output) NAWD '1001' and '1110' */
		/* SPI_GIOx_MuxCtrl
			0000 : GIO0
			0001 : GIO1
			0010 : GIO2
			0011 : GIO3
			0100 : GIO4
			0101 : GIO5
			0110 : GIO6
			0111 : GIO7
			1000 : GIO8
			1111 : no
			1001 to 1110 : Discarded by the logic
		*/
		unsigned int SPI_GIO1_MuxCtrl	: 4; /* Mux Control command bit for GIO1 : 1111 / R/W('1111'= GIO1 not configured as digital input controlled output) NAWD '1001' and '1110' */
		unsigned int SPI_GIO2_MuxCtrl	: 4; /* Mux Control command bit for GIO2 : 1111 / R/W('1111'= GIO2 not configured as digital input controlled output) NAWD '1001' and '1110' */
		} field;
} L9369_REG_DATA_0x73;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_GPIO0_MuxCtrl	: 3; /* Mux Control command bit for GPIO0 : 111 / R/W('111'= GPIO0 not configured as digital input controlled output) NAWD '100','101','110' */
		/* SPI_GPIOx_MuxCtrl
			000 : GPIO0
			001 : GPIO1
			010 : GPIO2
			011 : GPIO3
			111 : No connection, All others are not allowed
		*/	
		unsigned int SPI_GPIO1_MuxCtrl	: 3; /* Mux Control command bit for GPIO1 : 111 / R/W('111'= GPIO1 not configured as digital input controlled output) NAWD '100','101','110' */
		unsigned int SPI_GPIO2_MuxCtrl	: 3; /* Mux Control command bit for GPIO2 : 111 / R/W('111'= GPIO2 not configured as digital input controlled output) NAWD '100','101','110' */
		unsigned int SPI_GPIO3_MuxCtrl	: 3; /* Mux Control command bit for GPIO3 : 111 / R/W('111'= GPIO3 not configured as digital input controlled output) NAWD '100','101','110' */
		} field;
} L9369_REG_DATA_0x74;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_GIO6_MuxCtrl	: 4; /* Mux Control command bit for GIO6 stage : 1111 / R/W('1111'= GIO6 not configured as digital input controlled output) NAWD '1001' and '1110' */
		/* SPI_GIOx_MuxCtrl
			0000 : GIO0
			0001 : GIO1
			0010 : GIO2
			0011 : GIO3
			0100 : GIO4
			0101 : GIO5
			0110 : GIO6
			0111 : GIO7
			1000 : GIO8
			1111 : no
			1001 to 1110 : Discarded by the logic
		*/
		unsigned int SPI_GIO7_MuxCtrl	: 4; /* Mux Control command bit for GIO7 stage : 1111 / R/W('1111'= GIO7 not configured as digital input controlled output) NAWD '1001' and '1110' */
		unsigned int SPI_GIO8_MuxCtrl	: 4; /* Mux Control command bit for GIO8 stage : 1111 / R/W('1111'= GIO8 not configured as digital input controlled output) NAWD '1001' and '1110' */
		} field;
} L9369_REG_DATA_0x75;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int SPI_Oltest_GPIO0		: 1; /* Open-load test Enable for GPIO0 stage : 0 / R/W('1' = open-load test enabled on GPIO0 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
		unsigned int SPI_Oltest_GPIO1		: 1; /* Open-load test Enable for GPIO1 stage : 0 / R/W('1' = open-load test enabled on GPIO1 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
		unsigned int SPI_Oltest_GPIO2		: 1; /* Open-load test Enable for GPIO2 stage : 0 / R/W('1' = open-load test enabled on GPIO2 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
		unsigned int SPI_Oltest_GPIO3		: 1; /* Open-load test Enable for GPIO3 stage : 0 / R/W('1' = open-load test enabled on GPIO3 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
		unsigned int SPI_GIO3_Wake_HSctrl	: 1; /* Configuration command for GIO3 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
		unsigned int SPI_GIO2_Wake_HSctrl	: 1; /* Configuration command for GIO2 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
		unsigned int SPI_GIO1_Wake_HSctrl	: 1; /* Configuration command for GIO1 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
		unsigned int SPI_GIO0_Wake_HSctrl	: 1; /* Configuration command for GIO0 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
		unsigned int SPI_GIO5_Wake_LSctrl	: 1; /* Configuration mode for GIO5 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
		unsigned int SPI_GIO4_Wake_LSctrl	: 1; /* Configuration mode for GIO4 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
		unsigned int SPI_GIO2_Wake_LSctrl	: 1; /* Configuration mode for GIO2 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
		unsigned int SPI_GIO0_Wake_LSctrl	: 1; /* Configuration mode for GIO0 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
		
	} field;
} L9369_REG_DATA_0x76;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int SPI_GIO0_HSctrl	: 1; /* Command bit for HS stage control for GIO0 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO1_HSctrl	: 1; /* Command bit for HS stage control for GIO1 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO2_HSctrl	: 1; /* Command bit for HS stage control for GIO2 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO3_HSctrl	: 1; /* Command bit for HS stage control for GIO3 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO4_HSctrl	: 1; /* Command bit for HS stage control for GIO4 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO5_HSctrl	: 1; /* Command bit for HS stage control for GIO5 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO6_HSctrl	: 1; /* Command bit for HS stage control for GIO6 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO7_HSctrl	: 1; /* Command bit for HS stage control for GIO7 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_GIO8_HSctrl	: 1; /* Command bit for HS stage control for GIO8 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
		unsigned int SPI_enPWM_GIO		: 1; /* Enable command bit for Current shaping for GIO : 1 / R/W('0' = Current shaping option disabled) */
		unsigned int reserved			: 1; /* 1 / R/W */
		unsigned int SPI_ACT_valid		: 1; /* Validation bit for H-bridge actuation data : 0 / R/W(Read fix value 0, write 1 is actuation valid trigger for Hbridge stage) */
	} field;
} L9369_REG_DATA_0x77;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used2 			: 4; /* 0000 */
		unsigned int GHS1_B_fail_SPI	: 1; /* Gate source monitoring for HS1 stage B : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int GHS2_B_fail_SPI	: 1; /* Gate source monitoring for HS2 stage B : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int GLS1_B_fail_SPI	: 1; /* Gate source monitoring for LS1 stage B : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int GLS2_B_fail_SPI	: 1; /* Gate source monitoring for LS2 stage B : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int OC1_det_A_SPI		: 1; /* Overcurrent detection on CSA1 for stage A : 0 / R ('1' = overcurrent failure detected on CSA1 stage A) */
		unsigned int OC2_det_A_SPI		: 1; /* Overcurrent detection on CSA2 for stage A : 0 / R ('1' = overcurrent failure detected on CSA2 stage A) */
		unsigned int OC1_det_B_SPI		: 1; /* Overcurrent detection on CSA1 for stage B : 0 / R ('1' = overcurrent failure detected on CSA1 stage B) */
		unsigned int OC2_det_B_SPI		: 1; /* Overcurrent detection on CSA2 for stage B : 0 / R ('1' = overcurrent failure detected on CSA2 stage B) */
		unsigned int Not_used1 			: 4; /* 1111 */
		} field;
} L9369_REG_DATA_0x78;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int DSHS1_A_fail_SPI	: 1; /* Drain-Source Monitoring for HS1 stage A : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSHS2_A_fail_SPI	: 1; /* Drain-Source Monitoring for HS2 stage A : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSLS1_A_fail_SPI	: 1; /* Drain-Source Monitoring for LS1 stage A : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSLS2_A_fail_SPI	: 1; /* Drain-Source Monitoring for LS2 stage A : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSHS1_B_fail_SPI	: 1; /* Drain-Source Monitoring for HS1 stage B : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSHS2_B_fail_SPI	: 1; /* Drain-Source Monitoring for HS2 stage B : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSLS1_B_fail_SPI	: 1; /* Drain-Source Monitoring for LS1 stage B : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int DSLS2_B_fail_SPI	: 1; /* Drain-Source Monitoring for LS2 stage B : 0 / R ('1' = drain-source monitoring failure detected) */
		unsigned int GHS1_A_fail_SPI	: 1; /* Gate-Source Monitoring on HS1 stage A : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int GHS2_A_fail_SPI	: 1; /* Gate-Source Monitoring on HS2 stage A : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int GLS1_A_fail_SPI	: 1; /* Gate-Source Monitoring on LS1 stage A : 0 / CoR ('1' = gate-source monitoring failure detected) */
		unsigned int GLS2_A_fail_SPI	: 1; /* Gate-Source Monitoring on LS2 stage A : 0 / CoR ('1' = gate-source monitoring failure detected) */
	} field;
} L9369_REG_DATA_0x79;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int GIO3_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO3 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO4_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO4 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO5_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO5 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO6_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO6 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO7_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO7 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO8_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO8 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO_ot_SPI			: 1; /* General overtemperature detection on GIO block : 0 / CoR ('1' = general over-temperature failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int Unused 			: 5; /* 11111 */
		} field;
} L9369_REG_DATA_0x7A;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int GIO0_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO0 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO1_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO1 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO2_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO2 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO3_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO3 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO4_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO4 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO5_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO5 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO6_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO6 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO7_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO7 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO8_failSCG_SPI	: 1; /* Short-to-ground failure detection on GIO8 : 0 / CoR ('1' = short to ground failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO0_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO0 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO1_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO1 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int GIO2_failSCB_SPI	: 1; /* Short-to-battery failure detection on GIO2 : 0 / CoR ('1' = short to battery failure detected) (no influence on pre-driver stage turn-off) */
		
		} field;
} L9369_REG_DATA_0x7B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int GPIO_ot_SPI	: 1; /* Overtemperature on GPIO stage : 0 / CoR ('1' = Overtemperature detected) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT1_ol_SPI	: 1; /* Open-load on HSOUT1 : 0 / CoR ('1' = Open-load failure detected on HSOUT1) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT1_low_SPI	: 1; /* Low level detected on HSOUT1 : 0 / CoR ('1' = Low level detected on HSOUT1) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT1_high_SPI: 1; /* High level detected on HSOUT1 : 0 / CoR ('1' = High level detected on HSOUT1) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT1_ot_SPI	: 1; /* Overtemperature on HSOUT1 stage : 0 / CoR ('1' = Overtemperature failure detected on HSOUT1) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT0_ol_SPI	: 1; /* Open-load on HSOUT0 : 0 / CoR ('1' = Open-load failure detected on HSOUT0) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT0_low_SPI	: 1; /* Low level detected on HSOUT0 : 0 / CoR ('1' = Low level detected on HSOUT0) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT0_high_SPI: 1; /* High level detected on HSOUT0 : 0 / CoR ('1' = High level detected on HSOUT0) (no influence on pre-driver stage turn-off) */
		unsigned int HSOUT00_ot_SPI	: 1; /* Overtemperature on HSOUT0 stage : 0 / CoR ('1' = Overtemperature failure detected on HSOUT0) (no influence on pre-driver stage turn-off) */
		unsigned int unused 		: 3; /* 111 */
		
		} field;
} L9369_REG_DATA_0x7C;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int GPIO0_high_SPI		: 1; /* High level detected on GPIO0 : undefined / CoR ('1' = High level detected on GPIO0) (please review AN for further details) */
		unsigned int GPIO0_low_SPI		: 1; /* Low level detected on GPIO0 : undefined / CoR ('1' = Low level detected on GPIO0) (please review AN for further details) */
		unsigned int GPIO1_high_SPI		: 1; /* High level detected on GPIO1 : undefined / CoR ('1' = High level detected on GPIO1) (please review AN for further details) */
		unsigned int GPIO1_low_SPI		: 1; /* Low level detected on GPIO1 : undefined / CoR ('1' = Low level detected on GPIO1) (please review AN for further details) */
		unsigned int GPIO2_high_SPI		: 1; /* High level detected on GPIO2 : undefined / CoR ('1' = High level detected on GPIO2) (please review AN for further details) */
		unsigned int GPIO2_low_SPI		: 1; /* Low level detected on GPIO2 : undefined / CoR ('1' = Low level detected on GPIO2) (please review AN for further details) */
		unsigned int GPIO3_high_SPI		: 1; /* High level detected on GPIO3 : undefined / CoR ('1' = High level detected on GPIO3) (please review AN for further details) */
		unsigned int GPIO3_low_SPI		: 1; /* Low level detected on GPIO3 : undefined / CoR ('1' = Low level detected on GPIO3) (please review AN for further details) */
		unsigned int GPIO0_failSCB_SPI	: 1; /* Short-to-battery failure detection for GPIO0 : 0 / CoR ('1' = short to battery failure detected on GPIO0) (no influence on pre-driver stage turn-off) */
		unsigned int GPIO1_failSCB_SPI	: 1; /* Short-to-battery failure detection for GPIO1 : 0 / CoR ('1' = short to battery failure detected on GPIO1) (no influence on pre-driver stage turn-off) */
		unsigned int GPIO2_failSCB_SPI	: 1; /* Short-to-battery failure detection for GPIO2 : 0 / CoR ('1' = short to battery failure detected on GPIO2) (no influence on pre-driver stage turn-off) */
		unsigned int GPIO3_failSCB_SPI	: 1; /* Short-to-battery failure detection for GPIO3 : 0 / CoR ('1' = short to battery failure detected on GPIO3) (no influence on pre-driver stage turn-off) */
		
		} field;
} L9369_REG_DATA_0x7D;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int WD_fail_SPI		: 1; /* WD failure detected : 0 / R */
		unsigned int WD_DatafailCNT_SPI	: 2; /* Returns data failure : 00 / R */
		unsigned int WD_on_SPI			: 1; /* WD enable : 0 / R */
		unsigned int WD_OVtimeCNT_SPI	: 6; /* Returns Overflow timer value : 000000 / R */
		unsigned int OTP_CRC_fail_SPI	: 1; /* CRC check on trimming cells : 0 ('1' = CRC failure detected) */
		unsigned int Unused				: 1; /* 1 */
		} field;
} L9369_REG_DATA_0x7E;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int Trimming_SPI		: 1; /* L9369 trimming indication : 1 / R */
		unsigned int PROM_rdCNT2_SPI	: 1; /* Trimming bit read cycle counter bit 2 : 0 / R */
		unsigned int PROM_rdCNT1_SPI	: 1; /* Trimming bit read cycle counter bit 1 : 0 / R */
		unsigned int GPIO3_ol_SPI		: 1; /* OL detection on GPIO3 : 0 / R ('1' = Open-load failure detected on GPIO3) (no influence on pre-driver stage turn-off) */
		unsigned int GPIO2_ol_SPI		: 1; /* OL detection on GPIO2 : 0 / R ('1' = Open-load failure detected on GPIO2) (no influence on pre-driver stage turn-off) */
		unsigned int GPIO1_ol_SPI		: 1; /* OL detection on GPIO1 : 0 / R ('1' = Open-load failure detected on GPIO1) (no influence on pre-driver stage turn-off) */
		unsigned int GPIO0_ol_SPI		: 1; /* OL detection on GPIO0 : 0 / R ('1' = Open-load failure detected on GPIO0) (no influence on pre-driver stage turn-off) */
		unsigned int SYNC5_fail_SPI		: 1; /* Synchronization failure on SYNC5 : 1 / CoR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SYNC1_fail_SPI		: 1; /* Synchronization failure on SYNC1, (too many SYNC1 triggers detected) : 1 / CoR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int ENEL_LowState_SPI	: 1; /* Low level detected on EN_EL : Pin status dependant / CoR (1 low state detected) Please review AN for further details */
		unsigned int ENEL_HighState_SPI	: 1; /* High level detected on EN_EL : Pin status dependant / CoR (1 high level detected) Please review AN for further details */
		unsigned int ENEL_State_SPI		: 1; /* Current status on EN_EL pin : Pin status dependant / 0 Please review AN for further details */
	} field;
} L9369_REG_DATA_0x7F;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int MSS_B_ot_SPI		: 1; /* Overtemperature detection on MSS stage B : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int MSS_A_ot_SPI		: 1; /* Overtemperature detection on MSS stage A : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int MSP_B_STB_SPI		: 1; /* Short-to-battery detection on MSP stage B : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int MSP_A_STB_SPI		: 1; /* Short-to-battery detection on MSP stage A : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_B_K7_UV_SPI		: 1; /* UV detection on K7 comparator, stage B : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_B_K6_SPI		: 1; /* K6 comparator SCB on MSM stage B (T2 switch) : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_B_K3_UC_SPI		: 1; /* Undercurrent detection on K3 comparator stage B : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_B_K1_SPI		: 1; /* K1 comparator for SCG detection (T1 switch) stage B : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_A_K7_UV_SPI		: 1; /* UV detection on K7 comparator, stage A : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_A_K6_SPI		: 1; /* K6 comparator SCB on MSM stage A (T2 switch) : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_A_K3_UC_SPI		: 1; /* Undercurrent detection on K3 comparator stage A : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
		unsigned int SF_A_K1_SPI		: 1; /* K1 comparator for SCG detection (T1 switch) stage A : 0 / CR ('1' = failure detected) (no influence on pre-driver stage turn-off) */
	} field;
} L9369_REG_DATA_0x80;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int RVPG_on_SPI		: 1; /* RVPG actuation bit : 0 / CoR ('1' enabled) */
		unsigned int CP_fail_SPI		: 1; /* Charge pump output voltage too low failure bit : 0 / CoR ('1' enabled bit) (to be cleared after WAU='1') */
		unsigned int VSCP_low_SPI		: 1; /* UV detection on Charge pump supply : 0 / CoR ('1' = failure detected) */
		unsigned int OSC_fail_SPI		: 1; /* Internal Oscillator failure (if oscillators frequency differs from more than 10%) : 0 / CoR ('1' = failure detected) */
		unsigned int VBP_Ust_fail_SPI	: 1; /* UV on VBP with short filtering time : 0 / CoR ('1' = failure detected on VBP) */
		unsigned int VBP_Ulg_fail_SPI	: 1; /* UV on VBP with long filtering time : 0 / CoR ('1' = failure detected on VBP) */
		unsigned int VBP_Ov_fail_SPI	: 1; /* OV on VBP : 0 / CoR ('1' = OV failure detected on VBP) */
		unsigned int OT_fail_SPI		: 1; /* General overtemperature failure : 0 / CoR ('1' = OT failure detected) */
		unsigned int Vint_Uv_fail_SPI	: 1; /* UV fault on internal voltage regulators : 0 / CoR ('1' = failure detected) (internal reset after filtering time and pre-driver stage turn-off via RL mode) */
		unsigned int Vint_Ov_fail_SPI	: 1; /* OV fault on internal voltage regulators : 0 / CoR ('1' = failure detected) (internal reset after filtering time and pre-driver stage turn-off via RL mode) */
		unsigned int GND_loss_fail_SPI	: 1; /* GND loss detection : 0 / CoR ('1' = failure detected) (internal reset after filtering time and pre-driver stage turn-off via RL mode) */
		unsigned int V5V_Uv_fail_SPI	: 1; /* UV detection on V5V (specific internal voltage regulator for pull-up sources in button interface block) : 0 / CoR ('1' = failure detected) */
		
	} field;
} L9369_REG_DATA_0x81;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int MSStest_fail_SPI	: 12; /* 15bit MSS test failure register (12-bit low part) : 0000 0000 0000 / CR */
	} field;
} L9369_REG_DATA_0x82;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int MSStest_stateCNT_SPI	: 3; /* MSS test state counter : 000 / CR */
		unsigned int unused2				: 1; /* 1 */
		unsigned int MSStest_failCNT_SPI	: 3; /* MSS test fail counter : 000 / CR */
		unsigned int unused1				: 1; /* 1 */
		unsigned int MSStest_fail_SPI		: 3; /* MSS test fail(3-bit high part) : 000 / CR */
	} field;
} L9369_REG_DATA_0x83;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 2; /* 00 */
		unsigned int MSSresult_reg_SPI	: 14; /* 14bit MSS test result register : 00000000000000 / R/W */
	} field;
} L9369_REG_DATA_0x84;

typedef union {
	uint16 data;
	struct {
	
		unsigned int Not_used 					: 4; /* 0000 */
		unsigned int SPI_K7_Thr					: 4; /* K7 comparator threshold. MSS UV detection threshold : 0000 / R/W */
		/* Reference Ai K7-min(V) K7 typ(V) K7-max(V) SPI_K7_thres
			A0          3.85      4         4.15      0000 (default)
			A1          3.85      4         4.15      0001
			A2          4.1       4.25      4.4       0010
			A3          4.35      4.5       4.65      0011
			A4          4.6       4.75      4.9       0100
			A5          4.85      5         5.15      0101
			A6          5.1       5.25      5.4       0110
			A7          5.35      5.5       5.65      0111
			A8          5.6       5.75      5.9       1000
			A9          5.85      6         6.15      1001
			A10         6.1       6.25      6.4       1010
			A11         6.35      6.5       6.65      1011
			A12         6.6       6.75      6.9       1100
			A13         6.85      7         7.15      1101
			A14         7.1       7.25      7.4       1110
			A15         7.1       7.25      7.4       1111
		*/
		unsigned int SPI_Mtest_B				: 1; /* Motor Test actuation on stage B : 0 (write 1 is trigger bit, reset by L9369 when test is finished) */
		unsigned int MSStest_start_busy_SPI		: 1; /* MSS test is running : 0 / R/W */
		unsigned int MSStest_test_start_busy_SPI: 1; /* Test of the MSS test is running : 0 / R/W */
		unsigned int SPI_Mtest_A				: 1; /* Motor Test actuation on stage A : 0 (write 1 is trigger bit, reset by L9369 when test is finished) */
		unsigned int SPI_K4_Thr 				: 2; /* K4 comparator threshold MSS speed signal detection threshold : 00 / R/W */
		unsigned int MSSresult_reg_SPI			: 2; /* 2bit MSS test result register : 00 / R/W */
		
	} field;
} L9369_REG_DATA_0x85;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 		: 12; /* 000001000001 (Error is returned in case of writing access) */
		unsigned int SPI_off_GPIO3	: 1; /* Driving control for GPIO3 stage : R/W '0' = GPIO3 is controlled via SPI command '1'= GPIO3 is controlled via multiplexer (default) */
		unsigned int SPI_off_GPIO2	: 1; /* Driving control for GPIO2 stage : R/W '0' = GPIO2 is controlled via SPI command '1'= GPIO2 is controlled via multiplexer (default) */
		unsigned int SPI_off_GPIO1	: 1; /* Driving control for GPIO1 stage : R/W '0' = GPIO1 is controlled via SPI command '1'= GPIO1 is controlled via multiplexer (default) */
		unsigned int SPI_off_GPIO0	: 1; /* Driving control for GPIO0 stage : R/W '0' = GPIO0 is controlled via SPI command '1'= GPIO0 is controlled via multiplexer (default) */
	} field;
} L9369_REG_DATA_0x86;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_off_GIO0	: 1; /* Driving control for GIO0 LS stage : R/W '0' = GIO0 is controlled via SPI command '1'= GIO0 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO1	: 1; /* Driving control for GIO1 LS stage : R/W '0' = GIO1 is controlled via SPI command '1'= GIO1 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO2	: 1; /* Driving control for GIO2 LS stage : R/W '0' = GIO2 is controlled via SPI command '1'= GIO2 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO3	: 1; /* Driving control for GIO3 LS stage : R/W '0' = GIO3 is controlled via SPI command '1'= GIO3 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO4	: 1; /* Driving control for GIO4 LS stage : R/W '0' = GIO4 is controlled via SPI command '1'= GIO4 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO5	: 1; /* Driving control for GIO5 LS stage : R/W '0' = GIO5 is controlled via SPI command '1'= GIO5 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO6	: 1; /* Driving control for GIO6 LS stage : R/W '0' = GIO6 is controlled via SPI command '1'= GIO6 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO7	: 1; /* Driving control for GIO7 LS stage : R/W '0' = GIO7 is controlled via SPI command '1'= GIO7 is controlled via multiplexer (default) */
		unsigned int SPI_off_GIO8	: 1; /* Driving control for GIO8 LS stage : R/W '0' = GIO8 is controlled via SPI command '1'= GIO8 is controlled via multiplexer (default) */
		unsigned int unused 		: 3; /* 111 */
			

	} field;
} L9369_REG_DATA_0x87;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 4; /* 0000 */
		unsigned int GIO8_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO8 pin : undefined / R */
		unsigned int GIO7_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO7 pin : undefined / R */
		unsigned int GIO6_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO6 pin : undefined / R */
		unsigned int GIO5_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO5 pin : undefined / R */
		unsigned int GIO4_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO4 pin : undefined / R */
		unsigned int GIO3_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO3 pin : undefined / R */
		unsigned int GIO2_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO2 pin : undefined / R */
		unsigned int GIO1_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO1 pin : undefined / R */
		unsigned int GIO0_status			: 1; /* Status feedback for diagnostic in Normal Mode on relative GIO0 pin : undefined / R */
		unsigned int SYS_WAKE_STATUS_CLEAR	: 1; /* Clear SYS_WAKE_IN_STATUS bit : 0 / R/W ('1' = SYS_WAKE_IN_STATUS bit is reset) */
		unsigned int GPIO_dly_dis			: 1; /* Disable the delay between the GPIO configured as digital input and the relevant GPIO driven through the multiplexer : 0 / R/W */
		unsigned int OSC1_fail				: 1; /* Result bit of oscillator (CLK1) monitor test. (stuck-at fail detection test) : 0 / R (reset by 81h read) */
				


	} field;
} L9369_REG_DATA_0x88;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 13; /* 0000 1111 1111 1 */
		unsigned int GIO_CAP_LS_LOW		: 1; /* Pre-trigger CAP test : 0 */
		unsigned int GIO_CAP_Tsample	: 1; /* Tsample configuration for O/S diagnostics : 0 */
		unsigned int Unused				: 1; /* 0 */
	} field;
} L9369_REG_DATA_0x89;

typedef union {
	uint16 data;
	struct {

		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int Fault_pin_config	: 4; /* Configuration table of the fault pin on GPIO0 : 0000 */
		/* Fault_pin_config Fault
			0000            All failures (default)
			0001            Overcurrent on stage A
			0010            Overcurrent on stage B
			0011            Overcurrent on both stages
			0100            VDS monitoring (HS and LS) (1)
			0101            Gate-source Monitoring (HS and LS)(1)
			0110            VDS & Gate-source Monitoring (1)
			0111            Power Supply Monitoring (VBP overvoltage and undervoltage, Vinternal UV/OV (2), Charge Pump failure (CP_fail_SPI, VSCP_low_SPI)
			1000            Overtemperature
			1001            Init_fail_SPI
			1010            Oscillator failure
			1011            Watchdog failure (WD_fail_SPI)
			1100            Synchronization failure on SYNC1 or SYNC5 ( SYNC1 _fail_SPI, SYNC5_fail_SPI)
			1101            HSOUTx failure (Open Load in Offstate HSOUTx_ol_SPI, overtemperature HSOUTx_ot_SPI) (3)
			1110            Button Interface Failure (overtemperature, SCG, SCB)
			1111            GPIOy failure (overtemperature and SCB) (4)
							1. DS fault on stage A or B.
							2. UV or OV on internal regulators Vanalog, Vdigital, Vpre.
							3. Also for LS stage. SPI bit is present in register 8Ah/138d.
							4. OT failure reporting is valid only for GPIO2 and GPIO3 due to thermal sensor sharing between GPIO0 and GPIO1.
		*/		
		unsigned int Fault_pin_en		: 1; /* Fault pin enable on GPIO0 : 0  */
		unsigned int HSOUT0_LS_mode		: 1; /* Enable command for LS configuration for lamp driver stage #0 : 0 */
		unsigned int HSOUT1_LS_mode		: 1; /* Enable command for LS configuration for lamp driver stage #1 : 0 */
		unsigned int EN_EL_RST_EN		: 1; /* RST function enable on EN_EL pin (#47) : 0 */
		unsigned int SYS_WAKE_OUT_EN	: 1; /* SYS wake/SYS_WAKE_IN enable bit : 1 */
		unsigned int SYS_WAKE_IN_status	: 1; /* SYS_WAKE_IN monitoring bit : 1/R */
		unsigned int i_gio_hs_low		: 1; /* Reduced current configuration bit : 1 (default) = IGIOx_pu_red2 0 = IGIOx_pu_red1 */
		unsigned int Unused				: 1; /* 1 */

	} field;
} L9369_REG_DATA_0x8A;

typedef union {
	uint16 data;
	struct {

		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int GIO8_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO7_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO6_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO5_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO4_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO3_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO2_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO1_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int GIO0_OL_CAP	: 1; /* GIO_CAP open load test result : 0 */
		unsigned int o_i_gio_high_th: 1; /* SCG comparator enable for button with diode and integrated resistors : 0 */
		unsigned int STUCK_MODE 	: 2; /* Stuck mode enable : 01 */
	

		
		} field;
} L9369_REG_DATA_0x8B;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 	: 4; /* 0000 */
		unsigned int SPARE0		: 12; /* PROM SPARE application register : 000000000000 / R/W */
		} field;
} L9369_REG_DATA_0xCA;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 	: 4; /* 0000 */
		unsigned int SPARE1		: 12; /* PROM SPARE application register : 000000000000 / R/W */
		} field;
} L9369_REG_DATA_0xCB;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 	: 4; /* 0000 */
		unsigned int SPARE2		: 12; /* PROM SPARE application register : 000000000000 / R/W */
		} field;
} L9369_REG_DATA_0xCC;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 	: 4; /* 0000 */
		unsigned int SPARE3		: 12; /* PROM SPARE application register : 000000000000 / R/W */
		} field;
} L9369_REG_DATA_0xCD;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 				: 8; /* 0000 1111 */
		unsigned int SPI_VSBRIDGE_B_Ctrl	: 1; /* Disable function of ADC stage for VSBRIDGE voltage measurement path on stage B : 1 / R/W ('1' = voltage measurement path on VSBRIDGE stage B is enabled) ('0' = voltage measurement path on VSBRIDGE stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_CSIN1_B_Ctrl		: 1; /* Disable function on ADC for CSIN1 current measurement path for stage B : 1 / R/W ('1' = Current measurement path on CSA1 stage B is enabled) ('0' = Current measurement path on CSA1 stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_SH2_B_Ctrl			: 1; /* Disable function on ADC stage for SH2 voltage measurement path of stage B : 1 / R/W ('1' = Voltage measurement path on SH2 on stage B is enabled) ('0' = Voltage measurement path on SH2 on stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_SH1_B_Ctrl			: 1; /* Disable function on ADC stage for SH1 voltage measurement path of stage B : 1 / R/W ('1' = Voltage measurement path on SH1 on stage B is enabled) ('0' = Voltage measurement path on SH1 on stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_VSBRIDGE_A_Ctrl	: 1; /* Disable function of ADC stage for VSBRIDGE voltage measurement path on stage A : 1 / R/W ('1' = voltage measurement path on VSBRIDGE stage A is enabled) ('0' = voltage measurement path on VSBRIDGE stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_CSIN1_A_Ctrl		: 1; /* Disable function on ADC for CSIN1 current measurement path for stage A : 1 / R/W ('1' = Current measurement path on CSA1 stage A is enabled) ('0' = Current measurement path on CSA1 stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_SH2_A_Ctrl			: 1; /* Disable function on ADC stage for SH2 voltage measurement path of stage A : 1 / R/W ('1' = Voltage measurement path on SH2 on stage A is enabled) ('0' = Voltage measurement path on SH2 on stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		unsigned int SPI_SH1_A_Ctrl			: 1; /* Disable function on ADC stage for SH1 voltage measurement path of stage A : 1 / R/W ('1' = Voltage measurement path on SH1 on stage A is enabled) ('0' = Voltage measurement path on SH1 on stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
		} field;
} L9369_REG_DATA_0xCE;

typedef union {
	uint16 data;
	struct {
		
		unsigned int Not_used 		: 4; /* 0000 */
		unsigned int SPI_OC_B_res	: 1; /* Reset for Overcurrent failure for stage B : 0 ('1' = reset Overcurrent failure) */
		unsigned int SPI_cfg_DSblank: 2; /* Blanking time configuration for DS monitoring : 00 (please verify pre-driver chapter for relative detailed configuration table) */
		/* SPI_cfg_DSblank Tds_blank_min(伊s) Tds_blank_typ(伊s) Tds_blank_max(伊s)
			00 (default)      22.8                 24                  25.62
			01                19                   20                  21.42
			10                15.2                 16                  17.22
			11                 3.8                  4                   4.62
		*/
		unsigned int Unused			: 7; /* 0000000 */
		} field;
} L9369_REG_DATA_0xD1;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 13; /* 0 */
		unsigned int SPI_on100_HSOUT1	: 1; /* Full 100% on PWM mode on HSOUT1 lamp driver stage : 0 / RW = not 100% PWM mode active */
		unsigned int SPI_on100_HSOUT0	: 1; /* Full 100% on PWM mode on HSOUT0 lamp driver stage : 0 / RW = not 100% PWM mode active */
		unsigned int Unused				: 1; /* 1/R */
		} field;
} L9369_REG_DATA_0xD3;

typedef union {
	uint16 data;
	struct {
		unsigned int Not_used 			: 4; /* 0000 */
		unsigned int Unused2			: 2; /* 11 */
		unsigned int Frame_CNT_error	: 1; /* Frame counter error detected : 0 / R ('1' = failure detected) */
		unsigned int CRC_Error			: 1; /* 0 */
		unsigned int Frame_too_short	: 1; /* SPI Frame too short detected : 0 / R ('1' = failure detected) */
		unsigned int Frame_too_long		: 1; /* SPI Frame too long detected : 0 / R ('1' = failure detected) */
		unsigned int Unused1			: 5; /* 11111 */
		
		} field;
} L9369_REG_DATA_0xFD;


typedef struct {
	unsigned int HS1_A		: 1;
	unsigned int HS2_A		: 1;
	unsigned int HS1_B		: 1;
	unsigned int HS2_B		: 1;
	unsigned int LS1_A		: 1;
	unsigned int LS2_A		: 1;
	unsigned int LS1_B		: 1;
	unsigned int LS2_B		: 1; /* 1 */
	unsigned int HSOUT0		: 1;
	unsigned int PWM_A		: 7; /* 2 */
	unsigned int HSOUT1		: 1;
	unsigned int PWM_B		: 7; /* 3 */
	unsigned int WD_Cmd		: 4;
	unsigned int Init		: 1;
	unsigned int GIO0		: 1;
	unsigned int GIO1		: 1;
	unsigned int GIO2		: 1; /* 4*/
	unsigned int GIO3		: 1;
	unsigned int GIO4		: 1;
	unsigned int GIO5		: 1;
	unsigned int GIO6		: 1;
	unsigned int GIO7		: 1;
	unsigned int GIO8		: 1;
	unsigned int SWGIO0		: 1;
	unsigned int SWGIO2		: 1;
	unsigned int SWGIO3		: 1;
	unsigned int SWGIO5		: 1;
	unsigned int ACT_Valid	: 1;
	unsigned int Step		: 4;
	unsigned int T1_A 		: 1;
	unsigned int T1_B 		: 1;
	unsigned int T2_A 		: 1;
	unsigned int T2_B 		: 1;
	unsigned int SUBFAILSAFE : 1;
	unsigned int Sync_1ms 	: 1;
	unsigned int Sync_5ms 	: 1;
	unsigned int SyncStep	: 4;
	unsigned int SwitchMode	: 8;

} L9369_RegWrite_t;


typedef struct {
	L9369_REG_DATA_0x02 _0x02;
	L9369_REG_DATA_0x03 _0x03;
	L9369_REG_DATA_0x04 _0x04;
	L9369_REG_DATA_0x05 _0x05;
	L9369_REG_DATA_0x06 _0x06;
	L9369_REG_DATA_0x07 _0x07;
	L9369_REG_DATA_0x08 _0x08;
	L9369_REG_DATA_0x09 _0x09;
	L9369_REG_DATA_0x0A _0x0A;
	L9369_REG_DATA_0x0B _0x0B;
	L9369_REG_DATA_0x0C _0x0C;
	L9369_REG_DATA_0x0D _0x0D;
	L9369_REG_DATA_0x0E _0x0E;
	L9369_REG_DATA_0x0F _0x0F;
	L9369_REG_DATA_0x10 _0x10;
	L9369_REG_DATA_0x11 _0x11;
	L9369_REG_DATA_0x12 _0x12;
	L9369_REG_DATA_0x13 _0x13;
	L9369_REG_DATA_0x14 _0x14;
	L9369_REG_DATA_0x15 _0x15;
	L9369_REG_DATA_0x16 _0x16;
	L9369_REG_DATA_0x17 _0x17;
	L9369_REG_DATA_0x18 _0x18;
	L9369_REG_DATA_0x19 _0x19;
	L9369_REG_DATA_0x1A _0x1A;
	L9369_REG_DATA_0x1B _0x1B;
	L9369_REG_DATA_0x1C _0x1C;
	L9369_REG_DATA_0x1D _0x1D;
	L9369_REG_DATA_0x1E _0x1E;
	L9369_REG_DATA_0x1F _0x1F;
	L9369_REG_DATA_0x20 _0x20;
	L9369_REG_DATA_0x21 _0x21;
	L9369_REG_DATA_0x22 _0x22;
	L9369_REG_DATA_0x23 _0x23;
	L9369_REG_DATA_0x24 _0x24;
	L9369_REG_DATA_0x25 _0x25;
	L9369_REG_DATA_0x26 _0x26;
	L9369_REG_DATA_0x27 _0x27;
	L9369_REG_DATA_0x28 _0x28;
	L9369_REG_DATA_0x29 _0x29;
	L9369_REG_DATA_0x2A _0x2A;
	L9369_REG_DATA_0x2B _0x2B;
	L9369_REG_DATA_0x2C _0x2C;
	L9369_REG_DATA_0x2D _0x2D;
	L9369_REG_DATA_0x2E _0x2E;
	L9369_REG_DATA_0x2F _0x2F;
	L9369_REG_DATA_0x30 _0x30;
	L9369_REG_DATA_0x31 _0x31;
	L9369_REG_DATA_0x32 _0x32;
	L9369_REG_DATA_0x33 _0x33;
	L9369_REG_DATA_0x34 _0x34;
	L9369_REG_DATA_0x35 _0x35;
	L9369_REG_DATA_0x36 _0x36;
	L9369_REG_DATA_0x37 _0x37;
	L9369_REG_DATA_0x38 _0x38;
	L9369_REG_DATA_0x39 _0x39;
	L9369_REG_DATA_0x3A _0x3A;
	L9369_REG_DATA_0x3B _0x3B;
	L9369_REG_DATA_0x3C _0x3C;
	L9369_REG_DATA_0x3D _0x3D;
	L9369_REG_DATA_0x3E _0x3E;
	L9369_REG_DATA_0x3F _0x3F;
	L9369_REG_DATA_0x40 _0x40;
	L9369_REG_DATA_0x41 _0x41;
	L9369_REG_DATA_0x42 _0x42;
	L9369_REG_DATA_0x43 _0x43;
	L9369_REG_DATA_0x44 _0x44;
	L9369_REG_DATA_0x45 _0x45;
	L9369_REG_DATA_0x46 _0x46;
	L9369_REG_DATA_0x47 _0x47;
	L9369_REG_DATA_0x48 _0x48;
	L9369_REG_DATA_0x49 _0x49;
	L9369_REG_DATA_0x4A _0x4A;
	L9369_REG_DATA_0x4B _0x4B;
	L9369_REG_DATA_0x4C _0x4C;
	L9369_REG_DATA_0x4D _0x4D;
	L9369_REG_DATA_0x4E _0x4E;
	L9369_REG_DATA_0x4F _0x4F;
	L9369_REG_DATA_0x50 _0x50;
	L9369_REG_DATA_0x51 _0x51;
	L9369_REG_DATA_0x52 _0x52;
	L9369_REG_DATA_0x53 _0x53;
	L9369_REG_DATA_0x54 _0x54;
	L9369_REG_DATA_0x55 _0x55;
	L9369_REG_DATA_0x56 _0x56;
	L9369_REG_DATA_0x57 _0x57;
	L9369_REG_DATA_0x58 _0x58;
	L9369_REG_DATA_0x59 _0x59;
	L9369_REG_DATA_0x5A _0x5A;
	L9369_REG_DATA_0x5B _0x5B;
	L9369_REG_DATA_0x5C _0x5C;
	L9369_REG_DATA_0x5D _0x5D;
	L9369_REG_DATA_0x5E _0x5E;
	L9369_REG_DATA_0x5F _0x5F;
	L9369_REG_DATA_0x60 _0x60;
	L9369_REG_DATA_0x61 _0x61;
	L9369_REG_DATA_0x62 _0x62;
	L9369_REG_DATA_0x63 _0x63;
	L9369_REG_DATA_0x64 _0x64;
	L9369_REG_DATA_0x65 _0x65;
	L9369_REG_DATA_0x66 _0x66;
	L9369_REG_DATA_0x67 _0x67;
	L9369_REG_DATA_0x68 _0x68;
	L9369_REG_DATA_0x69 _0x69;
	L9369_REG_DATA_0x6A _0x6A;
	L9369_REG_DATA_0x6B _0x6B;
	L9369_REG_DATA_0x6C _0x6C;
	L9369_REG_DATA_0x6D _0x6D;
	L9369_REG_DATA_0x6E _0x6E;
	L9369_REG_DATA_0x6F _0x6F;
	L9369_REG_DATA_0x70 _0x70;
	L9369_REG_DATA_0x71 _0x71;
	L9369_REG_DATA_0x72 _0x72;
	L9369_REG_DATA_0x73 _0x73;
	L9369_REG_DATA_0x74 _0x74;
	L9369_REG_DATA_0x75 _0x75;
	L9369_REG_DATA_0x76 _0x76;
	L9369_REG_DATA_0x77 _0x77;
	L9369_REG_DATA_0x78 _0x78;
	L9369_REG_DATA_0x79 _0x79;
	L9369_REG_DATA_0x7A _0x7A;
	L9369_REG_DATA_0x7B _0x7B;
	L9369_REG_DATA_0x7C _0x7C;
	L9369_REG_DATA_0x7D _0x7D;
	L9369_REG_DATA_0x7E _0x7E;
	L9369_REG_DATA_0x7F _0x7F;
	L9369_REG_DATA_0x80 _0x80;
	L9369_REG_DATA_0x81 _0x81;
	L9369_REG_DATA_0x82 _0x82;
	L9369_REG_DATA_0x83 _0x83;
	L9369_REG_DATA_0x84 _0x84;
	L9369_REG_DATA_0x85 _0x85;
	L9369_REG_DATA_0x86 _0x86;
	L9369_REG_DATA_0x87 _0x87;
	L9369_REG_DATA_0x88 _0x88;
	L9369_REG_DATA_0x89 _0x89;
	L9369_REG_DATA_0x8A _0x8A;
	L9369_REG_DATA_0x8B _0x8B;
	L9369_REG_DATA_0xCA _0xCA;
	L9369_REG_DATA_0xCB _0xCB;
	L9369_REG_DATA_0xCC _0xCC;
	L9369_REG_DATA_0xCD _0xCD;
	L9369_REG_DATA_0xCE _0xCE;
	L9369_REG_DATA_0xD1 _0xD1;
	L9369_REG_DATA_0xD3 _0xD3;
	L9369_REG_DATA_0xFD _0xFD;
} L9369_RegVar_t;


typedef struct {
	unsigned int OC2_det_B_SPI		: 1; /* 0x78 */
	unsigned int OC1_det_B_SPI		: 1; /* 0x78 */
	unsigned int OC2_det_A_SPI		: 1; /* 0x78 */
	unsigned int OC1_det_A_SPI		: 1; /* 0x78 */
	unsigned int GLS2_B_fail_SPI	: 1; /* 0x78 */
	unsigned int GLS1_B_fail_SPI	: 1; /* 0x78 */
	unsigned int GHS2_B_fail_SPI	: 1; /* 0x78 */
	unsigned int GHS1_B_fail_SPI	: 1; /* 0x78 */ /* 8 */
	unsigned int GLS2_A_fail_SPI	: 1; /* 0x79 */
	unsigned int GLS1_A_fail_SPI	: 1; /* 0x79 */
	unsigned int GHS2_A_fail_SPI	: 1; /* 0x79 */
	unsigned int GHS1_A_fail_SPI	: 1; /* 0x79 */
	unsigned int DSLS2_B_fail_SPI	: 1; /* 0x79 */
	unsigned int DSLS1_B_fail_SPI	: 1; /* 0x79 */
	unsigned int DSHS2_B_fail_SPI	: 1; /* 0x79 */
	unsigned int DSHS1_B_fail_SPI	: 1; /* 0x79 */ /* 16 */
	unsigned int DSLS2_A_fail_SPI	: 1; /* 0x79 */
	unsigned int DSLS1_A_fail_SPI	: 1; /* 0x79 */
	unsigned int DSHS2_A_fail_SPI	: 1; /* 0x79 */
	unsigned int DSHS1_A_fail_SPI	: 1; /* 0x79 */
	unsigned int OTP_CRC_fail_SPI	: 1; /* 0x7E */
	unsigned int WD_fail_SPI		: 1; /* 0x7E */
	unsigned int V5V_Uv_fail_SPI	: 1; /* 0x81 */
	unsigned int GND_loss_fail_SPI	: 1; /* 0x81 */ /* 24 */
	unsigned int Vint_Ov_fail_SPI	: 1; /* 0x81 */
	unsigned int Vint_Uv_fail_SPI	: 1; /* 0x81 */
	unsigned int OT_fail_SPI		: 1; /* 0x81 */
	unsigned int VBP_Ov_fail_SPI	: 1; /* 0x81 */
	unsigned int VBP_Ulg_fail_SPI	: 1; /* 0x81 */
	unsigned int VBP_Ust_fail_SPI	: 1; /* 0x81 */
	unsigned int OSC_fail_SPI		: 1; /* 0x81 */
	unsigned int VSCP_low_SPI		: 1; /* 0x81 */ /* 32 */
	unsigned int CP_fail_SPI		: 1; /* 0x81 */
	unsigned int OSC1_fail			: 1; /* 0x88 */
	unsigned int GIO8_failSCB_SPI	: 1; /* 0x7A */
	unsigned int GIO7_failSCB_SPI	: 1; /* 0x7A */
	unsigned int GIO6_failSCB_SPI	: 1; /* 0x7A */
	unsigned int GIO5_failSCB_SPI	: 1; /* 0x7A */
	unsigned int GIO4_failSCB_SPI	: 1; /* 0x7A */
	unsigned int GIO3_failSCB_SPI	: 1; /* 0x7A */ /* 40 */
	unsigned int GIO2_failSCB_SPI	: 1; /* 0x7B */
	unsigned int GIO1_failSCB_SPI	: 1; /* 0x7B */
	unsigned int GIO0_failSCB_SPI	: 1; /* 0x7B */
	unsigned int GIO8_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO7_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO6_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO5_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO4_failSCG_SPI	: 1; /* 0x7B */ /* 48 */
	unsigned int GIO3_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO2_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO1_failSCG_SPI	: 1; /* 0x7B */
	unsigned int GIO0_failSCG_SPI	: 1; /* 0x7B */
	unsigned int Init_fail_SPI		: 1; /* 0x02 */
	unsigned int Init_busy_SPI		: 1; /* 0x02 */
	unsigned int CRC_fail_SPI		: 1; /* 0x03 */
	unsigned int ADW_fail_SPI		: 1; /* 0x03 */
	unsigned int ADR_fail_SPI		: 1; /* 0x03 */ /* 56 */
	unsigned int SPICLK_fail_SPI	: 1; /* 0x03 */
	unsigned int SYNC1_fail_SPI 	: 1; /* 0x7F */
	unsigned int SYNC5_fail_SPI 	: 1; /* 0x7F */
	unsigned int reserved			: 5; /* reserved */ /* 64 */
} L9369_FaultStatus_t;



typedef enum {
  TestActuatorState_Neutral = 0,
  TestActuatorState_Released = 1,
  TestActuatorState_Applied = 2, 
  TestActuatorState_Releasing = 3,
  TestActuatorState_Applying = 4
} TestActuatorState;        

typedef struct
{
	unsigned int fMaxApplied_Left				:1;
	unsigned int fMaxApplied_Right				:1;
	unsigned int fMaxReleased_Left				:1;
	unsigned int fMaxReleased_Right				:1;
	unsigned int fForceMaxApplied				:1;
	unsigned int fForceMaxReleased				:1;
	unsigned int fAppSW_DigMutex				:1;
	unsigned int fReleasSW_DigMutex				:1;
	unsigned int fAppSW_DigMutex_Force			:1;
	unsigned int fReleasSW_DigMutex_Force		:1;
	unsigned int fActuatorApplying_L 			:1;
	unsigned int fActuatorApplying_R 			:1;
	unsigned int fActuatorReleasing_L			:1;
	unsigned int fActuatorReleasing_R 			:1;
	unsigned int fmutex_apply_L					:1;			
	unsigned int fmutex_release_L 				:1;			
	unsigned int fmutex_apply_R 				:1;			
	unsigned int fmutex_release_R 				:1;
	unsigned int EpbStatusLeft					:3;
	unsigned int EpbStatusRight 				:3;
	unsigned int EpbMotorTestFlag				:4;			
	uint16 Cnt_Limit_Apply; 					
	uint16 Cnt_Limit_Release;					
	float Test_Cur_Limit;			
	
}L9369_Status_t;

void L9369_Read_Period(const uint8 *addr);
void L9369_Write(uint32_t RxData);
void L9369_Read(const uint8 *addr ,uint8 Num);


void CheckL9369RegStatus(uint8 step);
uint8 L9369CheckCrc5(uint32_t data);
L9369_RegWrite_t *L9369RegInit(void);
uint32_t* L9369_CRCtoArray(uint32_t value);
uint32_t L9369_CRCGeneration(uint32_t Data[],uint8 byteLength);
uint32_t L9369_CRCCal(uint32_t CRC_initial,uint32_t Data);
uint8_t L9369MakeCRC5(uint32_t data);
void L9369_Communication(uint16_t Mode, uint8_t Address, uint16_t Data);

uint32_t L9369GetMsg(uint8 addr);
uint32_t L9369GetMsg_0xFD(void);
uint32_t L9369SetMsg_0x68(uint8 DrainFlagReset);
uint32_t L9369SetMsg_0x69(uint8 wd);
uint32_t L9369SetMsg_0x6A(void);
uint32_t L9369SetMsg_0x6B(void);
uint32_t L9369SetMsg_0x6C(void);
uint32_t L9369SetMsg_0x02(uint8 reset_cmd);
uint32_t L9369SetMsg_0x6D(void);
uint32_t L9369SetMsg_0x6E(void);
uint32_t L9369SetMsg_0x6F(void);
uint32_t L9369SetMsg_0x70(void);
uint32_t L9369SetMsg_0x71(void);
uint32_t L9369SetMsg_0x72(uint8 gio3, uint8 gio4, uint8 gio5);
uint32_t L9369SetMsg_0x73(uint8 gio0, uint8 gio1, uint8 gio2);
uint32_t L9369SetMsg_0x74(void);
uint32_t L9369SetMsg_0x75(uint8 gio6, uint8 gio7, uint8 gio8);
uint32_t L9369SetMsg_0x76(void);
uint32_t L9369SetMsg_0x77(void);
uint32_t L9369SetMsg_0x81(void);
uint32_t L9369SetMsg_0x84(void);
uint32_t L9369SetMsg_0x85(void);
uint32_t L9369SetMsg_0x86(void);
uint32_t L9369SetMsg_0x87(uint8 gio0, uint8 gio1, uint8 gio2, uint8 gio3, uint8 gio4, uint8 gio5, uint8 gio6, uint8 gio7, uint8 gio8);
uint32_t L9369SetMsg_0x88(void);
uint32_t L9369SetMsg_0x8A(void);
uint32_t L9369SetMsg_0x8B(void);
uint32_t L9369SetMsg_0xCE(void);
uint32_t L9369SetMsg_0xD3(void);

void L9369WSS_Test(void);
void L9369CheckStatus(uint32 data);
void EpbMotorTestLogic(void);
float EpbMotorSHVoltageA(uint8 input);
float EpbMotorSHVoltageB(uint8 input);
uint8 GetEpbWakeupStatus(void);
void EpbMotorStatusSave(void);
void CheckL9369Watchdog(void);
uint8 L9369Init(uint8 flag);
void L9369Initialize(void);
float EpbMotorVoltage(uint8 isLeft);
float EpbMotorCurrent(uint8 isLeft);
void SetWarningLampDriver(uint8 hsout0, uint8 hsout1);
void EpbMotorDriveProcess(void);
void L9369SwitchInitialize(void);
void GetFailSafeStatus(void);
void L9369PowerOff(void);
uint8 L9369_StartupCheck(void);
void L9369_TASK_1ms(void);
void GetFailSafeStatus(void);
void L9369AdcAvr(void);
void L9369Check_5ms(void);
void L9369Check_20ms(void);
uint8 L9369Fail_Test1(void);

uint8 L9369Fail_Test2(void);

extern void Delayms(uint32_t time);

