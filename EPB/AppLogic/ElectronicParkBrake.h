/*
 * File: ElectronicParkBrake.h
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

#ifndef RTW_HEADER_ElectronicParkBrake_h_
#define RTW_HEADER_ElectronicParkBrake_h_
#ifndef ElectronicParkBrake_COMMON_INCLUDES_
# define ElectronicParkBrake_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* ElectronicParkBrake_COMMON_INCLUDES_ */

#include "ElectronicParkBrake_types.h"

/* Child system includes */
#include "ParkingBrakeControl.h"
#include "StandStillManager.h"

/* Macros for accessing real-time model data structure */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  SSMtoPBCBus SSMtoPBC;                /* '<S3>/Bus Creator' */
  PbcInDriverBus PbcInDriverFiltered;  /* '<S5>/Bus Creator' */
  PBCtoSSMBus PreviousPBCtoSSM;        /* '<S1>/PreviousPBCtoSSM' */
  PBCtoSSMBus PBCtoSSM;                /* '<S2>/Bus Creator3' */
  PBCtoSSMBus PreviousPBCtoSSM_DSTATE; /* '<S1>/PreviousPBCtoSSM' */
  real_T PreviousMotorCurrentFiltered_DS;/* '<S7>/PreviousMotorCurrentFiltered' */
  real_T PreviousMotorCurrentFiltered__c;/* '<S8>/PreviousMotorCurrentFiltered' */
  real_T PreviousCorrectedLongAccelerati;/* '<S17>/PreviousCorrectedLongAccelerationFiltered' */
  real_T PreviousLongAccelerationFiltere;/* '<S18>/PreviousLongAccelerationFiltered' */
  real_T PreviousVehicleAccelerationFilt;/* '<S20>/PreviousVehicleAccelerationFiltered' */
  real_T VehicleSpeedFiltered10_DSTATE;/* '<S19>/VehicleSpeedFiltered10' */
  real_T PreviousVehicleSpeedFiltered_DS;/* '<S21>/PreviousVehicleSpeedFiltered' */
  real_T VehicleSpeedFiltered9_DSTATE; /* '<S19>/VehicleSpeedFiltered9' */
  real_T VehicleSpeedFiltered8_DSTATE; /* '<S19>/VehicleSpeedFiltered8' */
  real_T VehicleSpeedFiltered7_DSTATE; /* '<S19>/VehicleSpeedFiltered7' */
  real_T VehicleSpeedFiltered6_DSTATE; /* '<S19>/VehicleSpeedFiltered6' */
  real_T VehicleSpeedFiltered5_DSTATE; /* '<S19>/VehicleSpeedFiltered5' */
  real_T VehicleSpeedFiltered4_DSTATE; /* '<S19>/VehicleSpeedFiltered4' */
  real_T VehicleSpeedFiltered3_DSTATE; /* '<S19>/VehicleSpeedFiltered3' */
  real_T VehicleSpeedFiltered2_DSTATE; /* '<S19>/VehicleSpeedFiltered2' */
  real_T VehicleSpeedFiltered1_DSTATE; /* '<S19>/VehicleSpeedFiltered1' */
  real32_T PbcApplyCurrentTemp;        /* '<S2>/Pbc' */
  real32_T PbcApplyCurrentTempLeft;    /* '<S2>/Pbc' */
  real32_T PbcApplyCurrentTempRight;   /* '<S2>/Pbc' */
  int16_T PbcDataHallMemory;           /* '<S2>/Pbc' */
  uint16_T PbcInApplyReleaseRequest;   /* '<S3>/StandStillManager' */
  uint16_T PbcInRollerbenchTargetForce;/* '<S3>/StandStillManager' */
  uint16_T SSMDataDynamicApply;        /* '<S3>/StandStillManager' */
  uint16_T SSMOutRollerbenchRoutineID; /* '<S3>/StandStillManager' */
  uint16_T PbcInApplyReleaseRequestLeft;/* '<S3>/StandStillManager' */
  uint16_T PbcInApplyReleaseRequestRight;/* '<S3>/StandStillManager' */
  uint16_T PbcOutMotorCommandLeft;     /* '<S2>/Pbc' */
  uint16_T PbcOutMotorCommandRight;    /* '<S2>/Pbc' */
  uint16_T PbcOutMotorDutyLeft;        /* '<S2>/Pbc' */
  uint16_T PbcOutMotorDutyRight;       /* '<S2>/Pbc' */
  uint16_T PbcOutActuatorStateLeft;    /* '<S2>/Pbc' */
  uint16_T PbcOutActuatorStateRight;   /* '<S2>/Pbc' */
  uint16_T PbcPreviousAppliedForce;    /* '<S2>/Pbc' */
  uint16_T PbcSensorStatusLeft;        /* '<S2>/Pbc' */
  uint16_T PbcSensorStatusRight;       /* '<S2>/Pbc' */
  uint16_T PbcApplyingTypeLeft;        /* '<S2>/Pbc' */
  uint16_T PbcApplyingTypeRight;       /* '<S2>/Pbc' */
  uint16_T SSMDataAVH;                 /* '<S3>/StandStillManager' */
  uint16_T SSMDataZeroVehSpeedTimer;   /* '<S3>/StandStillManager' */
  uint16_T SSMDataRollerbenchProcessTimer;/* '<S3>/StandStillManager' */
  uint16_T SSMDataDiagnosis;           /* '<S3>/StandStillManager' */
  uint16_T SSMDataRollerbench;         /* '<S3>/StandStillManager' */
  uint16_T SSMDataAfterRunTimer;       /* '<S3>/StandStillManager' */
  uint16_T SSMDataAutoAVH;             /* '<S3>/StandStillManager' */
  uint16_T PbcReleasingCounter;        /* '<S2>/Pbc' */
  uint16_T PbcApplyForce;              /* '<S2>/Pbc' */
  uint16_T PbcApplyForceTemp;          /* '<S2>/Pbc' */
  uint16_T PbcApplyCableForce;         /* '<S2>/Pbc' */
  uint16_T PbcReleaseTime;             /* '<S2>/Pbc' */
  uint16_T PbcApplyTime;               /* '<S2>/Pbc' */
  uint16_T PbcReleaseCableForce;       /* '<S2>/Pbc' */
  uint16_T PbcDataReleaseOC_Counter;   /* '<S2>/Pbc' */
  uint16_T PbcDataInrush_Counter_Right_OC;/* '<S2>/Pbc' */
  uint16_T PbcDataReleaseOC_Counter_Left;/* '<S2>/Pbc' */
  uint16_T PbcDataInrush_Counter_Left_OC;/* '<S2>/Pbc' */
  uint16_T PbcReleaseTimeLeft;         /* '<S2>/Pbc' */
  uint16_T PbcApplyTimeLeft;           /* '<S2>/Pbc' */
  uint16_T PbcApplyTimeTempLeft;       /* '<S2>/Pbc' */
  uint16_T PbcApplyTimeTempRight;      /* '<S2>/Pbc' */
  uint16_T PbcApplyTimeRight;          /* '<S2>/Pbc' */
  uint16_T PbcDataReleaseNormal2UC_Counter;/* '<S2>/Pbc' */
  uint16_T PbcDataReleaseOC_Counter_Right;/* '<S2>/Pbc' */
  uint16_T PbcReleaseTimeRight;        /* '<S2>/Pbc' */
  uint16_T PbcDataDynamicRequestLeft;  /* '<S2>/Pbc' */
  uint16_T PbcDataDynamicRequestRight; /* '<S2>/Pbc' */
  uint16_T PbcApplyCountRight;         /* '<S2>/Pbc' */
  uint16_T PbcApplyCountLeft;          /* '<S2>/Pbc' */
  uint16_T PbcDataReleaseNormal2UC_Count_m;/* '<S2>/Pbc' */
  uint16_T PbcDataReleaseUC2Normal_Counter;/* '<S2>/Pbc' */
  uint16_T PbcDataReleaseUC2Normal_Count_j;/* '<S2>/Pbc' */
  uint16_T PbcDataApplyOC2Normal_Counter_L;/* '<S2>/Pbc' */
  uint16_T PbcData_UC_Counter_Left;    /* '<S2>/Pbc' */
  uint16_T PbcDataApplyOC2Normal_Counter_R;/* '<S2>/Pbc' */
  uint16_T OCMarurationTimeDuration;   /* '<S2>/Pbc' */
  uint16_T UCMarurationTimeDuration;   /* '<S2>/Pbc' */
  uint16_T PbcDataInrush_Counter_Left_UC;/* '<S2>/Pbc' */
  uint16_T PbcDataInrush_Counter_Right_UC;/* '<S2>/Pbc' */
  uint16_T PbcData_OC_Counter_Left;    /* '<S2>/Pbc' */
  uint16_T PbcDataApplyUC2Normal_Counter_L;/* '<S2>/Pbc' */
  uint16_T OCDemarurationTimeDuration; /* '<S2>/Pbc' */
  uint16_T PbcDataApplyOC_Counter_Left;/* '<S2>/Pbc' */
  uint16_T PbcOutDataLatchdownTimer;   /* '<S2>/Pbc' */
  uint8_T SSMOutRollerbenchCondition;  /* '<S3>/StandStillManager' */
  uint8_T SSMOutRollerbenchRoutineResults;/* '<S3>/StandStillManager' */
  uint8_T SSMOutRollerbenchPhase;      /* '<S3>/StandStillManager' */
  uint8_T SSMOutRollerbenchActive;     /* '<S3>/StandStillManager' */
  uint8_T is_active_c8_ElectronicParkBrak;/* '<S3>/IgnitionStateChart' */
  uint8_T is_IgnitionState;            /* '<S3>/IgnitionStateChart' */
  uint8_T is_active_c4_ElectronicParkBrak;/* '<S3>/CAN_Tx' */
  uint8_T is_active_c13_ElectronicParkBra;/* '<S6>/PbcInWheelDirectionInfo' */
  uint8_T is_c13_ElectronicParkBrake;  /* '<S6>/PbcInWheelDirectionInfo' */
  uint8_T is_active_c3_ElectronicParkBrak;/* '<S2>/Pbc' */
  uint8_T is_EcuPowerLatchRequest;     /* '<S2>/Pbc' */
  uint8_T PbcDataHallSensor_Counter;   /* '<S2>/Pbc' */
  uint8_T PbcDataHallSensor_Swap;      /* '<S2>/Pbc' */
  boolean_T SSMOutTZeroVehSpeedDetectFlag;/* '<S3>/StandStillManager' */
  boolean_T SSMDataServiceRelease;     /* '<S3>/StandStillManager' */
  boolean_T SSMDataDegradedIntelligent;/* '<S24>/DegradeModeManagerChart' */
  boolean_T PbcOutEcuPowerLatchRequest;/* '<S2>/Pbc' */
  boolean_T SSMDataBrakePedalPressed;  /* '<S3>/StandStillManager' */
  boolean_T SSMDataAccelatorPressed;   /* '<S3>/StandStillManager' */
  boolean_T SSMDataAutomaticApply;     /* '<S3>/StandStillManager' */
  boolean_T SSMDataDriveAway;          /* '<S3>/StandStillManager' */
  boolean_T SSMDataIsAutoPark;         /* '<S3>/StandStillManager' */
  boolean_T SSMDataIsAutoSafetyApply;  /* '<S3>/StandStillManager' */
  boolean_T SSMDataIsAutomaticApply;   /* '<S3>/StandStillManager' */
  boolean_T SSMDataIsAutomaticApplyDisable;/* '<S3>/StandStillManager' */
  boolean_T SSMDataIsAutoApplyDisable; /* '<S3>/StandStillManager' */
  boolean_T SSMDataReClamp;            /* '<S3>/StandStillManager' */
  boolean_T SSMDataSwitchFailureTrigger;/* '<S3>/StandStillManager' */
  boolean_T SSMDataSwitchFailureIndTrigger2;/* '<S3>/CAN_Tx' */
  boolean_T SSMDataSwitchFailureIndTrigger1;/* '<S3>/CAN_Tx' */
  boolean_T PbcReclampEnable;          /* '<S2>/Pbc' */
  boolean_T PbcDataRelease;            /* '<S2>/Pbc' */
  boolean_T PbcDataApply;              /* '<S2>/Pbc' */
  boolean_T PbcDataReleaseOC;          /* '<S2>/Pbc' */
  boolean_T PbcDataHallSensorInit;     /* '<S2>/Pbc' */
  boolean_T PbcDataHallSensor_Reset;   /* '<S2>/Pbc' */
  boolean_T PbcDataReleaseLeft;        /* '<S2>/Pbc' */
  boolean_T PbcDataReleaseUC_Left;     /* '<S2>/Pbc' */
  boolean_T PbcDataApplyLeft;          /* '<S2>/Pbc' */
  boolean_T PbcDataApplyRight;         /* '<S2>/Pbc' */
  boolean_T PbcDataReleaseUC_Right;    /* '<S2>/Pbc' */
  boolean_T PbcDataReleaseRight;       /* '<S2>/Pbc' */
  boolean_T PbcReclampEnableLeft;      /* '<S2>/Pbc' */
  boolean_T PbcReclampEnableRight;     /* '<S2>/Pbc' */
  boolean_T PbcDataApplyOC_Left;       /* '<S2>/Pbc' */
  boolean_T PbcDataApplyOC_Right;      /* '<S2>/Pbc' */
  boolean_T PbcDataApplyUC_Left;       /* '<S2>/Pbc' */
  boolean_T PbcDataApplyUC_Right;      /* '<S2>/Pbc' */
} DW_ElectronicParkBrake_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  SSMOutputCANBus SSMOutputCAN;        /* '<Root>/SSMOutputCAN' */
  SSMOutputBus SSMOutput;              /* '<Root>/SSMOutput' */
  PbcOutputBus PbcOutput;              /* '<Root>/PbcOutput' */
} ExtY_ElectronicParkBrake_T;

