/************************ (C) COPYRIGHT 2007  **********************************
* File Name				: linflex_uart.c
* Author					: Gabriel Kang
* Date First Issued			: 2017-02-01 
* Description				: UART Configuration C Source File
********************************************************************************
* History:
* 2017-02-01  v0.01
********************************************************************************
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "linflex_uart.h"
#include "Interface.h"
#include "Can_Api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define	RxHeader			0x55
#define	ResponseHeader		0xAA

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char LINFLEX_Uart_Rx_Buff[10] = { 0, };
static int Tx_Index;
static int Rx_Index;
extern Data_Packet RxBuffer[10];

/* Private function prototypes -----------------------------------------------*/
void LinFlex_0_Int_Config( void );
static void ConfigureSciPins(void);
static void Linflex_SetSpeed(void);

/* Private functions ---------------------------------------------------------*/

/********************************************************************************
* Function Name	: ConfigureSciPins
* Description		: UART Pin Configuration
* input			: None
* return			: None
********************************************************************************/
static void ConfigureSciPins( void )
{
   // TX_A output
   /* set PA=1, OBE=1, IBE=0, ODE=0, HYS=0, SRC=0, WPE=0, WPS=0 */
   SIU.PCR[PAD_LIN0_TXD].B.PA  =0x1;
   SIU.PCR[PAD_LIN0_TXD].B.OBE =0x1;
	 
   // RX_A input
   /* set PA=1, OBE=0, IBE=1, ODE=0, HYS=0, SRC=0, WPE=0, WPS=0 */
   SIU.PCR[PAD_LIN0_RXD].B.PA  =0x1;
   SIU.PCR[PAD_LIN0_RXD].B.IBE =0x1;	
}

/********************************************************************************
* Function Name	: Linflex_SetSpeed
* Description		: UART0 Baudrate Configuration according as system clock
* input			: None
* return			: None
********************************************************************************/
static void Linflex_SetSpeed( void )
{
#if (FSYS == FSYS_08_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 8Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x34;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x1;			
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 8Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x1A;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x1;			
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 8Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0xD;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x0;			
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 8Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x4;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x5;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */
   
#elif (FSYS == FSYS_16_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 16Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x68;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x3;			
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 16Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x34;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x1;			
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 16Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x1A;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x1;			
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 16Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x8;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0xB;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */

#elif (FSYS == FSYS_32_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 32Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0xD0;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x5;			
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 32Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x68;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x3;			
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 32Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x34;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x1;			
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 32Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x11;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x6;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */

#elif (FSYS == FSYS_40_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 40Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x104;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x7;			
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 40Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x82;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x3;
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 40Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x41;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x4;
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 40Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x15;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0xB;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */

#elif (FSYS == FSYS_48_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 48Mz  */
	   // 16 * 9600 = 153600
	   // LFDIV = 48000000 / 153600 = 312.5
	   // DIV_M = 312
	   // DIV_F = 16 x 0.5
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x138;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x8;			
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 48Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x9C;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x4;
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 48Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x4E;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x2;
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 48Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x1A;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x1;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */

#elif (FSYS == FSYS_64_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x1A0;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0xB;
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0xD0;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x5;
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x68;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x3;
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x22;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0xC;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */

#elif (FSYS == FSYS_80_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x208;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0xD;
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x104;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x7;
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x82;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x3;
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x2B;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x6;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */
   
#elif (FSYS == FSYS_120_MHz)    
	#if (UART_BAUDRATE == UART_9600_BAUDRATE)    
	   /* baudrate = 9600 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x30D;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x4;
	#elif (UART_BAUDRATE == UART_19200_BAUDRATE)    
	   /* baudrate = 19200 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x186;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0xA;
	#elif (UART_BAUDRATE == UART_38400_BAUDRATE)    
	   /* baudrate = 38400 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0xC3;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x5;
	#elif (UART_BAUDRATE == UART_115200_BAUDRATE)    
	   /* baudrate = 115200 with Fsys = 64Mz  */
	   LINFLEX_0.LINIBRR.B.DIV_M = 0x41;
	   LINFLEX_0.LINFBRR.B.DIV_F = 0x2;
	#else
		#error "Please choose correct UART Baudrate"
	#endif /*endif for UART_BAUDRATE */
#else
	#error "Please choose correct System clock"
#endif
   
}

