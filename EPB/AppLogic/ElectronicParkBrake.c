/*
 * File: ElectronicParkBrake.c
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

#include "ElectronicParkBrake.h"
#include "ElectronicParkBrake_private.h"

/* Exported block signals */
DiagFlagBus DiagInput;                 /* '<Root>/DiagInput' */
SSMInputBus SSMInput;                  /* '<Root>/SSMInput' */
PbcInputBus PBCInput;                  /* '<Root>/PBCInput' */
PbcInDriverBus PbcInDriver;            /* '<Root>/PbcInDriver' */

/* Exported block parameters */
LT_MotClutch LookupTableMotClutch = {
  { 0.0F, 4.0F, 6.0F, 8.0F, 10.0F, 12.0F, 16.0F, 18.0F, 20.0F, 30.0F },

  { 30000.0F, 30020.0F, 30040.0F, 30060.0F, 30080.0F, 30100.0F, 30120.0F,
    30140.0F, 30160.0F, 30180.0F }
} ;                                    /* Variable: LookupTableMotClutch
                                        * Referenced by: '<S23>/MotClutch Threshold 1-D Lookup Table'
                                        */

real_T PbcDataCorrectedLongAccelerationFilterFrequency = 4.0;/* Variable: PbcDataCorrectedLongAccelerationFilterFrequency
                                                              * Referenced by: '<S17>/CorrectedLongAccelerationFilterGain'
                                                              * 1st order low pass filter cut-off frequency for corrected longitudinal acceleration
                                                              */
real_T PbcDataCurrentFilterFrequency = 3.0;/* Variable: PbcDataCurrentFilterFrequency
                                            * Referenced by:
                                            *   '<S7>/CurrentFilterGain'
                                            *   '<S8>/CurrentFilterGain'
                                            * 1st order low pass filter cut-off frequency for motor current
                                            */
real_T PbcDataLongAccelerationFilterFrequency = 4.0;/* Variable: PbcDataLongAccelerationFilterFrequency
                                                     * Referenced by: '<S18>/LongAccelerationFilterGain'
                                                     * 1st order low pass filter cut-off frequency for longitudinal acceleration
                                                     */
real_T PbcDataVehicleAccelerationFilterFrequency = 4.0;/* Variable: PbcDataVehicleAccelerationFilterFrequency
                                                        * Referenced by: '<S20>/VehicleAccelerationFilterGain'
                                                        * 1st order low pass filter cut-off frequency for vehicle acceleration
                                                        */
real_T PbcDataVehicleSpeedFilterFrequency = 4.0;/* Variable: PbcDataVehicleSpeedFilterFrequency
                                                 * Referenced by: '<S21>/VehicleSpeedFilterGain'
                                                 * 1st order low pass filter cut-off frequency for vehicle speed
                                                 */
real32_T C_Light_Apply_Current = 10.0F;/* Variable: C_Light_Apply_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 241 means 13A
                                        */
real32_T C_Max_Apply_Current = 10.0F;  /* Variable: C_Max_Apply_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 315 means 17A

                                          2020.11.13 Seungbum Han
                                          [Daechang]
                                          Firmware 확인 필요
                                        */
real32_T C_Mid_Apply_Current = 12.0F;  /* Variable: C_Mid_Apply_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 278 means 15A
                                        */
real32_T C_Mid_Release_Current = 6.0F; /* Variable: C_Mid_Release_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Current(A) = ADC*0.054
                                          ADC 278 means 15A
                                        */
real32_T C_Apply_OC_TH = 14.0F;        /* Variable: C_OC_TH
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          Calibration Data (Diag.c)에 의해 초기화됨.
                                        */
real32_T C_Release_Current = 6.0F;     /* Variable: C_Release_Current
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [ NAMI , Daechang]
                                          Current(A) = ADC*0.054
                                          ADC 111 means 6A
                                        */
real32_T C_Apply_UC_TH = 0.5F;         /* Variable: C_UC_TH
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          Calibration Data (Diag.c)에 의해 초기화됨.
                                        */
