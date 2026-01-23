#include "Logic_Api.h"
#include "ElectronicParkBrake.h"

void ApplicationLogic(void)
{
	/* Step the model */
	/* Attach the model to a timer or interrupt service routine with
   * period 0.01 seconds (the model's base sample time) here.
   */
	ElectronicParkBrake_step();				//1 Application Logic call

}

uint8 GetEpbPowerLatchRequest(void)
{
	return (uint8)ElectronicParkBrake_Y.PbcOutput.PbcOutEcuPowerLatchRequest;
}

