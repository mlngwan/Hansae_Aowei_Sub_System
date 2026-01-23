
#include "Spi_L9369Data_Types.h"
#include "Swspi.h"



L9369_REG_MOSI g_L9369_REG_MOSI;
L9369_REG_MISO g_L9369_REG_MISO;
L9369_RegWrite_t g_L9369_RegWrite;
L9369_FaultStatus_t g_L9369_FaultStatus;
extern L9369_Status_t g_L9369_Status;

uint32_t L9369GetMsg(uint8 addr)
{
	uint32_t data;
	
	data = 0x00U;
	g_L9369_REG_MOSI.data32 = 0x00U;

	
	g_L9369_REG_MOSI.field.r_w = 0x00U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = addr; /* 0x02~0xFD */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = 0x00U;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;
	

	return data;
}

uint32_t L9369SetMsg_0x02(uint8 reset_cmd)
{
	L9369_REG_DATA_0x02 value;
	uint32_t data;


	value.data = 0x00U;
	value.field.SPI_RES_HW = reset_cmd;

	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x02U; /* 0x02 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;
	

	return data;
}

uint32_t L9369SetMsg_0x68(uint8 DrainFlagReset)
{
	L9369_REG_DATA_0x68 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.reserved1 = 0x02U; /* 010 */
	value.field.reserved2 = 0x01U; /* 01 */
	value.field.SPI_OSCtest = 0x00U; /* Trigger command oscillator test : 0/ R/W */
	value.field.reserved3 = 0x03U; /* 0011 */
	value.field.SPI_DS_A_res = DrainFlagReset; /* DS monitoring reset for Stage A : 0 / R/W('1' = reset diagnostics failure) */
	value.field.SPI_DS_B_res = DrainFlagReset; /* DS monitoring reset for Stage B : 0 / R/W('1' = reset diagnostics failure) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x68U; /* 0x68 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x69(uint8 wd)
{
	L9369_REG_DATA_0x69 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_OC_A_res = 0x00U; /* Overcurrent Reset for stage A : 0 */
	value.field.SPI_set_LPF = 0x00U; /* Low Pass Filter Cut off Freq configuration (155Hz/465Hz) : 0 */
	value.field.SPI_res_WD = wd;// 0x02U; /* Integrated WD reset command : 01 / R/W(01 default, 10 reset) */
	value.field.SPI_ACT_OFF_HS1_A = 0x01U; /* Redundant safety switch-off command for HS1 stage A : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
	value.field.SPI_ACT_OFF_HS2_A = 0x01U; /* Redundant safety switch-off command for HS2 stage A : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
	value.field.SPI_ACT_OFF_HS1_B = 0x01U; /* Redundant safety switch-off command for HS1 stage B : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
	value.field.SPI_ACT_OFF_HS2_B = 0x01U; /* Redundant safety switch-off command for HS stage2 stage B : 10 / R/W('01' = HS enabled), ('10' = HS disabled) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x69U; /* 0x69 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x6A(void)
{
	L9369_REG_DATA_0x6A value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_on_RVPG = (uint8)(g_L9369_RegWrite.SUBFAILSAFE & 0x01U); /* RVPG gate driver enabled : 0 / R/W('1'= RVPG enabled) */
	value.field.SPI_PWM_A = 0x7FU;//(uint8)(g_L9369_RegWrite.PWM_A & 0x7FU); /* PWM mode enabled on H-bridge A : 1111111 / R/W(please review pre-driver stage for detailed configuration table) */
	/* PWM Duty 
		 0 : 0.00,  16 : 12.60, 32 : 25.20, 48 : 37.80, 64 : 50.39, 80 : 62.99,  96 : 75.59, 112 : 88.19
		 1 : 0.79,  17 : 13.39, 33 : 25.98, 49 : 38.58, 65 : 51.18, 81 : 63.78,  97 : 76.38, 113 : 88.98
		 2 : 1.57,  18 : 14.17, 34 : 26.77, 50 : 39.37, 66 : 51.97, 82 : 64.57,  98 : 77.17, 114 : 89.76
		 3 : 2.36,  19 : 14.96, 35 : 27.56, 51 : 40.16, 67 : 52.76, 83 : 65.35,  99 : 77.95, 115 : 90.55
		 4 : 3.15,  20 : 15.75, 36 : 28.35, 52 : 40.94, 68 : 53.54, 84 : 66.14, 100 : 78.74, 116 : 91.34
		 5 : 3.94,  21 : 16.54, 37 : 29.13, 53 : 41.73, 69 : 54.33, 85 : 66.93, 101 : 79.53, 117 : 92.13
		 6 : 4.72,  22 : 17.32, 38 : 29.92, 54 : 42.52, 70 : 55.12, 86 : 67.72, 102 : 80.31, 118 : 92.91
		 7 : 5.51,  23 : 18.11, 39 : 30.71, 55 : 43.31, 71 : 55.91, 87 : 68.50, 103 : 81.10, 119 : 93.70
		 8 : 6.30,  24 : 18.90, 40 : 31.50, 56 : 44.09, 72 : 56.69, 88 : 69.29, 104 : 81.89, 120 : 94.49
		 9 : 7.09,  25 : 19.69, 41 : 32.28, 57 : 44.88, 73 : 57.48, 89 : 70.08, 105 : 82.68, 121 : 95.28
		10 : 7.87,  26 : 20.47, 42 : 33.07, 58 : 45.67, 74 : 58.27, 90 : 70.87, 106 : 83.46, 122 : 96.06
		11 : 8.66,  27 : 21.26, 43 : 33.86, 59 : 46.46, 75 : 59.06, 91 : 71.65, 107 : 84.25, 123 : 96.85
		12 : 9.45,  28 : 22.05, 44 : 34.65, 60 : 47.24, 76 : 59.84, 92 : 72.44, 108 : 85.04, 124 : 97.64
		13 : 10.24, 29 : 22.83, 45 : 35.43, 61 : 48.03, 77 : 60.63, 93 : 73.23, 109 : 85.83, 125 : 98.43
		14 : 11.02, 30 : 23.62, 46 : 36.22, 62 : 48.82, 78 : 61.42, 94 : 74.02, 110 : 86.61, 126 : 99.21
		15 : 11.81, 31 : 24.41, 47 : 37.01, 63 : 49.61, 79 : 62.20, 95 : 74.80, 111 : 87.40, 127 : 100.00
	*/
	value.field.SPI_set_PWM = 0x00U; /* Select the PWM frequency for H-bridge : 00 / R/W => 00 : 610Hz, 01 : 1220Hz, 10 : 4880Hz, 11 : 19520Hz */
	/* SPI_set_PWM	H-bridge PWM frequency(Hz)
		00(default) 610
		01			1220
		10			4880
		11			19520 
	*/
	value.field.SPI_set_WDfail_limit = 0x03U; /* WD fail counter setting : 11 => 00 : 1 error, 01 : 1 error, 10 : 2 errors, 11 : 3 errors */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x6AU; /* 0x6A */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x6B(void)
{
	L9369_REG_DATA_0x6B value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_RefA_on = 0x01U; /* Reference Voltage enable on H-bridge A : 1 / R/W('0' = voltage reference disabled) */
	value.field.SPI_PWM_B = 0x7FU;//(uint8)(g_L9369_RegWrite.PWM_B & 0x7FU); /* PWM mode enabled on H-bridge B : 1111111 / R/W(please review pre-driver stage for detailed configuration table) */
	/* PWM Duty 
		 0 : 0.00,  16 : 12.60, 32 : 25.20, 48 : 37.80, 64 : 50.39, 80 : 62.99,  96 : 75.59, 112 : 88.19
		 1 : 0.79,  17 : 13.39, 33 : 25.98, 49 : 38.58, 65 : 51.18, 81 : 63.78,  97 : 76.38, 113 : 88.98
		 2 : 1.57,  18 : 14.17, 34 : 26.77, 50 : 39.37, 66 : 51.97, 82 : 64.57,  98 : 77.17, 114 : 89.76
		 3 : 2.36,  19 : 14.96, 35 : 27.56, 51 : 40.16, 67 : 52.76, 83 : 65.35,  99 : 77.95, 115 : 90.55
		 4 : 3.15,  20 : 15.75, 36 : 28.35, 52 : 40.94, 68 : 53.54, 84 : 66.14, 100 : 78.74, 116 : 91.34
		 5 : 3.94,  21 : 16.54, 37 : 29.13, 53 : 41.73, 69 : 54.33, 85 : 66.93, 101 : 79.53, 117 : 92.13
		 6 : 4.72,  22 : 17.32, 38 : 29.92, 54 : 42.52, 70 : 55.12, 86 : 67.72, 102 : 80.31, 118 : 92.91
		 7 : 5.51,  23 : 18.11, 39 : 30.71, 55 : 43.31, 71 : 55.91, 87 : 68.50, 103 : 81.10, 119 : 93.70
		 8 : 6.30,  24 : 18.90, 40 : 31.50, 56 : 44.09, 72 : 56.69, 88 : 69.29, 104 : 81.89, 120 : 94.49
		 9 : 7.09,  25 : 19.69, 41 : 32.28, 57 : 44.88, 73 : 57.48, 89 : 70.08, 105 : 82.68, 121 : 95.28
		10 : 7.87,  26 : 20.47, 42 : 33.07, 58 : 45.67, 74 : 58.27, 90 : 70.87, 106 : 83.46, 122 : 96.06
		11 : 8.66,  27 : 21.26, 43 : 33.86, 59 : 46.46, 75 : 59.06, 91 : 71.65, 107 : 84.25, 123 : 96.85
		12 : 9.45,  28 : 22.05, 44 : 34.65, 60 : 47.24, 76 : 59.84, 92 : 72.44, 108 : 85.04, 124 : 97.64
		13 : 10.24, 29 : 22.83, 45 : 35.43, 61 : 48.03, 77 : 60.63, 93 : 73.23, 109 : 85.83, 125 : 98.43
		14 : 11.02, 30 : 23.62, 46 : 36.22, 62 : 48.82, 78 : 61.42, 94 : 74.02, 110 : 86.61, 126 : 99.21
		15 : 11.81, 31 : 24.41, 47 : 37.01, 63 : 49.61, 79 : 62.20, 95 : 74.80, 111 : 87.40, 127 : 100.00
	*/
	value.field.SPI_on_HS2_A = (uint8)(g_L9369_RegWrite.HS2_A & 0x01U); /* Enable for HS2 gate pre-driver, Stage A : 0 / R/W('1' = HS2 pre-driver stage A turned on) */
	value.field.SPI_on_HS1_A = (uint8)(g_L9369_RegWrite.HS1_A & 0x01U); /* Enable for HS1 gate pre-driver, Stage A : 0 / R/W('1' = HS1 pre-driver stage A turned on) */
	value.field.SPI_on_HS2_B = (uint8)(g_L9369_RegWrite.HS2_B & 0x01U); /* Enable for HS2 gate pre-driver, Stage B : 0 / R/W('1' = HS2 pre-driver stage B turned on) */
	value.field.SPI_on_HS1_B = (uint8)(g_L9369_RegWrite.HS1_B & 0x01U); /* Enable for HS1 gate pre-driver, Stage B : 0 / R/W('1' = HS1 pre-driver stage B turned on) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x6BU; /* 0x6B */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x6C(void)
{
	L9369_REG_DATA_0x6C value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_DSthLS = 0x0FU; /* DS monitoring thresholds for LS stage (A,B) : 1111 / R/W(please refer to pre-driver stage for detailed configuration table) */
	/* 0000 : 0.765, 0001 : 0.915, 0010 : 1.065, 0011 : 1.215, 0100 : 1.365, 0101 : 1.515, 0110 : 1.665, 0111 : 1.815, 1000 : 0.315, 1001 : 0.465, 1010 : 0.615, 1011 : 0.765, 1100 : 0.915, 1101 : 1.065, 1110 : 1.215, 1111(default) : 1.365 */
	value.field.SPI_DSthHS = 0x0FU; /* DS monitoring thresholds for HS stage (A,B) : 1111 / R/W(please refer to pre-driver stage for detailed configuration table) */
	/* 0000 : 0.75, 0001 : 0.90, 0010 : 1.05, 0011 : 1.20, 0100 : 1.35, 0101 : 1.50, 0110 : 1.65, 0111 : 1.80, 1000 : 0.3, 1001 : 0.45, 1010 : 0.6, 1011 : 0.75, 1100 : 0.90, 1101 : 1.05, 1110 : 1.20, 1111(default) : 1.35 */
	value.field.SPI_RefB_on = 0x01U; /* H-bridge stage B reference voltage enabled : 1 / R/W('0' = voltage reference disabled) */
	value.field.SPI_MTPth = 0x03U; /* Configuration of Motor test Threshold : 011 / R/W(please refer to pre-driver stage for detailed configuration table) */
	/* VSBRIDGE_x = 13.5 V
		SPI_MTPth OVC threshold(3 mΩ shunt) OVC threshold(2 mΩ shunt)
		000          ±8.41                  ±12.61
		001          ±9.48                  ±14.21
		010          ±10.54                 ±15.82
		011(default) ±11.61                 ±17.42
		100          ±12.68                 ±19.02
		101          ±13.75                 ±20.62
		110          ±14.81                 ±22.22
		111          ±15.88                 ±23.82
	*/
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x6CU; /* 0x6C */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x6D(void)
{
	L9369_REG_DATA_0x6D value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_MTPtime = 0x00U; /* Motor test time configuration : 01 / R/W */
	value.field.SPI_set_WDTime = 0x03U; /* Watchdog : 01 / R/W => 00 : 11.26ms, 01 : 22.52ms, 10 : 45.04ms, 11 : 90.11ms */
	value.field.SPI_test_CSA2_B = 0x00U; /* Test enable for CSA2 stage B : 0 / R/W('1' = test enabled on CSA2 stage B) */
	value.field.SPI_test_CSA1_B = 0x00U; /* Test enable for CSA1 stage B : 0 / R/W('1' = test enabled on CSA1 stage B) */
	value.field.SPI_test_CSA2_A = 0x00U; /* Test enable for CSA2 stage A : 0 / R/W('1' = test enabled on CSA2 stage A) */
	value.field.SPI_test_CSA1_A = 0x00U; /* Test enable for CSA1 stage A : 0 / R/W('1' = test enabled on CSA1 stage A) */
	value.field.SPI_on_T1_A = (uint8)(g_L9369_RegWrite.T1_A & 0x01U); /* T1 NFET enable in MSS stage A : 0 /R/W('1' = T1 FET stage A enabled) */
	value.field.SPI_on_T1_B = (uint8)(g_L9369_RegWrite.T1_B & 0x01U); /* T1 NFET enable in MSS stage B : 0 /R/W('1' = T1 FET stage B enabled) */
	value.field.SPI_on_T2_A = (uint8)(g_L9369_RegWrite.T2_A & 0x01U); /* T2 NFET enable in MSS stage A : 0 /R/W('1' = T2 FET stage A enabled) */
	value.field.SPI_on_T2_B = (uint8)(g_L9369_RegWrite.T2_B & 0x01U); /* T2 NFET enable in MSS stage B : 0 /R/W('1' = T2 FET stage B enabled) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x6DU; /* 0x6D */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x6E(void)
{
	L9369_REG_DATA_0x6E value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_OCth_A = 0x06U; /* OVC thresholds configuration for Hbridge stage A : 1010 / R/W */
	value.field.SPI_OCth_B = 0x06U; /* OVC thresholds configuration for Hbridge stage B : 1010 / R/W */
	/* SPI_OCth_X Overcurrent Threshold [A] Overcurrent Threshold [A]
		(x=A,B) 	 (3 mΩ shunt)				   (2 mΩ shunt)
		0000		  ± 36.17						± 54. 25
		0001		  ± 38.30						± 57.46
		0010		  ± 40.44						± 60.66
		0011		  ± 42.57						± 63.86
		0100		  ± 44.71						± 67.07
		0101		  ± 46.85						± 70.27
		0110		  ± 48.98						± 73.47
		0111		  ± 51.12						± 76.67
		1000		  ± 53.25						± 79.88
		1001		  ± 55.39						± 83.08
		1010(default) ± 57.52						± 86.28
		1011		  ± 59.66						± 89.49
		1100		  ± 61.75						± 92.69
		1101		  ± 63.93						± 95.89
		1110		  ± 66.06						± 99.10
		1111		  ± 68.20						± 102.30
	*/
	value.field.SPI_on_LS2_A = (uint8)(g_L9369_RegWrite.LS2_A & 0x01U); /* LS2 turn-on command for H-bridge stage A : 0 / R/W('1' = LS2 stage A turned on) */
	value.field.SPI_on_LS1_A = (uint8)(g_L9369_RegWrite.LS1_A & 0x01U); /* LS1 turn-on command for H-bridge stage A : 0 / R/W('1' = LS1 stage A turned on) */
	value.field.SPI_on_LS2_B = (uint8)(g_L9369_RegWrite.LS2_B & 0x01U); /* LS2 turn-on command for H-bridge stage B : 0 / R/W('1' = LS2 stage B turned on) */
	value.field.SPI_on_LS1_B = (uint8)(g_L9369_RegWrite.LS1_B & 0x01U); /* LS1 turn-on command for H-bridge stage B : 0 / R/W('1' = LS1 stage B turned on) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x6EU; /* 0x6E */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x6F(void)
{
	L9369_REG_DATA_0x6F value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_LSoff_high = 0x00U; /* Low ohmic path switch off disable bit for LS pre-driver stage A and B : 0 / R/W (default)('1' = low ohmic path enabled) */
	value.field.SPI_HSoff_high = 0x00U; /* Low ohmic path switch off disable bit for HS pre-driver stage A and B : 0 / R/W (default)('1' = low ohmic path enabled) */
	value.field.SPI_OLTest_HSOUT0 = 0x00U; /* OL test enable for lamp driver HSOUT0 : 0 / R/W('1' = Open-load test enabled on HSOUT0) (to be set to '0' by app sw when test is finished) */
	value.field.SPI_OLTest_HSOUT1 = 0x00U; /* OL test enable for lamp driver HSOUT1 : 0 / R/W('1' = Open-load test enabled on HSOUT1) (to be set to '0' by app sw when test is finished) */
	value.field.SPI_on_HSOUT0 = (uint8)(g_L9369_RegWrite.HSOUT0 & 0x01U); /* HSOUT0 lamp driver enable command bit : 0 / R/W(1 enabled) */
	value.field.SPI_on_HSOUT1 = (uint8)(g_L9369_RegWrite.HSOUT1 & 0x01U); /* HSOUT1 lamp driver enable command bit : 0 / R/W(1 enabled) */
	value.field.SPI_MSSon_A = 0x00U; /* Enable command bit for MSS interface stage A : 0 / R/W(1 enabled bit) */
	value.field.SPI_MSSon_B = 0x00U; /* Enable command bit for MSS interface stage B : 0 / R/W(1 enabled bit) */
	value.field.SPI_com_WD = (uint8)(g_L9369_RegWrite.WD_Cmd & 0x0FU); /* WD command : 0000 / R/W */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x6FU; /* 0x6F */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x70(void)
{
	L9369_REG_DATA_0x70 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_voltage_HSOUT0 = 0x000U;/*0xFFFU;*/ /* Programmable Lamp driver output voltage in PWM mode for HSOUT0 : 000000000000 / R/W(please refer to lamp driver chapter for detailed configuration table) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x70U; /* 0x70 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x71(void)
{
	L9369_REG_DATA_0x71 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_voltage_HSOUT1 = 0x000U;/*0xFFFU;*/ /* Programmable Lamp driver output voltage in PWM mode for HSOUT1 : 000000000000 / R/W(please refer to lamp driver chapter for detailed configuration table) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x71U; /* 0x71 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x72(uint8 gio3, uint8 gio4, uint8 gio5)
{
	L9369_REG_DATA_0x72 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_GIO5_MuxCtrl = (uint8)(gio5 & 0x0FU); /* Mux Control command bit for GIO5 : 1111 / R/W('1111'= GIO5 not configured as digital input controlled output) NAWD '1001' and '1110' */
	value.field.SPI_GIO4_MuxCtrl = (uint8)(gio4 & 0x0FU); /* Mux Control command bit for GIO4 : 1111 / R/W('1111'= GIO4 not configured as digital input controlled output) NAWD '1001' and '1110' */
	value.field.SPI_GIO3_MuxCtrl = (uint8)(gio3 & 0x0FU); /* Mux Control command bit for GIO3 : 1111 / R/W('1111'= GIO3 not configured as digital input controlled output) NAWD '1001' and '1110' */
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
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x72U; /* 0x72 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x73(uint8 gio0, uint8 gio1, uint8 gio2)
{
	L9369_REG_DATA_0x73 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_GIO2_MuxCtrl = (uint8)(gio2 & 0x0FU); /* Mux Control command bit for GIO2 : 1111 / R/W('1111'= GIO2 not configured as digital input controlled output) NAWD '1001' and '1110' */
	value.field.SPI_GIO1_MuxCtrl = (uint8)(gio1 & 0x0FU); /* Mux Control command bit for GIO1 : 1111 / R/W('1111'= GIO1 not configured as digital input controlled output) NAWD '1001' and '1110' */
	value.field.SPI_GIO0_MuxCtrl = (uint8)(gio0 & 0x0FU); /* Mux Control command bit for GIO0 : 1111 / R/W('1111'= GIO0 not configured as digital input controlled output) NAWD '1001' and '1110' */
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
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x73U; /* 0x73 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x74(void)
{
	L9369_REG_DATA_0x74 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_GPIO3_MuxCtrl = 0X07U; //0x07U; /* Mux Control command bit for GPIO3 : 111 / R/W('111'= GPIO3 not configured as digital input controlled output) NAWD '100','101','110' */
	value.field.SPI_GPIO2_MuxCtrl = 0x07U; /* Mux Control command bit for GPIO2 : 111 / R/W('111'= GPIO2 not configured as digital input controlled output) NAWD '100','101','110' */
	value.field.SPI_GPIO1_MuxCtrl = 0x07U; /* Mux Control command bit for GPIO1 : 111 / R/W('111'= GPIO1 not configured as digital input controlled output) NAWD '100','101','110' */
	value.field.SPI_GPIO0_MuxCtrl = 0x07U; /* Mux Control command bit for GPIO0 : 111 / R/W('111'= GPIO0 not configured as digital input controlled output) NAWD '100','101','110' */
	/* SPI_GPIOx_MuxCtrl
		000 : GPIO0
		001 : GPIO1
		010 : GPIO2
		011 : GPIO3
		111 : No connection, All others are not allowed
	*/
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x74U; /* 0x74 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x75(uint8 gio6, uint8 gio7, uint8 gio8)
{
	L9369_REG_DATA_0x75 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_GIO8_MuxCtrl = (uint8)(gio8 & 0x0FU); /* Mux Control command bit for GIO8 stage : 1111 / R/W('1111'= GIO8 not configured as digital input controlled output) NAWD '1001' and '1110' */
	value.field.SPI_GIO7_MuxCtrl = (uint8)(gio7 & 0x0FU); /* Mux Control command bit for GIO7 stage : 1111 / R/W('1111'= GIO7 not configured as digital input controlled output) NAWD '1001' and '1110' */
	value.field.SPI_GIO6_MuxCtrl = (uint8)(gio6 & 0x0FU); /* Mux Control command bit for GIO6 stage : 1111 / R/W('1111'= GIO6 not configured as digital input controlled output) NAWD '1001' and '1110' */
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
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x75U; /* 0x75 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x76(void)
{
	L9369_REG_DATA_0x76 value;
	uint32_t data;

	value.data = 0x00U;
	
	value.field.SPI_GIO0_Wake_LSctrl = 0x00U; /* Configuration mode for GIO0 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
	value.field.SPI_GIO2_Wake_LSctrl = 0x00U; /* Configuration mode for GIO2 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
	value.field.SPI_GIO4_Wake_LSctrl = 0x00U; /* Configuration mode for GIO4 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
	value.field.SPI_GIO5_Wake_LSctrl = 0x00U; /* Configuration mode for GIO5 wake LS control during Sleep Mode only. (Stored in PROM) : 0 / R/W */
	value.field.SPI_GIO0_Wake_HSctrl = 0x00U; /* Configuration command for GIO0 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
	value.field.SPI_GIO1_Wake_HSctrl = 0x00U; /* Configuration command for GIO1 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
	value.field.SPI_GIO2_Wake_HSctrl = 0x00U; /* Configuration command for GIO2 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */
	value.field.SPI_GIO3_Wake_HSctrl = 0x00U; /* Configuration command for GIO3 wake-up HS control during Sleep Mode only. (Stored in PROM) : 1 / R/W(please refer to button interface chapter for detailed configuration) */

	value.field.SPI_Oltest_GPIO3 = 0x01U; /* Open-load test Enable for GPIO3 stage : 0 / R/W('1' = open-load test enabled on GPIO3 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
	value.field.SPI_Oltest_GPIO2 = 0x01U; /* Open-load test Enable for GPIO2 stage : 0 / R/W('1' = open-load test enabled on GPIO2 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
	value.field.SPI_Oltest_GPIO1 = 0x01U; /* Open-load test Enable for GPIO1 stage : 0 / R/W('1' = open-load test enabled on GPIO1 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
	value.field.SPI_Oltest_GPIO0 = 0x01U; /* Open-load test Enable for GPIO0 stage : 0 / R/W('1' = open-load test enabled on GPIO0 stage) Trigger bit to start OL test on GPIO and set to '0' automatically by device when test is finished */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x76U; /* 0x76 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x77(void)
{
	L9369_REG_DATA_0x77 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_ACT_valid = (uint8)(g_L9369_RegWrite.ACT_Valid & 0x01U); /* Validation bit for H-bridge actuation data : 0 / R/W(Read fix value 0, write 1 is actuation valid trigger for Hbridge stage) */
	value.field.reserved = 0x01U; /* 1 / R/W */
	value.field.SPI_enPWM_GIO = 0x01U; /* Enable command bit for Current shaping for GIO : 1 / R/W('0' = Current shaping option disabled) */
	value.field.SPI_GIO8_HSctrl = (uint8)(g_L9369_RegWrite.GIO8 & 0x01U); /* Command bit for HS stage control for GIO8 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO7_HSctrl = (uint8)(g_L9369_RegWrite.GIO7 & 0x01U); /* Command bit for HS stage control for GIO7 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO6_HSctrl = (uint8)(g_L9369_RegWrite.GIO6 & 0x01U); /* Command bit for HS stage control for GIO6 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO5_HSctrl = (uint8)(g_L9369_RegWrite.GIO5 & 0x01U); /* Command bit for HS stage control for GIO5 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO4_HSctrl = (uint8)(g_L9369_RegWrite.GIO4 & 0x01U); /* Command bit for HS stage control for GIO4 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO3_HSctrl = (uint8)(g_L9369_RegWrite.GIO3 & 0x01U); /* Command bit for HS stage control for GIO3 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO2_HSctrl = (uint8)(g_L9369_RegWrite.GIO2 & 0x01U); /* Command bit for HS stage control for GIO2 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO1_HSctrl = (uint8)(g_L9369_RegWrite.GIO1 & 0x01U); /* Command bit for HS stage control for GIO1 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	value.field.SPI_GIO0_HSctrl = (uint8)(g_L9369_RegWrite.GIO0 & 0x01U); /* Command bit for HS stage control for GIO0 (pull up source actuation bit for button diagnostics in normal mode) : 0 / R/W('1' = pull-up current source enabled) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x77U; /* 0x77 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}



uint32_t L9369SetMsg_0x81(void)
{
	L9369_REG_DATA_0x81 value;
	uint32_t data;

	value.data = 0x00U;
	
	g_L9369_REG_MOSI.field.addr = 0x81U; /* 0x81 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}


uint32_t L9369SetMsg_0x84(void)
{
	L9369_REG_DATA_0x84 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.MSSresult_reg_SPI = 0x00U; /* 14bit MSS test result register : 00000000000000 / R/W */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x84U; /* 0x84 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x85(void)
{
	L9369_REG_DATA_0x85 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.MSSresult_reg_SPI = 0x00U; /* 2bit MSS test result register : 00 / R/W */
	value.field.SPI_K4_Thr = 0x00U; /* K4 comparator threshold MSS speed signal detection threshold : 00 / R/W */
	value.field.SPI_Mtest_A = 0x00U; /* Motor Test actuation on stage A : 0 (write 1 is trigger bit, reset by L9369 when test is finished) */
	value.field.MSStest_test_start_busy_SPI = 0x00U; /* Test of the MSS test is running : 0 / R/W */
	value.field.MSStest_start_busy_SPI = 0x00U; /* MSS test is running : 0 / R/W */
	value.field.SPI_Mtest_B = 0x00U; /* Motor Test actuation on stage B : 0 (write 1 is trigger bit, reset by L9369 when test is finished) */
	value.field.SPI_K7_Thr = 0x00U; /* K7 comparator threshold. MSS UV detection threshold : 0000 / R/W */
	/* Reference Ai K7-min(V) K7 typ(V) K7-max(V) SPI_K7_thres
		A0			3.85	  4 		4.15	  0000 (default)
		A1			3.85	  4 		4.15	  0001
		A2			4.1 	  4.25		4.4 	  0010
		A3			4.35	  4.5		4.65	  0011
		A4			4.6 	  4.75		4.9 	  0100
		A5			4.85	  5 		5.15	  0101
		A6			5.1 	  5.25		5.4 	  0110
		A7			5.35	  5.5		5.65	  0111
		A8			5.6 	  5.75		5.9 	  1000
		A9			5.85	  6 		6.15	  1001
		A10 		6.1 	  6.25		6.4 	  1010
		A11 		6.35	  6.5		6.65	  1011
		A12 		6.6 	  6.75		6.9 	  1100
		A13 		6.85	  7 		7.15	  1101
		A14 		7.1 	  7.25		7.4 	  1110
		A15 		7.1 	  7.25		7.4 	  1111
	*/
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x85U; /* 0x85 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x86(void)
{
	L9369_REG_DATA_0x86 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_off_GPIO0 = 0x01U; /* Driving control for GPIO0 stage : R/W '0' = GPIO0 is controlled via SPI command '1'= GPIO0 is controlled via multiplexer (default) */
	value.field.SPI_off_GPIO1 = 0x01U; /* Driving control for GPIO1 stage : R/W '0' = GPIO1 is controlled via SPI command '1'= GPIO1 is controlled via multiplexer (default) */
	value.field.SPI_off_GPIO2 = 0x01U; /* Driving control for GPIO2 stage : R/W '0' = GPIO2 is controlled via SPI command '1'= GPIO2 is controlled via multiplexer (default) */
	value.field.SPI_off_GPIO3 = 0x01U; /* Driving control for GPIO3 stage : R/W '0' = GPIO3 is controlled via SPI command '1'= GPIO3 is controlled via multiplexer (default) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x86U; /* 0x86 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x87(uint8 gio0, uint8 gio1, uint8 gio2, uint8 gio3, uint8 gio4, uint8 gio5, uint8 gio6, uint8 gio7, uint8 gio8)
{
	L9369_REG_DATA_0x87 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.unused = 0x07U; /* 111 */
	value.field.SPI_off_GIO8 = (uint8)(gio8 & 0x01U); /* Driving control for GIO8 LS stage : R/W '0' = GIO8 is controlled via SPI command '1'= GIO8 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO7 = (uint8)(gio7 & 0x01U); /* Driving control for GIO7 LS stage : R/W '0' = GIO7 is controlled via SPI command '1'= GIO7 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO6 = (uint8)(gio6 & 0x01U); /* Driving control for GIO6 LS stage : R/W '0' = GIO6 is controlled via SPI command '1'= GIO6 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO5 = (uint8)(gio5 & 0x01U); /* Driving control for GIO5 LS stage : R/W '0' = GIO5 is controlled via SPI command '1'= GIO5 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO4 = (uint8)(gio4 & 0x01U); /* Driving control for GIO4 LS stage : R/W '0' = GIO4 is controlled via SPI command '1'= GIO4 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO3 = (uint8)(gio3 & 0x01U); /* Driving control for GIO3 LS stage : R/W '0' = GIO3 is controlled via SPI command '1'= GIO3 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO2 = (uint8)(gio2 & 0x01U); /* Driving control for GIO2 LS stage : R/W '0' = GIO2 is controlled via SPI command '1'= GIO2 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO1 = (uint8)(gio1 & 0x01U); /* Driving control for GIO1 LS stage : R/W '0' = GIO1 is controlled via SPI command '1'= GIO1 is controlled via multiplexer (default) */
	value.field.SPI_off_GIO0 = (uint8)(gio0 & 0x01U); /* Driving control for GIO0 LS stage : R/W '0' = GIO0 is controlled via SPI command '1'= GIO0 is controlled via multiplexer (default) */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x87U; /* 0x87 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}
uint32_t L9369SetMsg_0x88(void)
{
	L9369_REG_DATA_0x88 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SYS_WAKE_STATUS_CLEAR = 0x01U;
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x88U; /* 0x88 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x8A(void)
{
	L9369_REG_DATA_0x8A value;
	uint32_t data;

	value.data = 0x00U;
	value.field.i_gio_hs_low = 0x01U;/* Reduced current configuration bit : 1 (default) = IGIOx_pu_red2 0 = IGIOx_pu_red1 */
	//value.field.SYS_WAKE_IN_status; /* SYS_WAKE_IN monitoring bit : 1/R */
	value.field.SYS_WAKE_OUT_EN = 0x01U; /* SYS wake/SYS_WAKE_IN enable bit : 1 */
	value.field.EN_EL_RST_EN = 0x01U; /* RST function enable on EN_EL pin (#47) : 0 */ //240612  0 -> 1
	value.field.HSOUT1_LS_mode = HSOUT1_MODE; /* Enable command for LS configuration for lamp driver stage #1 : 0 */
	value.field.HSOUT0_LS_mode = HSOUT0_MODE; /* Enable command for LS configuration for lamp driver stage #0 : 0 */
	value.field.Fault_pin_en = 0x01U; /* Fault pin enable on GPIO0 : 0  */
	value.field.Fault_pin_config = 0x00U; /* Configuration table of the fault pin on GPIO0 : 0000 */
	value.field.Not_used = 0x00U; /* 0000 */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x8AU; /* 0x8A */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0x8B(void)
{
	L9369_REG_DATA_0x8B value;
	uint32_t data;

	value.data = 0x00U;
	value.field.STUCK_MODE = 0x01U; /* Stuck mode enable : 01 */
	value.field.o_i_gio_high_th = 0x00U; /* SCG comparator enable for button with diode and integrated resistors : 0 */
	value.field.GIO0_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO1_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO2_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO3_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO4_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO5_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO6_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO7_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.GIO8_OL_CAP = 0x00U; /* GIO_CAP open load test result : 0 */
	value.field.Not_used = 0x00U; /* 0000 */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0x8BU; /* 0x8B */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0xCA(void)
{
	L9369_REG_DATA_0xCA value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPARE0 = 0x00U; /* PROM SPARE application register : 000000000000 / R/W */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xCAU; /* 0xCA */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0xCB(void)
{
	L9369_REG_DATA_0xCB value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPARE1 = 0x00U; /* PROM SPARE application register : 000000000000 / R/W */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xCBU; /* 0xCB */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0xCC(void)
{
	L9369_REG_DATA_0xCC value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPARE2 = 0x00U; /* PROM SPARE application register : 000000000000 / R/W */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xCCU; /* 0xCC */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0xCD(void)
{
	L9369_REG_DATA_0xCD value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPARE3 = 0x00U; /* PROM SPARE application register : 000000000000 / R/W */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xCDU; /* 0xCD */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0xCE(void)
{
	L9369_REG_DATA_0xCE value;
	uint32_t data;

	value.data = 0x00U;
	value.field.SPI_SH1_A_Ctrl = 0x01U; /* Disable function on ADC stage for SH1 voltage measurement path of stage A : 1 / R/W ('1' = Voltage measurement path on SH1 on stage A is enabled) ('0' = Voltage measurement path on SH1 on stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_SH2_A_Ctrl = 0x01U; /* Disable function on ADC stage for SH2 voltage measurement path of stage A : 1 / R/W ('1' = Voltage measurement path on SH2 on stage A is enabled) ('0' = Voltage measurement path on SH2 on stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_CSIN1_A_Ctrl = 0x01U; /* Disable function on ADC for CSIN1 current measurement path for stage A : 1 / R/W ('1' = Current measurement path on CSA1 stage A is enabled) ('0' = Current measurement path on CSA1 stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_VSBRIDGE_A_Ctrl = 0x01U; /* Disable function of ADC stage for VSBRIDGE voltage measurement path on stage A : 1 / R/W ('1' = voltage measurement path on VSBRIDGE stage A is enabled) ('0' = voltage measurement path on VSBRIDGE stage A is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_SH1_B_Ctrl = 0x01U; /* Disable function on ADC stage for SH1 voltage measurement path of stage B : 1 / R/W ('1' = Voltage measurement path on SH1 on stage B is enabled) ('0' = Voltage measurement path on SH1 on stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_SH2_B_Ctrl = 0x01U; /* Disable function on ADC stage for SH2 voltage measurement path of stage B : 1 / R/W ('1' = Voltage measurement path on SH2 on stage B is enabled) ('0' = Voltage measurement path on SH2 on stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_CSIN1_B_Ctrl = 0x01U; /* Disable function on ADC for CSIN1 current measurement path for stage B : 1 / R/W ('1' = Current measurement path on CSA1 stage B is enabled) ('0' = Current measurement path on CSA1 stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.SPI_VSBRIDGE_B_Ctrl = 0x01U; /* Disable function of ADC stage for VSBRIDGE voltage measurement path on stage B : 1 / R/W ('1' = voltage measurement path on VSBRIDGE stage B is enabled) ('0' = voltage measurement path on VSBRIDGE stage B is disabled for current consumption savings during Normal mode in no motor actuation phase) */
	value.field.Not_used = 0x0FU; /* 0000 1111 */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xCEU; /* 0xCE */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369SetMsg_0xD3(void)
{
	L9369_REG_DATA_0xD3 value;
	uint32_t data;

	value.data = 0x00U;
	value.field.Unused = 0x01U; /* 1/R */
	value.field.SPI_on100_HSOUT0 = 0x01U; /* Full 100% on PWM mode on HSOUT0 lamp driver stage : 0 / RW = not 100% PWM mode active */
	value.field.SPI_on100_HSOUT1 = 0x01U; /* Full 100% on PWM mode on HSOUT1 lamp driver stage : 0 / RW = not 100% PWM mode active */
	
	g_L9369_REG_MOSI.field.r_w = 0x01U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xD3U; /* 0xD3 */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	g_L9369_REG_MOSI.field.data = (uint16)value.data;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	data = g_L9369_REG_MOSI.data32;

	return data;
}

uint32_t L9369GetMsg_0xFD(void)
{
	uint32_t data;

	g_L9369_REG_MOSI.field.r_w = 0x00U; /* Read : 0, Write : 1 */
	g_L9369_REG_MOSI.field.addr = 0xFDU; /* 0x02~0xFD */
	g_L9369_REG_MOSI.field.tm = 0x00U;
	/*g_L9369_REG_MOSI.field.cnt = 0x00U;*/
	g_L9369_REG_MOSI.field.data = 0x00U;
	g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
	//g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);

	data = g_L9369_REG_MOSI.data32;
	
	return data;
}

L9369_RegWrite_t *L9369RegInit(void )
{
	g_L9369_RegWrite.Init = TRUE;
	g_L9369_RegWrite.HS1_A = FALSE;
	g_L9369_RegWrite.HS2_A = FALSE;
	g_L9369_RegWrite.HS1_B = FALSE;
	g_L9369_RegWrite.HS2_B = FALSE;
	g_L9369_RegWrite.LS1_A = FALSE;
	g_L9369_RegWrite.LS2_A = FALSE;
	g_L9369_RegWrite.LS1_B = FALSE;
	g_L9369_RegWrite.LS2_B = FALSE;
	g_L9369_RegWrite.HSOUT0 = FALSE;
	/* PWM Duty 
		 0 : 0.00,  16 : 12.60, 32 : 25.20, 48 : 37.80, 64 : 50.39, 80 : 62.99,  96 : 75.59, 112 : 88.19
		 1 : 0.79,  17 : 13.39, 33 : 25.98, 49 : 38.58, 65 : 51.18, 81 : 63.78,  97 : 76.38, 113 : 88.98
		 2 : 1.57,  18 : 14.17, 34 : 26.77, 50 : 39.37, 66 : 51.97, 82 : 64.57,  98 : 77.17, 114 : 89.76
		 3 : 2.36,  19 : 14.96, 35 : 27.56, 51 : 40.16, 67 : 52.76, 83 : 65.35,  99 : 77.95, 115 : 90.55
		 4 : 3.15,  20 : 15.75, 36 : 28.35, 52 : 40.94, 68 : 53.54, 84 : 66.14, 100 : 78.74, 116 : 91.34
		 5 : 3.94,  21 : 16.54, 37 : 29.13, 53 : 41.73, 69 : 54.33, 85 : 66.93, 101 : 79.53, 117 : 92.13
		 6 : 4.72,  22 : 17.32, 38 : 29.92, 54 : 42.52, 70 : 55.12, 86 : 67.72, 102 : 80.31, 118 : 92.91
		 7 : 5.51,  23 : 18.11, 39 : 30.71, 55 : 43.31, 71 : 55.91, 87 : 68.50, 103 : 81.10, 119 : 93.70
		 8 : 6.30,  24 : 18.90, 40 : 31.50, 56 : 44.09, 72 : 56.69, 88 : 69.29, 104 : 81.89, 120 : 94.49
		 9 : 7.09,  25 : 19.69, 41 : 32.28, 57 : 44.88, 73 : 57.48, 89 : 70.08, 105 : 82.68, 121 : 95.28
		10 : 7.87,  26 : 20.47, 42 : 33.07, 58 : 45.67, 74 : 58.27, 90 : 70.87, 106 : 83.46, 122 : 96.06
		11 : 8.66,  27 : 21.26, 43 : 33.86, 59 : 46.46, 75 : 59.06, 91 : 71.65, 107 : 84.25, 123 : 96.85
		12 : 9.45,  28 : 22.05, 44 : 34.65, 60 : 47.24, 76 : 59.84, 92 : 72.44, 108 : 85.04, 124 : 97.64
		13 : 10.24, 29 : 22.83, 45 : 35.43, 61 : 48.03, 77 : 60.63, 93 : 73.23, 109 : 85.83, 125 : 98.43
		14 : 11.02, 30 : 23.62, 46 : 36.22, 62 : 48.82, 78 : 61.42, 94 : 74.02, 110 : 86.61, 126 : 99.21
		15 : 11.81, 31 : 24.41, 47 : 37.01, 63 : 49.61, 79 : 62.20, 95 : 74.80, 111 : 87.40, 127 : 100.00
	*/
	g_L9369_RegWrite.PWM_A = 0x0U;//0x7FU;/* 100.00 % */
	g_L9369_RegWrite.HSOUT1 = FALSE;
	g_L9369_RegWrite.PWM_B = 0x0U;//0x7FU;/* 100.00 % */
	g_L9369_RegWrite.WD_Cmd = 0x00U;
	g_L9369_RegWrite.GIO0 = TRUE;/* SW4(GIO0) : H */
	g_L9369_RegWrite.GIO1 = TRUE; /* SW5(GIO1) : H */
	g_L9369_RegWrite.GIO2 = !GIO2_MODE; /* none(GIO2) : L/H */
	g_L9369_RegWrite.GIO3 = !GIO3_MODE; /* none(GIO3) : L/H */
	g_L9369_RegWrite.GIO4 = TRUE; /* SW3(GIO4) : H */
	g_L9369_RegWrite.GIO5 = TRUE; /* SW7(GIO5) : H */
	g_L9369_RegWrite.GIO6 = TRUE; /* SW1(GIO6) : H */
	g_L9369_RegWrite.GIO7 = !GIO7_MODE; /* none(GIO7) : L/H */
	g_L9369_RegWrite.GIO8 = TRUE; /* SW2(GIO8): H */

	g_L9369_RegWrite.ACT_Valid = TRUE;
	g_L9369_RegWrite.Step = 0x00U;
	g_L9369_REG_MOSI.field.cnt = 0x00U;

	g_L9369_RegWrite.SyncStep = 5U;
	g_L9369_Status.Test_Cur_Limit = 10U;

	return &g_L9369_RegWrite;
}


uint8_t L9369MakeCRC5(uint32_t data)
{
	L9369_REG_MOSI mosi;
	uint32_t value = 0 ;
	uint32_t* Array_SpiTxMsg=0x0U;

	mosi.data32 = data ; 
	Array_SpiTxMsg=L9369_CRCtoArray(mosi.data32);
	value|=L9369_CRCGeneration(Array_SpiTxMsg,CRC_CAL_SIZE); 

	return (uint8_t)(value);

}


uint32_t L9369_CRCCal(uint32_t CRC_initial,uint32_t Data)
{
	uint32_t Temp_Bit = 0x0U,CRC = 0x0U;
	uint32_t bit0 = 0x0U,bit1 = 0x0U,bit2 = 0x0U,bit3 = 0x0U,bit4 = 0x0U;

	bit0 = CRC_initial & BIT0;
	bit1 = (CRC_initial & BIT1)>>1;
	bit2 = (CRC_initial & BIT2)>>2;
	bit3 = (CRC_initial & BIT3)>>3;
	bit4 = (CRC_initial & BIT4)>>4;

	Temp_Bit = Data ^ bit4; //CRC5-USB
	bit4 = bit3;
	bit3 = bit2;
	bit1 = bit1^ Temp_Bit;
	bit2 = bit1;
	bit1 = bit0;
	bit0 = Temp_Bit;
	   
	CRC = (bit4<<4)|(bit3<<3)|(bit2<<2)|(bit1<<1)|bit0;
	return CRC;
}

uint32_t L9369_CRCGeneration(uint32_t Data[],uint8 byteLength)
{
   uint8 i=0;
   uint32_t CRCtemp;

   for(i=0;i<byteLength;i++)
   {
      if(i==0)
      {
         CRCtemp=L9369_CRCCal(0x1F,Data[i]);//initial value
      }
      else
      {
         CRCtemp=L9369_CRCCal(CRCtemp,Data[i]);
      }
   }
   return CRCtemp;
}

uint32_t* L9369_CRCtoArray(uint32_t value)
{
 
   static uint32_t  Array[CRC_CAL_SIZE]={0};
   uint32_t front_Val=0;
   uint32_t back_Val=0;
   uint32_t exclude_Fram_CNT_bit=0;
    uint8 i;

   front_Val=(value&0xFFC00000);
   back_Val=(value&0x001FFFFF);
   back_Val|=(front_Val>>1);
   exclude_Fram_CNT_bit=back_Val>>5;
   
   for(i=0;i<26;i++)
     {
      if(exclude_Fram_CNT_bit&0x1)
      {
         Array[26-1-i]=1;
      }
      else
      {
         Array[26-1-i]=0;
      }
      exclude_Fram_CNT_bit=exclude_Fram_CNT_bit>>1;
     }
   return Array;
}
 
/* 7개 read 를 위해 8번 읽어옴 */
void L9369_Read_Period(const uint8 *addr)
{
    uint32_t retData = 0x00U;
	uint8 i = 0U;
	ConfigSpiPin(PORT_PIN_C15, PORT_PIN_C14, PORT_PIN_C13, PORT_PIN_C12);
	
	if (g_L9369_REG_MISO.field.spi_err == 1) {
		g_L9369_REG_MOSI.field.cnt = (g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U : 0x01U;
	}

    for (i = 0; i < NUM_ADDRS + 1; i++) {
        
		g_L9369_REG_MOSI.field.addr = addr[i];
		g_L9369_REG_MOSI.field.r_w = 0x00U; 
		g_L9369_REG_MOSI.field.tm = 0x00U;
		g_L9369_REG_MOSI.field.data = 0x00U;
		g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);
			
        retData = SPI_Read32(g_L9369_REG_MOSI.data32);

        g_L9369_REG_MISO.data32 = retData;

        L9369CheckStatus(retData);
		
		g_L9369_REG_MOSI.field.cnt = !g_L9369_REG_MOSI.field.cnt;
		
    }
}

void L9369_Write(uint32_t RxData)
{
	ConfigSpiPin(PORT_PIN_C15, PORT_PIN_C14, PORT_PIN_C13,PORT_PIN_C12);

	if(g_L9369_REG_MISO.field.spi_err == 1)
	{
		g_L9369_REG_MOSI.field.cnt = (uint8)((g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U: 0x01U);
	}

	g_L9369_REG_MOSI.data32 = RxData;
	
	SPI_Write32(g_L9369_REG_MOSI.data32);
	
	g_L9369_REG_MOSI.field.cnt = !g_L9369_REG_MOSI.field.cnt;
}  

/* n개 read를 위해 n+1번 읽어옴 */
void L9369_Read(const uint8 *addr ,uint8 Num)
{
    uint32_t retData = 0x00U;
	uint8 i = 0U;

    ConfigSpiPin(PORT_PIN_C15, PORT_PIN_C14, PORT_PIN_C13, PORT_PIN_C12);

    for (i = 0; i < Num; i++) {
		
		if (g_L9369_REG_MISO.field.spi_err == 1) {
			g_L9369_REG_MOSI.field.cnt = (g_L9369_REG_MOSI.field.cnt == 0x01U) ? 0x00U : 0x01U;
		}

		g_L9369_REG_MOSI.field.addr = addr[i];
		g_L9369_REG_MOSI.field.r_w = 0x00U; 
		g_L9369_REG_MOSI.field.tm = 0x00U;
		g_L9369_REG_MOSI.field.data = 0x00U;
		g_L9369_REG_MOSI.field.crc = L9369MakeCRC5(g_L9369_REG_MOSI.data32);

		retData = SPI_Read32(g_L9369_REG_MOSI.data32);

		g_L9369_REG_MISO.data32 = retData;

		L9369CheckStatus(retData);

		g_L9369_REG_MOSI.field.cnt = !g_L9369_REG_MOSI.field.cnt;
    }
}

