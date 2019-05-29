#ifndef __TMC4671_H
#define __TMC4671_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "cp2130.h"

// TMC4671 Frame
#define TMC4671_FRAME_WRITE     0x80
#define TMC4671_FRAME_READ      0x00

// TMC4671 Register Address
#define TMC4671_CHIPINFO_DATA                      0x00
#define TMC4671_CHIPINFO_ADDR                      0x01

#define TMC4671_ADC_RAW_DATA                       0x02
#define TMC4671_ADC_RAW_ADDR                       0x03

#define TMC4671_dsADC_MCFG_B_MCFG_A                0x04
#define TMC4671_dsADC_MCLK_A                       0x05
#define TMC4671_dsADC_MCLK_B                       0x06
#define TMC4671_dsADC_MDEC_B_MDEC_A                0x07

#define TMC4671_ADC_I1_SCALE_OFFSET                0x08
#define TMC4671_ADC_I0_SCALE_OFFSET                0x09
#define TMC4671_ADC_I_SELECT                       0x0A
#define TMC4671_ADC_I1_I0_EXT                      0x0B

#define TMC4671_DS_ANALOG_INPUT_STAGE_CFG          0x0C

#define TMC4671_AENC_0_SCALE_OFFSET                0x0D
#define TMC4671_AENC_1_SCALE_OFFSET                0x0E
#define TMC4671_AENC_2_SCALE_OFFSET                0x0F

#define TMC4671_AENC_SELECT                        0x11

#define TMC4671_ADC_IWY_IUX                        0x12
#define TMC4671_ADC_IV                             0x13
#define TMC4671_AENC_WY_UX                         0x15
#define TMC4671_AENC_VN                            0x16

#define TMC4671_PWM_POLARITIES                     0x17
#define TMC4671_PWM_MAXCNT                         0x18
#define TMC4671_PWM_BBM_H_BBM_L                    0x19
#define TMC4671_PWM_SV_CHOP                        0x1A
#define TMC4671_MOTOR_TYPE_N_POLE_PAIRS            0x1B

#define TMC4671_PHI_E_EXT                          0x1C
#define TMC4671_PHI_M_EXT                          0x1D
#define TMC4671_POSITION_EXT                       0x1E
#define TMC4671_OPENLOOP_MODE                      0x1F
#define TMC4671_OPENLOOP_ACCELERATION              0x20
#define TMC4671_OPENLOOP_VELOCITY_TARGET           0x21
#define TMC4671_OPENLOOP_VELOCITY_ACTUAL           0x22
#define TMC4671_OPENLOOP_PHI                       0x23
#define TMC4671_UQ_UD_EXT                          0x24

#define TMC4671_ABN_DECODER_MODE                   0x25
#define TMC4671_ABN_DECODER_PPR                    0x26
#define TMC4671_ABN_DECODER_COUNT                  0x27
#define TMC4671_ABN_DECODER_COUNT_N                0x28
#define TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET     0x29
#define TMC4671_ABN_DECODER_PHI_E_PHI_M            0x2A

#define TMC4671_ABN_2_DECODER_MODE                 0x2C
#define TMC4671_ABN_2_DECODER_PPR                  0x2D
#define TMC4671_ABN_2_DECODER_COUNT                0x2E
#define TMC4671_ABN_2_DECODER_COUNT_N              0x2F
#define TMC4671_ABN_2_DECODER_PHI_M_OFFSET         0x30
#define TMC4671_ABN_2_DECODER_PHI_M                0x31

#define TMC4671_HALL_MODE                          0x33
#define TMC4671_HALL_POSITION_060_000              0x34
#define TMC4671_HALL_POSITION_180_120              0x35
#define TMC4671_HALL_POSITION_300_240              0x36
#define TMC4671_HALL_PHI_E_PHI_M_OFFSET            0x37
#define TMC4671_HALL_DPHI_MAX                      0x38
#define TMC4671_HALL_PHI_E_INTERPOLATED_PHI_E      0x39
#define TMC4671_HALL_PHI_M                         0x3A

#define TMC4671_AENC_DECODER_MODE                  0x3B
#define TMC4671_AENC_DECODER_N_MASK_N_THRESHOLD    0x3C
#define TMC4671_AENC_DECODER_PHI_A_RAW             0x3D
#define TMC4671_AENC_DECODER_PHI_A_OFFSET          0x3E
#define TMC4671_AENC_DECODER_PHI_A                 0x3F

#define TMC4671_AENC_DECODER_PPR                   0x40
#define TMC4671_AENC_DECODER_COUNT                 0x41
#define TMC4671_AENC_DECODER_COUNT_N               0x42
#define TMC4671_AENC_DECODER_PHI_E_PHI_M_OFFSET    0x45
#define TMC4671_AENC_DECODER_PHI_E_PHI_M           0x46
#define TMC4671_AENC_DECODER_POSITION              0x47

#define TMC4671_PIDIN_TORQUE_TARGET_FLUX_TARGET    0x4A
#define TMC4671_PIDIN_VELOCITY_TARGET              0x4B
#define TMC4671_PIDIN_POSITION_TARGET              0x4C

#define TMC4671_CONFIG_DATA                        0x4D
#define TMC4671_CONFIG_ADDR                        0x4E

#define TMC4671_VELOCITY_SELECTION                 0x50
#define TMC4671_POSITION_SELECTION                 0x51
#define TMC4671_PHI_E_SELECTION                    0x52
#define TMC4671_PHI_E                              0x53

#define TMC4671_PID_FLUX_P_FLUX_I                  0x54
#define TMC4671_PID_TORQUE_P_TORQUE_I              0x56
#define TMC4671_PID_VELOCITY_P_VELOCITY_I          0x58
#define TMC4671_PID_POSITION_P_POSITION_I          0x5A
#define TMC4671_PID_TORQUE_FLUX_TARGET_DDT_LIMITS  0x5C
#define TMC4671_PIDOUT_UQ_UD_LIMITS                0x5D
#define TMC4671_PID_TORQUE_FLUX_LIMITS             0x5E
#define TMC4671_PID_ACCELERATION_LIMIT             0x5F
#define TMC4671_PID_VELOCITY_LIMIT                 0x60
#define TMC4671_PID_POSITION_LIMIT_LOW             0x61
#define TMC4671_PID_POSITION_LIMIT_HIGH            0x62

#define TMC4671_MODE_RAMP_MODE_MOTION              0x63
#define TMC4671_PID_TORQUE_FLUX_TARGET             0x64
#define TMC4671_PID_TORQUE_FLUX_OFFSET             0x65
#define TMC4671_PID_VELOCITY_TARGET                0x66
#define TMC4671_PID_VELOCITY_OFFSET                0x67
#define TMC4671_PID_POSITION_TARGET                0x68

#define TMC4671_PID_TORQUE_FLUX_ACTUAL             0x69
#define TMC4671_PID_VELOCITY_ACTUAL                0x6A
#define TMC4671_PID_POSITION_ACTUAL                0x6B

#define TMC4671_PID_ERROR_DATA                     0x6C
#define TMC4671_PID_ERROR_ADDR                     0x6D
#define TMC4671_INTERIM_DATA                       0x6E
#define TMC4671_INTERIM_ADDR                       0x6F

#define TMC4671_WATCHDOG_CFG                       0x74
#define TMC4671_ADC_VM_LIMITS                      0x75
#define TMC4671_INPUTS_RAW                         0x76
#define TMC4671_OUTPUTS_RAW                        0x77

#define TMC4671_STEP_WIDTH                         0x78

#define TMC4671_UART_BPS                           0x79
#define TMC4671_UART_ADDRS                         0x7A

#define TMC4671_GPIO_dsADCI_CONFIG                 0x7B

#define TMC4671_STATUS_FLAGS                       0x7C
#define TMC4671_STATUS_MASK                        0x7D

// motion modes
#define TMC4671_MOTION_MODE_STOPPED    0
#define TMC4671_MOTION_MODE_TORQUE     1
#define TMC4671_MOTION_MODE_VELOCITY   2
#define TMC4671_MOTION_MODE_POSITION   3
#define TMC4671_MOTION_MODE_UQ_UD_EXT  8

// phi_e selections
#define TMC4671_PHI_E_EXTERNAL   1
#define TMC4671_PHI_E_OPEN_LOOP  2
#define TMC4671_PHI_E_ABN        3
#define TMC4671_PHI_E_HALL       5
#define TMC4671_PHI_E_AENC       6
#define TMC4671_PHI_A_AENC       7

// velocity/position selection
#define TMC4671_VELOCITY_PHI_E_SELECTION	0
#define TMC4671_VELOCITY_PHI_E_EXT			1
#define TMC4671_VELOCITY_PHI_E_OPENLOOP		2
#define TMC4671_VELOCITY_PHI_E_ABN			3

#define TMC4671_VELOCITY_PHI_E_HAL			5
#define TMC4671_VELOCITY_PHI_E_AENC			6
#define TMC4671_VELOCITY_PHI_A_AENC			7

#define TMC4671_VELOCITY_PHI_M_ABN			9
#define TMC4671_VELOCITY_PHI_M_ABN_2		10
#define TMC4671_VELOCITY_PHI_M_AENC			11
#define TMC4671_VELOCITY_PHI_M_HAL	12

