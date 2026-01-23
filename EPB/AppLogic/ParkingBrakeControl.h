/*
 * File: ParkingBrakeControl.h
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

#ifndef RTW_HEADER_ParkingBrakeControl_h_
#define RTW_HEADER_ParkingBrakeControl_h_
#include <math.h>
#ifndef ElectronicParkBrake_COMMON_INCLUDES_
# define ElectronicParkBrake_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* ElectronicParkBrake_COMMON_INCLUDES_ */

#include "ElectronicParkBrake_types.h"

extern void Electr_ParkingBrakeControl_Init(void);
extern void ElectronicP_ParkingBrakeControl(void);

#endif                                 /* RTW_HEADER_ParkingBrakeControl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