/* Block signals and states (default storage) */
extern DW_ElectronicParkBrake_T ElectronicParkBrake_DW;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_ElectronicParkBrake_T ElectronicParkBrake_Y;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern DiagFlagBus DiagInput;          /* '<Root>/DiagInput' */
extern SSMInputBus SSMInput;           /* '<Root>/SSMInput' */
extern PbcInputBus PBCInput;           /* '<Root>/PBCInput' */
extern PbcInDriverBus PbcInDriver;     /* '<Root>/PbcInDriver' */

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern LT_MotClutch LookupTableMotClutch;/* Variable: LookupTableMotClutch
                                          * Referenced by: '<S23>/MotClutch Threshold 1-D Lookup Table'
                                          */
extern real_T PbcDataCorrectedLongAccelerationFilterFrequency;/* Variable: PbcDataCorrectedLongAccelerationFilterFrequency
                                                               * Referenced by: '<S17>/CorrectedLongAccelerationFilterGain'
                                                               * 1st order low pass filter cut-off frequency for corrected longitudinal acceleration
                                                               */
extern real_T PbcDataCurrentFilterFrequency;/* Variable: PbcDataCurrentFilterFrequency
                                             * Referenced by:
                                             *   '<S7>/CurrentFilterGain'
                                             *   '<S8>/CurrentFilterGain'
                                             * 1st order low pass filter cut-off frequency for motor current
                                             */
