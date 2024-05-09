#include "ad779x.h"
#include <string.h>
#include "nrf_log.h"

// #region Defines

#define AD7798_CHIP_ID 0x08
#define AD7799_CHIP_ID 0x09

#define AD7799_REG_COMM 0x00
#define AD7799_REG_STATUS 0x00
#define AD7799_REG_MODE 0x08
#define AD7799_REG_CONF 0x10
#define AD7799_REG_DATA 0x18
#define AD7799_REG_ID 0x20
#define AD7799_REG_IO 0x28
#define AD7799_REG_OFFSET 0x30
#define AD7799_REG_FULL_SCALE 0x38

#define AD7799_COMM_READ 0b01000000
#define AD7799_COMM_WRITE 0b00000000

#define AD7799_STATUS_NREADY 0b10000000

#define AD7799_READY_MASK 0b10000000
#define AD7799_ERROR_MASK 0b01000000
#define AD7799_NOREF_MASK 0b00100000
#define AD7799_MODEL_MASK 0b00001000
#define AD7799_CHAN1_MASK 0b00000001
#define AD7799_CHAN2_MASK 0b00000010
#define AD7799_CHAN3_MASK 0b00000100
#define AD7799_MODE_MASK 0b1110000000000000
#define AD7799_PSW_MASK 0b0001000000000000
#define AD7799_FILTER_MASK 0b0000000000001111
#define AD7799_BURNOUT_MASK 0b0010000000000000
#define AD7799_CODING_MASK 0b0001000000000000
#define AD7799_GAIN_MASK 0b0000011100000000
#define AD7799_REF_DETECT_MASK 0b0000000000100000
#define AD7799_BUF_MODE_MASK 0b0000000000010000
#define AD7799_CHAN_SEL_MASK 0b0000000000000111
#define AD7799_AIN3_SET_MASK 0b01000000
#define AD7799_P1_STATE_MASK 0b00100000
#define AD7799_P2_STATE_MASK 0b00010000

// #endregion

// #region Private Functions

static uint32_t read_register(
    device_context_t *const device,
    uint8_t address,
    uint8_t size);

static void write_register(
    device_context_t *const device,
    uint8_t address,
    uint8_t size,
    uint32_t value);

static uint8_t ad7799_read_status(device_context_t *const device);

static uint16_t ad7799_read_mode(device_context_t *const device);
static void write_mode(
    device_context_t *const device,
    uint16_t value);

static uint16_t read_conf(device_context_t *const device);
static void write_conf(
    device_context_t *const device,
    uint16_t value);

static uint32_t read_data(device_context_t *const device);

static uint8_t read_id(device_context_t *const device);

static uint8_t read_io(device_context_t *const device);
static void write_io(
    device_context_t *const device,
    uint8_t value);

static uint32_t read_offset(device_context_t *const device);
static void write_offset(
    device_context_t *const device,
    uint32_t value);

static uint32_t read_fullscale(device_context_t *const device);
static void write_fullscale(
    device_context_t *const device,
    uint32_t value);

// #endregion

// #region Public Functions

ret_code_t ad7799_init(device_context_t *const device)
{
    ad7799_reset(device);

    uint8_t id = (uint8_t)(read_register(device, AD7799_REG_ID, 1) & 0x0F);
    //uint8_t id = ad7799_get_id(device);
    //uint8_t id = read_id(device);

    NRF_LOG_INFO("Chip ID: %u", id);

    if (id == AD7799_CHIP_ID)
    {
        return NRF_SUCCESS;
    }
    else
    {
        return NRF_ERROR_INVALID_DATA;
    }
}

