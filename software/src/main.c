#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <stdio.h>

#include <signal.h>

#include "utils.h"
#include "debug_macros.h"
#include "cp2130.h"
#include "tmc4671.h"

static volatile sig_atomic_t gxKeepRunning = 1;

void sig_handler(int signo);

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    signal(SIGINT, sig_handler);

    libusb_context *ctx = NULL;
    if(libusb_init(&ctx) < 0)
    {
		printf("error: failed in libusb init\n\r");
        return EXIT_FAILURE;
	}
	libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_WARNING);

    cp2130_device_t *spi = cp2130_init(ctx, CP2130_DEFAULT_VID, CP2130_DEFAULT_PID);

    if(!spi)
    {
        printf("error: failed to initialize device\n\r");
        return EXIT_FAILURE;
    }

    uint8_t ubMajor, ubMinor;
    cp2130_get_version(spi, &ubMajor, &ubMinor);
    printf("Version: %0d.%0d\n\r", ubMajor, ubMinor);

    cp2130_set_usb_cfg(spi, 0x10C4, 0x87A0, CP2130_USP_MAX_POW_MA(500u), CP2130_USB_BUS_POW_REG_EN, 3, 0, CP2130_USB_PRIORITY_WRITE, CP2130_USB_CFG_MSK_MAX_POW);

    uint16_t usVid, usPid;
    uint8_t ubMaxPow, ubPowMode, ubTransferPriority;
    cp2130_get_usb_cfg(spi, &usVid, &usPid, &ubMaxPow, &ubPowMode, &ubMajor, &ubMinor, &ubTransferPriority);
    printf("VID: 0x%04X\n\r", usVid);
    printf("PID: 0x%04X\n\r", usPid);
    printf("Max Power: %dma\n\r", ubMaxPow * 2);
    printf("Power Mode: 0x%02X\n\r", ubPowMode);
    printf("Transfer Priority: 0x%02X\n\r", ubTransferPriority);

    uint8_t ubStr[63];
    memset(ubStr, 0x00, 63);
    cp2130_get_manufacturer_string(spi, ubStr);
    printf("Manufacturer String: %s\n\r", ubStr);
    memset(ubStr, 0x00, 63);
    cp2130_get_prod_string(spi, ubStr);
    printf("Product String: %s\n\r", ubStr);
    memset(ubStr, 0x00, 63);
    cp2130_get_serial(spi, ubStr);
    printf("Serial: %s\n\r", ubStr);

    /*
    uint8_t ubPinCfg[20];
    cp2130_get_pin_cfg(spi, ubPinCfg);
    for(uint8_t i = 0; i < 11; i++)
    {
        printf("GPIO%d OTP ROM cfg: 0x%02X\n\r", i, ubPinCfg[i]);
    }*/

    cp2130_set_gpio_mode_level(spi, CP2130_GPIO0, CP2130_GPIO_OUT_PP, CP2130_GPIO_HIGH);
    //cp2130_set_gpio_mode_level(spi, CP2130_GPIO5, CP2130_GPIO_OUT_PP, CP2130_GPIO_LOW);
    //cp2130_set_gpio_mode_level(spi, CP2130_GPIO7, CP2130_GPIO_OUT_PP, CP2130_GPIO_LOW);

    //cp2130_set_gpio_values(spi, 0x00, CP2130_GPIO7_MSK);

    cp2130_set_event_counter(spi, CP2130_EVNT_CNT_FALLING_EDG, 0);

    cp2130_set_clockdiv(spi, 3); // 24 MHz / 3 = 8MHz

    cp2130_set_spi_word(spi, CP2130_SPI_CH0, CP2130_SPI_WRD_MODE3 | CP2130_SPI_WRD_CS_MODE_PP | CP2130_SPI_WRD_CLK_750K);

    cp2130_set_gpio_cs(spi, CP2130_CS_CH0, CP2130_CS_MD_EN_DIS_OTHERS);

    //sleep(5);

    uint8_t ubEvntMd;
    uint16_t ubCount;
    cp2130_get_event_counter(spi, &ubEvntMd, &ubCount);
    printf("Event Counter: %d\n\r\n\r", ubCount);

    tmc4671_init(spi);

    printf("Chip info:\n\r");

    tmc4671_write(TMC4671_CHIPINFO_ADDR, TMC4671_CHIPINFO_ADDR_SI_TYPE);
    uint32_t ulSiType = tmc4671_read(TMC4671_CHIPINFO_DATA);
    printf("SI Type: %c%c%c%c\n\r", (ulSiType >> 24) & 0xFF, (ulSiType >> 16) & 0xFF, (ulSiType >> 8) & 0xFF, ulSiType & 0xFF);

    tmc4671_write(TMC4671_CHIPINFO_ADDR, TMC4671_CHIPINFO_ADDR_SI_VERSION);
    uint32_t ulVersion = tmc4671_read(TMC4671_CHIPINFO_DATA);
    printf("SI Version: %hu.%hu\n\r\n\r", (ulVersion >> 16) & 0xFF, ulVersion & 0xFF);