// Variants
#define TMC4671_CHIPINFO_ADDR_SI_TYPE                                   0
#define TMC4671_CHIPINFO_ADDR_SI_VERSION                                1
#define TMC4671_CHIPINFO_ADDR_SI_DATA                                   2
#define TMC4671_CHIPINFO_ADDR_SI_TIME                                   3
#define TMC4671_CHIPINFO_ADDR_SI_VARIANT                                4
#define TMC4671_CHIPINFO_ADDR_SI_BUILD                                  5
#define TMC4671_ADC_RAW_ADDR_ADC_I1_RAW_ADC_I0_RAW                      0
#define TMC4671_ADC_RAW_ADDR_ADC_AGPI_A_RAW_ADC_VM_RAW                  1
#define TMC4671_ADC_RAW_ADDR_ADC_AENC_UX_RAW_ADC_AGPI_B_RAW             2
#define TMC4671_ADC_RAW_ADDR_ADC_AENC_WY_RAW_ADC_AENC_VN_RAW            3
#define TMC4671_CONFIG_ADDR_biquad_x_a_1                                1
#define TMC4671_CONFIG_ADDR_biquad_x_a_2                                2
#define TMC4671_CONFIG_ADDR_biquad_x_b_0                                4
#define TMC4671_CONFIG_ADDR_biquad_x_b_1                                5
#define TMC4671_CONFIG_ADDR_biquad_x_b_2                                6
#define TMC4671_CONFIG_ADDR_biquad_x_enable                             7
#define TMC4671_CONFIG_ADDR_biquad_v_a_1                                9
#define TMC4671_CONFIG_ADDR_biquad_v_a_2                                10
#define TMC4671_CONFIG_ADDR_biquad_v_b_0                                12
#define TMC4671_CONFIG_ADDR_biquad_v_b_1                                13
#define TMC4671_CONFIG_ADDR_biquad_v_b_2                                14
#define TMC4671_CONFIG_ADDR_biquad_v_enable                             15
#define TMC4671_CONFIG_ADDR_biquad_t_a_1                                17
#define TMC4671_CONFIG_ADDR_biquad_t_a_2                                18
#define TMC4671_CONFIG_ADDR_biquad_t_b_0                                20
#define TMC4671_CONFIG_ADDR_biquad_t_b_1                                21
#define TMC4671_CONFIG_ADDR_biquad_t_b_2                                22
#define TMC4671_CONFIG_ADDR_biquad_t_enable                             23
#define TMC4671_CONFIG_ADDR_biquad_f_a_1                                25
#define TMC4671_CONFIG_ADDR_biquad_f_a_2                                26
#define TMC4671_CONFIG_ADDR_biquad_f_b_0                                28
#define TMC4671_CONFIG_ADDR_biquad_f_b_1                                29
#define TMC4671_CONFIG_ADDR_biquad_f_b_2                                30
#define TMC4671_CONFIG_ADDR_biquad_f_enable                             31
#define TMC4671_CONFIG_ADDR_prbs_amplitude                              32
#define TMC4671_CONFIG_ADDR_prbs_down_sampling_ratio                    33
#define TMC4671_CONFIG_ADDR_feed_forward_velocity_gain                  40
#define TMC4671_CONFIG_ADDR_feed_forward_velicity_filter_constant       41
#define TMC4671_CONFIG_ADDR_feed_forward_torque_gain                    42
#define TMC4671_CONFIG_ADDR_feed_forward_torgue_filter_constant         43
#define TMC4671_CONFIG_ADDR_VELOCITY_METER_PPTM_MIN_POS_DEV             50
#define TMC4671_CONFIG_ADDR_ref_switch_config                           51
#define TMC4671_CONFIG_ADDR_Encoder_Init_hall_Enable                    52
#define TMC4671_CONFIG_ADDR_SINGLE_PIN_IF_STATUS_CFG                    60
#define TMC4671_CONFIG_ADDR_SINGLE_PIN_IF_SCALE_OFFSET                  61
#define TMC4671_PID_ERROR_ADDR_PID_TORQUE_ERROR                         0
#define TMC4671_PID_ERROR_ADDR_PID_FLUX_ERROR                           1
#define TMC4671_PID_ERROR_ADDR_PID_VELOCITY_ERROR                       2
#define TMC4671_PID_ERROR_ADDR_PID_POSITION_ERROR                       3
#define TMC4671_PID_ERROR_ADDR_PID_TORQUE_ERROR_SUM                     4
#define TMC4671_PID_ERROR_ADDR_PID_FLUX_ERROR_SUM                       5
#define TMC4671_PID_ERROR_ADDR_PID_VELOCITY_ERROR_SUM                   6
#define TMC4671_PID_ERROR_ADDR_PID_POSITION_ERROR_SUM                   7
#define TMC4671_INTERIM_ADDR_PIDIN_TARGET_TORQUE                        0
#define TMC4671_INTERIM_ADDR_PIDIN_TARGET_FLUX                          1
#define TMC4671_INTERIM_ADDR_PIDIN_TARGET_VELOCITY                      2
#define TMC4671_INTERIM_ADDR_PIDIN_TARGET_POSITION                      3
#define TMC4671_INTERIM_ADDR_PIDOUT_TARGET_TORQUE                       4
#define TMC4671_INTERIM_ADDR_PIDOUT_TARGET_FLUX                         5
#define TMC4671_INTERIM_ADDR_PIDOUT_TARGET_VELOCITY                     6
#define TMC4671_INTERIM_ADDR_PIDOUT_TARGET_POSITION                     7
#define TMC4671_INTERIM_ADDR_FOC_IWY_IUX                                8
#define TMC4671_INTERIM_ADDR_FOC_IV                                     9
#define TMC4671_INTERIM_ADDR_FOC_IB_IA                                  10
#define TMC4671_INTERIM_ADDR_FOC_IQ_ID                                  11
#define TMC4671_INTERIM_ADDR_FOC_UQ_UD                                  12
#define TMC4671_INTERIM_ADDR_FOC_UQ_UD_LIMITED                          13
#define TMC4671_INTERIM_ADDR_FOC_UB_UA                                  14
#define TMC4671_INTERIM_ADDR_FOC_UWY_UUX                                15
#define TMC4671_INTERIM_ADDR_FOC_UV                                     16
#define TMC4671_INTERIM_ADDR_PWM_WY_UX                                  17
#define TMC4671_INTERIM_ADDR_PWM_UV                                     18
#define TMC4671_INTERIM_ADDR_ADC_I1_I0                                  19
#define TMC4671_INTERIM_ADDR_PID_TORQUE_TARGET_FLUX_TARGET_TORQUE_ACTUAL_FLUX_ACTUAL_DIV256 20
#define TMC4671_INTERIM_ADDR_PID_TORQUE_TARGET_TORQUE_ACTUAL            21
#define TMC4671_INTERIM_ADDR_PID_FLUX_TARGET_FLUX_ACTUAL                22
#define TMC4671_INTERIM_ADDR_PID_VELOCITY_TARGET_VELOCITY_ACTUAL_DIV256 23
#define TMC4671_INTERIM_ADDR_PID_VELOCITY_TARGET_VELOCITY_ACTUAL        24
#define TMC4671_INTERIM_ADDR_PID_POSITION_TARGET_POSITION_ACTUAL_DIV256 25
#define TMC4671_INTERIM_ADDR_PID_POSITION_TARGET_POSITION_ACTUAL        26
#define TMC4671_INTERIM_ADDR_FF_VELOCITY                                27
#define TMC4671_INTERIM_ADDR_FF_TORQUE                                  28
#define TMC4671_INTERIM_ADDR_ACTUAL_VELOCITY_PPTM                       29
#define TMC4671_INTERIM_ADDR_REF_SWITCH_STATUS                          30
#define TMC4671_INTERIM_ADDR_HOME_POSITION                              31
#define TMC4671_INTERIM_ADDR_LEFT_POSITION                              32
#define TMC4671_INTERIM_ADDR_RIGHT_POSITION                             33
#define TMC4671_INTERIM_ADDR_ENC_INIT_HALL_STATUS                       34
#define TMC4671_INTERIM_ADDR_ENC_INIT_HALL_PHI_E_ABN_OFFSET             35
#define TMC4671_INTERIM_ADDR_ENC_INIT_HALL_PHI_E_AENC_OFFSET            36
#define TMC4671_INTERIM_ADDR_ENC_INIT_HALL_PHI_A_AENC_OFFSET            37
#define TMC4671_INTERIM_ADDR_enc_init_mini_move_u_d_status              40
#define TMC4671_INTERIM_ADDR_enc_init_mini_move_phi_e_phi_e_offset      41
#define TMC4671_INTERIM_ADDR_SINGLE_PIN_IF_PWM_DUTY_CYCLE_TORQUE_TARGET 42
#define TMC4671_INTERIM_ADDR_SINGLE_PIN_IF_VELOCITY_TARGET              43
#define TMC4671_INTERIM_ADDR_SINGLE_PIN_IF_POSITION_TARGET              44
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_1_0                            192
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_3_2                            193
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_5_4                            194
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_7_6                            195
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_9_8                            196
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_11_10                          197
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_13_12                          198
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_15_14                          199
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_16                             200
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_17                             201
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_18                             202
#define TMC4671_INTERIM_ADDR_DEBUG_VALUE_19                             203
#define TMC4671_INTERIM_ADDR_CONFIG_REG_0                               208
#define TMC4671_INTERIM_ADDR_CONFIG_REG_1                               209
#define TMC4671_INTERIM_ADDR_CTRL_PARAM_10                              210
#define TMC4671_INTERIM_ADDR_CTRL_PARAM_32                              211
#define TMC4671_INTERIM_ADDR_STATUS_REG_0                               212
#define TMC4671_INTERIM_ADDR_STATUS_REG_1                               213
#define TMC4671_INTERIM_ADDR_STATUS_PARAM_10                            214
#define TMC4671_INTERIM_ADDR_STATUS_PARAM_32                            215
#define TMC4671_PWM_OFF                                                 0
#define TMC4671_PWM_OFF_LOW_SIDE_ON                                     1
#define TMC4671_PWM_OFF_HIGH_SIDE_ON                                    2
#define TMC4671_PWM_LOW_SIDE_ONLY                                       5
#define TMC4671_PWM_HIGH_SIDE_ONLY                                      6
#define TMC4671_PWM_CENTERED                                            7
#define TMC4671_PHI_E_SELECTION_PHI_E_EXT                               1
#define TMC4671_PHI_E_SELECTION_PHI_E_OPENLOOP                          2
#define TMC4671_PHI_E_SELECTION_PHI_E_ABN                               3
#define TMC4671_PHI_E_SELECTION_PHI_E_HAL                               5
#define TMC4671_PHI_E_SELECTION_PHI_E_AENC                              6
#define TMC4671_PHI_E_SELECTION_PHI_A_AENC                              7
#define TMC4671_POSITION_PHI_E_SELECTION	                            0
#define TMC4671_POSITION_PHI_E_EXT			                            1
#define TMC4671_POSITION_PHI_E_OPENLOOP		                            2
#define TMC4671_POSITION_PHI_E_ABN			                            3
#define TMC4671_POSITION_PHI_E_HAL			                            5
#define TMC4671_POSITION_PHI_E_AENC			                            6
#define TMC4671_POSITION_PHI_A_AENC			                            7
#define TMC4671_POSITION_PHI_M_ABN			                            9
#define TMC4671_POSITION_PHI_M_ABN_2		                            10
#define TMC4671_POSITION_PHI_M_AENC			                            11
#define TMC4671_POSITION_PHI_M_HAL			                            12

