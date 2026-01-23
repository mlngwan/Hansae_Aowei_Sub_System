/************************ (C) COPYRIGHT 2007  **********************************
* File Name				: dspi.c
* Author						: Gabriel Kang
* Date First Issued	: 2017-01-02 
* Description				: SPI Source file
********************************************************************************
* History:
* 2017-01-02  v0.01
********************************************************************************
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#include "Dspi_Api.h"
#include "Spi_L9369Data_Types.h"
#include "Spi_ExtEEPROM_Api.h"
#include "Spi_TLE9461_Data_Types.h"
#include "Port_Api.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define	_Use_Manual
#define	_For_1Kbit
//#define	_For_4Kbit

#ifdef	_For_1Kbit
#define NUMBITS     9                  // # of bits of EWEN cmd.
#endif

#ifdef	_For_4Kbit
#define NUMBITS     11                  // # of bits of EWEN cmd.
#endif

/* Private macro -------------------------------------------------------------*/
//#define	EEP_SCK(Status)		((Status) ? PORT_EPR_CLK = 1 : PORT_EPR_CLK = 0)				//1 SPI Clock Control pin of EEPROM 
#define	EEP_MISO			PORT_CAN_EEPROM_MISO													//1 MISO pin of EEPROM 
//#define	EEP_MOSI(Status)	((Status) ? PORT_EPR_MOSI = 1 : PORT_EPR_MOSI = 0)				//1 MOSI pin of EEPROM 
//#define	EEP_CS(Status)		((Status) ? PORT_SUB_EEPROM_CS = 1 : PORT_SUB_EEPROM_CS = 0)				//1 SPI Chip Select pin of EEPROM 

/* Private variables ---------------------------------------------------------*/
unsigned int command;                  // Command byte variable
unsigned int Con_address;               // Address word variable
unsigned char buffer; 
uword retVal = 0;
extern ExtEEPROM_t g_ExtEEPROM;

/* Private function prototypes -----------------------------------------------*/
void SendCommand(void);
unsigned int ReadX16(void);
void WriteX16(uint16_t data);
void SPI_EEP_poll(void);
void SendCmd(ubyte cmd);
void byteout(void);
void bitout(void);
void bytein(void);
void SPI_Delay(uword ms);


/* Private functions ---------------------------------------------------------*/
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************
void SPI_Delay(uword ms)
{
	while(ms){
		--ms;
	}
}

/********************************************************************
 * Function:        void SendCmd(ubyte cmd)
 *
 * Description:     This function sends an EWEN command to the device.
 *                  Once this command has been given, writing to the
 *                  device array will be enabled, and will remain as
 *                  such until an EWDS command is given or power is
 *                  removed from the device.
 *******************************************************************/
void SendCmd(ubyte cmd)
{
	command = cmd;							// Load EWEN command value
	Con_address = 0;						// Clear address word
	PORT_SUB_EEPROM_CS = 1;						// Enable Chip Select
	SendCommand();							// Output command to device
	PORT_SUB_EEPROM_CS = 0;						// Disable Chip Select
}

/********************************************************************
 * Function:        void WriteX16(unsigned int data)
 *
 * Description:     This function writes the 16-bit value stored in
 *                  data to the serial EEPROM device, at the location
 *                  specified by address.
 *******************************************************************/
void WriteX16(uint16_t data)
{
	command = WRITE_CMD;				// Load WRITE command value
	PORT_SUB_EEPROM_CS = 1;						// Enable Chip Select
	SendCommand();							// Output command to device
	buffer = (char)(data >> 8);	// Copy data MSB to buffer
	byteout();									// Output byte
	buffer = (char)data;				// Copy data LSB to buffer
	byteout();									// Output byte
	PORT_SUB_EEPROM_CS = 0;						// Disable Chip Select
	SPI_EEP_poll();							// Begin ready/busy polling
}

/********************************************************************
 * Function:        unsigned int ReadX16(void)
 *
 * Description:     This function reads a 16-bit value from the
 *                  serial EEPROM device, from the location
 *                  specified by address, returns it.
 *******************************************************************/
unsigned int ReadX16(void)
{
	unsigned int retval;				// Return value variable

	command = READ_CMD;					// Load READ command value
	PORT_SUB_EEPROM_CS = 1;						// Enable Chip Select
	SendCommand();							// Output command to device
	bytein();										// Input byte from device
	retval = buffer;						// Copy byte to retval MSB
	bytein();										// Input byte from device
	retval = (retval << 8) | buffer;	// Copy byte to retval LSB
	PORT_SUB_EEPROM_CS = 0;						// Disable Chip Select
	return retval;							// Return value
}

/********************************************************************
 * Function:        void SPI_EEP_poll(void)
 *
 * Description:     This function brings CS high to initiate the
 *                  Ready/Busy polling feature. DO is then continuously
 *                  polled to see when it goes high, thus indicating
 *                  that the write cycle has completed.
 *******************************************************************/
ubyte	Cnt_EepromTimeOut;
void SPI_EEP_poll(void)
{
	PORT_SUB_EEPROM_CS = 1;						// Set CS high
	SPI_Delay(1);								// Avoid violating Tsv
	Cnt_EepromTimeOut = 0;
	while (EEP_MISO == 0)				// Wait until DI is high
	{
		if(Cnt_EepromTimeOut > 9)	// Timeout 10ms
		{
			g_ExtEEPROM.fEepromTimeOut = 1;
			break;
		}
	};
	PORT_SUB_EEPROM_CS = 0;						// Bring CS low
}

/********************************************************************
 * Function:        void bitout(void)
 *
 * Description:     This function outputs the MSb of buffer to the
 *                  serial EEPROM device.
 *******************************************************************/