uint16_T C_Accelator_Position_Threshold = 3U;/* Variable: C_Accelator_Position_Threshold
                                              * Referenced by: '<S3>/StandStillManager'
                                              * 2020.11.13 Seungbum Han
                                                !! 확인 후 Update 필요 !!

                                                [DC]
                                                Scaling Factor : 0.25
                                              //Threshold 값은 임시임. ( NAMI 기준 8로 설정)

                                                [ NAMI ]
                                                Scaling Factor : 0.39
                                              */
uint16_T C_Additional_Release_Hall_Count = 50U;/* Variable: C_Additional_Release_Hall_Count
                                                * Referenced by: '<S2>/Pbc'
                                                * 2020.11.13 Seungbum Han
                                                  Additional release Hall Count Default 값, Diag.c 내 Calibration Data로 최신화됨.

                                                */
uint16_T C_Additional_Release_PWM = 100U;/* Variable: C_Additional_Release_PWM
                                          * Referenced by: '<S2>/Pbc'
                                          * If the cable force becomes less than 5kgf during release operation,
                                            additional release will be performed,
                                            and PWM is changed to the C_Additional_Release_PWM at this time.
                                          */
uint16_T C_Apply_Time = 300U;          /* Variable: C_Apply_Time
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          500 means 5000ms(5sec)

                                          2020.11.13 Seungbum Han
                                          [Daechang]
                                          Time Out 발생 Treshold 시간.

                                        */
uint16_T C_AutoSafety_Vehicle_Speed_Threshold = 100U;/* Variable: C_AutoSafety_Vehicle_Speed_Threshold
                                                      * Referenced by: '<S3>/StandStillManager'
                                                      * [ NAMI ]
                                                        Scaling Factor : 0.01

                                                        2020.11.13 Seungbum Han
                                                        [Daechang]
                                                        사용되지않음.
                                                      */
uint16_T C_Brake_Pedal_Pressed_True_Pressure = 30U;/* Variable: C_Brake_Pedal_Pressed_True_Pressure
                                                    * Referenced by: '<S3>/StandStillManager'
                                                    * [ NAMI ]
                                                      Scaling Factor : 0.1

                                                      2020.11.13 Seungbum Han
                                                      [Daechang]
                                                      Scaling Factor : 1
                                                    */
uint16_T C_Degraded_Manual_Release_Switch_Timer = 1000U;/* Variable: C_Degraded_Manual_Release_Switch_Timer
                                                         * Referenced by: '<S3>/StandStillManager'
                                                         * [ NAMI ]
                                                           Scaling Factor : 100
                                                           Default value 1000 means 10 seconds.
                                                           "Manual release" is activated when driver pushes a switch more than X seconds only.


                                                           2020.11.13 Seungbum Han
                                                           [Daechang]
                                                           사용되지않음.

                                                         */
uint16_T C_Dynamic_Braking_EPB_TargetDecel = 80U;/* Variable: C_Dynamic_Braking_EPB_TargetDecel
                                                  * Referenced by: '<S3>/StandStillManager'
                                                  * 2021.03.12
                                                    [DBSi]
                                                    Scaling Factor : 0.05
                                                    Offset : X
                                                    Default value 80 means -4m/s^2(-0.4g).


                                                  */
uint16_T C_Dynamic_Braking_Vehicle_Speed_Threshold = 100U;/* Variable: C_Dynamic_Braking_Vehicle_Speed_Threshold
                                                           * Referenced by: '<S3>/StandStillManager'
                                                           *
                                                             2020.11.13 Seungbum Han
                                                             [Daechang]
                                                             Scaling Factor : 1 --> 30km




                                                             [ NAMI ]
                                                             Scaling Factor :  0.01


                                                           */
uint16_T C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold = 6U;/* Variable: C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold
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
uint16_T C_Light_Apply_Force = 160U;   /* Variable: C_Light_Apply_Force
                                        * Referenced by:
                                        *   '<S2>/Pbc'
                                        *   '<S3>/CAN_Tx'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지않음.
                                        */
