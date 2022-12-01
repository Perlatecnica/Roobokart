#ifndef __STALS_PLATFORM__
#define __STALS_PLATFORM__ 1

#include "mbed.h"
#include "STALS.h"


/* following functions must be implemented by platform integrator */
//STALS_ErrCode_t STALS_WrByte(void *pClient, uint8_t index, uint8_t data);
//STALS_ErrCode_t STALS_RdByte(void *pClient, uint8_t index, uint8_t *data);
/* following function is optional. There is a default weak implementation */
//STALS_ErrCode_t STALS_RdMultipleBytes(void *pClient, uint8_t index, uint8_t *data, int nb);

/* following functions must be implemented by platform integrator */
STALS_ErrCode_t STALS_WrByte(DevI2C * pClient, uint8_t dev_7bits_i2c_addr, uint8_t index, uint8_t data);
STALS_ErrCode_t STALS_RdByte(DevI2C * pClient, uint8_t dev_7bits_i2c_addr, uint8_t index, uint8_t *data);

/* following function is optional. There is a default weak implementation */
STALS_ErrCode_t STALS_RdMultipleBytes(DevI2C * pClient, uint8_t dev_7bits_i2c_addr, uint8_t index, uint8_t *data, int nb);
//void *platform_get_client(int i2c_address_in_7_bits);

#endif /* __STALS_PLATFORM__ */
