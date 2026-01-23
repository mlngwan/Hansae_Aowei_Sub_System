#include "Spi_TC277_Api.h"
#include "Typedefs.h"
#include "Spi_L9369Data_Types.h"
#include "Main_Api.h"

TC277_t g_TC277;
TC277_MOSI g_TC277_MOSI;// TC277 Master ,  SPC560 Slave
TC277_MISO g_TC277_MISO;// TC277 Master ,  SPC560 Slave

extern Main_t g_MAIN;
uint16_t vrx_check = 0, vprx_check = 0;
uint16_t vrx_error_cnt = 0;


uint16_t TC277_RegStatus(uint16_t data)
{
	g_TC277_MOSI.data = data;
	if(g_TC277_MOSI.field.r_w==0)//read
	{
		if(g_TC277_MOSI.field.addr==0) //
		{
			g_TC277_MOSI.field.data = (g_ADC.Physical.Power.UBB*0.01 & 0x3FF);
		}
		else if 
		(g_TC277_MOSI.field.addr==1) //
		{
			g_TC277_MOSI.field.data = (g_ADC.Physical.Power.UBVR*0.01 & 0x3FF);
		}
		else if 
		(g_TC277_MOSI.field.addr==2) //
		{
			g_TC277_MOSI.field.data = (g_ADC.Physical.Power.IGN*0.01 & 0x3FF);
		}
		else if 
		(g_TC277_MOSI.field.addr==3) //
		{
			g_TC277_MOSI.field.data = 341;//0b01 0101 0101
		}
		
	}
}

uint16_t TC277_Response(void) 
{
	g_TC277.received_data = 0x0U;

	vprx_check = vrx_check; //past value update
    // 수신이 가능한지 확인
    if (DSPI_1.SR.B.RFDF == 1) { 
        g_TC277.received_data = DSPI_1.POPR.R; // 수신된 데이터 읽기
		TC277_RegStatus(g_TC277.received_data);
        vrx_check = DSPI_1.SR.B.POPNXTPTR;
		
	    // 송신이 가능한지 확인
	    if (DSPI_1.SR.B.TFFF == 1) { 
	        DSPI_1.PUSHR.B.TXDATA = (uint16_t)g_TC277_MOSI.data16;  // 송신할 데이터 설정
	    }
    }
#if EPB_DEBUG_ON
	if(g_MAIN.Debug == 0x00U)
#else
	if(vprx_check == vrx_check)
#endif
	{
		if(++g_TC277.MissingCnt > 150)
		{
			if(g_MAIN.SystemStatus == 0x01U)
			{
				g_TC277.MissingCnt = 150;
				g_TC277.fMainfault = TRUE;
			}
		}
	}
	else
	{
		g_TC277.MissingCnt = 0;
		g_TC277.fMainfault = FALSE; 
	}
	
	return (uint16_t)g_TC277.received_data;
}
/* 20ms period */
void CheckTC277Status(void)
{
	TC277_Response();
	GetFailSafeStatus();
}

