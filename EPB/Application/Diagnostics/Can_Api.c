#include "Adc_Api.h"
#include "Can_Api.h"
#include "ElectronicParkBrake.h"
#include "Spi_ExtEEPROM_Api.h"
#include "Dspi_Api.h"
#include "board.h"
#include "EB_Prj.h"
#include "Spi_L9369Data_Types.h"

Can_t g_CAN;
Uds_t g_UDS;
extern ADC_t g_ADC;
Board_t g_BOARD;
SaveDtcType	stSaveDTC;
Status_of_CAN_DTC DTC_CAN_Status[DTC_CAN_NUM];
Status_of_DTC DTC_Status[DEM_DTC_NUMBER];
static Data_Packet RxBuffer[10];
extern ExtY_ElectronicParkBrake_T ElectronicParkBrake_Y;
extern tDtcLocalStatus m_astDtcLocalStatus[DEM_DTC_NUMBER];
extern ubyte pass_stack[5][14];
extern DiagFlagBus DiagInput;
extern SSMInputBus SSMInput;
extern L9369_Status_t g_L9369_Status;
/*===========================================================================
 * MCAL Layer
 *===========================================================================*/
 
/*===========================================================================
 * BSW Layer - Basic Software (Data Access & Communication Services)
 *===========================================================================*/
static void Bsw_Can_Initialize(void)
{
    g_UDS.gnRxFrameFlag = COM_TRUE;
}

/* Rx */
static uint8 Bsw_Can_SetClear_fbRxInd(void)
{
    uint8 rxInd = (uint8)g_CAN.fbRxInd_IEHB;

    if (rxInd != 0U)
    {
        g_CAN.Cnt_Fail_IEHB = 0U;
        g_CAN.fbRxInd_IEHB  = FALSE;
    }
    return rxInd;
}

static uint8 Bsw_Can_Get_Fail_Count(void)
{
    return (uint8)g_CAN.Cnt_Fail_IEHB;
}

/* L9369 */
static uint8 Bsw_L9369_Get_Epb_Status_Left(void)
{
    return g_L9369_Status.EpbStatusLeft;
}

static uint8 Bsw_L9369_Get_Epb_Status_Right(void)
{
    return g_L9369_Status.EpbStatusRight;
}

static uint8 Bsw_L9369_Get_Max_Applied_Left(void)
{
    return g_L9369_Status.fMaxApplied_Left;
}

static uint8 Bsw_L9369_Get_Max_Applied_Right(void)
{
    return g_L9369_Status.fMaxApplied_Right;
}

/* Diag */
static uint8 Bsw_Diag_Get_MotorCur_OvA(void)
{
    return DiagInput.D11_MtrCurOvA;
}

static uint8 Bsw_Diag_Get_MotorCur_UvA(void)
{
    return DiagInput.D11_MtrCurUvA;
}

static uint8 Bsw_Diag_Get_MotorCur_OvB(void)
{
    return DiagInput.D11_MtrCurOvB;
}

static uint8 Bsw_Diag_Ge_MotorCur_UvB(void)
{
    return DiagInput.D11_MtrCurUvB;
}

static uint8 Bsw_Epb_Get_Failure_Status(void)
{
    return ElectronicParkBrake_Y.SSMOutputCAN.EPB_FailureSts;
}

/* ADC */
static uint16 Bsw_Adc_Get_Board_Voltage(void)
{
    return g_ADC.Physical.Power.UBB;
}

static uint16 Bsw_Adc_Get_MotorA_Current(void)
{
    return g_ADC.Physical.Motor.MOTORA_CUR;
}

static uint16 Bsw_Adc_Get_MotorA_Voltage(void)
{
    return g_ADC.Physical.Motor.MOTORA;
}

static uint16 Bsw_Adc_Get_MotorB_Current(void)
{
    return g_ADC.Physical.Motor.MOTORB_CUR;
}

static uint16 Bsw_Adc_Get_MotorB_Voltage(void)
{
    return g_ADC.Physical.Motor.MOTORB;
}

/* ADC */
static uint8 Bsw_Adc_Convert_Voltage_To_U8(uint16 adcValue)
{
    uint32 val = (uint32)adcValue / 100U;  
    if (val > 255U) { val = 255U; }        
    return (uint8)val;
}

static uint16 Bsw_Adc_Convert_Current_To_U16(uint16 adcValue)
{
    uint32 val = (uint32)adcValue / 100U;  
    if (val > 65535U) { val = 65535U; }    
    return (uint16)val;
}

