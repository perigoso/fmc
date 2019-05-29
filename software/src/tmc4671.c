#include "tmc4671.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STATE_NOTHING_TO_DO    0
#define STATE_START_INIT       1
#define STATE_WAIT_INIT_TIME   2
#define STATE_ESTIMATE_OFFSET  3

cp2130_device_t *pCpDev = NULL;

void tmc4671_init(cp2130_device_t *pCpInDev)
{
    pCpDev = pCpInDev;
}

// Data Transfer Frame
void tmc4671_write(uint8_t ubAddr, uint32_t ulData)
{
    uint8_t ubBuf[5];
    memset(ubBuf, 0x00, 5);

    ubBuf[0] = TMC4671_FRAME_WRITE | (ubAddr & 0x7F);
    ubBuf[1] = (ulData >> 24) & 0xFF;
    ubBuf[2] = (ulData >> 16) & 0xFF;
    ubBuf[3] = (ulData >> 8) & 0xFF;
    ubBuf[4] = ulData & 0xFF;

    cp2130_spi_transfer(pCpDev, ubBuf, 5);
}
uint32_t tmc4671_read(uint8_t ubAddr)
{
    uint8_t ubBuf[5];
    memset(ubBuf, 0x00, 5);

    ubBuf[0] = TMC4671_FRAME_READ | (ubAddr & 0x7F);

    cp2130_spi_transfer(pCpDev, ubBuf, 5);

    return ((uint32_t)ubBuf[1] << 24) | ((uint32_t)ubBuf[2] << 16) | ((uint32_t)ubBuf[3] << 8) | (uint32_t)ubBuf[4];
}
void tmc4671_modify(uint8_t ubAddr, uint32_t ulData, uint32_t ulMask)
{
    tmc4671_write(ubAddr, (tmc4671_read(ubAddr) & ~ulMask) | (ulData & ulMask));
}

void tmc4671_switch_motion_mode(uint32_t ubMode)
{
	// switch motion mode
    tmc4671_modify(TMC4671_MODE_RAMP_MODE_MOTION, ubMode, TMC4671_MODE_MOTION_MASK);
}

void tmc4671_set_target_torque_raw(int32_t lTargetTorque)
{
	tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_TORQUE);
	tmc4671_modify(TMC4671_PID_TORQUE_FLUX_TARGET, lTargetTorque << TMC4671_PID_TORQUE_TARGET_SHIFT, TMC4671_PID_TORQUE_TARGET_MASK);
}

int32_t tmc4671_get_target_torque_raw()
{
	// remember last set index
	uint32_t ulLastIndex = tmc4671_read(TMC4671_INTERIM_ADDR);

	// get value
	tmc4671_write(TMC4671_INTERIM_ADDR, TMC4671_INTERIM_ADDR_PIDIN_TARGET_TORQUE);
	int32_t lReturn = (int32_t)tmc4671_read(TMC4671_INTERIM_DATA);

	// reset last set index
	tmc4671_write(TMC4671_INTERIM_ADDR, ulLastIndex);

	return lReturn;
}

int32_t tmc4671_get_actual_torque_raw()
{
	return (int16_t)(tmc4671_read(TMC4671_PID_TORQUE_FLUX_ACTUAL) >> TMC4671_PID_TORQUE_ACTUAL_SHIFT);
}


void tmc4671_set_target_torque_mA(uint16_t usTorqueMeasurementFactor, int32_t lTargetTorque)
{
	tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_TORQUE);
	tmc4671_modify(TMC4671_PID_TORQUE_FLUX_TARGET, ((lTargetTorque * 256) / (int32_t) usTorqueMeasurementFactor) << TMC4671_PID_TORQUE_TARGET_SHIFT, TMC4671_PID_TORQUE_TARGET_MASK);
}

int32_t tmc4671_get_target_torque_mA(uint16_t usTorqueMeasurementFactor)
{
	return (tmc4671_get_target_torque_raw() * (int32_t)usTorqueMeasurementFactor) / 256;
}

int32_t tmc4671_get_actual_torque_mA(uint16_t usTorqueMeasurementFactor)
{
	return (tmc4671_get_actual_torque_raw() * (int32_t)usTorqueMeasurementFactor) / 256;
}

