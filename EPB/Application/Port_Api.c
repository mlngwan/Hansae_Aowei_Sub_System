#include "Typedefs.h"
#include "Port_Api.h"
#include "Adc_Api.h"
#include "Main_Api.h"
#include "Spi_L9369Data_Types.h"
#include "Spi_TC277_Api.h"

Port_t g_PORT;
extern ADC_t g_ADC;
extern L9369_RegWrite_t g_L9369_RegWrite;
extern TC277_t g_TC277;
SwitchStatus_t g_SwitchStatus;

/*===========================================================================
 * 반환 타입 정의
 * FS가 결정한 결과를 ASW가 사용하기 위한 반환 타입 정의
 *===========================================================================*/

/* 시스템 상태 */
typedef enum {
	FS_SYS_ON = 0,
	FS_SYS_DOWN = 1,
	FS_SYS_OFF = 2
} Fs_SystemState_t;

/*===========================================================================
 * MCAL Layer
 * - 레지스터 직접 접근하는 경우
 * - PORT/PCR 레지스터 설정 및 GPIO Read/Write 담당
 *===========================================================================*/

/* 모든 PORT 핀의 입출력 모드 및 기능 설정 (PCR 레지스터 직접 접근) */
void Mcal_Port_ConfigurePads(void)
{
	PCR_SUB_UBB_EN = PORT_PINCFG_OUTPUT_MODE;
	PCR_SUB_UBVR_EN = PORT_PINCFG_OUTPUT_MODE;
	PCR_SUB_L9369_CTRL = PORT_PINCFG_OUTPUT_MODE;
	PCR_CAN_SBC_INT = PORT_PINCFG_INPUT_MODE;		
	PCR_CAN_EEPROM_MISO = PORT_PINCFG_INPUT_MODE | PORT_PINCFG_WEAK_PULL_UP;
	PCR_CAN_EEPROM_MOSI = PORT_PINCFG_OUTPUT_MODE;	
	PCR_CAN_EEPROM_SCLK = PORT_PINCFG_OUTPUT_MODE;	
	PCR_SUB_EEPROM_CS = PORT_PINCFG_OUTPUT_MODE;	
	PCR_SUB_IGN_MON = PORT_PINCFG_INPUT_MODE;	
	PCR_SPC560_SCLK = PORT_PINCFG_INPUT_MODE|PORT_PINCFG_ALTERNATE_MODE1;		
	PCR_SPC560_CS = PORT_PINCFG_INPUT_MODE | PORT_PINCFG_WEAK_PULL_UP|PORT_PINCFG_ALTERNATE_MODE1;		
	PCR_SPC560_MOSI = PORT_PINCFG_INPUT_MODE;		
	PCR_SPC560_MISO = PORT_PINCFG_OUTPUT_MODE|PORT_PINCFG_ALTERNATE_MODE1;		
	PCR_SUB_L9369_SYNC5 = PORT_PINCFG_OUTPUT_MODE;	
	PCR_SUB_L9369_SYNC1 = PORT_PINCFG_OUTPUT_MODE;				
	PCR_SUB_L9369_MISO = PORT_PINCFG_INPUT_MODE | PORT_PINCFG_WEAK_PULL_UP; 	
	PCR_SUB_L9369_MOSI = PORT_PINCFG_OUTPUT_MODE;	
	PCR_SUB_L9369_SCLK = PORT_PINCFG_OUTPUT_MODE;	
	PCR_SUB_L9369_CS = PORT_PINCFG_OUTPUT_MODE; 
	PCR_SUB_UBVR_MON = PORT_PINCFG_INPUT_MODE;				
	PCR_SUB_UBB_MON = PORT_PINCFG_INPUT_MODE;			
	PCR_SUB_L9369_RST = PORT_PINCFG_OUTPUT_MODE;
	PCR_SUB_L9369_WAU = PORT_PINCFG_OUTPUT_MODE;		
	PCR_SUB_DEBUG = PORT_PINCFG_OUTPUT_MODE;			
	PCR_CAN_SBC_CS = PORT_PINCFG_OUTPUT_MODE;			
	PCR_SUB_EPB_SBC_FAULT = PORT_PINCFG_INPUT_MODE; 

	PCR_SUB_IGN_MON = PORT_PINCFG_ANALOGON;
	PCR_SUB_UBVR_MON = PORT_PINCFG_ANALOGON;
	PCR_SUB_UBB_MON = PORT_PINCFG_ANALOGON;
}

