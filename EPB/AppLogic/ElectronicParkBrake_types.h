/*
 * File: ElectronicParkBrake_types.h
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

#ifndef RTW_HEADER_ElectronicParkBrake_types_h_
#define RTW_HEADER_ElectronicParkBrake_types_h_
#include "rtwtypes.h"
#include "LookupTable.h"
#ifndef DEFINED_TYPEDEF_FOR_DiagFlagBus_
#define DEFINED_TYPEDEF_FOR_DiagFlagBus_

typedef struct {
  boolean_T D1_Actuator;
  boolean_T D2_Battery;
  boolean_T D3_MotorDriver;
  boolean_T D4_Temperature;
  boolean_T D5_SwitchApply;
  boolean_T D6_SwitchRelease;
  boolean_T D7_SwitchNeutral;
  boolean_T D8_SwitchSignalCross;
  boolean_T D11_MtrCurOvA;
  boolean_T D11_MtrCurUvA;
  boolean_T D11_MtrCurOvB;
  boolean_T D11_MtrCurUvB;
  boolean_T D13_FIU_VM;
  boolean_T D14;
  boolean_T D15_EMS;
  boolean_T D16_TCU;
  boolean_T D17_ESP;
  boolean_T D18_YRS;
  boolean_T D19_ACU;
  boolean_T D20_EPB_Stuck;
  boolean_T D21_CAN_BusOff;
  boolean_T D22_ForceAndHallSensor;
  boolean_T C1_EMS_3;
  boolean_T C2_ECU_14;
  boolean_T C3_ECU_Veh_02;
  boolean_T C4_VCU;
  boolean_T C5_ESP_speed;

  /* FALSE:Available  TRUE:Not available */
  boolean_T C6_CDPDecelAvailable;
  boolean_T C7_ESP_04;
  boolean_T C8_ESP_09;
  boolean_T C9_ESP_YRS_02;
  boolean_T C10_ACU_01;
  boolean_T C11_BCM;
  boolean_T C12_BCM_LVBatteryVoltage;
  boolean_T C13_LowVoltage;
  boolean_T C14_ratAccPedSts;
  boolean_T C15_VehicleSpeed_ValueError;
} DiagFlagBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SSMInputBus_
#define DEFINED_TYPEDEF_FOR_SSMInputBus_

/* 2020.11.12 Seungbum Han
   SSM에서 필요한 Input Bus.
   Firmware로 부터 받으며, Interface 정의 필요. */
typedef struct {
  uint16_T SSMInSwitch;
  uint16_T SSMInIgnition;
  real32_T SSMInVoltage;
  uint16_T SSMInVehicleModeState;
  uint16_T SSMInCoEng_st;

  /* 0 : Full Mode
     1 : Manual Mode */
  uint16_T SSMInEPB_OpMode;
  uint16_T SSMInGear;

  /* [ NAMI ]
     Scaling Factor : 0.01
     Offset : 0
     Data type : uint16
     Unit : kph */
  uint16_T SSMInVehicleSpeed;
  uint16_T SSMInBrakePedal;
  uint16_T SSMInAccelator;
  uint16_T SSMInIsDoorOpen;
  uint16_T SSMInIsSeatbeltUnfasten;
  uint16_T SSMInIsSportMode;
  uint16_T SSMInAVH_Active;
  uint16_T SSMInRequestToEPB;

  /* Indicates that deceleration request from EPB is executed
     0x0 Not Active
     0x1 Active */
  boolean_T SSMInCDPDecelActive;

  /* Indicates that CDP is available for decelerating the vehicle.
     0x0 = Not Available
     0x1 = Available */
  boolean_T SSMInCDPDecelAvailable;

  /* [ NAMI ] */
  uint16_T SSMInMotClutch;

  /* 0x00 : None
     0x01 : Reserved
     0x02 : Stop command
     0x03~4D : Reserved
     0x4E : Start routine 184E
     0x4F~FF : Reserved */
  uint16_T SSMInRollerbenchRoutine;
  uint16_T SSMInDiagnosisInput;

  /* 1: Default Session
     3: extended Session */
  uint16_T SSMInSession;

  /* Resolution : 10 */
  uint16_T SSMInWheelSpeedFL;

  /* Resolution : 10 */
  uint16_T SSMInWheelSpeedFR;

  /* Resolution : 10 */
  uint16_T SSMInWheelSpeedRL;

  /* Resolution : 10 */
  uint16_T SSMInWheelSpeedRR;

  /* 2021.07.21. Seungryong Baek
     Changed from int16 to float to increase the resolution of slip ratio. */
  real32_T SSMInWheelSlipRL;

  /* 2021.07.21. Seungryong Baek
     Changed from int16 to float to increase the resolution of slip ratio. */
  real32_T SSMInWheelSlipRR;

  /* 2021.07.21. Seungryong Baek.
     Longitudinal acceleration of the vehicle.
     Negative numbers indicate slowdown.
     Scale Factor : 1/1000 */
  int16_T SSMInAcceleration;
  uint16_T SSMInWSS_Fault_Type;

  /* Indicates that CDP is available for decelerating the vehicle.
     0x0 = Not Available
     0x1 = Available */
  uint16_T SSMInIsCanCommandLeft;

  /* Indicates that CDP is available for decelerating the vehicle.
     0x0 = Not Available
     0x1 = Available */
  uint16_T SSMInIsCanCommandRight;
  real32_T SSMInTemperature;
  real32_T SSMInMotorVoltage;
} SSMInputBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PbcInputBus_
#define DEFINED_TYPEDEF_FOR_PbcInputBus_

