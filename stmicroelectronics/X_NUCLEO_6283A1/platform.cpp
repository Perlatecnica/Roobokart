#include "STALS_platform.h"


STALS_ErrCode_t STALS_WrByte(DevI2C * pClient, uint8_t dev_7bits_i2c_addr, uint8_t index, uint8_t data)
{
    uint8_t pBuffer[] = { data };
    int res;
    res = pClient->i2c_write(pBuffer, dev_7bits_i2c_addr << 1, index, 1);
    if (res) return STALS_ERROR_WRITE;
    return STALS_NO_ERROR;
}

STALS_ErrCode_t STALS_RdByte(DevI2C * pClient, uint8_t dev_7bits_i2c_addr, uint8_t index, uint8_t *data)
{
    int res;
    res = pClient->i2c_read(data, dev_7bits_i2c_addr << 1, index, 1);
    if (res) return STALS_ERROR_READ;
    return STALS_NO_ERROR;
}

/* following function is optional. There is a default weak implementation */
STALS_ErrCode_t STALS_RdMultipleBytes(DevI2C * pClient, uint8_t dev_7bits_i2c_addr, uint8_t index, uint8_t *data, int nb)
{
    int res;
    res = pClient->i2c_read(data, dev_7bits_i2c_addr << 1, index, nb);
    if (res) return STALS_ERROR_READ;
    return STALS_NO_ERROR;
}
