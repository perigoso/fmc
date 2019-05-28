// SiLabs CP2130 via LibUSB

#include "cp2130.h"

#ifdef __cplusplus
extern "C" {
#endif

cp2130_device_t *cp2130_init(libusb_context *ctx, uint16_t usVid, uint16_t usPid)
{
    if(!ctx)
    {
        DBGPRINTLN_CTX("error: invalid libUSB context.");
        return NULL;
    }

    DBGPRINTLN_CTX("allocating space for device...");
    cp2130_device_t *pCp2130_dev = (cp2130_device_t *)malloc(sizeof(cp2130_device_t));
    if(!pCp2130_dev)
    {
        DBGPRINTLN_CTX("error: device space allocation failed.");
        return NULL;
    }

    pCp2130_dev->pDev = NULL;
    pCp2130_dev->ubKernelAttached = 0;
    pCp2130_dev->usTimeout = 0;

    DBGPRINTLN_CTX("opening device...");
    pCp2130_dev->pDev = libusb_open_device_with_vid_pid(ctx, usVid, usPid);
    if(pCp2130_dev->pDev == NULL)
    {
        DBGPRINTLN_CTX("error: libusb could not open device.");
        free(pCp2130_dev);
        return NULL;
    }

    DBGPRINTLN_CTX("checking if attached to kernel...");
    if(libusb_kernel_driver_active(pCp2130_dev->pDev, 0) != 0)
    {
        DBGPRINTLN_CTX("device was atached to kernel, detaching...");
        libusb_detach_kernel_driver(pCp2130_dev->pDev, 0);
        pCp2130_dev->ubKernelAttached = 1;
    }

    DBGPRINTLN_CTX("claiming interface...");
    if(libusb_claim_interface(pCp2130_dev->pDev, 0) < 0)
    {
        DBGPRINTLN_CTX("error: failed to claim interface.");
        if(pCp2130_dev->ubKernelAttached)
            libusb_attach_kernel_driver(pCp2130_dev->pDev, 0);

        libusb_close(pCp2130_dev->pDev);
        free(pCp2130_dev);
        return NULL;
    }

    DBGPRINTLN_CTX("initialization success.");
    return pCp2130_dev;
}
void cp2130_free(cp2130_device_t *pCpDev)
{
    if(!pCpDev)
    {
        DBGPRINTLN_CTX("error: invalid device.");
        return;
    }

    libusb_release_interface(pCpDev->pDev, 0);

    if(pCpDev->ubKernelAttached)
    {
        DBGPRINTLN_CTX("reattaching kernel...");
        libusb_attach_kernel_driver(pCpDev->pDev, 0);
    }

    libusb_close(pCpDev->pDev);

    free(pCpDev);

    DBGPRINTLN_CTX("device freed...");
}

static void cp2130_control_transfer(cp2130_device_t *pCpDev, uint8_t ubReqType, uint8_t ubRequest, uint8_t * pubData, uint16_t usLen, uint16_t usWvalue, uint16_t usWindex)
{
    if(!pCpDev)
    {
        DBGPRINTLN_CTX("error: invalid device");
        return;
    }

    int32_t ulR = libusb_control_transfer(pCpDev->pDev, ubReqType, ubRequest, usWvalue, usWindex, pubData, usLen, pCpDev->usTimeout);
    if(ulR < 0)
        DBGPRINTLN_CTX("error: failed with return code %d.", ulR);
}
static void cp2130_bulk_transfer(cp2130_device_t *pCpDev, uint8_t ubEp, uint8_t * pubData, uint32_t ulLen)
{
    if(!pCpDev)
    {
        DBGPRINTLN_CTX("error: invalid device");
        return;
    }

    uint32_t ulTransferred;
    uint32_t ulR = libusb_bulk_transfer(pCpDev->pDev, ubEp, pubData, ulLen, (int *)(&ulTransferred), pCpDev->usTimeout);
    if(ulR < 0)
    {
        DBGPRINTLN_CTX("error: failed with return code %d", ulR);
        return;
    }

    if(ulTransferred < ulLen)
        DBGPRINTLN_CTX("error: failed short, got %d bytes of %d requested", ulTransferred, ulLen);
}

// Data Transfer Commands (Bulk Transfers)
void cp2130_spi_transfer(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen)
{
    if(ulLen > 120)
    {
        DBGPRINTLN_CTX("warning: cannot transfer more than 120 bytes, use write or read for longer transfers.");
        return;
    }

    if(!pubData)
    {
        DBGPRINTLN_CTX("error: null data buffer.");
        return;
    }

    uint8_t *pubCmdBuf = (uint8_t *)malloc(MIN(ulLen + 8, 64u));
    if(!pubCmdBuf)
    {
        DBGPRINTLN_CTX("error: failed to allocate Buffer. ");
        return;
    }
    memset(pubCmdBuf, 0x00, MIN(ulLen + 8, 64u));

	pubCmdBuf[0] = 0x00;  // reserved must be 0x00
    pubCmdBuf[1] = 0x00;  // reserved must be 0x00
    pubCmdBuf[2] = CP2130_CMDID_WRITEREAD;  // Command ID
    pubCmdBuf[3] = 0x00;  // reserved
    //pubDataBuf[7] = (uint8_t)((ulLen >> 24) & 0xFF); pubDataBuf[6] = (uint8_t)((ulLen >> 16) & 0xFF); pubDataBuf[5] = (uint8_t)((ulLen >> 8) & 0xFF);  // Length
    pubCmdBuf[4] = (uint8_t)(ulLen & 0xFF);  // Length Little Endian (since max is 120 we only need this)

    memcpy((pubCmdBuf + 8u), pubData, MIN(ulLen, 56u));

	cp2130_bulk_transfer(pCpDev, CP2130_EP_HOST_DEVICE, pubCmdBuf, MIN(ulLen + 8u, 64u));
	cp2130_bulk_transfer(pCpDev, CP2130_EP_DEVICE_HOST, pubData, MIN(ulLen, 56u));

    for(uint32_t ulTransfered = 56; ulTransfered < ulLen; ulTransfered += 64u)
    {
        uint32_t ulToTransfer = MIN(ulLen - ulTransfered, 64u);
        cp2130_bulk_transfer(pCpDev, CP2130_EP_HOST_DEVICE, pubData + ulTransfered, ulToTransfer);
		cp2130_bulk_transfer(pCpDev, CP2130_EP_DEVICE_HOST, pubData + ulTransfered, ulToTransfer);
    }
}
void cp2130_spi_write(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen)
{
    if(!pubData)
    {
        DBGPRINTLN_CTX("error: null data buffer.");
        return;
    }

    uint8_t *pubCmdBuf = (uint8_t *)malloc(MIN(ulLen + 8, 64u));
    if(!pubCmdBuf)
    {
        DBGPRINTLN_CTX("error: failed to allocate Buffer. ");
        return;
    }
    memset(pubCmdBuf, 0x00, MIN(ulLen + 8, 64u));

	pubCmdBuf[0] = 0x00;  // reserved must be 0x00
    pubCmdBuf[1] = 0x00;  // reserved must be 0x00
    pubCmdBuf[2] = CP2130_CMDID_WRITE;  // Command ID
    pubCmdBuf[3] = 0x00;  // reserved
    pubCmdBuf[7] = (uint8_t)((ulLen >> 24) & 0xFF);    // Length
    pubCmdBuf[6] = (uint8_t)((ulLen >> 16) & 0xFF);    // Length
    pubCmdBuf[5] = (uint8_t)((ulLen >> 8) & 0xFF);     // Length
    pubCmdBuf[4] = (uint8_t)(ulLen & 0xFF);        // Length Little Endian

    memcpy((pubCmdBuf + 8u), pubData, MIN(ulLen, 56u));

	// send the command and first 56 or fewer bytes
	cp2130_bulk_transfer(pCpDev, CP2130_EP_HOST_DEVICE, pubCmdBuf, MIN(ulLen + 8u, 64u));
	// send the remainder
	uint32_t count = 56;
	for(uint32_t ulTransfered = 56; ulTransfered < ulLen; ulTransfered += 64u)
    {
        cp2130_bulk_transfer(pCpDev, CP2130_EP_HOST_DEVICE, pubData + ulTransfered, MIN(ulLen - ulTransfered, 64u));
    }
}
void cp2130_spi_read(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen)
{
    if(!pubData)
    {
        DBGPRINTLN_CTX("error: null data buffer.");
        return;
    }

    uint8_t *pubCmdBuf = (uint8_t *)malloc(MIN(ulLen + 8, 64u));
    if(!pubCmdBuf)
    {
        DBGPRINTLN_CTX("error: failed to allocate Buffer. ");
        return;
    }
    memset(pubCmdBuf, 0x00, MIN(ulLen + 8, 64u));

	pubCmdBuf[0] = 0x00;  // reserved must be 0x00
    pubCmdBuf[1] = 0x00;  // reserved must be 0x00
    pubCmdBuf[2] = CP2130_CMDID_READ;  // Command ID
    pubCmdBuf[3] = 0x00;  // reserved
    pubCmdBuf[7] = (uint8_t)((ulLen >> 24) & 0xFF);    // Length
    pubCmdBuf[6] = (uint8_t)((ulLen >> 16) & 0xFF);    // Length
    pubCmdBuf[5] = (uint8_t)((ulLen >> 8) & 0xFF);     // Length
    pubCmdBuf[4] = (uint8_t)(ulLen & 0xFF);        // Length Little Endian

	cp2130_bulk_transfer(pCpDev, CP2130_EP_HOST_DEVICE, pubCmdBuf, 8u);   // send the command
	// fetch the data
	for(uint32_t ulTransfered = 0; ulTransfered < ulLen; ulTransfered += 64u)
    {
		cp2130_bulk_transfer(pCpDev, CP2130_EP_DEVICE_HOST, pubData + ulTransfered, MIN(ulLen - ulTransfered, 64u));
	}
}
void cp2130_spi_read_rtr(cp2130_device_t *pCpDev, uint8_t *pubData, uint32_t ulLen)
{
    if(!pubData)
    {
        DBGPRINTLN_CTX("error: null data buffer.");
        return;
    }

    uint8_t *pubCmdBuf = (uint8_t *)malloc(MIN(ulLen + 8, 64u));
    if(!pubCmdBuf)
    {
        DBGPRINTLN_CTX("error: failed to allocate Buffer. ");
        return;
    }
    memset(pubCmdBuf, 0x00, MIN(ulLen + 8, 64u));

	pubCmdBuf[0] = 0x00;  // reserved must be 0x00
    pubCmdBuf[1] = 0x00;  // reserved must be 0x00
    pubCmdBuf[2] = CP2130_CMDID_READRTR;  // Command ID
    pubCmdBuf[3] = 0x00;  // reserved
    pubCmdBuf[7] = (uint8_t)((ulLen >> 24) & 0xFF);    // Length
    pubCmdBuf[6] = (uint8_t)((ulLen >> 16) & 0xFF);    // Length
    pubCmdBuf[5] = (uint8_t)((ulLen >> 8) & 0xFF);     // Length
    pubCmdBuf[4] = (uint8_t)(ulLen & 0xFF);        // Length Little Endian

	cp2130_bulk_transfer(pCpDev, CP2130_EP_HOST_DEVICE, pubCmdBuf, 8u);   // send the command
	// fetch the data
	for(uint32_t ulTransfered = 0; ulTransfered < ulLen; ulTransfered += 64u)
    {
		cp2130_bulk_transfer(pCpDev, CP2130_EP_DEVICE_HOST, pubData + ulTransfered, MIN(ulLen - ulTransfered, 64u));
	}
}

// Configuration and Control Commands (Control Transfers)
void cp2130_reset(cp2130_device_t *pCpDev)
{
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_RESET, NULL, 0, 0, 0);
}