/********************************************************************************
* Function Name	: LinFlex_0_Int_Config
* Description		: UART0 Interrupt configuration
* input			: None
* return			: None
********************************************************************************/
void LinFlex_0_Int_Config( void )
{
	LINFLEX_0.LINIER.B.DRIE = 1;	// Receive Interrupt Enable
//	LINFLEX_0.LINIER.B.DTIE = 1;	// Transmit Interrupt Enable
	
	return;
}

/********************************************************************************
* Function Name	: LinFlex_0_Init
* Description		: UART0 Configuration
* input			: None
* return			: None
********************************************************************************/
void LinFlex_0_Init( void )
{
	Tx_Index = 0;
	ConfigureSciPins();

	// Enter initialization mode by setting INIT bit
	LINFLEX_0.LINCR1.R = 0x1;

	LINFLEX_0.UARTCR.B.UART = 0x1;		//Linflex working in UART mode

	LINFLEX_0.UARTCR.B.TXEN = 0x1;		// Enable transmission of data now
	LINFLEX_0.UARTCR.B.RXEN = 0x1;		//Receiver enabled
	LINFLEX_0.UARTCR.B.WL   = 0x1;		//8 bit data
	LINFLEX_0.UARTCR.B.PCE  = 0x0;		//Use parity for sub MCU system boot loader
	LINFLEX_0.UARTCR.B.OP	= 0x0;		// even parity bit use for sub MCU system boot loader

	Linflex_SetSpeed();

	// Leave initialization mode by clearing INIT bit
	LINFLEX_0.LINCR1.R = 0x0;

//	INTC.PSR[79].R = 3;					//RX Interrupt
//   INTC.PSR[80].R = 3;					//TX Interrupt
	LinFlex_0_Int_Config();
   return;
}

/********************************************************************************
* Function Name	: LinFlex_0_RX_ISR
* Description		: UART RX Interrupt Routine
********************************************************************************/
uint8_t CntRxBuf = 0,CntRxData = 0;
void LinFlex_0_RX_ISR( void )
{
#if 0
	uint8_t Temp;
	
	//IVT 79
	// Wait for the reception to complete   
/*	
	while(LINFLEX_0.UARTSR.B.DRF == 0) {
		asm("nop");
	}
*/
	RxBuffer[CntRxBuf].Data[CntRxData] = (uint8_t) LINFLEX_0.BDRM.R;
	
	if(CntRxData > 0)
	{
		if(RxBuffer[CntRxBuf].Data[0] == 0x55)							// Header Check
		{
			if(RxBuffer[CntRxBuf].Data[1] < 10)								// Task ID range check
			{
				Temp = RxBuffer[CntRxBuf].Data[1];
				RxBuffer[Temp].Data[0] = RxBuffer[CntRxBuf].Data[0];
				RxBuffer[Temp].Data[1] = RxBuffer[CntRxBuf].Data[1];
				CntRxBuf = Temp;
				
				if(RxBuffer[CntRxBuf].Data[1] != ID_Version)
				{
					if(CntRxData > 3)
					{
						Bit_RxCompletion = 1;
						CntRxData = 0xFF;
						if(++CntRxBuf > 9)
						{
							CntRxBuf = 0;
						}
					}
				}
			}
			else
			{
				CntRxBuf = 0;
				CntRxData = 0xFF;						// Task ID value error
			}
		}
		else if(RxBuffer[CntRxBuf].Data[1] == 0x55)					// Data 중간부터 수신할 경우.
		{
			RxBuffer[CntRxBuf].Data[0] = RxBuffer[CntRxBuf].Data[1];
			CntRxData = 0;
		}
		else																// Header Value error
		{
			CntRxData = 0xFF;
			CntRxBuf = 0;
		}
	}

	++CntRxData;
	if(CntRxData > 7)											// Task ID가 ID_Version일 경우는 CntRxData > 7 일수 있음.
	{
		CntRxData = 0;
		Bit_RxCompletion = 1;
		CntRxBuf = 0;
	}

#if 0
	if(!Bit_HeaderCheck)
	{
		TempHeader = (uint8_t) LINFLEX_0.BDRM.R;
		CntRxData = 0;
		Bit_HeaderCheck = 1;
	  if(TempHeader == RxHeader)
	  {
				Bit_RxTxHeader = 1;
	  }
		else if(TempHeader == ResponseHeader)
		{
				Bit_RxTxHeader = 0;
	  }
		else
		{
				Bit_HeaderCheck = 0;
	  }
	}
	else
	{
		if(Bit_RxTxHeader)
		{
			++CntRxData;
			if(CntRxData > 4)
			{
				Bit_HeaderCheck = 0;
				Bit_RxCompletion = 1;
				Bit_RxResponse = 0;
			}
		}
		else
		{
			++CntRxData;
			if(CntRxData > 1)
			{
				Bit_HeaderCheck = 0;
				Bit_RxCompletion = 1;
				Bit_RxResponse = 1;
			}
		}
	}
#endif

	// Clear RMB (Release Message Buffer) and DRF (Data Reception Completed)
	//  flags, which are set when receiving data in UART mode
	LINFLEX_0.UARTSR.B.RMB = 1;
	LINFLEX_0.UARTSR.B.DRF = 1;
	//	LINFLEX_0.LINIER.B.DRIE = 1;	// Receive Interrupt Enable
	return;
}

