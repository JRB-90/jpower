#ifndef CALIBRATE_BLE_SRV_H__
#define CALIBRATE_BLE_SRV_H__

#include <stdint.h>
#include "app_error.h"
#include "strain.h"

// 128-bit base UUID
#define BLE_CAL_SRV_BASE_UUID                   { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define BLE_CAL_SRV_SERVICE_UUID                0xC000
#define BLE_CAL_SRV_SEND_REQ_UUID               0xC001
#define BLE_CAL_SRV_PUSH_CAL_CHAR_UUID          0xCC02
#define BLE_CAL_SRV_PULL_CAL_RES_CHAR_UUID      0xCC03
#define BLE_CAL_SRV_MEASURE_RES_CHAR_UUID       0xCC04
#define BLE_CAL_SRV_ZERO_RES_CHAR_UUID          0xCC05

#define BLE_CAL_SRV_CHAR_COUNT                  5
#define BLE_CAL_SRV_SEND_REQ_INDEX              0
#define BLE_CAL_SRV_PUSH_CAL_INDEX              1
#define BLE_CAL_SRV_PULL_CAL_RES_INDEX          2
#define BLE_CAL_SRV_MEASURE_RES_INDEX           3
#define BLE_CAL_SRV_ZERO_RES_INDEX              4

typedef void (*pull_cal_cb)(calibration_data_t *const calibration);
typedef void (*cal_pushed_cb)(calibration_data_t *const calibration);
typedef uint32_t (*measure_cb)(uint8_t);
typedef void (*zero_offset_cb)(void);

extern ret_code_t calibrate_srv_init();
extern void calibrate_reg_cal_pushed_cb(cal_pushed_cb callback);
extern void calibrate_reg_pull_cal_cb(pull_cal_cb callback);
extern void calibrate_reg_measure_cb(measure_cb callback);
extern void calibrate_reg_zero_offset_cb(zero_offset_cb callback);

#endif // CALIBRATE_BLE_SRV_H__