void ad7799_reset(device_context_t *const device)
{
    uint8_t reset_value[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    ret_code_t err_code = device->write_func(reset_value, 4);
    APP_ERROR_CHECK(err_code);
    
    device->delay_func(AD7799_RESET_TIME_MS);
}

bool ad7799_is_ready(device_context_t *const device)
{
    uint8_t status = ad7799_read_status(device);

    return !(status & AD7799_STATUS_NREADY);
}

ad7799_status_reg_t ad7799_get_status(device_context_t *const device)
{
    uint8_t value = ad7799_read_status(device);

    ad7799_status_reg_t status =
    {
        .is_ready = !(value & AD7799_READY_MASK),
        .is_error = value & AD7799_ERROR_MASK,
        .is_no_ref = value & AD7799_NOREF_MASK,
        .chip_model = (chip_model_t)(value & AD7799_MODEL_MASK),
        .adc_channel_1_en = value & AD7799_CHAN1_MASK,
        .adc_channel_2_en = value & AD7799_CHAN2_MASK,
        .adc_channel_3_en = value & AD7799_CHAN3_MASK,
    };

    return status;
}

ad7799_mode_reg_t ad7799_get_mode(device_context_t *const device)
{
    uint16_t value = ad7799_read_mode(device);

    ad7799_mode_reg_t mode =
    {
        .mode = value & AD7799_MODE_MASK,
        .psw_state = value & AD7799_PSW_MASK,
        .filter_rate = value & AD7799_FILTER_MASK,
    };

    return mode;
}

void ad7799_set_mode(
    device_context_t *const device,
    const ad7799_mode_reg_t *const mode)
{
    ad7799_set_mode_direct(
        device,
        mode->mode,
        mode->psw_state,
        mode->filter_rate);
}

void ad7799_set_mode_direct(
    device_context_t *const device,
    ad7799_mode_t mode,
    ad7799_psw_state_t psw_state,
    ad7799_filter_rate_t filter_rate)
{
    uint16_t value =
        (uint16_t)mode |
        (uint16_t)psw_state |
        (uint16_t)filter_rate;

    write_mode(device, value);
}

ad7799_conf_reg_t ad7799_get_conf(device_context_t *const device)
{
    uint16_t value = read_conf(device);

    ad7799_conf_reg_t mode =
        {
            .burnout_enable = value & AD7799_BURNOUT_MASK,
            .coding_polarity = value & AD7799_CODING_MASK,
            .gain = value & AD7799_GAIN_MASK,
            .ref_detect = value & AD7799_REF_DETECT_MASK,
            .buffer_mode = value & AD7799_BUF_MODE_MASK,
            .channel_select = value & AD7799_CHAN_SEL_MASK,
        };

    return mode;
}

void ad7799_set_conf(
    device_context_t *const device,
    const ad7799_conf_reg_t *const conf)
{
    ad7799_set_conf_direct(
        device,
        conf->burnout_enable,
        conf->coding_polarity,
        conf->gain,
        conf->ref_detect,
        conf->buffer_mode,
        conf->channel_select);
}

void ad7799_set_conf_direct(
    device_context_t *const device,
    ad7799_burnout_enable_state_t burnout_enable_state,
    ad7799_coding_polarity_t coding_polarity,
    ad7799_gain_t gain,
    ad7799_ref_detect_t ref_detect,
    ad7799_buffer_mode_t buffer_mode,
    ad7799_channel_select_t channel_select)
{
    uint16_t value =
        (uint16_t)burnout_enable_state |
        (uint16_t)coding_polarity |
        (uint16_t)gain |
        (uint16_t)ref_detect |
        (uint16_t)buffer_mode |
        (uint16_t)channel_select;

    write_conf(device, value);
}

uint32_t ad7799_get_data(device_context_t *const device)
{
    // TODO - Support continous mode?
    return read_data(device);
}

uint8_t ad7799_get_id(device_context_t *const device)
{
    uint8_t id = read_id(device);

    return id;
}

ad7799_io_reg_t ad7799_get_io(device_context_t *const device)
{
    uint8_t value = read_io(device);

    ad7799_io_reg_t mode =
        {
            .ain3_setting = value & AD7799_AIN3_SET_MASK,
            .p1_state = (bool)(value & AD7799_P1_STATE_MASK),
            .p2_state = (bool)(value & AD7799_P2_STATE_MASK),
        };

    return mode;
}

void ad7799_set_io(
    device_context_t *const device,
    ad7799_ain3_setting_t ain3_setting,
    bool p1_state,
    bool p2_state)
{
    uint8_t p1 = p1_state ? AD7799_P1_STATE_MASK : 0b00000000;
    uint8_t p2 = p2_state ? AD7799_P2_STATE_MASK : 0b00000000;

    uint8_t value =
        (uint8_t)ain3_setting |
        p1 |
        p2;

    write_io(device, value);
}

uint32_t ad7799_get_offset(device_context_t *const device)
{
    return read_offset(device);
}

void ad7799_set_offset(device_context_t *const device, uint32_t value)
{
    write_offset(device, value);
}

uint32_t ad7799_get_fullscale(device_context_t *const device)
{
    return read_fullscale(device);
}

void ad7799_set_fullscale(device_context_t *const device, uint32_t value)
{
    write_fullscale(device, value);
}

void ad7799_conf_to_str(
    const ad7799_conf_reg_t *const config,
    char *output_str)
{
    char burnout_str[32];

    switch (config->burnout_enable)
    {
    case AD7799_CONF_BURNOUT_ENABLED:
        sprintf(burnout_str, "Burnout Enabled");
        break;
    case AD7799_CONF_BURNOUT_DISABLED:
        sprintf(burnout_str, "Burnout Disabled");
        break;
    default:
        sprintf(burnout_str, "Unknown");
        break;
    }

    char coding_str[32];

    switch (config->coding_polarity)
    {
    case AD7799_CONF_UNIPOLAR:
        sprintf(coding_str, "UniPolar");
        break;
    case AD7799_CONF_BIPOLAR:
        sprintf(coding_str, "BiPolar");
        break;
    default:
        sprintf(coding_str, "Unknown");
        break;
    }

    char gain_str[32];

    switch (config->gain)
    {
    case AD7799_CONF_GAIN_1:
        sprintf(gain_str, "1");
        break;
    case AD7799_CONF_GAIN_2:
        sprintf(gain_str, "2");
        break;
    case AD7799_CONF_GAIN_4:
        sprintf(gain_str, "4");
        break;
    case AD7799_CONF_GAIN_8:
        sprintf(gain_str, "8");
        break;
    case AD7799_CONF_GAIN_16:
        sprintf(gain_str, "16");
        break;
    case AD7799_CONF_GAIN_32:
        sprintf(gain_str, "32");
        break;
    case AD7799_CONF_GAIN_64:
        sprintf(gain_str, "64");
        break;
    case AD7799_CONF_GAIN_128:
        sprintf(gain_str, "128");
        break;
    default:
        sprintf(gain_str, "Unknown");
        break;
    }

    char ref_str[32];

    switch (config->ref_detect)
    {
    case AD7799_CONF_REF_DET_ENABLED:
        sprintf(ref_str, "Det Enabled");
        break;
    case AD7799_CONF_REF_DET_DISABLED:
        sprintf(ref_str, "Det Disabled");
        break;
    default:
        sprintf(ref_str, "Unknown");
        break;
    }

    char buffer_str[32];

    switch (config->buffer_mode)
    {
    case AD7799_CONF_BUFFERED:
        sprintf(buffer_str, "Buffered");
        break;
    case AD7799_CONF_UNBUFFERED:
        sprintf(buffer_str, "Unbuffered");
        break;
    default:
        sprintf(buffer_str, "Unknown");
        break;
    }

    char channel_str[32];

    switch (config->channel_select)
    {
    case AD7799_CONF_CHAN_AIN1_POS:
        sprintf(channel_str, "AIN1 +");
        break;
    case AD7799_CONF_CHAN_AIN2_POS:
        sprintf(channel_str, "AIN2 +");
        break;
    case AD7799_CONF_CHAN_AIN3_POS:
        sprintf(channel_str, "AIN3 +");
        break;
    case AD7799_CONF_CHAN_AIN1_NEG:
        sprintf(channel_str, "AIN1 -");
        break;
    case AD7799_CONF_CHAN_AVDD_MON:
        sprintf(channel_str, "AVDD Monitor");
        break;
    default:
        sprintf(channel_str, "Unknown");
        break;
    }

    sprintf(
        output_str,
        "BE: %s, CP: %s, G: %s, RF: %s, BF: %s, CH: %s",
        burnout_str,
        coding_str,
        gain_str,
        ref_str,
        buffer_str,
        channel_str);
}

// #endregion

// #region Private Functions

static uint32_t read_register(
    device_context_t *const device,
    uint8_t address,
    uint8_t size)
{
    uint8_t out_data[1] = {AD7799_COMM_READ | address};
    uint8_t in_data[3] = {0x00, 0x00, 0x00};
    uint32_t received = 0x00;

    APP_ERROR_CHECK(device->write_func(out_data, 1));

    switch (size)
    {
    case 1:
        APP_ERROR_CHECK(device->read_func(in_data, 1));
        received |= in_data[0] << 0;
        break;

    case 2:
        APP_ERROR_CHECK(device->read_func(in_data, 2));
        received |= in_data[0] << 8;
        received |= in_data[1] << 0;
        break;

    case 3:
        APP_ERROR_CHECK(device->read_func(in_data, 3));
        received |= in_data[0] << 16;
        received |= in_data[1] << 8;
        received |= in_data[2] << 0;
        break;

    default:
        APP_ERROR_CHECK(NRF_ERROR_INVALID_PARAM);
    }

    return received;
}

static void write_register(
    device_context_t *const device,
    uint8_t address,
    uint8_t size,
    uint32_t value)
{
    uint8_t out_data[4] =
        {
            AD7799_COMM_WRITE | address,
            0x00,
            0x00,
            0x00};

    if (size == 1)
    {
        out_data[1] = (uint8_t)value;
    }

    if (size == 2)
    {
        out_data[2] = (uint8_t)((value & 0x0000FF) >> 0);
        out_data[1] = (uint8_t)((value & 0x00FF00) >> 8);
    }

    if (size == 3)
    {
        out_data[3] = (uint8_t)((value & 0x0000FF) >> 0);
        out_data[2] = (uint8_t)((value & 0x00FF00) >> 8);
        out_data[1] = (uint8_t)((value & 0xFF0000) >> 16);
    }

    APP_ERROR_CHECK(device->write_func(out_data, size + 1));
}

static uint8_t ad7799_read_status(device_context_t *const device)
{
    return (uint8_t)(read_register(device, AD7799_REG_STATUS, 1) & 0x000000FF);
}

static uint16_t ad7799_read_mode(device_context_t *const device)
{
    return (uint16_t)(read_register(device, AD7799_REG_MODE, 2) & 0x0000FFFF);
}

static void write_mode(device_context_t *const device, uint16_t value)
{
    write_register(device, AD7799_REG_MODE, 2, (uint32_t)value);
}

static uint16_t read_conf(device_context_t *const device)
{
    return (uint16_t)(read_register(device, AD7799_REG_CONF, 2) & 0x0000FFFF);
}

static void write_conf(device_context_t *const device, uint16_t value)
{
    write_register(device, AD7799_REG_CONF, 2, (uint32_t)value);
}

static uint32_t read_data(device_context_t *const device)
{
    return read_register(device, AD7799_REG_DATA, 3) & 0x00FFFFFF;
}

static uint8_t read_id(device_context_t *const device)
{
    return (uint8_t)(read_register(device, AD7799_REG_ID, 1) & 0x000000FF);
}

static uint8_t read_io(device_context_t *const device)
{
    return (uint8_t)(read_register(device, AD7799_REG_IO, 1) & 0x000000FF);
}

static void write_io(device_context_t *const device, uint8_t value)
{
    write_register(device, AD7799_REG_IO, 1, (uint32_t)value);
}

static uint32_t read_offset(device_context_t *const device)
{
    return (read_register(device, AD7799_REG_OFFSET, 3) & 0x00FFFFFF);
}

static void write_offset(device_context_t *const device, uint32_t value)
{
    write_register(device, AD7799_REG_OFFSET, 3, value);
}

static uint32_t read_fullscale(device_context_t *const device)
{
    return (read_register(device, AD7799_REG_FULL_SCALE, 3) & 0x00FFFFFF);
}

static void write_fullscale(device_context_t *const device, uint32_t value)
{
    write_register(device, AD7799_REG_FULL_SCALE, 3, value);
}

// #endregion