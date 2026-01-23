#include "Spi_L9369Data_Types.h"
#include "Spi_ExtEEPROM_Api.h"
#include "Dspi_Api.h"
#include "Port_Api.h"
#include "Main_Api.h"
#include "ElectronicParkBrake.h"
#include "Spi_TC277_Api.h"

extern L9369_RegWrite_t g_L9369_RegWrite;
static L9369_RegWrite_t *g_pL9369_RegWrite;
L9369_FaultStatus_t g_L9369_FaultStatus;
L9369_REG_MISO g_L9369_REG_MISO;
L9369_Adc_Value_t g_L9369_Adc_Value;
L9369_RegVar_t g_L9369_RegRead;
L9369_Status_t g_L9369_Status;
extern Main_t g_MAIN;
extern PbcInDriverBus PbcInDriver; 
extern DW_ElectronicParkBrake_T ElectronicParkBrake_DW;
extern ExtY_ElectronicParkBrake_T ElectronicParkBrake_Y;
extern TC277_t g_TC277;
extern SwitchStatus_t g_SwitchStatus;


const uint8 L9369_WDTxTable[16]={0x0U,0x1U,0x2U,0x3U,0x4U,0x5U,0x6U,0x7U,0x8U,0x9U,0xAU,0xBU,0xCU,0xDU,0xEU,0xFU};
const uint8 L9369_WDRxTable[16]={0x0U,0x1U,0x3U,0x2U,0x6U,0x7U,0x5U,0x4U,0xCU,0xDU,0xFU,0xEU,0xAU,0xBU,0x9U,0x8U};
const uint8 L9369_WDAddrTable[2] = {0x6FU, 0x02U};
const uint8 L9369_CheckAddr1[2] = {0x02U, 0x02U};
const uint8 L9369_CheckAddr2[2] = {0x7EU, 0x02U};
const uint8 L9369_Read_0x88[2] = {0x88U, 0x02U};



const uint8 L9369_Addrs[12][4] = {
    /* Step 0 */ {0x44U, 0x68U, 0x03U, 0x03U},
    /* Step 1 */ {0x4EU, 0x78U, 0xFDU, 0x03U},
    /* Step 2 */ {0x44U, 0x7DU, 0x62U, 0x03U},
    /* Step 3 */ {0x4EU, 0x7FU, 0x12U, 0x03U},
    /* Step 4 */ {0x44U, 0x58U, 0x03U, 0x03U},
    /* Step 5 */ {0x4EU, 0x08U, 0xFDU, 0x03U},
    /* Step 6 */ {0x44U, 0x1CU, 0x26U, 0x03U},
    /* Step 7 */ {0x4EU, 0x30U, 0x3AU, 0x03U},
    /* Step 8 */ {0x44U, 0x69U, 0x03U, 0x03U},
    /* Step 9 */ {0x4EU, 0x79U, 0xFDU, 0x03U},
    /* Step 10 */ {0x44U, 0x7EU, 0x03U, 0x03U},
    /* Step 11 */ {0x4EU, 0x81U, 0x03U, 0x03U},
};

void L9369AdcAvr(void)
{
	uint8 i;
	g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR] = g_L9369_Adc_Value.VSBRIDGE_A[2];
	g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR] = g_L9369_Adc_Value.VSBRIDGE_B[2];
	g_L9369_Adc_Value.SH1_A[L9369_ADC_AVR] = g_L9369_Adc_Value.SH1_A[2];
	g_L9369_Adc_Value.SH1_B[L9369_ADC_AVR] = g_L9369_Adc_Value.SH1_B[2];
	g_L9369_Adc_Value.SH2_A[L9369_ADC_AVR] = g_L9369_Adc_Value.SH2_A[2];
	g_L9369_Adc_Value.SH2_B[L9369_ADC_AVR] = g_L9369_Adc_Value.SH2_B[2];
	g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR] = g_L9369_Adc_Value.CS1_A[2];
	g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR] = g_L9369_Adc_Value.CS1_B[2];
	g_L9369_Adc_Value.CS2_A[L9369_ADC_AVR] = g_L9369_Adc_Value.CS2_A[2];
	g_L9369_Adc_Value.CS2_B[L9369_ADC_AVR] = g_L9369_Adc_Value.CS2_B[2];

}
void L9369Check_5ms(void)
{
	if(L9369Fail_Test1() == FALSE)
	{
		PORT_SUB_L9369_WAU = FALSE;
		Delayms(5);
		PORT_SUB_L9369_WAU = TRUE;
		g_pL9369_RegWrite = L9369RegInit();
		L9369Initialize();
	}
	else{
	}
}

void L9369Check_20ms(void)
{
	if(L9369Fail_Test2() == FALSE)
	{
		g_pL9369_RegWrite = L9369RegInit();
		L9369Initialize();
	}
	else{
	}
}

