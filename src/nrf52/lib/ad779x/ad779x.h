#ifndef AD7799_H__
#define AD7799_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"

#define AD7799_BOOT_TIME_MS     10
#define AD7799_RESET_TIME_MS    1

// #region Data Types

typedef void (*delay_func_t)(uint32_t millis);
typedef ret_code_t (*write_func_t)(const uint8_t *, uint8_t);
typedef ret_code_t (*read_func_t)(uint8_t *, uint8_t);

typedef struct
{
    delay_func_t delay_func;
    write_func_t write_func;
    read_func_t read_func;
} device_context_t;

// Status Register Setting Types

typedef enum
{
    AD7798 = 0b00000000,
    AD7799 = 0b00001000
} chip_model_t;

typedef struct
{
    bool is_ready;
    bool is_error;
    bool is_no_ref;
    chip_model_t chip_model;
    bool adc_channel_1_en;
    bool adc_channel_2_en;
    bool adc_channel_3_en;
} ad7799_status_reg_t;

// Mode Register Setting Types

typedef enum
{
    AD7799_MODE_CONTINOUS = 0b0000000000000000,
    AD7799_MODE_SINGLE = 0b0010000000000000,
    AD7799_MODE_IDLE = 0b0100000000000000,
    AD7799_MODE_PWR_DOWN = 0b0110000000000000,
    AD7799_MODE_CAL_INT_ZERO = 0b1000000000000000,
    AD7799_MODE_CAL_INT_FSCALE = 0b1010000000000000,
    AD7799_MODE_CAL_SYS_ZERO = 0b1100000000000000,
    AD7799_MODE_CAL_SYS_FSCALE = 0b1110000000000000,
} ad7799_mode_t;

typedef enum
{
    AD7799_PSW_CLOSED = 0b0001000000000000,
    AD7799_PSW_OPEN = 0b0000000000000000,
} ad7799_psw_state_t;

typedef enum
{
    AD7799_FILTER_RATE_470HZ = 0b0000000000000001,
    AD7799_FILTER_RATE_242HZ = 0b0000000000000010,
    AD7799_FILTER_RATE_123HZ = 0b0000000000000011,
    AD7799_FILTER_RATE_62HZ = 0b0000000000000100,
    AD7799_FILTER_RATE_50HZ = 0b0000000000000101,
    AD7799_FILTER_RATE_39HZ = 0b0000000000000110,
    AD7799_FILTER_RATE_33_2HZ = 0b0000000000000111,
    AD7799_FILTER_RATE_19_6HZ = 0b0000000000001000,
    AD7799_FILTER_RATE_16_7HZ_80DB = 0b0000000000001001,
    AD7799_FILTER_RATE_16_7HZ_65DB = 0b0000000000001010,
    AD7799_FILTER_RATE_12_5HZ = 0b0000000000001011,
    AD7799_FILTER_RATE_10HZ = 0b0000000000001100,
    AD7799_FILTER_RATE_8_33HZ = 0b0000000000001101,
    AD7799_FILTER_RATE_6_25HZ = 0b0000000000001110,
    AD7799_FILTER_RATE_4_7HZ = 0b0000000000001111,
} ad7799_filter_rate_t;

typedef struct
{
    ad7799_mode_t mode;
    ad7799_psw_state_t psw_state;
    ad7799_filter_rate_t filter_rate;
} ad7799_mode_reg_t;

// Configuration Register Setting Types

typedef enum
{
    AD7799_CONF_BURNOUT_ENABLED = 0b0010000000000000,
    AD7799_CONF_BURNOUT_DISABLED = 0b0000000000000000,
} ad7799_burnout_enable_state_t;

typedef enum
{
    AD7799_CONF_UNIPOLAR = 0b0001000000000000,
    AD7799_CONF_BIPOLAR = 0b0000000000000000,
} ad7799_coding_polarity_t;