uint16_T C_Light_Apply_Grade_Threshold = 600U;/* Variable: C_Light_Apply_Grade_Threshold
                                               * Referenced by: '<S2>/Pbc'
                                               * [ NAMI ]
                                                 Scaling Factor :  0.01

                                                 2020.11.13 Seungbum Han
                                                 [Daechang]
                                                 사용되지않음.
                                               */
uint16_T C_Max_Apply_Force = 230U;     /* Variable: C_Max_Apply_Force
                                        * Referenced by:
                                        *   '<S2>/Pbc'
                                        *   '<S3>/CAN_Tx'
                                        *   '<S3>/StandStillManager'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지 않음, C_Apply_Force4로 적용됨.
                                        */
uint16_T C_Max_Apply_Grade_Threshold = 1200U;/* Variable: C_Max_Apply_Grade_Threshold
                                              * Referenced by: '<S2>/Pbc'
                                              * [ NAMI ]
                                                Scaling Factor :  0.01

                                                2020.11.13 Seungbum Han
                                                [Daechang]
                                                사용되지 않음,
                                              */
uint16_T C_Max_Cable_Force_ADC = 700U; /* Variable: C_Max_Cable_Force_ADC
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Default value of maximum cable force of 230kgf (10bits ADC).

                                          2020.11.13 Seungbum Han
                                          [Daechang]
                                          Calibration Data (Diag.c)에 의해 초기화됨.

                                        */
uint16_T C_Mid_Apply_Force = 190U;     /* Variable: C_Mid_Apply_Force
                                        * Referenced by:
                                        *   '<S2>/Pbc'
                                        *   '<S3>/CAN_Tx'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지 않음,
                                        */
uint16_T C_Release_Cable_Force_ADC = 200U;/* Variable: C_Release_Cable_Force_ADC
                                           * Referenced by: '<S2>/Pbc'
                                           * [ NAMI ]
                                             Default value of release cable force of 5kgf (10bits ADC).

                                             2020.11.13 Seungbum Han
                                             [Daechang]
                                           */
uint16_T C_Release_Force = 5U;         /* Variable: C_Release_Force
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [ NAMI , Daechang]
                                        */
uint16_T C_Release_Time = 110U;        /* Variable: C_Release_Time
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [ NAMI , Daechang]
                                          300 means 3000ms(3sec)
                                        */
uint16_T C_Rollerbench_Force_Level1 = 140U;/* Variable: C_Rollerbench_Force_Level1
                                            * Referenced by: '<S3>/StandStillManager'
                                            * [ NAMI ]

                                              2020.11.13 Seungbum Han
                                              [Daechang]
                                              사용되지 않음.
                                            */
uint16_T C_Rollerbench_Force_Level2 = 170U;/* Variable: C_Rollerbench_Force_Level2
                                            * Referenced by: '<S3>/StandStillManager'
                                            * [ NAMI ]

                                              2020.11.13 Seungbum Han
                                              [Daechang]
                                              사용되지 않음.
                                            */
uint16_T C_Rollerbench_Force_Level3 = 200U;/* Variable: C_Rollerbench_Force_Level3
                                            * Referenced by: '<S3>/StandStillManager'
                                            * [ NAMI ]

                                              2020.11.13 Seungbum Han
                                              [Daechang]
                                              사용되지 않음.
                                            */
uint16_T C_Rollerbench_Max_Process_Time = 600U;/* Variable: C_Rollerbench_Max_Process_Time
                                                * Referenced by: '<S3>/StandStillManager'
                                                * [ NAMI ]

                                                  2020.11.13 Seungbum Han
                                                  [Daechang]
                                                  사용되지 않음.
                                                */
uint16_T PbcDataMaxForce = 120U;       /* Variable: PbcDataMaxForce
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI ]
                                          Default value of maximum force of 120kgf.
                                        */
uint16_T PbcDataReleaseForce = 5U;     /* Variable: PbcDataReleaseForce
                                        * Referenced by: '<S2>/Pbc'
                                        * [ NAMI , Daechang ]
                                          Default value of release force of 5kgf.
                                        */
uint16_T PbcInDataStorageEpbStatusLeft = 3U;/* Variable: PbcInDataStorageEpbStatusLeft
                                             * Referenced by: '<S2>/Pbc'
                                             * 0 : Unknown
                                               1 : Applied
                                               2 : Reserved
                                               3 : Released
                                             */