void cp2130_get_clockdiv(cp2130_device_t *pCpDev, uint8_t *pubClockDiv)
{
    if(pubClockDiv)
        cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_CLK_DIV, pubClockDiv, 1, 0, 0);
}
void cp2130_set_clockdiv(cp2130_device_t *pCpDev, uint8_t ubClockDiv)
{
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_CLK_DIV, &ubClockDiv, 1, 0, 0);
}

void cp2130_get_event_counter(cp2130_device_t *pCpDev, uint8_t *pubMode, uint16_t *pusCount)
{
    uint8_t ubBuf[3] = {0, 0, 0};

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_EVNT_CNT, ubBuf, 3, 0, 0);

    if(pubMode)
        *pubMode = ubBuf[0];

    if(pusCount)
    {
        *pusCount = (uint16_t)ubBuf[1] << 8;
        *pusCount |= ubBuf[2];
    }
}
void cp2130_set_event_counter(cp2130_device_t *pCpDev, uint8_t ubMode, uint16_t usCount)
{
    uint8_t ubBuf[3];
    ubBuf[0] = ubMode;
    ubBuf[1] = (uint8_t)(usCount >> 8);
    ubBuf[2] = (uint8_t)(usCount & 0xFF);

    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_EVNT_CNT, ubBuf, 3, 0, 0);
}