typedef enum
{
    AD7799_CONF_GAIN_1 = 0b0000000000000000,
    AD7799_CONF_GAIN_2 = 0b0000000100000000,
    AD7799_CONF_GAIN_4 = 0b0000001000000000,
    AD7799_CONF_GAIN_8 = 0b0000001100000000,
    AD7799_CONF_GAIN_16 = 0b0000010000000000,
    AD7799_CONF_GAIN_32 = 0b0000010100000000,
    AD7799_CONF_GAIN_64 = 0b0000011000000000,
    AD7799_CONF_GAIN_128 = 0b0000011100000000,
} ad7799_gain_t;

typedef enum
{
    AD7799_CONF_REF_DET_ENABLED = 0b0000000000100000,
    AD7799_CONF_REF_DET_DISABLED = 0b0000000000000000,
} ad7799_ref_detect_t;

typedef enum
{
    AD7799_CONF_BUFFERED = 0b0000000000010000,
    AD7799_CONF_UNBUFFERED = 0b0000000000000000,
} ad7799_buffer_mode_t;

typedef enum
{
    AD7799_CONF_CHAN_AIN1_POS = 0b0000000000000000,
    AD7799_CONF_CHAN_AIN2_POS = 0b0000000000000001,
    AD7799_CONF_CHAN_AIN3_POS = 0b0000000000000010,
    AD7799_CONF_CHAN_AIN1_NEG = 0b0000000000000011,
    AD7799_CONF_CHAN_AVDD_MON = 0b0000000000000111,
} ad7799_channel_select_t;

typedef struct
{
    ad7799_burnout_enable_state_t burnout_enable;
    ad7799_coding_polarity_t coding_polarity;
    ad7799_gain_t gain;
    ad7799_ref_detect_t ref_detect;
    ad7799_buffer_mode_t buffer_mode;
    ad7799_channel_select_t channel_select;
} ad7799_conf_reg_t;

// IO Register Setting Types

typedef enum
{
    AD7799_IO_ANALOG_INPUT = 0b00000000,
    AD7799_IO_DIGITAL_OUTPUT = 0b01000000,
} ad7799_ain3_setting_t;

typedef struct
{
    ad7799_ain3_setting_t ain3_setting;
    bool p1_state;
    bool p2_state;
} ad7799_io_reg_t;

// #endregion

// #region Public Functions

extern ret_code_t ad7799_init(device_context_t *const device);

extern void ad7799_reset(device_context_t *const device);

extern bool ad7799_is_ready(device_context_t *const device);

extern ad7799_status_reg_t ad7799_get_status(device_context_t *const device);

extern ad7799_mode_reg_t ad7799_get_mode(device_context_t *const device);

extern void ad7799_set_mode(
    device_context_t *const device,
    const ad7799_mode_reg_t *const mode);

extern void ad7799_set_mode_direct(
    device_context_t *const device,
    ad7799_mode_t mode,
    ad7799_psw_state_t psw_state,
    ad7799_filter_rate_t filter_rate);

extern ad7799_conf_reg_t ad7799_get_conf(device_context_t *const device);

extern void ad7799_set_conf(
    device_context_t *const device,
    const ad7799_conf_reg_t *const conf);

extern void ad7799_set_conf_direct(
    device_context_t *const device,
    ad7799_burnout_enable_state_t burnout_enable_state,
    ad7799_coding_polarity_t coding_polarity,
    ad7799_gain_t gain,
    ad7799_ref_detect_t ref_detect,
    ad7799_buffer_mode_t buffer_mode,
    ad7799_channel_select_t channel_select);

extern uint32_t ad7799_get_data(device_context_t *const device);

extern uint8_t ad7799_get_id(device_context_t *const device);

extern ad7799_io_reg_t ad7799_get_io(device_context_t *const device);

extern void ad7799_set_io(
    device_context_t *const device,
    ad7799_ain3_setting_t ain3_setting,
    bool p1_state,
    bool p2_state);

extern uint32_t ad7799_get_offset(device_context_t *const device);

// Note: Must be in Idle or Power Down state to set this
extern void ad7799_set_offset(device_context_t *const device, uint32_t value);

extern uint32_t ad7799_get_fullscale(device_context_t *const device);

extern void ad7799_set_fullscale(device_context_t *const device, uint32_t value);

extern void ad7799_conf_to_str(
    const ad7799_conf_reg_t *const config,
    char *output_str);

// #endregion

#endif // AD7799_H__