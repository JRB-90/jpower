#include "imu.h"

#include <string.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "i2c_helper.h"

static stmdev_ctx_t dev_ctx;
static nrf_drv_twi_t* twi = NULL;
static lsm6ds3tr_c_fs_xl_t accel_range = LSM6DS3TR_C_16g;
static lsm6ds3tr_c_fs_g_t gyro_range = LSM6DS3TR_C_2000dps;

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

ret_code_t imu_init(
    nrf_drv_twi_t* twi_instance, 
    const uint32_t imu_pwr_pin,
    const uint32_t scl_pin,
    const uint32_t sda_pin)
{
    ret_code_t err_code;

    twi = twi_instance; 
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;

    nrf_gpio_cfg_output(imu_pwr_pin);

    // For some reason, we have to configure some pins as below
    // I don't know exactly why, but i2c init can fail if this 
    // isn't in place. I took it from the seeed arduino library
    // for the lsm6ds3
    // https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3/blob/master/LSM6DS3.cpp
    NRF_P1->PIN_CNF[8] = 
        ((uint32_t)NRF_GPIO_PIN_DIR_OUTPUT << GPIO_PIN_CNF_DIR_Pos) |
        ((uint32_t)NRF_GPIO_PIN_INPUT_DISCONNECT << GPIO_PIN_CNF_INPUT_Pos) |
        ((uint32_t)NRF_GPIO_PIN_NOPULL << GPIO_PIN_CNF_PULL_Pos) |
        ((uint32_t)NRF_GPIO_PIN_H0H1 << GPIO_PIN_CNF_DRIVE_Pos) |
        ((uint32_t)NRF_GPIO_PIN_NOSENSE << GPIO_PIN_CNF_SENSE_Pos);

    nrf_gpio_pin_set(imu_pwr_pin);

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
    lsm6ds3tr_c_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LSM6DS3TR_C_ID)
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }

    // Restore default configuration
    lsm6ds3tr_c_reset_set(&dev_ctx, PROPERTY_ENABLE);
    lsm6ds3tr_c_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    lsm6ds3tr_c_xl_data_rate_set(&dev_ctx, LSM6DS3TR_C_XL_ODR_104Hz);
    lsm6ds3tr_c_xl_full_scale_set(&dev_ctx, LSM6DS3TR_C_16g);
    lsm6ds3tr_c_gy_data_rate_set(&dev_ctx, LSM6DS3TR_C_GY_ODR_104Hz);
    lsm6ds3tr_c_gy_full_scale_set(&dev_ctx, LSM6DS3TR_C_2000dps);

    accel_range = LSM6DS3TR_C_16g;
    gyro_range = LSM6DS3TR_C_2000dps;

    nrf_delay_ms(LSM6DS3TR_BOOT_TIME_MS);

    return NRF_SUCCESS;
}

ret_code_t imu_read_accel(float* const data)
{
    int16_t raw_data[3];
    if (lsm6ds3tr_c_acceleration_raw_get(&dev_ctx, raw_data))
    {
        return NRF_ERROR_INVALID_DATA;
    }

    data[0] = convert_accel_data(raw_data[0]) / 1000.0f;
    data[1] = convert_accel_data(raw_data[1]) / 1000.0f;
    data[2] = convert_accel_data(raw_data[2]) / 1000.0f;

    return NRF_SUCCESS;
}

ret_code_t imu_read_gyro(float* const data)
{
    int16_t raw_data[3];
    if (lsm6ds3tr_c_angular_rate_raw_get(&dev_ctx, raw_data))
    {
        return NRF_ERROR_INVALID_DATA;
    }

    data[0] = convert_gyro_data(raw_data[0]) / 1000.0f;
    data[1] = convert_gyro_data(raw_data[1]) / 1000.0f;
    data[2] = convert_gyro_data(raw_data[2]) / 1000.0f;

    return NRF_SUCCESS;
}

ret_code_t imu_take_reading_raw(
    float* const accel,
    float* const gyro)
{
    uint8_t buff[12];
    int16_t accelValues[3];
    int16_t gyroValues[3];

    int32_t ret =
        lsm6ds3tr_c_read_reg(
            &dev_ctx, 
            LSM6DS3TR_C_OUTX_L_G, 
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

    gyro[0] = convert_gyro_data(gyroValues[0]) / 1000.0f;
    gyro[1] = convert_gyro_data(gyroValues[1]) / 1000.0f;
    gyro[2] = convert_gyro_data(gyroValues[2]) / 1000.0f;

    accelValues[0] = (int16_t)buff[7];
    accelValues[0] = (accelValues[0] * 256) + (int16_t)buff[6];
    accelValues[1] = (int16_t)buff[9];
    accelValues[1] = (accelValues[1] * 256) + (int16_t)buff[8];
    accelValues[2] = (int16_t)buff[11];
    accelValues[2] = (accelValues[2] * 256) + (int16_t)buff[10];

    accel[0] = convert_accel_data(accelValues[0]) / 1000.0f;
    accel[1] = convert_accel_data(accelValues[1]) / 1000.0f;
    accel[2] = convert_accel_data(accelValues[2]) / 1000.0f;

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
        case LSM6DS3TR_C_2g:
            return lsm6ds3tr_c_from_fs2g_to_mg(raw_value);
        
        case LSM6DS3TR_C_4g:
            return lsm6ds3tr_c_from_fs4g_to_mg(raw_value);

        case LSM6DS3TR_C_8g:
            return lsm6ds3tr_c_from_fs8g_to_mg(raw_value);

        case LSM6DS3TR_C_16g:
            return lsm6ds3tr_c_from_fs16g_to_mg(raw_value);

        default:
            return NAN;
    }
}

static float convert_gyro_data(int32_t raw_value)
{
    switch(gyro_range)
    {
        case LSM6DS3TR_C_125dps:
            return lsm6ds3tr_c_from_fs125dps_to_mdps(raw_value);

        case LSM6DS3TR_C_250dps:
            return lsm6ds3tr_c_from_fs250dps_to_mdps(raw_value);

        case LSM6DS3TR_C_500dps:
            return lsm6ds3tr_c_from_fs500dps_to_mdps(raw_value);

        case LSM6DS3TR_C_1000dps:
            return lsm6ds3tr_c_from_fs1000dps_to_mdps(raw_value);

        case LSM6DS3TR_C_2000dps:
            return lsm6ds3tr_c_from_fs2000dps_to_mdps(raw_value);

        default:
            return NAN;
    }
}