/* CAN 신호 Write */
static void Bsw_Can_Write_MotorA_State(uint8 state)
{
    IL_u8Wr_EPBMotorAstate(state);
}

static void Bsw_Can_Write_MotorA_Indication(uint8 indication)
{
    IL_u8Wr_EPBMotorAindication(indication);
}

static void Bsw_Can_Write_MotorB_State(uint8 state)
{
    IL_u8Wr_EPBMotorBstate(state);
}

static void Bsw_Can_Write_MotorB_Indication(uint8 indication)
{
    IL_u8Wr_EPBMotorBindication(indication);
}

static void Bsw_Can_Write_MotorA_Overcurrent(uint8 status)
{
    IL_BoolWr_EPBMotorAovercurrent(status);
}

static void Bsw_Can_Write_MotorA_Undercurrent(uint8 status)
{
    IL_BoolWr_EPBMotorAundercurrent(status);
}

static void Bsw_Can_Write_MotorB_Overcurrent(uint8 status)
{
    IL_BoolWr_EPBMotorBovercurrent(status);
}

static void Bsw_Can_Write_MotorB_Undercurrent(uint8 status)
{
    IL_BoolWr_EPBMotorBundercurrent(status);
}

static void Bsw_Can_Write_Epb_RxError(uint8 error)
{
    IL_BoolWr_EPBRxError(error);
}

static void Bsw_Can_Write_Board_Voltage(uint8 voltage)
{
    IL_u8Wr_EPBBoardvoltage(voltage);
}

static void Bsw_Can_Write_MotorA_Current(uint16 current)
{
    IL_u16Wr_EPBMotorAcurrent(current);
}

static void Bsw_Can_Write_MotorA_Voltage(uint8 voltage)
{
    IL_u8Wr_EPBMotorAvoltage(voltage);
}

static void Bsw_Can_Write_MotorB_Current(uint16 current)
{
    IL_u16Wr_EPBMotorBcurrent(current);
}

static void Bsw_Can_Write_MotorB_Voltage(uint8 voltage)
{
    IL_u8Wr_EPBMotorBvoltage(voltage);
}

/*===========================================================================
 * FS Layer - Functional Safety (Decision Logic)
 *===========================================================================*/
typedef enum
{
    FS_IEHB_RX_RECEIVED = 0,
    FS_IEHB_RX_NOT_RECEIVED,
    FS_IEHB_RX_TIMEOUT
} Fs_RxStatus_t;

typedef struct {
    uint8 motorAState;
    uint8 motorAInd;
    uint8 motorBState;
    uint8 motorBInd;
    uint8 motorAOverCur;
    uint8 motorAUnderCur;
    uint8 motorBOverCur;
    uint8 motorBUnderCur;
    uint8 RxError;
    uint8 boardVoltage;
    uint16 motorACurrent;
    uint8 motorAVoltage;
    uint16 motorBCurrent;
    uint8 motorBVoltage;
} Fs_CanTxSig_t;

static void Fs_Can_Initialize(void)
{
    Bsw_Can_Initialize();
}

static Fs_RxStatus_t Fs_Can_Rx_Status(void)
{
    if (Bsw_Can_SetClear_fbRxInd() != 0U)
    {
        return FS_IEHB_RX_RECEIVED;
    }

    if (Bsw_Can_Get_Fail_Count() > 9U)
    {
        return FS_IEHB_RX_TIMEOUT;
    }

    return FS_IEHB_RX_NOT_RECEIVED;
}

