#ifndef STRAIN_H__
#define STRAIN_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_spi.h"

extern ret_code_t strain_init(
    nrf_drv_spi_t* spi_instance,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin,
    const uint8_t ss_pin
);

extern ret_code_t strain_get_raw_adc_value(uint32_t* value);

#endif // STRAIN_H__