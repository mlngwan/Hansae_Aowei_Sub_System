#include "Typedefs.h"

#define	MCU5VA_AdcChennel 					14			//PD[10] , GPI[14]
#define	MCU5VCC_AdcChennel 					13 			//PD[9] , GPI[13]
#define MCU3V3UC_AdcChennel					 1 			//PB[5] , GPI[1]
#define	BAT_CHK_AdcChennel 					10			//PD[6] , GPI[10]
#define	L_BD_TEMP_AdcChennel				11			//PD[7] , GPI[11]
#define R_BD_TEMP_AdcChennel				0			//PB[4] , GPI[0]
#define	VStb_AdcChennel 					2 			//PB[6] GPI[2]
#define	IGN_AdcChennel						32			 //PB[8] ANS[0]

#define	ADC_CH_SUB_IGN_MON					32			//PB[8] , ANS[0]
#define	ADC_CH_SUB_UBVR_MON 					9			//PD[5] , GPI[9]
#define	ADC_CH_SUB_UBB_MON					10			//PD[6] , GPI[10]

#define	ADC_REF		1000

#define	r300		300
#define	r75			75
#define	r51			51
#define	r30			30
#define	r12			12
#define	r10			10
#define	r7_5		7.5
#define	r3			3
#define	ref_3v3		3.3
#define	ref_5v		5.0

typedef struct
{
	struct 
	{
		uint16 IBAT; //ignore battory voltage
		uint16 UBVR;
		uint16 IGN;
		uint16 UBB;
	}Power;
	struct
	{
		uint16 MOTORA;
		uint16 MOTORB;
		uint16 MOTORA_CUR;
		uint16 MOTORB_CUR;
	}Motor;
}ADC_Physical_t;

typedef struct
{
	uint16 UBVR;
	uint16 IGN;
	uint16 UBB;
}ADC_Raw_t;
typedef struct
{
	unsigned int Timeout		: 1;
}ADC_Flag_t;

typedef struct
{
    ADC_Physical_t Physical;
    ADC_Raw_t Raw;
	ADC_Flag_t Flag;
} ADC_t;

extern void ADCInitialize(void);
uword GetAdcAverage(ubyte ADC_Channel);
void CheckAdcStatus(void);
void SetLogicData(void);
extern void CheckAdcStatus(void);
void GetRawData(void);

