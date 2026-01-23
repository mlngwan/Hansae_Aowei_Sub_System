/************************ (C) COPYRIGHT 2007  **********************************
* File Name					: Interface.h
* Author						: Gabriel Kang
* Date First Issued		: 2016-12-06 
* Description				: UART Interface Packet Processing Header file
********************************************************************************
* History:
* 2016-12-06  v0.01

Data Packet
- Sender
式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
弛Header		弛Task ID	弛Data Length		弛Data						弛Checksum	弛
弛(1 Byte)	弛(1 Byte)   弛(1 Byte)			弛(8 Byte Max)		弛(1 Byte)		弛
式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
- Response
式式式式式式式式式式式式式式式式式式
弛Header		弛Task ID	弛Rx_Result	弛
弛(1 Byte)	弛(1 Byte)   弛(1 Byte)	弛
式式式式式式式式式式式式式式式式式式

********************************************************************************
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Interface_H
#define __Interface_H
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	ID_Bat = 0,
	ID_Temp,
	ID_Switch2,
	ID_Switch3,
	ID_Switch7,
	ID_Current,
	ID_Force,
	ID_Ripple,
	ID_Hall,
	ID_Version
}TaskID;

/* Private define ------------------------------------------------------------*/
#define	Tx							1
#define	Rx							0

#define	ReceiveOK				0x00
#define	ChecksumErr			0x01
#define	DataRequest			0x02
#define	RepeatRequest		0x03

#define	TxReceiveOK				0xE0
#define	TxChecksumErr			0xE1
#define	TxDataRequest			0xE2
#define	TxRepeatRequest		0xE3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void RxPecketAnalysis(void);
void ResponseAnalysis(uint8_t RxTx, uint8_t RxResult);

#endif	/* __Interface_H */