void cp2130_get_full_threshold(cp2130_device_t *pCpDev, uint8_t *pubThreshold)
{
    if (pubThreshold)
        cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_FULL_TH, pubThreshold, 1, 0, 0);
}
void cp2130_set_full_threshold(cp2130_device_t *pCpDev, uint8_t ubThreshold)
{
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_FULL_TH, &ubThreshold, 1, 0, 0);
}

void cp2130_get_gpio_cs(cp2130_device_t *pCpDev, uint16_t *pusCsEn, uint16_t *pusPinCsEn)
{
    uint8_t ubBuf[4];
    memset(ubBuf, 0x00, 4);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_GPIO_CS, ubBuf, 4, 0, 0);

    if(pusCsEn)
    {
        *pusCsEn = ((uint16_t)ubBuf[0] << 8) & 0xFF00;
        *pusCsEn |= (uint16_t)ubBuf[1] & 0x00FF;
    }
    if(pusPinCsEn)
    {
        *pusPinCsEn = ((uint16_t)ubBuf[2] << 8) & 0xFF00;
        *pusPinCsEn |= (uint16_t)ubBuf[3] & 0x00FF;
    }
}
void cp2130_set_gpio_cs(cp2130_device_t *pCpDev, uint8_t usCh, uint8_t usCtrl)
{
    uint8_t ubBuf[2];
    ubBuf[0] = usCh;
    ubBuf[1] = usCtrl;

    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_GPIO_CS, ubBuf, 2, 0, 0);
}