/*
    for(uint8_t i = 0x00; i < 0x7D; i++)
    {
        printf("0x%02X: %08X\n\r", i,tmc4671_read(i));
    }
*/
    printf("\n\rPWM freq: %huHz\n\r", tmc4671_get_PWM_freq());
    printf("ENABLE_IN: %d\n\r", !!(tmc4671_read(TMC4671_INPUTS_RAW) & TMC4671_ENABLE_IN_RAW_MASK));

    printf("STATUS_FLAGS: %08X\n\r\n\r", tmc4671_read(TMC4671_STATUS_FLAGS));

    printf("Motor Voltage: %.2f V\n\r", tmc4671_get_vm_v());

    // Type of motor &  PWM configuration
    printf("Type of motor &  PWM configuration\n\r");
    tmc4671_set_pole_pairs(7);
    tmc4671_set_motor_type(TMC4671_THREE_PHASE_BLDC_MOTOR);
    tmc4671_set_pwm_polarity(0, 0);
    tmc4671_set_PWM_freq(25000);
	tmc4671_set_dead_time(20, 20);
    tmc4671_enable_PWM();

    // ADC configuration
    printf("ADC configuration\n\r");
    tmc4671_write(TMC4671_dsADC_MDEC_B_MDEC_A, (0x014E << TMC4671_DSADC_MDEC_A_SHIFT) | (0x014E << TMC4671_DSADC_MDEC_B_SHIFT)); // Decimation configuration register.
    tmc4671_modify(TMC4671_DS_ANALOG_INPUT_STAGE_CFG, (4 << TMC4671_ADC_VM_SHIFT), TMC4671_ADC_VM_MASK);
	tmc4671_modify(TMC4671_dsADC_MCFG_B_MCFG_A, (1 << TMC4671_SEL_NCLK_MCLK_I_A_SHIFT) | (1 << TMC4671_SEL_NCLK_MCLK_I_B_SHIFT), TMC4671_SEL_NCLK_MCLK_I_A_MASK | TMC4671_SEL_NCLK_MCLK_I_B_MASK);
	tmc4671_write(TMC4671_dsADC_MCLK_A, 0x20000000);
	tmc4671_write(TMC4671_dsADC_MCLK_B, 0x00000000);

    //ADC scale & offset
    printf("ADC scale & offset\n\r");
    tmc4671_write(TMC4671_ADC_I0_SCALE_OFFSET, (33000 & TMC4671_ADC_I0_OFFSET_MASK) | (((uint16_t)(-256) << TMC4671_ADC_I0_SCALE_SHIFT) & TMC4671_ADC_I0_SCALE_MASK));
    tmc4671_write(TMC4671_ADC_I1_SCALE_OFFSET, (33000 & TMC4671_ADC_I1_OFFSET_MASK) | (((uint16_t)(-256) << TMC4671_ADC_I1_SCALE_SHIFT) & TMC4671_ADC_I1_SCALE_MASK));

    tmc4671_write(TMC4671_ADC_I_SELECT, (0 & TMC4671_ADC_I0_SELECT_MASK) | ((1 << TMC4671_ADC_I1_SHIFT) & TMC4671_ADC_I1_SELECT_MASK) | ((0 << TMC4671_ADC_I_UX_SELECT_SHIFT) & TMC4671_ADC_I_UX_SELECT_MASK) | ((2 << TMC4671_ADC_I_V_SELECT_SHIFT) & TMC4671_ADC_I_V_SELECT_MASK) | ((1 << TMC4671_ADC_I_WY_SELECT_SHIFT) & TMC4671_ADC_I_WY_SELECT_MASK));

    printf("raw adc I0: 0x%04X\n\r", tmc4671_get_I0_raw());
    printf("raw adc I0(V): %.0f mV\n\r", (float)tmc4671_get_I0_raw() * 0.076295109f);
    printf("scaled adc IUX: %d\n\r", (int16_t)(tmc4671_read(TMC4671_ADC_IWY_IUX) & TMC4671_ADC_IUX_MASK));


    printf("raw adc I1: 0x%04X\n\r", tmc4671_get_I1_raw());
    printf("raw adc I1(V): %.0f mV\n\r", (float)tmc4671_get_I1_raw() * 0.076295109f);
    printf("scaled adc IWY: %d\n\r", (int16_t)((tmc4671_read(TMC4671_ADC_IWY_IUX) & TMC4671_ADC_IWY_MASK) >> TMC4671_ADC_IWY_SHIFT));

    printf("scaled adc IV: %d\n\r", (int16_t)(tmc4671_read(TMC4671_ADC_IV) & TMC4671_ADC_IV_MASK));

	// Encoder configuration
    printf("Encoder configuration\n\r");
	tmc4671_write(TMC4671_ABN_DECODER_MODE, 0x00000000); // Control bits how to handle ABN decoder signals.
	tmc4671_write(TMC4671_ABN_DECODER_PPR, 8192);
	tmc4671_write(TMC4671_ABN_DECODER_COUNT, 0x00000000);
	tmc4671_write(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);

    // set limits
    printf("set limits\n\r");
    tmc4671_set_uq_ud_limit(10000); // 23767
    tmc4671_set_torque_flux_limit(1000);
    tmc4671_set_accelaration_limit(500);
    tmc4671_set_velocity_limit(1000);

    // selections
    printf("selections\n\r");
	tmc4671_write(TMC4671_VELOCITY_SELECTION, 2);  // phi_m_abn
	tmc4671_write(TMC4671_POSITION_SELECTION, 2);  // phi_m_abn

    // set PI parameter
    printf("set PI parameter\n\r");
    tmc4671_set_torque_flux_PI(256, 256);  // P and I parameter for the flux regulator and for the torque regulator.
    tmc4671_set_velocity_PI(256, 256);    // P and I parameter for the velocity regulator.
    tmc4671_set_position_PI(256, 256);     // P parameter for the position regulator.

    // init encoder
    printf("init encoder\n\r");
    tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_UQ_UD_EXT);
    tmc4671_write(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
    tmc4671_write(TMC4671_PHI_E_SELECTION, TMC4671_PHI_E_EXTERNAL);
    tmc4671_write(TMC4671_PHI_E_EXT, 0x00000000);
    tmc4671_write(TMC4671_UQ_UD_EXT, 5000);
    sleep(1);

    // clear abn decoder count
    printf("clear abn decoder count\n\r");
    tmc4671_write(TMC4671_ABN_DECODER_COUNT, 0x00000000);

    // feedback selection
    printf("feedback selection\n\r");
    tmc4671_write(TMC4671_PHI_E_SELECTION, TMC4671_PHI_E_SELECTION_PHI_E_ABN);
    tmc4671_write(TMC4671_VELOCITY_SELECTION, TMC4671_VELOCITY_PHI_M_ABN);

    // switch to torque mode
    printf("switch to torque mode\n\r");
    tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_TORQUE);

    // rotate right
    printf("rotate right\n\r");
    //tmc4671_write(TMC4671_PID_TORQUE_FLUX_TARGET, 0x03E80000);
    sleep(2);

    // rotate left
    printf("rotate left\n\r");
    //tmc4671_write(TMC4671_PID_TORQUE_FLUX_TARGET, 0xFC180000);
    sleep(2);

    // stop
    printf("stop\n\r");
    tmc4671_write(TMC4671_PID_TORQUE_FLUX_TARGET, 0x00000000);

