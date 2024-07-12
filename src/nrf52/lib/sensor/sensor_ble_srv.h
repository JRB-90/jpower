#ifndef SENSOR_BLE_SRV_H__
#define SENSOR_BLE_SRV_H__

#include <stdint.h>
#include "app_error.h"
#include "imu_types.h"

// 128-bit base UUID
#define BLE_SENSOR_SRV_BASE_UUID                { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_SENSOR_SRV_SERVICE_UUID             0xA000
#define BLE_SENSOR_SRV_DIAG_DATA_CHAR_UUID      0xAA01

#define BLE_SENSOR_SRV_CHAR_COUNT               1
#define BLE_SENSOR_SRV_DIAG_DATA_INDEX          0

typedef struct
{
    float temp_c;
    uint32_t adc_value;
    float force_n;
    float torque_nm;
    imu_reading_t imu_data;
    FusionQuaternion orient;
    uint16_t cadence_rpm;
    uint16_t power_w;    
} sensor_diag_data_t;

extern ret_code_t sensor_srv_init();
extern void sensor_srv_update_diag_data(const sensor_diag_data_t *const diag_data);

#endif // SENSOR_BLE_SRV_H__