// Masks & Shifts
#define TMC4671_SI_TYPE_MASK                                 0xFFFFFFFF // CHIPINFO_DATA //
#define TMC4671_SI_TYPE_SHIFT                                0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_SI_VERSION_MASK                              0xFFFFFFFF // CHIPINFO_DATA //
#define TMC4671_SI_VERSION_SHIFT                             0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_SI_DATE_MASK                                 0xFFFFFFFF // CHIPINFO_DATA //
#define TMC4671_SI_DATE_SHIFT                                0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_SI_TIME_MASK                                 0xFFFFFFFF // CHIPINFO_DATA //
#define TMC4671_SI_TIME_SHIFT                                0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_SI_VARIANT_MASK                              0xFFFFFFFF // CHIPINFO_DATA //
#define TMC4671_SI_VARIANT_SHIFT                             0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_SI_BUILD_MASK                                0xFFFFFFFF // CHIPINFO_DATA //
#define TMC4671_SI_BUILD_SHIFT                               0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_CHIP_INFO_ADDRESS_MASK                       0xFF // CHIPINFO_ADDR //
#define TMC4671_CHIP_INFO_ADDRESS_SHIFT                      0 // min.: 0, max.: 5, default: 0
#define TMC4671_ADC_I0_RAW_MASK                              0xFFFF // ADC_RAW_DATA // Raw phase current I0
#define TMC4671_ADC_I0_RAW_SHIFT                             0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_I1_RAW_MASK                              0xFFFF0000 // ADC_RAW_DATA // Raw phase current I1
#define TMC4671_ADC_I1_RAW_SHIFT                             16 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_VM_RAW_MASK                              0xFFFF // ADC_RAW_DATA // aw supply voltage value.
#define TMC4671_ADC_VM_RAW_SHIFT                             0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_AGPI_A_RAW_MASK                          0xFFFF0000 // ADC_RAW_DATA // Raw analog gpi A value.
#define TMC4671_ADC_AGPI_A_RAW_SHIFT                         16 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_AGPI_B_RAW_MASK                          0xFFFF // ADC_RAW_DATA // Raw analog gpi B value.
#define TMC4671_ADC_AGPI_B_RAW_SHIFT                         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_AENC_UX_RAW_MASK                         0xFFFF0000 // ADC_RAW_DATA // Raw analog encoder signal.
#define TMC4671_ADC_AENC_UX_RAW_SHIFT                        16 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_AENC_VN_RAW_MASK                         0xFFFF // ADC_RAW_DATA // Raw analog encoder signal.
#define TMC4671_ADC_AENC_VN_RAW_SHIFT                        0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_AENC_WY_RAW_MASK                         0xFFFF0000 // ADC_RAW_DATA // Raw analog encoder signal.
#define TMC4671_ADC_AENC_WY_RAW_SHIFT                        16 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_RAW_ADDR_MASK                            0xFF // ADC_RAW_ADDR //
#define TMC4671_ADC_RAW_ADDR_SHIFT                           0 // min.: 0, max.: 3, default: 0
#define TMC4671_CFG_DSMODULATOR_A_MASK                       0x03 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_CFG_DSMODULATOR_A_SHIFT                      0 // min.: 0, max.: 3, default: 0
#define TMC4671_MCLK_POLARITY_A_MASK                         0x04 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_MCLK_POLARITY_A_SHIFT                        2 // min.: 0, max.: 1, default: 0
#define TMC4671_MDAT_POLARITY_A_MASK                         0x08 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_MDAT_POLARITY_A_SHIFT                        3 // min.: 0, max.: 1, default: 0
#define TMC4671_SEL_NCLK_MCLK_I_A_MASK                       0x10 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_SEL_NCLK_MCLK_I_A_SHIFT                      4 // min.: 0, max.: 1, default: 0
#define TMC4671_BLANKING_A_MASK                              0xFF00 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_BLANKING_A_SHIFT                             8 // min.: 0, max.: 255, default: 0
#define TMC4671_CFG_DSMODULATOR_B_MASK                       0x030000 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_CFG_DSMODULATOR_B_SHIFT                      16 // min.: 0, max.: 3, default: 0
#define TMC4671_MCLK_POLARITY_B_MASK                         0x040000 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_MCLK_POLARITY_B_SHIFT                        18 // min.: 0, max.: 1, default: 0
#define TMC4671_MDAT_POLARITY_B_MASK                         0x080000 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_MDAT_POLARITY_B_SHIFT                        19 // min.: 0, max.: 1, default: 0
#define TMC4671_SEL_NCLK_MCLK_I_B_MASK                       0x100000 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_SEL_NCLK_MCLK_I_B_SHIFT                      20 // min.: 0, max.: 1, default: 0
#define TMC4671_BLANKING_B_MASK                              0xFF000000 // dsADC_MCFG_B_MCFG_A //
#define TMC4671_BLANKING_B_SHIFT                             24 // min.: 0, max.: 255, default: 0
#define TMC4671_DSADC_MCLK_A_MASK                            0xFFFFFFFF // dsADC_MCLK_A // fMCLK_A = 2^31 / (fCLK * (dsADC_MCLK_A+1)), dsADC_MCLK_A = (2^31 / (fMCLK * fCLK)) - 1
#define TMC4671_DSADC_MCLK_A_SHIFT                           0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_DSADC_MCLK_B_MASK                            0xFFFFFFFF // dsADC_MCLK_B // fMCLK_B = 2^31 / (fCLK * (dsADC_MCLK_B+1)), dsADC_MCLK_B = (2^31 / (fMCLK * fCLK)) - 1
#define TMC4671_DSADC_MCLK_B_SHIFT                           0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_DSADC_MDEC_A_MASK                            0xFFFF // dsADC_MDEC_B_MDEC_A // 0: PWM synchron, others according to register content
#define TMC4671_DSADC_MDEC_A_SHIFT                           0 // min.: 0, max.: 65535, default: 0
#define TMC4671_DSADC_MDEC_B_MASK                            0xFFFF0000 // dsADC_MDEC_B_MDEC_A // 0: PWM synchron, others according to register content
#define TMC4671_DSADC_MDEC_B_SHIFT                           16 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_I1_OFFSET_MASK                           0xFFFF // ADC_I1_SCALE_OFFSET // Offset for current ADC channel 1.
#define TMC4671_ADC_I1_OFFSET_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_I1_SCALE_MASK                            0xFFFF0000 // ADC_I1_SCALE_OFFSET // Scaling factor for current ADC channel 1.
#define TMC4671_ADC_I1_SCALE_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ADC_I0_OFFSET_MASK                           0xFFFF // ADC_I0_SCALE_OFFSET // Offset for current ADC channel 0.
#define TMC4671_ADC_I0_OFFSET_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_I0_SCALE_MASK                            0xFFFF0000 // ADC_I0_SCALE_OFFSET // Scaling factor for current ADC channel 0.
#define TMC4671_ADC_I0_SCALE_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ADC_I0_SELECT_MASK                           0xFF // ADC_I_SELECT // Select input for raw current ADC_I0_RAW.
#define TMC4671_ADC_I0_SELECT_SHIFT                          0 // min.: 0, max.: 3, default: 0
#define TMC4671_ADC_I1_SELECT_MASK                           0xFF00 // ADC_I_SELECT // Select input for raw current ADC_I1_RAW.
#define TMC4671_ADC_I1_SELECT_SHIFT                          8 // min.: 0, max.: 3, default: 0
#define TMC4671_ADC_I_UX_SELECT_MASK                         0x03000000 // ADC_I_SELECT //
#define TMC4671_ADC_I_UX_SELECT_SHIFT                        24 // min.: 0, max.: 2, default: 0
#define TMC4671_ADC_I_V_SELECT_MASK                          0x0C000000 // ADC_I_SELECT //
#define TMC4671_ADC_I_V_SELECT_SHIFT                         26 // min.: 0, max.: 2, default: 0
#define TMC4671_ADC_I_WY_SELECT_MASK                         0x30000000 // ADC_I_SELECT //
#define TMC4671_ADC_I_WY_SELECT_SHIFT                        28 // min.: 0, max.: 2, default: 0
#define TMC4671_ADC_I0_EXT_MASK                              0xFFFF // ADC_I1_I0_EXT // Register for write of ADC_I0 value from external source (eg. CPU).
#define TMC4671_ADC_I0_EXT_SHIFT                             0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_I1_EXT_MASK                              0xFFFF0000 // ADC_I1_I0_EXT // Register for write of ADC_I1 value from external source (eg. CPU).
#define TMC4671_ADC_I1_EXT_SHIFT                             16 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_I0_MASK                                  0x0F // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_I0_SHIFT                                 0 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_I1_MASK                                  0xF0 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_I1_SHIFT                                 4 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_VW_MASK                                  0x0F00 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_VW_SHIFT                                 8 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_AGPI_A_MASK                              0xF000 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_AGPI_A_SHIFT                             12 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_AGPI_B_MASK                              0x0F0000 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_AGPI_B_SHIFT                             16 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_AENC_UX_MASK                             0xF00000 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_AENC_UX_SHIFT                            20 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_AENC_VN_MASK                             0x0F000000 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_AENC_VN_SHIFT                            24 // min.: 0, max.: 7, default: 0
#define TMC4671_ADC_AENC_WY_MASK                             0xF0000000 // DS_ANALOG_INPUT_STAGE_CFG //
#define TMC4671_ADC_AENC_WY_SHIFT                            28 // min.: 0, max.: 7, default: 0
#define TMC4671_AENC_0_OFFSET_MASK                           0xFFFF // AENC_0_SCALE_OFFSET // Offset for Analog Encoder ADC channel 0.
#define TMC4671_AENC_0_OFFSET_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_AENC_0_SCALE_MASK                            0xFFFF0000 // AENC_0_SCALE_OFFSET // Scaling factor for Analog Encoder ADC channel 0.
#define TMC4671_AENC_0_SCALE_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_1_OFFSET_MASK                           0xFFFF // AENC_1_SCALE_OFFSET // Offset for Analog Encoder ADC channel 1.
#define TMC4671_AENC_1_OFFSET_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_AENC_1_SCALE_MASK                            0xFFFF0000 // AENC_1_SCALE_OFFSET // Scaling factor for Analog Encoder ADC channel 1.
#define TMC4671_AENC_1_SCALE_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_2_OFFSET_MASK                           0xFFFF // AENC_2_SCALE_OFFSET // Offset for Analog Encoder ADC channel 2.
#define TMC4671_AENC_2_OFFSET_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_AENC_2_SCALE_MASK                            0xFFFF0000 // AENC_2_SCALE_OFFSET // Scaling factor for Analog Encoder ADC channel 2.
#define TMC4671_AENC_2_SCALE_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_0_SELECT_MASK                           0xFF // AENC_SELECT // Select analog encoder ADC channel for raw analog encoder signal AENC_0_RAW.
#define TMC4671_AENC_0_SELECT_SHIFT                          0 // min.: 0, max.: 2, default: 0
#define TMC4671_AENC_1_SELECT_MASK                           0xFF00 // AENC_SELECT // Select analog encoder ADC channel for raw analog encoder signal AENC_1_RAW.
#define TMC4671_AENC_1_SELECT_SHIFT                          8 // min.: 0, max.: 2, default: 0
#define TMC4671_AENC_2_SELECT_MASK                           0xFF0000 // AENC_SELECT // Select analog encoder ADC channel for raw analog encoder signal AENC_2_RAW.
#define TMC4671_AENC_2_SELECT_SHIFT                          16 // min.: 0, max.: 2, default: 0
#define TMC4671_ADC_IUX_MASK                                 0xFFFF // ADC_IWY_IUX // Register of scaled current ADC value including signed added offset as input for the FOC.
#define TMC4671_ADC_IUX_SHIFT                                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ADC_IWY_MASK                                 0xFFFF0000 // ADC_IWY_IUX // Register of scaled current ADC value including signed added offset as input for the FOC.
#define TMC4671_ADC_IWY_SHIFT                                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ADC_IV_MASK                                  0xFFFF // ADC_IV // Register of scaled current ADC value including signed added offset as input for the FOC.
#define TMC4671_ADC_IV_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_UX_MASK                                 0xFFFF // AENC_WY_UX // Register of scaled analog encoder value including signed added offset as input for the interpolator.
#define TMC4671_AENC_UX_SHIFT                                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_WY_MASK                                 0xFFFF0000 // AENC_WY_UX // Register of scaled analog encoder value including signed added offset as input for the interpolator.
#define TMC4671_AENC_WY_SHIFT                                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_VN_MASK                                 0xFFFF // AENC_VN // Register of scaled analog encoder value including signed added offset as input for the interpolator.
#define TMC4671_AENC_VN_SHIFT                                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PWM_POLARITIES_0_MASK                        0x01 // PWM_POLARITIES // polarity of Low Side (LS) gate control signal
#define TMC4671_PWM_POLARITIES_0_SHIFT                       0 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_1_MASK                        0x02 // PWM_POLARITIES // polarity of High Side (HS) gate control signal
#define TMC4671_PWM_POLARITIES_1_SHIFT                       1 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_2_MASK                        0x04 // PWM_POLARITIES // pulse AB polarity
#define TMC4671_PWM_POLARITIES_2_SHIFT                       2 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_3_MASK                        0x08 // PWM_POLARITIES // pulse B polarity
#define TMC4671_PWM_POLARITIES_3_SHIFT                       3 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_4_MASK                        0x10 // PWM_POLARITIES // pulse C center polarity
#define TMC4671_PWM_POLARITIES_4_SHIFT                       4 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_5_MASK                        0x20 // PWM_POLARITIES // pulse A polarity
#define TMC4671_PWM_POLARITIES_5_SHIFT                       5 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_6_MASK                        0x40 // PWM_POLARITIES // pulse zero Z pulse polarity
#define TMC4671_PWM_POLARITIES_6_SHIFT                       6 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_POLARITIES_7_MASK                        0x80 // PWM_POLARITIES // over current signal polarity
#define TMC4671_PWM_POLARITIES_7_SHIFT                       7 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_MAXCNT_MASK                              0xFFFF // PWM_MAXCNT // PWM maximum (count-1), PWM frequency is fPWM[Hz] = 100MHz/(PWM_MAXCNT+1)
#define TMC4671_PWM_MAXCNT_SHIFT                             0 // min.: 0, max.: 65535, default: 0
#define TMC4671_PWM_BBM_L_MASK                               0xFF // PWM_BBM_H_BBM_L // Break Before Make time tBBM_L[10ns] for low side MOS-FET gate control
#define TMC4671_PWM_BBM_L_SHIFT                              0 // min.: 0, max.: 255, default: 0
#define TMC4671_PWM_BBM_H_MASK                               0xFF00 // PWM_BBM_H_BBM_L // Break Before Make time tBBM_H[10ns] for high side MOS-FET gate control
#define TMC4671_PWM_BBM_H_SHIFT                              8 // min.: 0, max.: 255, default: 0
#define TMC4671_PWM_CHOP_MASK                                0xFF // PWM_SV_CHOP // PWM chopper mode, defining how to chopper
#define TMC4671_PWM_CHOP_SHIFT                               0 // min.: 0, max.: 7, default: 0
#define TMC4671_PWM_SV_MASK                                  0x0100 // PWM_SV_CHOP // use Space Vector PWM
#define TMC4671_PWM_SV_SHIFT                                 8 // min.: 0, max.: 1, default: 0
#define TMC4671_N_POLE_PAIRS_MASK                            0xFFFF // MOTOR_TYPE_N_POLE_PAIRS // Number n of pole pairs of the motor for calcualtion phi_e = phi_m / N_POLE_PAIRS.
#define TMC4671_N_POLE_PAIRS_SHIFT                           0 // min.: 1, max.: 65535, default: 0
#define TMC4671_MOTOR_TYPE_MASK                              0xFF0000 // MOTOR_TYPE_N_POLE_PAIRS //
#define TMC4671_MOTOR_TYPE_SHIFT                             16 // min.: 0, max.: 3, default: 0
#define TMC4671_PHI_E_EXT_MASK                               0xFFFF // PHI_E_EXT // Electrical angle phi_e_ext for external writing into this register.
#define TMC4671_PHI_E_EXT_SHIFT                              0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PHI_M_EXT_MASK                               0xFFFF // PHI_M_EXT // Mechanical angle phi_m_ext for external writing into this register.
#define TMC4671_PHI_M_EXT_SHIFT                              0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_POSITION_EXT_MASK                            0xFFFFFFFF // POSITION_EXT // Mechanical (multi turn) position for external writing into this register.
#define TMC4671_POSITION_EXT_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_OPENLOOP_PHI_DIRECTION_MASK                  0x1000 // OPENLOOP_MODE // Open loop phi direction.
#define TMC4671_OPENLOOP_PHI_DIRECTION_SHIFT                 12 // min.: 0, max.: 1, default: 0
#define TMC4671_OPENLOOP_ACCELERATION_MASK                   0xFFFFFFFF // OPENLOOP_ACCELERATION // Acceleration of open loop phi.
#define TMC4671_OPENLOOP_ACCELERATION_SHIFT                  0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_OPENLOOP_VELOCITY_TARGET_MASK                0xFFFFFFFF // OPENLOOP_VELOCITY_TARGET // Target velocity of open loop phi.
#define TMC4671_OPENLOOP_VELOCITY_TARGET_SHIFT               0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_OPENLOOP_VELOCITY_ACTUAL_MASK                0xFFFFFFFF // OPENLOOP_VELOCITY_ACTUAL // Actual velocity of open loop generator.
#define TMC4671_OPENLOOP_VELOCITY_ACTUAL_SHIFT               0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_OPENLOOP_PHI_MASK                            0xFFFF // OPENLOOP_PHI // Angle phi open loop (either mapped to electrical angel phi_e or mechanical angle phi_m).
#define TMC4671_OPENLOOP_PHI_SHIFT                           0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_UD_EXT_MASK                                  0xFFFF // UQ_UD_EXT // External writable parameter for open loop voltage control mode, usefull during system setup, U_D component.
#define TMC4671_UD_EXT_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_UQ_EXT_MASK                                  0xFFFF0000 // UQ_UD_EXT // External writable parameter for open loop voltage control mode, usefull during system setup, U_Q component.
#define TMC4671_UQ_EXT_SHIFT                                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_APOL_MASK                                    0x01 // ABN_DECODER_MODE // Polarity of A pulse.
#define TMC4671_APOL_SHIFT                                   0 // min.: 0, max.: 1, default: 0
#define TMC4671_BPOL_MASK                                    0x02 // ABN_DECODER_MODE // Polarity of B pulse.
#define TMC4671_BPOL_SHIFT                                   1 // min.: 0, max.: 1, default: 0
#define TMC4671_NPOL_MASK                                    0x04 // ABN_DECODER_MODE // Polarity of N pulse.
#define TMC4671_NPOL_SHIFT                                   2 // min.: 0, max.: 1, default: 0
#define TMC4671_USE_ABN_AS_N_MASK                            0x08 // ABN_DECODER_MODE // 0: Ignore A and B polarity with Npulse = N, 1 : Npulse = N and A and B
#define TMC4671_USE_ABN_AS_N_SHIFT                           3 // min.: 0, max.: 1, default: 0
#define TMC4671_CLN_MASK                                     0x0100 // ABN_DECODER_MODE // Clear writes ABN_DECODER_COUNT_N into decoder count at Npulse.
#define TMC4671_CLN_SHIFT                                    8 // min.: 0, max.: 1, default: 0
#define TMC4671_ABN_DIRECTION_MASK                           0x1000 // ABN_DECODER_MODE // Decoder count direction.
#define TMC4671_ABN_DIRECTION_SHIFT                          12 // min.: 0, max.: 1, default: 0
#define TMC4671_ABN_DECODER_PPR_MASK                         0xFFFFFF // ABN_DECODER_PPR // Decoder pules per mechanical revolution.
#define TMC4671_ABN_DECODER_PPR_SHIFT                        0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_ABN_DECODER_COUNT_MASK                       0xFFFFFF // ABN_DECODER_COUNT // Raw decoder count; the digital decoder engine counts modulo (decoder_ppr).
#define TMC4671_ABN_DECODER_COUNT_SHIFT                      0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_ABN_DECODER_COUNT_N_MASK                     0xFFFFFF // ABN_DECODER_COUNT_N // Decoder count latched on N pulse, when N pulse clears decoder_count also decoder_count_n is 0.
#define TMC4671_ABN_DECODER_COUNT_N_SHIFT                    0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_ABN_DECODER_PHI_M_OFFSET_MASK                0xFFFF // ABN_DECODER_PHI_E_PHI_M_OFFSET // ABN_DECODER_PHI_M_OFFSET to shift (rotate) angle DECODER_PHI_M.
#define TMC4671_ABN_DECODER_PHI_M_OFFSET_SHIFT               0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ABN_DECODER_PHI_E_OFFSET_MASK                0xFFFF0000 // ABN_DECODER_PHI_E_PHI_M_OFFSET // ABN_DECODER_PHI_E_OFFSET to shift (rotate) angle DECODER_PHI_E.
#define TMC4671_ABN_DECODER_PHI_E_OFFSET_SHIFT               16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ABN_DECODER_PHI_M_MASK                       0xFFFF // ABN_DECODER_PHI_E_PHI_M // ABN_DECODER_PHI_M = ABN_DECODER_COUNT * 2^16 / ABN_DECODER_PPR + ABN_DECODER_PHI_M_OFFSET;
#define TMC4671_ABN_DECODER_PHI_M_SHIFT                      0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ABN_DECODER_PHI_E_MASK                       0xFFFF0000 // ABN_DECODER_PHI_E_PHI_M // ABN_DECODER_PHI_E = (ABN_DECODER_PHI_M * N_POLE_PAIRS_) + ABN_DECODER_PHI_E_OFFSET
#define TMC4671_ABN_DECODER_PHI_E_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_APOL_MASK                                    0x01 // ABN_2_DECODER_MODE // Polarity of A pulse.
#define TMC4671_APOL_SHIFT                                   0 // min.: 0, max.: 1, default: 0
#define TMC4671_BPOL_MASK                                    0x02 // ABN_2_DECODER_MODE // Polarity of B pulse.
#define TMC4671_BPOL_SHIFT                                   1 // min.: 0, max.: 1, default: 0
#define TMC4671_NPOL_MASK                                    0x04 // ABN_2_DECODER_MODE // Polarity of N pulse.
#define TMC4671_NPOL_SHIFT                                   2 // min.: 0, max.: 1, default: 0
#define TMC4671_USE_ABN_AS_N_MASK                            0x08 // ABN_2_DECODER_MODE // 0: Ignore A and B polarity with Npulse = N, 1 : Npulse = N and A and B
#define TMC4671_USE_ABN_AS_N_SHIFT                           3 // min.: 0, max.: 1, default: 0
#define TMC4671_CLN_MASK                                     0x0100 // ABN_2_DECODER_MODE // Clear writes ABN_2_DECODER_COUNT_N into decoder count at Npulse.
#define TMC4671_CLN_SHIFT                                    8 // min.: 0, max.: 1, default: 0
#define TMC4671_ABN_2_DIRECTION_MASK                         0x1000 // ABN_2_DECODER_MODE // Decoder count direction.
#define TMC4671_ABN_2_DIRECTION_SHIFT                        12 // min.: 0, max.: 1, default: 0
#define TMC4671_ABN_2_DECODER_PPR_MASK                       0xFFFFFF // ABN_2_DECODER_PPR // Decoder_2 pules per mechanical revolution. This 2nd ABN encoder interface is for positioning or velocity control but NOT for motor commutation.
#define TMC4671_ABN_2_DECODER_PPR_SHIFT                      0 // min.: 1, max.: 16777215, default: 0
#define TMC4671_ABN_2_DECODER_COUNT_MASK                     0xFFFFFF // ABN_2_DECODER_COUNT // Raw decoder_2 count; the digital decoder engine counts modulo (decoder_2_ppr).
#define TMC4671_ABN_2_DECODER_COUNT_SHIFT                    0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_ABN_2_DECODER_COUNT_N_MASK                   0xFFFFFF // ABN_2_DECODER_COUNT_N // Decoder_2 count latched on N pulse, when N pulse clears decoder_2_count also decoder_2_count_n is 0.
#define TMC4671_ABN_2_DECODER_COUNT_N_SHIFT                  0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_ABN_2_DECODER_PHI_M_OFFSET_MASK              0xFFFF // ABN_2_DECODER_PHI_M_OFFSET // ABN_2_DECODER_PHI_M_OFFSET to shift (rotate) angle DECODER_2_PHI_M.
#define TMC4671_ABN_2_DECODER_PHI_M_OFFSET_SHIFT             0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ABN_2_DECODER_PHI_M_MASK                     0xFFFF // ABN_2_DECODER_PHI_M // ABN_2_DECODER_PHI_M = ABN_2_DECODER_COUNT * 2^16 / ABN_2_DECODER_PPR + ABN_2_DECODER_PHI_M_OFFSET;
#define TMC4671_ABN_2_DECODER_PHI_M_SHIFT                    0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_MODE_MASK                           	 0xFFFF // HALL_MODE
#define TMC4671_HALL_MODE_SHIFT                              0
#define TMC4671_HALL_POLARITY_MASK                           0x01 // HALL_MODE // polarity
#define TMC4671_HALL_POLARITY_SHIFT                          0 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_INTERPOLATION_MASK                      0x0100 // HALL_MODE // interpolation
#define TMC4671_HALL_INTERPOLATION_SHIFT                     8 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_DIRECTION_MASK                          0x1000 // HALL_MODE // direction
#define TMC4671_HALL_DIRECTION_SHIFT                         12 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_BLANK_MASK                              0x0FFF0000 // HALL_MODE // tBLANK = 10ns * HALL_BLANK
#define TMC4671_HALL_BLANK_SHIFT                             16 // min.: 0, max.: 4095, default: 0
#define TMC4671_HALL_POSITION_000_MASK                       0xFFFF // HALL_POSITION_060_000 // int16_t hall sensor position at 0\xc2\xb0
#define TMC4671_HALL_POSITION_000_SHIFT                      0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_POSITION_060_MASK                       0xFFFF0000 // HALL_POSITION_060_000 // int16_t hall sensor position at 60\xc2\xb0.
#define TMC4671_HALL_POSITION_060_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_POSITION_120_MASK                       0xFFFF // HALL_POSITION_180_120 // int16_t hall sensor position at 120\xc2\xb0.
#define TMC4671_HALL_POSITION_120_SHIFT                      0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_POSITION_180_MASK                       0xFFFF0000 // HALL_POSITION_180_120 // int16_t hall sensor position at 180\xc2\xb0.
#define TMC4671_HALL_POSITION_180_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_POSITION_240_MASK                       0xFFFF // HALL_POSITION_300_240 // int16_t hall sensor position at 240\xc2\xb0.
#define TMC4671_HALL_POSITION_240_SHIFT                      0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_POSITION_300_MASK                       0xFFFF0000 // HALL_POSITION_300_240 // int16_t hall sensor position at 300\xc2\xb0.
#define TMC4671_HALL_POSITION_300_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_PHI_M_OFFSET_MASK                       0xFFFF // HALL_PHI_E_PHI_M_OFFSET // Offset of mechanical angle hall_phi_m of hall decoder.
#define TMC4671_HALL_PHI_M_OFFSET_SHIFT                      0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_PHI_E_OFFSET_MASK                       0xFFFF0000 // HALL_PHI_E_PHI_M_OFFSET // Offset for electrical angle hall_phi_e of hall decoder.
#define TMC4671_HALL_PHI_E_OFFSET_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_DPHI_MAX_MASK                           0xFFFF // HALL_DPHI_MAX // Maximum dx for interpolation (default for digital hall: u16/6).
#define TMC4671_HALL_DPHI_MAX_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_HALL_PHI_E_MASK                              0xFFFF // HALL_PHI_E_INTERPOLATED_PHI_E // Raw electrical angle hall_phi_e of hall decoder, selection programmed via HALL_MODE control bit.
#define TMC4671_HALL_PHI_E_SHIFT                             0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_PHI_E_INTERPOLATED_MASK                 0xFFFF0000 // HALL_PHI_E_INTERPOLATED_PHI_E // Interpolated electrical angle hall_phi_e_interpolated, selection programmed via HALL_MODE control bit.
#define TMC4671_HALL_PHI_E_INTERPOLATED_SHIFT                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_HALL_PHI_M_MASK                              0xFFFF // HALL_PHI_M // Mechanical angle hall_phi_m of hall decoder.
#define TMC4671_HALL_PHI_M_SHIFT                             0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_MODE_0_MASK                     0x01 // AENC_DECODER_MODE // nXY_UVW : 0: SinCos Mode // 1: 0\xc2\xb0 120\xc2\xb0 240\xc2\xb0 Mode
#define TMC4671_AENC_DECODER_MODE_0_SHIFT                    0 // min.: 0, max.: 1, default: 0
#define TMC4671_AENC_DECODER_MODE_12_MASK                    0x1000 // AENC_DECODER_MODE // decoder count direction
#define TMC4671_AENC_DECODER_MODE_12_SHIFT                   12 // min.: 0, max.: 1, default: 0
#define TMC4671_AENC_DECODER_N_THRESHOLD_MASK                0xFFFF // AENC_DECODER_N_THRESHOLD // Threshold for generating of N pulse from analog AENC_N signal (only needed for analog SinCos encoders with analog N signal).
#define TMC4671_AENC_DECODER_N_THRESHOLD_SHIFT               0 // min.: 0, max.: 65535, default: 0
#define TMC4671_AENC_DECODER_N_MASK_MASK                     0xFFFF0000 // AENC_DECODER_N_THRESHOLD // Optional position mask (position) for the analog N pulse within phi_a period to be and-ed with the digital N pulse generated via aenc_decoder_n_threshold.
#define TMC4671_AENC_DECODER_N_FIELDS                    16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_PHI_A_RAW_MASK                  0xFFFF // AENC_DECODER_PHI_A_RAW // Raw analog angle phi calculated from analog AENC inputs (analog hall, analog SinCos, ...).
#define TMC4671_AENC_DECODER_PHI_A_RAW_SHIFT                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_PHI_A_OFFSET_MASK               0xFFFF // AENC_DECODER_PHI_A_OFFSET // Offset for angle phi from analog decoder (analog hall, analog SinCos, ...).
#define TMC4671_AENC_DECODER_PHI_A_OFFSET_SHIFT              0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_PHI_A_MASK                      0xFFFF // AENC_DECODER_PHI_A // Resulting phi available for the FOC (phi_e might need to be calculated from this angle via aenc_decoder_ppr, for analog hall sensors phi_a might be used directly as phi_e depends on analog hall signal type).
#define TMC4671_AENC_DECODER_PHI_A_SHIFT                     0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_AENC_DECODER_PPR_MASK                        0xFFFF // AENC_DECODER_PPR // Number of periods per revolution also called lines per revolution (different nomenclatur compared to digital ABN encoders).
#define TMC4671_AENC_DECODER_PPR_SHIFT                       0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_COUNT_MASK                      0xFFFFFFFF // AENC_DECODER_COUNT // Decoder position, raw unscaled.
#define TMC4671_AENC_DECODER_COUNT_SHIFT                     0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_AENC_DECODER_COUNT_N_MASK                    0xFFFFFFFF // AENC_DECODER_COUNT_N //  Latched decoder position on analog N pulse event.
#define TMC4671_AENC_DECODER_COUNT_N_SHIFT                   0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_AENC_DECODER_PHI_M_OFFSET_MASK               0xFFFF // AENC_DECODER_PHI_E_PHI_M_OFFSET // Offset for mechanical angle phi_m.
#define TMC4671_AENC_DECODER_PHI_M_OFFSET_SHIFT              0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_PHI_E_OFFSET_MASK               0xFFFF0000 // AENC_DECODER_PHI_E_PHI_M_OFFSET // Offset for electrical angle phi_e.
#define TMC4671_AENC_DECODER_PHI_E_OFFSET_SHIFT              16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_PHI_M_MASK                      0xFFFF // AENC_DECODER_PHI_E_PHI_M // Resulting angle phi_m.
#define TMC4671_AENC_DECODER_PHI_M_SHIFT                     0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_PHI_E_MASK                      0xFFFF0000 // AENC_DECODER_PHI_E_PHI_M // Resulting angle phi_e.
#define TMC4671_AENC_DECODER_PHI_E_SHIFT                     16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_AENC_DECODER_POSITION_MASK                   0xFFFFFFFF // AENC_DECODER_POSITION // Multi-turn position.
#define TMC4671_AENC_DECODER_POSITION_SHIFT                  0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDIN_VELOCITY_TARGET_MASK                   0xFFFFFFFF // PIDIN_VELOCITY_TARGET // Target velocity at PI controller input.
#define TMC4671_PIDIN_VELOCITY_TARGET_SHIFT                  0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDIN_POSITION_TARGET_MASK                   0xFFFFFFFF // PIDIN_POSITION_TARGET // Target position at PI controller input.
#define TMC4671_PIDIN_POSITION_TARGET_SHIFT                  0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_X_A_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_X_A_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_X_A_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_X_A_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_X_B_0_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_X_B_0_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_X_B_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_X_B_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_X_B_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_X_B_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_X_ENABLE_MASK                         0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_X_ENABLE_SHIFT                        0 // min.: 0, max.: 1, default: 0
#define TMC4671_BIQUAD_V_A_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_V_A_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_V_A_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_V_A_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_V_B_0_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_V_B_0_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_V_B_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_V_B_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_V_B_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_V_B_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_V_ENABLE_MASK                         0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_V_ENABLE_SHIFT                        0 // min.: 0, max.: 1, default: 0
#define TMC4671_BIQUAD_T_A_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_T_A_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_T_A_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_T_A_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_T_B_0_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_T_B_0_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_T_B_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_T_B_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_T_B_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_T_B_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_T_ENABLE_MASK                         0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_T_ENABLE_SHIFT                        0 // min.: 0, max.: 1, default: 0
#define TMC4671_BIQUAD_F_A_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_F_A_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_F_A_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_F_A_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_F_B_0_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_F_B_0_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_F_B_1_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_F_B_1_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_F_B_2_MASK                            0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_F_B_2_SHIFT                           0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_BIQUAD_F_ENABLE_MASK                         0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_BIQUAD_F_ENABLE_SHIFT                        0 // min.: 0, max.: 1, default: 0
#define TMC4671_PRBS_AMPLITUDE_MASK                          0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_PRBS_AMPLITUDE_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PRBS_DOWN_SAMPLING_RATIO_MASK                0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_PRBS_DOWN_SAMPLING_RATIO_SHIFT               0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_FEED_FORWARD_VELOCITY_GAIN_MASK              0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_FEED_FORWARD_VELOCITY_GAIN_SHIFT             0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_FEED_FORWARD_VELICITY_FILTER_CONSTANT_MASK   0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_FEED_FORWARD_VELICITY_FILTER_CONSTANT_SHIFT  0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_FEED_FORWARD_TORQUE_GAIN_MASK                0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_FEED_FORWARD_TORQUE_GAIN_SHIFT               0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_FEED_FORWARD_TORGUE_FILTER_CONSTANT_MASK     0xFFFFFFFF // CONFIG_DATA //
#define TMC4671_FEED_FORWARD_TORGUE_FILTER_CONSTANT_SHIFT    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_VELOCITY_METER_PPTM_MIN_POS_DEV_MASK         0xFFFF // CONFIG_DATA //
#define TMC4671_VELOCITY_METER_PPTM_MIN_POS_DEV_SHIFT        0 // min.: 0, max.: 65535, default: 0
#define TMC4671_REF_SWITCH_CONFIG_MASK                       0xFFFF // CONFIG_DATA //
#define TMC4671_REF_SWITCH_CONFIG_SHIFT                      0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENCODER_INIT_HALL_ENABLE_MASK                0x01 // CONFIG_DATA //
#define TMC4671_ENCODER_INIT_HALL_ENABLE_SHIFT               0 // min.: 0, max.: 1, default: 0
#define TMC4671_CONFIG_ADDR_MASK                             0xFFFFFFFF // CONFIG_ADDR //
#define TMC4671_CONFIG_ADDR_SHIFT                            0 // min.: 1, max.: 52, default: 0
#define TMC4671_VELOCITY_SELECTION_MASK                      0xFF // VELOCITY_SELECTION // Selects the source of the velocity source for velocity measurement.
#define TMC4671_VELOCITY_SELECTION_SHIFT                     0 // min.: 0, max.: 12, default: 0
#define TMC4671_VELOCITY_METER_SELECTION_MASK                0xFF00 // VELOCITY_SELECTION //
#define TMC4671_VELOCITY_METER_SELECTION_SHIFT               8 // min.: 0, max.: 1, default: 0
#define TMC4671_POSITION_SELECTION_MASK                      0xFF // POSITION_SELECTION //
#define TMC4671_POSITION_SELECTION_SHIFT                     0 // min.: 0, max.: 12, default: 0
#define TMC4671_PHI_E_SELECTION_MASK                         0xFF // PHI_E_SELECTION //
#define TMC4671_PHI_E_SELECTION_SHIFT                        0 // min.: 0, max.: 7, default: 0
#define TMC4671_PHI_E_MASK                                   0xFFFF // PHI_E // Angle used for the inner FOC loop.
#define TMC4671_PHI_E_SHIFT                                  0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_FLUX_I_MASK                              0xFFFF // PID_FLUX_P_FLUX_I //
#define TMC4671_PID_FLUX_I_SHIFT                             0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_FLUX_P_MASK                              0xFFFF0000 // PID_FLUX_P_FLUX_I //
#define TMC4671_PID_FLUX_P_SHIFT                             16 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_I_MASK                            0xFFFF // PID_TORQUE_P_TORQUE_I //
#define TMC4671_PID_TORQUE_I_SHIFT                           0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_P_MASK                            0xFFFF0000 // PID_TORQUE_P_TORQUE_I //
#define TMC4671_PID_TORQUE_P_SHIFT                           16 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_I_MASK                          0xFFFF // PID_VELOCITY_P_VELOCITY_I //
#define TMC4671_PID_VELOCITY_I_SHIFT                         0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_P_MASK                          0xFFFF0000 // PID_VELOCITY_P_VELOCITY_I //
#define TMC4671_PID_VELOCITY_P_SHIFT                         16 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_POSITION_I_MASK                          0xFFFF // PID_POSITION_P_POSITION_I //
#define TMC4671_PID_POSITION_I_SHIFT                         0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_POSITION_P_MASK                          0xFFFF0000 // PID_POSITION_P_POSITION_I //
#define TMC4671_PID_POSITION_P_SHIFT                         16 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_FLUX_TARGET_DDT_LIMITS_MASK       0xFFFFFFFF // PID_TORQUE_FLUX_TARGET_DDT_LIMITS // Limits of change in time [d/dt] of the target torque and target flux.
#define TMC4671_PID_TORQUE_FLUX_TARGET_DDT_LIMITS_SHIFT      0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PIDOUT_UQ_UD_LIMITS_MASK                     0xFFFF // PIDOUT_UQ_UD_LIMITS // Two dimensional circular limiter for inputs of iPark.
#define TMC4671_PIDOUT_UQ_UD_LIMITS_SHIFT                    0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_FLUX_LIMITS_MASK                  0xFFFF // PID_TORQUE_FLUX_LIMITS // PID torque limt and PID flux limit, limits the target values coming from the target registers.
#define TMC4671_PID_TORQUE_FLUX_LIMITS_SHIFT                 0 // min.: 0, max.: 32767, default: 0
#define TMC4671_PID_ACCELERATION_LIMIT_MASK                  0xFFFFFFFF // PID_ACCELERATION_LIMIT // Acceleration limit.
#define TMC4671_PID_ACCELERATION_LIMIT_SHIFT                 0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_PID_VELOCITY_LIMIT_MASK                      0xFFFFFFFF // PID_VELOCITY_LIMIT // Velocity limit.
#define TMC4671_PID_VELOCITY_LIMIT_SHIFT                     0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_PID_POSITION_LIMIT_LOW_MASK                  0xFFFFFFFF // PID_POSITION_LIMIT_LOW // Position limit low, programmable positon barrier.
#define TMC4671_PID_POSITION_LIMIT_LOW_SHIFT                 0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_POSITION_LIMIT_HIGH_MASK                 0xFFFFFFFF // PID_POSITION_LIMIT_HIGH // Position limit high, programmable positon barrier.
#define TMC4671_PID_POSITION_LIMIT_HIGH_SHIFT                0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_MODE_MOTION_MASK                             0xFF // MODE_RAMP_MODE_MOTION //
#define TMC4671_MODE_MOTION_SHIFT                            0 // min.: 0, max.: 15, default: 0
#define TMC4671_MODE_RAMP_MASK                               0xFF00 // MODE_RAMP_MODE_MOTION //
#define TMC4671_MODE_RAMP_SHIFT                              8 // min.: 0, max.: 7, default: 0
#define TMC4671_MODE_FF_MASK                                 0xFF0000 // MODE_RAMP_MODE_MOTION //
#define TMC4671_MODE_FF_SHIFT                                16 // min.: 0, max.: 2, default: 0
#define TMC4671_MODE_PID_SMPL_MASK                           0x7F000000 // MODE_RAMP_MODE_MOTION //
#define TMC4671_MODE_PID_SMPL_SHIFT                          24 // min.: 0, max.: 127, default: 0
#define TMC4671_MODE_PID_TYPE_MASK                           0x80000000 // MODE_RAMP_MODE_MOTION //
#define TMC4671_MODE_PID_TYPE_SHIFT                          31 // min.: 0, max.: 1, default: 0
#define TMC4671_PID_FLUX_TARGET_MASK                         0xFFFF // PID_TORQUE_FLUX_TARGET //
#define TMC4671_PID_FLUX_TARGET_SHIFT                        0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_TARGET_MASK                       0xFFFF0000 // PID_TORQUE_FLUX_TARGET //
#define TMC4671_PID_TORQUE_TARGET_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_FLUX_OFFSET_MASK                         0xFFFF // PID_TORQUE_FLUX_OFFSET // Flux offset for feed forward control.
#define TMC4671_PID_FLUX_OFFSET_SHIFT                        0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_OFFSET_MASK                       0xFFFF0000 // PID_TORQUE_FLUX_OFFSET // Torque offset for feed forward control.
#define TMC4671_PID_TORQUE_OFFSET_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_TARGET_MASK                     0xFFFFFFFF // PID_VELOCITY_TARGET // Target velocity register (for velocity mode).
#define TMC4671_PID_VELOCITY_TARGET_SHIFT                    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_VELOCITY_OFFSET_MASK                     0xFFFFFFFF // PID_VELOCITY_OFFSET // Velocity offset for feed forward control.
#define TMC4671_PID_VELOCITY_OFFSET_SHIFT                    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_POSITION_TARGET_MASK                     0xFFFFFFFF // PID_POSITION_TARGET // Target position register (for position mode).
#define TMC4671_PID_POSITION_TARGET_SHIFT                    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_FLUX_ACTUAL_MASK                         0xFFFF // PID_TORQUE_FLUX_ACTUAL //
#define TMC4671_PID_FLUX_ACTUAL_SHIFT                        0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_TORQUE_ACTUAL_MASK                       0xFFFF0000 // PID_TORQUE_FLUX_ACTUAL //
#define TMC4671_PID_TORQUE_ACTUAL_SHIFT                      16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_ACTUAL_MASK                     0xFFFFFFFF // PID_VELOCITY_ACTUAL // Actual velocity.
#define TMC4671_PID_VELOCITY_ACTUAL_SHIFT                    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_POSITION_ACTUAL_MASK                     0xFFFFFFFF // PID_POSITION_ACTUAL // Actual multi turn position for positioning. WRITE on PID_POSITION_ACTUAL writes same value into PID_POSITION_TARGET to avoid unwanted move.
#define TMC4671_PID_POSITION_ACTUAL_SHIFT                    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_TORQUE_ERROR_MASK                        0xFFFFFFFF // PID_ERROR_DATA // PID torque error.
#define TMC4671_PID_TORQUE_ERROR_SHIFT                       0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_FLUX_ERROR_MASK                          0xFFFFFFFF // PID_ERROR_DATA // PID flux error.
#define TMC4671_PID_FLUX_ERROR_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_VELOCITY_ERROR_MASK                      0xFFFFFFFF // PID_ERROR_DATA // PID velocity error.
#define TMC4671_PID_VELOCITY_ERROR_SHIFT                     0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_POSITION_ERROR_MASK                      0xFFFFFFFF // PID_ERROR_DATA // PID position error.
#define TMC4671_PID_POSITION_ERROR_SHIFT                     0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_TORQUE_ERROR_SUM_MASK                    0xFFFFFFFF // PID_ERROR_DATA // PID torque error.
#define TMC4671_PID_TORQUE_ERROR_SUM_SHIFT                   0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_FLUX_ERROR_SUM_MASK                      0xFFFFFFFF // PID_ERROR_DATA // PID flux error sum.
#define TMC4671_PID_FLUX_ERROR_SUM_SHIFT                     0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_VELOCITY_ERROR_SUM_MASK                  0xFFFFFFFF // PID_ERROR_DATA // PID velocity error sum.
#define TMC4671_PID_VELOCITY_ERROR_SUM_SHIFT                 0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_POSITION_ERROR_SUM_MASK                  0xFFFFFFFF // PID_ERROR_DATA // PID position error sum.
#define TMC4671_PID_POSITION_ERROR_SUM_SHIFT                 0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PID_ERROR_ADDR_MASK                          0xFF // PID_ERROR_ADDR //
#define TMC4671_PID_ERROR_ADDR_SHIFT                         0 // min.: 0, max.: 7, default: 0
#define TMC4671_PIDIN_TARGET_TORQUE_MASK                     0xFFFFFFFF // INTERIM_DATA // PIDIN target torque.
#define TMC4671_PIDIN_TARGET_TORQUE_SHIFT                    0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDIN_TARGET_FLUX_MASK                       0xFFFFFFFF // INTERIM_DATA // PIDIN target flux.
#define TMC4671_PIDIN_TARGET_FLUX_SHIFT                      0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDIN_TARGET_VELOCITY_MASK                   0xFFFFFFFF // INTERIM_DATA // PIDIN target velocity.
#define TMC4671_PIDIN_TARGET_VELOCITY_SHIFT                  0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDIN_TARGET_POSITION_MASK                   0xFFFFFFFF // INTERIM_DATA // PIDIN target position.
#define TMC4671_PIDIN_TARGET_POSITION_SHIFT                  0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDOUT_TARGET_TORQUE_MASK                    0xFFFFFFFF // INTERIM_DATA // PIDOUT target torque.
#define TMC4671_PIDOUT_TARGET_TORQUE_SHIFT                   0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDOUT_TARGET_FLUX_MASK                      0xFFFFFFFF // INTERIM_DATA // PIDOUT target flux.
#define TMC4671_PIDOUT_TARGET_FLUX_SHIFT                     0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDOUT_TARGET_VELOCITY_MASK                  0xFFFFFFFF // INTERIM_DATA // PIDOUT target velocity.
#define TMC4671_PIDOUT_TARGET_VELOCITY_SHIFT                 0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_PIDOUT_TARGET_POSITION_MASK                  0xFFFFFFFF // INTERIM_DATA // PIDOUT target position.
#define TMC4671_PIDOUT_TARGET_POSITION_SHIFT                 0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_FOC_IUX_MASK                                 0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_IUX_SHIFT                                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_IWY_MASK                                 0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_IWY_SHIFT                                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_IV_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_IV_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_IA_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_IA_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_IB_MASK                                  0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_IB_SHIFT                                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_ID_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_ID_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_IQ_MASK                                  0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_IQ_SHIFT                                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UD_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_UD_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UQ_MASK                                  0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_UQ_SHIFT                                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UD_LIMITED_MASK                          0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_UD_LIMITED_SHIFT                         0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UQ_LIMITED_MASK                          0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_UQ_LIMITED_SHIFT                         16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UA_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_UA_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UB_MASK                                  0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_UB_SHIFT                                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UUX_MASK                                 0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_UUX_SHIFT                                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UWY_MASK                                 0xFFFF0000 // INTERIM_DATA //
#define TMC4671_FOC_UWY_SHIFT                                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FOC_UV_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_FOC_UV_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PWM_UX_MASK                                  0xFFFF // INTERIM_DATA //
#define TMC4671_PWM_UX_SHIFT                                 0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PWM_WY_MASK                                  0xFFFF0000 // INTERIM_DATA //
#define TMC4671_PWM_WY_SHIFT                                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PWM_V_MASK                                   0xFFFF // INTERIM_DATA //
#define TMC4671_PWM_V_SHIFT                                  0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ADC_I_0_MASK                                 0xFFFF // INTERIM_DATA //
#define TMC4671_ADC_I_0_SHIFT                                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ADC_I_1_MASK                                 0xFFFF0000 // INTERIM_DATA //
#define TMC4671_ADC_I_1_SHIFT                                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_FLUX_ACTUAL_DIV256_MASK                  0xFF // INTERIM_DATA //
#define TMC4671_PID_FLUX_ACTUAL_DIV256_SHIFT                 0 // min.: -128, max.: 127, default: 0
#define TMC4671_PID_TORQUE_ACTUAL_DIV256_MASK                0xFF00 // INTERIM_DATA //
#define TMC4671_PID_TORQUE_ACTUAL_DIV256_SHIFT               8 // min.: -128, max.: 127, default: 0
#define TMC4671_PID_FLUX_TARGET_DIV256_MASK                  0xFF0000 // INTERIM_DATA //
#define TMC4671_PID_FLUX_TARGET_DIV256_SHIFT                 16 // min.: -128, max.: 127, default: 0
#define TMC4671_PID_TORQUE_TARGET_DIV256_MASK                0xFF000000 // INTERIM_DATA //
#define TMC4671_PID_TORQUE_TARGET_DIV256_SHIFT               24 // min.: -128, max.: 127, default: 0
#define TMC4671_PID_VELOCITY_ACTUAL_DIV256_MASK              0xFFFF // INTERIM_DATA //
#define TMC4671_PID_VELOCITY_ACTUAL_DIV256_SHIFT             0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_TARGET_DIV256_MASK              0xFFFF0000 // INTERIM_DATA //
#define TMC4671_PID_VELOCITY_TARGET_DIV256_SHIFT             16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_ACTUAL_LSB_MASK                 0xFFFF // INTERIM_DATA //
#define TMC4671_PID_VELOCITY_ACTUAL_LSB_SHIFT                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_VELOCITY_TARGET_LSB_MASK                 0xFFFF0000 // INTERIM_DATA //
#define TMC4671_PID_VELOCITY_TARGET_LSB_SHIFT                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_POSITION_ACTUAL_DIV256_MASK              0xFFFF // INTERIM_DATA //
#define TMC4671_PID_POSITION_ACTUAL_DIV256_SHIFT             0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_POSITION_TARGET_DIV256_MASK              0xFFFF0000 // INTERIM_DATA //
#define TMC4671_PID_POSITION_TARGET_DIV256_SHIFT             16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_POSITION_ACTUAL_LSB_MASK                 0xFFFF // INTERIM_DATA //
#define TMC4671_PID_POSITION_ACTUAL_LSB_SHIFT                0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_PID_POSITION_TARGET_LSB_MASK                 0xFFFF0000 // INTERIM_DATA //
#define TMC4671_PID_POSITION_TARGET_LSB_SHIFT                16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_FF_VELOCITY_MASK                             0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_FF_VELOCITY_SHIFT                            0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_FF_TORQUE_MASK                               0xFFFF // INTERIM_DATA //
#define TMC4671_FF_TORQUE_SHIFT                              0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ACTUAL_VELOCITY_PPTM_MASK                    0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_ACTUAL_VELOCITY_PPTM_SHIFT                   0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_REF_SWITCH_STATUS_MASK                       0xFFFF // INTERIM_DATA //
#define TMC4671_REF_SWITCH_STATUS_SHIFT                      0 // min.: 0, max.: 65535, default: 0
#define TMC4671_HOME_POSITION_MASK                           0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_HOME_POSITION_SHIFT                          0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_LEFT_POSITION_MASK                           0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_LEFT_POSITION_SHIFT                          0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_RIGHT_POSITION_MASK                          0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_RIGHT_POSITION_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_ENC_INIT_HALL_STATUS_MASK                    0xFFFF // INTERIM_DATA //
#define TMC4671_ENC_INIT_HALL_STATUS_SHIFT                   0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENC_INIT_HALL_PHI_E_ABN_OFFSET_MASK          0xFFFF // INTERIM_DATA //
#define TMC4671_ENC_INIT_HALL_PHI_E_ABN_OFFSET_SHIFT         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENC_INIT_HALL_PHI_E_AENC_OFFSET_MASK         0xFFFF // INTERIM_DATA //
#define TMC4671_ENC_INIT_HALL_PHI_E_AENC_OFFSET_SHIFT        0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENC_INIT_HALL_PHI_A_AENC_OFFSET_MASK         0xFFFF // INTERIM_DATA //
#define TMC4671_ENC_INIT_HALL_PHI_A_AENC_OFFSET_SHIFT        0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENC_INIT_MINI_MOVE_STATUS_MASK               0xFFFF // INTERIM_DATA //
#define TMC4671_ENC_INIT_MINI_MOVE_STATUS_SHIFT              0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENC_INIT_MINI_MOVE_U_D_MASK                  0xFFFF0000 // INTERIM_DATA //
#define TMC4671_ENC_INIT_MINI_MOVE_U_D_SHIFT                 16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_ENC_INIT_MINI_MOVE_PHI_E_OFFSET_MASK         0xFFFF // INTERIM_DATA //
#define TMC4671_ENC_INIT_MINI_MOVE_PHI_E_OFFSET_SHIFT        0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ENC_INIT_MINI_MOVE_PHI_E_MASK                0xFFFF0000 // INTERIM_DATA //
#define TMC4671_ENC_INIT_MINI_MOVE_PHI_E_SHIFT               16 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_0_MASK                           0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_0_SHIFT                          0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_1_MASK                           0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_1_SHIFT                          16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_2_MASK                           0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_2_SHIFT                          0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_3_MASK                           0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_3_SHIFT                          16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_4_MASK                           0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_4_SHIFT                          0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_5_MASK                           0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_5_SHIFT                          16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_6_MASK                           0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_6_SHIFT                          0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_7_MASK                           0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_7_SHIFT                          16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_DEBUG_VALUE_8_MASK                           0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_8_SHIFT                          0 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_9_MASK                           0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_9_SHIFT                          16 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_10_MASK                          0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_10_SHIFT                         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_11_MASK                          0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_11_SHIFT                         16 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_12_MASK                          0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_12_SHIFT                         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_13_MASK                          0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_13_SHIFT                         16 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_14_MASK                          0xFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_14_SHIFT                         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_15_MASK                          0xFFFF0000 // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_15_SHIFT                         16 // min.: 0, max.: 65535, default: 0
#define TMC4671_DEBUG_VALUE_16_MASK                          0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_16_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_DEBUG_VALUE_17_MASK                          0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_17_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_DEBUG_VALUE_18_MASK                          0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_18_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_DEBUG_VALUE_19_MASK                          0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_DEBUG_VALUE_19_SHIFT                         0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_CONFIG_REG_0_MASK                            0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_CONFIG_REG_0_SHIFT                           0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_CONFIG_REG_1_MASK                            0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_CONFIG_REG_1_SHIFT                           0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_CTRL_PARAM_0_MASK                            0xFFFF // INTERIM_DATA //
#define TMC4671_CTRL_PARAM_0_SHIFT                           0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_CTRL_PARAM_1_MASK                            0xFFFF0000 // INTERIM_DATA //
#define TMC4671_CTRL_PARAM_1_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_CTRL_PARAM_2_MASK                            0xFFFF // INTERIM_DATA //
#define TMC4671_CTRL_PARAM_2_SHIFT                           0 // min.: -32768, max.: 32767, default: 0
#define TMC4671_CTRL_PARAM_3_MASK                            0xFFFF0000 // INTERIM_DATA //
#define TMC4671_CTRL_PARAM_3_SHIFT                           16 // min.: -32768, max.: 32767, default: 0
#define TMC4671_STATUS_REG_0_MASK                            0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_STATUS_REG_0_SHIFT                           0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_STATUS_REG_1_MASK                            0xFFFFFFFF // INTERIM_DATA //
#define TMC4671_STATUS_REG_1_SHIFT                           0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_STATUS_PARAM_0_MASK                          0xFFFF // INTERIM_DATA //
#define TMC4671_STATUS_PARAM_0_SHIFT                         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_STATUS_PARAM_1_MASK                          0xFFFF0000 // INTERIM_DATA //
#define TMC4671_STATUS_PARAM_1_SHIFT                         16 // min.: 0, max.: 65535, default: 0
#define TMC4671_STATUS_PARAM_2_MASK                          0xFFFF // INTERIM_DATA //
#define TMC4671_STATUS_PARAM_2_SHIFT                         0 // min.: 0, max.: 65535, default: 0
#define TMC4671_STATUS_PARAM_3_MASK                          0xFFFF0000 // INTERIM_DATA //
#define TMC4671_STATUS_PARAM_3_SHIFT                         16 // min.: 0, max.: 65535, default: 0
#define TMC4671_INTERIM_ADDR_MASK                            0xFF // INTERIM_ADDR //
#define TMC4671_INTERIM_ADDR_SHIFT                           0 // min.: 0, max.: 215, default: 0
#define TMC4671_WATCHDOG_CFG_MASK                            0x03 // WATCHDOG_CFG //
#define TMC4671_WATCHDOG_CFG_SHIFT                           0 // min.: 0, max.: 3, default: 0
#define TMC4671_ADC_VM_LIMIT_LOW_MASK                        0xFFFF // ADC_VM_LIMITS // Low limit for brake chopper output BRAKE_OUT.
#define TMC4671_ADC_VM_LIMIT_LOW_SHIFT                       0 // min.: 0, max.: 65535, default: 0
#define TMC4671_ADC_VM_LIMIT_HIGH_MASK                       0xFFFF0000 // ADC_VM_LIMITS // High limit for brake chopper output BRAKE_OUT.
#define TMC4671_ADC_VM_LIMIT_HIGH_SHIFT                      16 // min.: 0, max.: 65535, default: 0
#define TMC4671_A_OF_ABN_RAW_MASK                            0x01 // TMC4671_INPUTS_RAW // A of ABN_RAW
#define TMC4671_A_OF_ABN_RAW_SHIFT                           0 // min.: 0, max.: 1, default: 0
#define TMC4671_B_OF_ABN_RAW_MASK                            0x02 // TMC4671_INPUTS_RAW // B of ABN_RAW
#define TMC4671_B_OF_ABN_RAW_SHIFT                           1 // min.: 0, max.: 1, default: 0
#define TMC4671_N_OF_ABN_RAW_MASK                            0x04 // TMC4671_INPUTS_RAW // N of ABN_RAW
#define TMC4671_N_OF_ABN_RAW_SHIFT                           2 // min.: 0, max.: 1, default: 0
#define TMC4671_A_OF_ABN_2_RAW_MASK                          0x10 // TMC4671_INPUTS_RAW // A of ABN_2_RAW
#define TMC4671_A_OF_ABN_2_RAW_SHIFT                         4 // min.: 0, max.: 1, default: 0
#define TMC4671_B_OF_ABN_2_RAW_MASK                          0x20 // TMC4671_INPUTS_RAW // B of ABN_2_RAW
#define TMC4671_B_OF_ABN_2_RAW_SHIFT                         5 // min.: 0, max.: 1, default: 0
#define TMC4671_N_OF_ABN_2_RAW_MASK                          0x40 // TMC4671_INPUTS_RAW // N of ABN_2_RAW
#define TMC4671_N_OF_ABN_2_RAW_SHIFT                         6 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_UX_OF_HALL_RAW_MASK                     0x0100 // TMC4671_INPUTS_RAW // HALL_UX of HALL_RAW
#define TMC4671_HALL_UX_OF_HALL_RAW_SHIFT                    8 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_V_OF_HALL_RAW_MASK                      0x0200 // TMC4671_INPUTS_RAW // HALL_V of HALL_RAW
#define TMC4671_HALL_V_OF_HALL_RAW_SHIFT                     9 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_WY_OF_HALL_RAW_MASK                     0x0400 // TMC4671_INPUTS_RAW // HALL_WY of HALL_RAW
#define TMC4671_HALL_WY_OF_HALL_RAW_SHIFT                    10 // min.: 0, max.: 1, default: 0
#define TMC4671_REF_SW_R_RAW_MASK                            0x1000 // TMC4671_INPUTS_RAW // REF_SW_R_RAW
#define TMC4671_REF_SW_R_RAW_SHIFT                           12 // min.: 0, max.: 1, default: 0
#define TMC4671_REF_SW_H_RAW_MASK                            0x2000 // TMC4671_INPUTS_RAW // REF_SW_H_RAW
#define TMC4671_REF_SW_H_RAW_SHIFT                           13 // min.: 0, max.: 1, default: 0
#define TMC4671_REF_SW_L_RAW_MASK                            0x4000 // TMC4671_INPUTS_RAW // REF_SW_L_RAW
#define TMC4671_REF_SW_L_RAW_SHIFT                           14 // min.: 0, max.: 1, default: 0
#define TMC4671_ENABLE_IN_RAW_MASK                           0x8000 // TMC4671_INPUTS_RAW // ENABLE_IN_RAW
#define TMC4671_ENABLE_IN_RAW_SHIFT                          15 // min.: 0, max.: 1, default: 0
#define TMC4671_STP_OF_DIRSTP_RAW_MASK                       0x010000 // TMC4671_INPUTS_RAW // STP of DIRSTP_RAW
#define TMC4671_STP_OF_DIRSTP_RAW_SHIFT                      16 // min.: 0, max.: 1, default: 0
#define TMC4671_DIR_OF_DIRSTP_RAW_MASK                       0x020000 // TMC4671_INPUTS_RAW // DIR of DIRSTP_RAW
#define TMC4671_DIR_OF_DIRSTP_RAW_SHIFT                      17 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_IN_RAW_MASK                              0x040000 // TMC4671_INPUTS_RAW // PWM_IN_RAW
#define TMC4671_PWM_IN_RAW_SHIFT                             18 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_UX_FILT_MASK                            0x100000 // TMC4671_INPUTS_RAW // ESI_0 of ESI_RAW
#define TMC4671_HALL_UX_FILT_SHIFT                           20 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_V_FILT_MASK                             0x200000 // TMC4671_INPUTS_RAW // ESI_1 of ESI_RAW
#define TMC4671_HALL_V_FILT_SHIFT                            21 // min.: 0, max.: 1, default: 0
#define TMC4671_HALL_WY_FILT_MASK                            0x400000 // TMC4671_INPUTS_RAW // ESI_2 of ESI_RAW
#define TMC4671_HALL_WY_FILT_SHIFT                           22 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_IDLE_L_RAW_MASK                          0x10000000 // TMC4671_INPUTS_RAW // PWM_IDLE_L_RAW
#define TMC4671_PWM_IDLE_L_RAW_SHIFT                         28 // min.: 0, max.: 1, default: 0
#define TMC4671_PWM_IDLE_H_RAW_MASK                          0x20000000 // TMC4671_INPUTS_RAW // PWM_IDLE_H_RAW
#define TMC4671_PWM_IDLE_H_RAW_SHIFT                         29 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_0_MASK                           0x01 // TMC4671_OUTPUTS_RAW // PWM_UX1_L
#define TMC4671_OUTPUTS_RAW_0_SHIFT                          0 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_1_MASK                           0x02 // TMC4671_OUTPUTS_RAW // PWM_UX1_H
#define TMC4671_OUTPUTS_RAW_1_SHIFT                          1 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_2_MASK                           0x04 // TMC4671_OUTPUTS_RAW // PWM_VX2_L
#define TMC4671_OUTPUTS_RAW_2_SHIFT                          2 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_3_MASK                           0x08 // TMC4671_OUTPUTS_RAW // PWM_VX2_H
#define TMC4671_OUTPUTS_RAW_3_SHIFT                          3 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_4_MASK                           0x10 // TMC4671_OUTPUTS_RAW // PWM_WY1_L
#define TMC4671_OUTPUTS_RAW_4_SHIFT                          4 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_5_MASK                           0x20 // TMC4671_OUTPUTS_RAW // PWM_WY1_H
#define TMC4671__OUTPUTS_RAW_5_SHIFT                          5 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_6_MASK                           0x40 // TMC4671_OUTPUTS_RAW // PWM_Y2_L
#define TMC4671_OUTPUTS_RAW_6_SHIFT                          6 // min.: 0, max.: 1, default: 0
#define TMC4671_OUTPUTS_RAW_7_MASK                           0x80 // TMC4671_OUTPUTS_RAW // PWM_Y2_H
#define TMC4671_OUTPUTS_RAW_7_SHIFT                          7 // min.: 0, max.: 1, default: 0
#define TMC4671_STEP_WIDTH_MASK                              0xFFFFFFFF // STEP_WIDTH // STEP WIDTH = 0 => STP pulses ignored, resulting direction = DIR XOR sign(STEP_WIDTH), effects PID_POSITION_TARGET
#define TMC4671_STEP_WIDTH_SHIFT                             0 // min.: -2147483648, max.: 2147483647, default: 0
#define TMC4671_UART_BPS_MASK                                0xFFFFFF // UART_BPS // 9600, 115200, 921600, 3000000 (default=9600)
#define TMC4671_UART_BPS_SHIFT                               0 // min.: 0, max.: 16777215, default: 0
#define TMC4671_ADDR_A_MASK                                  0xFF // UART_ADDRS //
#define TMC4671_ADDR_A_SHIFT                                 0 // min.: 0, max.: 255, default: 0
#define TMC4671_ADDR_B_MASK                                  0xFF00 // UART_ADDRS //
#define TMC4671_ADDR_B_SHIFT                                 8 // min.: 0, max.: 255, default: 0
#define TMC4671_ADDR_C_MASK                                  0xFF0000 // UART_ADDRS //
#define TMC4671_ADDR_C_SHIFT                                 16 // min.: 0, max.: 255, default: 0
#define TMC4671_ADDR_D_MASK                                  0xFF000000 // UART_ADDRS //
#define TMC4671_ADDR_D_SHIFT                                 24 // min.: 0, max.: 255, default: 0
#define TMC4671_GPIO_DSADCI_CONFIG_MASK                      0xFFFFFFFF // GPIO_dsADCI_CONFIG // Configuration of GPIO (enable, disable, input, output, analog dsADC input)
#define TMC4671_GPIO_DSADCI_CONFIG_SHIFT                     0 // min.: 0, max.: 4294967295, default: 0
#define TMC4671_STATUS_FLAGS_0_MASK                          0x01 // STATUS_FLAGS // pid_x_target_limit
#define TMC4671_STATUS_FLAGS_0_SHIFT                         0 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_1_MASK                          0x02 // STATUS_FLAGS // pid_x_target_ddt_limit
#define TMC4671_STATUS_FLAGS_1_SHIFT                         1 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_2_MASK                          0x04 // STATUS_FLAGS // pid_x_errsum_limit
#define TMC4671_STATUS_FLAGS_2_SHIFT                         2 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_3_MASK                          0x08 // STATUS_FLAGS // pid_x_output_limit
#define TMC4671_STATUS_FLAGS_3_SHIFT                         3 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_4_MASK                          0x10 // STATUS_FLAGS // pid_v_target_limit
#define TMC4671_STATUS_FLAGS_4_SHIFT                         4 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_5_MASK                          0x20 // STATUS_FLAGS // pid_v_target_ddt_limit
#define TMC4671_STATUS_FLAGS_5_SHIFT                         5 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_6_MASK                          0x40 // STATUS_FLAGS // pid_v_errsum_limit
#define TMC4671_STATUS_FLAGS_6_SHIFT                         6 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_7_MASK                          0x80 // STATUS_FLAGS // pid_v_output_limit
#define TMC4671_STATUS_FLAGS_7_SHIFT                         7 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_8_MASK                          0x0100 // STATUS_FLAGS // pid_id_target_limit
#define TMC4671_STATUS_FLAGS_8_SHIFT                         8 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_9_MASK                          0x0200 // STATUS_FLAGS // pid_id_target_ddt_limit
#define TMC4671_STATUS_FLAGS_9_SHIFT                         9 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_10_MASK                         0x0400 // STATUS_FLAGS // pid_id_errsum_limit
#define TMC4671_STATUS_FLAGS_10_SHIFT                        10 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_11_MASK                         0x0800 // STATUS_FLAGS // pid_id_output_limit
#define TMC4671_STATUS_FLAGS_11_SHIFT                        11 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_12_MASK                         0x1000 // STATUS_FLAGS // pid_iq_target_limit
#define TMC4671_STATUS_FLAGS_12_SHIFT                        12 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_13_MASK                         0x2000 // STATUS_FLAGS // pid_iq_target_ddt_limit
#define TMC4671_STATUS_FLAGS_13_SHIFT                        13 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_14_MASK                         0x4000 // STATUS_FLAGS // pid_iq_errsum_limit
#define TMC4671_STATUS_FLAGS_14_SHIFT                        14 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_15_MASK                         0x8000 // STATUS_FLAGS // pid_iq_output_limit
#define TMC4671_STATUS_FLAGS_15_SHIFT                        15 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_16_MASK                         0x010000 // STATUS_FLAGS // ipark_cirlim_limit_u_d
#define TMC4671_STATUS_FLAGS_16_SHIFT                        16 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_17_MASK                         0x020000 // STATUS_FLAGS // ipark_cirlim_limit_u_q
#define TMC4671_STATUS_FLAGS_17_SHIFT                        17 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_18_MASK                         0x040000 // STATUS_FLAGS // ipark_cirlim_limit_u_r
#define TMC4671_STATUS_FLAGS_18_SHIFT                        18 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_19_MASK                         0x080000 // STATUS_FLAGS // not_PLL_locked
#define TMC4671_STATUS_FLAGS_19_SHIFT                        19 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_20_MASK                         0x100000 // STATUS_FLAGS // ref_sw_r
#define TMC4671_STATUS_FLAGS_20_SHIFT                        20 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_21_MASK                         0x200000 // STATUS_FLAGS // ref_sw_h
#define TMC4671_STATUS_FLAGS_21_SHIFT                        21 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_22_MASK                         0x400000 // STATUS_FLAGS // ref_sw_l
#define TMC4671_STATUS_FLAGS_22_SHIFT                        22 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_23_MASK                         0x800000 // STATUS_FLAGS // ---
#define TMC4671_STATUS_FLAGS_23_SHIFT                        23 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_24_MASK                         0x01000000 // STATUS_FLAGS // pwm_min
#define TMC4671_STATUS_FLAGS_24_SHIFT                        24 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_25_MASK                         0x02000000 // STATUS_FLAGS // pwm_max
#define TMC4671_STATUS_FLAGS_25_SHIFT                        25 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_26_MASK                         0x04000000 // STATUS_FLAGS // adc_i_clipped
#define TMC4671_STATUS_FLAGS_26_SHIFT                        26 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_27_MASK                         0x08000000 // STATUS_FLAGS // aenc_clipped
#define TMC4671_STATUS_FLAGS_27_SHIFT                        27 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_28_MASK                         0x10000000 // STATUS_FLAGS // enc_n
#define TMC4671_STATUS_FLAGS_28_SHIFT                        28 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_29_MASK                         0x20000000 // STATUS_FLAGS // enc_2_n
#define TMC4671_STATUS_FLAGS_29_SHIFT                        29 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_30_MASK                         0x40000000 // STATUS_FLAGS // aenc_n
#define TMC4671_STATUS_FLAGS_30_SHIFT                        30 // min.: 0, max.: 1, default: 0
#define TMC4671_STATUS_FLAGS_31_MASK                         0x80000000 // STATUS_FLAGS // wd_error
#define TMC4671_STATUS_FLAGS_31_SHIFT                        31 // min.: 0, max.: 1, default: 0
#define TMC4671_WARNING_MASK_MASK                            0xFFFFFFFF // STATUS_MASK //
#define TMC4671_WARNING_FIELDS                               0 // min.: 0, max.: 4294967295, default: 0s

