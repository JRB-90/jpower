#include "sensor_subsystem.h"

#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "imu_types.h"
#include "imu.h"
#include "cadence.h"
#include "strain.h"

#define DESIRED_PRINT_HZ    1
#define COUNTER_TRIGGER     100 / DESIRED_PRINT_HZ

static nrf_drv_twi_t twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE_ID);
static uint32_t counter = 0;

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

    if (counter >= COUNTER_TRIGGER)
    {
#ifdef DEBUG_PRINT
        FusionQuaternion attitude;
        cadence_get_attitude(&attitude);

        pedal_state_t pedal_state;
        cadence_get_pedal_state(&pedal_state);

        uint32_t adc_value = strain_get_current_adv_value();

        char imu_str[128];
        sprintf(
            imu_str,
            "A [%.3f, %.3f, %.3f, %.3f], C [%u, %.3f]",
            attitude.array[0], attitude.array[1],
            attitude.array[2], attitude.array[3],
            pedal_state.cadence,
            pedal_state.angular_velocity
        );
        
        char strain_str[128];
        sprintf(
            strain_str,
            "T %.3fs [ %u ]",
            time_delta_s, adc_value
        );

        NRF_LOG_INFO("%s", imu_str);
        NRF_LOG_INFO("%s", strain_str);
        NRF_LOG_FLUSH();
#endif
        counter = 0;
    }
    else
    {
        counter++;
    }
}
