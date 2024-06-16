#ifndef AD779X_H__
#define AD779X_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_spi.h"

// #region Data Types

// Status Register Setting Types
typedef enum
{
    AD7798_ID                       = 0b00000000,
    AD7799_ID                       = 0b00001000
} ad779x_model_t;

typedef struct
{
    ad779x_model_t  model_type;
    bool            is_ready;
    bool            is_error;
    bool            is_no_ref;
    bool            adc_channel_1_en;
    bool            adc_channel_2_en;
    bool            adc_channel_3_en;
} ad779x_status_reg_t;

// Mode Register Setting Types
typedef enum
{
    AD779X_MODE_CONTINOUS           = 0b0000000000000000,
    AD779X_MODE_SINGLE              = 0b0010000000000000,
    AD779X_MODE_IDLE                = 0b0100000000000000,
    AD779X_MODE_PWR_DOWN            = 0b0110000000000000,
    AD779X_MODE_CAL_INT_ZERO        = 0b1000000000000000,
    AD779X_MODE_CAL_INT_FSCALE      = 0b1010000000000000,
    AD779X_MODE_CAL_SYS_ZERO        = 0b1100000000000000,
    AD779X_MODE_CAL_SYS_FSCALE      = 0b1110000000000000,
} ad779x_mode_t;

typedef enum
{
    AD779X_PSW_CLOSED               = 0b0001000000000000,
    AD779X_PSW_OPEN                 = 0b0000000000000000,
} ad779x_psw_state_t;

typedef enum
{
    AD779X_FILTER_RATE_470HZ        = 0b0000000000000001,
    AD779X_FILTER_RATE_242HZ        = 0b0000000000000010,
    AD779X_FILTER_RATE_123HZ        = 0b0000000000000011,
    AD779X_FILTER_RATE_62HZ         = 0b0000000000000100,
    AD779X_FILTER_RATE_50HZ         = 0b0000000000000101,
    AD779X_FILTER_RATE_39HZ         = 0b0000000000000110,
    AD779X_FILTER_RATE_33_2HZ       = 0b0000000000000111,
    AD779X_FILTER_RATE_19_6HZ       = 0b0000000000001000,
    AD779X_FILTER_RATE_16_7HZ_80DB  = 0b0000000000001001,
    AD779X_FILTER_RATE_16_7HZ_65DB  = 0b0000000000001010,
    AD779X_FILTER_RATE_12_5HZ       = 0b0000000000001011,
    AD779X_FILTER_RATE_10HZ         = 0b0000000000001100,
    AD779X_FILTER_RATE_8_33HZ       = 0b0000000000001101,
    AD779X_FILTER_RATE_6_25HZ       = 0b0000000000001110,
    AD779X_FILTER_RATE_4_7HZ        = 0b0000000000001111,
} ad779x_filter_rate_t;

typedef struct
{
    ad779x_mode_t           mode;
    ad779x_psw_state_t      psw_state;
    ad779x_filter_rate_t    filter_rate;
} ad779x_mode_reg_t;

// Configuration Register Setting Types
typedef enum
{
    AD779X_CONF_BURNOUT_ENABLED     = 0b0010000000000000,
    AD779X_CONF_BURNOUT_DISABLED    = 0b0000000000000000,
} ad779x_burnout_enable_state_t;

typedef enum
{
    AD779X_CONF_UNIPOLAR            = 0b0001000000000000,
    AD779X_CONF_BIPOLAR             = 0b0000000000000000,
} ad779x_coding_polarity_t;

typedef enum
{
    AD779X_CONF_GAIN_1              = 0b0000000000000000,
    AD779X_CONF_GAIN_2              = 0b0000000100000000,
    AD779X_CONF_GAIN_4              = 0b0000001000000000,
    AD779X_CONF_GAIN_8              = 0b0000001100000000,
    AD779X_CONF_GAIN_16             = 0b0000010000000000,
    AD779X_CONF_GAIN_32             = 0b0000010100000000,
    AD779X_CONF_GAIN_64             = 0b0000011000000000,
    AD779X_CONF_GAIN_128            = 0b0000011100000000,
} ad779x_gain_t;

typedef enum
{
    AD779X_CONF_REF_DET_ENABLED     = 0b0000000000100000,
    AD779X_CONF_REF_DET_DISABLED    = 0b0000000000000000,
} ad779x_ref_detect_t;

typedef enum
{
    AD779X_CONF_BUFFERED            = 0b0000000000010000,
    AD779X_CONF_UNBUFFERED          = 0b0000000000000000,
} ad779x_buffer_mode_t;

typedef enum
{
    AD779X_CONF_CHAN_AIN1_POS       = 0b0000000000000000,
    AD779X_CONF_CHAN_AIN2_POS       = 0b0000000000000001,
    AD779X_CONF_CHAN_AIN3_POS       = 0b0000000000000010,
    AD779X_CONF_CHAN_AIN1_NEG       = 0b0000000000000011,
    AD779X_CONF_CHAN_AVDD_MON       = 0b0000000000000111,
} ad779x_channel_select_t;

typedef struct
{
    ad779x_burnout_enable_state_t   burnout_enable;
    ad779x_coding_polarity_t        coding_polarity;
    ad779x_gain_t                   gain;
    ad779x_ref_detect_t             ref_detect;
    ad779x_buffer_mode_t            buffer_mode;
    ad779x_channel_select_t         channel_select;
} ad779x_conf_reg_t;

// IO Register Setting Types
typedef enum
{
    AD779x_IO_ANALOG_INPUT          = 0b00000000,
    AD779x_IO_DIGITAL_OUTPUT        = 0b01000000,
} ad779x_ain3_setting_t;

typedef struct
{
    ad779x_ain3_setting_t   ain3_setting;
    bool                    p1_state;
    bool                    p2_state;
} ad779x_io_reg_t;

// #endregion

// #region Public Functions

extern ret_code_t ad779x_init(
    nrf_drv_spi_t* spi_instance,
    const uint8_t sck_pin,
    const uint8_t mosi_pin,
    const uint8_t miso_pin,
    const uint8_t ss_pin
);

extern void ad779x_reset();
extern bool ad779x_is_ready();
extern void ad779x_set_default_settings();

extern void ad779x_internal_zeroscale_calibration();
extern void ad779x_internal_fullscale_calibration();
extern void ad779x_system_zeroscale_calibration();
extern void ad779x_system_fullscale_calibration();

extern uint8_t ad779x_read_id_reg();
extern ad779x_status_reg_t ad779x_read_status_reg();
extern ad779x_mode_reg_t ad779x_read_mode_reg();
extern void ad779x_write_mode_reg(const ad779x_mode_reg_t* const mode);
extern ad779x_conf_reg_t ad779x_read_conf_reg();
extern void ad779x_write_conf_reg(const ad779x_conf_reg_t* const conf);

extern uint16_t ad7798_read_raw_data_single();
extern uint32_t ad7799_read_raw_data_single();
extern float ad7798_read_mV_single();
extern float ad7799_read_mV_single();

// #endregion

#endif // AD779X_H__