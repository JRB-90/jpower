#include "ad779x.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "spi_helper.h"

// #region Defines

#define AD779X_BOOT_TIME_MS         10
#define AD779X_RESET_TIME_US        500

#define AD7798_CHIP_ID              0x08
#define AD7799_CHIP_ID              0x09

#define AD779X_REG_COMM             0x00
#define AD779X_REG_STATUS           0x00
#define AD779X_REG_MODE             0x08
#define AD779X_REG_CONF             0x10
#define AD779X_REG_DATA             0x18
#define AD779X_REG_ID               0x20
#define AD779X_REG_IO               0x28
#define AD779X_REG_OFFSET           0x30
#define AD779X_REG_FULL_SCALE       0x38

#define AD779X_COMM_READ            0b01000000
#define AD779X_COMM_WRITE           0b00000000

#define AD779X_READY_MASK           0b10000000
#define AD779X_ERROR_MASK           0b01000000
#define AD779X_NOREF_MASK           0b00100000
#define AD779X_MODEL_MASK           0b00001000
#define AD779X_CHAN1_MASK           0b00000001
#define AD779X_CHAN2_MASK           0b00000010
#define AD779X_CHAN3_MASK           0b00000100
#define AD779X_MODE_MASK            0b1110000000000000
#define AD779X_PSW_MASK             0b0001000000000000
#define AD779X_FILTER_MASK          0b0000000000001111
#define AD779X_BURNOUT_MASK         0b0010000000000000
#define AD779X_CODING_MASK          0b0001000000000000
#define AD779X_GAIN_MASK            0b0000011100000000
#define AD779X_REF_DETECT_MASK      0b0000000000100000
#define AD779X_BUF_MODE_MASK        0b0000000000010000
#define AD779X_CHAN_SEL_MASK        0b0000000000000111
#define AD779X_AIN3_SET_MASK        0b01000000
#define AD779X_P1_STATE_MASK        0b00100000
#define AD779X_P2_STATE_MASK        0b00010000

const float vRef                    = 3.3f;
const float gain                    = 128.0f;

// #endregion

// #region Private Function Defs

static void write_register_8bit(
    const uint8_t register_address,
    const uint8_t value);

static void write_register_16bit(
    const uint8_t register_address,
    const uint16_t value);

static uint8_t read_register_8bit(const uint8_t register_address);
static uint16_t read_register_16bit(const uint8_t register_address);
static uint32_t read_register_24bit(const uint8_t register_address);

// #endregion

// #region Private Data

static nrf_drv_spi_t* spi = NULL;

static ad779x_mode_reg_t default_mode =
{
    .mode = AD779X_MODE_IDLE,
    .psw_state = AD779X_PSW_OPEN,
    .filter_rate = AD779X_FILTER_RATE_50HZ,
};

static ad779x_conf_reg_t default_conf =
{
    .burnout_enable = AD779X_CONF_BURNOUT_DISABLED,
    .coding_polarity = AD779X_CONF_BIPOLAR,
    .gain = AD779X_CONF_GAIN_128,
    .ref_detect = AD779X_CONF_REF_DET_DISABLED,
    .buffer_mode = AD779X_CONF_BUFFERED,
    .channel_select = AD779X_CONF_CHAN_AIN2_POS,
};

// #endregion

// #region Public Functions Implementations

ret_code_t ad779x_init(
    nrf_drv_spi_t* spi_instance,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin,
    const uint8_t ss_pin)
{
    ret_code_t err_code;

    spi = spi_instance;

    // nrf_gpio_cfg(
    //     adc_pwr_pin,
    //     NRF_GPIO_PIN_DIR_OUTPUT,
    //     NRF_GPIO_PIN_INPUT_DISCONNECT,
    //     NRF_GPIO_PIN_NOPULL,
    //     NRF_GPIO_PIN_H0H1,
    //     NRF_GPIO_PIN_NOSENSE
    // );
    // nrf_gpio_pin_clear(adc_pwr_pin);
    nrf_delay_ms(AD779X_BOOT_TIME_MS); // Let ADC Boot

    nrf_drv_spi_config_t spi_config =
        {
            .frequency = NRF_DRV_SPI_FREQ_500K,
            .mode = NRF_DRV_SPI_MODE_3,
            .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,
            .irq_priority = 3,
            .sck_pin = sck_pin,
            .mosi_pin = mosi_pin,
            .miso_pin = miso_pin,
            .ss_pin = ss_pin,
        };

    err_code =
        spi_init(
            spi,
            &spi_config
        );

    APP_ERROR_CHECK(err_code);

    nrf_delay_ms(AD779X_BOOT_TIME_MS); // Let SPI wake up

    ad779x_reset();

    uint8_t chip_id = ad779x_read_id_reg();

    if (chip_id != AD7799_CHIP_ID)
    {
        APP_ERROR_CHECK(NRF_ERROR_INVALID_DATA);
    }

    ad779x_set_default_settings();

    ad779x_internal_fullscale_calibration();

    return NRF_SUCCESS;
}