void cp2130_get_gpio_mode_level(cp2130_device_t *pCpDev, uint16_t *pusLevel, uint16_t *pusMode)
{
    uint8_t ubBuf[4];
    memset(ubBuf, 0x00, 4);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_GPIO_MDLVL, ubBuf, 4, 0, 0);

    if(pusLevel)
    {
        *pusLevel = ((uint16_t)ubBuf[1] << 8) & 0xFF00;
        *pusLevel |= (uint16_t)ubBuf[0] & 0x00FF;
    }
    if(pusMode)
    {
        *pusMode = ((uint16_t)ubBuf[3] << 8) & 0xFF00;
        *pusMode |= (uint16_t)ubBuf[2] & 0x00FF;
    }
}
void cp2130_set_gpio_mode_level(cp2130_device_t *pCpDev, uint8_t ubIndex, uint8_t ubMode, uint8_t ubLevel)
{
    uint8_t ubBuf[3];
    ubBuf[0] = ubIndex;
    ubBuf[1] = ubMode;
    ubBuf[2] = ubLevel;

    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_GPIO_MDLVL, ubBuf, 3, 0, 0);
}

void cp2130_get_gpio_values(cp2130_device_t *pCpDev, uint16_t *pusLevel)
{
    uint8_t ubBuf[2];
    memset(ubBuf, 0x00, 2);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_GPIO_VAL, ubBuf, 2, 0, 0);

    if(pusLevel)
    {
        *pusLevel = ((uint16_t)ubBuf[0] << 8) & 0xFF00;
        *pusLevel |= (uint16_t)ubBuf[1] & 0x00FF;
    }
}
void cp2130_set_gpio_values(cp2130_device_t *pCpDev, uint16_t usLevel, uint16_t usMask)
{
    uint8_t ubBuf[4];
    ubBuf[0] = (uint8_t)(usLevel >> 8);
    ubBuf[1] = (uint8_t)(usLevel & 0x00FF);
    ubBuf[2] = (uint8_t)(usMask >> 8);
    ubBuf[3] = (uint8_t)(usMask & 0x00FF);

    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_GPIO_VAL, ubBuf, 4, 0, 0);
}

