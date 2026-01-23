/************************ (C) COPYRIGHT 2007  **********************************
* File Name				: swspi.c
* Author						: Seungbum Han
* Date First Issued	: 2019-05-20 
* Description				: Switch Selector SPI Source file
********************************************************************************
* History:
* 2017-01-02  v0.01
********************************************************************************
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "Port_Api.h"
#include "SWspi.h"

unsigned int sw_command;                  // Command byte variable
unsigned int sw_Con_address;               // Address word variable
uint32_t sw_buffer; 
uint8_t	SPI_CS,SPI_SCK,SPI_MISO,SPI_MOSI;

extern void SPI_Delay(uword ms);

/********************************************************************
 * Function:        void ConfigSpiPin(ubyte PinCS,ubyte PinSCK,ubyte PinMOSI,ubyte PinMISO)
 *
 * Description:     The SPI pin can to match through this function 
 *******************************************************************/

void ConfigSpiPin(ubyte PinCS,ubyte PinSCK,ubyte PinMOSI,ubyte PinMISO)
{
	SPI_CS = PinCS;
	SPI_SCK = PinSCK;
	SPI_MOSI = PinMOSI;
	SPI_MISO = PinMISO;
}


void SW_ByteOut16(uint16_t Data)
{
	unsigned char i;

	for (i = 0; i < 16; i++)			// Loop through each bit
	{
		SIU.GPDO[SPI_SCK].R = 0;
		SIU.GPDO[SPI_SCK].R = 0;
		SIU.GPDO[SPI_SCK].R = 0;
		
		if (Data & 0x8000)			// Check if next bit is a 1
		{
			SIU.GPDO[SPI_MOSI].R = 1;	// send a 1
			SIU.GPDO[SPI_MOSI].R = 1;
		}
		else
		{
			SIU.GPDO[SPI_MOSI].R = 0;	// Send a 0
		}
		
		
		SIU.GPDO[SPI_SCK].R = 1;
		SIU.GPDO[SPI_SCK].R = 1;
		SIU.GPDO[SPI_SCK].R = 1;
		
		
		Data = Data << 1;						// Rotate left for next bit
	}
}

/********************************************************************
 * Function:        void bytein(uint32_t Data)
 *
 * Description:     This function inputs a byte from the serial
 *                  EEPROM device and stores it in buffer.
 *******************************************************************/

uint16_t SW_ByteIn16(uint16_t Data)
{
	uint16_t retVal;
	unsigned char i;
		


	retVal = 0;
	for (i = 0; i < 16; i++)
	{
		retVal = retVal << 1;	
		//SPI_Delay(1);// Rotate left for next bit
		SIU.GPDO[SPI_SCK].R = 0;			// Bring SCK to latch data
		SIU.GPDO[SPI_SCK].R = 0;
		SIU.GPDO[SPI_SCK].R = 0;
		
		//SPI_Delay(1);
		if (Data & 0x8000)			// Check if next bit is a 1
		{
			SIU.GPDO[SPI_MOSI].R = 1;	// send a 1
			SIU.GPDO[SPI_MOSI].R = 1;
		}
		else
		{
			SIU.GPDO[SPI_MOSI].R = 0;	// Send a 0
			SIU.GPDO[SPI_MOSI].R = 0;
		}
		
		//SPI_Delay(1);
		SIU.GPDO[SPI_SCK].R = 1;			// Bring SCK low
		SIU.GPDO[SPI_SCK].R = 1;
		SIU.GPDO[SPI_SCK].R = 1;
		
		if(SIU.GPDI[SPI_MISO].R == 1)	// Check if DI is high
		{
			retVal |= 0x01;					// If high, set next bit
		}
		//SPI_Delay(1);

		Data = Data << 1;						// Rotate left for next bit
	}
	
	return retVal;
}

/********************************************************************
 * Function:        uint32_t SPI_Write32(uint32_t Data)
 *
 * Description:     This function writes the 32-bit data tthrough the SPI.
 *******************************************************************/
void SPI_Write16(uint16_t Data)
{
	SIU.GPDO[SPI_CS].R = 0;
	SIU.GPDO[SPI_CS].R = 0;
	SPI_Delay(1);
	SW_ByteOut16(Data);
	SIU.GPDO[SPI_SCK].R = 0;
	SIU.GPDO[SPI_SCK].R = 0;
	SPI_Delay(1);
	SIU.GPDO[SPI_CS].R = 1;						// Disable Chip Select
	SIU.GPDO[SPI_CS].R = 1;
	SPI_Delay(1);
}

/********************************************************************
 * Function:        uint32_t SPI_Read32(uint32_t Address)
 *
 * Description:     This function read the 32-bit data from the SPI.
 *******************************************************************/
