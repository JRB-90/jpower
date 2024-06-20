#include "sensor_subsystem.h"

#include "nrf_log.h"
#include "imu.h"
#include "strain.h"

static nrf_drv_twi_t twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE_ID);

ret_code_t sensor_subsystem_init(const sensor_config_t* const config)
{
    ret_code_t err_code;

    err_code =
        imu_init(
            &twi,
            config->i2c_scl_pin,
            config->i2c_sda_pin
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        strain_init(
            &spi,
            config->spi_sck_pin,
            config->spi_mosi_pin,
            config->spi_miso_pin,
            config->spi_cs_pin
        );
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

void sensor_subsystem_update_10ms(float time_delta_s)
{
    strain_update_10ms(time_delta_s);
    imu_update_10ms(time_delta_s);
}
