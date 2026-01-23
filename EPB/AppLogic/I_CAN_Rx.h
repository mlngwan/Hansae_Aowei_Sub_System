/*
 * File: I_CAN_Rx.h
 *
 * Code generated for Simulink model 'DBSi_Proto9'.
 *
 * Model version                  : 1.3213
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Thu Sep  9 13:43:56 2021
 */

#ifndef RTW_HEADER_I_CAN_Rx_h_
#define RTW_HEADER_I_CAN_Rx_h_
#include "rtwtypes.h"

/* All of signals to determine which vehicle is under stat
   2020.11.13 Seungbum Han
   [Daechang]
   Simulation 용, State Root 최상단에 존재. */
typedef struct {
  uint16_T WheelSpeed_FL;
  uint16_T WheelSpeed_FR;
  uint16_T WheelSpeed_ValueError_FL;
  uint16_T WheelSpeed_ValueError_FR;
  uint16_T WheelSpeed_RL;
  uint16_T WheelSpeed_RR;
  uint16_T WheelSpeed_ValueError_RL;
  uint16_T WheelSpeed_ValueError_RR;
  uint16_T WheelDirectionInfo_RL;
  uint16_T WheelDirectionInfo_RR;
  uint16_T M_MotClutch;
  uint16_T ratAccPed;
  uint16_T ratAccPedSts;
  uint16_T VehicleSpeed;
  uint16_T VehicleSpeed_ValueError;
  uint16_T AVH_Active;
  uint16_T RequestToEPB;
  uint16_T CDPDecelActive;
  uint16_T CDPDecelAvailable;
  uint16_T BrakePressureMC;
  uint16_T AX;
  uint16_T AX_STAT;
  uint16_T Hillmode;
  uint16_T Roadangle;
  uint16_T CoEng_st;
  uint16_T VehicleModeState;
  uint16_T stGearLvr;
  uint16_T BCM_LVBatteryVoltage;
  uint16_T DoorAjarState_FL;
  uint16_T BLS_Fault;
  uint16_T Bls_Out;
  uint16_T ACUSysDeactEn;
  uint16_T FrontBuckleDriver_L;
  uint16_T EPB_OpMode;
} I_CAN_Rx;

#endif                                 /* RTW_HEADER_I_CAN_Rx_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
