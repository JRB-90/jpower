#ifndef SPI_HELPER_H__
#define SPI_HELPER_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_spi.h"

extern ret_code_t spi_init(
    const nrf_drv_spi_t* spiInstance,
    const nrf_drv_spi_config_t* const config
);

extern ret_code_t spi_read(
    const nrf_drv_spi_t* spiInstance,
    uint8_t* rx_buffer,
    uint8_t rx_buffer_length
);

extern ret_code_t spi_write(
    const nrf_drv_spi_t* spiInstance,
    const uint8_t* tx_buffer,
    uint8_t tx_buffer_length
);

#endif // SPI_HELPER_H__