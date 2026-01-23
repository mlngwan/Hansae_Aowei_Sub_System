#ifndef _LINFLEX_INCLUDED_
#define _LINFLEX_INCLUDED_

#define UART_SendByte(data)	LINFLEX_0.BDRL.R=data
#define UART_TxComplete()	(LINFLEX_0.UARTSR.R&0x0002)
#define UART_ClearDTF()		LINFLEX_0.UARTSR.R=0x0002
#define UART_RxReady()		(LINFLEX_0.UARTSR.R&0x0004)
#define UART_ClearDRF()		LINFLEX_0.UARTSR.R=0x0004
#define UART_ReadByte()		LINFLEX_0.BDRM.B.DATA4

#define Tx_Byte(data)	UART_SendByte(data);\
								while(!UART_TxComplete());\
								UART_ClearDTF();\
								UART_ClearDRF()
#define KLINE_RxReady()			UART_RxReady()
//#define KLINE_ReadData()		UART_ReadByte()

// Wait for the transmission to complete
//clear the trasmission completed flag
#define	PrepareTx()				while (LINFLEX_0.UARTSR.B.DTF == 0);\
														LINFLEX_0.UARTSR.B.DTF = 1)
#define FSYS_08_MHz  1
#define FSYS_12_MHz  2
#define FSYS_16_MHz  3
#define FSYS_32_MHz  4
#define FSYS_40_MHz  5
#define FSYS_48_MHz  6
#define FSYS_64_MHz  7
#define FSYS_80_MHz	 8
#define FSYS_120_MHz 9

// System frequency in MHz
#define FSYS FSYS_48_MHz


#define UART_9600_BAUDRATE   1
#define UART_19200_BAUDRATE  2
#define UART_38400_BAUDRATE  3
#define UART_115200_BAUDRATE 4

// Uart baudrate
#define UART_BAUDRATE  UART_115200_BAUDRATE

#define PAD_LIN0_TXD 18
#define PAD_LIN0_RXD 19

extern void LinFlex_0_RX_ISR( void );
#if 0
void LinFlex_0_TX_ISR( void );
char sci_a_getchar (void);
void sci_a_putchar (char ch);
#endif
void LinFlex_0_Init( void );
#endif