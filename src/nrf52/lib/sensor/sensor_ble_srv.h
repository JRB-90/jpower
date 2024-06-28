#ifndef SENSOR_BLE_SRV_H__
#define SENSOR_BLE_SRV_H__

#include <stdint.h>
#include "app_error.h"

// 128-bit base UUID
#define BLE_SENSOR_SRV_BASE_UUID                { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_SENSOR_SRV_SERVICE_UUID             0xA000
#define BLE_SENSOR_SRV_ZERO_OFFSET_CHAR_UUID    0xAA01
#define BLE_SENSOR_SRV_ADC_CHAR_UUID            0xAA02
#define BLE_SENSOR_SRV_POWER_CHAR_UUID          0xAA03
#define BLE_SENSOR_SRV_ACCEL_CHAR_UUID          0xAA04
#define BLE_SENSOR_SRV_GYRO_CHAR_UUID           0xAA05
#define BLE_SENSOR_SRV_ORIENT_CHAR_UUID         0xAA06
#define BLE_SENSOR_SRV_CADENCE_CHAR_UUID        0xAA07
#define BLE_SENSOR_SRV_TEMP_CHAR_UUID           0xAA08

#define BLE_SENSOR_SRV_CHAR_COUNT               8
#define BLE_SENSOR_SRV_ZERO_OFFSET_INDEX        0
#define BLE_SENSOR_SRV_ADC_INDEX                1
#define BLE_SENSOR_SRV_POWER_INDEX              2
#define BLE_SENSOR_SRV_ACCEL_INDEX              3
#define BLE_SENSOR_SRV_GYRO_INDEX               4
#define BLE_SENSOR_SRV_ORIENT_INDEX             5
#define BLE_SENSOR_SRV_CADENCE_INDEX            6
#define BLE_SENSOR_SRV_TEMP_INDEX               7

typedef void (*zero_offset_request_cb)(void);

extern ret_code_t sensor_srv_init();
extern void sensor_srv_reg_zero_offset_cb(zero_offset_request_cb callback);
extern void sensor_srv_update_adc(uint32_t value);
extern void sensor_srv_update_power(uint16_t value);
extern void sensor_srv_update_accel(float* values);
extern void sensor_srv_update_gyro(float* values);
extern void sensor_srv_update_orient(float* values);
extern void sensor_srv_update_cadence(uint16_t value);
extern void sensor_srv_update_temp(float value);

#endif // SENSOR_BLE_SRV_H__