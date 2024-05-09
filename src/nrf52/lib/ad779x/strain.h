#ifndef STRAIN_H__
#define STRAIN_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_spi.h"

extern ret_code_t strain_init(
    nrf_drv_spi_t *spiInstance,
    const uint32_t adc_pwr_pin,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin);

extern uint32_t strain_take_reading();
extern void strain_zero();
void strain_calibrate();

#endif // STRAIN_H__