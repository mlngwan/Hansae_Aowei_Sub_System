/*
 * File: StandStillManager.c
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

#include "StandStillManager.h"

/* Include model header file for global data */
#include "ElectronicParkBrake.h"
#include "ElectronicParkBrake_private.h"

/* Named constants for Chart: '<S3>/IgnitionStateChart' */
#define ElectronicParkBr_IN_IgnitionOFF ((uint8_T)2U)
#define ElectronicParkBrake_IN_EngineOn ((uint8_T)1U)
#define ElectronicParkBrake_IN_Unknown ((uint8_T)3U)

/* Forward declaration for local functions */
static void ElectronicParkBra_IgnitionState(uint16_T *SSMDataIgnitionState);
static void El_enter_internal_IgnitionState(uint16_T *SSMDataIgnitionState);
static uint8_T ElectronicParkB_ParkBrakeStLeft(void);
static uint8_T ElectronicPark_ParkBrakeStRight(void);
static boolean_T ElectronicParkBra_FailureStatus(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T);
static uint8_T ElectronicParkBra_StatusIndLeft(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T);
static uint8_T ElectronicParkBr_StatusIndRight(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T);
static void ElectronicParkBrak_SSMOutputCAN(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T);
static void Elect_enter_atomic_SSMOutputCAN(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T);

/* Function for Chart: '<S3>/IgnitionStateChart' */
static void ElectronicParkBra_IgnitionState(uint16_T *SSMDataIgnitionState)
{
  /* During 'IgnitionState': '<S25>:20' */
  /* 2020.11.09 Seungbum Han

     Ignition 상태를 판단하는 State

   * 대창 EPB의 경우 Engine State 관련된 Signal이 없으므로, Ignition 상태로만 판단한다.
   */
  switch (ElectronicParkBrake_DW.is_IgnitionState) {
   case ElectronicParkBrake_IN_EngineOn:
    *SSMDataIgnitionState = (uint16_T)IgnitionState_EngineON;

    /* Inport: '<Root>/SSMInput' */
    /* During 'EngineOn': '<S25>:28' */
    /* 2020.11.10 Seungbum Han

       Ignition Input이 없을 경우 Ignition OFF State로 변경됨. */
    if (((int32_T)SSMInput.SSMInIgnition) == 0) {
      /* Transition: '<S25>:24' */
      ElectronicParkBrake_DW.is_IgnitionState = ElectronicParkBr_IN_IgnitionOFF;

      /* Entry 'IgnitionOFF': '<S25>:27' */
      *SSMDataIgnitionState = (uint16_T)IgnitionState_IgnitionOFF;
    }
    break;

   case ElectronicParkBr_IN_IgnitionOFF:
    *SSMDataIgnitionState = (uint16_T)IgnitionState_IgnitionOFF;

    /* Inport: '<Root>/SSMInput' */
    /* During 'IgnitionOFF': '<S25>:27' */
    /* 2020.11.10 Seungbum Han

       Ignition Input이 있을 경우 Engine ON state로 변경됨. */
    if (((int32_T)SSMInput.SSMInIgnition) == 1) {
      /* Transition: '<S25>:25' */
      ElectronicParkBrake_DW.is_IgnitionState = ElectronicParkBrake_IN_EngineOn;

      /* Entry 'EngineOn': '<S25>:28' */
      *SSMDataIgnitionState = (uint16_T)IgnitionState_EngineON;
    }
    break;

   default:
    *SSMDataIgnitionState = (uint16_T)IgnitionState_Unknown;

    /* Inport: '<Root>/SSMInput' */
    /* During 'Unknown': '<S25>:26' */
    /* 2020.11.10 Seungbum Han

       최초에 Ignition State는 Unknown 이며,
       이후 Ignition Input이 있을 경우 Engine ON.
       Ignition Input이 없을 경우 Ignition OFF State로 변경됨.

     */
    switch (SSMInput.SSMInIgnition) {
     case 0:
      /* Transition: '<S25>:22' */
      ElectronicParkBrake_DW.is_IgnitionState = ElectronicParkBr_IN_IgnitionOFF;

      /* Entry 'IgnitionOFF': '<S25>:27' */
      *SSMDataIgnitionState = (uint16_T)IgnitionState_IgnitionOFF;
      break;

     case 1:
      /* Transition: '<S25>:23' */
      ElectronicParkBrake_DW.is_IgnitionState = ElectronicParkBrake_IN_EngineOn;

      /* Entry 'EngineOn': '<S25>:28' */
      *SSMDataIgnitionState = (uint16_T)IgnitionState_EngineON;
      break;

     default:
      /* no actions */
      break;
    }
    break;
  }
}

