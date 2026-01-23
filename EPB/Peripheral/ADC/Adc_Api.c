#include "Adc_Api.h"
#include "Spi_L9369Data_Types.h"
#include "ElectronicParkBrake_types.h"

extern SSMInputBus SSMInput;
extern PbcInDriverBus PbcInDriver;

ADC_t g_ADC;

/*===========================================================================
 * MCAL Layer
 *===========================================================================*/

void Mcal_Adc_Initialize(void)
{
	ADC.MCR.R = 0x00000000; /* reset register and get out of power-down mode */
	ADC.MCR.B.OWREN = 1;
	ADC.MCR.B.ADCLKSEL = 1;

	ADC.CTR[0].B.INPLATCH = 1; /* set to 1 when INPCMP>=2 */
	ADC.CTR[0].B.INPCMP = 2;
	ADC.CTR[0].B.INPSAMP = 17;

	/* Teval=10*INPCMP*Tck = 20Tck = 833ns @ fADC=48MHz/2 */
	/* Tsamp=INPSAMP*Tck = 17Tck = 708ns @ fADC=48MHz/2 */

	/* Teval=10*INPCMP*Tck = 20Tck = 312ns @ fADC=64MHz */
	/* Tsamp=INPSAMP*Tck = 17Tck = 265ns @ fADC=64MHz */
	/* Tconv = 624ns*/
}

uword Mcal_Adc_ReadChannel(uint8_t adc_ch)
{
	uword adc_data_volt = 0;
	uint8_t shift_ch = 0;
	vuint32_t *adc_reg = (void *)0; // Null Pinter initialize

	if (adc_ch < 32)
	{
		shift_ch = 0;
		adc_reg = &ADC.NCMR[0].R;
	}
	else if (adc_ch < 64)
	{
		shift_ch = 32;
		adc_reg = &ADC.NCMR[1].R;
	}
	else
	{
		shift_ch = 64;
		adc_reg = &ADC.NCMR[2].R;
	}

	*adc_reg = (uint32_t)0x00000001 << (adc_ch - shift_ch); /* enable CH64~ normal conversion */

	ADC.MCR.B.NSTART = 1; /* start normal conversion */
	g_ADC.Flag.Timeout = 0;
	while (ADC.MSR.B.NSTART != 0)
	{
		/* wait until the conversion is finished */
		if (g_ADC.Flag.Timeout)
			break;
	}

	adc_data_volt = (uint32_t)ADC.CDR[adc_ch].R & 0x000003FF; /* read AD result from data register */

	return adc_data_volt;
}

float Mcal_Adc_ReadL9369Motor(uint8_t motor_id, uint8_t data_type)
{
	/* L9369 IC communication - external device access */
	if (data_type == 0) /* Voltage */
	{
		if (motor_id == L9369_LEFT)
		{
			return (float)(g_L9369_Adc_Value.VSBRIDGE_A[L9369_ADC_AVR]);
		}
		else
		{
			return (float)(g_L9369_Adc_Value.VSBRIDGE_B[L9369_ADC_AVR]);
		}
	}
	else /* Current */
	{
		float value;
		if (motor_id == L9369_LEFT)
		{
			value = (float)(g_L9369_Adc_Value.CS1_A[L9369_ADC_AVR]);
		}
		else
		{
			value = (float)(g_L9369_Adc_Value.CS1_B[L9369_ADC_AVR]);
		}
		if (value < 0)
			value = 0; /* Current cannot be negative */
		return value;
	}
}

/*===========================================================================
 * BSW Layer
 *===========================================================================*/

uword Bsw_Adc_GetAveraged(ubyte ADC_Channel)
{
	ubyte i;
	uword Temp_Cur = 0, Max = 0, Min = 0xffff, Total = 0;

	Max = 0;
	Min = 0xffff;
	Total = 0;
	for (i = 0; i < 6; i++)
	{
		Temp_Cur = Mcal_Adc_ReadChannel(ADC_Channel);
		if (Temp_Cur > Max)
		{
			Max = Temp_Cur;
		}
		if (Temp_Cur < Min)
		{
			Min = Temp_Cur;
		}

		Total += Temp_Cur;
	}

	return (Total - (Max + Min)) / 4;
}

