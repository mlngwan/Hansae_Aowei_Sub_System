/*
 * File: ParkingBrakeControl.c
 *
 * Code generated for Simulink model 'ElectronicParkBrake'.
 *
 * Model version                  : 1.3601
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Thu Nov 21 18:46:22 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Custom Processor->Custom
 * Code generation objectives:
 *    1. MISRA C:2012 guidelines
 *    2. Execution efficiency
 *    3. Traceability
 *    4. Debugging
 * Validation result: Passed (21), Warning (0), Errors (3)
 */

#include "ParkingBrakeControl.h"

/* Include model header file for global data */
#include "ElectronicParkBrake.h"
#include "ElectronicParkBrake_private.h"

/* Named constants for Chart: '<S2>/Pbc' */
#define ElectronicParkBr_IN_DOWN_ENABLE ((uint8_T)1U)
#define ElectronicParkBra_IN_DOWN_TIMER ((uint8_T)2U)
#define ElectronicParkBrake_IN_UP      ((uint8_T)3U)
#define ElectronicPark_IN_Wait_PBC_Stop ((uint8_T)4U)

/* Named constants for Chart: '<S6>/PbcInWheelDirectionInfo' */
#define ElectronicParkBrake_IN_Backward ((uint8_T)1U)
#define ElectronicParkBrake_IN_Forward ((uint8_T)2U)

/* Forward declaration for local functions */
static void Electronic_EcuPowerLatchRequest(void);
static void enter_internal_EcuPowerLatchReq(void);