/* 2020.11.12 Seungbum Han
   [Daechang]
   - RoadGrade에 따라 Intelligent Apply를 위한 Function.
   - RoadGrade 및 방향 파악을 위한 Parameter로 미사용됨. */
typedef struct {
  /* [ NAMI ]
     <LongAcceleration>
     Scaling Factor : 0.0001274
     Offset : -4.1745
     Data Type : uint16
     Unit : g
     <RoadGrade>
     Scaling Factor : 100
     Data Type : uint16
     Unit : % */
  uint16_T PbcInLongAcceleration;
  uint16_T PbcInWheelDIrectionInfoRL;
  uint16_T PbcInWheelDIrectionInfoRR;
} PbcInputBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PbcInDriverBus_
#define DEFINED_TYPEDEF_FOR_PbcInDriverBus_

typedef struct {
  /* Scale : 100 */
  real32_T MotorCurrentLeft;

  /* Scale : 100 */
  real32_T MotorVoltageLeft;
  uint16_T MotorDriverStateLeft;

  /* Scale : 100 */
  real32_T MotorCurrentRight;

  /* Scale : 100 */
  real32_T MotorVoltageRight;
  uint16_T MotorDriverStateRight;

  /* Scale : 100 */
  uint16_T MotorDriverSupplyVoltage;
} PbcInDriverBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PbcOutputBus_
#define DEFINED_TYPEDEF_FOR_PbcOutputBus_

typedef struct {
  uint16_T PbcOutMotorCommandLeft;
  uint16_T PbcOutMotorCommandRight;
  boolean_T PbcOutEcuPowerLatchRequest;
  uint16_T PbcOutMotorDutyLeft;
  uint16_T PbcOutMotorDutyRight;
  uint16_T PbcSensorStatusLeft;
  uint16_T PbcSensorStatusRight;
} PbcOutputBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PBCtoSSMBus_
#define DEFINED_TYPEDEF_FOR_PBCtoSSMBus_

typedef struct {
  uint16_T PbcOutActuatorStateLeft;
  uint16_T PbcOutActuatorStateRight;
  uint16_T PbcPreviousAppliedForce;
  int16_T PbcDataRoadGrade;
  uint16_T PbcApplyingTypeLeft;
  uint16_T PbcApplyingTypeRight;
} PBCtoSSMBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SSMtoPBCBus_
#define DEFINED_TYPEDEF_FOR_SSMtoPBCBus_

/* 2020.11.12 Seungbum Han
   PBC에서 필요한 SSM의 Output Bus */
typedef struct {
  boolean_T PbcInDegradedIntelligent;
  boolean_T PbcInHallControllerValid;
  uint16_T PbcInIgnitionState;
  boolean_T PbcInShutDown;
  uint16_T PbcInApplyReleaseRequest;
  uint16_T PbcInRollerbenchTargetForce;
  uint16_T PbcInDynamicApply;
  boolean_T PbcInServiceRelease;
  uint8_T PbcInRollerbenchActive;

  /* [ NAMI ]
     Scaling Factor : 0.01
     Offset : 0
     Data type : uint16
     Unit : kph */
  uint16_T PbcInVehicleSpeed;

  /* 2021.07.21. Seungryong Baek
     Changed from int16 to float to increase the resolution of slip ratio. */
  real32_T PbcInWheelSlipRL;

  /* 2021.07.21. Seungryong Baek
     Changed from int16 to float to increase the resolution of slip ratio. */
  real32_T PbcInWheelSlipRR;

  /* 2021.07.21. Seungryong Baek.
     Longitudinal acceleration of the vehicle.
     Negative numbers indicate slowdown.
     Scale Factor : 1/1000 */
  int16_T PbcInAcceleration;
  uint16_T PbcInWSS_Fault_Type;
  uint16_T PbcInApplyReleaseRequestLeft;
  uint16_T PbcInApplyReleaseRequestRight;
} SSMtoPBCBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PbcInSlipControl_
#define DEFINED_TYPEDEF_FOR_PbcInSlipControl_

