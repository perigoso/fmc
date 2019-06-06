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
void tmc4671_set_torque_flux_limit(uint16_t usTorqueFluxLimit)
{
    tmc4671_write(TMC4671_PID_TORQUE_FLUX_LIMITS, usTorqueFluxLimit & TMC4671_PID_TORQUE_FLUX_LIMITS_MASK);
}
void tmc4671_set_uq_ud_limit(uint16_t usUqUdLimit)
{
    tmc4671_write(TMC4671_PIDOUT_UQ_UD_LIMITS, usUqUdLimit & TMC4671_PIDOUT_UQ_UD_LIMITS_MASK);
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
void tmc4671_encoder_initialize(uint16_t usStartVoltage)
{
    printf("do Encoder initialization (Mode 0)\n\r");
	// do Encoder initialization (Mode 0)
	tmc4671_write(TMC4671_MODE_RAMP_MODE_MOTION, TMC4671_MOTION_MODE_UQ_UD_EXT);	 	// use UQ_UD_EXT for motion
	tmc4671_write(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0);	 	// set ABN_DECODER_PHI_E_OFFSET to zero
	tmc4671_write(TMC4671_PHI_E_SELECTION, TMC4671_PHI_E_SELECTION_PHI_E_EXT);					// select phi_e_ext
	tmc4671_write(TMC4671_PHI_E_EXT, 0);							// set the "zero" angle
	tmc4671_write(TMC4671_UQ_UD_EXT, usStartVoltage);		// set an initialization voltage on UD_EXT (to the flux, not the torque!)
	tmc4671_write(TMC4671_PID_POSITION_ACTUAL, 0);				// critical: needed to set ABN_DECODER_COUNT to zero

	uint32_t reply;

    printf("clear DECODER_COUNT and check\n\r");
	// clear DECODER_COUNT and check
	do
	{
		// set internal encoder value to zero
		tmc4671_write(TMC4671_ABN_DECODER_COUNT, 0);
		reply = tmc4671_read(TMC4671_ABN_DECODER_COUNT);
	}while(reply != 0);

	// use encoder
	tmc4671_write(TMC4671_PHI_E_SELECTION, TMC4671_PHI_E_SELECTION_PHI_E_ABN); // select phi_abn for rotor position angle phi_e

	// switch to velocity mode
	tmc4671_write(TMC4671_MODE_RAMP_MODE_MOTION, TMC4671_MODE_RAMP_MODE_MOTION);
	tmc4671_write(TMC4671_PID_VELOCITY_TARGET, 10);
    sleep(1);

	// and search the N-Channel to clear the actual position
	tmc4671_write(TMC4671_ABN_DECODER_PHI_E_PHI_M, 0);
	tmc4671_write(TMC4671_ABN_DECODER_MODE, 0);

	tmc4671_write(TMC4671_PID_VELOCITY_TARGET, 20); // test
    sleep(1);

	// clear DECODER_COUNT_N and check
	printf("clear DECODER_COUNT_N and check\n\r");
    do
	{
		tmc4671_write(TMC4671_ABN_DECODER_COUNT_N, 0);
		reply = tmc4671_read(TMC4671_ABN_DECODER_COUNT_N);
	}while(reply != 0);

	tmc4671_write(TMC4671_PID_VELOCITY_TARGET, 30); // test
    sleep(1);

	// search N-channel
    printf("search N-channel\n\r");
	do
	{
		reply = tmc4671_read(TMC4671_ABN_DECODER_COUNT_N);
		tmc4671_write(TMC4671_PID_POSITION_ACTUAL, 0);
	}while(reply == 0);

	tmc4671_write(TMC4671_PID_VELOCITY_TARGET, 5); // test
    sleep(1);

	// drive to zero position using position mode
	//tmc4671_write(TMC4671_PID_POSITION_TARGET, 0);
	//tmc4671_write(TMC4671_MODE_RAMP_MODE_MOTION, TMC4671_MOTION_MODE_POSITION);
}
void tmc4671_update_phi_selection_and_initialize(uint8_t ubDesiredPhiESelection)
{
	if((tmc4671_read(TMC4671_PHI_E_SELECTION) & TMC4671_PHI_E_SELECTION_MASK) != ubDesiredPhiESelection)
	{
		tmc4671_write(TMC4671_PHI_E_SELECTION, ubDesiredPhiESelection & TMC4671_PHI_E_SELECTION_MASK);

		switch(ubDesiredPhiESelection)
        {
        case TMC4671_PHI_E_SELECTION_PHI_E_ABN:
            tmc4671_encoder_initialize(6000);
            break;
        }
	}
}


void tmc4671_disable_PWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, TMC4671_PWM_OFF, TMC4671_PWM_CHOP_MASK);
}
void tmc4671_enable_PWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, TMC4671_PWM_CENTERED, TMC4671_PWM_CHOP_MASK);
}
void tmc4671_disable_SVPWM()
{
	tmc4671_modify(TMC4671_PWM_SV_CHOP, 0, TMC4671_PWM_SV_MASK);
}
void tmc4671_enable_SVPWM()
{
    tmc4671_modify(TMC4671_PWM_SV_CHOP, BIT(8), TMC4671_PWM_SV_MASK);
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

uint8_t tmc4671_get_motor_type()
{
	return tmc4671_read(TMC4671_MOTOR_TYPE_N_POLE_PAIRS) & TMC4671_N_POLE_PAIRS_MASK;
}
void tmc4671_set_motor_type(uint8_t ubMotorType)
{
	tmc4671_modify(TMC4671_MOTOR_TYPE_N_POLE_PAIRS, (ubMotorType << TMC4671_MOTOR_TYPE_SHIFT), TMC4671_MOTOR_TYPE_MASK);
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

uint32_t tmc4671_get_adc_raw(uint8_t ubAdcRawAddr)
{
    // get value
	tmc4671_write(TMC4671_ADC_RAW_ADDR, ubAdcRawAddr & 0xFF);
	return tmc4671_read(TMC4671_ADC_RAW_DATA);
}

float tmc4671_get_vm_v()
{
    const float BATTERY_ADC_MAX = 61485.f;
    const float BATTERY_ADC_MIN = 38825.f;
    const float BATTERY_MAX = 62.f;

    return ((float)(tmc4671_get_adc_raw(TMC4671_ADC_RAW_ADDR_ADC_AGPI_A_RAW_ADC_VM_RAW) & TMC4671_ADC_VM_RAW_MASK) - BATTERY_ADC_MIN) * (BATTERY_MAX / (BATTERY_ADC_MAX - BATTERY_ADC_MIN));
}

uint16_t tmc4671_get_I0_raw()
{
    return tmc4671_get_adc_raw(TMC4671_ADC_RAW_ADDR_ADC_I1_RAW_ADC_I0_RAW) & TMC4671_ADC_I0_RAW_MASK;
}

uint16_t tmc4671_get_I1_raw()
{
	return (tmc4671_get_adc_raw(TMC4671_ADC_RAW_ADDR_ADC_I1_RAW_ADC_I0_RAW) & TMC4671_ADC_I1_RAW_MASK) >> TMC4671_ADC_I1_RAW_SHIFT;
}

#ifdef __cplusplus
}
#endif