extern real_T PbcDataLongAccelerationFilterFrequency;/* Variable: PbcDataLongAccelerationFilterFrequency
                                                      * Referenced by: '<S18>/LongAccelerationFilterGain'
                                                      * 1st order low pass filter cut-off frequency for longitudinal acceleration
                                                      */
extern real_T PbcDataVehicleAccelerationFilterFrequency;/* Variable: PbcDataVehicleAccelerationFilterFrequency
                                                         * Referenced by: '<S20>/VehicleAccelerationFilterGain'
                                                         * 1st order low pass filter cut-off frequency for vehicle acceleration
                                                         */
extern real_T PbcDataVehicleSpeedFilterFrequency;/* Variable: PbcDataVehicleSpeedFilterFrequency
                                                  * Referenced by: '<S21>/VehicleSpeedFilterGain'
                                                  * 1st order low pass filter cut-off frequency for vehicle speed
                                                  */
extern real32_T C_Light_Apply_Current; /* Variable: C_Light_Apply_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 241 means 13A
                                        */
extern real32_T C_Max_Apply_Current;   /* Variable: C_Max_Apply_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 315 means 17A

                                          2020.11.13 Seungbum Han
                                          [Daechang]
                                          Firmware 확인 필요
                                        */
extern real32_T C_Mid_Apply_Current;   /* Variable: C_Mid_Apply_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 278 means 15A
                                        */