void bitout(void)
{
	if (buffer & 0x80)					// Check if next bit is a 1
	{
		PORT_CAN_EEPROM_MOSI = 1;				// If so, send a 1
	}
	else												// Otherwise
	{
		PORT_CAN_EEPROM_MOSI = 0;				// Send a 0
	}
	PORT_CAN_EEPROM_SCLK = 1;						// Bring SCK high to latch data
	SPI_Delay(1);								// Avoid violating Tckh
	PORT_CAN_EEPROM_SCLK = 0;						// Bring SCK low for next bit
}

/********************************************************************
 * Function:        void bitin(void)
 *
 * Description:     This function inputs a bit from the serial EEPROM
 *                  device and stores it in the LSb of buffer.
 *******************************************************************/
void bitin(void)
{
	buffer &= 0xFE;							// Assume next bit will be 0
	PORT_CAN_EEPROM_SCLK = 1;						// Bring SCK to latch data
	SPI_Delay(1);								// Avoid violating Tckh
	PORT_CAN_EEPROM_SCLK = 0;						// Bring SCK low
	if (EEP_MISO == 1)					// Check if DI is high
	{
		buffer |= 0x01;						// If high, set next bit
	}
}

/********************************************************************
 * Function:        void byteout(void)
 *
 * Description:     This function outputs the byte specified in
 *                  buffer to the serial EEPROM device.
 *******************************************************************/
void byteout(void)
{
	unsigned char i;						// Loop counter

	for (i = 0; i < 8; i++)			// Loop through each bit
	{
		bitout();									// Output bit
		buffer = buffer << 1;			// Rotate left for next bit
	}
}

/********************************************************************
 * Function:        void bytein(void)
 *
 * Description:     This function inputs a byte from the serial
 *                  EEPROM device and stores it in buffer.
 *******************************************************************/
void bytein(void)
{
	unsigned char i;						// Loop counter

	buffer = 0;
	for (i = 0; i < 8; i++)			// Loop through each bit
	{
		buffer = buffer << 1;			// Rotate left for next bit
		bitin();									// Input bit
	}
}

/********************************************************************
 * Function:        void SendCommand(void)
 * 
 * Description:     This function sends the Start bit and opcode
 *                  specified in the MSb's of command, as well as
 *                  the required number of address or dummy bits,
 *                  to the serial EEPROM device.
 *******************************************************************/
void SendCommand(void)
{
	static unsigned char i;             // Loop counter
	static unsigned int cmd_addr;       // Variable for command & address
	static unsigned int temp;           // Temp. variable

	cmd_addr = Con_address;             // Copy address to cmd_addr;
	// First, align address bits to be combined with command
	for (i = 0; i < (16-g_ExtEEPROM.Eeprom_BitSize); i++)  // Skip through unused addr. bits
	{
	  cmd_addr = cmd_addr << 1;					// Rotate left to skip bit
	}

	// Next, combine command into address word
	cmd_addr &= 0x1FFF;                 // Mask off upper 3 bits
	temp = command;                     // Copy command value to temp
	cmd_addr |= (temp<<8);              // Combine address & command

	// Finally, output entire command to device
	for (i = 0; i < g_ExtEEPROM.Eeprom_BitSize; i++)       // Loop through each bit
	{
	  buffer = (char)(cmd_addr >> 8); // Copy address MSB to buffer
	  bitout();                       // Output next bit
	  cmd_addr = cmd_addr << 1;       // Rotate left for next bit
	}
}

/********************************************************************************
* Function Name		: SPI_EEP_Send_X16
* Description			: Send the 16bit data to EEPROM
* input						: ubyte Address, uint16_t Data
* return					: None
********************************************************************************/
void SPI_EEP_Send_X16(ubyte Address, uint16_t Data, ubyte cmd)
{
	uword data;

	if(cmd == ERAL_CMD)
	{
		SendCmd(EWEN_CMD);             			// Send Write enable command
		//SendCmd(cmd);                  			// Send erase all command
	}
	else
	{
		SendCmd(cmd);                  			// Send command
	}
	data = Data;                        // Assign 0x55 to data
	Con_address = Address;              // Assign 0x10 to address
	WriteX16(data);                     // Output data byte
	SendCmd(EWDS_CMD);                  // Send EWDS command
}

/********************************************************************************
* Function Name		: SPI_EEP_Read_X16
* Description			: Read the 16bit data from EEPROM
* input						: ubyte Address
* return					: uint16_t
********************************************************************************/
uint16_t SPI_EEP_Read_X16(ubyte Address)
{
	uword data;

	Con_address = Address;
	data = ReadX16();                   // Input data byte
	return data;
}

void TC277Init(void)
{
	DSPI_1.MCR.R = 0x00028001; // MSTR = 0 (Slave), HALT = 1
	DSPI_1.CTAR[0].R = 0x7A000000; // FMSZ = 15 (16비트), CPOL = 0, CPHA = 1
	/* inactive state SCK is low, Data is changed on the leading edge of SCK and captured on the following edge */
	DSPI_1.RSER.R = 0x00000000; // RFDF (Receive FIFO Drain Flag) 인터럽트 비 활성화
	DSPI_1.PUSHR.R = 0x00000000; // 송신할 데이터초기화 
	DSPI_1.SR.R = 0xFFFFFFFF;	 /* 상태 플래그 초기화 */ 
	DSPI_1.MCR.B.HALT = 0; // DSPI1 활성화,  HALT = 0 
}


void SpiInitialize(void)
{	
	TC277Init();
	ExtEEPROMInit();
	CANSBCInit();
}