uint8 L9369Init(uint8 flag)
{
	uint8 StartupCheck = FALSE, state = FALSE, SelfTest = FALSE;
	uint8 step = 0;
	
	step = g_MAIN.L9369_InitStep;

	if(flag == TRUE){
		state = flag;
	}
	else{
		switch(step)
		{
			case 0:
				PORT_SUB_L9369_CTRL = TRUE;
				step++;
				break;
			case 1:
				PORT_SUB_L9369_WAU = TRUE;
				step++;
				break;
			case 2:
				if(g_L9369_RegRead._0x02.field.Revision_ID_SPI == 0x33U)
				{
					step++;// analyis require
				}
				step++;
				break;
			case 3:		
				if(L9369_StartupCheck() == TRUE)
				{
					step++;
				}
				break;
			case 4:
				if(L9369Fail_Test1() == TRUE)
				{
					step++;
				}
				break;
			case 5:
				g_pL9369_RegWrite = L9369RegInit();
				L9369Initialize();
				step++;
				state = TRUE;
				break;
			default:
				step = 0;
				break;
		}
	#if 0	
		if(L9369_StartupCheck() == TRUE)
		{
			if(L9369Fail_Test1() == TRUE)
			{
				g_pL9369_RegWrite = L9369RegInit();
				L9369Initialize();
				state = TRUE;
			}
			else{
				state = FALSE;
			}
		}
		else
		{
			state = FALSE;
		}
#endif
	}
	g_MAIN.L9369_InitStep = step;
	return state;
}
/*	Period 5ms */
void getL9369Switch(void)
{
	uint8 gio2, gio3, gio7;
	
	L9369_Write(L9369SetMsg_0x77()); /* GIO0 ~ 8 pull-up source */
	if (g_pL9369_RegWrite->SwitchMode == 0U) {
		g_pL9369_RegWrite->SwitchMode = 1U;
		L9369_Write(L9369SetMsg_0x87(FALSE, FALSE, gio2, gio3, TRUE, TRUE, FALSE, gio7, TRUE)); /* SW4(GIO0) : H, SW5(GIO1) : H, none(GIO2) : L/H, none(GIO3) : L/H, SW3(GIO4) : H, SW7(GIO5) : H, SW1(GIO6) : H, none(GIO7) : L/H, SW2(GIO8) : H */
		L9369_Read(L9369_Read_0x88,2); /* GIO0, 1, 2, 3, 4, 5, 6, 7, 8 status */
	}
	else 
	{
		g_pL9369_RegWrite->SwitchMode = 0U;
		L9369_Write(L9369SetMsg_0x87(TRUE, TRUE, !gio2, !gio3, TRUE, TRUE, TRUE, !gio7, TRUE)); /* SW4(GIO0) : H, SW5(GIO1) : H, none(GIO2) : L/H, none(GIO3) : L/H, SW3(GIO4) : H, SW7(GIO5) : H, SW1(GIO6) : H, none(GIO7) : L/H, SW2(GIO8) : H */
	}
}
void L9369CheckStatus(uint32 data)
{

	L9369_REG_MISO miso;

	miso.data32 = data;
	switch (miso.field.addr) {
		case 0x02U:
			g_L9369_RegRead._0x02.data = miso.field.data;
			if (g_L9369_FaultStatus.Init_busy_SPI == 0x00U) {
				g_L9369_FaultStatus.Init_busy_SPI = g_L9369_RegRead._0x02.field.Init_busy_SPI;
			}
			if (g_L9369_FaultStatus.Init_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.Init_fail_SPI = g_L9369_RegRead._0x02.field.Init_fail_SPI;
			}
			break;
		case 0x03U:
			g_L9369_RegRead._0x03.data = miso.field.data;
			if (g_L9369_FaultStatus.CRC_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.CRC_fail_SPI = g_L9369_RegRead._0x03.field.CRC_fail_SPI;
			}
			
			if (g_L9369_FaultStatus.ADW_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.ADW_fail_SPI = g_L9369_RegRead._0x03.field.ADW_fail_SPI;
			}
			
			if (g_L9369_FaultStatus.ADR_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.ADR_fail_SPI = g_L9369_RegRead._0x03.field.ADR_fail_SPI;
			}
			
			if (g_L9369_FaultStatus.SPICLK_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.SPICLK_fail_SPI = g_L9369_RegRead._0x03.field.SPICLK_fail_SPI;
			}
			break;
		case 0x04U:
			g_L9369_RegRead._0x04.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_A[0] = (float)(34.2F * (4095.0F - (float)g_L9369_RegRead._0x04.field.VSBRIDGE_A_result1_SPI) / 4096.0F); /* VSB-B (V) = 34.2 * ( 4095 - CODE ) / 4096 */
			break;
		case 0x05U:
			g_L9369_RegRead._0x05.data = miso.field.data;
			break;
		case 0x06U:
			g_L9369_RegRead._0x06.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_A[1] = (float)(34.2F * (4095.0F - (float)g_L9369_RegRead._0x06.field.VSBRIDGE_A_result2_SPI) / 4096.0F); /* VSB-B (V) = 34.2 * ( 4095 - CODE ) / 4096 */
			break;
		case 0x07U:
			g_L9369_RegRead._0x07.data = miso.field.data;
			break;
		case 0x08U:
			g_L9369_RegRead._0x08.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_A[2] = (float)(34.2F * (4095.0F - (float)g_L9369_RegRead._0x08.field.VSBRIDGE_A_result3_SPI) / 4096.0F); /* VSB-B (V) = 34.2 * ( 4095 - CODE ) / 4096 */
			break;
		case 0x09U:
			g_L9369_RegRead._0x09.data = miso.field.data;
			break;
		case 0x0AU:
			g_L9369_RegRead._0x0A.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_A[3] = (float)(34.2F * (4095.0F - (float)g_L9369_RegRead._0x0A.field.VSBRIDGE_A_result4_SPI) / 4096.0F); /* VSB-B (V) = 34.2 * ( 4095 - CODE ) / 4096 */
			break;
		case 0x0BU:
			g_L9369_RegRead._0x0B.data = miso.field.data;
			break;
		case 0x0CU:
			g_L9369_RegRead._0x0C.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_A[4] = (float)(34.2F * (4095.0F - (float)g_L9369_RegRead._0x0C.field.VSBRIDGE_A_result5_SPI) / 4096.0F); /* VSB-B (V) = 34.2 * ( 4095 - CODE ) / 4096 */
			break;
		case 0x0DU:
			g_L9369_RegRead._0x0D.data = miso.field.data;
			break;
		case 0x0EU:
			g_L9369_RegRead._0x0E.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_B[0] = (float)(34.2F * (float)g_L9369_RegRead._0x0E.field.VSBRIDGE_B_result1_SPI / 4096.0F); /* VSB-A (V) = 34.2 * CODE / 4096 */
			break;
		case 0x0FU:
			g_L9369_RegRead._0x0F.data = miso.field.data;
			break;
		case 0x10U:
			g_L9369_RegRead._0x10.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_B[1] = (float)(34.2F * (float)g_L9369_RegRead._0x10.field.VSBRIDGE_B_result2_SPI / 4096.0F); /* VSB-A (V) = 34.2 * CODE / 4096 */
			break;
		case 0x11U:
			g_L9369_RegRead._0x11.data = miso.field.data;
			break;
		case 0x12U:
			g_L9369_RegRead._0x12.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_B[2] = (float)(34.2F * (float)g_L9369_RegRead._0x12.field.VSBRIDGE_B_result3_SPI / 4096.0F); /* VSB-A (V) = 34.2 * CODE / 4096 */
			break;
		case 0x13U:
			g_L9369_RegRead._0x13.data = miso.field.data;
			break;
		case 0x14U:
			g_L9369_RegRead._0x14.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_B[3] = (float)(34.2F * (float)g_L9369_RegRead._0x14.field.VSBRIDGE_B_result4_SPI / 4096.0F); /* VSB-A (V) = 34.2 * CODE / 4096 */
			break;
		case 0x15U:
			g_L9369_RegRead._0x15.data = miso.field.data;
			break;
		case 0x16U:
			g_L9369_RegRead._0x16.data = miso.field.data;
			g_L9369_Adc_Value.VSBRIDGE_B[4] = (float)(34.2F * (float)g_L9369_RegRead._0x16.field.VSBRIDGE_B_result5_SPI / 4096.0F); /* VSB-A (V) = 34.2 * CODE / 4096 */
			break;
		case 0x17U:
			g_L9369_RegRead._0x17.data = miso.field.data;
			break;
		case 0x18U:
			g_L9369_RegRead._0x18.data = miso.field.data;
			g_L9369_Adc_Value.SH1_A[0] = (float)(34.2F * ((float)g_L9369_RegRead._0x18.field.SH1_A_result1_SPI - 4096.0F) / 4096.0F); /* VSH1A (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x19U:
			g_L9369_RegRead._0x19.data = miso.field.data;
			break;
		case 0x1AU:
			g_L9369_RegRead._0x1A.data = miso.field.data;
			g_L9369_Adc_Value.SH1_A[1] = (float)(34.2F * ((float)g_L9369_RegRead._0x1A.field.SH1_A_result2_SPI - 4096.0F) / 4096.0F); /* VSH1A (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x1BU:
			g_L9369_RegRead._0x1B.data = miso.field.data;
			break;
		case 0x1CU:
			g_L9369_RegRead._0x1C.data = miso.field.data;
			g_L9369_Adc_Value.SH1_A[2] = (float)(34.2F * ((float)g_L9369_RegRead._0x1C.field.SH1_A_result3_SPI - 4096.0F) / 4096.0F); /* VSH1A (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x1DU:
			g_L9369_RegRead._0x1D.data = miso.field.data;
			break;
		case 0x1EU:
			g_L9369_RegRead._0x1E.data = miso.field.data;
			g_L9369_Adc_Value.SH1_A[3] = (float)(34.2F * ((float)g_L9369_RegRead._0x1E.field.SH1_A_result4_SPI - 4096.0F) / 4096.0F); /* VSH1A (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x1FU:
			g_L9369_RegRead._0x1F.data = miso.field.data;
			break;
		case 0x20U:
			g_L9369_RegRead._0x20.data = miso.field.data;
			g_L9369_Adc_Value.SH1_A[4] = (float)(34.2F * ((float)g_L9369_RegRead._0x20.field.SH1_A_result5_SPI - 4096.0F) / 4096.0F); /* VSH1A (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x21U:
			g_L9369_RegRead._0x21.data = miso.field.data;
			break;
		case 0x22U:
			g_L9369_RegRead._0x22.data = miso.field.data;
			g_L9369_Adc_Value.SH1_B[0] = (float)(34.2F * ((float)g_L9369_RegRead._0x22.field.SH1_B_result1_SPI - 4096.0F) / 4096.0F); /* VSH1B (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x23U:
			g_L9369_RegRead._0x23.data = miso.field.data;
			break;
		case 0x24U:
			g_L9369_RegRead._0x24.data = miso.field.data;
			g_L9369_Adc_Value.SH1_B[1] = (float)(34.2F * ((float)g_L9369_RegRead._0x24.field.SH1_B_result2_SPI - 4096.0F) / 4096.0F); /* VSH1B (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x25U:
			g_L9369_RegRead._0x25.data = miso.field.data;
			break;
		case 0x26U:
			g_L9369_RegRead._0x26.data = miso.field.data;
			g_L9369_Adc_Value.SH1_B[2] = (float)(34.2F * ((float)g_L9369_RegRead._0x26.field.SH1_B_result3_SPI - 4096.0F) / 4096.0F); /* VSH1B (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x27U:
			g_L9369_RegRead._0x27.data = miso.field.data;
			break;
		case 0x28U:
			g_L9369_RegRead._0x28.data = miso.field.data;
			g_L9369_Adc_Value.SH1_B[3] = (float)(34.2F * ((float)g_L9369_RegRead._0x28.field.SH1_B_result4_SPI - 4096.0F) / 4096.0F); /* VSH1B (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x29U:
			g_L9369_RegRead._0x29.data = miso.field.data;
			break;
		case 0x2AU:
			g_L9369_RegRead._0x2A.data = miso.field.data;
			g_L9369_Adc_Value.SH1_B[4] = (float)(34.2F * ((float)g_L9369_RegRead._0x2A.field.SH1_B_result5_SPI - 4096.0F) / 4096.0F); /* VSH1B (V) = 34.2 * ( CODE - 4096 ) / 4096 */
			break;
		case 0x2BU:
			g_L9369_RegRead._0x2B.data = miso.field.data;
			break;
		case 0x2CU:
			g_L9369_RegRead._0x2C.data = miso.field.data;
			g_L9369_Adc_Value.SH2_A[0] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x2C.field.SH2_A_result1_SPI) - 4096.0F) / 4096.0F); /* VSH2A (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x2DU:
			g_L9369_RegRead._0x2D.data = miso.field.data;
			break;
		case 0x2EU:
			g_L9369_RegRead._0x2E.data = miso.field.data;
			g_L9369_Adc_Value.SH2_A[1] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x2E.field.SH2_A_result2_SPI) - 4096.0F) / 4096.0F); /* VSH2A (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x2FU:
			g_L9369_RegRead._0x2F.data = miso.field.data;
			break;
		case 0x30U:
			g_L9369_RegRead._0x30.data = miso.field.data;
			g_L9369_Adc_Value.SH2_A[2] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x30.field.SH2_A_result3_SPI) - 4096.0F) / 4096.0F); /* VSH2A (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x31U:
			g_L9369_RegRead._0x31.data = miso.field.data;
			break;
		case 0x32U:
			g_L9369_RegRead._0x32.data = miso.field.data;
			g_L9369_Adc_Value.SH2_A[3] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x32.field.SH2_A_result4_SPI) - 4096.0F) / 4096.0F); /* VSH2A (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x33U:
			g_L9369_RegRead._0x33.data = miso.field.data;
			break;
		case 0x34U:
			g_L9369_RegRead._0x34.data = miso.field.data;
			g_L9369_Adc_Value.SH2_A[4] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x34.field.SH2_A_result5_SPI) - 4096.0F) / 4096.0F); /* VSH2A (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x35U:
			g_L9369_RegRead._0x35.data = miso.field.data;
			break;
		case 0x36U:
			g_L9369_RegRead._0x36.data = miso.field.data;
			g_L9369_Adc_Value.SH2_B[0] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x36.field.SH2_B_result1_SPI) - 4096.0F) / 4096.0F); /* VSH2B (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x37U:
			g_L9369_RegRead._0x37.data = miso.field.data;
			break;
		case 0x38U:
			g_L9369_RegRead._0x38.data = miso.field.data;
			g_L9369_Adc_Value.SH2_B[1] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x38.field.SH2_B_result2_SPI) - 4096.0F) / 4096.0F); /* VSH2B (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x39U:
			g_L9369_RegRead._0x39.data = miso.field.data;
			break;
		case 0x3AU:
			g_L9369_RegRead._0x3A.data = miso.field.data;
			g_L9369_Adc_Value.SH2_B[2] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x3A.field.SH2_B_result3_SPI) - 4096.0F) / 4096.0F); /* VSH2B (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x3BU:
			g_L9369_RegRead._0x3B.data = miso.field.data;
			break;
		case 0x3CU:
			g_L9369_RegRead._0x3C.data = miso.field.data;
			g_L9369_Adc_Value.SH2_B[3] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x3C.field.SH2_B_result4_SPI) - 4096.0F) / 4096.0F); /* VSH2B (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x3DU:
			g_L9369_RegRead._0x3D.data = miso.field.data;
			break;
		case 0x3EU:
			g_L9369_RegRead._0x3E.data = miso.field.data;
			g_L9369_Adc_Value.SH2_B[4] = (float)(34.2F * ((8191.0F - (float)g_L9369_RegRead._0x3E.field.SH2_B_result5_SPI) - 4096.0F) / 4096.0F); /* VSH2B (V) = 34.2 * ( ( 8191 - CODE ) - 4096 ) / 4096 */
			break;
		case 0x3FU:
			g_L9369_RegRead._0x3F.data = miso.field.data;
			break;
		case 0x40U:
			g_L9369_RegRead._0x40.data = miso.field.data;
			g_L9369_Adc_Value.CS1_A[0] = (float)(((((float)g_L9369_RegRead._0x40.field.CS1_A_result1_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x41U:
			g_L9369_RegRead._0x41.data = miso.field.data;
			break;
		case 0x42U:
			g_L9369_RegRead._0x42.data = miso.field.data;
			g_L9369_Adc_Value.CS1_A[1] = (float)(((((float)g_L9369_RegRead._0x42.field.CS1_A_result2_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x43U:
			g_L9369_RegRead._0x43.data = miso.field.data;
			break;
		case 0x44U:
			g_L9369_RegRead._0x44.data = miso.field.data;
			g_L9369_Adc_Value.CS1_A[2] = (float)(((((float)g_L9369_RegRead._0x44.field.CS1_A_result3_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x45U:
			g_L9369_RegRead._0x45.data = miso.field.data;
			break;
		case 0x46U:
			g_L9369_RegRead._0x46.data = miso.field.data;
			g_L9369_Adc_Value.CS1_A[3] = (float)(((((float)g_L9369_RegRead._0x46.field.CS1_A_result4_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x47U:
			g_L9369_RegRead._0x47.data = miso.field.data;
			break;
		case 0x48U:
			g_L9369_RegRead._0x48.data = miso.field.data;
			g_L9369_Adc_Value.CS1_A[4] = (float)(((((float)g_L9369_RegRead._0x48.field.CS1_A_result5_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x49U:
			g_L9369_RegRead._0x49.data = miso.field.data;
			break;
		case 0x4AU:
			g_L9369_RegRead._0x4A.data = miso.field.data;
			g_L9369_Adc_Value.CS1_B[0] = (float)(((((float)g_L9369_RegRead._0x4A.field.CS1_B_result1_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x4BU:
			g_L9369_RegRead._0x4B.data = miso.field.data;
			break;
		case 0x4CU:
			g_L9369_RegRead._0x4C.data = miso.field.data;
			g_L9369_Adc_Value.CS1_B[1] = (float)(((((float)g_L9369_RegRead._0x4C.field.CS1_B_result2_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x4DU:
			g_L9369_RegRead._0x4D.data = miso.field.data;
			break;
		case 0x4EU:
			g_L9369_RegRead._0x4E.data = miso.field.data;
			g_L9369_Adc_Value.CS1_B[2] = (float)(((((float)g_L9369_RegRead._0x4E.field.CS1_B_result3_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x4FU:
			g_L9369_RegRead._0x4F.data = miso.field.data;
			break;
		case 0x50U:
			g_L9369_RegRead._0x50.data = miso.field.data;
			g_L9369_Adc_Value.CS1_B[3] = (float)(((((float)g_L9369_RegRead._0x50.field.CS1_B_result4_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x51U:
			g_L9369_RegRead._0x51.data = miso.field.data;
			break;
		case 0x52U:
			g_L9369_RegRead._0x52.data = miso.field.data;
			g_L9369_Adc_Value.CS1_B[4] = (float)(((((float)g_L9369_RegRead._0x52.field.CS1_B_result5_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x53U:
			g_L9369_RegRead._0x53.data = miso.field.data;
			break;
		case 0x54U:
			g_L9369_RegRead._0x54.data = miso.field.data;
			g_L9369_Adc_Value.CS2_A[0] = (float)(((((float)g_L9369_RegRead._0x54.field.CS2_A_result1_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x55U:
			g_L9369_RegRead._0x55.data = miso.field.data;
			break;
		case 0x56U:
			g_L9369_RegRead._0x56.data = miso.field.data;
			g_L9369_Adc_Value.CS2_A[1] = (float)(((((float)g_L9369_RegRead._0x56.field.CS2_A_result2_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x57U:
			g_L9369_RegRead._0x57.data = miso.field.data;
			break;
		case 0x58U:
			g_L9369_RegRead._0x58.data = miso.field.data;
			g_L9369_Adc_Value.CS2_A[2] = (float)(((((float)g_L9369_RegRead._0x58.field.CS2_A_result3_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x59U:
			g_L9369_RegRead._0x59.data = miso.field.data;
			break;
		case 0x5AU:
			g_L9369_RegRead._0x5A.data = miso.field.data;
			g_L9369_Adc_Value.CS2_A[3] = (float)(((((float)g_L9369_RegRead._0x5A.field.CS2_A_result4_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x5BU:
			g_L9369_RegRead._0x5B.data = miso.field.data;
			break;
		case 0x5CU:
			g_L9369_RegRead._0x5C.data = miso.field.data;
			g_L9369_Adc_Value.CS2_A[4] = (float)(((((float)g_L9369_RegRead._0x5C.field.CS2_A_result5_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x5DU:
			g_L9369_RegRead._0x5D.data = miso.field.data;
			break;
		case 0x5EU:
			g_L9369_RegRead._0x5E.data = miso.field.data;
			g_L9369_Adc_Value.CS2_B[0] = (float)(((((float)g_L9369_RegRead._0x5E.field.CS2_B_result1_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x5FU:
			g_L9369_RegRead._0x5F.data = miso.field.data;
			break;
		case 0x60U:
			g_L9369_RegRead._0x60.data = miso.field.data;
			g_L9369_Adc_Value.CS2_B[1] = (float)(((((float)g_L9369_RegRead._0x60.field.CS2_B_result2_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x61U:
			g_L9369_RegRead._0x61.data = miso.field.data;
			break;
		case 0x62U:
			g_L9369_RegRead._0x62.data = miso.field.data;
			g_L9369_Adc_Value.CS2_B[2] = (float)(((((float)g_L9369_RegRead._0x62.field.CS2_B_result3_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x63U:
			g_L9369_RegRead._0x63.data = miso.field.data;
			break;
		case 0x64U:
			g_L9369_RegRead._0x64.data = miso.field.data;
			g_L9369_Adc_Value.CS2_B[3] = (float)(((((float)g_L9369_RegRead._0x64.field.CS2_B_result4_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x65U:
			g_L9369_RegRead._0x65.data = miso.field.data;
			break;
		case 0x66U:
			g_L9369_RegRead._0x66.data = miso.field.data;
			g_L9369_Adc_Value.CS2_B[4] = (float)(((((float)g_L9369_RegRead._0x66.field.CS2_B_result5_SPI / 8192.0F) * 410.0F) - 205.0F) / L9369_Rsense_Ohms); /* I(A) = ((CODE / 8192 * 410mV) - 205 mV) / Rsense(mOhm) */
			break;
		case 0x67U:
			g_L9369_RegRead._0x67.data = miso.field.data;
			break;
		case 0x68U:
			g_L9369_RegRead._0x68.data = miso.field.data;
			break;
		case 0x69U:
			g_L9369_RegRead._0x69.data = miso.field.data;
			break;
		case 0x6AU:
			g_L9369_RegRead._0x6A.data = miso.field.data;
			break;
		case 0x6BU:
			g_L9369_RegRead._0x6B.data = miso.field.data;
			break;
		case 0x6CU:
			g_L9369_RegRead._0x6C.data = miso.field.data;
			break;
		case 0x6DU:
			g_L9369_RegRead._0x6D.data = miso.field.data;
			break;
		case 0x6EU:
			g_L9369_RegRead._0x6E.data = miso.field.data;
			break;
		case 0x6FU:
			g_L9369_RegRead._0x6F.data = miso.field.data;
			break;
		case 0x70U:
			g_L9369_RegRead._0x70.data = miso.field.data;
			break;
		case 0x71U:
			g_L9369_RegRead._0x71.data = miso.field.data;
			break;
		case 0x72U:
			g_L9369_RegRead._0x72.data = miso.field.data;
			break;
		case 0x73U:
			g_L9369_RegRead._0x73.data = miso.field.data;
			break;
		case 0x74U:
			g_L9369_RegRead._0x74.data = miso.field.data;
			break;
		case 0x75U:
			g_L9369_RegRead._0x75.data = miso.field.data;
			break;
		case 0x76U:
			g_L9369_RegRead._0x76.data = miso.field.data;
			break;
		case 0x77U:
			g_L9369_RegRead._0x77.data = miso.field.data;
			break;
		case 0x78U:
			g_L9369_RegRead._0x78.data = miso.field.data;
			if (g_L9369_FaultStatus.OC2_det_B_SPI == 0x00U) {
				g_L9369_FaultStatus.OC2_det_B_SPI = g_L9369_RegRead._0x78.field.OC2_det_B_SPI;
			}
			if (g_L9369_FaultStatus.OC1_det_B_SPI == 0x00U) {
				g_L9369_FaultStatus.OC1_det_B_SPI = g_L9369_RegRead._0x78.field.OC1_det_B_SPI;
			}
			if (g_L9369_FaultStatus.OC2_det_A_SPI == 0x00U) {
				g_L9369_FaultStatus.OC2_det_A_SPI = g_L9369_RegRead._0x78.field.OC2_det_A_SPI;
			}
			if (g_L9369_FaultStatus.OC1_det_A_SPI == 0x00U) {
				g_L9369_FaultStatus.OC1_det_A_SPI = g_L9369_RegRead._0x78.field.OC1_det_A_SPI;
			}
			if (g_L9369_FaultStatus.GLS2_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GLS2_B_fail_SPI = g_L9369_RegRead._0x78.field.GLS2_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.GLS1_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GLS1_B_fail_SPI = g_L9369_RegRead._0x78.field.GLS1_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.GHS2_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GHS2_B_fail_SPI = g_L9369_RegRead._0x78.field.GHS2_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.GHS1_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GHS1_B_fail_SPI = g_L9369_RegRead._0x78.field.GHS1_B_fail_SPI;
			}
			break;
		case 0x79U:
			g_L9369_RegRead._0x79.data = miso.field.data;
			if (g_L9369_FaultStatus.GLS2_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GLS2_A_fail_SPI = g_L9369_RegRead._0x79.field.GLS2_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.GLS1_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GLS1_A_fail_SPI = g_L9369_RegRead._0x79.field.GLS1_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.GHS2_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GHS2_A_fail_SPI = g_L9369_RegRead._0x79.field.GHS2_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.GHS1_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GHS1_A_fail_SPI = g_L9369_RegRead._0x79.field.GHS1_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSLS2_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSLS2_B_fail_SPI = g_L9369_RegRead._0x79.field.DSLS2_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSLS1_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSLS1_B_fail_SPI = g_L9369_RegRead._0x79.field.DSLS1_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSHS2_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSHS2_B_fail_SPI = g_L9369_RegRead._0x79.field.DSHS2_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSHS1_B_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSHS1_B_fail_SPI = g_L9369_RegRead._0x79.field.DSHS1_B_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSLS2_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSLS2_A_fail_SPI = g_L9369_RegRead._0x79.field.DSLS2_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSLS1_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSLS1_A_fail_SPI = g_L9369_RegRead._0x79.field.DSLS1_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSHS2_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSHS2_A_fail_SPI = g_L9369_RegRead._0x79.field.DSHS2_A_fail_SPI;
			}
			if (g_L9369_FaultStatus.DSHS1_A_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.DSHS1_A_fail_SPI = g_L9369_RegRead._0x79.field.DSHS1_A_fail_SPI;
			}
			break;
		case 0x7AU:
			g_L9369_RegRead._0x7A.data = miso.field.data;
			if (g_L9369_FaultStatus.GIO8_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO8_failSCB_SPI = g_L9369_RegRead._0x7A.field.GIO8_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO7_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO7_failSCB_SPI = g_L9369_RegRead._0x7A.field.GIO7_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO6_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO6_failSCB_SPI = g_L9369_RegRead._0x7A.field.GIO6_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO5_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO5_failSCB_SPI = g_L9369_RegRead._0x7A.field.GIO5_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO4_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO4_failSCB_SPI = g_L9369_RegRead._0x7A.field.GIO4_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO3_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO3_failSCB_SPI = g_L9369_RegRead._0x7A.field.GIO3_failSCB_SPI;
			}
			break;
		case 0x7BU:
			g_L9369_RegRead._0x7B.data = miso.field.data;
			if (g_L9369_FaultStatus.GIO2_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO2_failSCB_SPI = g_L9369_RegRead._0x7B.field.GIO2_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO1_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO1_failSCB_SPI = g_L9369_RegRead._0x7B.field.GIO1_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO0_failSCB_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO0_failSCB_SPI = g_L9369_RegRead._0x7B.field.GIO0_failSCB_SPI;
			}
			if (g_L9369_FaultStatus.GIO8_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO8_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO8_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO7_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO7_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO7_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO6_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO6_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO6_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO5_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO5_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO5_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO4_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO4_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO4_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO3_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO3_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO3_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO2_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO2_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO2_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO1_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO1_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO1_failSCG_SPI;
			}
			if (g_L9369_FaultStatus.GIO0_failSCG_SPI == 0x00U) {
				g_L9369_FaultStatus.GIO0_failSCG_SPI = g_L9369_RegRead._0x7B.field.GIO0_failSCG_SPI;
			}
			break;
		case 0x7CU:
			g_L9369_RegRead._0x7C.data = miso.field.data;
			break;
		case 0x7DU:
			g_L9369_RegRead._0x7D.data = miso.field.data;
			break;
		case 0x7EU:
			g_L9369_RegRead._0x7E.data = miso.field.data;
			if (g_L9369_FaultStatus.OTP_CRC_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.OTP_CRC_fail_SPI = g_L9369_RegRead._0x7E.field.OTP_CRC_fail_SPI;
			}
			if (g_L9369_FaultStatus.WD_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.WD_fail_SPI = g_L9369_RegRead._0x7E.field.WD_fail_SPI;
			}
			break;
		case 0x7FU:
			g_L9369_RegRead._0x7F.data = miso.field.data;
			if (g_L9369_FaultStatus.SYNC1_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.SYNC1_fail_SPI = g_L9369_RegRead._0x7F.field.SYNC1_fail_SPI;
			}
			if (g_L9369_FaultStatus.SYNC5_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.SYNC5_fail_SPI = g_L9369_RegRead._0x7F.field.SYNC5_fail_SPI;
			}
			break;
		case 0x80U:
			g_L9369_RegRead._0x80.data = miso.field.data;
			break;
		case 0x81U:
			g_L9369_RegRead._0x81.data = miso.field.data;
			if (g_L9369_FaultStatus.V5V_Uv_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.V5V_Uv_fail_SPI = g_L9369_RegRead._0x81.field.V5V_Uv_fail_SPI;
			}
			if (g_L9369_FaultStatus.GND_loss_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.GND_loss_fail_SPI = g_L9369_RegRead._0x81.field.GND_loss_fail_SPI;
			}
			if (g_L9369_FaultStatus.Vint_Ov_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.Vint_Ov_fail_SPI = g_L9369_RegRead._0x81.field.Vint_Ov_fail_SPI;
			}
			if (g_L9369_FaultStatus.Vint_Uv_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.Vint_Uv_fail_SPI = g_L9369_RegRead._0x81.field.Vint_Uv_fail_SPI;
			}
			if (g_L9369_FaultStatus.OT_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.OT_fail_SPI = g_L9369_RegRead._0x81.field.OT_fail_SPI;
			}
			if (g_L9369_FaultStatus.VBP_Ov_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.VBP_Ov_fail_SPI = g_L9369_RegRead._0x81.field.VBP_Ov_fail_SPI;
			}
			if (g_L9369_FaultStatus.VBP_Ulg_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.VBP_Ulg_fail_SPI = g_L9369_RegRead._0x81.field.VBP_Ulg_fail_SPI;
			}
			if (g_L9369_FaultStatus.VBP_Ust_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.VBP_Ust_fail_SPI = g_L9369_RegRead._0x81.field.VBP_Ust_fail_SPI;
			}
			if (g_L9369_FaultStatus.OSC_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.OSC_fail_SPI = g_L9369_RegRead._0x81.field.OSC_fail_SPI;
			}
			if (g_L9369_FaultStatus.VSCP_low_SPI == 0x00U) {
				g_L9369_FaultStatus.VSCP_low_SPI = g_L9369_RegRead._0x81.field.VSCP_low_SPI;
			}
			if (g_L9369_FaultStatus.CP_fail_SPI == 0x00U) {
				g_L9369_FaultStatus.CP_fail_SPI = g_L9369_RegRead._0x81.field.CP_fail_SPI;
			}
			break;
		case 0x82U:
			g_L9369_RegRead._0x82.data = miso.field.data;
			break;
		case 0x83U:
			g_L9369_RegRead._0x83.data = miso.field.data;
			break;
		case 0x84U:
			g_L9369_RegRead._0x84.data = miso.field.data;
			break;
		case 0x85U:
			g_L9369_RegRead._0x85.data = miso.field.data;
			break;
		case 0x86U:
			g_L9369_RegRead._0x86.data = miso.field.data;
			break;
		case 0x87U:
			g_L9369_RegRead._0x87.data = miso.field.data;
			break;
		case 0x88U:
			g_L9369_RegRead._0x88.data = miso.field.data;
			if (g_L9369_FaultStatus.OSC1_fail == 0x00U) {
				g_L9369_FaultStatus.OSC1_fail = g_L9369_RegRead._0x88.field.OSC1_fail;
			}
			else
			{
			/*nop*/
			}
			if (g_L9369_RegRead._0x88.field.GIO6_status == TRUE) {
				SetEpbSwitch(EPB_SW1, TRUE);
			}
			else {
				SetEpbSwitch(EPB_SW1, FALSE);
			}
			if (g_L9369_RegRead._0x88.field.GIO8_status == TRUE) {
				SetEpbSwitch(EPB_SW2, TRUE);
			}
			else {
				SetEpbSwitch(EPB_SW2, FALSE);
			}
			if (g_L9369_RegRead._0x88.field.GIO4_status == TRUE) {
				SetEpbSwitch(EPB_SW3, TRUE);
			}
			else {
				SetEpbSwitch(EPB_SW3, FALSE);
			}
			if (g_L9369_RegRead._0x88.field.GIO0_status == TRUE) {
				SetEpbSwitch(EPB_SW4, TRUE);
			}
			else {
				SetEpbSwitch(EPB_SW4, FALSE);
			}
			if (g_L9369_RegRead._0x88.field.GIO1_status == TRUE) {
				SetEpbSwitch(EPB_SW5, TRUE);
			}
			else {
				SetEpbSwitch(EPB_SW5, FALSE);
			}
			if (g_L9369_RegRead._0x88.field.GIO5_status == TRUE) {
				SetEpbSwitch(EPB_SW7, TRUE);
			}
			else {
				SetEpbSwitch(EPB_SW7, FALSE);
			}
			break;
		case 0x89U:
			g_L9369_RegRead._0x89.data = miso.field.data;
			break;
		case 0x8AU:
			g_L9369_RegRead._0x8A.data = miso.field.data;
			break;
		case 0x8BU:
			g_L9369_RegRead._0x8B.data = miso.field.data;
			break;
		case 0xCAU:
			g_L9369_RegRead._0xCA.data = miso.field.data;
			break;
		case 0xCBU:
			g_L9369_RegRead._0xCB.data = miso.field.data;
			break;
		case 0xCCU:
			g_L9369_RegRead._0xCC.data = miso.field.data;
			break;
		case 0xCDU:
			g_L9369_RegRead._0xCD.data = miso.field.data;
			break;
		case 0xCEU:
			g_L9369_RegRead._0xCE.data = miso.field.data;
			break;
		case 0xD1U:
			g_L9369_RegRead._0xD1.data = miso.field.data;
			break;
		case 0xD3U:
			g_L9369_RegRead._0xD3.data = miso.field.data;
			break;
		case 0xFDU:
			g_L9369_RegRead._0xFD.data = miso.field.data;
			break;
		default:
			break;
	}
}

void CheckL9369RegStatus(uint8 step) {
    L9369_Read_Period(L9369_Addrs[g_pL9369_RegWrite->Step]);
    if (g_pL9369_RegWrite->Step < 0x0BU) {
    	g_pL9369_RegWrite->Step++;
    } else {
        g_pL9369_RegWrite->Step = 0x00U;
		L9369Check_5ms();
		getL9369Switch();
    }
    if (step == SPI_L9369_RUN) {
    } else {
    }
}

uint8 fMutex_AutoApply = FALSE;
void GetFailSafeStatus(void)
{
	if(g_TC277.fMainfault == TRUE && g_MAIN.MtDrvStatus == 0x01)
	{
		if(!fMutex_AutoApply)
		{
			fMutex_AutoApply = TRUE;
			g_L9369_Status.EpbMotorTestFlag = 3U; //auto apply
		}
		if(((g_L9369_Status.EpbStatusLeft== TestActuatorState_Applied) || (g_L9369_Status.EpbStatusLeft== TestActuatorState_Released)) &&
			((g_L9369_Status.EpbStatusRight== TestActuatorState_Applied) || (g_L9369_Status.EpbStatusRight== TestActuatorState_Released)))
		{
			if (g_SwitchStatus.epb_apply == TRUE)
			{
				g_L9369_Status.EpbMotorTestFlag = 3U;
			}
			else if (g_SwitchStatus.epb_release == TRUE)
			{
				g_L9369_Status.EpbMotorTestFlag = 6U;
			}
			else{
			}
		}
	} 
	else{
		fMutex_AutoApply = FALSE;
	}
}

void EpbMotorDriveControlTest(uint8 dir, uint8 onOff, uint8 isLeft)
{
/*	ONLY Test	*/
	if (onOff == FALSE) {
		if (isLeft == TRUE) {
			g_L9369_RegWrite.PWM_A = 0x00U;
			g_pL9369_RegWrite->HS1_A = FALSE;
			g_pL9369_RegWrite->HS2_A = FALSE;
			g_pL9369_RegWrite->LS1_A = FALSE;
			g_pL9369_RegWrite->LS2_A = FALSE;
			PbcInDriver.MotorDriverStateLeft = 0x03U;
			g_L9369_Status.fActuatorApplying_L= FALSE;
			g_L9369_Status.fActuatorReleasing_L = FALSE;
		}
		else {
			g_L9369_RegWrite.PWM_B = 0x00U;
			g_pL9369_RegWrite->HS1_B = FALSE;
			g_pL9369_RegWrite->HS2_B = FALSE;
			g_pL9369_RegWrite->LS1_B = FALSE;
			g_pL9369_RegWrite->LS2_B = FALSE;
			PbcInDriver.MotorDriverStateRight = 0x03U;
			g_L9369_Status.fActuatorApplying_R= FALSE;
			g_L9369_Status.fActuatorReleasing_R = FALSE;
		}
		g_pL9369_RegWrite->ACT_Valid = TRUE;
	}
	else /* if (onOff == TRUE) */{
		if (dir == L9369_EPB_RELEASE) { /* CCW : RELEASE*/
			if (isLeft == TRUE) {
				g_L9369_RegWrite.PWM_A = 0x7FU;
				g_pL9369_RegWrite->HS1_A = TRUE;
				g_pL9369_RegWrite->HS2_A = FALSE;
				g_pL9369_RegWrite->LS1_A = FALSE;
				g_pL9369_RegWrite->LS2_A = TRUE;
				PbcInDriver.MotorDriverStateLeft = 0x02U;
				g_L9369_Status.fActuatorApplying_L= FALSE;
				g_L9369_Status.fActuatorReleasing_L = TRUE;
			}
			else {
				g_L9369_RegWrite.PWM_B = 0x7FU;
				g_pL9369_RegWrite->HS1_B = FALSE;
				g_pL9369_RegWrite->HS2_B = TRUE;
				g_pL9369_RegWrite->LS1_B = TRUE;
				g_pL9369_RegWrite->LS2_B = FALSE;
				PbcInDriver.MotorDriverStateRight = 0x02U;
				g_L9369_Status.fActuatorApplying_R= FALSE;
				g_L9369_Status.fActuatorReleasing_R = TRUE;
			}
			g_pL9369_RegWrite->ACT_Valid = TRUE;
		}
		else /*if (dir == 0x00U) */{ /* CW : APPLY */
			if (isLeft == TRUE) {
				g_L9369_RegWrite.PWM_A = 0x7FU;
				g_pL9369_RegWrite->HS1_A = FALSE;
				g_pL9369_RegWrite->HS2_A = TRUE;
				g_pL9369_RegWrite->LS1_A = TRUE;
				g_pL9369_RegWrite->LS2_A = FALSE;
				PbcInDriver.MotorDriverStateLeft = 0x01U;
				g_L9369_Status.fActuatorApplying_L= TRUE;
				g_L9369_Status.fActuatorReleasing_L = FALSE;
			}
			else {	
				g_L9369_RegWrite.PWM_B = 0x7FU;
				g_pL9369_RegWrite->HS1_B = TRUE;
				g_pL9369_RegWrite->HS2_B = FALSE;
				g_pL9369_RegWrite->LS1_B = FALSE;
				g_pL9369_RegWrite->LS2_B = TRUE;
				PbcInDriver.MotorDriverStateRight = 0x01U;
				g_L9369_Status.fActuatorApplying_R= TRUE;
				g_L9369_Status.fActuatorReleasing_R = FALSE;
			}
			g_pL9369_RegWrite->ACT_Valid = TRUE;
		}
	}

}


void EpbMotorDriveControlLeft(uint8 dir, uint8 onOff)
{
	if (onOff == FALSE) {
		g_L9369_RegWrite.PWM_A = 0x00U;
		g_pL9369_RegWrite->HS1_A = FALSE;
		g_pL9369_RegWrite->HS2_A = FALSE;
		g_pL9369_RegWrite->LS1_A = FALSE;
		g_pL9369_RegWrite->LS2_A = FALSE;
		PbcInDriver.MotorDriverStateLeft = 0x03U;
		g_L9369_Status.fActuatorApplying_L= FALSE;
		g_L9369_Status.fActuatorReleasing_L = FALSE;
		g_pL9369_RegWrite->ACT_Valid = TRUE;
	}
	else /* if (onOff == TRUE) */{
		if (dir == L9369_EPB_RELEASE) { 
			g_L9369_RegWrite.PWM_A = (uword)((uint16_T)ElectronicParkBrake_Y.PbcOutput.PbcOutMotorDutyLeft / 100) * Period;
			g_pL9369_RegWrite->HS1_A = FALSE;
			g_pL9369_RegWrite->HS2_A = TRUE;
			g_pL9369_RegWrite->LS1_A = TRUE;
			g_pL9369_RegWrite->LS2_A = FALSE;
			PbcInDriver.MotorDriverStateLeft = 0x02U;
			g_L9369_Status.fActuatorApplying_L= FALSE;
			g_L9369_Status.fActuatorReleasing_L = TRUE;
			g_pL9369_RegWrite->ACT_Valid = TRUE;
		}
		else /*if (dir == 0x00U) */{ /* CW : APPLY */
			g_L9369_RegWrite.PWM_A = (uword)((uint16_T)ElectronicParkBrake_Y.PbcOutput.PbcOutMotorDutyLeft / 100) * Period;
			g_pL9369_RegWrite->HS1_A = TRUE;
			g_pL9369_RegWrite->HS2_A = FALSE;
			g_pL9369_RegWrite->LS1_A = FALSE;
			g_pL9369_RegWrite->LS2_A = TRUE;
			PbcInDriver.MotorDriverStateLeft = 0x01U;
			g_L9369_Status.fActuatorApplying_L= TRUE;
			g_L9369_Status.fActuatorReleasing_L = FALSE;
			g_pL9369_RegWrite->ACT_Valid = TRUE;
		}
	}

}
void EpbMotorDriveControlRight(uint8 dir, uint8 onOff)
{
	if (onOff == FALSE) {
		g_L9369_RegWrite.PWM_B = 0x00U;
		g_pL9369_RegWrite->HS1_B = FALSE;
		g_pL9369_RegWrite->HS2_B = FALSE;
		g_pL9369_RegWrite->LS1_B = FALSE;
		g_pL9369_RegWrite->LS2_B = FALSE;
		PbcInDriver.MotorDriverStateRight = 0x03U;
		g_L9369_Status.fActuatorApplying_R= 0;
		g_L9369_Status.fActuatorReleasing_R = 0;
		g_pL9369_RegWrite->ACT_Valid = TRUE;
	}
	else /* if (onOff == TRUE) */{
		if (dir == L9369_EPB_RELEASE) { /* CCW : RELEASE*/
			g_L9369_RegWrite.PWM_B = (uword)((uint16_T)ElectronicParkBrake_Y.PbcOutput.PbcOutMotorDutyRight / 100) * Period;
			g_pL9369_RegWrite->HS1_B = FALSE;
			g_pL9369_RegWrite->HS2_B = TRUE;
			g_pL9369_RegWrite->LS1_B = TRUE;
			g_pL9369_RegWrite->LS2_B = FALSE;
			PbcInDriver.MotorDriverStateRight = 0x02U;
			g_L9369_Status.fActuatorApplying_R= FALSE;
			g_L9369_Status.fActuatorReleasing_R = TRUE;
			g_pL9369_RegWrite->ACT_Valid = TRUE;
		}
		else /*if (dir == 0x00U) */{ /* CW : APPLY */
			g_L9369_RegWrite.PWM_B =(uword)((uint16_T)ElectronicParkBrake_Y.PbcOutput.PbcOutMotorDutyRight / 100) * Period;
			g_pL9369_RegWrite->HS1_B = TRUE;
			g_pL9369_RegWrite->HS2_B = FALSE;
			g_pL9369_RegWrite->LS1_B = FALSE;
			g_pL9369_RegWrite->LS2_B = TRUE;
			PbcInDriver.MotorDriverStateRight = 0x01U;
			g_L9369_Status.fActuatorApplying_R= TRUE;
			g_L9369_Status.fActuatorReleasing_R = FALSE;
			g_pL9369_RegWrite->ACT_Valid = TRUE;
		}
	}
}

void EpbMotorDriveProcess(void)
{
	L9369_Write(L9369SetMsg_0x6A());
	L9369_Write(L9369SetMsg_0x6B()); /* HS1, 2(A, B) gate driver */
	L9369_Write(L9369SetMsg_0x6E()); /* LS1, 2(A, B) gate driver */
	L9369_Write(L9369SetMsg_0x77());
}

void EpbMotorStatusSave(void)
{	
	uint8 ParkApplied = 0x01U, EmergencyApplied = 0x02U;
	//left state save
	if(((g_L9369_Status.fMaxApplied_Left) && (g_MAIN.Session == ExtendedMode)) 
		|| ((ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateLeft == ParkApplied) && (g_MAIN.Session == DefaultMode))
		|| ((ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateLeft == EmergencyApplied) && (g_MAIN.Session == DefaultMode)))
	{
		if(ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateLeft == ParkApplied){
			if(!g_L9369_Status.fmutex_apply_L)
			{
				g_L9369_Status.fmutex_apply_L = TRUE;
				g_L9369_Status.fmutex_release_L = FALSE;
				SPI_EEP_Send_X16(EpbStatusLeft_Add,ParkApplied,EWEN_CMD);
				Delayms(1);
			}
		}
		else if (ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateLeft == EmergencyApplied){
			if(!g_L9369_Status.fmutex_apply_L)
			{
				g_L9369_Status.fmutex_apply_L = TRUE;
				g_L9369_Status.fmutex_release_L = FALSE;
				SPI_EEP_Send_X16(EpbStatusLeft_Add,EmergencyApplied,EWEN_CMD);
				Delayms(1);
			}
		}
	}
	else if (((g_L9369_Status.fMaxReleased_Left) && (g_MAIN.Session == ExtendedMode))
		|| ((ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateLeft == 3) && (g_MAIN.Session == DefaultMode)))
	{
		
		if(!g_L9369_Status.fmutex_release_L)
		{
			g_L9369_Status.fmutex_release_L = TRUE;
			g_L9369_Status.fmutex_apply_L = FALSE;
			SPI_EEP_Send_X16(EpbStatusLeft_Add,0x03U,EWEN_CMD);
			Delayms(1);
		}		
	}
	//right state save
	if(((g_L9369_Status.fMaxApplied_Right) && (g_MAIN.Session == ExtendedMode))
		|| ((ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateRight == ParkApplied) && (g_MAIN.Session == DefaultMode))
		|| ((ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateRight == EmergencyApplied) && (g_MAIN.Session == DefaultMode)))
	{
		if(ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateRight == ParkApplied){
			if(!g_L9369_Status.fmutex_apply_R)
			{
				g_L9369_Status.fmutex_apply_R = TRUE;
				g_L9369_Status.fmutex_release_R = FALSE;
				SPI_EEP_Send_X16(EpbStatusRight_Add,ParkApplied,EWEN_CMD);
				Delayms(1);
			}
		}
		else if (ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateRight == EmergencyApplied){
			if(!g_L9369_Status.fmutex_apply_R)
			{
				g_L9369_Status.fmutex_apply_R = TRUE;
				g_L9369_Status.fmutex_release_R = FALSE;
				SPI_EEP_Send_X16(EpbStatusRight_Add,EmergencyApplied,EWEN_CMD);
				Delayms(1);
			}
		}
	}
	else if (((g_L9369_Status.fMaxReleased_Right) && (g_MAIN.Session == ExtendedMode))
		|| ((ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateRight == 3) && (g_MAIN.Session == DefaultMode)))
	{
		
		if(!g_L9369_Status.fmutex_release_R)
		{
			g_L9369_Status.fmutex_release_R = TRUE;
			g_L9369_Status.fmutex_apply_R = FALSE;
			SPI_EEP_Send_X16(EpbStatusRight_Add,0x03U,EWEN_CMD);
			Delayms(1);
		}		
	}
}
/* 10ms */
void EpbMotorTestLogic(void)
{
	g_L9369_Adc_Value.tCS1_A = EpbMotorCurrent(L9369_LEFT);
	g_L9369_Adc_Value.tCS1_B = EpbMotorCurrent(L9369_RIGHT);
	switch(g_L9369_Status.EpbMotorTestFlag) {
		case 0:
			EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
			EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
			g_L9369_Status.Cnt_Limit_Apply = 0;
			g_L9369_Status.Cnt_Limit_Release = 0;
			break;
		case 1: // left apply command
			if(++g_L9369_Status.Cnt_Limit_Apply < TIME_3S)// 3s
			{
				if(g_L9369_Status.Cnt_Limit_Apply < TIME_500MS)// 0.5s
				{
					g_L9369_Adc_Value.tCS1_A = 0; //inrush current
				};
				if(!g_L9369_Status.fMaxApplied_Left)
				{
					if(g_L9369_Adc_Value.tCS1_A > g_L9369_Status.Test_Cur_Limit)
					{
						//max applied
						g_L9369_Status.fMaxApplied_Left = TRUE;
						g_L9369_Status.fMaxReleased_Left = FALSE;
						g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
						EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
						g_L9369_Status.EpbMotorTestFlag = 0U;
					}
					else 
					{
						g_L9369_Status.fMaxReleased_Left = FALSE;
						g_L9369_Status.EpbStatusLeft = TestActuatorState_Applying;
						EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_LEFT); // left ,apply 
					}
				}
				else	// past applied 
				{
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
					g_L9369_Status.EpbMotorTestFlag = 0U;
				}
			}
			else	//time out applied
			{
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;			
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
			break;
		case 2: // right apply command
			if(++g_L9369_Status.Cnt_Limit_Apply < TIME_3S)// 3s
			{
				if(g_L9369_Status.Cnt_Limit_Apply < TIME_500MS)// 0.5s
				{
					g_L9369_Adc_Value.tCS1_B = 0; //inrush current
				};
				
				if(!g_L9369_Status.fMaxApplied_Right)
				{
					if(g_L9369_Adc_Value.tCS1_B > g_L9369_Status.Test_Cur_Limit)
					{
						//max applied
						g_L9369_Status.fMaxApplied_Right = TRUE;
						g_L9369_Status.fMaxReleased_Right = FALSE;
						g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;
						EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT); // right ,neutral 
						g_L9369_Status.EpbMotorTestFlag = 0U;
					}
					else 
					{
						g_L9369_Status.fMaxReleased_Right = FALSE;
						g_L9369_Status.EpbStatusRight = TestActuatorState_Applying;
						EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_RIGHT); // right ,apply 
					}
				}
				else	// past applied 
				{
					g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT); // right ,neutral 
					g_L9369_Status.EpbMotorTestFlag = 0U;
				}
			}
			else	//time out applied
			{
				g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;			
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT); // right ,neutral 
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
			break;
		case 3: // both apply command
			if(++g_L9369_Status.Cnt_Limit_Apply < TIME_3S)// 3s
			{
				if(g_L9369_Status.Cnt_Limit_Apply < TIME_500MS)// 0.5s
				{
					g_L9369_Adc_Value.tCS1_A = 0;
					g_L9369_Adc_Value.tCS1_B = 0;  
				};
				
				if((!g_L9369_Status.fMaxApplied_Left)&&(!g_L9369_Status.fMaxApplied_Right))
				{
					if(g_L9369_Adc_Value.tCS1_A > g_L9369_Status.Test_Cur_Limit)
					{
						g_L9369_Status.fMaxApplied_Left = TRUE;
						g_L9369_Status.fMaxReleased_Left = FALSE;
						g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
						EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
					}
					else
					{
						g_L9369_Status.fMaxReleased_Left = FALSE;
						g_L9369_Status.EpbStatusLeft = TestActuatorState_Applying;
						EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_LEFT); // left ,apply 
					}
					
					if(g_L9369_Adc_Value.tCS1_B > g_L9369_Status.Test_Cur_Limit)
					{
						g_L9369_Status.fMaxApplied_Right = TRUE;
						g_L9369_Status.fMaxReleased_Right = FALSE;
						g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;	
						EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
					}
					else
					{
						g_L9369_Status.fMaxReleased_Right = FALSE;
						g_L9369_Status.EpbStatusRight = TestActuatorState_Applying;
						EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_RIGHT);// right ,apply
					}
				}
				else
				{
					if(g_L9369_Status.fMaxApplied_Left == 0)
					{
						if(g_L9369_Adc_Value.tCS1_A > g_L9369_Status.Test_Cur_Limit)
						{
							g_L9369_Status.fMaxApplied_Left = TRUE;
							g_L9369_Status.fMaxReleased_Left = FALSE;
							g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
							EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
						}
						else
						{
							g_L9369_Status.fMaxReleased_Left = FALSE;
							g_L9369_Status.EpbStatusLeft = TestActuatorState_Applying;
							EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_LEFT); // left ,apply 
							
						}
					}
					
					if(g_L9369_Status.fMaxApplied_Right == 0)
					{
						if(g_L9369_Adc_Value.tCS1_B >g_L9369_Status.Test_Cur_Limit)
						{
							g_L9369_Status.fMaxApplied_Right = TRUE;
							g_L9369_Status.fMaxReleased_Right = FALSE;
							g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;
							EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
						}
						else
						{	
							g_L9369_Status.fMaxReleased_Right = FALSE;
							g_L9369_Status.EpbStatusRight = TestActuatorState_Applying;
							EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_RIGHT);// right ,apply	
						}		
					}
					
					if((g_L9369_Status.fMaxApplied_Left == TRUE)&&(g_L9369_Status.fMaxApplied_Right == TRUE))
					{
						g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
						g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;
						EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
						EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
						g_L9369_Status.EpbMotorTestFlag = 0U;
					}
				}
			}
			else
			{
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
				g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;				
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
			
			break;
		case 4:// left release command
			if(++g_L9369_Status.Cnt_Limit_Release < TIME_1500MS)
			{
				if(!g_L9369_Status.fMaxReleased_Left)
				{ 
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Releasing;
					EpbMotorDriveControlTest(L9369_RELEASE, TRUE, L9369_LEFT); //left, release
				}
				else
				{
					g_L9369_Status.fMaxReleased_Left = TRUE;
					g_L9369_Status.fMaxApplied_Left = FALSE;
					g_L9369_Status.fMaxApplied_Right = FALSE; 
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Released;
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT);// left ,neutral 
					g_L9369_Status.EpbMotorTestFlag = 0U;
				}
			}
			else
			{
				g_L9369_Status.fMaxReleased_Left = TRUE;
				g_L9369_Status.fMaxApplied_Left = FALSE;
				g_L9369_Status.fMaxApplied_Right = FALSE; 
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Released;
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT);// left ,neutral 
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
		break;
		case 5:// right release command
			if(++g_L9369_Status.Cnt_Limit_Release < TIME_1500MS) 
			{
				if(!g_L9369_Status.fMaxReleased_Right) //1s
				{
					g_L9369_Status.EpbStatusRight = TestActuatorState_Releasing;
					EpbMotorDriveControlTest(L9369_RELEASE, TRUE, L9369_RIGHT); 
				}
				else
				{
					g_L9369_Status.fMaxReleased_Right = TRUE;
					g_L9369_Status.fMaxApplied_Left = FALSE;
					g_L9369_Status.fMaxApplied_Right = FALSE; 
					g_L9369_Status.EpbStatusRight = TestActuatorState_Released;
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);
					g_L9369_Status.EpbMotorTestFlag = 0U;
				}
			}
			else
			{
				g_L9369_Status.fMaxReleased_Right = TRUE;
				g_L9369_Status.fMaxApplied_Left = FALSE;
				g_L9369_Status.fMaxApplied_Right = FALSE; 
				g_L9369_Status.EpbStatusRight = TestActuatorState_Released;
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
			break;	
		case 6:// both release command
			if(++g_L9369_Status.Cnt_Limit_Release < TIME_1500MS) 
			{
				if((!g_L9369_Status.fMaxReleased_Left)&&(!g_L9369_Status.fMaxReleased_Right)) //1s
				{
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Releasing;
					g_L9369_Status.EpbStatusRight = TestActuatorState_Releasing;
					EpbMotorDriveControlTest(L9369_RELEASE, TRUE, L9369_LEFT); //left, release
					EpbMotorDriveControlTest(L9369_RELEASE, TRUE, L9369_RIGHT);// right ,release
				}
				else
				{
					g_L9369_Status.fMaxReleased_Left = TRUE;
					g_L9369_Status.fMaxReleased_Right = TRUE;
					g_L9369_Status.fMaxApplied_Left = FALSE;
					g_L9369_Status.fMaxApplied_Right = FALSE; 
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Released;
					g_L9369_Status.EpbStatusRight = TestActuatorState_Released;
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT);// left ,neutral 
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
					g_L9369_Status.EpbMotorTestFlag = 0U;
				}
			}
			else
			{
				g_L9369_Status.fMaxReleased_Left = TRUE;
				g_L9369_Status.fMaxReleased_Right = TRUE;
				g_L9369_Status.fMaxApplied_Left = FALSE;
				g_L9369_Status.fMaxApplied_Right = FALSE; 
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Released;
				g_L9369_Status.EpbStatusRight = TestActuatorState_Released;
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT);// left ,neutral 
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}	
			break;
		
		case 8: //force apply
			if(++g_L9369_Status.Cnt_Limit_Apply < TIME_3S)// 3s
			{
				if(g_L9369_Status.Cnt_Limit_Apply < TIME_500MS)// 0.5s
				{
					g_L9369_Adc_Value.tCS1_A = 0;
					g_L9369_Adc_Value.tCS1_B = 0;  
				};
				
				if(g_L9369_Adc_Value.tCS1_A > g_L9369_Status.Test_Cur_Limit)
				{
					g_L9369_Status.fMaxApplied_Left = TRUE;
					g_L9369_Status.fMaxReleased_Left = FALSE;
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
					
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
				}
				else
				{
					g_L9369_Status.fMaxReleased_Left = FALSE;
					g_L9369_Status.EpbStatusLeft = TestActuatorState_Applying;
					EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_LEFT); // left ,apply 
				}
				
				if(g_L9369_Adc_Value.tCS1_B > g_L9369_Status.Test_Cur_Limit)
				{
					g_L9369_Status.fMaxApplied_Right = TRUE;
					g_L9369_Status.fMaxReleased_Right = FALSE;
					g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;
					
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
				}
				
				else
				{
					g_L9369_Status.fMaxReleased_Right = FALSE;
					g_L9369_Status.EpbStatusRight = TestActuatorState_Applying;
					EpbMotorDriveControlTest(L9369_APPLY, TRUE, L9369_RIGHT);// right ,apply
				}
			
			
				if((g_L9369_Status.fMaxApplied_Left)&&(g_L9369_Status.fMaxApplied_Right))
				{
					g_L9369_Status.EpbStatusLeft= TestActuatorState_Applied;
					g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
					EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
					g_L9369_Status.EpbMotorTestFlag = 0U;
				}
			}
			else
			{
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Applied;
				g_L9369_Status.EpbStatusRight = TestActuatorState_Applied;				
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT); // left ,neutral 
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
		break;	
		
		case 9://force release
			if(++g_L9369_Status.Cnt_Limit_Release < TIME_1500MS)
			{
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Releasing;
				g_L9369_Status.EpbStatusRight = TestActuatorState_Releasing;
				EpbMotorDriveControlTest(L9369_RELEASE, TRUE, L9369_LEFT); //left, release
				EpbMotorDriveControlTest(L9369_RELEASE, TRUE, L9369_RIGHT);// right ,release
				
			}
			else
			{
				g_L9369_Status.fMaxReleased_Left = TRUE;
				g_L9369_Status.fMaxReleased_Right = TRUE;
				g_L9369_Status.fMaxApplied_Left = FALSE;
				g_L9369_Status.fMaxApplied_Right = FALSE; 
				g_L9369_Status.EpbStatusLeft = TestActuatorState_Released;
				g_L9369_Status.EpbStatusRight = TestActuatorState_Released;
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT);// left ,neutral 
				EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
				g_L9369_Status.EpbMotorTestFlag = 0U;
			}
		break;
				
		default:
			EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_LEFT);// left ,neutral 
			EpbMotorDriveControlTest(L9369_NEUTRAL, FALSE, L9369_RIGHT);// right ,neutral
			g_L9369_Status.EpbStatusLeft = TestActuatorState_Neutral;
			g_L9369_Status.EpbStatusRight = TestActuatorState_Neutral;
			break;
	}
}


void CheckL9369Watchdog(void)
{
	if (g_L9369_RegWrite.Init == TRUE) {
        L9369Check_20ms(); 
		L9369_Read(L9369_WDAddrTable,2);
		L9369_Write(L9369SetMsg_0x6F());
		if((uint8)g_L9369_RegRead._0x6F.field.SPI_com_WD == L9369_WDRxTable[g_L9369_RegWrite.WD_Cmd])
		{
			g_L9369_RegWrite.WD_Cmd++;
		}
		if(g_L9369_RegWrite.WD_Cmd == 0x10)
		{
			g_L9369_RegWrite.WD_Cmd=0x0U;
		}
	}
}
uint8 L9369Fail_Test1(void)
{
	uint8 Init_fail_SPI = 0,SPI_ERR = 0;
	uint8 state = FALSE;
	
	L9369_Read(L9369_CheckAddr1, 2);
	Init_fail_SPI = g_L9369_RegRead._0x02.field.Init_fail_SPI;
	SPI_ERR = g_L9369_REG_MISO.field.spi_err;
	
	if(Init_fail_SPI|SPI_ERR)
	{
		state = FALSE;
	}
	else{
		state = TRUE; //PASS
	}
	
	return state;
}

uint8 L9369Fail_Test2(void)
{
	uint8 WD_fail_SPI = 0;
	uint8 state = FALSE;
	
	L9369_Read(L9369_CheckAddr2, 2);
	WD_fail_SPI = g_L9369_RegRead._0x7E.field.WD_fail_SPI;
	
	if(WD_fail_SPI)
	{	
		L9369_Write(L9369SetMsg_0x69(0x02));
		state = FALSE;
	}
	else{
		state = TRUE; //PASS
	}
	
	return state;
}

void L9369Initialize(void)
{
	
	//L9369_ReadWrite(L9369GetMsg(0x02U)); /* SPI status */
	L9369_Write(L9369SetMsg_0x8A()); /* LS configuration for lamp driver */
	L9369_Write(L9369SetMsg_0xD3()); /* Lamp Driver : HSOUT0, 1 */
	L9369_Write(L9369SetMsg_0x70()); /* Lamp Driver HSOUT0 Voltage */
	L9369_Write(L9369SetMsg_0x71()); /* Lamp Driver HSOUT1 Voltage */
	L9369_Write(L9369SetMsg_0x72(0x0FU, 0x0FU, 0x0FU)); /* GIO3 : 0b1111, GIO4 : 0b1111, GIO5 : 0b1111 => Mux Control */
	L9369_Write(L9369SetMsg_0x73(0x0FU, 0x0FU, 0x0FU)); /* GIO0 : 0b1111, GIO1 : 0b1111, GIO2 : 0b1111 => Mux Control */
	L9369_Write(L9369SetMsg_0x75(0x0FU, 0x0FU, 0x0FU)); /* GIO6 : 0b1111, GIO7 : 0b1111, GIO8 : 0b1111 => Mux Control */

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

	L9369_Write(L9369SetMsg_0x74()); /* GPIO0, 1, 2, 3 Mux Control */
	L9369_Write(L9369SetMsg_0x76()); /* GIO0, 1, 2, 3, 4, 5 Wakeup Control */
	L9369_Write(L9369SetMsg_0x86()); /* GPIO0, 1, 2, 3 Driving Control */	
	L9369_Write(L9369SetMsg_0x77()); /* GIO0 ~ 8 pull-up source */
	L9369_Write(L9369SetMsg_0x87(TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE)); /* SW4(GIO0) : H, SW5(GIO1) : H, none(GIO2) : L/H, none(GIO3) : L/H, SW3(GIO4) : H, SW7(GIO5) : H, SW1(GIO6) : H, none(GIO7) : L/H, SW2(GIO8) : H */
	L9369_Write(L9369SetMsg_0x6A()); /* HS PWM Set */
	L9369_Write(L9369SetMsg_0x6B()); /* HS1, 2(A, B) gate driver */
	L9369_Write(L9369SetMsg_0x6C()); /* H-bridge stage B reference voltage enabled */
	L9369_Write(L9369SetMsg_0x69(0x02U)); /* HS enable */
	L9369_Write(L9369SetMsg_0x6D());
	L9369_Write(L9369SetMsg_0x6E()); /* LS1, 2(A, B) gate driver */
	L9369_Write(L9369SetMsg_0xCE()); /* ADC Enable */
	L9369_Write(L9369GetMsg(0x81U)); /* Fail status */
	L9369_Write(L9369GetMsg(0x03U)); /* SPI status */
	L9369_Write(L9369GetMsg(0x7EU)); /* SPI status */

	g_pL9369_RegWrite->Init = TRUE;
	g_pL9369_RegWrite->WD_Cmd = 0x00U;

}

void L9369PowerOff(void)
{
	EpbMotorDriveControlTest(0x00U, FALSE, FALSE);
	EpbMotorDriveControlLeft(0x00U, FALSE);
	EpbMotorDriveControlRight(0x00U, FALSE);
	EpbMotorDriveProcess();
	g_pL9369_RegWrite->GIO0 = TRUE;/* SW4(GIO0) : H */
	g_pL9369_RegWrite->GIO1 = FALSE; /* SW5(GIO1) : L */
	g_pL9369_RegWrite->GIO2 = FALSE; /* none(GIO2) : L */
	g_pL9369_RegWrite->GIO3 = FALSE; /* none(GIO3) : L */
	g_pL9369_RegWrite->GIO4 = FALSE; /* SW3(GIO4) : L */
	g_pL9369_RegWrite->GIO5 = FALSE; /* SW7(GIO5) : L */
	g_pL9369_RegWrite->GIO6 = FALSE; /* SW2(GIO6) : L */
	g_pL9369_RegWrite->GIO7 = FALSE; /* none(GIO7) : L */
	g_pL9369_RegWrite->GIO8 = FALSE; /* SW1(GIO8): L */
	g_L9369_RegWrite.ACT_Valid = TRUE;
	L9369_Write(L9369SetMsg_0x77());
	PORT_SUB_L9369_WAU = FALSE; // The L9369 go to sleep 


}


float EpbMotorCurrent(uint8 isLeft)
{
	float value;

	if (isLeft == TRUE) {
		value = (float)(g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR]); //high side current sensing
	}
	else {
		value = (float)(g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR]);
	}

	if(value < 0){
		value = 0;
	}

	return value;
}

float EpbMotorVoltage(uint8 isLeft)
{
	float value;

	if (isLeft == TRUE) {
		value = (float)(g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR]);
	}
	else {
		value = (float)(g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR]);
	}

	return value;
}

float EpbMotorSHVoltageA(uint8 input)
{
	float value;

	if (input == TRUE) {
		value = (float)(g_L9369_Adc_Value.SH1_A[L9369_ADC_AVR]);
	}
	else {
		value = (float)(g_L9369_Adc_Value.SH2_A[L9369_ADC_AVR]);
	}

	return value;
}

float EpbMotorSHVoltageB(uint8 input)
{
	float value;

	if (input == TRUE) {
		value = (float)(g_L9369_Adc_Value.SH1_B[L9369_ADC_AVR]);
	}
	else {
		value = (float)(g_L9369_Adc_Value.SH2_B[L9369_ADC_AVR]);
	}

	return value;
}

void SetWarningLampDriver(uint8 hsout0, uint8 hsout1)
{
	g_pL9369_RegWrite->HSOUT0 = hsout0;
	g_pL9369_RegWrite->HSOUT1 = hsout1;
}

uint16 EpbAdcValue(uint8 index)
{
	uint16 value = 0U;

	switch (index) {
		case 0U:
			value = (uint16)(g_L9369_Adc_Value.SH1_A[L9369_ADC_AVR] * 100.0F);
			break;
		case 1U:
			value = (uint16)(g_L9369_Adc_Value.SH1_B[L9369_ADC_AVR] * 100.0F);
			break;
		case 2U:
			value = (uint16)(g_L9369_Adc_Value.SH2_A[L9369_ADC_AVR] * 100.0F);
			break;
		case 3U:
			value = (uint16)(g_L9369_Adc_Value.SH2_B[L9369_ADC_AVR] * 100.0F);
			break;
		case 4U:
			value = (uint16)(g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR] * 100.0F);
			break;
		case 5U:
			value = (uint16)(g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR] * 100.0F);
			break;
		case 6U:
			value = (uint16)(g_L9369_Adc_Value.CS2_A[L9369_ADC_AVR] * 100.0F);
			break;
		case 7U:
			value = (uint16)(g_L9369_Adc_Value.CS2_B[L9369_ADC_AVR] * 100.0F);
			break;
		case 8U:
			value = (uint16)(g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR] * 100.0F);
			break;
		case 9U:
			value = (uint16)(g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR] * 100.0F);
			break;
		default:
			break;
	}

	return value;
}

void L9369_Sync_1ms(void)
{
	g_L9369_RegWrite.Sync_1ms=!g_L9369_RegWrite.Sync_1ms;
	PORT_SUB_L9369_SYNC1 = g_L9369_RegWrite.Sync_1ms;
	
	
}
void L9369_Sync_5ms(void)
{
	PORT_SUB_L9369_SYNC5 = g_L9369_RegWrite.Sync_5ms;
	g_L9369_RegWrite.Sync_5ms=!g_L9369_RegWrite.Sync_5ms;
}
uint8 L9369_StartupCheck(void)
{
	uint8 Init_busy_SPI = 0, state;
/*	During initialization phase the application SW must check the fall edge transition of Init_busy_SPI bit 
	before proceeding with any other operation.	*/
	
	L9369_Read(L9369_CheckAddr1,2);
	Init_busy_SPI = g_L9369_RegRead._0x02.field.Init_busy_SPI;
	if(Init_busy_SPI != 0){
		state = FALSE;
	}else{
		state = TRUE; // PASS
	}
	return state;
	
}
uint8 GetEpbWakeupStatus(void)
{
	return (uint8)g_L9369_RegRead._0x8A.field.SYS_WAKE_IN_status;
}
void L9369_TASK_1ms(void)
{
	if(g_MAIN.SystemStatus == SYSTEM_STATUS_NORMAL)
	{
		L9369_Sync_1ms();
		g_L9369_RegWrite.SyncStep++;
		if(g_L9369_RegWrite.SyncStep>=5){
			g_L9369_RegWrite.SyncStep=0;
			L9369_Sync_5ms();
		}
	}	
}