#if 0
/********************************************************************************
* Function Name	: LinFlex_0_TX_ISR
* Description		: UART Tx Interrupt Routine
********************************************************************************/
void LinFlex_0_TX_ISR( void )
{
	//IVT 80
//	LINFLEX_0.UARTCR.B.TXEN = 1;

	// Wait for the transmission to complete
	while (LINFLEX_0.UARTSR.B.DTF == 0) {
		asm("nop");
	}

	LINFLEX_0.UARTSR.B.DTF = 1;		//clear the trasmission completed flag	
//	LINFLEX_0.UARTCR.B.TXEN = 0;		// Disable transmission

	return;
}

/********************************************************************
 *	SCI_A
 ********************************************************************/
char sci_a_getchar (void)
{
#if 0
	char cDummy;
	// Wait for the reception to complete   
	while(LINFLEX_0.UARTSR.B.DRF == 0) {
		asm("nop");
	}

	cDummy = (uint8_t) LINFLEX_0.BDRM.R;  // Retrieve data received from LinFlex	

	// Clear RMB (Release Message Buffer) and DRF (Data Reception Completed)
	//  flags, which are set when receiving data in UART mode
	LINFLEX_0.UARTSR.B.RMB = 1;
	LINFLEX_0.UARTSR.B.DRF = 1;
	return cDummy;
#else
	return LINFLEX_Uart_Rx_Buff[Rx_Index-1];
#endif

}

/********************************************************************/
void sci_a_putchar (char ch)
{
#if 0
	LINFLEX_0.UARTCR.B.TXEN = 1;
	LINFLEX_0.BDRL.R = (uint32_t) ch; // Start transmission by writing the data 
	
	// Wait for the transmission to complete
	while (LINFLEX_0.UARTSR.B.DTF == 0) {
		asm("nop");
	}
	
	LINFLEX_0.UARTSR.B.DTF = 1;		//clear the trasmission completed flag	
	LINFLEX_0.UARTCR.B.TXEN = 0;	// Disable transmission
#else
	LINFLEX_0.UARTCR.B.TXEN = 1;
	LINFLEX_0.BDRL.R = (uint32_t) ch; // Start transmission by writing the data 

#endif
}
/********************************************************************/
#endif
#endif
}