/* 모든 PORT 핀의 초기 출력 상태 설정 (HIGH/LOW) */
void Mcal_Port_SetInitialStates(void)
{
	/*L9369 Configuration*/
	PORT_SUB_L9369_CS = TRUE;//high to low
	PORT_SUB_L9369_MOSI = FALSE;
	PORT_SUB_L9369_SCLK = FALSE;
	PORT_SUB_L9369_WAU = FALSE;
	
	/*CAN_EEPROM SPI Configuration*/
	PORT_SUB_EEPROM_CS = FALSE; // low to high
	PORT_CAN_SBC_CS = TRUE; // high to low
	PORT_CAN_EEPROM_MOSI = FALSE;
	PORT_CAN_EEPROM_SCLK = FALSE;
	PORT_SUB_DEBUG = FALSE; //Develop mode
	
	/*Motor Driver Current AD Sync*/
	PORT_SUB_L9369_SYNC1 = FALSE;
	PORT_SUB_L9369_SYNC5 = FALSE;
	
	/*TC277 SPI Configuration*/
	PORT_SPC560_MISO = FALSE;
	PORT_SUB_L9369_RST = FALSE;
	PORT_SUB_UBVR_EN = TRUE;
	PORT_SUB_UBB_EN = TRUE;
}

/* SBC Fault 핀 상태 읽기 (레지스터 직접 접근) */
uint8 Mcal_Port_ReadPin_SbcFault(void)
{
	return PORT_SUB_EPB_SBC_FAULT;
}

/* L9369 모터 드라이버 제어 핀 쓰기 (레지스터 직접 접근) */
void Mcal_Port_WritePin_L9369Ctrl(uint8 state)
{
	PORT_SUB_L9369_CTRL = state;
}

/*===========================================================================
 * BSW Layer (하드웨어 서비스)
 * - MCAL을 받아와 Get/Set 제공
 * - Physical -> Logical 변환
 *===========================================================================*/

/* IGN 전압을 논리 상태(ON/OFF)로 변환 (Physical → Logical) */
uint8 Bsw_Port_GetIgnitionState(void)
{
	/* IGN signal interpretation from ADC physical value */
	if(g_ADC.Physical.Power.IGN > IGN_ON) {
		return TRUE;  /* IGN ON */
	} else {
		return FALSE; /* IGN OFF */
	}
}

/* SBC Fault 핀 상태 읽기 서비스 (MCAL 래핑) */
uint8 Bsw_Port_GetSbcFaultState(void)
{
	/* Read SBC fault pin state */
	return Mcal_Port_ReadPin_SbcFault();
}

/* L9369 모터 드라이버 Enable/Disable 제어 서비스 (MCAL 래핑) */
void Bsw_Port_SetMotorDriverControl(uint8 enable)
{
	/* Control L9369 motor driver enable pin */
	Mcal_Port_WritePin_L9369Ctrl(enable);
}

/* 초기화 */
void Bsw_Port_initialize(void)
{
	Mcal_Port_ConfigurePads();
	Mcal_Port_SetInitialStates();
}
/*===========================================================================
 * FS Layer (필터를 통해 결정 후 반환)
 * - 필터를 거쳐 State 및 Command 결정하고 ASW에서 사용 가능하도록 반환
 * - 실행하지 않음
 *===========================================================================*/

 /*포트 초기화 - 단순 Layer 구분용*/
 void Fs_Port_Initialize(void)
{
	Bsw_Port_initialize();
}

