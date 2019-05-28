#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "debug_macros.h"
#include "cp2130.h"
#include "tmc4671.h"

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    libusb_context *ctx = NULL;
    if(libusb_init(&ctx) < 0)
    {
		printf("error: failed in libusb init\n\r");
        return -1;
	}
	libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_WARNING);

    cp2130_device_t *spi = cp2130_init(ctx, CP2130_DEFAULT_VID, CP2130_DEFAULT_PID);

    if(!spi)
    {
        printf("error: failed to initialize device\n\r");
        return -1;
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

    uint8_t ubPinCfg[20];
    cp2130_get_pin_cfg(spi, ubPinCfg);
    for(uint8_t i = 0; i < 11; i++)
    {
        printf("GPIO%d OTP ROM cfg: 0x%02X\n\r", i, ubPinCfg[i]);
    }

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
    printf("Event Counter: %d\n\r", ubCount);

    tmc4671_init(spi);

    printf("Chip info:\n\r");

    tmc4671_write(TMC4671_ADDR_CHIPINFO_ADDR, TMC4671_CHIPINFO_SI_TYPE);
    uint32_t ulSiType = tmc4671_read(TMC4671_ADDR_CHIPINFO_DATA);
    printf("SI Type: %c%c%c%c\n\r", (ulSiType >> 24) & 0xFF, (ulSiType >> 16) & 0xFF, (ulSiType >> 8) & 0xFF, ulSiType & 0xFF);

    tmc4671_write(TMC4671_ADDR_CHIPINFO_ADDR, TMC4671_CHIPINFO_SI_VERSION);
    uint32_t ulVersion = tmc4671_read(TMC4671_ADDR_CHIPINFO_DATA);
    printf("SI Version: %hu.%hu\n\r", (ulVersion >> 16) & 0xFF, ulVersion & 0xFF);

    printf("PWM max cnt: %hu\n\r", tmc4671_read(TMC4671_ADDR_PWM_MAXCNT) & 0xFFFF);

/*
    printf("Transfering 100 Bytes...\n\r");
    uint8_t ubBuf[256];
    for(uint8_t ubCount = 0; ubCount < 100; ubCount++)
    {
        ubBuf[ubCount] = ubCount;
    }
    cp2130_spi_transfer(spi, ubBuf, 100);
    printf("got:\n\r");
    for(uint8_t ubCount = 0; ubCount < 100; ubCount++)
    {
        printf(" 0x%02X", ubBuf[ubCount]);
    }

    printf("\n\rWriting 255 Bytes...\n\r");
    for(uint8_t ubCount = 0; ubCount < 255; ubCount++)
    {
        ubBuf[ubCount] = ubCount;
    }
    cp2130_spi_write(spi, ubBuf, 255);

    printf("\n\rReading 255 Bytes...\n\r");
    memset(ubBuf, 0x00, 256);
    cp2130_spi_read(spi, ubBuf, 255);
    printf("got:\n\r");
    for(uint8_t ubCount = 0; ubCount < 255; ubCount++)
    {
        printf(" 0x%02X", ubBuf[ubCount]);
    }

    ubBuf[0] = 0x03;    // mcp2515 read
    ubBuf[1] = 0b00001110;    // addr
    ubBuf[2] = 0x00;  // data

    cp2130_spi_transfer(spi, ubBuf, 3);

    printf("\n\rGot 0x%02X from mcp2515\n\r", ubBuf[2]);
*/
    cp2130_free(spi);

	if(ctx)
		libusb_exit(ctx);
}