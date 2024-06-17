#ifndef IMU_H__
#define IMU_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_twi.h"

#define OVERSAMPLE_RATIO        4
#define MAGBUFFSIZE             650
#define I2C_BOOT_TIME_MS        10
#define LSM6DS3TR_BOOT_TIME_MS  10
#define LSM6DS3TR_I2C_ADDR      0x6A
#define LSM6DSR_BOOT_TIME_MS    10
#define LSM6DSR_I2C_ADDR        0x6A

typedef void (*on_accel_value_updated_t)(float value);
typedef void (*on_gyro_value_updated_t)(float value);

extern ret_code_t imu_init(
    nrf_drv_twi_t* twi_instance,
    const uint32_t scl_pin,
    const uint32_t sda_pin
);
extern void imu_register_accel_value_updated_cb(on_accel_value_updated_t callback);
extern void imu_register_gyro_value_updated_cb(on_gyro_value_updated_t callback);
extern void imu_update();
extern ret_code_t imu_read_accel(float* const data);
extern ret_code_t imu_read_gyro(float* const data);
extern ret_code_t imu_take_reading_raw(
    float* const accel,
    float* const gyro
);

#endif // IMU_H__