int32_t tmc4671_get_target_torque_flux_sum_mA(uint16_t usTorqueMeasurementFactor)
{
	// remember last set index
	uint32_t ulLastIndex = tmc4671_read(TMC4671_INTERIM_ADDR);

	// get target torque value
	tmc4671_write(TMC4671_INTERIM_ADDR, TMC4671_INTERIM_ADDR_PIDIN_TARGET_TORQUE);
	int32_t lTorque = (int32_t)tmc4671_read(TMC4671_INTERIM_DATA);

	// get target flux value
    tmc4671_write(TMC4671_INTERIM_ADDR, TMC4671_INTERIM_ADDR_PIDIN_TARGET_FLUX);
	int32_t lFlux = (int32_t)tmc4671_read(TMC4671_INTERIM_DATA);

	// reset last set index
	tmc4671_write(TMC4671_INTERIM_ADDR, ulLastIndex);

	return (((int32_t)lFlux + (int32_t)lTorque) * (int32_t)usTorqueMeasurementFactor) / 256;
}

int32_t tmc4671_get_actual_torque_flux_sum_mA(uint16_t usTorqueMeasurementFactor)
{
	int32_t lRegisterValue = tmc4671_read(TMC4671_PID_TORQUE_FLUX_ACTUAL);
	int16_t sFlux = (lRegisterValue & TMC4671_PID_FLUX_ACTUAL_MASK);
	int16_t sTorque = (lRegisterValue & TMC4671_PID_TORQUE_ACTUAL_MASK) >> TMC4671_PID_TORQUE_ACTUAL_SHIFT;
	return (((int32_t)sFlux + (int32_t)sTorque) * (int32_t)usTorqueMeasurementFactor) / 256;
}

void tmc4671_set_target_flux_raw(int32_t targetFlux)
{
	// do not change the MOTION_MODE here! target flux can also be used during velocity and position modes
	tmc4671_modify(TMC4671_PID_TORQUE_FLUX_TARGET, targetFlux, TMC4671_PID_FLUX_TARGET_MASK);
}

int32_t tmc4671_get_target_flux_raw()
{
	// remember last set index
	uint32_t lLastIndex = tmc4671_read(TMC4671_INTERIM_ADDR);

	// get value
	tmc4671_write(TMC4671_INTERIM_ADDR, TMC4671_INTERIM_ADDR_PIDIN_TARGET_FLUX);
	int32_t lReturn = (int32_t)tmc4671_read(TMC4671_INTERIM_DATA);

	// reset last set index
	tmc4671_write(TMC4671_INTERIM_ADDR, lLastIndex);
	return lReturn;
}

int32_t tmc4671_get_actual_flux_raw()
{
	return (int16_t)(tmc4671_read(TMC4671_PID_TORQUE_FLUX_ACTUAL) & TMC4671_PID_FLUX_ACTUAL_MASK);
}

void tmc4671_set_target_flux_mA(uint16_t usTorqueMeasurementFactor, int32_t lTargetFlux)
{
	// do not change the MOTION_MODE here! target flux can also be used during velocity and position modes
	tmc4671_modify(TMC4671_PID_TORQUE_FLUX_TARGET, ((lTargetFlux * 256) / (int32_t)usTorqueMeasurementFactor), TMC4671_PID_FLUX_TARGET_MASK);
}

int32_t tmc4671_get_target_flux_mA(uint16_t usTorqueMeasurementFactor)
{
	return (tmc4671_get_target_flux_raw() * (int32_t)usTorqueMeasurementFactor) / 256;
}

int32_t tmc4671_get_actual_flux_mA(uint16_t usTorqueMeasurementFactor)
{
	return (tmc4671_get_actual_flux_raw() * (int32_t)usTorqueMeasurementFactor) / 256;
}

void tmc4671_set_torque_flux_limit_mA(uint16_t usTorqueMeasurementFactor, int32_t lMax)
{
	tmc4671_write(TMC4671_PID_TORQUE_FLUX_LIMITS, ((lMax * 256) / (int32_t)usTorqueMeasurementFactor) & TMC4671_PID_TORQUE_FLUX_LIMITS_MASK);
}

int32_t tmc4671_get_torque_flux_limit_mA(uint16_t usTorqueMeasurementFactor)
{
	return ((int32_t)(tmc4671_read(TMC4671_PID_TORQUE_FLUX_LIMITS) & TMC4671_PID_TORQUE_FLUX_LIMITS_MASK) * (int32_t)usTorqueMeasurementFactor) / 256;
}

void tmc4671_set_target_velocity(int32_t lTargetVelocity)
{
	tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_VELOCITY);
	tmc4671_write(TMC4671_PID_VELOCITY_TARGET, lTargetVelocity);
}

int32_t tmc4671_get_target_velocity()
{
	return (int32_t)tmc4671_read(TMC4671_PID_VELOCITY_TARGET);
}