/* Function for Chart: '<S2>/Pbc' */
static void Electronic_EcuPowerLatchRequest(void)
{
  int32_T tmp;

  /* During 'EcuPowerLatchRequest': '<S4>:3262' */
  /* 20.01.22 Seran Lee

     1. EcuPowerLatchRequest는 전원 관련 신호를 펌웨어에 전송하여 EPB 전원을 관리하는 state이다.

     2. EcuPowerLatchRequest는 EPB 종료 단계를 3개의 하위 state로 정의하고 조건에 따라 state를 transition한다.

     3. 3개의 하위 state는 'UP', 'Wait_PBC_Stop', 'DOWN' 이다. */
  switch (ElectronicParkBrake_DW.is_EcuPowerLatchRequest) {
   case ElectronicParkBr_IN_DOWN_ENABLE:
    ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = false;

    /* During 'DOWN_ENABLE': '<S4>:5227' */
    /* 20.01.29 Seran Lee

       1. DOWN state는 PbcOutEcuPowerLatchRequest에 'false'를 할당하므로써 EPB가 종료될 준비를 마쳤음을 알린다.

       2. 조건에 따라 'UP' state로 전환된다. */
    if ((((int32_T)ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) ==
         ((int32_T)IgnitionState_IgnitionON)) || (((int32_T)
          ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) == ((int32_T)
          IgnitionState_EngineON))) {
      /* Transition: '<S4>:3267' */
      /* 20.01.29 Seran Lee

         IgnitionState가 'IgnitionON' 또는 'EngineON' 이거나 PbcShutDown 신호가 'false'이면, 'DOWN' state가 'UP' state로 전환된다.
       */
      ElectronicParkBrake_DW.is_EcuPowerLatchRequest = ElectronicParkBrake_IN_UP;

      /* Entry 'UP': '<S4>:3268' */
      ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = true;
    }
    break;

   case ElectronicParkBra_IN_DOWN_TIMER:
    /* During 'DOWN_TIMER': '<S4>:3270' */
    /* 20.01.29 Seran Lee

       1. DOWN state는 PbcOutEcuPowerLatchRequest에 'false'를 할당하므로써 EPB가 종료될 준비를 마쳤음을 알린다.

       2. 조건에 따라 'UP' state로 전환된다. */
    if ((((int32_T)ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) ==
         ((int32_T)IgnitionState_IgnitionON)) || (((int32_T)
          ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) == ((int32_T)
          IgnitionState_EngineON))) {
      /* Transition: '<S4>:5235' */
      ElectronicParkBrake_DW.is_EcuPowerLatchRequest = ElectronicParkBrake_IN_UP;

      /* Entry 'UP': '<S4>:3268' */
      ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = true;
    } else if (((int32_T)ElectronicParkBrake_DW.PbcOutDataLatchdownTimer) == 0)
    {
      /* Transition: '<S4>:5232' */
      ElectronicParkBrake_DW.is_EcuPowerLatchRequest =
        ElectronicParkBr_IN_DOWN_ENABLE;

      /* Entry 'DOWN_ENABLE': '<S4>:5227' */
      ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = false;
    } else if ((((((int32_T)
                   ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequestRight)
                  != ((int32_T)ApplyReleaseRequest_None)) || (((int32_T)
        ElectronicParkBrake_DW.PbcOutMotorCommandRight) != ((int32_T)
        MotorCommand_None))) || ((((int32_T)
        ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateRight) !=
                  ((int32_T)MotorDriverState_Stop)) && (((int32_T)
        ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateRight) !=
                  ((int32_T)MotorDriverState_None)))) || (((((int32_T)
        ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequestLeft) !=
                  ((int32_T)ApplyReleaseRequest_None)) || (((int32_T)
        ElectronicParkBrake_DW.PbcOutMotorCommandLeft) != ((int32_T)
        MotorCommand_None))) || ((((int32_T)
        ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateLeft) !=
                  ((int32_T)MotorDriverState_Stop)) && (((int32_T)
        ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateLeft) !=
                  ((int32_T)MotorDriverState_None))))) {
      /* Transition: '<S4>:5237' */
      ElectronicParkBrake_DW.is_EcuPowerLatchRequest =
        ElectronicPark_IN_Wait_PBC_Stop;

      /* Entry 'Wait_PBC_Stop': '<S4>:3269' */
      ElectronicParkBrake_DW.PbcOutDataLatchdownTimer = 500U;
    } else {
      tmp = ((int32_T)ElectronicParkBrake_DW.PbcOutDataLatchdownTimer) - 1;
      if (tmp < 0) {
        tmp = 0;
      }

      ElectronicParkBrake_DW.PbcOutDataLatchdownTimer = (uint16_T)tmp;
    }
    break;

   case ElectronicParkBrake_IN_UP:
    ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = true;

    /* During 'UP': '<S4>:3268' */
    /* 20.01.29 Seran Lee

       1. UP state는 PbcOutEcuPowerLatchRequest에 'true'를 할당함으로써 EPB 전원이 켜져 있음을 펌웨어에 알린다.

       2. 조건에 따라 Wait_PBC_Stop state로 전환된다. */
    if (((int32_T)ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) ==
        ((int32_T)IgnitionState_IgnitionOFF)) {
      /* Transition: '<S4>:3264' */
      /* 20.01.29 Seran Lee

         IgnitionState가 'IgnitionOFF'이고 SSM으로부터 온 신호 'PbcShutDown'이 'true'이면, 'UP' state가 'Wait_PBC_Stop' state로 전환된다.
       */
      ElectronicParkBrake_DW.is_EcuPowerLatchRequest =
        ElectronicPark_IN_Wait_PBC_Stop;

      /* Entry 'Wait_PBC_Stop': '<S4>:3269' */
      ElectronicParkBrake_DW.PbcOutDataLatchdownTimer = 500U;
    }
    break;

   default:
    /* During 'Wait_PBC_Stop': '<S4>:3269' */
    /* 20.01.29 Seran Lee

       1. Wait_PBC_Stop은 SSM으로 부터 PBC에 EPB 종료 요청이 들어온 이후 종료 조건이 만족할 때까지 대기하는 state이다.

       2. 조건에 따라 'UP' state 또는 'DOWN' state로 전환된다. */
    if ((((int32_T)ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) ==
         ((int32_T)IgnitionState_IgnitionON)) || (((int32_T)
          ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState) == ((int32_T)
          IgnitionState_EngineON))) {
      /* Transition: '<S4>:3265' */
      /* 20.01.29 Seran Lee

         IgnitionState가 'IgnitionON' 또는 'EngineON' 이거나 PbcShutDown 신호가 'false'이면, 'Wait_PBC_Stop' state가 'UP' state로 전환된다.
       */
      ElectronicParkBrake_DW.is_EcuPowerLatchRequest = ElectronicParkBrake_IN_UP;

      /* Entry 'UP': '<S4>:3268' */
      ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = true;
    } else {
      if ((((((int32_T)
              ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequestRight) ==
             ((int32_T)ApplyReleaseRequest_None)) && (((int32_T)
              ElectronicParkBrake_DW.PbcOutMotorCommandRight) == ((int32_T)
              MotorCommand_None))) && ((((int32_T)
              ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateRight) ==
             ((int32_T)MotorDriverState_Stop)) || (((int32_T)
              ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateRight) ==
             ((int32_T)MotorDriverState_None)))) && (((((int32_T)
              ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequestLeft) ==
             ((int32_T)ApplyReleaseRequest_None)) && (((int32_T)
              ElectronicParkBrake_DW.PbcOutMotorCommandLeft) == ((int32_T)
              MotorCommand_None))) && ((((int32_T)
              ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateLeft) ==
             ((int32_T)MotorDriverState_Stop)) || (((int32_T)
              ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateLeft) ==
             ((int32_T)MotorDriverState_None))))) {
        /* Transition: '<S4>:3266' */
        /* 20.01.29 Seran Lee

           ApplyReleaseRequest, MotorCommand가 'None'이고, MotorDriverState가 'Stop' 또는 'None'이면, 'Wait_PBC_Stop' state가 'DOWN' state로 전환된다.
         */
        ElectronicParkBrake_DW.is_EcuPowerLatchRequest =
          ElectronicParkBra_IN_DOWN_TIMER;
      }
    }
    break;
  }
}