/* Function for Chart: '<S3>/IgnitionStateChart' */
static void El_enter_internal_IgnitionState(uint16_T *SSMDataIgnitionState)
{
  /* Entry Internal 'IgnitionState': '<S25>:20' */
  /* Transition: '<S25>:21' */
  ElectronicParkBrake_DW.is_IgnitionState = ElectronicParkBrake_IN_Unknown;

  /* Entry 'Unknown': '<S25>:26' */
  *SSMDataIgnitionState = (uint16_T)IgnitionState_Unknown;
}

/* Function for Chart: '<S3>/CAN_Tx' */
static uint8_T ElectronicParkB_ParkBrakeStLeft(void)
{
  uint8_T CurrParkBrakeStLeft;

  /* Graphical Function 'ParkBrakeStLeft': '<S22>:3044' */
  /* Transition: '<S22>:3059' */
  /* Transition: '<S22>:3060' */
  /*  ParkApplied HoldApplied  */
  if ((((int32_T)ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft)
       == ((int32_T)ActuatorState_ParkApplied)) || (((int32_T)
        ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
       ((int32_T)ActuatorState_EmergencyApplied))) {
    /* Transition: '<S22>:3061' */
    /* Transition: '<S22>:3063' */
    /*  'A2':Engaged  */
    CurrParkBrakeStLeft = 2U;
  } else {
    /* Transition: '<S22>:3062' */
    /*  Released CompletelyReleased  */
    if ((((int32_T)
          ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
         ((int32_T)ActuatorState_Released)) || (((int32_T)
          ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
         ((int32_T)ActuatorState_CompletelyReleased))) {
      /* Transition: '<S22>:3064' */
      /* Transition: '<S22>:3066' */
      /*  'A1':Released  */
      CurrParkBrakeStLeft = 1U;
    } else {
      /* Transition: '<S22>:3065' */
      /*  Applying  */
      if (((int32_T)
           ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
          ((int32_T)ActuatorState_Applying)) {
        /* Transition: '<S22>:3067' */
        /* Transition: '<S22>:3069' */
        /*  'A3':Engagement Under Progress  */
        CurrParkBrakeStLeft = 4U;
      } else {
        /* Transition: '<S22>:3068' */
        /*  Releasing  */
        if (((int32_T)
             ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
            ((int32_T)ActuatorState_Releasing)) {
          /* Transition: '<S22>:3070' */
          /* Transition: '<S22>:3072' */
          /*  'A4':Release Under Progress  */
          CurrParkBrakeStLeft = 3U;
        } else {
          /* Transition: '<S22>:3071' */
          /*  'A0':Not Initialized  */
          CurrParkBrakeStLeft = 0U;
        }
      }
    }
  }

  return CurrParkBrakeStLeft;
}

/* Function for Chart: '<S3>/CAN_Tx' */
static uint8_T ElectronicPark_ParkBrakeStRight(void)
{
  uint8_T CurrParkBrakeStRight;

  /* Graphical Function 'ParkBrakeStRight': '<S22>:3120' */
  /* Transition: '<S22>:3135' */
  /* Transition: '<S22>:3136' */
  /*  ParkApplied HoldApplied  */
  if ((((int32_T)
        ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
       ((int32_T)ActuatorState_ParkApplied)) || (((int32_T)
        ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
       ((int32_T)ActuatorState_EmergencyApplied))) {
    /* Transition: '<S22>:3137' */
    /* Transition: '<S22>:3138' */
    /*  'A2':Engaged  */
    CurrParkBrakeStRight = 2U;
  } else {
    /* Transition: '<S22>:3139' */
    /*  Released CompletelyReleased  */
    if ((((int32_T)
          ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
         ((int32_T)ActuatorState_Released)) || (((int32_T)
          ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
         ((int32_T)ActuatorState_CompletelyReleased))) {
      /* Transition: '<S22>:3140' */
      /* Transition: '<S22>:3141' */
      /*  'A1':Released  */
      CurrParkBrakeStRight = 1U;
    } else {
      /* Transition: '<S22>:3142' */
      /*  Applying  */
      if (((int32_T)
           ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
          ((int32_T)ActuatorState_Applying)) {
        /* Transition: '<S22>:3143' */
        /* Transition: '<S22>:3145' */
        /*  'A3':Engagement Under Progress  */
        CurrParkBrakeStRight = 4U;
      } else {
        /* Transition: '<S22>:3144' */
        /*  Releasing  */
        if (((int32_T)
             ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
            ((int32_T)ActuatorState_Releasing)) {
          /* Transition: '<S22>:3146' */
          /* Transition: '<S22>:3148' */
          /*  'A4':Release Under Progress  */
          CurrParkBrakeStRight = 3U;
        } else {
          /* Transition: '<S22>:3147' */
          /*  'A0':Not Initialized  */
          CurrParkBrakeStRight = 0U;
        }
      }
    }
  }

  return CurrParkBrakeStRight;
}

/* Function for Chart: '<S3>/CAN_Tx' */
static boolean_T ElectronicParkBra_FailureStatus(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T)
{
  boolean_T FailureSts;

  /* Graphical Function 'FailureStatus': '<S22>:3032' */
  /* Transition: '<S22>:3038' */
  /* Transition: '<S22>:3039' */
  /*  Failure  */
  if ((BusConversion_InsertedFor_CAN_T->D21_CAN_BusOff) ||
      (BusConversion_InsertedFor_CAN_T->C4_VCU)) {
    /* Transition: '<S22>:3040' */
    /* Transition: '<S22>:3042' */
    /*  'A1':Failure  */
    FailureSts = true;
  } else {
    /* Transition: '<S22>:3041' */
    /*  'A0':No Failure  */
    FailureSts = false;
  }

  return FailureSts;
}

/* Function for Chart: '<S3>/CAN_Tx' */
static uint8_T ElectronicParkBra_StatusIndLeft(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T)
{
  uint8_T StatusIndReqLeft;

  /* Graphical Function 'StatusIndLeft': '<S22>:3074' */
  /* Transition: '<S22>:3088' */
  /*  FailModeAppling */
  if (((int32_T)ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcApplyingTypeLeft) ==
      ((int32_T)ApplyReleaseRequest_EmergencyApply)) {
    /* Transition: '<S22>:3092' */
    /* Transition: '<S22>:3094' */
    /*  'A2':Emergency Appling Indication  */
    StatusIndReqLeft = 2U;
  } else {
    /* Transition: '<S22>:3230' */
    /*  Failure  */
    if (((((((((BusConversion_InsertedFor_CAN_T->D1_Actuator) ||
               (BusConversion_InsertedFor_CAN_T->D2_Battery)) ||
              (BusConversion_InsertedFor_CAN_T->D3_MotorDriver)) ||
             (BusConversion_InsertedFor_CAN_T->D4_Temperature)) ||
            (BusConversion_InsertedFor_CAN_T->D11_MtrCurOvA)) ||
           (BusConversion_InsertedFor_CAN_T->D11_MtrCurUvA)) ||
          (BusConversion_InsertedFor_CAN_T->D11_MtrCurOvB)) ||
         (BusConversion_InsertedFor_CAN_T->D11_MtrCurUvB)) ||
        (BusConversion_InsertedFor_CAN_T->D20_EPB_Stuck)) {
      /* Transition: '<S22>:3098' */
      /* Transition: '<S22>:3100' */
      /*  'A3':Failure Indication  */
      StatusIndReqLeft = 3U;
    } else {
      /* Transition: '<S22>:3209' */
      /*  Applied */
      if ((((int32_T)
            ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
           ((int32_T)ActuatorState_ParkApplied)) || (((int32_T)
            ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateLeft) ==
           ((int32_T)ActuatorState_EmergencyApplied))) {
        /* Transition: '<S22>:3089' */
        /* Transition: '<S22>:3091' */
        /*  'A1':Engaged Brake Indication  */
        StatusIndReqLeft = 1U;
      } else {
        /* Transition: '<S22>:3093' */
        /*  Released CompletelyReleased Applying / Releasing
           'A0':No Indication  */
        StatusIndReqLeft = 0U;
      }
    }
  }

  return StatusIndReqLeft;
}

/* Function for Chart: '<S3>/CAN_Tx' */
static uint8_T ElectronicParkBr_StatusIndRight(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T)
{
  uint8_T StatusIndReqRight;

  /* Graphical Function 'StatusIndRight': '<S22>:3178' */
  /* Transition: '<S22>:3241' */
  /*  FailModeAppling */
  if (((int32_T)ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcApplyingTypeRight) ==
      ((int32_T)ApplyReleaseRequest_EmergencyApply)) {
    /* Transition: '<S22>:3248' */
    /* Transition: '<S22>:3250' */
    /*  'A2':Emergency Apply Indication  */
    StatusIndReqRight = 2U;
  } else {
    /* Transition: '<S22>:3231' */
    /*  Failure  */
    if (((((((((BusConversion_InsertedFor_CAN_T->D1_Actuator) ||
               (BusConversion_InsertedFor_CAN_T->D2_Battery)) ||
              (BusConversion_InsertedFor_CAN_T->D3_MotorDriver)) ||
             (BusConversion_InsertedFor_CAN_T->D4_Temperature)) ||
            (BusConversion_InsertedFor_CAN_T->D11_MtrCurOvA)) ||
           (BusConversion_InsertedFor_CAN_T->D11_MtrCurUvA)) ||
          (BusConversion_InsertedFor_CAN_T->D11_MtrCurOvB)) ||
         (BusConversion_InsertedFor_CAN_T->D11_MtrCurUvB)) ||
        (BusConversion_InsertedFor_CAN_T->D20_EPB_Stuck)) {
      /* Transition: '<S22>:3242' */
      /* Transition: '<S22>:3245' */
      /*  'A3':Failure Indication  */
      StatusIndReqRight = 3U;
    } else {
      /* Transition: '<S22>:3243' */
      /*  Applied */
      if ((((int32_T)
            ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
           ((int32_T)ActuatorState_ParkApplied)) || (((int32_T)
            ElectronicParkBrake_DW.PreviousPBCtoSSM.PbcOutActuatorStateRight) ==
           ((int32_T)ActuatorState_EmergencyApplied))) {
        /* Transition: '<S22>:3246' */
        /* Transition: '<S22>:3247' */
        /*  'A1':Engaged Brake Indication  */
        StatusIndReqRight = 1U;
      } else {
        /* Transition: '<S22>:3249' */
        /*  Released CompletelyReleased Applying / Releasing
           'A0':No Indication  */
        StatusIndReqRight = 0U;
      }
    }
  }

  return StatusIndReqRight;
}

/* Function for Chart: '<S3>/CAN_Tx' */
static void ElectronicParkBrak_SSMOutputCAN(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T)
{
  /* SystemInitialize for Outport: '<Root>/SSMOutputCAN' */
  /* During 'SSMOutputCAN': '<S22>:3' */
  /* SSMOutputCAN.EPB_AudibleChimeReq = false, */
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrParkBrakeStLeft =
    ElectronicParkB_ParkBrakeStLeft();
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrParkBrakeStRight =
    ElectronicPark_ParkBrakeStRight();

  /* SSMOutputCAN.EPB_BrakeLightReq = BrakeLight(),
     SSMOutputCAN.EPB_AppliedForceSt = AppliedForce(PBCtoSSM.PbcPreviousAppliedForce),
     SSMOutputCAN.EPB_TargetDecel = uint8(EPB_TargetDecel), */
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_FailureSts =
    ElectronicParkBra_FailureStatus(BusConversion_InsertedFor_CAN_T);
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_StatusIndReqLeft =
    ElectronicParkBra_StatusIndLeft(BusConversion_InsertedFor_CAN_T);
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_StatusIndReqRight =
    ElectronicParkBr_StatusIndRight(BusConversion_InsertedFor_CAN_T);

  /* EPB_DynamicEmergencyApply = EPB_DynamicEmergencyApply,
     SSMOutputCAN.EPB_CurrEPBSwSt = SwSt(SSMInput.SSMInSwitch); */
}

/* Function for Chart: '<S3>/CAN_Tx' */
static void Elect_enter_atomic_SSMOutputCAN(const DiagFlagBus
  *BusConversion_InsertedFor_CAN_T)
{
  /* SystemInitialize for Outport: '<Root>/SSMOutputCAN' */
  /* Entry 'SSMOutputCAN': '<S22>:3' */
  /* SSMOutputCAN.EPB_AudibleChimeReq = false, */
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrParkBrakeStLeft =
    ElectronicParkB_ParkBrakeStLeft();
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrParkBrakeStRight =
    ElectronicPark_ParkBrakeStRight();

  /* SSMOutputCAN.EPB_BrakeLightReq = BrakeLight(),
     SSMOutputCAN.EPB_AppliedForceSt = AppliedForce(PBCtoSSM.PbcPreviousAppliedForce),
     SSMOutputCAN.EPB_TargetDecel = uint8(EPB_TargetDecel), */
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_FailureSts =
    ElectronicParkBra_FailureStatus(BusConversion_InsertedFor_CAN_T);
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_StatusIndReqLeft =
    ElectronicParkBra_StatusIndLeft(BusConversion_InsertedFor_CAN_T);
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_StatusIndReqRight =
    ElectronicParkBr_StatusIndRight(BusConversion_InsertedFor_CAN_T);
}

/* System initialize for atomic system: '<S1>/StandStillManager' */
void Electron_StandStillManager_Init(void)
{
  /* SystemInitialize for Chart: '<S3>/StandStillManager' */
  ElectronicParkBrake_DW.SSMDataIsAutomaticApplyDisable = true;
  ElectronicParkBrake_DW.SSMDataZeroVehSpeedTimer = 100U;
  ElectronicParkBrake_DW.SSMDataAfterRunTimer = 1000U;
  ElectronicParkBrake_DW.SSMOutRollerbenchCondition = 15U;
  ElectronicParkBrake_DW.SSMOutRollerbenchRoutineResults = 15U;
  ElectronicParkBrake_DW.SSMOutRollerbenchPhase = 1U;
  ElectronicParkBrake_DW.SSMOutRollerbenchRoutineID = 255U;

  /* SystemInitialize for Chart: '<S3>/CAN_Tx' incorporates:
   *  Outport: '<Root>/SSMOutputCAN'
   */
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_AudibleChimeReq = false;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrParkBrakeStLeft = 0U;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrParkBrakeStRight = 0U;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_BrakeLightReq = false;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_AppliedForceSt = 0U;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_TargetDecel = 0U;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_FailureSts = false;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_StatusIndReqLeft = 0U;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_StatusIndReqRight = 0U;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_DynamicEmergencyApply = false;
  ElectronicParkBrake_Y.SSMOutputCAN.EPB_CurrEPBSwSt = 0U;
}

/*
 * Output and update for atomic system: '<S1>/StandStillManager'
 * Block description for: '<S1>/StandStillManager'
 *   2020.11.13 Seungbum Han
 *   - 기능의 동작 가/불 여부 (Degrade Mode)
 *   - 기능 조건판단(StandStillmanager)
 *   - 동작 상태에 따른 Indication out
 */
void ElectronicPar_StandStillManager(void)
{
  uint16_T SSMDataIgnitionState;

  /* Chart: '<S3>/IgnitionStateChart' */
  /* Gateway: ElectronicParkBrake/StandStillManager/IgnitionStateChart */
  /* During: ElectronicParkBrake/StandStillManager/IgnitionStateChart */
  /* 2020.11.11 Seungbum Han

     Ignition State 판단을 위함이며,
     Daechang의 경우 Hardware 신호에 따라 구분되는 단순한 State임. */
  if (((uint32_T)ElectronicParkBrake_DW.is_active_c8_ElectronicParkBrak) == 0U)
  {
    /* Entry: ElectronicParkBrake/StandStillManager/IgnitionStateChart */
    ElectronicParkBrake_DW.is_active_c8_ElectronicParkBrak = 1U;

    /* Entry Internal: ElectronicParkBrake/StandStillManager/IgnitionStateChart */
    El_enter_internal_IgnitionState(&SSMDataIgnitionState);
  } else {
    ElectronicParkBra_IgnitionState(&SSMDataIgnitionState);
  }

  /* End of Chart: '<S3>/IgnitionStateChart' */

  /* BusCreator: '<S3>/Bus Creator' */
  ElectronicParkBrake_DW.SSMtoPBC.PbcInDegradedIntelligent =
    ElectronicParkBrake_DW.SSMDataDegradedIntelligent;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInHallControllerValid = false;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInIgnitionState = SSMDataIgnitionState;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInShutDown =
    ElectronicParkBrake_DW.SSMOutTZeroVehSpeedDetectFlag;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequest =
    ElectronicParkBrake_DW.PbcInApplyReleaseRequest;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInRollerbenchTargetForce =
    ElectronicParkBrake_DW.PbcInRollerbenchTargetForce;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInDynamicApply =
    ElectronicParkBrake_DW.SSMDataDynamicApply;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInServiceRelease =
    ElectronicParkBrake_DW.SSMDataServiceRelease;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInRollerbenchActive =
    ElectronicParkBrake_DW.SSMOutRollerbenchActive;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInVehicleSpeed = 0U;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInWheelSlipRL = 0.0F;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInWheelSlipRR = 0.0F;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInAcceleration = 0;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInWSS_Fault_Type = 0U;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequestLeft =
    ElectronicParkBrake_DW.PbcInApplyReleaseRequestLeft;
  ElectronicParkBrake_DW.SSMtoPBC.PbcInApplyReleaseRequestRight =
    ElectronicParkBrake_DW.PbcInApplyReleaseRequestRight;

  /* BusCreator: '<S3>/Bus Creator1' incorporates:
   *  Outport: '<Root>/SSMOutput'
   */
  ElectronicParkBrake_Y.SSMOutput.SSMOutTZeroVehSpeedDetect =
    ElectronicParkBrake_DW.SSMOutTZeroVehSpeedDetectFlag;
  ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchCondition =
    ElectronicParkBrake_DW.SSMOutRollerbenchCondition;
  ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchRoutineResults =
    ElectronicParkBrake_DW.SSMOutRollerbenchRoutineResults;
  ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchPhase =
    ElectronicParkBrake_DW.SSMOutRollerbenchPhase;
  ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchRoutineID =
    ElectronicParkBrake_DW.SSMOutRollerbenchRoutineID;
  ElectronicParkBrake_Y.SSMOutput.SSMOutRollerbenchActive =
    ElectronicParkBrake_DW.SSMOutRollerbenchActive;

  /* Chart: '<S3>/CAN_Tx' incorporates:
   *  Inport: '<Root>/DiagInput'
   */
  /* Gateway: ElectronicParkBrake/StandStillManager/CAN_Tx */
  /* During: ElectronicParkBrake/StandStillManager/CAN_Tx */
  /* 2020.11.11 Seungbum Han

     EPB 기능에 따른 Display될 항목을 Manage 하는부분
     1. Brake Status.
     2. Brake Light
     3. Applied Force
     4. Status Indicate
     5. Failure State
     6. Switch State */
  if (((uint32_T)ElectronicParkBrake_DW.is_active_c4_ElectronicParkBrak) == 0U)
  {
    /* Entry: ElectronicParkBrake/StandStillManager/CAN_Tx */
    ElectronicParkBrake_DW.is_active_c4_ElectronicParkBrak = 1U;

    /* Entry Internal: ElectronicParkBrake/StandStillManager/CAN_Tx */
    Elect_enter_atomic_SSMOutputCAN(&DiagInput);
  } else {
    ElectronicParkBrak_SSMOutputCAN(&DiagInput);
  }

  /* End of Chart: '<S3>/CAN_Tx' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
