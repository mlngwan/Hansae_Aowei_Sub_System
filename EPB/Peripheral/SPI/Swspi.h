/************************ (C) COPYRIGHT 2007  **********************************
* File Name				: spi.h
* Author					: Seungbum Han
* Date First Issued			: 2019-05-20 
* Description				: Switch SPI Declaration header file
********************************************************************************
* History:
* 2017-01-02  v0.01
********************************************************************************
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Reg_SpiCheck					0b00000000
#define	Reg_DeviceConfig			0b00000010
#define Reg_ReadSwitchSta			0b00111110
#define	Reg_Reset							0b01001000
#define Reg_TriStateSP				0b00000100
#define Reg_TriStateSG				0b00000110
#define Reg_WettingCurSP			0b00010110
#define Reg_WettingCurSG			0b00011000
#define Reg_LpmConfig					0b00011110
#define Reg_LpmEnterCmd				0b00111000

#define Reg_Wakeup_SG					0b00100010
#define Reg_Wakeup_SP					0b00100000
#define Reg_INT_SG					0b00011100
#define Reg_INT_SP					0b00011010


/*Device Config*/
#define DevConf_VBATP 		0x001000 // 1 = disable , 0 = enable
#define DevConf_WakeB 		0x000800 // bit11 1 = , 0 = 
#define DevConf_SP7 			0x000080
#define DevConf_SP6 			0x000040
#define DevConf_SP5 			0x000020
#define DevConf_SP4 			0x000010
#define DevConf_SP3 			0x000008
#define DevConf_SP2 			0x000004
#define DevConf_SP1 			0x000002
#define DevConf_SP0 			0x000001
#define DevConf_EnableAll 0x0008FF // VBATP disable & wake_B & SP all
//18FF

/*Tri-State SP*/
#define Tri_SP7						0x000080
#define Tri_SP6						0x000040
#define Tri_SP5						0x000020
#define Tri_SP4						0x000010
#define Tri_SP3						0x000008
#define Tri_SP2						0x000004
#define Tri_SP1						0x000002
#define Tri_SP0						0x000001
#define Tri_SP_EnableAll	0x0000FF

/*Tri-State SG*/
#define Tri_SG13					0x002000
#define Tri_SG12					0x001000
#define Tri_SG11					0x000800
#define Tri_SG10					0x000400
#define Tri_SG9						0x000200
#define Tri_SG8						0x000100
#define Tri_SG7						0x000080
#define Tri_SG6						0x000040
#define Tri_SG5						0x000020
#define Tri_SG4						0x000010
#define Tri_SG3						0x000008
#define Tri_SG2						0x000004
#define Tri_SG1						0x000002
#define Tri_SG0						0x000001
#define Tri_SG_EnableAll	0x001EFF	//SG13, SG8 Disable


/*Continuous Wetting Current level  SP*/
#define WettingCur_SP7		0x000080
#define WettingCur_SP6		0x000040
#define WettingCur_SP5		0x000020
#define WettingCur_SP4		0x000010
#define WettingCur_SP3		0x000008
#define WettingCur_SP2		0x000004
#define WettingCur_SP1		0x000002
#define WettingCur_SP0		0x000001
#define WettingCur_SP_EnableAll	0x0000D0	// FF


/*Continuous Wetting Current level  SG*/
#define WettingCur_SG13		0x002000
#define WettingCur_SG12		0x001000
#define WettingCur_SG11		0x000800
#define WettingCur_SG10		0x000400
#define WettingCur_SG9		0x000200
#define WettingCur_SG8		0x000100
#define WettingCur_SG7		0x000080
#define WettingCur_SG6		0x000040
#define WettingCur_SG5		0x000020
#define WettingCur_SG4		0x000010
#define WettingCur_SG3		0x000008
#define WettingCur_SG2		0x000004
#define WettingCur_SG1		0x000002
#define WettingCur_SG0		0x000001
#define WettingCur_SG_EnableAll 0x002000


/*Low Power mode poll time set */
#define LPM_poll_3ms			0x000000
#define LPM_poll_6ms			0x000001
#define LPM_poll_12ms			0x000002
#define LPM_poll_24ms			0x000003
#define LPM_poll_48ms			0x000004
#define LPM_poll_68ms			0x000005
#define LPM_poll_64ms			0x00000F
//... 76 128 32 36 40 44 52 56 60 64 (+1 ~ max F)
extern uint8_t	SPI_CS,SPI_SCK,SPI_MISO,SPI_MOSI;

void initSWSPI0(void);
extern void SPI_Delay (uword ms);
void SPI_Write16 (uint16_t Data);
uint16_t SPI_Read16( uint16_t Address);
void SPI_Write32 (uint32_t Data);
uint32_t SPI_Read32 (uint32_t Address);
extern void ConfigSpiPin(ubyte PinCS,ubyte PinSCK,ubyte PinMOSI,ubyte PinMISO);