void cp2130_get_rtr_state(cp2130_device_t *pCpDev, uint8_t *pubActive)
{
    if(pubActive)
        cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_RTR_STATE, pubActive, 1, 0, 0);
}
void cp2130_set_rtr_stop(cp2130_device_t *pCpDev, uint8_t ubAbort)
{
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_RTR_STOP, &ubAbort, 1, 0, 0);
}

void cp2130_get_spi_word(cp2130_device_t *pCpDev, uint8_t *pubChNword)
{
    if(pubChNword)
        cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_SPI_WORD, pubChNword, 11, 0, 0);
}
void cp2130_set_spi_word(cp2130_device_t *pCpDev, uint8_t ubCh, uint8_t ubChWord)
{
    uint8_t ubBuf[2];
    ubBuf[0] = ubCh;
    ubBuf[1] = ubChWord;

    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_SPI_WORD, ubBuf, 2, 0, 0);
}

void cp2130_get_spi_delay(cp2130_device_t *pCpDev, uint8_t ubSpiCh, uint8_t *pubMask, uint16_t *pusInBDelay, uint16_t *pusPostDelay, uint16_t *pusPreDelay)
{
    uint8_t ubBuf[8];
    memset(ubBuf, 0x00, 8);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_SPI_DELAY, ubBuf, 8, 0, ubSpiCh);

    if(pubMask)
        *pubMask = ubBuf[1];
    if(pusInBDelay)
    {
        *pusInBDelay = ((uint16_t)ubBuf[2] << 8) & 0xFF00;
        *pusInBDelay |= (uint16_t)ubBuf[3] & 0x00FF;
    }
    if(pusPostDelay)
    {
        *pusPostDelay = ((uint16_t)ubBuf[4] << 8) & 0xFF00;
        *pusPostDelay |= (uint16_t)ubBuf[5] & 0x00FF;
    }
    if(pusPreDelay)
    {
        *pusPreDelay = ((uint16_t)ubBuf[6] << 8) & 0xFF00;
        *pusPreDelay |= (uint16_t)ubBuf[7] & 0x00FF;
    }
}
void cp2130_set_spi_delay(cp2130_device_t *pCpDev, uint8_t ubSpiCh, uint8_t ubMask, uint16_t usInBDelay, uint16_t usPostDelay, uint16_t usPreDelay)
{
    uint8_t ubBuf[8];
    ubBuf[0] = ubSpiCh;
    ubBuf[1] = ubMask;
    ubBuf[2] = (uint8_t)(usInBDelay >> 8);
    ubBuf[3] = (uint8_t)(usInBDelay & 0xFF);
    ubBuf[4] = (uint8_t)(usPostDelay >> 8);
    ubBuf[5] = (uint8_t)(usPostDelay & 0xFF);
    ubBuf[6] = (uint8_t)(usPreDelay >> 8);
    ubBuf[7] = (uint8_t)(usPreDelay & 0xFF);

    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_SPI_DELAY, ubBuf, 8, 0, 0);
}

