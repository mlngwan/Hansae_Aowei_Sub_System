/*
 * File: Calibration.c
 *
 * Code generated for Simulink model 'ElectronicParkBrake'.
 *
 * C source code generated on : THU Dec  19  2018
 *
 * Author : SEUNGRYONG BAEK
 *
 * Last updated on : MON Jan 13 2020 / SEUNGRYONG BAEK
 */
 
/* Modified at 18.12.27 / SEUNGRYONG BAEK, function has modified that according to add calibration data.
 * LT_Force LookupTableForce
 * uint16_T C_Rollerbench_Force_Level1
 * uint16_T C_Rollerbench_Force_Level2
 * uint16_T C_Rollerbench_Force_Level3
 * uint16_T C_Rollerbench_Max_Process_Time
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
 
/* Modified at 19.04.04 / SEUNGRYONG BAEK, added calibration data
 * uint16_T C_Dynamic_Braking_EPB_TargetDecel
 */  

/* Modified at 19.11.27 / SEUNGRYONG BAEK, added calibration data
 * uint16_T C_Degraded_Manual_Release_Switch_Timer;
 */ 
 
#include "Calibration.h"
#include "ElectronicParkBrake.h"
 
CalibrationBus CalibrationDatas;

void initializeCal(void)
{

  LookupTableMotClutch = CalibrationDatas.LookupTableMotClutch;
  //PbcDataCorrectedLongAccelerationFilterFrequency = CalibrationDatas.PbcDataCorrectedLongAccelerationFilterFrequency;
  PbcDataCurrentFilterFrequency = CalibrationDatas.PbcDataCurrentFilterFrequency;
  PbcDataLongAccelerationFilterFrequency = CalibrationDatas.PbcDataLongAccelerationFilterFrequency;
  PbcDataVehicleAccelerationFilterFrequency = CalibrationDatas.PbcDataVehicleAccelerationFilterFrequency;
  PbcDataVehicleSpeedFilterFrequency = CalibrationDatas.PbcDataVehicleSpeedFilterFrequency;
  
  C_Accelator_Position_Threshold = CalibrationDatas.C_Accelator_Position_Threshold;
  C_Additional_Release_Hall_Count = CalibrationDatas.C_Additional_Release_Hall_Count;
  C_Additional_Release_PWM = CalibrationDatas.C_Additional_Release_PWM;
  C_Apply_Time = CalibrationDatas.C_Apply_Time;
  C_AutoSafety_Vehicle_Speed_Threshold = CalibrationDatas.C_AutoSafety_Vehicle_Speed_Threshold;
  C_Brake_Pedal_Pressed_True_Pressure = CalibrationDatas.C_Brake_Pedal_Pressed_True_Pressure;
  C_Dynamic_Braking_EPB_TargetDecel = CalibrationDatas.C_Dynamic_Braking_EPB_TargetDecel;
  C_Dynamic_Braking_Vehicle_Speed_Threshold = CalibrationDatas.C_Dynamic_Braking_Vehicle_Speed_Threshold;  
  C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold = CalibrationDatas.C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold;  
  C_Light_Apply_Force = CalibrationDatas.C_Light_Apply_Force;
  C_Light_Apply_Grade_Threshold = CalibrationDatas.C_Light_Apply_Grade_Threshold;  
  C_Max_Apply_Current = CalibrationDatas.C_Max_Apply_Current;
  C_Max_Apply_Force = CalibrationDatas.C_Max_Apply_Force;
  C_Max_Apply_Grade_Threshold = CalibrationDatas.C_Max_Apply_Grade_Threshold;
  C_Mid_Apply_Force = CalibrationDatas.C_Mid_Apply_Force;  
  C_Release_Current = CalibrationDatas.C_Release_Current;
  C_Release_Force = CalibrationDatas.C_Release_Force;
  C_Release_Time = CalibrationDatas.C_Release_Time;
  
  C_Degraded_Manual_Release_Switch_Timer = CalibrationDatas.C_Degraded_Manual_Release_Switch_Timer;
  
  C_Rollerbench_Force_Level1 = CalibrationDatas.C_Rollerbench_Force_Level1;
  C_Rollerbench_Force_Level2 = CalibrationDatas.C_Rollerbench_Force_Level2;
  C_Rollerbench_Force_Level3 = CalibrationDatas.C_Rollerbench_Force_Level3;
  C_Rollerbench_Max_Process_Time = CalibrationDatas.C_Rollerbench_Max_Process_Time;
  
  PbcDataMaxForce = CalibrationDatas.PbcDataMaxForce;
  PbcDataReleaseForce = CalibrationDatas.PbcDataReleaseForce;
}

