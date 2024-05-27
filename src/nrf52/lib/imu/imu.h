#ifndef IMU_H__
#define IMU_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "lsm6ds3tr-c_reg.h"

#define OVERSAMPLE_RATIO        4
#define MAGBUFFSIZE             650
#define I2C_BOOT_TIME_MS        10
#define LSM6DS3TR_BOOT_TIME_MS  10
#define LSM6DS3TR_I2C_ADDR      0x6A

// ===== Functions =====

extern ret_code_t imu_init(
    nrf_drv_twi_t* twi_instance,
    const uint32_t imu_pwr_pin,
    const uint32_t scl_pin,
    const uint32_t sda_pin
);

extern ret_code_t imu_read_accel(float* const data);
extern ret_code_t imu_read_gyro(float* const data);
extern ret_code_t imu_take_reading_raw(
    float* const accel,
    float* const gyro
);

#endif // IMU_H__