/* Function for Chart: '<S2>/Pbc' */
static void enter_internal_EcuPowerLatchReq(void)
{
  /* Entry Internal 'EcuPowerLatchRequest': '<S4>:3262' */
  /* Transition: '<S4>:3263' */
  ElectronicParkBrake_DW.is_EcuPowerLatchRequest = ElectronicParkBrake_IN_UP;

  /* Entry 'UP': '<S4>:3268' */
  ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = true;
}

/* System initialize for atomic system: '<S1>/ParkingBrakeControl' */
void Electr_ParkingBrakeControl_Init(void)
{
  /* SystemInitialize for Chart: '<S2>/Pbc' */
  ElectronicParkBrake_DW.PbcApplyForce = C_Light_Apply_Force;
  ElectronicParkBrake_DW.PbcDataReleaseOC_Counter = 10U;
  ElectronicParkBrake_DW.PbcDataHallSensor_Reset = true;
  ElectronicParkBrake_DW.PbcDataReleaseOC_Counter_Left = 10U;
  ElectronicParkBrake_DW.PbcApplyTimeTempLeft = 100U;
  ElectronicParkBrake_DW.PbcApplyTimeTempRight = 100U;
  ElectronicParkBrake_DW.PbcDataReleaseOC_Counter_Right = 10U;
  ElectronicParkBrake_DW.PbcData_UC_Counter_Left = 10U;
  ElectronicParkBrake_DW.OCMarurationTimeDuration = 5U;
  ElectronicParkBrake_DW.UCMarurationTimeDuration = 20U;
  ElectronicParkBrake_DW.PbcData_OC_Counter_Left = 10U;
  ElectronicParkBrake_DW.PbcDataApplyOC_Counter_Left = 10U;
  ElectronicParkBrake_DW.PbcOutDataLatchdownTimer = 3000U;
  ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest = true;
}

/*
 * Output and update for atomic system: '<S1>/ParkingBrakeControl'
 * Block description for: '<S1>/ParkingBrakeControl'
 *   2020.11.13 Seungbum Han
 *   - SSM으로부터 발생된 Request에 따른 동작 수행
 *   - 동작 중 Sensor 및 Motorstate, EPB State 감시
 */
