#ifndef __CP2130_H
#define __CP2130_H

#include <libusb-1.0/libusb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug_macros.h"
#include "utils.h"

// OTP ROM WRITE PROTECT
#define CP2130_OTP_ROM_WRITE_PROTECT    // prevents unwanted writes to the one time programmable prom

// Product and vendor ids
#define CP2130_DEFAULT_VID  0x10C4      // Silicon Labs VID
#define CP2130_DEFAULT_PID  0x87A0      // default pid Silabs allocated for cp2130

// EndPoints
#define CP2130_EP_HOST_DEVICE   0x01
#define CP2130_EP_DEVICE_HOST   0x82

// Request types
#define CP2130_REQ_DEVICE_HOST_VENDOR   0xC0
#define CP2130_REQ_HOST_DEVICE_VENDOR   0x40

// memory Key
#define CP2130_MEM_KEY  0xA5F1

// Command ID's
// Data Transfer Commands (Bulk Transfers)
#define CP2130_CMDID_READ           0x00
#define CP2130_CMDID_WRITE          0x01
#define CP2130_CMDID_WRITEREAD      0x02
#define CP2130_CMDID_READRTR        0x04
// Configuration and Control Commands (Control Transfers)
#define CP2130_CMDID_GET_CLK_DIV    0x46
#define CP2130_CMDID_SET_CLK_DIV    0x47
#define CP2130_CMDID_GET_EVNT_CNT   0x44
#define CP2130_CMDID_SET_EVNT_CNT   0x45
#define CP2130_CMDID_GET_FULL_TH    0x34
#define CP2130_CMDID_SET_FULL_TH    0x35
#define CP2130_CMDID_GET_GPIO_CS    0x24
#define CP2130_CMDID_SET_GPIO_CS    0x25
#define CP2130_CMDID_GET_GPIO_MDLVL 0x22
#define CP2130_CMDID_SET_GPIO_MDLVL 0x23
#define CP2130_CMDID_GET_GPIO_VAL   0x20
#define CP2130_CMDID_SET_GPIO_VAL   0x21
#define CP2130_CMDID_GET_RTR_STATE  0x36
#define CP2130_CMDID_RTR_STOP       0x37
#define CP2130_CMDID_GET_SPI_WORD   0x30
#define CP2130_CMDID_SET_SPI_WORD   0x31
#define CP2130_CMDID_GET_SPI_DELAY  0x32
#define CP2130_CMDID_SET_SPI_DELAY  0x33
#define CP2130_CMDID_GET_VERSION    0x11
#define CP2130_CMDID_RESET          0x10
// OTP ROM Configuration Commands (Control Transfers)
#define CP2130_CMDID_GET_LOCK_BYTE  0x6E
#define CP2130_CMDID_SET_LOCK_BYTE  0x6F
#define CP2130_CMDID_GET_MAN_STR_1  0x62
#define CP2130_CMDID_SET_MAN_STR_1  0x63
#define CP2130_CMDID_GET_MAN_STR_2  0x64
#define CP2130_CMDID_SET_MAN_STR_2  0x65
#define CP2130_CMDID_GET_PIN_CFG    0x6C
#define CP2130_CMDID_SET_PIN_CFG    0x6D
#define CP2130_CMDID_GET_PROD_STR_1 0x66
#define CP2130_CMDID_SET_PROD_STR_1 0x63
#define CP2130_CMDID_GET_PROD_STR_2 0x68
#define CP2130_CMDID_SET_PROD_STR_2 0x69
#define CP2130_CMDID_GET_PROM_CFG   0x70
#define CP2130_CMDID_SET_PROM_CFG   0x71
#define CP2130_CMDID_GET_SERIAL_STR 0x6A
#define CP2130_CMDID_SET_SERIAL_STR 0x6B
#define CP2130_CMDID_GET_USB_CFG    0x60
#define CP2130_CMDID_SET_USB_CFG    0x61

// Event Counter
#define CP2130_EVNT_CNT_OF_FLAG     0x80
#define CP2130_EVNT_CNT_RISING_EDG  0x02
#define CP2130_EVNT_CNT_FALLING_EDG 0x03
#define CP2130_EVNT_CNT_NEG_PULSE   0x06
#define CP2130_EVNT_CNT_POS_PULSE   0x07

