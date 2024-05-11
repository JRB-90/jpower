#include "calibrate.h"

#include "storage_helper.h"
#include "nrf_log.h"
#include "fds.h"

static bool is_calibrated = false;
static calibration_data_t calibration_data;

ret_code_t calibrate_init()
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
        is_calibrated = true;
        NRF_LOG_INFO("Calibration data loaded");
    }
    else
    {
        NRF_LOG_INFO("No calibration data found");
    }

    return NRF_SUCCESS;
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

    fds_record_t record =
    {
        .file_id = CAL_FILE_ID,
        .key = CAL_RECORD_KEY,
        .data.p_data = &calibration,
        .data.length_words = (sizeof(calibration) + 3) / sizeof(uint32_t),
    };

    return storage_write(&record);
}