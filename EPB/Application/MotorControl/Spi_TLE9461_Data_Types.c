
#include "Spi_TLE9461_Data_Types.h"
#include "SWspi.h"
#include "Port_Api.h"


CANSBC_REG_MOSI g_CANSBC_REG_MOSI;
CANSBC_REG_MISO g_CANSBC_REG_MISO;
volatile CANSBC_RegVar_t g_CANSBC_RegRead;
CANSBC_RegWrite_t g_CANSBC_RegWrite;
CANSBC_FaultStatus_t g_CANSBC_FaultStatus;
CANSBC_Status_t g_CANSBC_Status;

/*

iEHB ASIC Configuration v0.0 ÂüÁ¶ 

*/
	
uint16_t CANSBCGetMsg(uint8 addr)
{
	uint32_t data = 0x00U;
	
	g_CANSBC_REG_MOSI.data16 = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x00U; 
	g_CANSBC_REG_MOSI.field.addr = addr; 
	g_CANSBC_REG_MOSI.field.data = 0x00U;
	data = g_CANSBC_REG_MOSI.data16;
	
	return data;
}


uint16_t CANSBCSetMsg_0x01(uint8 Mode)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x01_M_S_CTRL value;

	value.data = 0x00U;
	value.field.MODE = Mode;
	value.field.VCC2_ON = g_CANSBC_RegWrite.VCC2_ON;
	value.field.VCC1_OV_RST = 0x00U;
	value.field.VCC1_RT = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x01U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}