// GPIO CS
#define CP2130_CS_CH0   0
#define CP2130_CS_CH1   1
#define CP2130_CS_CH2   2
#define CP2130_CS_CH3   3
#define CP2130_CS_CH4   4
#define CP2130_CS_CH5   5
#define CP2130_CS_CH6   6
#define CP2130_CS_CH7   7
#define CP2130_CS_CH8   8
#define CP2130_CS_CH9   9
#define CP2130_CS_CH10  10

#define CP2130_CS_MD_DIS    0x00
#define CP2130_CS_MD_EN     0x01
#define CP2130_CS_MD_EN_DIS_OTHERS 0x02

#define CP2130_CS_EN_MSK_CH0    0x0001
#define CP2130_CS_EN_MSK_CH1    0x0002
#define CP2130_CS_EN_MSK_CH2    0x0004
#define CP2130_CS_EN_MSK_CH3    0x0008
#define CP2130_CS_EN_MSK_CH4    0x0010
#define CP2130_CS_EN_MSK_CH5    0x0020
#define CP2130_CS_EN_MSK_CH6    0x0040
#define CP2130_CS_EN_MSK_CH7    0x0080
#define CP2130_CS_EN_MSK_CH8    0x0100
#define CP2130_CS_EN_MSK_CH9    0x0200
#define CP2130_CS_EN_MSK_CH10   0x0400

#define CP2130_CS_PIN_EN_MSK_CH0    0x0008
#define CP2130_CS_PIN_EN_MSK_CH1    0x0010
#define CP2130_CS_PIN_EN_MSK_CH2    0x0020
#define CP2130_CS_PIN_EN_MSK_CH3    0x0040
#define CP2130_CS_PIN_EN_MSK_CH4    0x0080
#define CP2130_CS_PIN_EN_MSK_CH5    0x0100
#define CP2130_CS_PIN_EN_MSK_CH6    0x0400
#define CP2130_CS_PIN_EN_MSK_CH7    0x0800
#define CP2130_CS_PIN_EN_MSK_CH8    0x1000
#define CP2130_CS_PIN_EN_MSK_CH9    0x2000
#define CP2130_CS_PIN_EN_MSK_CH10   0x4000

// GPIO mask mode level and value
#define CP2130_GPIO0_MSK    0x0008
#define CP2130_GPIO1_MSK    0x0010
#define CP2130_GPIO2_MSK    0x0020
#define CP2130_GPIO3_MSK    0x0040
#define CP2130_GPIO4_MSK    0x0080
#define CP2130_GPIO5_MSK    0x0100
#define CP2130_GPIO6_MSK    0x0400
#define CP2130_GPIO7_MSK    0x0800
#define CP2130_GPIO8_MSK    0x1000
#define CP2130_GPIO9_MSK    0x2000
#define CP2130_GPIO10_MSK   0x4000

#define CP2130_GPIO0    0
#define CP2130_GPIO1    1
#define CP2130_GPIO2    2
#define CP2130_GPIO3    3
#define CP2130_GPIO4    4
#define CP2130_GPIO5    5
#define CP2130_GPIO6    6
#define CP2130_GPIO7    7
#define CP2130_GPIO8    8
#define CP2130_GPIO9    9
#define CP2130_GPIO10   10

#define CP2130_GPIO_IN      0x00
#define CP2130_GPIO_OUT_OD  0x01
#define CP2130_GPIO_OUT_PP  0x02

#define CP2130_GPIO_LOW     0x00
#define CP2130_GPIO_HIGH    0x01

// RTR
#define CP2130_ABORT_READRTR    0x01

