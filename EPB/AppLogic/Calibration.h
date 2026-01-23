/*
 * File: Calibration.h
 *
 * Code generated for Simulink model 'ElectronicParkBrake'.
 *
 * C source code generated on : Mon Dec  3  2018
 *
 * Author : SEUNGRYONG BAEK
 *
 * Last updated on : Mon Jan 13 2020 / SEUNGRYONG BAEK
 */

/* Modified at 18.12.27 / SEUNGRYONG BAEK, added calibration data 
 * LT_Force LookupTableForce
 * uint16_T C_Rollerbench_Force_Level1
 * uint16_T C_Rollerbench_Force_Level2
 * uint16_T C_Rollerbench_Force_Level3
 * uint16_T C_Rollerbench_Max_Process_Time
 */
 
/* Modified at 18.12.27 / SEUNGRYONG BAEK, deleted unused calibration data 
 * uint16_T C_Light_Apply_CableForce
 * uint16_T C_Max_Apply_CableForce
 * uint16_T C_Mid_Apply_CableForce
 */
 
/* Modified at 19.02.19 / SEUNGRYONG BAEK, deleted unused calibration data 
 * uint16_T C_Release_CableForce
 */ 
/* Modified at 19.02.19 / SEUNGRYONG BAEK, added calibration data
 * uint16_T C_Release_Force
 * uint16_T C_Rollerbench_Max_Routine_Time
 */
 
/* Modified at 19.04.01 / SEUNGRYONG BAEK, deleted unused calibration data 
 * LT_Force LookupTableForce
 * uint16_T C_Degraded_Manual_Apply_Switch_Timer
 * uint16_T C_Degraded_Manual_Release_Switch_Timer
 * uint16_T C_Rollerbench_Max_Routine_Time
 */
/* Modified at 19.04.01 / SEUNGRYONG BAEK, added calibration data
 * real_T PbcDataCurrentFilterFrequency
 * int16_T C_Release_Current
 * uint16_T C_Additional_Release_Hall_Count
 * uint16_T C_Additional_Release_PWM
 * uint16_T PbcDataMaxForce;
 * uint16_T PbcDataReleaseForce;
 */
  
/* Modified at 19.04.04 / SEUNGRYONG BAEK, deleted unused calibration data 
 * int16_T C_Max_Apply_Hall
 * int16_T C_Release_Hall
 */
 
/* Modified at 19.11.27 / SEUNGRYONG BAEK, added calibration data
 * uint16_T C_Degraded_Manual_Release_Switch_Timer;
 */ 
  
  
#ifndef RTW_HEADER_Calibration_h_
#define RTW_HEADER_Calibration_h_
#include <math.h>
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#include "LookupTable.h"
#include "ElectronicParkBrake_types.h"


#ifndef DEFINED_TYPEDEF_FOR_CalibrationBus_
#define DEFINED_TYPEDEF_FOR_CalibrationBus_

typedef struct {  
  LT_MotClutch LookupTableMotClutch;
  real_T PbcDataCorrectedLongAccelerationFilterFrequency;
  real_T PbcDataCurrentFilterFrequency;
  real_T PbcDataLongAccelerationFilterFrequency;
  real_T PbcDataVehicleAccelerationFilterFrequency;
  real_T PbcDataVehicleSpeedFilterFrequency;
  
  uint16_T C_Accelator_Position_Threshold;
  uint16_T C_Additional_Release_Hall_Count;
  uint16_T C_Additional_Release_PWM;
  uint16_T C_Apply_Time;
  uint16_T C_AutoSafety_Vehicle_Speed_Threshold;
  uint16_T C_Brake_Pedal_Pressed_True_Pressure;
  uint16_T C_Dynamic_Braking_EPB_TargetDecel;
  uint16_T C_Dynamic_Braking_Vehicle_Speed_Threshold;
  uint16_T C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold;  
  uint16_T C_Light_Apply_Force;
  uint16_T C_Light_Apply_Grade_Threshold;  
  int16_T C_Max_Apply_Current;
  uint16_T C_Max_Apply_Force;
  uint16_T C_Max_Apply_Grade_Threshold;  
  uint16_T C_Mid_Apply_Force;  
  int16_T C_Release_Current;
  uint16_T C_Release_Force;
  uint16_T C_Release_Time;
  
  uint16_T C_Degraded_Manual_Release_Switch_Timer;
  
  uint16_T C_Rollerbench_Force_Level1;
  uint16_T C_Rollerbench_Force_Level2;
  uint16_T C_Rollerbench_Force_Level3;
  uint16_T C_Rollerbench_Max_Process_Time;

  uint16_T PbcDataMaxForce;
  uint16_T PbcDataReleaseForce;
} CalibrationBus;

#endif

extern CalibrationBus CalibrationDatas;

void initializeCal(void);
void terminateCal(void);

#endif                                 /* RTW_HEADER_Calibration_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