// Initialization
void tmc4671_init(cp2130_device_t *pCpInDev);

// Data Transfer Frame
void tmc4671_write(uint8_t ubAddr, uint32_t ulData);
uint32_t tmc4671_read(uint8_t ubAddr);
void tmc4671_modify(uint8_t ubAddr, uint32_t ulData, uint32_t ulMask);

// === modes of operation ===
void tmc4671_switch_motion_mode(uint32_t ubMode);

// torque mode
void tmc4671_set_target_torque_raw(int32_t lTargetTorque);
int32_t tmc4671_get_target_torque_raw();
int32_t tmc4671_get_actual_torque_raw();
void tmc4671_set_target_torque_mA(uint16_t usTorqueMeasurementFactor, int32_t lTargetTorque);
int32_t tmc4671_get_target_torque_mA(uint16_t usTorqueMeasurementFactor);
int32_t tmc4671_get_actual_torque_mA(uint16_t usTorqueMeasurementFactor);
int32_t tmc4671_get_target_torque_flux_sum_mA(uint16_t usTorqueMeasurementFactor);
int32_t tmc4671_get_actual_torque_flux_sum_mA(uint16_t usTorqueMeasurementFactor);

// flux
void tmc4671_set_target_flux_raw(int32_t targetFlux);
int32_t tmc4671_get_target_flux_raw();
int32_t tmc4671_get_actual_flux_raw();
void tmc4671_set_target_flux_mA(uint16_t usTorqueMeasurementFactor, int32_t lTargetFlux);
int32_t tmc4671_get_target_flux_mA(uint16_t usTorqueMeasurementFactor);
int32_t tmc4671_get_actual_flux_mA(uint16_t usTorqueMeasurementFactor);