void cp2130_get_version(cp2130_device_t *pCpDev, uint8_t *pubMajor, uint8_t *pubMinor)
{
    uint8_t ubBuf[2];
    memset(ubBuf, 0x00, 2);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_VERSION, ubBuf, 2, 0, 0);

    if(pubMajor)
        *pubMajor = ubBuf[0];
    if(pubMinor)
        *pubMinor = ubBuf[1];
}

// OTP ROM Configuration Commands (Control Transfers)
void cp2130_get_lock_byte(cp2130_device_t *pCpDev, uint16_t *pusLock)
{
    uint8_t ubBuf[2];
    memset(ubBuf, 0x00, 2);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_LOCK_BYTE, ubBuf, 2, 0, 0);

    if (pusLock) 
    {
        *pusLock = ((uint16_t)ubBuf[0] << 8) & 0xFF00;
        *pusLock |= (uint16_t)ubBuf[1] & 0x00FF;
    }
}
void cp2130_set_lock_byte(cp2130_device_t *pCpDev, uint16_t usLock)
{
    uint8_t ubBuf[2];
    ubBuf[0] = (uint8_t)(usLock >> 8);
    ubBuf[1] = (uint8_t)(usLock & 0x00FF);

    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_LOCK_BYTE, ubBuf, 2, 0, 0);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

void cp2130_get_manufacturer_string(cp2130_device_t *pCpDev, uint8_t *pubStr)
{
    if(!pubStr)
    {
        DBGPRINTLN_CTX("error: invalid str pointer.");
        return;
    }

    uint8_t ubBuf[64];

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_MAN_STR_1, ubBuf, 64, 0, 0);

    uint8_t ubStrLen = ubBuf[0];
    for(uint8_t ubPos = 0; ubPos < MIN(ubStrLen, 31); ubPos++)
    {
        if(ubBuf[3 + (2 * ubPos)])
            pubStr[ubPos] = '?';
        else
            pubStr[ubPos] = ubBuf[2 + (2 * ubPos)];
    }

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_MAN_STR_2, ubBuf, 64, 0, 0);

    for(uint8_t ubPos = 31; ubPos < MIN(ubStrLen, 63); ubPos++)
    {
        if(ubBuf[3 + (2 * ubPos)])
            pubStr[ubPos] = '?';
        else
            pubStr[ubPos] = ubBuf[2 * (ubPos - 31)];
    }
    pubStr[ubStrLen] = 0x00;
}
void cp2130_set_manufacturer_string(cp2130_device_t *pCpDev, uint8_t *pubStr)
{
    if(!pubStr)
    {
        DBGPRINTLN_CTX("error: invalid str pointer.");
        return;
    }

    uint8_t ubStrLen = strlen((const char *)pubStr);
	if(ubStrLen > 62)
    {
		DBGPRINTLN_CTX("warning: string too long, max 62 chars.");
        return;
    }

    uint8_t ubCmdBuf1[64];
    uint8_t ubCmdBuf2[64];
    memset(ubCmdBuf1, 0x00, 64);
    memset(ubCmdBuf2, 0x00, 64);

    ubCmdBuf1[0] = (ubStrLen * 2) + 2;
    ubCmdBuf1[1] = 0x03;

    for(uint8_t ubPos = 0; ubPos < MIN(ubStrLen, 31); ubPos++)
    {
        ubCmdBuf1[2 + (ubPos * 2)] = pubStr[ubPos];
    }

    for(uint8_t ubPos = 31; ubPos < MIN(ubStrLen, 63); ubPos++)
    {
        ubCmdBuf2[(ubPos - 31) * 2] = pubStr[ubPos];
    }

    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_MAN_STR_1, ubCmdBuf1, 64, CP2130_MEM_KEY, 0);
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_MAN_STR_2, ubCmdBuf2, 64, CP2130_MEM_KEY, 0);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