uint16_t CANSBCSetMsg_0x02(uint8 Mode)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x02_HW_CTRL_0 value;

	value.data = 0x00U;
	value.field.SOFT_RESET_RST = 0x00U;
	value.field.FO_ON = 0x00U;
	value.field.CP_EN = Mode;
	value.field.CFG1 = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x02U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x03(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x03_WD_CTRL value;
	
	value.data = 0x00U;
	
	value.field.CHECKSUM = g_CANSBC_RegWrite.CheckSum;
	value.field.WD_STM_EN_0 = g_CANSBC_RegWrite.WD_STM_EN_0;
	value.field.WD_WIN = 0x01U;
	value.field.WD_EN_WK_BUS = 0x00U;
	value.field.WD_TIMER = 0x02U;


	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x03U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x04(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x04_BUS_CTRL_0 value;

	value.data = 0x00U;
	value.field.CAN = g_CANSBC_RegWrite.CAN;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x04U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x06(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x06_WK_CTRL_0 value;

	value.data = 0x00U;
	value.field.TIMER_WK_EN = 0x00U;
	value.field.WD_STM_EN_1 = g_CANSBC_RegWrite.WD_STM_EN_1;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x06U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x07(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x07_WK_CTRL_1 value;

	value.data = 0x00U;
	value.field.INT_GLOBAL = 0x00U;
	value.field.WK_MEAS = 0x00U;
	value.field.WK_EN = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x07U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x08(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x08_WK_PUPD_CTRL value;

	value.data = 0x00U;
	value.field.GPIO_WK_PUPD = 0x03U; 
	value.field.WK_PUPD = 0x03U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x08U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x0B(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x0B_BUS_CTRL_3 value;

	value.data = 0x00U;
	value.field.CAN_Flash = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x0BU; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x0C(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x0C_TIMER_CTRL value;

	value.data = 0x00U;
	value.field.TIMER_ON = 0x00U;
	value.field.TIMER_PER = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x0CU; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x0E(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x0E_HW_CTRL_1 value;

	value.data = 0x00U;
	value.field.RSTN_HYS = 0x00U;
	value.field.TSD2_DEL = 0x00U;
	value.field.RSTN_DEL = 0x00U;
	//value.field.CFG_LOCK_0 = 

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x0EU; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}

uint16_t CANSBCSetMsg_0x0F(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x0F_HW_CTRL_2 value;

	value.data = 0x00U;
	value.field._2MHZ_FREQ = 0x02U;
	value.field.I_PEAK_TH = 0x00U;
	value.field.SS_MOD_FR = 0x00U;
	//value.field.CFG_LOCK_1 = 

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x0FU; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}

uint16_t CANSBCSetMsg_0x1D(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x1D_HW_CTRL_3 value;

	value.data = 0x00U;
	value.field.TSD_THR = 0x00U;
	value.field.ICC1_LIM_ADJ = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x1DU; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}


uint16_t CANSBCSetMsg_0x41(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x41_SUP_STAT_0 value;

	value.data = 0x00U;
	value.field.POR = 0x00U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x41U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}
uint16_t CANSBCSetMsg_0x46(void)
{
	uint16_t data;
	
	CANSBC_REG_DATA_0x46_WK_STAT_0 value;

	value.data = 0x00U;
	value.field.CAN_WU = 0x01U;

	g_CANSBC_REG_MOSI.field.r_w = 0x01U; 
	g_CANSBC_REG_MOSI.field.addr = 0x46U; 
	g_CANSBC_REG_MOSI.field.data = (uint16)value.data;
	data = g_CANSBC_REG_MOSI.data16;

	return data;
}

CANSBC_Status_t *CANSBCStatusRegInit(void )
{
	g_CANSBC_Status.CAN_WU = 0x00U;		
	g_CANSBC_Status.TIMER_WU = 0x00U;	
	g_CANSBC_Status.WK_WU = 0x00U;		
	g_CANSBC_Status.GPIO_WK_WU = 0x00U;	
	g_CANSBC_Status.SBC_DEV_LVL = 0x00U;
	g_CANSBC_Status.CFG0_STATE = 0x00U;	
	g_CANSBC_Status.GPIO_LVL = 0x00U;	
	g_CANSBC_Status.WK_LVL = 0x00U;		

	return &g_CANSBC_Status;
}

CANSBC_FaultStatus_t *CANSBCFaultRegInit(void )
{
	g_CANSBC_FaultStatus.VS_UV = 0x00U;
	g_CANSBC_FaultStatus.VS_OV = 0x00U;		
	g_CANSBC_FaultStatus.VCC1_OV = 0x00U;	
	g_CANSBC_FaultStatus.VCC1_WARN = 0x00U;	
	g_CANSBC_FaultStatus.POR = 0x00U;		
	g_CANSBC_FaultStatus.VCC2_OT = 0x00U;	
	g_CANSBC_FaultStatus.VCC2_UV = 0x00U;	
	g_CANSBC_FaultStatus.VCC1_SC = 0x00U;	
	g_CANSBC_FaultStatus.VCC1_UV = 0x00U;	
	g_CANSBC_FaultStatus.TSD2_SAFE = 0x00U;	
	g_CANSBC_FaultStatus.TSD2 = 0x00U;		
	g_CANSBC_FaultStatus.TSD1 = 0x00U;		
	g_CANSBC_FaultStatus.TPW = 0x00U;		
	g_CANSBC_FaultStatus.DEV_STAT = 0x00U;	
	g_CANSBC_FaultStatus.WD_FAIL = 0x00U;	
	g_CANSBC_FaultStatus.SPI_FAIL = 0x00U;	
	g_CANSBC_FaultStatus.FAILURE = 0x00U;	
	g_CANSBC_FaultStatus.CAN_FAIL = 0x00U;	
	g_CANSBC_FaultStatus.VCAN_UV = 0x00U;	
	g_CANSBC_FaultStatus.GPIO_OC = 0x00U;	
	g_CANSBC_FaultStatus.GPIO_OL = 0x00U;	

	return &g_CANSBC_FaultStatus;
}

CANSBC_RegWrite_t *CANSBCRegInit(void )
{
	
	g_CANSBC_RegWrite.Step = 0x00U;
	g_CANSBC_RegWrite.CheckSum = 0x00U;
	g_CANSBC_RegWrite.VCC2_ON = 0x01U;
	g_CANSBC_RegWrite.fFaultClear = 0x01U;
	g_CANSBC_RegWrite.WD_STM_EN_0 = 0x00U;
	g_CANSBC_RegWrite.WD_STM_EN_1 = 0x00U;
	g_CANSBC_RegWrite.fCANSBCDisableWatchdogStep = 0x00U;
	g_CANSBC_RegWrite.CAN = 0x03U;

	return &g_CANSBC_RegWrite;
}

uint16_t reverseBits16(uint16_t value) {
    uint16_t  reversed = 0;
    uint8_t i = 0;
    for (i = 0; i < 16; i++) {
        if (value & (1 << i)) {
            reversed |= (1 << (15 - i));
        }
    }
    
    return reversed;
}

void CANSBC_ReadWrite(uint16_t RxData)
{	
	uint16_t retData = 0x00U,SendData=0x00U;
	CANSBC_REG_MOSI data;
	data.data16 = RxData;

	ConfigSpiPin(PORT_PIN_E7,PORT_PIN_A14,PORT_PIN_A13,PORT_PIN_A12);

	/*
	PE[7] : SPI CS to CAN SBC
	PA[12] : CAN SBC/ EEPROM SPI MISO
	PA[13] : CAN SBC/ EEPROM SPI MOSI
	PA[14] : CAN SBC/ EEPROM SPI Clock

	SDO = CAN SBC/ EEPROM SPI MISO, Rising edge
	SDI = CAN SBC/ EEPROM SPI MOSI, Falling edge

	LSB first
	*/
	
	g_CANSBC_REG_MOSI.data16 = data.data16;
	//SPI_Write16(data.data16);
	retData = SPI_Read16(reverseBits16(data.data16));
		
	g_CANSBC_REG_MISO.data16 = reverseBits16(retData);


	if(g_CANSBC_REG_MOSI.field.r_w == 0 )
	{
		CANSBCCheckStatus(g_CANSBC_REG_MOSI.field.addr, g_CANSBC_REG_MISO.data16);
	}

	

} /*End of CANSBC_ReadWrite()*/