// torque/flux limit
void tmc4671_set_torque_flux_limit_mA(uint16_t usTorqueMeasurementFactor, int32_t lMax);
int32_t tmc4671_get_torque_flux_limit_mA(uint16_t usTorqueMeasurementFactor);

// velocity mode
void tmc4671_set_target_velocity(int32_t lTargetVelocity);
int32_t tmc4671_get_target_velocity();
int32_t tmc4671_get_actual_velocity();
void tmc4671_set_velocity_limit(uint32_t ulVelocityLimit);
void tmc4671_set_accelaration_limit(uint32_t ulAccelarationLimit);

// position mode
void tmc4671_set_absolute_target_position(int32_t lTargetPosition);
void tmc4671_set_relative_target_position(int32_t lRelativePosition);
int32_t tmc4671_get_target_position();
void tmc4671_set_actual_position(int32_t lActualPosition);
int32_t tmc4671_get_actual_position();

// do cyclic tasks
void tmc4671_task();

// encoder initialization
void tmc4671_start_encoder_initialization(uint8_t ubMode, uint32_t ulInitWaitTime, uint16_t usStartVoltage);
void tmc4671_update_phi_selection_and_initialize(uint8_t ubDesiredPhiESelection, uint8_t ubInitMode);

// pwm control
void tmc4671_disable_PWM();
void tmc4671_enable_PWM();
void tmc4671_disable_SVPWM();
void tmc4671_enable_SVPWM();
void tmc4671_set_PWM_freq(uint32_t ulFrequency);
uint32_t tmc4671_get_PWM_freq();
void tmc4671_set_pwm_polarity(uint8_t ubLowSideInverted, uint8_t ubHighSideInverted);
void tmc4671_set_dead_time(uint8_t ubLowDeadTime, uint8_t ubHighDeadTime);

uint8_t tmc4671_get_pole_pairs();
void tmc4671_set_pole_pairs(uint8_t ubPolePairs);

uint16_t tmc4671_get_adcI0_offset();
void tmc4671_set_adcI0_offset(uint16_t usOffset);
uint16_t tmc4671_get_adcI1_offset();
void tmc4671_set_adcI1_offset(uint16_t usOffset);

void tmc4671_set_torque_flux_PI(uint16_t usPParameter, uint16_t usIParameter);
void tmc4671_set_velocity_PI(uint16_t usPParameter, uint16_t usIParameter);
void tmc4671_set_position_PI(uint16_t usPParameter, uint16_t usIParameter);

#endif // __TMC4671_H