/* IGN 상태로부터 시스템 상태(ON/DOWN/OFF) 결정 */
Fs_SystemState_t Fs_Port_DetermineSystemStateFromIgn(void)
{
	Fs_SystemState_t determineState;
	uint8 ignLogical = Bsw_Port_GetIgnitionState();
	
	if(ignLogical) {
		/* IGN ON 의 경우 */
		determineState = FS_SYS_ON;
		g_PORT.IgnOffCount = 0;  /* FS policy state */
	} 
	else {
		/* IGN OFF 의 경우 */
		determineState = FS_SYS_DOWN;
		
		/* 1초 이상 OFF 시*/
		if(++g_PORT.IgnOffCount > IGN_OFF_TIME) {
			g_PORT.IgnOffCount = IGN_OFF_TIME;
			determineState = FS_SYS_OFF;
		}
	}
	
	return determineState;
}

/* TC277 통신 상태 확인 및 조건부 L9369 비활성화 */
uint8 Fs_Port_NeedDisableMotorCtrl(void)
{
	/* 기존 코드 : fMainfault==FALSE일 때만 CTRL=FALSE 수행, TRUE일 때는 기존 상태 유지 */
	if(g_TC277.fMainfault == FALSE) {  /* TC277와 통신 X */
		return TRUE;  /* 비활성화 타입 반환 */
	} else {
		return FALSE; /* 기존 상태 유지 타입 반환 */
	}
}

/* SBC Fault 논리 상태 산출 (BSW 서비스 호출) */
uint8 Fs_Port_DetermineSbcFaultLogical(void)
{
	return Bsw_Port_GetSbcFaultState();
}

/*===========================================================================
 * ASW Layer (실행)
 * - FS가 결정한 State와 Command를 받아서 실제로 실행
 *===========================================================================*/
/* PORT 초기화 */
void Asw_PortInitialize(void)
{
	Fs_Port_Initialize();
}

/* FS가 결정한 시스템 상태를 실행 (SetSystemOn/Down/Off 호출) */
void Asw_System_ApplySystemState(Fs_SystemState_t state)
{
	switch(state) {
		case FS_SYS_ON:
			SetSystemOn();
			break;
		case FS_SYS_DOWN:
			SetSystemDown();
			break;
		case FS_SYS_OFF:
			SetSystemOff();
			break;
		default:
			/* Unknown state - no action */
			break;
	}
}

/* FS가 결정한 조건부 모터 비활성화 실행 */
void Asw_Port_ApplyMotorCtrlDisableIfNeeded(uint8 needDisable)
{
	if(needDisable) {
		/* 레거시: fMainfault==FALSE일 때만 PORT_SUB_L9369_CTRL = FALSE 수행 */
		Bsw_Port_SetMotorDriverControl(FALSE);
	}
	/* needDisable==FALSE일 때는 아무것도 하지 않음 (레거시 동작 유지) */
}

/* FS가 산출한 SBC Fault 상태를 전역 변수에 저장 */
void Asw_Port_ApplySbcFaultStatus(uint8 faultLogical)
{
	g_PORT.SUB_EPB_SBC_FAULT = faultLogical;
}

void Asw_Check_Port_Status(void)
{
	/* 1. IGN 상태 확인 → 시스템 상태 적용 */
	Fs_SystemState_t sysState = Fs_Port_DetermineSystemStateFromIgn();
	Asw_System_ApplySystemState(sysState);
	
	/* 2. TC277 통신 상태 확인 → 조건에 따라 L9369_CTRL 비활성화 */
	uint8 needDisable = Fs_Port_NeedDisableMotorCtrl();
	Asw_Port_ApplyMotorCtrlDisableIfNeeded(needDisable);
	
	/* 3. SBC Fault 상태 읽기 → 전역 변수 반영 */
	Asw_Port_ApplySbcFaultStatus(Fs_Port_DetermineSbcFaultLogical());
}

/*===========================================================================
 * ASW Layer - EPB 도메인 로직
 * - 스위치 입력을 상태로 설정하고 Apply와 Release를 판단
 *===========================================================================*/