int32_t tmc4671_get_actual_velocity()
{
	return (int32_t)tmc4671_read(TMC4671_PID_VELOCITY_ACTUAL);
}

void tmc4671_set_absolute_target_position(int32_t lTargetPosition)
{
	tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_POSITION);
	tmc4671_write(TMC4671_PID_POSITION_TARGET, lTargetPosition);
}

void tmc4671_set_relative_target_position(int32_t lRelativePosition)
{
	tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_POSITION);
	// determine actual position and add relative position ticks
	tmc4671_write(TMC4671_PID_POSITION_TARGET, (int32_t)tmc4671_read(TMC4671_PID_POSITION_ACTUAL) + lRelativePosition);
}

int32_t tmc4671_get_target_position()
{
	return (int32_t)tmc4671_read(TMC4671_PID_POSITION_TARGET);
}

void tmc4671_set_actual_position(int32_t lActualPosition)
{
	tmc4671_write(TMC4671_PID_POSITION_ACTUAL, lActualPosition);
}

int32_t tmc4671_get_actual_position()
{
	return (int32_t)tmc4671_read(TMC4671_PID_POSITION_ACTUAL);
}

// encoder initialization
void tmc4671_doEncoderInitializationMode0(uint8_t motor, uint8_t *initState, uint16_t initWaitTime, uint16_t *actualInitWaitTime, uint16_t startVoltage)
{
	static uint16_t last_Phi_E_Selection = 0;
	static uint32_t last_UQ_UD_EXT = 0;
	static int16_t last_PHI_E_EXT = 0;

	switch (*initState)
	{
	case STATE_NOTHING_TO_DO:
		*actualInitWaitTime = 0;
		break;
	case STATE_START_INIT: // started by writing 1 to initState

		// save actual set values for PHI_E_SELECTION, UQ_UD_EXT, and PHI_E_EXT
		last_Phi_E_Selection = (uint16_t) tmc4671_readRegister16BitValue(motor, TMC4671_PHI_E_SELECTION, BIT_0_TO_15);
		last_UQ_UD_EXT = (uint32_t) tmc4671_readInt(motor, TMC4671_UQ_UD_EXT);
		last_PHI_E_EXT = (int16_t) tmc4671_readRegister16BitValue(motor, TMC4671_PHI_E_EXT, BIT_0_TO_15);

		// set ABN_DECODER_PHI_E_OFFSET to zero
		tmc4671_writeRegister16BitValue(motor, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, BIT_16_TO_31, 0);

		// select phi_e_ext
		tmc4671_writeRegister16BitValue(motor, TMC4671_PHI_E_SELECTION, BIT_0_TO_15, 1);

		// set an initialization voltage on UD_EXT (to the flux, not the torque!)
		tmc4671_writeRegister16BitValue(motor, TMC4671_UQ_UD_EXT, BIT_16_TO_31, 0);
		tmc4671_writeRegister16BitValue(motor, TMC4671_UQ_UD_EXT, BIT_0_TO_15, startVoltage);

		// set the "zero" angle
		tmc4671_writeRegister16BitValue(motor, TMC4671_PHI_E_EXT, BIT_0_TO_15, 0);

		*initState = STATE_WAIT_INIT_TIME;
		break;
	case STATE_WAIT_INIT_TIME:
		// wait until initialization time is over (until no more vibration on the motor)
		(*actualInitWaitTime)++;
		if(*actualInitWaitTime >= initWaitTime)
		{
			// set internal encoder value to zero
			tmc4671_writeInt(motor, TMC4671_ABN_DECODER_COUNT, 0);

			// switch back to last used UQ_UD_EXT setting
			tmc4671_writeInt(motor, TMC4671_UQ_UD_EXT, last_UQ_UD_EXT);

			// set PHI_E_EXT back to last value
			tmc4671_writeRegister16BitValue(motor, TMC4671_PHI_E_EXT, BIT_0_TO_15, last_PHI_E_EXT);

			// switch back to last used PHI_E_SELECTION setting
			tmc4671_writeRegister16BitValue(motor, TMC4671_PHI_E_SELECTION, BIT_0_TO_15, last_Phi_E_Selection);

			// go to next state
			*initState = STATE_ESTIMATE_OFFSET;
		}
		break;
	case STATE_ESTIMATE_OFFSET:
		// you can do offset estimation here (wait for N-Channel if available and save encoder value)

		// go to ready state
		*initState = 0;
		break;
	default:
		*initState = 0;
		break;
	}
}

int16_t tmc4671_getS16CircleDifference(int16_t newValue, int16_t oldValue)
{
	return (newValue - oldValue);
}