uint16_t SPI_Read16(uint16_t Address)
{
	uint16_t RetVal = 0;				// Return value variable
	SIU.GPDO[SPI_CS].R = 0;			//gpio pin data output // Enable Chip Select
	SIU.GPDO[SPI_CS].R = 0; 
	SPI_Delay(1);
	RetVal = SW_ByteIn16(Address);				// Read 32bit data
	SIU.GPDO[SPI_SCK].R = 0;
	SIU.GPDO[SPI_SCK].R = 0;
	SPI_Delay(1);
	SIU.GPDO[SPI_CS].R = 1;			// Disable Chip Select
	SIU.GPDO[SPI_CS].R = 1;
	SPI_Delay(1);
	return RetVal;							// Return value
}
void SW_ByteOut32(uint32_t Data)
{
	unsigned char i;

	for (i = 0; i < 32; i++)			// Loop through each bit
	{
		SIU.GPDO[SPI_SCK].R = 0;
		SIU.GPDO[SPI_SCK].R = 0;
		SIU.GPDO[SPI_SCK].R = 0;
		SPI_Delay(10);
		if (Data & 0x80000000)			// Check if next bit is a 1
		{
			SIU.GPDO[SPI_MOSI].R = 1;	// send a 1
			SIU.GPDO[SPI_MOSI].R = 1;
		}
		else
		{
			SIU.GPDO[SPI_MOSI].R = 0;	// Send a 0
		}
		
		SIU.GPDO[SPI_SCK].R = 1;
		SIU.GPDO[SPI_SCK].R = 1;
		SIU.GPDO[SPI_SCK].R = 1;
		SPI_Delay(10);
		Data = Data << 1;						// Rotate left for next bit
	}
}

/********************************************************************
 * Function:        void bytein(uint32_t Data)
 *
 * Description:     This function inputs a byte from the serial
 *                  EEPROM device and stores it in buffer.
 *******************************************************************/

uint32_t SW_ByteIn32(uint32_t Data)
{
	unsigned char i;
	uint32_t retVal;
	
	retVal = 0;
	for (i = 0; i < 32; i++)
	{
		retVal = retVal << 1;	
		SIU.GPDO[SPI_SCK].R = 0;			// Bring SCK to latch data
		SIU.GPDO[SPI_SCK].R = 0;
		SPI_Delay(10);

		if (Data & 0x80000000)			// Check if next bit is a 1
		{
			SIU.GPDO[SPI_MOSI].R = 1;	// send a 1
			SIU.GPDO[SPI_MOSI].R = 1;
		}
		else
		{
			SIU.GPDO[SPI_MOSI].R = 0;	// Send a 0
			SIU.GPDO[SPI_MOSI].R = 0;
		}
		
		
		SIU.GPDO[SPI_SCK].R = 1;			// Bring SCK low
		SIU.GPDO[SPI_SCK].R = 1;
		SPI_Delay(10);
		if(SIU.GPDI[SPI_MISO].R == 1)	// Check if DI is high
		{
			retVal |= 0x01;					// If high, set next bit
		}

		Data = Data << 1;						// Rotate left for next bit
	}

	return retVal;
}

/********************************************************************
 * Function:        uint32_t SPI_Write32(uint32_t Data)
 *
 * Description:     This function writes the 32-bit data tthrough the SPI.
 *******************************************************************/
void SPI_Write32(uint32_t Data)
{
	SIU.GPDO[SPI_CS].R = 0;
	SIU.GPDO[SPI_CS].R = 0;
	SW_ByteOut32(Data);
	SIU.GPDO[SPI_SCK].R = 0;
	SIU.GPDO[SPI_SCK].R = 0;
	SPI_Delay(1);
	SIU.GPDO[SPI_CS].R = 1;						// Disable Chip Select
	SIU.GPDO[SPI_CS].R = 1;
	SPI_Delay(1);

}

/********************************************************************
 * Function:        uint32_t SPI_Read32(uint32_t Address)
 *
 * Description:     This function read the 32-bit data from the SPI.
 *******************************************************************/


uint32_t SPI_Read32(uint32_t Address)
{
	uint32_t RetVal = 0;				// Return value variable
	
	SIU.GPDO[SPI_CS].R = 0;			//gpio pin data output // Enable Chip Select
	SIU.GPDO[SPI_CS].R = 0;
	RetVal = SW_ByteIn32(Address);				// Read 32bit data
	SIU.GPDO[SPI_SCK].R = 0;
	SIU.GPDO[SPI_SCK].R = 0;
	SPI_Delay(1);
	SIU.GPDO[SPI_CS].R = 1;			// Disable Chip Select
	SIU.GPDO[SPI_CS].R = 1;
	SPI_Delay(1);
	return RetVal;							// Return value
}

