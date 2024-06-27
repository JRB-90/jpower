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

#define BLE_SENSOR_SRV_CHAR_COUNT               3
#define BLE_SENSOR_SRV_ZERO_OFFSET_INDEX        0
#define BLE_SENSOR_SRV_ADC_INDEX                1
#define BLE_SENSOR_SRV_POWER_INDEX              2

extern ret_code_t sensor_ble_srv_init();

#endif // SENSOR_BLE_SRV_H__