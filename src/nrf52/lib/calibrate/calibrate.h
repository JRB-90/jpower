#ifndef CALIBRATE_H__
#define CALIBRATE_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "jp_utils.h"

#define CAL_FILE_ID     0x8020
#define CAL_RECORD_KEY  0x7020

typedef struct
{
    guid_t cal_id;
    float slope;
    float intercept;
} calibration_data_t;

typedef void (*on_adc_value_updated_t)(uint32_t value);

extern ret_code_t calibrate_init();
extern void calibrate_register_adc_value_updated_cb(on_adc_value_updated_t callback);
extern void calibrate_update();
extern bool calibrate_get_is_calibrated();
extern calibration_data_t calibrate_get_calibration();
extern ret_code_t calibrate_set_calibration(const calibration_data_t calibration);
extern ret_code_t calibrate_set_calibration_async(const calibration_data_t calibration);
extern ret_code_t calibrate_zero_offset();

#endif // CALIBRATE_H__