void cp2130_get_prod_string(cp2130_device_t *pCpDev, uint8_t *pubStr)
{
    if(!pubStr)
    {
        DBGPRINTLN_CTX("error: invalid str pointer.");
        return;
    }

    uint8_t ubBuf[64];

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_PROD_STR_1, ubBuf, 64, 0, 0);

    uint8_t ubStrLen = ubBuf[0];
    for(uint8_t ubPos = 0; ubPos < MIN(ubStrLen, 31); ubPos++)
    {
        if(ubBuf[3 + (2 * ubPos)])
            pubStr[ubPos] = '?';
        else
            pubStr[ubPos] = ubBuf[2 + (2 * ubPos)];
    }

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_PROD_STR_2, ubBuf, 64, 0, 0);

    for(uint8_t ubPos = 31; ubPos < MIN(ubStrLen, 63); ubPos++)
    {
        if(ubBuf[3 + (2 * ubPos)])
            pubStr[ubPos] = '?';
        else
            pubStr[ubPos] = ubBuf[2 * (ubPos - 31)];
    }
    pubStr[ubStrLen] = 0x00;
}
void cp2130_set_prod_string(cp2130_device_t *pCpDev, uint8_t *pubStr)
{
    if(!pubStr)
    {
        DBGPRINTLN_CTX("error: invalid str pointer.");
        return;
    }

    uint8_t ubStrLen = strlen((const char *)pubStr);
	if(ubStrLen > 62)
    {
		DBGPRINTLN_CTX("warning: string too long, max 62 chars.");
        return;
    }

    uint8_t ubData1[64];
    uint8_t ubData2[64];
    memset(ubData1, 0x00, 64);
    memset(ubData2, 0x00, 64);

    ubData1[0] = (ubStrLen * 2) + 2;
    ubData1[1] = 0x03;

    for(uint8_t i = 0; i < MIN(ubStrLen, 31); i++)
    {
        ubData1[2 + (i * 2)] = pubStr[i];
    }

    for(uint8_t i = 31; i < MIN(ubStrLen, 63); i++)
    {
        ubData2[(i - 31) * 2] = pubStr[i];
    }

    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, 0x40, CP2130_CMDID_SET_PROD_STR_1, ubData1, 64, CP2130_MEM_KEY, 0);
    cp2130_control_transfer(pCpDev, 0x40, CP2130_CMDID_SET_PROD_STR_2, ubData2, 64, CP2130_MEM_KEY, 0);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

void cp2130_get_serial(cp2130_device_t *pCpDev, uint8_t *pubStr)
{
    if(!pubStr)
    {
        DBGPRINTLN_CTX("error: invalid str pointer.");
        return;
    }

    uint8_t ubBuf[64];
    memset(ubBuf, 0x00, 64);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_SERIAL_STR, ubBuf, 64, 0, 0);

    for(uint8_t ubPos = 0; ubPos < ubBuf[0]; ubPos++)
    {
        if(ubBuf[3 + (2 * ubPos)])
            pubStr[ubPos] = '?';
        else
            pubStr[ubPos] = ubBuf[2 + (2 * ubPos)];
    }
    pubStr[ubBuf[0]] = 0x00;
}
void cp2130_set_serial(cp2130_device_t *pCpDev, uint8_t *pubStr)
{
    if(!pubStr)
    {
        DBGPRINTLN_CTX("error: invalid str pointer.");
        return;
    }

    uint8_t ubStrLen = strlen((const char *)pubStr);
	if(ubStrLen > 30)
    {
		DBGPRINTLN_CTX("warning: string too long, max 62 chars.");
        return;
    }

    uint8_t ubCmdBuf[64];
    memset(ubCmdBuf, 0x00, 64);

    ubCmdBuf[0] = (ubStrLen * 2) + 2;
    ubCmdBuf[1] = 0x03;

    for(uint8_t ubPos = 0; ubPos < MIN(ubStrLen, 30); ubPos++)
    {
        ubCmdBuf[2 + (ubPos * 2)] = pubStr[ubPos];
    }

    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_SERIAL_STR, ubCmdBuf, 64, CP2130_MEM_KEY, 0);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

