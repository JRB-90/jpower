#include "strain.h"

#include "ad779x.h"

static nrf_drv_spi_t* spi = NULL;
static calibration_data_t current_cal = { 0 };
static uint32_t current_adc_value = 0;
static float current_torque_nm = 0.0f;

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
        .slope = 0.134f,
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
    current_adc_value = ad7799_read_raw_data_single();

    // F = (slope * adc) + intercept
    double force = (current_cal.slope * current_adc_value) + current_cal.intercept;
    // T = F * r
    current_torque_nm = force * current_cal.crank_length_m;
}

void strain_get_calibration(calibration_data_t* const calibration)
{
    *calibration = current_cal;
}

void strain_set_calibration(calibration_data_t* const calibration)
{
    current_cal = *calibration;
}

uint32_t strain_get_current_adv_value()
{
    return current_adc_value;
}

float strain_get_current_torque_nm()
{
    return current_torque_nm;
}

ret_code_t strain_zero_offset()
{
    ad779x_system_zeroscale_calibration();

    return NRF_SUCCESS;
}