float Bsw_Adc_ConvertToPhysical(uint16_t adcvalue, float v_ref, uint16_t r1, uint16_t r2)
{
	float ConvertVolt, PhysicalVolt;

	PhysicalVolt = (adcvalue / 1023.0) * v_ref;

	ConvertVolt = PhysicalVolt * ((float)(r1 + r2) / r2);

	return ConvertVolt;
}

void Bsw_Adc_ReadAllChannels(void)
{
	// 1. UBB : Bat for Motor
	// 2. UBVR : Bat for Valve -> 3. VBATP : Bat for EPB Motor
	g_ADC.Raw.UBB = Bsw_Adc_GetAveraged(ADC_CH_SUB_UBB_MON);
	g_ADC.Raw.IGN = Bsw_Adc_GetAveraged(ADC_CH_SUB_IGN_MON);
	g_ADC.Raw.UBVR = Bsw_Adc_GetAveraged(ADC_CH_SUB_UBVR_MON);

	g_ADC.Physical.Power.UBB = Bsw_Adc_ConvertToPhysical(g_ADC.Raw.UBB, ref_3v3, r300, r51) * ADC_REF;
	// g_ADC.Physical.Power.IBAT = g_ADC.Physical.Power.BAT*ADC_REF;
	g_ADC.Physical.Power.IGN = Bsw_Adc_ConvertToPhysical(g_ADC.Raw.IGN, ref_3v3, r300, r51) * ADC_REF;
	g_ADC.Physical.Power.UBVR = Bsw_Adc_ConvertToPhysical(g_ADC.Raw.UBVR, ref_3v3, r300, r51) * ADC_REF;

	g_ADC.Physical.Motor.MOTORA = Mcal_Adc_ReadL9369Motor(L9369_LEFT, 0) * ADC_REF;
	g_ADC.Physical.Motor.MOTORB = Mcal_Adc_ReadL9369Motor(L9369_RIGHT, 0) * ADC_REF;
	g_ADC.Physical.Motor.MOTORA_CUR = Mcal_Adc_ReadL9369Motor(L9369_LEFT, 1) * ADC_REF;
	g_ADC.Physical.Motor.MOTORB_CUR = Mcal_Adc_ReadL9369Motor(L9369_RIGHT, 1) * ADC_REF;
}

/*===========================================================================
 * Fail-Safe Layer
 *===========================================================================*/

void Fs_Adc_UpdateData(void)
{
	SSMInput.SSMInVoltage = g_ADC.Physical.Power.UBB;

	PbcInDriver.MotorDriverSupplyVoltage = g_ADC.Physical.Power.UBB;
	PbcInDriver.MotorVoltageLeft = g_ADC.Physical.Motor.MOTORA;
	PbcInDriver.MotorVoltageRight = g_ADC.Physical.Motor.MOTORB;
	PbcInDriver.MotorCurrentLeft = g_ADC.Physical.Motor.MOTORA_CUR;
	PbcInDriver.MotorCurrentRight = g_ADC.Physical.Motor.MOTORB_CUR;
}

void Fs_Adc_All(void)
{
	Bsw_Adc_ReadAllChannels();
	Fs_Adc_UpdateData();
}

/*===========================================================================
 * 작동을 위한 기존 코드
 *===========================================================================*/
void ADCInitialize(void)
{
	Mcal_Adc_Initialize();
}

uword GetADCdata(uint8_t adc_ch)
{
	return Mcal_Adc_ReadChannel(adc_ch);
}

uword GetAdcAverage(ubyte ADC_Channel)
{
	return Bsw_Adc_GetAveraged(ADC_Channel);
}

float ConvertToPhysicalBatVolt(uint16_t adcvalue, float v_ref, uint16_t r1, uint16_t r2)
{
	return Bsw_Adc_ConvertToPhysical(adcvalue, v_ref, r1, r2);
}

void GetRawData(void)
{
	Bsw_Adc_ReadAllChannels();
}

void SetLogicData(void)
{
	Fs_Adc_UpdateData();
}

void CheckAdcStatus(void)
{
	Fs_Adc_All();
}
