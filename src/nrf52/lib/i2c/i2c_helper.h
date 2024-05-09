#ifndef I2C_HELPER_H__
#define I2C_HELPER_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_twi.h"

ret_code_t i2c_init(
    const nrf_drv_twi_t* twiInstance,
    uint32_t sclPin,
    uint32_t sdaPin
);

ret_code_t i2c_write(
    const nrf_drv_twi_t* twiInstance, 
    uint8_t deviceAddress, 
    uint8_t registerAddress, 
    const uint8_t* data, 
    uint16_t length
);

ret_code_t i2c_read(
    const nrf_drv_twi_t* twiInstance, 
    uint8_t deviceAddress, 
    uint8_t registerAddress, 
    uint8_t* data, 
    uint16_t length
);

uint8_t i2c_read_uint8(
    const nrf_drv_twi_t* twiInstance,
    uint8_t deviceAddress, 
    uint8_t registerAddress
);

void i2c_write_uint8(
    const nrf_drv_twi_t* twiInstance,
    uint8_t deviceAddress, 
    uint8_t registerAddress,
    uint8_t value
);

#endif // I2C_HELPER_H__