extern real32_T C_Mid_Release_Current; /* Variable: C_Mid_Release_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 278 means 15A
                                        */
extern real32_T C_Apply_OC_TH;         /* Variable: C_OC_TH
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          Calibration Data (Diag.c)에 의해 초기화됨.
                                        */
extern real32_T C_Release_Current;     /* Variable: C_Release_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [ NAMI , Daechang]
                                          Current(A) = ADC*0.054
                                          ADC 111 means 6A
                                        */
extern real32_T C_Apply_UC_TH;         /* Variable: C_UC_TH
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          Calibration Data (Diag.c)에 의해 초기화됨.
                                        */
extern uint16_T C_Accelator_Position_Threshold;/* Variable: C_Accelator_Position_Threshold
                                                * Referenced by: '<S3>/StandStillManager'
                                                * 2020.11.13 Seungbum Han
                                                  !! 확인 후 Update 필요 !!

                                                  [DC]
                                                  Scaling Factor : 0.25
                                                //Threshold 값은 임시임. ( NAMI 기준 8로 설정)

                                                  [ NAMI ]
                                                  Scaling Factor : 0.39
                                                */
extern uint16_T C_Additional_Release_Hall_Count;/* Variable: C_Additional_Release_Hall_Count
                                                 * Referenced by: '<S2>/Pbc'
                                                 * 2020.11.13 Seungbum Han
                                                   Additional release Hall Count Default 값, Diag.c 내 Calibration Data로 최신화됨.

                                                 */
