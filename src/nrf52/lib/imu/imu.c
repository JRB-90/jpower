#include "imu.h"

#include "nrf_log.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "i2c_helper.h"
#include "lsm6dso_reg.h"
#include "cadence.h"

static stmdev_ctx_t dev_ctx;
static nrf_drv_twi_t* twi = NULL;
static lsm6dso_fs_xl_t accel_range = LSM6DSO_16g;
static lsm6dso_fs_g_t gyro_range = LSM6DSO_2000dps;
static imu_reading_t current_reading = { 0 };

static activity_event_cb activity_event_callback;

static ret_code_t imu_take_reading();
static int32_t platform_write(
    void* handle, 
    uint8_t reg, 
    const uint8_t* bufp,
    uint16_t len
);
static int32_t platform_read(
    void* handle, 
    uint8_t reg, 
    uint8_t* bufp,
    uint16_t len
);
static float convert_accel_data(int32_t raw_value);
static float convert_gyro_data(int32_t raw_value);
static void wake_handler(
    nrf_drv_gpiote_pin_t pin, 
    nrf_gpiote_polarity_t action
);

ret_code_t imu_init(
    nrf_drv_twi_t* twi_instance, 
    const uint32_t scl_pin,
    const uint32_t sda_pin,
    const uint32_t wake_pin)
{
    ret_code_t err_code;

    twi = twi_instance; 
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;

    nrf_delay_ms(LSM6DS3TR_BOOT_TIME_MS);

    err_code = 
        i2c_init(
            twi,
            scl_pin,
            sda_pin
        );
    APP_ERROR_CHECK(err_code);

    nrf_delay_ms(I2C_BOOT_TIME_MS);

    uint8_t whoamI;
    lsm6dso_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LSM6DSO_ID)
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }

    // Restore default configuration
    lsm6dso_reset_set(&dev_ctx, PROPERTY_ENABLE);
    
    uint8_t reset_complete;

    do {
        lsm6dso_reset_get(&dev_ctx, &reset_complete);
    } while (reset_complete);

    lsm6dso_i3c_disable_set(&dev_ctx, LSM6DSO_I3C_DISABLE);
    lsm6dso_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    lsm6dso_xl_data_rate_set(&dev_ctx, LSM6DSO_XL_ODR_104Hz);
    lsm6dso_xl_full_scale_set(&dev_ctx, LSM6DSO_16g);
    lsm6dso_gy_data_rate_set(&dev_ctx, LSM6DSO_GY_ODR_104Hz);
    lsm6dso_gy_full_scale_set(&dev_ctx, LSM6DSO_2000dps);

    accel_range = LSM6DSO_16g;
    gyro_range = LSM6DSO_2000dps;

    // TODO - Set up wake interupt
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(wake_pin, &in_config, wake_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(wake_pin, true);

    nrf_delay_ms(LSM6DS3TR_BOOT_TIME_MS);

    err_code = cadence_init();
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

void imu_register_activity_event_cb(activity_event_cb callback)
{
    activity_event_callback = callback;
}

void imu_update_10ms(float time_delta_s)
{
    ret_code_t err_code;

    err_code = imu_take_reading();
    APP_ERROR_CHECK(err_code);

    pedal_state_t pedal_state;
    FusionQuaternion attitude;
    cadence_update(time_delta_s, &current_reading);
    cadence_get_attitude(&attitude);
    cadence_get_pedal_state(&pedal_state);
}

void imu_get_current_reading(imu_reading_t* const reading)
{
    *reading = current_reading;
}

void imu_enable_activity_tracking()
{
    // TODO - Have this as a configurable time?

    // ODR_XL = 104hz
    // FS_XL = 16g

    // Set duration for Activity detection to 9.62 ms (= 0x01 * 1 / ODR_XL)
    lsm6dso_wkup_dur_set(&dev_ctx, 0x01);

    // Set threshold calc to div by 2^8 (256)
    lsm6dso_wkup_ths_weight_set(&dev_ctx, LSM6DSO_LSb_FS_DIV_256);

    // Set Activity/Inactivity threshold to 62.5 mg (= 0x01 * FS_XL / 256)
    lsm6dso_wkup_threshold_set(&dev_ctx, 0x01);

    // Use hi pass filter on Wake-up monitor
    lsm6dso_xl_hp_path_internal_set(&dev_ctx, LSM6DSO_USE_SLOPE);

    // Set duration for Inactivity detection to ~10 s (= 0x02 * 512 / ODR_XL)
    lsm6dso_act_sleep_dur_set(&dev_ctx, 0x02);

    // Set gyro to switch to 12.5Hz when in sleep mode
    lsm6dso_act_mode_set(&dev_ctx, LSM6DSO_XL_12Hz5_GY_PD);

    lsm6dso_pin_int1_route_t int1_route;
    lsm6dso_pin_int1_route_get(&dev_ctx, &int1_route);
    int1_route.sleep_change = PROPERTY_ENABLE;
    int1_route.wake_up = PROPERTY_ENABLE;
    lsm6dso_pin_int1_route_set(&dev_ctx, int1_route);
}

void imu_disable_activity_tracking()
{
    lsm6dso_pin_int1_route_t int1_route;
    lsm6dso_pin_int1_route_get(&dev_ctx, &int1_route);
    int1_route.sleep_change = PROPERTY_DISABLE;
    int1_route.wake_up = PROPERTY_DISABLE;
    lsm6dso_pin_int1_route_set(&dev_ctx, int1_route);
}

static ret_code_t imu_take_reading()
{
    uint8_t buff[12];
    int16_t accelValues[3];
    int16_t gyroValues[3];

    int32_t ret =
        lsm6dso_read_reg(
            &dev_ctx, 
            LSM6DSO_OUTX_L_G, 
            buff, 
            12
        );

    if (ret)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    gyroValues[0] = (int16_t)buff[1];
    gyroValues[0] = (gyroValues[0] * 256) + (int16_t)buff[0];
    gyroValues[1] = (int16_t)buff[3];
    gyroValues[1] = (gyroValues[1] * 256) + (int16_t)buff[2];
    gyroValues[2] = (int16_t)buff[5];
    gyroValues[2] = (gyroValues[2] * 256) + (int16_t)buff[4];

    current_reading.gyro[0] = convert_gyro_data(gyroValues[0]) / 1000.0f;
    current_reading.gyro[1] = convert_gyro_data(gyroValues[1]) / 1000.0f;
    current_reading.gyro[2] = convert_gyro_data(gyroValues[2]) / 1000.0f;

    accelValues[0] = (int16_t)buff[7];
    accelValues[0] = (accelValues[0] * 256) + (int16_t)buff[6];
    accelValues[1] = (int16_t)buff[9];
    accelValues[1] = (accelValues[1] * 256) + (int16_t)buff[8];
    accelValues[2] = (int16_t)buff[11];
    accelValues[2] = (accelValues[2] * 256) + (int16_t)buff[10];

    current_reading.accel[0] = convert_accel_data(accelValues[0]) / 1000.0f;
    current_reading.accel[1] = convert_accel_data(accelValues[1]) / 1000.0f;
    current_reading.accel[2] = convert_accel_data(accelValues[2]) / 1000.0f;

    return NRF_SUCCESS;
}

static int32_t platform_write(
    void* handle, 
    uint8_t reg, 
    const uint8_t* bufp,
    uint16_t len)
{
    return
        i2c_write(
            twi,
            LSM6DS3TR_I2C_ADDR,
            reg,
            bufp,
            len
        );
}

static int32_t platform_read(
    void* handle, 
    uint8_t reg, 
    uint8_t* bufp,
    uint16_t len)
{
    return
        i2c_read(
            twi,
            LSM6DS3TR_I2C_ADDR,
            reg,
            bufp,
            len
        );
}

static float convert_accel_data(int32_t raw_value)
{
    switch(accel_range)
    {
        case LSM6DSO_2g:
            return lsm6dso_from_fs2_to_mg(raw_value);
        
        case LSM6DSO_4g:
            return lsm6dso_from_fs4_to_mg(raw_value);

        case LSM6DSO_8g:
            return lsm6dso_from_fs8_to_mg(raw_value);

        case LSM6DSO_16g:
            return lsm6dso_from_fs16_to_mg(raw_value);

        default:
            return NAN;
    }
}

static float convert_gyro_data(int32_t raw_value)
{
    switch(gyro_range)
    {
        case LSM6DSO_125dps:
            return lsm6dso_from_fs125_to_mdps(raw_value);

        case LSM6DSO_250dps:
            return lsm6dso_from_fs250_to_mdps(raw_value);

        case LSM6DSO_500dps:
            return lsm6dso_from_fs500_to_mdps(raw_value);

        case LSM6DSO_1000dps:
            return lsm6dso_from_fs1000_to_mdps(raw_value);

        case LSM6DSO_2000dps:
            return lsm6dso_from_fs2000_to_mdps(raw_value);

        default:
            return NAN;
    }
}

static void wake_handler(
    nrf_drv_gpiote_pin_t pin, 
    nrf_gpiote_polarity_t action)
{
    lsm6dso_all_sources_t all_source;
    lsm6dso_all_sources_get(&dev_ctx, &all_source);

    if (all_source.sleep_state)
    {
        activity_event_callback(IMU_ACTIVITY_EVENT_SLEEP);
    }

    if (all_source.wake_up)
    {
        activity_event_callback(IMU_ACTIVITY_EVENT_WAKE_UP);
    }
}