void terminateCal(void)
{
  CalibrationDatas.LookupTableMotClutch = LookupTableMotClutch;
  //CalibrationDatas.PbcDataCorrectedLongAccelerationFilterFrequency = PbcDataCorrectedLongAccelerationFilterFrequency;
  CalibrationDatas.PbcDataCurrentFilterFrequency = PbcDataCurrentFilterFrequency;
  CalibrationDatas.PbcDataLongAccelerationFilterFrequency = PbcDataLongAccelerationFilterFrequency;
  CalibrationDatas.PbcDataVehicleAccelerationFilterFrequency = PbcDataVehicleAccelerationFilterFrequency;
  CalibrationDatas.PbcDataVehicleSpeedFilterFrequency = PbcDataVehicleSpeedFilterFrequency;
  
  CalibrationDatas.C_Accelator_Position_Threshold = C_Accelator_Position_Threshold;
  CalibrationDatas.C_Additional_Release_Hall_Count = C_Additional_Release_Hall_Count;
  CalibrationDatas.C_Additional_Release_PWM = C_Additional_Release_PWM;
  CalibrationDatas.C_Apply_Time = C_Apply_Time;
  CalibrationDatas.C_AutoSafety_Vehicle_Speed_Threshold = C_AutoSafety_Vehicle_Speed_Threshold;
  CalibrationDatas.C_Brake_Pedal_Pressed_True_Pressure = C_Brake_Pedal_Pressed_True_Pressure;
  CalibrationDatas.C_Dynamic_Braking_EPB_TargetDecel = C_Dynamic_Braking_EPB_TargetDecel; // 19.04.04 new
  CalibrationDatas.C_Dynamic_Braking_Vehicle_Speed_Threshold = C_Dynamic_Braking_Vehicle_Speed_Threshold;
  CalibrationDatas.C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold = C_Electric_Park_Brake_Vehicle_Speed_Apply_Threshold;
  CalibrationDatas.C_Light_Apply_Force = C_Light_Apply_Force;
  CalibrationDatas.C_Light_Apply_Grade_Threshold = C_Light_Apply_Grade_Threshold;
  CalibrationDatas.C_Max_Apply_Current = C_Max_Apply_Current;
  CalibrationDatas.C_Max_Apply_Force = C_Max_Apply_Force;
  CalibrationDatas.C_Max_Apply_Grade_Threshold = C_Max_Apply_Grade_Threshold;
  CalibrationDatas.C_Mid_Apply_Force = C_Mid_Apply_Force;
  CalibrationDatas.C_Release_Current = C_Release_Current;
  CalibrationDatas.C_Release_Force = C_Release_Force;
  CalibrationDatas.C_Release_Time = C_Release_Time;
  
  CalibrationDatas.C_Degraded_Manual_Release_Switch_Timer = C_Degraded_Manual_Release_Switch_Timer;
  
  CalibrationDatas.C_Rollerbench_Force_Level1 = C_Rollerbench_Force_Level1;
  CalibrationDatas.C_Rollerbench_Force_Level2 = C_Rollerbench_Force_Level2;
  CalibrationDatas.C_Rollerbench_Force_Level3 = C_Rollerbench_Force_Level3;
  CalibrationDatas.C_Rollerbench_Max_Process_Time = C_Rollerbench_Max_Process_Time;
  
  CalibrationDatas.PbcDataMaxForce = PbcDataMaxForce;
  CalibrationDatas.PbcDataReleaseForce = PbcDataReleaseForce;

}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */