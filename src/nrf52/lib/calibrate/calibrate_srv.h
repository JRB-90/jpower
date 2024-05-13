#ifndef CALIBRATE_SRV_H__
#define CALIBRATE_SRV_H__

#include <stdint.h>
#include "app_error.h"
#include "ble_subsystem.h"

// 128-bit base UUID
#define BLE_UUID_CALIBRATE_SRV_BASE_UUID        { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_UUID_CALIBRATE_SRV_SERVICE          0xABCD
#define BLE_UUID_ADC_RAW_CHARACTERISTC_UUID     0xBEEF

typedef struct calibrate_srv_t
{
    uint16_t conn_handle;
    uint16_t service_handle;
    ble_gatts_char_handles_t char_handles;
} calibrate_srv_t;

extern void calibrate_srv_init(calibrate_srv_t* service);
extern void calibrate_srv_update_raw_adc(
    calibrate_srv_t* service, 
    int32_t* value
);

#endif // CALIBRATE_SRV_H__