void ElectronicP_ParkingBrakeControl(void)
{
  real_T rtb_Add2;
  real_T rtb_Add2_b;
  int8_T rtb_PbcInWheelDirectionInfo;
  real_T rtb_Add2_d;
  real_T rtb_Add2_j;
  real_T rtb_Add2_e2;
  real_T rtb_Add2_e;
  real_T u0;
  real_T rtb_Add2_gq;

  /* Gain: '<S7>/CurrentFilterGain' incorporates:
   *  Gain: '<S8>/CurrentFilterGain'
   */
  rtb_Add2_b = (6.2831853071795862 * PbcDataCurrentFilterFrequency) /
    ((6.2831853071795862 * PbcDataCurrentFilterFrequency) + 100.0);

  /* Sum: '<S7>/Add2' incorporates:
   *  DataTypeConversion: '<S7>/MotorCurrent_DataTypeConversion1'
   *  Gain: '<S7>/CurrentFilterGain'
   *  Inport: '<Root>/PbcInDriver'
   *  Sum: '<S7>/Add1'
   *  UnitDelay: '<S7>/PreviousMotorCurrentFiltered'
   */
  rtb_Add2 = (rtb_Add2_b * (((real_T)PbcInDriver.MotorCurrentLeft) -
    ElectronicParkBrake_DW.PreviousMotorCurrentFiltered_DS)) +
    ElectronicParkBrake_DW.PreviousMotorCurrentFiltered_DS;

  /* DataTypeConversion: '<S7>/MotorCurrent_DataTypeConversion2' */
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorCurrentLeft = (real32_T)
    rtb_Add2;

  /* Sum: '<S8>/Add2' incorporates:
   *  DataTypeConversion: '<S8>/MotorCurrent_DataTypeConversion1'
   *  Gain: '<S8>/CurrentFilterGain'
   *  Inport: '<Root>/PbcInDriver'
   *  Sum: '<S8>/Add1'
   *  UnitDelay: '<S8>/PreviousMotorCurrentFiltered'
   */
  rtb_Add2_b = (rtb_Add2_b * (((real_T)PbcInDriver.MotorCurrentRight) -
    ElectronicParkBrake_DW.PreviousMotorCurrentFiltered__c)) +
    ElectronicParkBrake_DW.PreviousMotorCurrentFiltered__c;

  /* DataTypeConversion: '<S8>/MotorCurrent_DataTypeConversion2' */
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorCurrentRight = (real32_T)
    rtb_Add2_b;

  /* BusCreator: '<S5>/Bus Creator' incorporates:
   *  Inport: '<Root>/PbcInDriver'
   */
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorVoltageLeft =
    PbcInDriver.MotorVoltageLeft;
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateLeft =
    PbcInDriver.MotorDriverStateLeft;
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorVoltageRight =
    PbcInDriver.MotorVoltageRight;
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverStateRight =
    PbcInDriver.MotorDriverStateRight;
  ElectronicParkBrake_DW.PbcInDriverFiltered.MotorDriverSupplyVoltage =
    PbcInDriver.MotorDriverSupplyVoltage;

  /* Chart: '<S6>/PbcInWheelDirectionInfo' incorporates:
   *  Inport: '<Root>/PBCInput'
   */
  /* Gateway: ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/PbcInWheelDirectionInfo */
  /* During: ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/PbcInWheelDirectionInfo */
  if (((uint32_T)ElectronicParkBrake_DW.is_active_c13_ElectronicParkBra) == 0U)
  {
    /* Entry: ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/PbcInWheelDirectionInfo */
    ElectronicParkBrake_DW.is_active_c13_ElectronicParkBra = 1U;

    /* Entry Internal: ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/PbcInWheelDirectionInfo */
    /* Transition: '<S14>:11' */
    ElectronicParkBrake_DW.is_c13_ElectronicParkBrake =
      ElectronicParkBrake_IN_Forward;

    /* Entry 'Forward': '<S14>:8' */
    rtb_PbcInWheelDirectionInfo = 1;
  } else if (((uint32_T)ElectronicParkBrake_DW.is_c13_ElectronicParkBrake) ==
             ElectronicParkBrake_IN_Backward) {
    rtb_PbcInWheelDirectionInfo = -1;

    /* During 'Backward': '<S14>:6' */
    if ((((int32_T)PBCInput.PbcInWheelDIrectionInfoRL) != 3) || (((int32_T)
          PBCInput.PbcInWheelDIrectionInfoRR) != 3)) {
      /* Transition: '<S14>:10' */
      ElectronicParkBrake_DW.is_c13_ElectronicParkBrake =
        ElectronicParkBrake_IN_Forward;

      /* Entry 'Forward': '<S14>:8' */
      rtb_PbcInWheelDirectionInfo = 1;
    }
  } else {
    rtb_PbcInWheelDirectionInfo = 1;

    /* During 'Forward': '<S14>:8' */
    if ((((int32_T)PBCInput.PbcInWheelDIrectionInfoRL) == 3) && (((int32_T)
          PBCInput.PbcInWheelDIrectionInfoRR) == 3)) {
      /* Transition: '<S14>:9' */
      ElectronicParkBrake_DW.is_c13_ElectronicParkBrake =
        ElectronicParkBrake_IN_Backward;

      /* Entry 'Backward': '<S14>:6' */
      rtb_PbcInWheelDirectionInfo = -1;
    }
  }

  /* End of Chart: '<S6>/PbcInWheelDirectionInfo' */

  /* Sum: '<S18>/Add2' incorporates:
   *  Constant: '<S15>/LongAcceleration_Offset'
   *  DataTypeConversion: '<S15>/LongAcceleration_DataTypeConversion'
   *  Gain: '<S15>/LongAcceleration_Rescale_Gain'
   *  Gain: '<S18>/LongAccelerationFilterGain'
   *  Inport: '<Root>/PBCInput'
   *  Sum: '<S15>/Add1'
   *  Sum: '<S18>/Add1'
   *  UnitDelay: '<S18>/PreviousLongAccelerationFiltered'
   */
  rtb_Add2_j = (((6.2831853071795862 * PbcDataLongAccelerationFilterFrequency) /
                 ((6.2831853071795862 * PbcDataLongAccelerationFilterFrequency)
                  + 100.0)) * (((0.0001274 * ((real_T)
    PBCInput.PbcInLongAcceleration)) + -4.1745) -
    ElectronicParkBrake_DW.PreviousLongAccelerationFiltere)) +
    ElectronicParkBrake_DW.PreviousLongAccelerationFiltere;

  /* Sum: '<S21>/Add2' incorporates:
   *  DataTypeConversion: '<S16>/Speed_DataTypeConversion'
   *  Gain: '<S16>/Speed_Rescale_Gain'
   *  Gain: '<S21>/VehicleSpeedFilterGain'
   *  Product: '<S16>/Product'
   *  Sum: '<S21>/Add1'
   *  UnitDelay: '<S21>/PreviousVehicleSpeedFiltered'
   */
  rtb_Add2_d = (((6.2831853071795862 * PbcDataVehicleSpeedFilterFrequency) /
                 ((6.2831853071795862 * PbcDataVehicleSpeedFilterFrequency) +
                  100.0)) * ((((real_T)((int32_T)(((int32_T)
    rtb_PbcInWheelDirectionInfo) * ((int32_T)
    ElectronicParkBrake_DW.SSMtoPBC.PbcInVehicleSpeed)))) * 0.01) -
    ElectronicParkBrake_DW.PreviousVehicleSpeedFiltered_DS)) +
    ElectronicParkBrake_DW.PreviousVehicleSpeedFiltered_DS;

  /* Sum: '<S20>/Add2' incorporates:
   *  Gain: '<S19>/Differentiation'
   *  Gain: '<S19>/GravityAccelerationGain'
   *  Gain: '<S19>/UnitGain'
   *  Gain: '<S20>/VehicleAccelerationFilterGain'
   *  Sum: '<S19>/Add1'
   *  Sum: '<S20>/Add1'
   *  UnitDelay: '<S19>/VehicleSpeedFiltered10'
   *  UnitDelay: '<S20>/PreviousVehicleAccelerationFiltered'
   */
  rtb_Add2_e2 = ((((((rtb_Add2_d -
                      ElectronicParkBrake_DW.VehicleSpeedFiltered10_DSTATE) *
                     10.0) * 0.27777777777777779) * 0.1019367991845056) -
                  ElectronicParkBrake_DW.PreviousVehicleAccelerationFilt) *
                 ((6.2831853071795862 *
                   PbcDataVehicleAccelerationFilterFrequency) /
                  ((6.2831853071795862 *
                    PbcDataVehicleAccelerationFilterFrequency) + 100.0))) +
    ElectronicParkBrake_DW.PreviousVehicleAccelerationFilt;

  /* Sum: '<S17>/Add2' incorporates:
   *  Gain: '<S17>/CorrectedLongAccelerationFilterGain'
   *  Sum: '<S12>/Add1'
   *  Sum: '<S17>/Add1'
   *  UnitDelay: '<S17>/PreviousCorrectedLongAccelerationFiltered'
   */
  rtb_Add2_e = (((6.2831853071795862 *
                  PbcDataCorrectedLongAccelerationFilterFrequency) /
                 ((6.2831853071795862 *
                   PbcDataCorrectedLongAccelerationFilterFrequency) + 100.0)) *
                ((rtb_Add2_j - rtb_Add2_e2) -
                 ElectronicParkBrake_DW.PreviousCorrectedLongAccelerati)) +
    ElectronicParkBrake_DW.PreviousCorrectedLongAccelerati;

  /* Chart: '<S2>/Pbc' */
  /* Gateway: ElectronicParkBrake/ParkingBrakeControl/Pbc */
  /* During: ElectronicParkBrake/ParkingBrakeControl/Pbc */
  if (((uint32_T)ElectronicParkBrake_DW.is_active_c3_ElectronicParkBrak) == 0U)
  {
    /* Entry: ElectronicParkBrake/ParkingBrakeControl/Pbc */
    ElectronicParkBrake_DW.is_active_c3_ElectronicParkBrak = 1U;

    /* Entry Internal: ElectronicParkBrake/ParkingBrakeControl/Pbc */
    enter_internal_EcuPowerLatchReq();
  } else {
    Electronic_EcuPowerLatchRequest();
  }

  /* End of Chart: '<S2>/Pbc' */

  /* BusCreator: '<S2>/Bus Creator2' incorporates:
   *  Outport: '<Root>/PbcOutput'
   */
  ElectronicParkBrake_Y.PbcOutput.PbcOutMotorCommandLeft =
    ElectronicParkBrake_DW.PbcOutMotorCommandLeft;
  ElectronicParkBrake_Y.PbcOutput.PbcOutMotorCommandRight =
    ElectronicParkBrake_DW.PbcOutMotorCommandRight;
  ElectronicParkBrake_Y.PbcOutput.PbcOutEcuPowerLatchRequest =
    ElectronicParkBrake_DW.PbcOutEcuPowerLatchRequest;
  ElectronicParkBrake_Y.PbcOutput.PbcOutMotorDutyLeft =
    ElectronicParkBrake_DW.PbcOutMotorDutyLeft;
  ElectronicParkBrake_Y.PbcOutput.PbcOutMotorDutyRight =
    ElectronicParkBrake_DW.PbcOutMotorDutyRight;
  ElectronicParkBrake_Y.PbcOutput.PbcSensorStatusLeft =
    ElectronicParkBrake_DW.PbcSensorStatusLeft;
  ElectronicParkBrake_Y.PbcOutput.PbcSensorStatusRight =
    ElectronicParkBrake_DW.PbcSensorStatusRight;

  /* Abs: '<S13>/Abs' */
  u0 = fabs(rtb_Add2_e);

  /* Trigonometry: '<S13>/asin' */
  if (u0 > 1.0) {
    u0 = 1.0;
  }

  /* Gain: '<S13>/toPercent' incorporates:
   *  Trigonometry: '<S13>/asin'
   *  Trigonometry: '<S13>/tan'
   */
  u0 = 100.0 * tan(asin(u0));

  /* Saturate: '<S13>/RoadGradeSaturation' */
  if (u0 > 100.0) {
    u0 = 100.0;
  } else {
    if (u0 < -100.0) {
      u0 = -100.0;
    }
  }

  /* End of Saturate: '<S13>/RoadGradeSaturation' */

  /* Signum: '<S13>/Sign' */
  if (rtb_Add2_e < 0.0) {
    rtb_Add2_gq = -1.0;
  } else if (rtb_Add2_e > 0.0) {
    rtb_Add2_gq = 1.0;
  } else {
    rtb_Add2_gq = rtb_Add2_e;
  }

  /* End of Signum: '<S13>/Sign' */

  /* DataTypeConversion: '<S13>/Data Type Conversion2' incorporates:
   *  Gain: '<S13>/Scale'
   *  Product: '<S13>/Product'
   */
  ElectronicParkBrake_DW.PBCtoSSM.PbcDataRoadGrade = (int16_T)floor((100.0 * u0)
    * rtb_Add2_gq);

  /* BusCreator: '<S2>/Bus Creator3' */
  ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateLeft =
    ElectronicParkBrake_DW.PbcOutActuatorStateLeft;
  ElectronicParkBrake_DW.PBCtoSSM.PbcOutActuatorStateRight =
    ElectronicParkBrake_DW.PbcOutActuatorStateRight;
  ElectronicParkBrake_DW.PBCtoSSM.PbcPreviousAppliedForce =
    ElectronicParkBrake_DW.PbcPreviousAppliedForce;
  ElectronicParkBrake_DW.PBCtoSSM.PbcApplyingTypeLeft =
    ElectronicParkBrake_DW.PbcApplyingTypeLeft;
  ElectronicParkBrake_DW.PBCtoSSM.PbcApplyingTypeRight =
    ElectronicParkBrake_DW.PbcApplyingTypeRight;

  /* Update for UnitDelay: '<S7>/PreviousMotorCurrentFiltered' */
  ElectronicParkBrake_DW.PreviousMotorCurrentFiltered_DS = rtb_Add2;

  /* Update for UnitDelay: '<S8>/PreviousMotorCurrentFiltered' */
  ElectronicParkBrake_DW.PreviousMotorCurrentFiltered__c = rtb_Add2_b;

  /* Update for UnitDelay: '<S17>/PreviousCorrectedLongAccelerationFiltered' */
  ElectronicParkBrake_DW.PreviousCorrectedLongAccelerati = rtb_Add2_e;

  /* Update for UnitDelay: '<S18>/PreviousLongAccelerationFiltered' */
  ElectronicParkBrake_DW.PreviousLongAccelerationFiltere = rtb_Add2_j;

  /* Update for UnitDelay: '<S20>/PreviousVehicleAccelerationFiltered' */
  ElectronicParkBrake_DW.PreviousVehicleAccelerationFilt = rtb_Add2_e2;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered10' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered9'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered10_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered9_DSTATE;

  /* Update for UnitDelay: '<S21>/PreviousVehicleSpeedFiltered' */
  ElectronicParkBrake_DW.PreviousVehicleSpeedFiltered_DS = rtb_Add2_d;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered9' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered8'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered9_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered8_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered8' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered7'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered8_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered7_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered7' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered6'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered7_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered6_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered6' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered5'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered6_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered5_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered5' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered4'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered5_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered4_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered4' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered3'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered4_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered3_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered3' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered2'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered3_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered2_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered2' incorporates:
   *  UnitDelay: '<S19>/VehicleSpeedFiltered1'
   */
  ElectronicParkBrake_DW.VehicleSpeedFiltered2_DSTATE =
    ElectronicParkBrake_DW.VehicleSpeedFiltered1_DSTATE;

  /* Update for UnitDelay: '<S19>/VehicleSpeedFiltered1' */
  ElectronicParkBrake_DW.VehicleSpeedFiltered1_DSTATE = rtb_Add2_d;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