/* EPB CAN Tx 신호 결정 */
static Fs_CanTxSig_t Fs_Can_Epb_All_Signals(void)
{
    Fs_CanTxSig_t signals;
    
    // Motor Status (L9369)
    signals.motorAState = Bsw_L9369_Get_Epb_Status_Left();
    signals.motorBState = Bsw_L9369_Get_Epb_Status_Right();
    
    // Applied Indication
    signals.motorAInd = (Bsw_L9369_Get_Max_Applied_Left() == 0x01U) ? 0x1U : 0x0U;
    signals.motorBInd = (Bsw_L9369_Get_Max_Applied_Right() == 0x01U) ? 0x1U : 0x0U;
    
    // Diag
    signals.motorAOverCur = Bsw_Diag_Get_MotorCur_OvA();
    signals.motorAUnderCur = Bsw_Diag_Get_MotorCur_UvA();
    signals.motorBOverCur = Bsw_Diag_Get_MotorCur_OvB();
    signals.motorBUnderCur = Bsw_Diag_Get_MotorCur_UvB();
    signals.RxError = Bsw_Epb_Get_Failure_Status();
    
    // ADC
    signals.boardVoltage = Bsw_Adc_Convert_Voltage_To_U8(Bsw_Adc_Get_Board_Voltage());
    signals.motorACurrent = Bsw_Adc_Convert_Current_To_U16(Bsw_Adc_Get_MotorA_Current());
    signals.motorAVoltage = Bsw_Adc_Convert_Voltage_To_U8(Bsw_Adc_Get_MotorA_Voltage());
    signals.motorBCurrent = Bsw_Adc_Convert_Current_To_U16(Bsw_Adc_Get_MotorB_Current());
    signals.motorBVoltage = Bsw_Adc_Convert_Voltage_To_U8(Bsw_Adc_Get_MotorB_Voltage());
    
    return signals;
}

/*===========================================================================
 * BSW Layer - UDS Reset Services
 *===========================================================================*/
static uint8 Bsw_Uds_GetResetBit(void)
{
    return g_UDS.fECU_Reset_Bit;
}

static uint16 Bsw_Uds_GetResetDelayCount(void)
{
    return g_UDS.Cnt_ResetDelay;
}

static void Bsw_Uds_IncrementResetDelayCount(void)
{
    g_UDS.Cnt_ResetDelay++;
}

static void Bsw_Uds_SetResetDelayCount(uint16 count)
{
    g_UDS.Cnt_ResetDelay = count;
}

static uint8 Bsw_Uds_GetBackupStep(void)
{
    return g_UDS.BackupStep;
}

static void Bsw_Uds_SetBackupStep(uint8 step)
{
    g_UDS.BackupStep = step;
}

static uint8 Bsw_Uds_GetBootWriteRequest(void)
{
    return g_UDS.fBootWriteRequest;
}

static uint8 Bsw_Uds_GetEcuReset(void)
{
    return g_UDS.fECU_Reset;
}

static void Bsw_Uds_ClearEcuReset(void)
{
    g_UDS.fECU_Reset = 0;
}

static uint8 Bsw_Uds_GetFingerPrintWrote(void)
{
    return g_UDS.fFingerPrintWrote;
}

static uint8 Bsw_Uds_GetEcuResetErase(void)
{
    return g_UDS.fECU_ResetErase;
}

static void Bsw_Uds_SetEcuResetErase(uint8 value)
{
    g_UDS.fECU_ResetErase = value;
}

static uint16 Bsw_Uds_GetBootWriteAddress(uint8 index)
{
    return g_UDS.BootWriteAdd[index];
}

static uint8 Bsw_Uds_GetSprReq(void)
{
    return g_UDS.SPR_Req;
}

static void Bsw_Uds_SetDiagTxData(uint8 index, uint8 value)
{
    g_UDS.DiagTxData[index] = value;
}

static uint32 Bsw_Eep_ReadX16(uint16 address)
{
    return SPI_EEP_Read_X16(address);
}

static void Bsw_Eep_SendX16(uint16 address, uint16 data, uint8 cmd)
{
    SPI_EEP_Send_X16(address, data, cmd);
}

static void Bsw_Can_SendDiagFrame(void)
{
    IL_u32Wr_Diag_From_EPB1R_Sig(g_UDS.DiagTxData);
    COM_SendFrameDirect(COM_FRAME_IDX_Diag_From_EPB1R);
}

static void Bsw_Sys_DisableInterrupt(void)
{
    APP_InterruptDisable();
}

static void Bsw_Sys_PerformReset(void)
{
    BoardPerformReset(PLATFORMS_SW_RESET);
}

/*===========================================================================
 * FS Layer - UDS Reset Decision Logic
 *===========================================================================*/
typedef enum
{
    FS_RESET_IDLE = 0,
    FS_RESET_BACKUP_STEP0,
    FS_RESET_BACKUP_STEP1, 
    FS_RESET_BACKUP_STEP2,
    FS_RESET_TIMEOUT,
    FS_RESET_EXECUTE,
    FS_RESET_CHECK_ERASE
} Fs_ResetAction_t;

typedef struct
{
    Fs_ResetAction_t action;
    uint16 writeData;
    uint16 address;
    uint8 nextStep;
} Fs_ResetDecision_t;

