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
��������������������������������������������������������������������������
��Header		��Task ID	��Data Length		��Data						��Checksum	��
��(1 Byte)	��(1 Byte)   ��(1 Byte)			��(8 Byte Max)		��(1 Byte)		��
��������������������������������������������������������������������������
- Response
������������������������������������
��Header		��Task ID	��Rx_Result	��
��(1 Byte)	��(1 Byte)   ��(1 Byte)	��
������������������������������������

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

/*===========================================================================
 * MCAL Layer
 *===========================================================================*/
 /*===========================================================================
 * BSW Layer
 *===========================================================================*/
/*===========================================================================
 * FS Layer
 *===========================================================================*/
// RxResult 각 case 별로 response code 반환
uint8_t Fs_Interface_Make_Response(uint8_t RxResult)
{
    uint8_t response_code;
    uint8_t temp = RxResult & 0x0F;

    switch(temp)
    {
        case ReceiveOK:
            response_code = TxReceiveOK;
            break;
        case ChecksumErr:
            response_code = TxChecksumErr;
            break;
        case DataRequest:
            response_code = TxDataRequest;
            break;
        case RepeatRequest:
            response_code = TxRepeatRequest;
            break;
        default:
            break;
    }
    
    return response_code;
}

// RxTx == Tx 인 경우 판단
uint8_t Fs_Interface_Should_Send_Response(uint8_t RxTx)
{
    return (RxTx == Tx) ? 1 : 0;
}

/*===========================================================================
 * ASW Layer
 *===========================================================================*/
// FS 판단 통과 시 수행할 Task
void Asw_Interface_Send_Response(uint8_t response_code)
{
    Tx_Byte(TxResponse);
    Tx_Byte(response_code);
}

// FS 판단부터 Task 수행까지의 전체 흐름
void Asw_Interface_All_Process(uint8_t RxTx, uint8_t RxResult)
{
    uint8_t response_code;
    uint8_t should_send;

    response_code = Fs_Interface_Make_Response(RxResult);
    should_send = Fs_Interface_Should_Send_Response(RxTx);
    
    if(should_send)
    {
        Asw_Interface_Send_Response(response_code);
    }
}
 /*===========================================================================
 * Test용 기존 코드
 *===========================================================================*/
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
/*===========================================================================
 * 레거시 호환성 (기존 API 유지)
 *===========================================================================*/

/********************************************************************************
* Function Name	: ResponseAnalysis
* Description		: Response Result analysis (Legacy Wrapper)
* input			: uint8_t RxTx for transmit or receive data, uint8_t RxResult for result of transmit or receive
* return			: None
********************************************************************************/
void ResponseAnalysis(uint8_t RxTx, uint8_t RxResult)
{
    /* 새 레이어 함수 호출로 변경 */
    Asw_Interface_All_Process(RxTx, RxResult);
}