void ad779x_reset()
{
    uint8_t reset_value[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    APP_ERROR_CHECK(spi_write(spi, reset_value, 4));
    nrf_delay_us(AD779X_RESET_TIME_US);
}

bool ad779x_is_ready()
{
    ad779x_status_reg_t status = ad779x_read_status_reg();

    return status.is_ready;
}

void ad779x_set_default_settings()
{
    ad779x_write_mode_reg(&default_mode);
    ad779x_write_conf_reg(&default_conf);
}

void ad779x_internal_zeroscale_calibration()
{
    ad779x_mode_reg_t mode = ad779x_read_mode_reg();
    mode.mode = AD779X_MODE_CAL_INT_ZERO;
    ad779x_write_mode_reg(&mode);

    while (!ad779x_is_ready()) { }
}

void ad779x_internal_fullscale_calibration()
{
    ad779x_mode_reg_t mode = ad779x_read_mode_reg();
    mode.mode = AD779X_MODE_CAL_INT_FSCALE;
    ad779x_write_mode_reg(&mode);

    while (!ad779x_is_ready()) { }
}

void ad779x_system_zeroscale_calibration()
{
    ad779x_mode_reg_t mode = ad779x_read_mode_reg();
    mode.mode = AD779X_MODE_CAL_SYS_ZERO;
    ad779x_write_mode_reg(&mode);

    while (!ad779x_is_ready()) { }
}

void ad779x_system_fullscale_calibration()
{
    ad779x_mode_reg_t mode = ad779x_read_mode_reg();
    mode.mode = AD779X_MODE_CAL_SYS_FSCALE;
    ad779x_write_mode_reg(&mode);

    while (!ad779x_is_ready()) { }
}

uint8_t ad779x_read_id_reg()
{
    uint8_t value = read_register_8bit(AD779X_REG_ID);
    uint8_t id = value & 0x0F;

    return id;
}

ad779x_status_reg_t ad779x_read_status_reg()
{
    uint8_t value = read_register_8bit(AD779X_REG_STATUS);

    ad779x_status_reg_t status =
    {
        .is_ready = (value & AD779X_READY_MASK) == 0,
        .is_error = (value & AD779X_ERROR_MASK) != 0,
        .is_no_ref = (value & AD779X_NOREF_MASK) != 0,
        .model_type = (ad779x_model_t)(value & AD779X_MODEL_MASK),
        .adc_channel_1_en = (value & AD779X_CHAN1_MASK) != 0,
        .adc_channel_2_en = (value & AD779X_CHAN2_MASK) != 0,
        .adc_channel_3_en = (value & AD779X_CHAN3_MASK) != 0,
    };

    return status;
}

ad779x_mode_reg_t ad779x_read_mode_reg()
{
    uint16_t value = read_register_16bit(AD779X_REG_MODE);

    ad779x_mode_reg_t mode =
    {
        .mode = value & AD779X_MODE_MASK,
        .psw_state = value & AD779X_PSW_MASK,
        .filter_rate = value & AD779X_FILTER_MASK,
    };

    return mode;
}

void ad779x_write_mode_reg(const ad779x_mode_reg_t* const mode)
{
    uint16_t value =
        (uint16_t)mode->mode |
        (uint16_t)mode->psw_state |
        (uint16_t)mode->filter_rate;

    write_register_16bit(AD779X_REG_MODE, value);
}

ad779x_conf_reg_t ad779x_read_conf_reg()
{
    uint16_t value = read_register_16bit(AD779X_REG_CONF);

    ad779x_conf_reg_t conf =
    {
        .burnout_enable = value & AD779X_BURNOUT_MASK,
        .coding_polarity = value & AD779X_CODING_MASK,
        .gain = value & AD779X_GAIN_MASK,
        .ref_detect = value & AD779X_REF_DETECT_MASK,
        .buffer_mode = value & AD779X_BUF_MODE_MASK,
        .channel_select = value & AD779X_CHAN_SEL_MASK,
    };

    return conf;
}

void ad779x_write_conf_reg(const ad779x_conf_reg_t* const conf)
{
    uint16_t value =
        (uint16_t)conf->burnout_enable |
        (uint16_t)conf->coding_polarity |
        (uint16_t)conf->gain |
        (uint16_t)conf->ref_detect |
        (uint16_t)conf->buffer_mode |
        (uint16_t)conf->channel_select;

    write_register_16bit(AD779X_REG_CONF, value);
}

uint16_t ad7798_read_raw_data_single()
{
    ad779x_mode_reg_t mode = ad779x_read_mode_reg();
    mode.mode = AD779X_MODE_SINGLE;
    ad779x_write_mode_reg(&mode);

    while (!ad779x_is_ready()) { }

    uint16_t value = read_register_16bit(AD779X_REG_DATA);

    return value;
}

uint32_t ad7799_read_raw_data_single()
{
    ad779x_mode_reg_t mode = ad779x_read_mode_reg();
    mode.mode = AD779X_MODE_SINGLE;
    ad779x_write_mode_reg(&mode);

    while (!ad779x_is_ready()) { }

    uint32_t value = read_register_24bit(AD779X_REG_DATA);

    return value;
}

float ad7798_read_mV_single()
{
    float rawData = (float)ad7798_read_raw_data_single();
    float voltage = ((rawData * 0.00000011920928955078125f - 1.0f) * vRef) / (gain * 1000.0f);

    return voltage;
}

float ad7799_read_mV_single()
{
    float rawData = (float)ad7799_read_raw_data_single();
    float voltage = ((rawData * 0.00000011920928955078125f - 1.0f) * vRef) / (gain * 1000.0f);

    return voltage;
}

// #endregion

// #region Private Functions Implementations

static void write_register_8bit(
    const uint8_t register_address,
    const uint8_t value)
{
    uint8_t out_data[2] =
        {
            AD779X_COMM_WRITE | register_address,
            value};

    APP_ERROR_CHECK(spi_write(spi, out_data, 2));
}

static void write_register_16bit(
    const uint8_t register_address,
    const uint16_t value)
{
    uint8_t out_data[3] =
        {
            AD779X_COMM_WRITE | register_address,
            (uint8_t)((value & 0x00FF00) >> 8),
            (uint8_t)((value & 0x0000FF) >> 0),
        };

    APP_ERROR_CHECK(spi_write(spi, out_data, 3));
}

static uint8_t read_register_8bit(const uint8_t register_address)
{
    uint8_t out_data[1] = {AD779X_COMM_READ | register_address};
    uint8_t in_data[1] = {0x00};
    uint8_t received = 0;

    APP_ERROR_CHECK(spi_write(spi, out_data, 1));
    APP_ERROR_CHECK(spi_read(spi, in_data, 1));

    received = in_data[0];

    return received;
}

static uint16_t read_register_16bit(const uint8_t register_address)
{
    uint8_t out_data[1] = {AD779X_COMM_READ | register_address};
    uint8_t in_data[2] = {0x00, 0x00};
    uint16_t received = 0;

    APP_ERROR_CHECK(spi_write(spi, out_data, 1));
    APP_ERROR_CHECK(spi_read(spi, in_data, 2));

    received |= in_data[0] << 8;
    received |= in_data[1] << 0;

    return received;
}

static uint32_t read_register_24bit(const uint8_t register_address)
{
    uint8_t out_data[1] = {AD779X_COMM_READ | register_address};
    uint8_t in_data[3] = {0x00, 0x00, 0x00};
    uint32_t received = 0;

    APP_ERROR_CHECK(spi_write(spi, out_data, 1));
    APP_ERROR_CHECK(spi_read(spi, in_data, 3));

    received |= in_data[0] << 16;
    received |= in_data[1] << 8;
    received |= in_data[2] << 0;

    return received;
}

// #endregion