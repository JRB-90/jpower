#include "calibrate.h"

#include "storage_helper.h"
#include "nrf_log.h"
#include "fds.h"
#include "ble_subsystem.h"
#include "calibrate_srv.h"

static bool is_calibrated = false;
static calibration_data_t calibration_data;
static uint32_t adc_value = 0;

// static void calibrate_srv_on_event(
//     ble_evt_t const* ble_evt, 
//     void* context
// );

ret_code_t calibrate_init()
{
    calibrate_srv_init();

    // NRF_SDH_BLE_OBSERVER(
    //     calibrate_srv_observer,
    //     APP_BLE_OBSERVER_PRIO, 
    //     calibrate_srv_on_event, 
    //     (void*)&calibrate_srv
    // );

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

void calibrate_update()
{
    calibrate_srv_update_raw_adc(adc_value);
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

    fds_record_t record =
    {
        .file_id = CAL_FILE_ID,
        .key = CAL_RECORD_KEY,
        .data.p_data = &calibration,
        .data.length_words = (sizeof(calibration) + 3) / sizeof(uint32_t),
    };

    return storage_write(&record);
}

// static void calibrate_srv_on_event(
//     ble_evt_t const* ble_evt, 
//     void* context)
// {
//     calibrate_srv_t* service = (calibrate_srv_t*) context;
	
//     switch (ble_evt->header.evt_id)
//     {
//         case BLE_GAP_EVT_CONNECTED:
//             service->conn_handle = ble_evt->evt.gap_evt.conn_handle;
//             break;
//         case BLE_GAP_EVT_DISCONNECTED:
//             service->conn_handle = BLE_CONN_HANDLE_INVALID;
//             break;
//         default:
//             // No implementation needed.
//             break;
//     }
// }
