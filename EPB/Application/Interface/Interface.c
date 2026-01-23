/************************ (C) COPYRIGHT 2007  **********************************
* File Name					: Interface.c
* Author						: Gabriel Kang
* Date First Issued		: 2016-12-06 
* Description				: UART Interface Packet Processing 
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
/* Includes ------------------------------------------------------------------*/
#include "Can_Api.h"
#include "Interface.h"
//#include "J1850CRC.h"
#include "linflex_uart.h"
#include "Common.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define	TxHeader				0x55
#define	TxResponse			0xAA

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern Data_Packet InterfaceData[10],FailSafeData[20],DTC_Data[20];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/********************************************************************************
* Function Name	: TxPacketTransmit
* Description		: Tx pecket Transmit
* input					: None
* return				: None
********************************************************************************/
#if 0
static uint8_t	CRC,TramsBuf[4];
void TxPacketTransmit(Data_Packet *TxBuf,TaskID TxID,uint8_t Length,uint16_t TxData)
{
	uint8_t i,temp;
	
	if(!Bit_RxCompletion)
	{
		if(!Bit_Transmit)
		{
			InterfaceTimeout = 0;
			Bit_Transmit = 1;
			if(Length < 1)
			{
				TxBuf[TxID].ID = TxID;
				TxBuf[TxID].Length = Length;
				TxBuf[TxID].Data = TxData;

				TramsBuf[0] = TxID;
				TramsBuf[1] = Length;
				TramsBuf[2] = (uint8_t)(TxData >> 8);
				TramsBuf[3] = (uint8_t)TxData;
//				CRC = j1850_crc((uint8_t *)TramsBuf,4);
			
				Tx_Byte(TxHeader);
				Tx_Byte(TxBuf[TxID].ID);
				Tx_Byte(TxBuf[TxID].Length);
				temp = (uint8_t)(TxBuf[TxID].Data >> 8);
				Tx_Byte(temp);
				temp = (uint8_t)TxBuf[TxID].Data;
				Tx_Byte(temp);
				Tx_Byte(CRC);
			}
			else
			{
				TxBuf[Length].ID = TxID;
				TxBuf[Length].Length = Length;
				TxBuf[Length].Data = TxData;
				TramsBuf[0] = TxID;
				TramsBuf[1] = Length;
				TramsBuf[2] = (uint8_t)(TxData >> 8);
				TramsBuf[3] = (uint8_t)TxData;
//				CRC = j1850_crc((uint8_t *)TramsBuf,4);
			
				Tx_Byte(TxHeader);
				Tx_Byte(TxBuf[Length].ID);
				Tx_Byte(TxBuf[Length].Length);
				temp = (uint8_t)(TxBuf[Length].Data >> 8);
				Tx_Byte(temp);
				temp = (uint8_t)TxBuf[Length].Data;
				Tx_Byte(temp);
				Tx_Byte(CRC);
			}
		}
	}
}
#endif
/********************************************************************************
* Function Name	: PecketAnalysis
* Description		: Rx pecket analysis
* input					: None
* return				: None
********************************************************************************/
uint8_t Cnt_RxBuf,Cal_CRC;
#if 0
void RxPecketAnalysis(void)
{
	if(Bit_RxCompletion)
	{
		Bit_RxCompletion = 0;
		if(Cnt_RxBuf == 9)
		{
//			Cal_CRC = j1850_crc(&RxBuffer[9].Data[2],5);
			if(RxBuffer[9].Data[7] == Cal_CRC)
			{
				fSubCRC_OK = 1;
				InterfaceData[ID_Version].Data[6] = RxBuffer[ID_Version].Data[6];
				InterfaceData[ID_Version].Data[5] = RxBuffer[ID_Version].Data[5];
				InterfaceData[ID_Version].Data[4] = RxBuffer[ID_Version].Data[4];
				InterfaceData[ID_Version].Data[3] = RxBuffer[ID_Version].Data[3];
				InterfaceData[ID_Version].Data[2] = RxBuffer[ID_Version].Data[2];
				InterfaceData[ID_Version].Data[1] = RxBuffer[ID_Version].Data[1];
				InterfaceData[ID_Version].Data[0] = RxBuffer[ID_Version].Data[0];

				Rx_SubSwVerion[0] = InterfaceData[ID_Version].Data[2];
				Rx_SubSwVerion[1] = InterfaceData[ID_Version].Data[3];
				Rx_SubSwVerion[2] = InterfaceData[ID_Version].Data[4];
				Rx_SubSwVerion[3] = InterfaceData[ID_Version].Data[5];
				Rx_SubSwVerion[4] = InterfaceData[ID_Version].Data[6];
			}
		}
		else
		{
//			Cal_CRC = j1850_crc(&RxBuffer[Cnt_RxBuf].Data[1],3);
			if(RxBuffer[Cnt_RxBuf].Data[4] == Cal_CRC)
			{
				fSubCRC_OK = 1;
				InterfaceData[Cnt_RxBuf].Data[3] = RxBuffer[Cnt_RxBuf].Data[3];
				InterfaceData[Cnt_RxBuf].Data[2] = RxBuffer[Cnt_RxBuf].Data[2];
				InterfaceData[Cnt_RxBuf].Data[1] = RxBuffer[Cnt_RxBuf].Data[1];
				InterfaceData[Cnt_RxBuf].Data[0] = RxBuffer[Cnt_RxBuf].Data[0];
			}
		}

		if(++Cnt_RxBuf > 9)
		{
			Cnt_RxBuf = 0;
		}
	}
}
#endif
/********************************************************************************
* Function Name	: ResponseAnalysis
* Description		: Response Result analysis
* input			: uint8_t RxTx for transmit or receive data, uint8_t RxResult for result of transmit or receive
* return			: None
********************************************************************************/
void ResponseAnalysis(uint8_t RxTx, uint8_t RxResult)
{
	uint8_t Temp;

	Temp = RxResult & 0x0F;

	if(RxTx == Tx)
	{
		Tx_Byte(TxResponse);
//		Tx_Byte(RxBuffer[0]);
	}
	
	switch(Temp)
	{
		case ReceiveOK:
			if(RxTx == Tx)
			{
				Tx_Byte(TxReceiveOK);
			}
			else
			{
			}
		break;
		case ChecksumErr:
			if(RxTx == Tx)
			{
				Tx_Byte(TxChecksumErr);
			}
			else
			{
			}
		break;
		case DataRequest:
			if(RxTx == Tx)
			{
				Tx_Byte(TxDataRequest);
			}
			else
			{
			}
		break;
		case RepeatRequest:
			if(RxTx == Tx)
			{
				Tx_Byte(TxRepeatRequest);
			}
			else
			{
			}
		break;
		default:
		break;
	}
}