typedef struct {
  /* 2021.07.21. Seungryong Baek
     Changed from int16 to float to increase the resolution of slip ratio. */
  real32_T SCInRL;

  /* 2021.07.21. Seungryong Baek
     Changed from int16 to float to increase the resolution of slip ratio. */
  real32_T SCInRR;

  /* 2021.07.21. Seungryong Baek.
     Longitudinal acceleration of the vehicle.
     Negative numbers indicate slowdown.
     Scale Factor : 1/1000 */
  int16_T SCInAX;
} PbcInSlipControl;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SSMOutputBus_
#define DEFINED_TYPEDEF_FOR_SSMOutputBus_

/* 2020.11.12 Seungbum Han
   SSM Output Bus, RollerBench 관련. */
typedef struct {
  boolean_T SSMOutTZeroVehSpeedDetect;

  /* 0x0 : Not Correct
     0x1 : Correct
     0x2 : Entrance condition check
     0x3~E : Reserved
     0xF : Default */
  uint8_T SSMOutRollerbenchCondition;

  /* 0x0 : Routine successfully completed
     0x1 : Routine failed to complete - EPB fault occurred, Entrance condition not correct, Exit condition
     0x2 : Routine failed to complete - Routine stop commanded or session time-out occurred during routine operation
     0x3 : Routine in progress
     0x4~E : Reserved
     0xF : Default */
  uint8_T SSMOutRollerbenchRoutineResults;

  /* 0x0 : Reserved
     0x1 : Initialize state
     0x2 : Standby state
     0x3 : Start routine - Entrance condition check
     0x4 : Start routine - Positive or negative response
     0x5 : Routine in progress
     0x6 : EPB Releasing - Stop command / session time-out / Exit condition / Termination
     0x7~F : Reserved */
  uint8_T SSMOutRollerbenchPhase;

  /* 0x00~4D : Reserved
     0x4E : 0x184E - The EPB system to apply several pre-set force levels to the parking brake mechanism.
     0x4F : 0x184F - Bedding procedure
     0x50~FE : Reserved
     0xFF : Default */
  uint16_T SSMOutRollerbenchRoutineID;

  /* 0x0 : Inactive
     0x1 : Active
     0x2~F : Reserved */
  uint8_T SSMOutRollerbenchActive;
} SSMOutputBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SSMOutputCANBus_
#define DEFINED_TYPEDEF_FOR_SSMOutputCANBus_

/* 2020.11.12 Seungbum Han
   SSM 내부에서 생성된 Indication, Failure, Switch State등과 관련된 CAN Tx Signal Output Bus */
typedef struct {
  /* 0x0 : No Request
     0x1 : Request */
  boolean_T EPB_AudibleChimeReq;

  /* 0x0 : Not Initialized
     0x1 : Released
     0x2 : Engaged
     0x3 : Release Under Progress
     0x4 : Engagement Under Progress
     0x5 : Reserved */
  uint8_T EPB_CurrParkBrakeStLeft;

  /* 0x0 : Not Initialized
     0x1 : Released
     0x2 : Engaged
     0x3 : Release Under Progress
     0x4 : Engagement Under Progress
     0x5 : Reserved */
  uint8_T EPB_CurrParkBrakeStRight;

  /* 0x0 : No Request
     0x1 : Request */
  boolean_T EPB_BrakeLightReq;

  /* 0x0 : No force applied
     0x1 : Low force applied
     0x2 : Medium force applied
     0x3 : Maximum force applied */
  uint8_T EPB_AppliedForceSt;

  /* Unit : m/s^2
     Factor : 0.05
     Offset : ?
     Minimum Value : -4m/s^2 (-80)
     Maximum Value : 8m/s^2 (160) */
  uint8_T EPB_TargetDecel;

  /* 0x0 : No Failure
     0x1 : Failure */
  boolean_T EPB_FailureSts;

  /* 0x0 : No Indication
     0x1 : Engaged Brake Indication
     0x2 : Dynamic Braking Indication
     0x3 : Failure Indication */
  uint8_T EPB_StatusIndReqLeft;

  /* 0x0 : No Indication
     0x1 : Engaged Brake Indication
     0x2 : Dynamic Braking Indication
     0x3 : Failure Indication */
  uint8_T EPB_StatusIndReqRight;

  /* 0x0 : Not Enabled
     0x1 : Enabled */
  boolean_T EPB_DynamicEmergencyApply;

  /* 0x0 : No Request
     0x1 : Release Brake
     0x2 : Lock Brake
     0x3 : Fault */
  uint8_T EPB_CurrEPBSwSt;
} SSMOutputCANBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_DegradedModeBus_
#define DEFINED_TYPEDEF_FOR_DegradedModeBus_

