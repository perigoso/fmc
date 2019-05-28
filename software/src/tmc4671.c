#include "tmc4671.h"

#ifdef __cplusplus
extern "C" {
#endif

cp2130_device_t *pCpDev = NULL;

void tmc4671_init(cp2130_device_t *pCpInDev)
{
    pCpDev = pCpInDev;
}

// Data Transfer Frame
void tmc4671_write(uint8_t ubAddr, uint32_t ubData)
{
    uint8_t ubBuf[5];
    memset(ubBuf, 0x00, 5);

    ubBuf[0] = TMC4671_FRAME_WRITE | (ubAddr & 0x7F);
    ubBuf[1] = (ubData >> 24) & 0xFF;
    ubBuf[2] = (ubData >> 16) & 0xFF;
    ubBuf[3] = (ubData >> 8) & 0xFF;
    ubBuf[4] = ubData & 0xFF;

    cp2130_spi_transfer(pCpDev, ubBuf, 5);
}
uint32_t tmc4671_read(uint8_t ubAddr)
{
    uint8_t ubBuf[5];
    memset(ubBuf, 0x00, 5);

    ubBuf[0] = TMC4671_FRAME_READ | (ubAddr & 0x7F);

    cp2130_spi_transfer(pCpDev, ubBuf, 5);

    return ((uint32_t)ubBuf[1] << 24) | ((uint32_t)ubBuf[2] << 16) | ((uint32_t)ubBuf[3] << 24) | (uint32_t)ubBuf[4];
}

#ifdef __cplusplus
}
#endif