extern uint16_T C_Additional_Release_PWM;/* Variable: C_Additional_Release_PWM
                                          * Referenced by: '<S2>/Pbc'
                                          * If the cable force becomes less than 5kgf during release operation,
                                            additional release will be performed,
                                            and PWM is changed to the C_Additional_Release_PWM at this time.
                                          */
extern uint16_T C_Apply_Time;          /* Variable: C_Apply_Time
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          500 means 5000ms(5sec)

                                          2020.11.13 Seungbum Han
                                          [Daechang]
                                          Time Out 발생 Treshold 시간.

                                        */
extern uint16_T C_AutoSafety_Vehicle_Speed_Threshold;/* Variable: C_AutoSafety_Vehicle_Speed_Threshold
                                                      * Referenced by: '<S3>/StandStillManager'
                                                      * [ NAMI ]
                                                        Scaling Factor : 0.01

                                                        2020.11.13 Seungbum Han
                                                        [Daechang]
                                                        사용되지않음.
                                                      */
extern uint16_T C_Brake_Pedal_Pressed_True_Pressure;/* Variable: C_Brake_Pedal_Pressed_True_Pressure
                                                     * Referenced by: '<S3>/StandStillManager'
                                                     * [ NAMI ]
                                                       Scaling Factor : 0.1

                                                       2020.11.13 Seungbum Han
                                                       [Daechang]
                                                       Scaling Factor : 1
                                                     */
extern uint16_T C_Degraded_Manual_Release_Switch_Timer;/* Variable: C_Degraded_Manual_Release_Switch_Timer
                                                        * Referenced by: '<S3>/StandStillManager'
                                                        * [ NAMI ]
                                                          Scaling Factor : 100
                                                          Default value 1000 means 10 seconds.
                                                          "Manual release" is activated when driver pushes a switch more than X seconds only.


                                                          2020.11.13 Seungbum Han
                                                          [Daechang]
                                                          사용되지않음.

                                                        */
extern uint16_T C_Dynamic_Braking_EPB_TargetDecel;/* Variable: C_Dynamic_Braking_EPB_TargetDecel
                                                   * Referenced by: '<S3>/StandStillManager'
                                                   * 2021.03.12
                                                     [DBSi]
                                                     Scaling Factor : 0.05
                                                     Offset : X
                                                     Default value 80 means -4m/s^2(-0.4g).


                                                   */
extern uint16_T C_Dynamic_Braking_Vehicle_Speed_Threshold;/* Variable: C_Dynamic_Braking_Vehicle_Speed_Threshold
                                                           * Referenced by: '<S3>/StandStillManager'
                                                           *
                                                             2020.11.13 Seungbum Han
                                                             [Daechang]
                                                             Scaling Factor : 1 --> 30km




                                                             [ NAMI ]
                                                             Scaling Factor :  0.01


                                                           */
