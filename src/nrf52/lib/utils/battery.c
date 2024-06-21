#include "battery.h"

#include "nrf_drv_saadc.h"

#define BATTERY_VOLTAGE 3700
#define SAMPLES_IN_BUFFER 10
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_RESULT) ((ADC_RESULT * 0.2197265625))

int16_t adc_value = 0;
int16_t adc_voltage_mv = 0;

ret_code_t battery_init()
{
    ret_code_t err_code;

    nrf_drv_saadc_config_t saadc_config;
    saadc_config.resolution = NRF_SAADC_RESOLUTION_14BIT;

    err_code = nrf_drv_saadc_init(&saadc_config, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t channel_config = 
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);

    return NRF_SUCCESS;
}

void battery_update()
{
    nrfx_err_t err_code = nrf_drv_saadc_sample_convert(0, &adc_value);
    APP_ERROR_CHECK(err_code);

    adc_voltage_mv = ADC_RESULT_IN_MILLI_VOLTS(adc_value);    
}

int16_t battery_get_level_mv()
{
    return adc_voltage_mv;
}

float battery_get_level_v()
{
    return ((float)adc_voltage_mv) / 1000.0f;
}

uint8_t battery_get_level_percentage()
{
    return (adc_voltage_mv * 200) / BATTERY_VOLTAGE;
}
