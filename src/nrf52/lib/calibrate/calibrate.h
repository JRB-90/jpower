#ifndef CALIBRATE_H__
#define CALIBRATE_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"

#define CAL_FILE_ID     0x8020
#define CAL_RECORD_KEY  0x7020

typedef struct
{
    uint32_t cal_id;
    float slope;
} calibration_data_t;

extern ret_code_t calibrate_init();
extern void calibrate_update();
extern bool calibrate_get_is_calibrated();
extern calibration_data_t calibrate_get_calibration();
extern ret_code_t calibrate_set_calibration(const calibration_data_t calibration);

#endif // CALIBRATE_H__