extern uint16_T C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold;/* Variable: C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold
                                                                     * Referenced by:
                                                                     *   '<S3>/CAN_Tx'
                                                                     *   '<S3>/StandStillManager'
                                                                     *
                                                                      2020.11.13 Seungbum Han
                                                                      [Daechang]
                                                                      Scaling Factor : 1 --> 3km




                                                                      [ NAMI ]
                                                                      Scaling Factor : 0.01
                                                                     */
extern uint16_T C_Light_Apply_Force;   /* Variable: C_Light_Apply_Force
                                        * Referenced by:
                                        *   '<S2>/Pbc'
                                        *   '<S3>/CAN_Tx'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지않음.
                                        */
extern uint16_T C_Light_Apply_Grade_Threshold;/* Variable: C_Light_Apply_Grade_Threshold
                                               * Referenced by: '<S2>/Pbc'
                                               * [ NAMI ]
                                                 Scaling Factor :  0.01

                                                 2020.11.13 Seungbum Han
                                                 [Daechang]
                                                 사용되지않음.
                                               */
extern uint16_T C_Max_Apply_Force;     /* Variable: C_Max_Apply_Force
                                        * Referenced by:
                                        *   '<S2>/Pbc'
                                        *   '<S3>/CAN_Tx'
                                        *   '<S3>/StandStillManager'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지 않음, C_Apply_Force4로 적용됨.
                                        */
extern uint16_T C_Max_Apply_Grade_Threshold;/* Variable: C_Max_Apply_Grade_Threshold
                                             * Referenced by: '<S2>/Pbc'
                                             * [ NAMI ]
                                               Scaling Factor :  0.01

                                               2020.11.13 Seungbum Han
                                               [Daechang]
                                               사용되지 않음,
                                             */
extern uint16_T C_Max_Cable_Force_ADC; /* Variable: C_Max_Cable_Force_ADC
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Default value of maximum cable force of 230kgf (10bits ADC).

                                          2020.11.13 Seungbum Han
                                          [Daechang]
                                          Calibration Data (Diag.c)에 의해 초기화됨.

                                        */
extern uint16_T C_Mid_Apply_Force;     /* Variable: C_Mid_Apply_Force
                                        * Referenced by:
                                        *   '<S2>/Pbc'
                                        *   '<S3>/CAN_Tx'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지 않음,
                                        */
extern uint16_T C_Release_Cable_Force_ADC;/* Variable: C_Release_Cable_Force_ADC
                                           * Referenced by: '<S2>/Pbc'
                                           * [ NAMI ]
                                             Default value of release cable force of 5kgf (10bits ADC).

                                             2020.11.13 Seungbum Han
                                             [Daechang]
                                           */
extern uint16_T C_Release_Force;       /* Variable: C_Release_Force
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [ NAMI , Daechang]
                                        */
extern uint16_T C_Release_Time;        /* Variable: C_Release_Time
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [ NAMI , Daechang]
                                          300 means 3000ms(3sec)
                                        */
extern uint16_T C_Rollerbench_Force_Level1;/* Variable: C_Rollerbench_Force_Level1
                                            * Referenced by: '<S3>/StandStillManager'
                                            * [ NAMI ]

                                              2020.11.13 Seungbum Han
                                              [Daechang]
                                              사용되지 않음.
                                            */
extern uint16_T C_Rollerbench_Force_Level2;/* Variable: C_Rollerbench_Force_Level2
                                            * Referenced by: '<S3>/StandStillManager'
                                            * [ NAMI ]

                                              2020.11.13 Seungbum Han
                                              [Daechang]
                                              사용되지 않음.
                                            */
extern uint16_T C_Rollerbench_Force_Level3;/* Variable: C_Rollerbench_Force_Level3
                                            * Referenced by: '<S3>/StandStillManager'
                                            * [ NAMI ]

                                              2020.11.13 Seungbum Han
                                              [Daechang]
                                              사용되지 않음.
                                            */
extern uint16_T C_Rollerbench_Max_Process_Time;/* Variable: C_Rollerbench_Max_Process_Time
                                                * Referenced by: '<S3>/StandStillManager'
                                                * [ NAMI ]

                                                  2020.11.13 Seungbum Han
                                                  [Daechang]
                                                  사용되지 않음.
                                                */