void tmc4671_doEncoderInitializationMode2(uint8_t motor, uint8_t *initState, uint16_t *actualInitWaitTime)
{
	static int16_t hall_phi_e_old = 0;
	static int16_t hall_phi_e_new = 0;
	static int16_t actual_coarse_offset = 0;

	switch (*initState)
	{
	case STATE_NOTHING_TO_DO:
		*actualInitWaitTime = 0;
		break;
	case STATE_START_INIT: // started by writing 1 to initState
		// turn hall_mode interpolation off (read, clear bit 8, write back)
		tmc4671_writeInt(motor, TMC4671_HALL_MODE, tmc4671_readInt(motor, TMC4671_HALL_MODE) & 0xFFFFFEFF);

		// set ABN_DECODER_PHI_E_OFFSET to zero
		tmc4671_writeRegister16BitValue(motor, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, BIT_16_TO_31, 0);

		// read actual hall angle
		hall_phi_e_old = (int16_t) tmc4671_readRegister16BitValue(motor, TMC4671_HALL_PHI_E_INTERPOLATED_PHI_E, BIT_0_TO_15);

		// read actual abn_decoder angle and compute difference to actual hall angle
		actual_coarse_offset = tmc4671_getS16CircleDifference(hall_phi_e_old, (int16_t) tmc4671_readRegister16BitValue(motor, TMC4671_ABN_DECODER_PHI_E_PHI_M, BIT_16_TO_31));

		// set ABN_DECODER_PHI_E_OFFSET to actual hall-abn-difference, to use the actual hall angle for coarse initialization
		tmc4671_writeRegister16BitValue(motor, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, BIT_16_TO_31, actual_coarse_offset);

		*initState = STATE_WAIT_INIT_TIME;
		break;
	case STATE_WAIT_INIT_TIME:
		// read actual hall angle
		hall_phi_e_new = (int16_t) tmc4671_readRegister16BitValue(motor, TMC4671_HALL_PHI_E_INTERPOLATED_PHI_E, BIT_0_TO_15);

		// wait until hall angle changed
		if(hall_phi_e_old != hall_phi_e_new)
		{
			// estimated value = old value + diff between old and new (handle int16_t overrun)
			int16_t hall_phi_e_estimated = hall_phi_e_old + tmc4671_getS16CircleDifference(hall_phi_e_new, hall_phi_e_old)/2;

			// read actual abn_decoder angle and consider last set abn_decoder_offset
			int16_t abn_phi_e_actual = (int16_t) tmc4671_readRegister16BitValue(motor, TMC4671_ABN_DECODER_PHI_E_PHI_M, BIT_16_TO_31) - actual_coarse_offset;

			// set ABN_DECODER_PHI_E_OFFSET to actual estimated angle - abn_phi_e_actual difference
			tmc4671_writeRegister16BitValue(motor, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, BIT_16_TO_31, tmc4671_getS16CircleDifference(hall_phi_e_estimated, abn_phi_e_actual));

			// go to ready state
			*initState = 0;
		}
		break;
	default:
		*initState = 0;
		break;
	}
}

void tmc4671_checkEncderInitialization(uint8_t motor, uint32_t actualSystick, uint8_t initMode, uint8_t *initState, uint16_t initWaitTime, uint16_t *actualInitWaitTime, uint16_t startVoltage)
{
	// use the systick as 1ms timer for encoder initialization
	static uint32_t lastSystick = 0;
	if(actualSystick != lastSystick)
	{
		// needs timer to use the wait time
		if(initMode == 0)
		{
			tmc4671_doEncoderInitializationMode0(motor, initState, initWaitTime, actualInitWaitTime, startVoltage);
		}
		lastSystick = actualSystick;
	}

	// needs no timer
	if(initMode == 2)
	{
		tmc4671_doEncoderInitializationMode2(motor, initState, actualInitWaitTime);
	}
}

void tmc4671_task(uint32_t ulActualSystick, uint8_t ubInitMode, uint8_t *pubInitState, uint16_t usInitWaitTime, uint16_t *pusActualInitWaitTime, uint16_t usStartVoltage)
{
	tmc4671_checkEncderInitialization(NULL, ulActualSystick, ubInitMode, pubInitState, usInitWaitTime, pusActualInitWaitTime, usStartVoltage);
}

void tmc4671_startEncoderInitialization(uint8_t mode, uint8_t *initMode, uint8_t *initState)
{
	// allow only a new initialization if no actual initialization is running
	if(*initState == STATE_NOTHING_TO_DO)
	{
		if(mode == 0) // estimate offset
		{
			// set mode
			*initMode = 0;

			// start initialization
			*initState = 1;
		}
		else if(mode == 2) // use hall sensor signals
		{
			// set mode
			*initMode = 2;

			// start initialization
			*initState = 1;
		}
	}
}