static Fs_ResetDecision_t Fs_Uds_DetermineResetAction(void)
{
    Fs_ResetDecision_t decision = {FS_RESET_IDLE, 0, 0, 0};
    
    if (Bsw_Uds_GetResetBit())
    {
        uint16 delayCount = Bsw_Uds_GetResetDelayCount();
        
        if (delayCount > 250U) /* 5초 경과 */
        {
            if (!Bsw_Uds_GetFingerPrintWrote())
            {
                decision.action = FS_RESET_EXECUTE;
            }
            else
            {
                decision.action = FS_RESET_TIMEOUT;
            }
        }
        else
        {
            uint8 backupStep = Bsw_Uds_GetBackupStep();
            
            switch (backupStep)
            {
                case 0:
                    if (Bsw_Uds_GetBootWriteRequest())
                    {
                        decision.action = FS_RESET_BACKUP_STEP0;
                        decision.writeData = (uint16)((Bsw_Uds_GetBootWriteAddress(0) << 8) | Bsw_Uds_GetBootWriteAddress(1));
                        decision.address = EEP_BootWriteAdd;
                        decision.nextStep = 1;
                    }
                    else
                    {
                        decision.action = FS_RESET_BACKUP_STEP0;
                        decision.nextStep = 2;
                    }
                    break;
                    
                case 1:
                    decision.action = FS_RESET_BACKUP_STEP1;
                    if (Bsw_Uds_GetBootWriteRequest())
                    {
                        decision.writeData = (uint16)((Bsw_Uds_GetBootWriteAddress(2) << 8) | Bsw_Uds_GetBootWriteAddress(3));
                        decision.address = EEP_BootWriteAdd + 1;
                    }
                    decision.nextStep = 2;
                    break;
                    
                case 2:
                    if (Bsw_Uds_GetEcuReset())
                    {
                        decision.action = FS_RESET_BACKUP_STEP2;
                        decision.writeData = (uint16)(0x5A00U | Bsw_Uds_GetSprReq());
                        decision.address = UDS_ResetECU_CheckAdd;
                        decision.nextStep = 3;
                    }
                    break;
                    
                default:
                    decision.action = FS_RESET_TIMEOUT;
                    break;
            }
        }
    }
    else
    {
        if (!Bsw_Uds_GetEcuResetErase())
        {
            decision.action = FS_RESET_CHECK_ERASE;
        }
        else
        {
            decision.action = FS_RESET_IDLE;
        }
    }
    
    return decision;
}
static void Asw_Can_Initialize(void)
{
    APP_Init();                         
    Fs_Can_Initialize(); 
}

static void Asw_Can_Task(void)
{
	CAN_AllPolling();          // EB 스택
	APP_Manage();              // App_Prg.c
	EB_Manage();               // EB 스택
	ccpDaq(1);                 // CCP
}

static void Asw_Can_Tx_Message(void)
{
	Asw_Can_Task();
	Asw_Can_Tx_100ms();
}

static void Asw_Can_iEHB_Message(void)
{
    Fs_RxStatus_t st = Fs_Can_Rx_Status();

    switch (st)
    {
        case FS_IEHB_RX_RECEIVED:
            //DTC_CAN_Missing(DTC_IEHB, DTC_PASS,DEM_DTC_IDX_DTC_MISSINGMESSAGEVCU);
						//g_CAN.Func_Result = IL_u8Rd_VCUMessageCounter((u8 *)&g_CAN.Rx_U8);
						//SSMInput.SSMInSwitch= (uint8_T)g_CAN.Rx_U8;
            break;

        case FS_IEHB_RX_TIMEOUT:
            //g_CAN.Cnt_Fail_IEHB = 0;
						// //DTC_CAN_Missing(DTC_IEHB, DTC_FAIL, DEM_DTC_IDX_DTC_MISSINGMESSAGEVCU);
            break;

        default:
            break;
    }
}

