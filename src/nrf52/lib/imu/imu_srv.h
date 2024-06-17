#ifndef IMU_SRV_H__
#define IMU_SRV_H__

#include <stdint.h>
#include "app_error.h"
#include "imu.h"

// 128-bit base UUID
#define BLE_IMU_SRV_BASE_UUID               { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_IMU_SRV_SERVICE_UUID            0xC000
#define BLE_IMU_SRV_ACCEL_RAW_CHAR_UUID     0xCC01
#define BLE_IMU_SRV_GYRO_RAW_CHAR_UUID      0xCC02

#define BLE_IMU_SRV_CHAR_COUNT              2
#define BLE_IMU_SRV_ACCEL_RAW_INDEX         0
#define BLE_IMU_SRV_GYRO_RAW_INDEX          1

extern ret_code_t imu_srv_init();
extern ret_code_t imu_srv_update_raw_accel(float value);
extern ret_code_t imu_srv_update_raw_gyro(float value);

#endif // IMU_SRV_H__