void tmc4671_updatePhiSelectionAndInitialize(uint8_t motor, uint8_t actualPhiESelection, uint8_t desiredPhiESelection, uint8_t initMode, uint8_t *initState)
{
	if(actualPhiESelection != desiredPhiESelection)
	{
		tmc4671_writeInt(motor, TMC4671_PHI_E_SELECTION, desiredPhiESelection);

		switch(desiredPhiESelection)
		{
			case 3:
				tmc4671_startEncoderInitialization(initMode, &initMode, initState);
				break;
		}
	}
}

// =====

void tmc4671_disable_PWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, TMC4671_PWM_OFF, TMC4671_PWM_CHOP_MASK);
}

uint8_t tmc4671_get_pole_pairs()
{
	return tmc4671_read(TMC4671_MOTOR_TYPE_N_POLE_PAIRS) & TMC4671_N_POLE_PAIRS_MASK;
}

void tmc4671_set_pole_pairs(uint8_t ubPolePairs)
{
	tmc4671_modify(TMC4671_MOTOR_TYPE_N_POLE_PAIRS, ubPolePairs, TMC4671_N_POLE_PAIRS_MASK);
}

uint16_t tmc4671_get_adcI0_offset()
{
	return tmc4671_read(TMC4671_ADC_I0_SCALE_OFFSET) & TMC4671_ADC_I0_OFFSET_MASK;
}

void tmc4671_set_adcI0_offset(uint16_t usOffset)
{
	tmc4671_modify(TMC4671_ADC_I0_SCALE_OFFSET, usOffset, TMC4671_ADC_I0_OFFSET_MASK);
}

uint16_t tmc4671_get_adcI1_offset()
{
	return (tmc4671_read(TMC4671_ADC_I1_SCALE_OFFSET) & TMC4671_ADC_I1_OFFSET_MASK);
}

void tmc4671_set_adcI1_offset(uint16_t usOffset)
{
	tmc4671_modify(TMC4671_ADC_I1_SCALE_OFFSET, usOffset, TMC4671_ADC_I1_OFFSET_MASK);
}

void tmc4671_set_torque_flux_PI(uint16_t usPParameter, uint16_t usIParameter)
{
	tmc4671_write(TMC4671_PID_FLUX_P_FLUX_I, (((uint32_t)usPParameter << TMC4671_PID_FLUX_P_SHIFT) & TMC4671_PID_FLUX_P_MASK) | ((uint32_t)usIParameter) & TMC4671_PID_FLUX_I_MASK);
	tmc4671_write(TMC4671_PID_TORQUE_P_TORQUE_I, (((uint32_t)usPParameter << TMC4671_PID_TORQUE_P_SHIFT) & TMC4671_PID_TORQUE_P_MASK) | ((uint32_t)usIParameter) & TMC4671_PID_TORQUE_I_MASK);
}

void tmc4671_set_velocity_PI(uint16_t usPParameter, uint16_t usIParameter)
{
	tmc4671_write(TMC4671_PID_VELOCITY_P_VELOCITY_I, (((uint32_t)usPParameter << TMC4671_PID_VELOCITY_P_SHIFT) & TMC4671_PID_VELOCITY_P_MASK) | ((uint32_t)usIParameter) & TMC4671_PID_VELOCITY_I_MASK);
}

void tmc4671_set_position_PI(uint16_t usPParameter, uint16_t usIParameter)
{
	tmc4671_write(TMC4671_PID_POSITION_P_POSITION_I, (((uint32_t)usPParameter << TMC4671_PID_POSITION_P_SHIFT) & TMC4671_PID_POSITION_P_MASK) | ((uint32_t)usIParameter) & TMC4671_PID_POSITION_I_MASK);
}

int32_t tmc4671_readFieldWithDependency(uint8_t motor, uint8_t reg, uint8_t dependsReg, uint32_t dependsValue, uint32_t mask, uint8_t shift)
{
	// remember old depends value
	uint32_t lastDependsValue = tmc4671_readInt(motor, dependsReg);

	// set needed depends value
	tmc4671_writeInt(motor, dependsReg, dependsValue);
	uint32_t value = FIELD_GET(tmc4671_readInt(motor, reg), mask, shift);

	// set old depends value
	tmc4671_writeInt(motor, dependsReg, lastDependsValue);
	return value;
}

#ifdef __cplusplus
}
#endif