/* EPB 스위치 상태 설정 및 Apply/Release 판단 (RAM 비트필드 조작) */
void SetEpbSwitch(uint8 index, uint8 bOnOff)
{
	SwitchStatus_t *pStatus = (SwitchStatus_t*)&g_SwitchStatus;
	uint8 epbStatus = (uint8)SWITCH_EPB_STATUS(pStatus);

	switch (index) {
		case EPB_SW1:
			SWITCH_EPB_SW1(pStatus) = bOnOff;
			epbStatus = (uint8)(epbStatus & 0xFEU);
			epbStatus = (uint8)(epbStatus | (bOnOff << 0U));
			break;
		case EPB_SW2:
			SWITCH_EPB_SW2(pStatus) = bOnOff;
			epbStatus = (uint8)(epbStatus & 0xFDU);
			epbStatus = (uint8)(epbStatus | (bOnOff << 1U));
			break;
		case EPB_SW3:
			SWITCH_EPB_SW3(pStatus) = bOnOff;
			epbStatus = (uint8)(epbStatus & 0xFBU);
			epbStatus = (uint8)(epbStatus | (bOnOff << 2U));
			break;
		case EPB_SW4:
			SWITCH_EPB_SW4(pStatus) = bOnOff;
			epbStatus = (uint8)(epbStatus & 0xF7U);
			epbStatus = (uint8)(epbStatus | (bOnOff << 3U));
			break;
		case EPB_SW5:
			SWITCH_EPB_SW5(pStatus) = bOnOff;
			epbStatus = (uint8)(epbStatus & 0xEFU);
			epbStatus = (uint8)(epbStatus | (bOnOff << 4U));
			break;
		case EPB_SW7:
			SWITCH_EPB_SW7(pStatus) = bOnOff;
			epbStatus = (uint8)(epbStatus & 0xBFU);
			epbStatus = (uint8)(epbStatus | (bOnOff << 6U));
			break;
		default:
			break;
	}

	SWITCH_EPB_STATUS(pStatus) = (uint8)epbStatus;
	
	if (epbStatus == EPB_SW_NEUTRAL) {
		SWITCH_EPB_APPLY(pStatus) = FALSE;
		SWITCH_EPB_RELEASE(pStatus) = FALSE;
	}
	else {
		if (epbStatus == EPB_SW_APPLY) {
			SWITCH_EPB_APPLY(pStatus) = TRUE;
		}
		else {
			SWITCH_EPB_APPLY(pStatus) = FALSE;
		}
		if (epbStatus == EPB_SW_RELEASE) {
			SWITCH_EPB_RELEASE(pStatus) = TRUE;
		}
		else {
			SWITCH_EPB_RELEASE(pStatus) = FALSE;
		}
	}
}

/* EPB 스위치 상태 읽기 (RAM 비트필드 조회) */
uint8 GetEpbSwitch(uint8 index)
{
	SwitchStatus_t *pStatus = (SwitchStatus_t*)&g_SwitchStatus;
	uint8 bOnOff = FALSE;

	switch (index) {
		case EPB_SW1:
			bOnOff = (uint8)SWITCH_EPB_SW1(pStatus);
			break;
		case EPB_SW2:
			bOnOff = (uint8)SWITCH_EPB_SW2(pStatus);
			break;
		case EPB_SW3:
			bOnOff = (uint8)SWITCH_EPB_SW3(pStatus);
			break;
		case EPB_SW4:
			bOnOff = (uint8)SWITCH_EPB_SW4(pStatus);
			break;
		case EPB_SW5:
			bOnOff = (uint8)SWITCH_EPB_SW5(pStatus);
			break;
		case EPB_SW7:
			bOnOff = (uint8)SWITCH_EPB_SW7(pStatus);
			break;
		default:
			break;
	}

	return bOnOff;
}

/*===========================================================================
 * Test용 기존 코드
 *===========================================================================*/

/* Pad 설정 래퍼 함수 */
void SetPadConfiguration(void)
{
	Mcal_Port_ConfigurePads();
}

/* Port 초기화 래퍼 함수 */
void SetPortInitialize(void)
{
	Mcal_Port_SetInitialStates();
}

void PORTInitialize(void)
{
	Asw_PortInitialize();
}

/* PORT 상태 모니터링 및 제어 */
void CheckPortStatus(void)
{
	Asw_Check_Port_Status();
}