/* 2020.11.13 Seungbum Han
   [Daechang]
   SSM 제어를 위해 Degrade Mode 결과를 SSM으로 전달. */
typedef struct {
  /* 0:OFF  1:ON */
  boolean_T AutoEmergencyApply;

  /* 0:OFF  1:ON  2:Xsec */
  boolean_T ManualApply;

  /* 0:OFF  1:ON  2:Xsec */
  boolean_T ServiceApply;

  /* 0:OFF  1:ON  2:Xsec */
  boolean_T ServiceRelease;

  /* 0:OFF  1:ON  2:Xsec */
  boolean_T ManualRelease;

  /* FALSE:OFF  TRUE:ON */
  boolean_T AutoPark;

  /* FALSE:OFF  TRUE:ON */
  boolean_T AutoSafety;

  /* FALSE:OFF  TRUE:ON */
  boolean_T AutoGearP;

  /* FALSE:OFF  TRUE:ON */
  boolean_T AutoAVH;

  /* FALSE:OFF  TRUE:ON */
  boolean_T DriveAway;

  /* FALSE:OFF  TRUE:ON */
  boolean_T Intelligent;

  /* FALSE:OFF  TRUE:ON */
  boolean_T ReClamp;

  /* FALSE:OFF  TRUE:ON */
  boolean_T DynamicByESP;

  /* FALSE:OFF  TRUE:ON */
  boolean_T DynamicByEPB;

  /* FALSE:OFF  TRUE:ON */
  boolean_T AutoVehicleHold;

  /* FALSE:OFF  TRUE:ON */
  boolean_T ESP_Request;

  /* FALSE:OFF  TRUE:ON */
  boolean_T RollerBench;

  /* FALSE:OFF  TRUE:ON */
  boolean_T Diagnosis;

  /* FALSE:OFF  TRUE:ON */
  boolean_T HallController;
} DegradedModeBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_IgnitionState_
#define DEFINED_TYPEDEF_FOR_IgnitionState_

typedef enum {
  IgnitionState_Unknown = 0,           /* Default value */
  IgnitionState_IgnitionOFF,
  IgnitionState_IgnitionON,
  IgnitionState_EngineON,
  IgnitionState_Fail
} IgnitionState;                       /* 2020.11.13 Seungbum Han
                                          [Daechang]
                                          Unknown / IgnitionOFF / EngineON 만 사용됨
                                          (NAMI Logic Mergy 편의를 위해) */

#endif

#ifndef DEFINED_TYPEDEF_FOR_ApplyReleaseRequest_
#define DEFINED_TYPEDEF_FOR_ApplyReleaseRequest_

typedef enum {
  ApplyReleaseRequest_None = 0,        /* Default value */
  ApplyReleaseRequest_ParkApply,
  ApplyReleaseRequest_EmergencyApply,
  ApplyReleaseRequest_RollerbenchApply,
  ApplyReleaseRequest_Release,
  ApplyReleaseRequest_DynamicApply,
  ApplyReleaseRequest_PadAdjustment
} ApplyReleaseRequest;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MotorCommand_
#define DEFINED_TYPEDEF_FOR_MotorCommand_

typedef enum {
  MotorCommand_None = 0,               /* Default value */
  MotorCommand_Apply,
  MotorCommand_Release,
  MotorCommand_Stop,
  MotorCommand_Freerun
} MotorCommand;                        /* 2020.11.13 Seungbum Han
                                          [Daechang]
                                          Freerun은 사용되지 않음. */

#endif

#ifndef DEFINED_TYPEDEF_FOR_MotorDriverState_
#define DEFINED_TYPEDEF_FOR_MotorDriverState_

typedef enum {
  MotorDriverState_None = 0,           /* Default value */
  MotorDriverState_Apply,
  MotorDriverState_Release,
  MotorDriverState_Stop,
  MotorDriverState_Freerun
} MotorDriverState;                    /* 0 : None
                                          1 : Apply
                                          2 : Release
                                          3 : Stop
                                          4 : Freerun */

#endif

#ifndef DEFINED_TYPEDEF_FOR_ActuatorState_
#define DEFINED_TYPEDEF_FOR_ActuatorState_

typedef enum {
  ActuatorState_ParkApplied = 1,       /* Default value */
  ActuatorState_EmergencyApplied = 2,
  ActuatorState_Released = 3,
  ActuatorState_Applying = 7,
  ActuatorState_Releasing = 8,
  ActuatorState_CompletelyReleased = 9,
  ActuatorState_Unknown = 10
} ActuatorState;                       /* 2020.11.12 Seungbum Han
                                          [Daechang]
                                          - 2, 9번은 사용되지 않음. ( Hold Apply 미사용 ) */

#endif
#endif                                 /* RTW_HEADER_ElectronicParkBrake_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