static void Asw_Can_Tx_100ms(void)
{
    /* FS Layer: 모든 CAN 신호 결정 */
    Fs_CanTxSig_t signals = Fs_Can_Epb_All_Signals();
    
    /* ASW Layer: 결정된 신호들을 CAN으로 송신 */
    Bsw_Can_Write_MotorA_State(signals.motorAState);
    Bsw_Can_Write_MotorA_Indication(signals.motorAInd);
    Bsw_Can_Write_MotorB_State(signals.motorBState);
    Bsw_Can_Write_MotorB_Indication(signals.motorBInd);
    Bsw_Can_Write_MotorA_Overcurrent(signals.motorAOverCur);
    Bsw_Can_Write_MotorA_Undercurrent(signals.motorAUnderCur);
    Bsw_Can_Write_MotorB_Overcurrent(signals.motorBOverCur);
    Bsw_Can_Write_MotorB_Undercurrent(signals.motorBUnderCur);
    Bsw_Can_Write_Epb_RxError(signals.RxError);
    Bsw_Can_Write_Board_Voltage(signals.boardVoltage);
    Bsw_Can_Write_MotorA_Current(signals.motorACurrent);
    Bsw_Can_Write_MotorA_Voltage(signals.motorAVoltage);
    Bsw_Can_Write_MotorB_Current(signals.motorBCurrent);
    Bsw_Can_Write_MotorB_Voltage(signals.motorBVoltage);
}

static void Asw_Uds_EcuResetCheck(void)
{
    /* FS Layer: 리셋 액션 결정 */
    Fs_ResetDecision_t decision = Fs_Uds_DetermineResetAction();
    
    /* ASW Layer: 결정된 액션 실행 */
    switch (decision.action)
    {
        case FS_RESET_IDLE:
            /* 리셋 상태 초기화 */
            Bsw_Uds_SetResetDelayCount(0);
            Bsw_Uds_SetBackupStep(0);
            break;
            
        case FS_RESET_BACKUP_STEP0:
        case FS_RESET_BACKUP_STEP1:
            /* EEPROM 백업 실행 */
            if (decision.writeData != 0)
            {
                Bsw_Eep_SendX16(decision.address, decision.writeData, EWEN_CMD);
            }
            Bsw_Uds_SetBackupStep(decision.nextStep);
            Bsw_Uds_IncrementResetDelayCount();
            break;
            
        case FS_RESET_BACKUP_STEP2:
            /* ECU 리셋 플래그 백업 */
            Bsw_Uds_ClearEcuReset();
            Bsw_Eep_SendX16(decision.address, decision.writeData, EWEN_CMD);
            Bsw_Uds_SetBackupStep(decision.nextStep);
            Bsw_Uds_IncrementResetDelayCount();
            break;
            
        case FS_RESET_TIMEOUT:
            /* 타임아웃 처리 */
            Bsw_Uds_SetResetDelayCount(1000);
            break;
            
        case FS_RESET_EXECUTE:
            /* 실제 리셋 실행 */
            Bsw_Sys_DisableInterrupt();
            Bsw_Sys_PerformReset();
            break;
            
        case FS_RESET_CHECK_ERASE:
            /* 리셋 후 확인 및 응답 */
            Bsw_Uds_SetEcuResetErase(1);
            {
                uint32 readData = Bsw_Eep_ReadX16(UDS_ResetECU_CheckAdd);
                if (readData & 0x5A5AU)
                {
                    /* UDS 응답 메시지 구성 */
                    Bsw_Uds_SetDiagTxData(0, 0x02);
                    Bsw_Uds_SetDiagTxData(1, 0x51);
                    Bsw_Uds_SetDiagTxData(2, 0x01);
                    
                    /* CAN 진단 메시지 송신 */
                    Bsw_Can_SendDiagFrame();
                    
                    /* EEPROM 플래그 클리어 */
                    Bsw_Eep_SendX16(UDS_ResetECU_CheckAdd, 0x00U, EWEN_CMD);
                }
            }
            break;
            
        default:
            /* 기본 카운터 증가 */
            if (Bsw_Uds_GetResetBit())
            {
                Bsw_Uds_IncrementResetDelayCount();
            }
            break;
    }
}

 /*===========================================================================
 * Test용 기존 코드
 *===========================================================================*/
 
void iEHB_Message(void)
{
	Asw_Can_iEHB_Message();
}

void Tx_Can_100ms(void)
{
	//IL_BoolWr_EPBMotorAEcho();
	//IL_BoolWr_EPBMotorBEcho(g_EPB_St.Can_Tx.Can_EchoB);
	Asw_Can_Tx_100ms();
}

/*	20msec	*/
void EcuResetCheck(void)
{
	Asw_Uds_EcuResetCheck();
}

void CanInitialize(void)
{
	Asw_Can_Initialize();
}

void CAN_Task(void)
{
	Asw_Can_Task();
}

void TxCanMessage(void)
{
	Asw_Can_Tx_Message();
}

