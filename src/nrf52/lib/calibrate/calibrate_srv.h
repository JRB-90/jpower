#ifndef CALIBRATE_SRV_H__
#define CALIBRATE_SRV_H__

#include <stdint.h>
#include "app_error.h"
#include "ble_gatts.h"

// 128-bit base UUID
#define BLE_CAL_SRV_BASE_UUID           { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_CAL_SRV_SERVICE_UUID        0xABCD
#define BLE_CAL_SRV_ADC_RAW_CHAR_UUID   0xBEEF
#define BLE_CAL_SRV_CHAR_COUNT          1
#define BLE_CAL_SRV_ADC_INDEX           0

extern void calibrate_srv_init();
extern void calibrate_srv_update_raw_adc(int32_t value);

#endif // CALIBRATE_SRV_H__