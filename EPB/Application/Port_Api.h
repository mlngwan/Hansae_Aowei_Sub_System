#include "Typedefs.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

//#define	PCR_SUB_ACT_OFF_LS1_A					SIU.PCR[PORT_PIN_A0].R
//#define	PCR_SUB_ACT_OFF_LS2_A					SIU.PCR[PORT_PIN_A1].R
#define	PCR_SUB_UBB_EN								SIU.PCR[PORT_PIN_A3].R
#define	PCR_SUB_UBVR_EN								SIU.PCR[PORT_PIN_A4].R
#define	PCR_SUB_L9369_CTRL							SIU.PCR[PORT_PIN_A5].R
//#define	PCR_MAIN_EPB_FAILSAFE					SIU.PCR[PORT_PIN_A6].R
#define	PCR_CAN_SBC_INT							SIU.PCR[PORT_PIN_A7].R
//#define	PCR_SUB_ACT_OFF_LS2_B					SIU.PCR[PORT_PIN_A10].R
//#define	PCR_SUB_ACT_OFF_LS1_B					SIU.PCR[PORT_PIN_A11].R
#define	PCR_CAN_EEPROM_MISO						SIU.PCR[PORT_PIN_A12].R
#define	PCR_CAN_EEPROM_MOSI						SIU.PCR[PORT_PIN_A13].R
#define	PCR_CAN_EEPROM_SCLK						SIU.PCR[PORT_PIN_A14].R
#define	PCR_SUB_EEPROM_CS						SIU.PCR[PORT_PIN_A15].R
#define	PCR_SUB_IGN_MON							SIU.PCR[PORT_PIN_B8].R

#define	PCR_SPC560_SCLK							SIU.PCR[PORT_PIN_C2].R
#define	PCR_SPC560_CS							SIU.PCR[PORT_PIN_C3].R
#define	PCR_SPC560_MOSI							SIU.PCR[PORT_PIN_C4].R
#define	PCR_SPC560_MISO							SIU.PCR[PORT_PIN_C5].R
#define	PCR_SUB_L9369_SYNC5						SIU.PCR[PORT_PIN_C8].R
#define	PCR_SUB_L9369_SYNC1						SIU.PCR[PORT_PIN_C9].R


#define	PCR_SUB_L9369_MISO						SIU.PCR[PORT_PIN_C12].R
#define	PCR_SUB_L9369_MOSI						SIU.PCR[PORT_PIN_C13].R
#define	PCR_SUB_L9369_SCLK						SIU.PCR[PORT_PIN_C14].R
#define	PCR_SUB_L9369_CS						SIU.PCR[PORT_PIN_C15].R

#define PCR_SUB_UBVR_MON						SIU.PCR[PORT_PIN_D5].R
#define PCR_SUB_UBB_MON							SIU.PCR[PORT_PIN_D6].R

#define	PCR_SUB_L9369_RST						SIU.PCR[PORT_PIN_E3].R
#define	PCR_SUB_L9369_WAU						SIU.PCR[PORT_PIN_E4].R
#define	PCR_SUB_DEBUG							SIU.PCR[PORT_PIN_E6].R
#define	PCR_CAN_SBC_CS							SIU.PCR[PORT_PIN_E7].R
#define	PCR_SUB_EPB_SBC_FAULT					SIU.PCR[PORT_PIN_E10].R
//#define	PCR_SUB_CAN1_MODE						SIU.PCR[PORT_PIN_E12].R



//#define	PORT_SUB_ACT_OFF_LS1_A					SIU.GPDO[PORT_PIN_A0].R
//#define	PORT_SUB_ACT_OFF_LS2_A					SIU.GPDO[PORT_PIN_A1].R
#define	PORT_SUB_UBB_EN							SIU.GPDO[PORT_PIN_A3].R
#define	PORT_SUB_UBVR_EN							SIU.GPDO[PORT_PIN_A4].R
#define	PORT_SUB_L9369_CTRL						SIU.GPDO[PORT_PIN_A5].R
//#define	PORT_MAIN_EPB_FAILSAFE					SIU.GPDI[PORT_PIN_A6].R
#define	PORT_CAN_SBC_INT						SIU.GPDI[PORT_PIN_A7].R
//#define	PORT_SUB_ACT_OFF_LS2_B					SIU.GPDO[PORT_PIN_A10].R
//#define	PORT_SUB_ACT_OFF_LS1_B					SIU.GPDO[PORT_PIN_A11].R
#define	PORT_CAN_EEPROM_MISO					SIU.GPDI[PORT_PIN_A12].R
#define	PORT_CAN_EEPROM_MOSI					SIU.GPDO[PORT_PIN_A13].R
#define	PORT_CAN_EEPROM_SCLK					SIU.GPDO[PORT_PIN_A14].R
#define	PORT_SUB_EEPROM_CS						SIU.GPDO[PORT_PIN_A15].R

#define	PORT_SUB_IGN_MON						SIU.GPDI[PORT_PIN_B8].R

