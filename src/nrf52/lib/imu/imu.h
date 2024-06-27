#ifndef IMU_H__
#define IMU_H__

#include <stdint.h>
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "imu_types.h"

#define OVERSAMPLE_RATIO        4
#define MAGBUFFSIZE             650
#define I2C_BOOT_TIME_MS        10
#define LSM6DS3TR_BOOT_TIME_MS  10
#define LSM6DS3TR_I2C_ADDR      0x6A
#define LSM6DSR_BOOT_TIME_MS    10
#define LSM6DSR_I2C_ADDR        0x6A

extern ret_code_t imu_init(
    nrf_drv_twi_t* twi_instance,
    const uint32_t scl_pin,
    const uint32_t sda_pin,
    const uint32_t wake_pin
);
extern void imu_register_activity_event_cb(activity_event_cb callback);
extern void imu_update_10ms(float time_delta_s);
extern void imu_get_current_reading(imu_reading_t* const reading);
extern void imu_enable_activity_tracking();
extern void imu_disable_activity_tracking();

#endif // IMU_H__