// SPI word
#define CP2130_SPI_WRD_MODE0                (CP2130_SPI_WRD_CPOL0 | CP2130_SPI_WRD_CPHA0)
#define CP2130_SPI_WRD_MODE1                (CP2130_SPI_WRD_CPOL0 | CP2130_SPI_WRD_CPHA1)
#define CP2130_SPI_WRD_MODE2                (CP2130_SPI_WRD_CPOL1 | CP2130_SPI_WRD_CPHA0)
#define CP2130_SPI_WRD_MODE3                (CP2130_SPI_WRD_CPOL1 | CP2130_SPI_WRD_CPHA1)
#define CP2130_SPI_WRD_CPHA0                0x00
#define CP2130_SPI_WRD_CPHA1                0x20
#define CP2130_SPI_WRD_CPOL0                0x00
#define CP2130_SPI_WRD_CPOL1                0x10
#define CP2130_SPI_WRD_CS_MODE_OD           0x00
#define CP2130_SPI_WRD_CS_MODE_PP           0x08
#define CP2130_SPI_WRD_CLK_12M              0x00
#define CP2130_SPI_WRD_CLK_6M               0x01
#define CP2130_SPI_WRD_CLK_3M               0x02
#define CP2130_SPI_WRD_CLK_1_5M             0x03
#define CP2130_SPI_WRD_CLK_750K             0x04
#define CP2130_SPI_WRD_CLK_375K             0x05
#define CP2130_SPI_WRD_CLK_187_5K           0x06
#define CP2130_SPI_WRD_CLK_93_8K            0x07

#define CP2130_SPI_CH0   0
#define CP2130_SPI_CH1   1
#define CP2130_SPI_CH2   2
#define CP2130_SPI_CH3   3
#define CP2130_SPI_CH4   4
#define CP2130_SPI_CH5   5
#define CP2130_SPI_CH6   6
#define CP2130_SPI_CH7   7
#define CP2130_SPI_CH8   8
#define CP2130_SPI_CH9   9
#define CP2130_SPI_CH10  10

// Lock Byte
#define CP2130_LOCK_PROD_STR1   0x0001
#define CP2130_LOCK_PROD_STR2   0x0002
#define CP2130_LOCK_SERIAL_STR1 0x0004
#define CP2130_LOCK_PIN_CFG     0x0008
#define CP2130_LOCK_VID         0x0100
#define CP2130_LOCK_PID         0x0200
#define CP2130_LOCK_MAX_POW     0x0400
#define CP2130_LOCK_POW_MODE    0x0800
#define CP2130_LOCK_RELEASE_VER 0x1000
#define CP2130_LOCK_MAN_STR2    0x2000
#define CP2130_LOCK_MAN_STR1    0x4000
#define CP2130_LOCK_TRANS_PRTY  0x8000

// SPI delay
#define CP2130_SPI_DELAY_MSK

// USB Config
#define CP2130_USP_MAX_POW_MA(ma)       ((uint8_t)(ma / 2u))
#define CP2130_USB_BUS_POW_REG_EN       0x00
#define CP2130_USB_SELF_POW_REG_DIS     0x01
#define CP2130_USB_SELF_POW_REG_EN      0x02
#define CP2130_USB_PRIORITY_WRITE       0x01
#define CP2130_USB_PRIORITY_READ        0x00
#define CP2130_USB_CFG_MSK_VID          0x01
#define CP2130_USB_CFG_MSK_PID          0x02
#define CP2130_USB_CFG_MSK_MAX_POW      0x04
#define CP2130_USB_CFG_MSK_POW_MODE     0x08
#define CP2130_USB_CFG_MSK_VERSION      0x10
#define CP2130_USB_CFG_MSK_PRIORITY     0x80

// device handle
typedef struct cp2130_device_t
{
    uint8_t ubKernelAttached;
    uint16_t usTimeout;
    libusb_device_handle *pDev;
} cp2130_device_t;

// initializer and destructor
cp2130_device_t *cp2130_init(libusb_context *ctx, uint16_t usVid, uint16_t usPid);
void cp2130_free(cp2130_device_t *pCpDev);

inline void cp2130_set_timeout(cp2130_device_t *pCpDev, uint16_t usTimeout)
{
    pCpDev->usTimeout = usTimeout;
}

// Data Transfer Commands (Bulk Transfers)
void cp2130_spi_transfer(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen);
void cp2130_spi_write(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen);
void cp2130_spi_read(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen);
void cp2130_spi_read_rtr(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen);

// Configuration and Control Commands (Control Transfers)
void cp2130_reset(cp2130_device_t *pCpDev);

void cp2130_get_clockdiv(cp2130_device_t *pCpDev, uint8_t *pubClockDiv);
void cp2130_set_clockdiv(cp2130_device_t *pCpDev, uint8_t ubClockDiv);