#define	PORT_SPC560_SCLK						SIU.GPDI[PORT_PIN_C2].R
#define	PORT_SPC560_CS							SIU.GPDI[PORT_PIN_C3].R
#define	PORT_SPC560_MOSI						SIU.GPDI[PORT_PIN_C4].R
#define	PORT_SPC560_MISO						SIU.GPDO[PORT_PIN_C5].R
#define	PORT_SUB_L9369_SYNC5					SIU.GPDO[PORT_PIN_C8].R
#define	PORT_SUB_L9369_SYNC1					SIU.GPDO[PORT_PIN_C9].R



#define	PORT_SUB_L9369_MISO						SIU.GPDI[PORT_PIN_C12].R
#define	PORT_SUB_L9369_MOSI						SIU.GPDO[PORT_PIN_C13].R
#define	PORT_SUB_L9369_SCLK						SIU.GPDO[PORT_PIN_C14].R
#define	PORT_SUB_L9369_CS						SIU.GPDO[PORT_PIN_C15].R

#define PORT_SUB_UBVR_MON						SIU.GPDI[PORT_PIN_D5].R
#define PORT_SUB_UBB_MON						SIU.GPDI[PORT_PIN_D6].R

#define	PORT_SUB_L9369_RST						SIU.GPDO[PORT_PIN_E3].R
#define	PORT_SUB_L9369_WAU						SIU.GPDO[PORT_PIN_E4].R
#define	PORT_SUB_DEBUG							SIU.GPDO[PORT_PIN_E6].R
#define	PORT_CAN_SBC_CS							SIU.GPDO[PORT_PIN_E7].R
#define	PORT_SUB_EPB_SBC_FAULT					SIU.GPDI[PORT_PIN_E10].R
//#define	PORT_SUB_CAN1_MODE						SIU.GPDO[PORT_PIN_E12].R
#define IGN_ON										6000U
#define IGN_OFF_TIME								200U
#define MODE_MAIN_FAILSAFE		 					(TRUE)
#define MODE_SUB_FAILSAFE	 						(FALSE)

#define EPB_SW1										(0U)
#define EPB_SW2										(1U)
#define EPB_SW3										(2U)
#define EPB_SW4										(3U)
#define EPB_SW5										(4U)
#define EPB_SW7										(5U)

#define EPB_SW_NEUTRAL								(0x00U) /* SW7(L), SW6(L), SW5(L), SW4(L), SW3(L), SW2(L), SW1(L) */
#define EPB_SW_APPLY								(0x40U) /* SW7(H), SW6(L), SW5(L), SW4(L), SW3(L), SW2(L), SW1(L) */
#define EPB_SW_RELEASE								(0x04U) /* SW7(L), SW6(L), SW5(L), SW4(L), SW3(H), SW2(L), SW1(L) */

	
typedef struct{
	uint16 IgnOffCount; /* 10 ms counter */
	unsigned int MAIN_EPB_FAILSAFE	:1;
	unsigned int SUB_EPB_SBC_FAULT	:1;
}Port_t;


typedef struct {
	unsigned int bls		: 1;
	unsigned int ign		: 1;
	unsigned int esc_on_off	: 1;
	unsigned int avh		: 1;
	unsigned int hdc		: 1;
	unsigned int pb			: 1;
	unsigned int clutch		: 1;
	unsigned int epb_sw1 	: 1; /* 8 */
	unsigned int epb_sw2 	: 1;
	unsigned int epb_sw3 	: 1;
	unsigned int epb_sw4 	: 1;
	unsigned int epb_sw5 	: 1;
	unsigned int epb_sw7 	: 1;
	unsigned int epb_sw_light: 1;
	unsigned int epb_apply	: 1;
	unsigned int epb_release: 1; /* 16 */
	uint8 epb_status;
	uint8 reserved; /* 32 */
} SwitchStatus_t;

#define SWITCH_BLS(P)			P->bls
#define SWITCH_IGN(P)			P->ign
#define SWITCH_ESC_ON_OFF(P)	P->esc_on_off
#define SWITCH_AVH(P)			P->avh
#define SWITCH_HDC(P)			P->hdc
#define SWITCH_PB(P)			P->pb
#define SWITCH_CLUTCH(P)		P->clutch
#define SWITCH_EPB_SW1(P)		P->epb_sw1
#define SWITCH_EPB_SW2(P)		P->epb_sw2
#define SWITCH_EPB_SW3(P)		P->epb_sw3
#define SWITCH_EPB_SW4(P)		P->epb_sw4
#define SWITCH_EPB_SW5(P)		P->epb_sw5
#define SWITCH_EPB_SW7(P)		P->epb_sw7
#define SWITCH_EPB_SW_LIGHT(P)	P->epb_sw_light
#define SWITCH_EPB_APPLY(P)		P->epb_apply
#define SWITCH_EPB_RELEASE(P)	P->epb_release
#define SWITCH_EPB_STATUS(P)	P->epb_status

void SetPortInitialize(void);
void SetPadConfiguration(void);
void PORTInitialize(void);
void CheckPortStatus(void);

extern void SetSystemDown(void);
extern void SetSystemOn(void);
extern void SetSystemOff(void);
extern void SetEpbSwitch(uint8 index, uint8 bOnOff);
extern uint8 GetEpbSwitch(uint8 index);





