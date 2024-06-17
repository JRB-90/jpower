#include "strain.h"
#include "ad779x.h"

static nrf_drv_spi_t* spi = NULL;

ret_code_t strain_init(
    nrf_drv_spi_t* spi_instance,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin,
    const uint8_t ss_pin)
{
    ret_code_t err_code;

    spi = spi_instance;

    err_code =
        ad779x_init(
            spi,
            sck_pin,
            mosi_pin,
            miso_pin,
            ss_pin
        );
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

ret_code_t strain_zero_offset()
{
    ad779x_system_zeroscale_calibration();

    return NRF_SUCCESS;
}

ret_code_t strain_get_raw_adc_value(uint32_t* value)
{
    *value = ad7799_read_raw_data_single();

    return NRF_SUCCESS;
}