void cp2130_get_event_counter(cp2130_device_t *pCpDev, uint8_t *pubMode, uint16_t *pusCount);
void cp2130_set_event_counter(cp2130_device_t *pCpDev, uint8_t ubMode, uint16_t usCount);

void cp2130_get_full_threshold(cp2130_device_t *pCpDev, uint8_t *pubThreshold);
void cp2130_set_full_threshold(cp2130_device_t *pCpDev, uint8_t ubThreshold);

void cp2130_get_gpio_cs(cp2130_device_t *pCpDev, uint16_t *pusCsEn, uint16_t *pusPinCsEn);
void cp2130_set_gpio_cs(cp2130_device_t *pCpDev, uint8_t usCh, uint8_t usCtrl);

void cp2130_get_gpio_mode_level(cp2130_device_t *pCpDev, uint16_t *pusLevel, uint16_t *pusMode);
void cp2130_set_gpio_mode_level(cp2130_device_t *pCpDev, uint8_t ubIndex, uint8_t ubMode, uint8_t ubLevel);

void cp2130_get_gpio_values(cp2130_device_t *pCpDev, uint16_t *pusLevel);
void cp2130_set_gpio_values(cp2130_device_t *pCpDev, uint16_t usLevel, uint16_t usMask);

void cp2130_get_rtr_state(cp2130_device_t *pCpDev, uint8_t *pubActive);
void cp2130_set_rtr_stop(cp2130_device_t *pCpDev, uint8_t ubAbort);

void cp2130_get_spi_word(cp2130_device_t *pCpDev, uint8_t *pubChNword);
void cp2130_set_spi_word(cp2130_device_t *pCpDev, uint8_t ubCh, uint8_t ubChWord);

void cp2130_get_spi_delay(cp2130_device_t *pCpDev, uint8_t ubSpiCh, uint8_t *pubMask, uint16_t *pusInBDelay, uint16_t *pusPostDelay, uint16_t *pusPreDelay);
void cp2130_set_spi_delay(cp2130_device_t *pCpDev, uint8_t ubSpiCh, uint8_t ubMask, uint16_t usInBDelay, uint16_t usPostDelay, uint16_t usPreDelay);

void cp2130_get_version(cp2130_device_t *pCpDev, uint8_t *pubMajor, uint8_t *pubMinor);

// OTP ROM Configuration Commands (Control Transfers)
void cp2130_get_lock_byte(cp2130_device_t *pCpDev, uint16_t *pusLock);
void cp2130_set_lock_byte(cp2130_device_t *pCpDev, uint16_t usLock);

void cp2130_get_manufacturer_string(cp2130_device_t *pCpDev, uint8_t *pubStr);
void cp2130_set_manufacturer_string(cp2130_device_t *pCpDev, uint8_t *pubStr);

void cp2130_get_prod_string(cp2130_device_t *pCpDev, uint8_t *pubStr);
void cp2130_set_prod_string(cp2130_device_t *pCpDev, uint8_t *pubStr);

void cp2130_get_serial(cp2130_device_t *pCpDev, uint8_t *pubStr);
void cp2130_set_serial(cp2130_device_t *pCpDev, uint8_t *pubStr);

void cp2130_get_pin_cfg(cp2130_device_t *pCpDev, uint8_t *pubPinCfg);
void cp2130_set_pin_cfg(cp2130_device_t *pCpDev, uint8_t *pubPinCfg);

void cp2130_get_prom_cfg(cp2130_device_t *pCpDev, uint8_t ubBlkIndex, uint8_t *pubBlk);
void cp2130_set_prom_cfg(cp2130_device_t *pCpDev, uint8_t ubBlkIndex,uint8_t *pubBlk);

void cp2130_get_usb_cfg(cp2130_device_t *pCpDev, uint16_t *pusVid, uint16_t *pusPid, uint8_t *pubMaxPow, uint8_t *pubPowMode, uint8_t *pubMajorRelease, uint8_t *pubMinorRelease, uint8_t *pubTransferPriority);
void cp2130_set_usb_cfg(cp2130_device_t *pCpDev, uint16_t usVid, uint16_t usPid, uint8_t ubMaxPow, uint8_t ubPowMode, uint8_t ubMajorRelease, uint8_t ubMinorRelease, uint8_t ubTransferPriority, uint8_t ubMask); // untested

#endif // __CP2130_H