#include "strain.h"

#include "nrf_log.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_spi.h"
#include "ad779x.h"
#include "spi_helper.h"

static device_context_t device;
static nrf_drv_spi_t* spi = NULL;

static void platform_delay(uint32_t millis);
static ret_code_t platform_write(const uint8_t* data, uint8_t length);
static ret_code_t platform_read(uint8_t* data, uint8_t length);

ret_code_t strain_init(
    nrf_drv_spi_t* spi_instance,
    const uint32_t adc_pwr_pin,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin)
{
    ret_code_t err_code;

    spi = spi_instance;
    device.delay_func = platform_delay;
    device.write_func = platform_write;
    device.read_func = platform_read;

    nrf_gpio_cfg_output(adc_pwr_pin);
    nrf_gpio_pin_clear(adc_pwr_pin);
    nrf_delay_ms(AD7799_BOOT_TIME_MS);

    nrf_drv_spi_config_t spi_config =
    {
        .frequency = NRF_DRV_SPI_FREQ_500K,
        .mode = NRF_DRV_SPI_MODE_3,
        .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,
        .irq_priority = 3,
        .sck_pin = sck_pin,
        .mosi_pin = mosi_pin,
        .miso_pin = miso_pin,
        .ss_pin = NRF_DRV_SPI_PIN_NOT_USED,
    };

    err_code =
        spi_init(
            spi_instance,
            &spi_config
        );
    
    APP_ERROR_CHECK(err_code);

    APP_ERROR_CHECK(ad7799_init(&device));

    NRF_LOG_INFO("AD7799 Initialised");

    return NRF_SUCCESS;
}

uint32_t strain_take_reading()
{
    // mode.mode = AD7799_MODE_SINGLE;
    // ad7799_set_mode(&device, &mode);

    // conf.channel_select = AD7799_CONF_CHAN_AIN1_POS;
    // ad7799_set_conf(&device, &conf);

    // while (!ad7799_is_ready(&device)) { }

    // uint8_t status = ad7799_read_status(&device);

    // if (status & 0x40)
    // {
    //     NRF_LOG_INFO("Out of range");

    //     return 0;
    // }
    // else
    // {
    //     return ad7799_get_data(&device);
    // }

    return 0;
}

void strain_zero()
{
    // mode.mode = AD7799_MODE_CAL_INT_ZERO;
    // ad7799_set_mode(&device, &mode);

    // while (!ad7799_is_ready(&device)) { }
}

void strain_calibrate()
{
    // NRF_LOG_INFO("%u", ad7799_get_fullscale(&device));

    // mode.mode = AD7799_MODE_CAL_INT_FSCALE;
    // ad7799_set_mode(&device, &mode);

    // while (!ad7799_is_ready(&device)) { }

    // NRF_LOG_INFO("%u", ad7799_get_fullscale(&device));
}

static void platform_delay(uint32_t millis)
{
    nrf_delay_ms(millis);
}

static ret_code_t platform_write(const uint8_t* data, uint8_t length)
{
    return spi_write(spi, data, length);
}

static ret_code_t platform_read(uint8_t* data, uint8_t length)
{
    return spi_read(spi, data, length);
}