extern uint16_T PbcDataMaxForce;       /* Variable: PbcDataMaxForce
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Default value of maximum force of 120kgf.
                                        */
extern uint16_T PbcDataReleaseForce;   /* Variable: PbcDataReleaseForce
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI , Daechang ]
                                          Default value of release force of 5kgf.
                                        */
extern uint16_T PbcInDataStorageEpbStatusLeft;/* Variable: PbcInDataStorageEpbStatusLeft
                                               * Referenced by: '<S2>/Pbc'
                                               * 0 : Unknown
                                                 1 : Applied
                                                 2 : Reserved
                                                 3 : Released
                                               */
extern uint16_T PbcInDataStorageEpbStatusRight;/* Variable: PbcInDataStorageEpbStatusRight
                                                * Referenced by: '<S2>/Pbc'
                                                * 0 : Unknown
                                                  1 : Applied
                                                  2 : Reserved
                                                  3 : Released
                                                */
extern boolean_T C_Apply_Force2;       /* Variable: C_OC_Left
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지않음.
                                        */
extern boolean_T F_EmergencyApply;     /* Variable: F_EmergencyApply
                                        * Referenced by: '<S3>/StandStillManager'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지않음.
                                        */

/* Model entry point functions */
extern void ElectronicParkBrake_initialize(void);
extern void ElectronicParkBrake_step(void);
extern void ElectronicParkBrake_terminate(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S7>/MotorCurrentCompare' : Unused code path elimination
 * Block '<S8>/MotorCurrentCompare' : Unused code path elimination
 * Block '<S19>/Scope' : Unused code path elimination
 * Block '<S13>/Display' : Unused code path elimination
 * Block '<S13>/Display1' : Unused code path elimination
 * Block '<S13>/RoadGradeCompare' : Unused code path elimination
 * Block '<S23>/Display' : Unused code path elimination
 * Block '<S23>/Display1' : Unused code path elimination
 * Block '<S23>/Display2' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('DBSi_Proto9/ElectronicParkBrake')    - opens subsystem DBSi_Proto9/ElectronicParkBrake
 * hilite_system('DBSi_Proto9/ElectronicParkBrake/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'DBSi_Proto9'
 * '<S1>'   : 'DBSi_Proto9/ElectronicParkBrake'
 * '<S2>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl'
 * '<S3>'   : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager'
 * '<S4>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/Pbc'
 * '<S5>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInDriverSignalProcessing'
 * '<S6>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing'
 * '<S7>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInDriverSignalProcessing/MotorCurrentLeftFilter'
 * '<S8>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInDriverSignalProcessing/MotorCurrentRightFilter'
 * '<S9>'   : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInDriverSignalProcessing/MotorDriverSupplyFilter'
 * '<S10>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInDriverSignalProcessing/MotorVoltageLeftFilter'
 * '<S11>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInDriverSignalProcessing/MotorVoltageRightFilter'
 * '<S12>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration'
 * '<S13>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/DetermineRoadGrade'
 * '<S14>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/PbcInWheelDirectionInfo'
 * '<S15>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CalculateLongAcceleration'
 * '<S16>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CalculateVehicleAcceleration'
 * '<S17>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CorrectedLongAccelerationFilter'
 * '<S18>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CalculateLongAcceleration/VehicleAccelerationFilter'
 * '<S19>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CalculateVehicleAcceleration/DifferentiateVehicleSpeed'
 * '<S20>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CalculateVehicleAcceleration/VehicleAccelerationFilter'
 * '<S21>'  : 'DBSi_Proto9/ElectronicParkBrake/ParkingBrakeControl/PbcInSignalProcessing/CorrectLongAcceleration/CalculateVehicleAcceleration/VehicleSpeedFilter'
 * '<S22>'  : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager/CAN_Tx'
 * '<S23>'  : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager/CalMotClutchThreshold'
 * '<S24>'  : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager/DegradeModeManager'
 * '<S25>'  : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager/IgnitionStateChart'
 * '<S26>'  : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager/StandStillManager'
 * '<S27>'  : 'DBSi_Proto9/ElectronicParkBrake/StandStillManager/DegradeModeManager/DegradeModeManagerChart'
 */

/*-
 * Requirements for '<Root>': ElectronicParkBrake
 */
#endif                                 /* RTW_HEADER_ElectronicParkBrake_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
