#ifndef CALIBRATE_SRV_H__
#define CALIBRATE_SRV_H__

#include <stdint.h>
#include "app_error.h"
#include "calibrate.h"

// 128-bit base UUID
#define BLE_CAL_SRV_BASE_UUID               { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_CAL_SRV_SERVICE_UUID            0xA000
#define BLE_CAL_SRV_ADC_RAW_CHAR_UUID       0xAA01
#define BLE_CAL_SRV_ZERO_CHAR_UUID          0xAA02
#define BLE_CAL_SRV_PULL_CAL_REQ_CHAR_UUID  0xAA03
#define BLE_CAL_SRV_PULL_CAL_CHAR_UUID      0xAA04
#define BLE_CAL_SRV_PUSH_CAL_CHAR_UUID      0xAA05
#define BLE_CAL_SRV_CHAR_COUNT              5
#define BLE_CAL_SRV_ADC_INDEX               0
#define BLE_CAL_SRV_ZERO_INDEX              1
#define BLE_CAL_SRV_PULL_CAL_REQ_INDEX      2
#define BLE_CAL_SRV_PULL_CAL_INDEX          3
#define BLE_CAL_SRV_PUSH_CAL_INDEX          4

extern ret_code_t calibrate_srv_init();
extern ret_code_t calibrate_srv_update_raw_adc(uint32_t value);
extern ret_code_t calibrate_srv_send_cal(calibration_data_t calibration);

#endif // CALIBRATE_SRV_H__