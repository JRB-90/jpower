#include "strain.h"

#include "ad779x.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define DESIRED_HZ          10
#define COUNTER_TRIGGER     100 / DESIRED_HZ

static nrf_drv_spi_t* spi = NULL;
static calibration_data_t current_cal = { 0 };
static uint32_t counter = 0;

ret_code_t strain_init(
    nrf_drv_spi_t* spi_instance,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin,
    const uint8_t ss_pin)
{
    ret_code_t err_code;

    spi = spi_instance;

    // Dummy data for now
    current_cal = (calibration_data_t)
    {
        .cal_id = {{ 0 }},
        .slope = 22.34f,
        .intercept = 567.89f,
        .crank_length_m = 0.1725f,
    };

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

void strain_update_10ms(float delta_time_s)
{
    uint32_t value = ad7799_read_raw_data_single();

    if (counter >= COUNTER_TRIGGER)
    {
        counter = 0;
        
        char str_buf[128];
        sprintf(
            str_buf,
            "T %.3fs [ %u ]",
            delta_time_s, value
        );

        NRF_LOG_INFO("%s", str_buf);
        NRF_LOG_FLUSH();
    }
    else
    {
        counter++;
    }
}

ret_code_t strain_zero_offset()
{
    ad779x_system_zeroscale_calibration();

    return NRF_SUCCESS;
}

void strain_get_calibration(calibration_data_t* const calibration)
{
    *calibration = current_cal;
}

void strain_set_calibration(calibration_data_t* const calibration)
{
    current_cal = *calibration;
}