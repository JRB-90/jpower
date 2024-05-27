#include "strain.h"
#include "ad779x.h"

static nrf_drv_spi_t* spi = NULL;

ret_code_t strain_init(
    nrf_drv_spi_t* spi_instance,
    const uint32_t adc_pwr_pin,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin)
{
    ret_code_t err_code;

    spi = spi_instance;

    // TODO - Add back in later when I have a ADC hooked up

    // err_code =
    //     ad779x_init(
    //         spi,
    //         adc_pwr_pin,
    //         sck_pin,
    //         mosi_pin,
    //         miso_pin
    //     );
    // APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}