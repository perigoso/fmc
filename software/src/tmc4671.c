#include "tmc4671.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MODE_ENC_ESTIMATE   0
#define MODE_ENC_USE_HALL   2

#define STATE_NOTHING_TO_DO    0
#define STATE_START_INIT       1
#define STATE_WAIT_INIT_TIME   2
#define STATE_ESTIMATE_OFFSET  3

struct encoder_init_t
{
    uint8_t ubInitMode;
    uint8_t ubInitState;
    uint16_t usStartVoltage;
    uint32_t ulInitWaitTime;
};

struct encoder_init_t xEncoderInitStruct = {.ubInitMode = 0, .ubInitState = STATE_NOTHING_TO_DO, .usStartVoltage = 6000, .ulInitWaitTime = 1000};

uint64_t ullActualSystick = 0;

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
void tmc4671_set_velocity_limit(uint32_t ulVelocityLimit)
{
	tmc4671_write(TMC4671_PID_VELOCITY_LIMIT, ulVelocityLimit);
}
void tmc4671_set_accelaration_limit(uint32_t ulAccelarationLimit)
{
	tmc4671_write(TMC4671_PID_ACCELERATION_LIMIT, ulAccelarationLimit);
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
static void tmc4671_encoder_init_task()
{
    if(xEncoderInitStruct.ubInitState != STATE_NOTHING_TO_DO)
    {
        static uint16_t usLastPhiESelection = 0;
        static uint32_t ulLastUQUDEXT = 0;
        static int16_t usLastPHIEEXT = 0;

        static uint64_t ullLastSysTick = 0;

        static int16_t sHallPhiEOld = 0;
        static int16_t sHallPhiENew = 0;
        static int16_t sActualCoarseOffset = 0;

        switch (xEncoderInitStruct.ubInitMode)
        {
        case MODE_ENC_ESTIMATE: // estimate offset
            switch(xEncoderInitStruct.ubInitState)
            {
            case STATE_START_INIT: // started by writing 1 to initState

                // save actual set values for PHI_E_SELECTION, UQ_UD_EXT, and PHI_E_EXT
                usLastPhiESelection = (uint16_t)(tmc4671_read(TMC4671_PHI_E_SELECTION) & TMC4671_PHI_E_MASK);
                ulLastUQUDEXT = (uint32_t)tmc4671_read(TMC4671_UQ_UD_EXT);
                usLastPHIEEXT = (int16_t)(tmc4671_read(TMC4671_PHI_E_EXT) & TMC4671_PHI_E_EXT_MASK);

                // set ABN_DECODER_PHI_E_OFFSET to zero
                tmc4671_modify(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0, TMC4671_ABN_DECODER_PHI_E_OFFSET_MASK);

                // select phi_e_ext
                tmc4671_modify(TMC4671_PHI_E_SELECTION, TMC4671_PHI_E_SELECTION_PHI_E_EXT, TMC4671_PHI_E_SELECTION_MASK);

                // set an initialization voltage on UD_EXT (to the flux, not the torque!)
                tmc4671_write(TMC4671_UQ_UD_EXT, xEncoderInitStruct.usStartVoltage & TMC4671_UD_EXT_MASK);

                // set the "zero" angle
                tmc4671_modify(TMC4671_PHI_E_EXT, 0, TMC4671_PHI_E_EXT_MASK);

                ullLastSysTick = ullActualSystick;

                xEncoderInitStruct.ubInitState = STATE_WAIT_INIT_TIME;
                break;

            case STATE_WAIT_INIT_TIME:
                // wait until initialization time is over (until no more vibration on the motor)
                if(ullActualSystick >= (ullLastSysTick + xEncoderInitStruct.ulInitWaitTime))
                {
                    // set internal encoder value to zero
                    tmc4671_write(TMC4671_ABN_DECODER_COUNT, 0);

                    // switch back to last used UQ_UD_EXT setting
                    tmc4671_write(TMC4671_UQ_UD_EXT, ulLastUQUDEXT);

                    // set PHI_E_EXT back to last value
                    tmc4671_modify(TMC4671_PHI_E_EXT, usLastPHIEEXT, TMC4671_PHI_E_EXT_MASK);

                    // switch back to last used PHI_E_SELECTION setting
                    tmc4671_modify(TMC4671_PHI_E_SELECTION, usLastPhiESelection, TMC4671_PHI_E_SELECTION_MASK);

                    // go to next state
                    xEncoderInitStruct.ubInitState = STATE_ESTIMATE_OFFSET;
                }
                break;

            case STATE_ESTIMATE_OFFSET:
                // you can do offset estimation here (wait for N-Channel if available and save encoder value)

                // go to ready state
                xEncoderInitStruct.ubInitState = STATE_NOTHING_TO_DO;
                break;

            default:
                xEncoderInitStruct.ubInitState = STATE_NOTHING_TO_DO;
                break;
            }
            break;

        case MODE_ENC_USE_HALL: // use hall sensors
            switch(xEncoderInitStruct.ubInitState)
            {
            case STATE_START_INIT: // started by writing 1 to initState

                // turn hall_mode interpolation off (read, clear bit 8, write back)
                tmc4671_modify(TMC4671_HALL_MODE, 0, BIT(8));

                // set ABN_DECODER_PHI_E_OFFSET to zero
                tmc4671_modify(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0, TMC4671_ABN_DECODER_PHI_E_OFFSET_MASK);

                // read actual hall angle
                sHallPhiEOld = (int16_t)(tmc4671_read(TMC4671_HALL_PHI_E_INTERPOLATED_PHI_E) & TMC4671_HALL_PHI_E_MASK);

                // read actual abn_decoder angle and compute difference to actual hall angle
                sActualCoarseOffset = sHallPhiEOld - (int16_t)((tmc4671_read(TMC4671_ABN_DECODER_PHI_E_PHI_M) & TMC4671_ABN_DECODER_PHI_E_MASK) >> TMC4671_ABN_DECODER_PHI_E_SHIFT);

                // set ABN_DECODER_PHI_E_OFFSET to actual hall-abn-difference, to use the actual hall angle for coarse initialization
                tmc4671_modify(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, sActualCoarseOffset << TMC4671_ABN_DECODER_PHI_E_OFFSET_SHIFT, TMC4671_ABN_DECODER_PHI_E_OFFSET_MASK);

                xEncoderInitStruct.ubInitState = STATE_WAIT_INIT_TIME;
                break;

            case STATE_WAIT_INIT_TIME:
                // read actual hall angle
                sHallPhiENew = (int16_t)(tmc4671_read(TMC4671_HALL_PHI_E_INTERPOLATED_PHI_E) & TMC4671_HALL_PHI_E_MASK);

                // wait until hall angle changed
                if(sHallPhiEOld != sHallPhiENew)
                {
                    // estimated value = old value + diff between old and new (handle int16_t overrun)
                    int16_t sHallPhiEEstimated = sHallPhiEOld + (sHallPhiENew - sHallPhiEOld) / 2;

                    // read actual abn_decoder angle and consider last set abn_decoder_offset
                    int16_t sAbnPhiEActual = (int16_t)(tmc4671_read(TMC4671_ABN_DECODER_PHI_E_PHI_M) >> TMC4671_ABN_DECODER_PHI_E_SHIFT) - sActualCoarseOffset;

                    // set ABN_DECODER_PHI_E_OFFSET to actual estimated angle - sAbnPhiEActual difference
                    tmc4671_modify(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, (sHallPhiEEstimated - sAbnPhiEActual) << TMC4671_ABN_DECODER_PHI_E_SHIFT, TMC4671_ABN_DECODER_PHI_E_MASK);

                    // go to ready state
                    xEncoderInitStruct.ubInitState = STATE_NOTHING_TO_DO;
                }
                break;

            default:
                xEncoderInitStruct.ubInitState = STATE_NOTHING_TO_DO;
                break;
            }
            break;

        default:
            xEncoderInitStruct.ubInitState = STATE_NOTHING_TO_DO;
            break;
        }
    }
}
void tmc4671_start_encoder_initialization(uint8_t ubMode, uint32_t ulInitWaitTime, uint16_t usStartVoltage)
{
	// allow only a new initialization if no actual initialization is running
	if(xEncoderInitStruct.ubInitState == STATE_NOTHING_TO_DO)
	{
        // set voltage
        xEncoderInitStruct.usStartVoltage = usStartVoltage;

        // set initialization time
        xEncoderInitStruct.ulInitWaitTime = ulInitWaitTime;

		// set mode
		xEncoderInitStruct.ubInitMode = ubMode;

		// start initialization
		xEncoderInitStruct.ubInitState = STATE_START_INIT;
	}
}
void tmc4671_update_phi_selection_and_initialize(uint8_t ubDesiredPhiESelection, uint8_t ubInitMode)
{
	if((tmc4671_read(TMC4671_PHI_E_SELECTION) & TMC4671_PHI_E_SELECTION_MASK) != ubDesiredPhiESelection)
	{
		tmc4671_write(TMC4671_PHI_E_SELECTION, ubDesiredPhiESelection & TMC4671_PHI_E_SELECTION_MASK);

		switch(ubDesiredPhiESelection)
        {
        case 3:
            tmc4671_start_encoder_initialization(ubInitMode, 1000, 6000);
            break;

        }
	}
}

void tmc4671_task()
{
    ullActualSystick++;
    tmc4671_encoder_init_task();
}

void tmc4671_disable_PWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, TMC4671_PWM_OFF, TMC4671_PWM_CHOP_MASK);
}
void tmc4671_enable_PWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, BIT(8), TMC4671_PWM_SV_MASK);
}
void tmc4671_disable_SVPWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, 0, TMC4671_PWM_SV_MASK);
}
void tmc4671_enable_SVPWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, TMC4671_PWM_CENTERED, TMC4671_PWM_CHOP_MASK);
}
void tmc4671_set_PWM_freq(uint32_t ulFrequency)
{
	tmc4671_write(TMC4671_PWM_MAXCNT, ((100000000 / ulFrequency) - 1) & TMC4671_PWM_MAXCNT_MASK);
}
uint32_t tmc4671_get_PWM_freq()
{
    return (100000000 / ((tmc4671_read(TMC4671_PWM_MAXCNT) & TMC4671_PWM_MAXCNT_MASK) + 1));
}
void tmc4671_set_pwm_polarity(uint8_t ubLowSideInverted, uint8_t ubHighSideInverted)
{
    tmc4671_write(TMC4671_PWM_POLARITIES, (ubLowSideInverted & TMC4671_PWM_POLARITIES_0_MASK) | ((ubLowSideInverted << TMC4671_PWM_POLARITIES_1_SHIFT) & TMC4671_PWM_POLARITIES_1_MASK));
}
void tmc4671_set_dead_time(uint8_t ubLowDeadTime, uint8_t ubHighDeadTime) // 10ns steps 255 max (2.55 us)
{
    tmc4671_write(TMC4671_PWM_BBM_H_BBM_L, (ubLowDeadTime & TMC4671_PWM_BBM_L_MASK) | ((ubHighDeadTime << TMC4671_PWM_BBM_H_SHIFT) & TMC4671_PWM_BBM_H_MASK));
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

#ifdef __cplusplus
}
#endif