void cp2130_get_pin_cfg(cp2130_device_t *pCpDev, uint8_t *pubPinCfg)
{
    if(pubPinCfg)
        cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_PIN_CFG, pubPinCfg, 0x0014, 0, 0);
}
void cp2130_set_pin_cfg(cp2130_device_t *pCpDev, uint8_t *pubPinCfg)
{
    if (!pubPinCfg)
    {
        DBGPRINTLN_CTX("error: invalid pointer.");
    }

    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_PIN_CFG, pubPinCfg, 0x0014, 0, 0);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

void cp2130_get_prom_cfg(cp2130_device_t *pCpDev, uint8_t ubBlkIndex, uint8_t *pubBlk)
{
    if(pubBlk)
        cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_PROM_CFG, pubBlk, 0x0014, 0, ubBlkIndex);
}
void cp2130_set_prom_cfg(cp2130_device_t *pCpDev, uint8_t ubBlkIndex, uint8_t *pubBlk)
{
    if (!pubBlk)
    {
        DBGPRINTLN_CTX("error: invalid pointer.");
    }
    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_SET_PROM_CFG, pubBlk, 0x0014, CP2130_MEM_KEY, ubBlkIndex);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

void cp2130_get_usb_cfg(cp2130_device_t *pCpDev, uint16_t *pusVid, uint16_t *pusPid, uint8_t *pubMaxPow, uint8_t *pubPowMode, uint8_t *pubMajorRelease, uint8_t *pubMinorRelease, uint8_t *pubTransferPriority)
{
    uint8_t ubBuf[9];
    memset(ubBuf, 0x00, 9);

    cp2130_control_transfer(pCpDev, CP2130_REQ_DEVICE_HOST_VENDOR, CP2130_CMDID_GET_USB_CFG, ubBuf, 9, 0, 0);

    if(pusVid)
    {
        *pusVid = ((uint16_t)ubBuf[1] << 8) & 0xFF00;   // little endian
        *pusVid |= (uint16_t)ubBuf[0] & 0x00FF;
    }
    if(pusPid)
    {
        *pusPid = ((uint16_t)ubBuf[3] << 8) & 0xFF00;   // little endian
        *pusPid |= (uint16_t)ubBuf[2] & 0x00FF;
    }
    if(pubMaxPow)
        *pubMaxPow = ubBuf[4];
    if(pubPowMode)
        *pubPowMode = ubBuf[5];
    if(pubMajorRelease)
        *pubMajorRelease = ubBuf[6];
    if(pubMinorRelease)
        *pubMinorRelease = ubBuf[7];
    if(pubTransferPriority)
        *pubTransferPriority = ubBuf[8];
}
void cp2130_set_usb_cfg(cp2130_device_t *pCpDev, uint16_t usVid, uint16_t usPid, uint8_t ubMaxPow, uint8_t ubPowMode, uint8_t ubMajorRelease, uint8_t ubMinorRelease, uint8_t ubTransferPriority, uint8_t ubMask)
{
    uint8_t ubBuf[10];
    ubBuf[0] = (uint8_t)(usVid & 0x00FF);   // little endian
    ubBuf[1] = (uint8_t)(usVid >> 8);
    ubBuf[2] = (uint8_t)(usPid & 0x00FF);   // little endian
    ubBuf[3] = (uint8_t)(usPid >> 8);
    ubBuf[4] = ubMaxPow;
    ubBuf[5] = ubPowMode;
    ubBuf[6] = ubMajorRelease;
    ubBuf[7] = ubMinorRelease;
    ubBuf[8] = ubTransferPriority;
    ubBuf[9] = ubMask;

    #ifndef CP2130_OTP_ROM_WRITE_PROTECT
    cp2130_control_transfer(pCpDev, CP2130_REQ_HOST_DEVICE_VENDOR, CP2130_CMDID_SET_USB_CFG, ubBuf, 10, CP2130_MEM_KEY, 0);
    #else
    DBGPRINTLN_CTX("warning: OTP ROM write protect enabled, ignoring write.");
    #endif
}

#ifdef __cplusplus
}
#endif