/*
    tmc4671_switch_motion_mode(TMC4671_MOTION_MODE_UQ_UD_EXT);
    tmc4671_write(TMC4671_UQ_UD_EXT, 6000 & TMC4671_UD_EXT_MASK);
    tmc4671_write(TMC4671_OPENLOOP_VELOCITY_TARGET, 750);
    tmc4671_write(TMC4671_OPENLOOP_ACCELERATION, 100);

    tmc4671_write(TMC4671_PHI_E_SELECTION, TMC4671_PHI_E_SELECTION_PHI_E_OPENLOOP);

    tmc4671_update_phi_selection_and_initialize(TMC4671_PHI_E_SELECTION_PHI_E_ABN);

    printf("\n\rTMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET %d\n\r", tmc4671_read(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET));
*/
    printf("\n\ropen loop velocity actual %d\n\r", tmc4671_read(TMC4671_OPENLOOP_VELOCITY_ACTUAL));
    printf("\n\rPHI_E %d\n\r", tmc4671_read(TMC4671_PHI_E));
    printf("\n\rTMC4671_PID_FLUX_P_FLUX_I %d\n\r", tmc4671_read(TMC4671_PID_FLUX_P_FLUX_I));
    printf("\n\rTMC4671_PID_VELOCITY_TARGET %d\n\r", tmc4671_read(TMC4671_PID_VELOCITY_TARGET));

    FILE *fp;

    fp = fopen("./tests/currents.csv", "w");

    if(!fp)
    {
        gxKeepRunning = 0;

        printf("could not open file.\n\r");
    }
    else
    {
        fprintf(fp, "IU:IV:IW:ENC\n\r");
    }

    gxKeepRunning = 0;

    while(gxKeepRunning)
    {
        fprintf(fp, "%d:%d:%d:%d\n\r", (int16_t)(tmc4671_read(TMC4671_ADC_IWY_IUX) & TMC4671_ADC_IUX_MASK), (int16_t)(tmc4671_read(TMC4671_ADC_IV) & TMC4671_ADC_IV_MASK), (int16_t)((tmc4671_read(TMC4671_ADC_IWY_IUX) & TMC4671_ADC_IWY_MASK) >> TMC4671_ADC_IWY_SHIFT), tmc4671_read(TMC4671_ABN_DECODER_COUNT) & TMC4671_ABN_DECODER_COUNT_MASK);
    }

    printf("leaving...\n\r");

    if(fp)
        fclose(fp);


    cp2130_free(spi);

	if(ctx)
		libusb_exit(ctx);

    return EXIT_SUCCESS;
}

void sig_handler(int signo)
{
    gxKeepRunning = 0;
}