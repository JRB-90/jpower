#include "calibrate.h"

#include "storage_helper.h"
#include "nrf_log.h"
#include "fds.h"
#include "ble_subsystem.h"

static bool is_calibrated = false;
static bool is_cal_write_pending = false;
static calibration_data_t calibration_data = { 0 };
static uint32_t adc_value = 0;
static on_adc_value_updated_t adc_value_updated_cb = NULL;

static ret_code_t read_calibration_data_from_disk();
static ret_code_t write_calibration_data_to_disk();

ret_code_t calibrate_init()
{
    ret_code_t err_code = read_calibration_data_from_disk();

    if (err_code == NRF_SUCCESS)
    {
        is_calibrated = true;
        NRF_LOG_INFO("Calibration data loaded");
    }
    else
    {
        is_calibrated = false;
        NRF_LOG_INFO("No calibration data found");
    }

    return NRF_SUCCESS;
}

void calibrate_register_adc_value_updated_cb(on_adc_value_updated_t callback)
{
    adc_value_updated_cb = callback;
}

void calibrate_update()
{    
    ret_code_t err_code;

    if (is_cal_write_pending)
    {
        err_code = write_calibration_data_to_disk();

        if (err_code == NRF_SUCCESS)
        {
            NRF_LOG_INFO("Calibration data saved");
        }
        else
        {
            NRF_LOG_ERROR("Calibration data failed to save");
        }

        is_cal_write_pending = false;
    }

    if (adc_value_updated_cb != NULL)
    {
        adc_value_updated_cb(adc_value);
    }
    adc_value++;
}

bool calibrate_get_is_calibrated()
{
    return is_calibrated;
}

calibration_data_t calibrate_get_calibration()
{
    return calibration_data;
}

ret_code_t calibrate_set_calibration(const calibration_data_t calibration)
{
    calibration_data = calibration;

    return write_calibration_data_to_disk();
}

ret_code_t calibrate_set_calibration_async(const calibration_data_t calibration)
{
    calibration_data = calibration;
    is_cal_write_pending = true;

    return NRF_SUCCESS;
}

ret_code_t calibrate_zero_offset()
{
    adc_value = 0;

    return NRF_SUCCESS;
}

static ret_code_t read_calibration_data_from_disk()
{
    fds_flash_record_t read_record;

    ret_code_t err_code =
        storage_read(
            CAL_FILE_ID,
            CAL_RECORD_KEY,
            &read_record
        );

    if (err_code == NRF_SUCCESS)
    {
        memcpy(&calibration_data, read_record.p_data, sizeof(calibration_data_t));
    }

    return err_code;
}

static ret_code_t write_calibration_data_to_disk()
{
    fds_record_t record =
    {
        .file_id = CAL_FILE_ID,
        .key = CAL_RECORD_KEY,
        .data.p_data = &calibration_data,
        .data.length_words = (sizeof(calibration_data_t) + 3) / sizeof(uint32_t),
    };

    return storage_write_async(&record);
}