uint16_T PbcInDataStorageEpbStatusRight = 3U;/* Variable: PbcInDataStorageEpbStatusRight
                                              * Referenced by: '<S2>/Pbc'
                                              * 0 : Unknown
                                                1 : Applied
                                                2 : Reserved
                                                3 : Released
                                              */
boolean_T C_Apply_Force2 = 0;          /* Variable: C_OC_Left
                                        * Referenced by: '<S2>/Pbc'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지않음.
                                        */
boolean_T F_EmergencyApply = 0;        /* Variable: F_EmergencyApply
                                        * Referenced by: '<S3>/StandStillManager'
                                        * 2020.11.13 Seungbum Han
                                          [Daechang]
                                          사용되지않음.
                                        */

/* Block signals and states (default storage) */
DW_ElectronicParkBrake_T ElectronicParkBrake_DW;

/* External outputs (root outports fed by signals with default storage) */
ExtY_ElectronicParkBrake_T ElectronicParkBrake_Y;

/* Model step function */
void ElectronicParkBrake_step(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/ElectronicParkBrake' */
  /* UnitDelay: '<S1>/PreviousPBCtoSSM' */
  ElectronicParkBrake_DW.PreviousPBCtoSSM =
    ElectronicParkBrake_DW.PreviousPBCtoSSM_DSTATE;

  /* Outputs for Atomic SubSystem: '<S1>/StandStillManager'
   *
   * Block description for '<S1>/StandStillManager':
   *  2020.11.13 Seungbum Han
   *  - 기능의 동작 가/불 여부 (Degrade Mode)
   *  - 기능 조건판단(StandStillmanager)
   *  - 동작 상태에 따른 Indication out
   */
  ElectronicPar_StandStillManager();

  /* End of Outputs for SubSystem: '<S1>/StandStillManager' */

  /* Outputs for Atomic SubSystem: '<S1>/ParkingBrakeControl'
   *
   * Block description for '<S1>/ParkingBrakeControl':
   *  2020.11.13 Seungbum Han
   *  - SSM으로부터 발생된 Request에 따른 동작 수행
   *  - 동작 중 Sensor 및 Motorstate, EPB State 감시

   */
  ElectronicP_ParkingBrakeControl();

  /* End of Outputs for SubSystem: '<S1>/ParkingBrakeControl' */

  /* Update for UnitDelay: '<S1>/PreviousPBCtoSSM' */
  ElectronicParkBrake_DW.PreviousPBCtoSSM_DSTATE =
    ElectronicParkBrake_DW.PBCtoSSM;

  /* End of Outputs for SubSystem: '<Root>/ElectronicParkBrake' */
}

/* Model initialize function */
void ElectronicParkBrake_initialize(void)
{
  /* SystemInitialize for Atomic SubSystem: '<Root>/ElectronicParkBrake' */

  /* SystemInitialize for Atomic SubSystem: '<S1>/StandStillManager'
   *
   * Block description for '<S1>/StandStillManager':
   *  2020.11.13 Seungbum Han
   *  - 기능의 동작 가/불 여부 (Degrade Mode)
   *  - 기능 조건판단(StandStillmanager)
   *  - 동작 상태에 따른 Indication out
   */
  Electron_StandStillManager_Init();

  /* End of SystemInitialize for SubSystem: '<S1>/StandStillManager' */

  /* SystemInitialize for Atomic SubSystem: '<S1>/ParkingBrakeControl'
   *
   * Block description for '<S1>/ParkingBrakeControl':
   *  2020.11.13 Seungbum Han
   *  - SSM으로부터 발생된 Request에 따른 동작 수행
   *  - 동작 중 Sensor 및 Motorstate, EPB State 감시

   */
  Electr_ParkingBrakeControl_Init();

  /* End of SystemInitialize for SubSystem: '<S1>/ParkingBrakeControl' */

  /* End of SystemInitialize for SubSystem: '<Root>/ElectronicParkBrake' */
}

/* Model terminate function */
void ElectronicParkBrake_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
