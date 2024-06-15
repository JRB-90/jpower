#include "calibrate_srv.h"

#include <stdbool.h>
#include <string.h>
#include "nrf_log.h"
#include "nrf_sdh_ble.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_gatts.h"
#include "ble_subsystem.h"
#include "jp_state.h"

static void on_connect();
static void on_disconnect();
static void on_zero_requested(uint8_t* data);
static void on_cal_pull_requested(uint8_t* data);
static void on_new_cal_pushed(uint8_t* data);
static void on_adc_value_updated(uint32_t value);

static ble_uuid128_t base_uuid = { BLE_CAL_SRV_BASE_UUID };

static ble_uuid_t srv_uuid =
{
    .uuid = BLE_CAL_SRV_SERVICE_UUID,
    .type = BLE_UUID_TYPE_UNKNOWN,
};

static ble_srv_desc_t cal_service_desc =
{
    .srv_description = "Calibrate Service",
    .srv_uuid = BLE_CAL_SRV_SERVICE_UUID,
    .srv_char_count = BLE_CAL_SRV_CHAR_COUNT,
    .on_connect = &on_connect,
    .on_disconnect = &on_disconnect,
};

static ble_srv_dyn_desc_t cal_service =
{
    .service_descriptor = &cal_service_desc,
    .uuid = &srv_uuid,
    .service_handler = 0,
    .conn_handle = 0,
};

static ble_srv_char_desc_t adc_char_desc =
{
    .char_description = "Raw ADC Stream",
    .char_uuid = BLE_CAL_SRV_ADC_RAW_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint32_t),
    .char_data_init = { 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t zero_char_desc =
{
    .char_description = "Zero ADC Offset",
    .char_uuid = BLE_CAL_SRV_ZERO_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint8_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = on_zero_requested,
};

static ble_srv_char_desc_t pull_cal_req_char_desc =
{
    .char_description = "Pull Cal Request",
    .char_uuid = BLE_CAL_SRV_PULL_CAL_REQ_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint8_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = on_cal_pull_requested,
};

static ble_srv_char_desc_t pull_cal_char_desc =
{
    .char_description = "Pull Cal",
    .char_uuid = BLE_CAL_SRV_PULL_CAL_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(calibration_data_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t push_cal_char_desc =
{
    .char_description = "Push Cal",
    .char_uuid = BLE_CAL_SRV_PUSH_CAL_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(calibration_data_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = on_new_cal_pushed,
};

NRF_SDH_BLE_OBSERVER(
    calibrate_srv_observer,
    APP_BLE_OBSERVER_PRIO,
    ble_srv_on_event, 
    &cal_service
);

ret_code_t calibrate_srv_init()
{
    ret_code_t err_code;

    cal_service.chars_handler[BLE_CAL_SRV_ADC_INDEX].char_descriptor = &adc_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_ADC_INDEX] = adc_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_ZERO_INDEX].char_descriptor = &zero_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_ZERO_INDEX] = zero_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_PULL_CAL_REQ_INDEX].char_descriptor = &pull_cal_req_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_PULL_CAL_REQ_INDEX] = pull_cal_req_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_PULL_CAL_INDEX].char_descriptor = &pull_cal_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_PULL_CAL_INDEX] = pull_cal_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_PUSH_CAL_INDEX].char_descriptor = &push_cal_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_PUSH_CAL_INDEX] = push_cal_char_desc;

    err_code =
        sd_ble_uuid_vs_add(
            &base_uuid,
            &cal_service.uuid->type
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        ble_srv_init_dyn_service(
            &cal_service
        );
    APP_ERROR_CHECK(err_code);

    calibrate_register_adc_value_updated_cb(on_adc_value_updated);

    return NRF_SUCCESS;
}

ret_code_t calibrate_srv_update_raw_adc(uint32_t value)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &cal_service,
            &cal_service.chars_handler[BLE_CAL_SRV_ADC_INDEX],
            (uint8_t*)&value
        );
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

ret_code_t calibrate_srv_send_cal(calibration_data_t calibration)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &cal_service,
            &cal_service.chars_handler[BLE_CAL_SRV_PULL_CAL_INDEX],
            (uint8_t*)&calibration
        );
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

static void on_connect()
{
}

static void on_disconnect()
{
}

static void on_zero_requested(uint8_t* data)
{
    calibrate_zero_offset();
    NRF_LOG_INFO("Zero");
}

static void on_cal_pull_requested(uint8_t* data)
{
    NRF_LOG_INFO("Pull requested");
    calibration_data_t current_cal = calibrate_get_calibration();
    ret_code_t err_code = calibrate_srv_send_cal(current_cal);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Pull sent");
    NRF_LOG_HEXDUMP_INFO(current_cal.cal_id.guid128, 16);
    NRF_LOG_INFO(
        "Slope: %d, Intercept: %d", 
        current_cal.slope, 
        current_cal.intercept
    );
}

static void on_new_cal_pushed(uint8_t* data)
{
    NRF_LOG_INFO("Push requested");
    jp_state_t current_state = jp_state_get_current_state();

    if (current_state != JP_STATE_CALIBRATING)
    {
        NRF_LOG_ERROR("Cannot push cal when not in cal state");
        return;
    }

    calibration_data_t calibration = {0};
    memcpy(&calibration, data, sizeof(calibration_data_t));

    // I can't call this here as it uses a write to storage and
    // thats a task that can't be done in a ble callback like this
    //calibrate_set_calibration(calibration);

    // So instead, we add it to the calibrate task queue to write
    // on the next update
    calibrate_set_calibration_async(calibration);

    NRF_LOG_INFO("Pushed");
    NRF_LOG_HEXDUMP_INFO(calibration.cal_id.guid128, 16);
    NRF_LOG_INFO(
        "Slope: %d, Intercept: %d", 
        calibration.slope, 
        calibration.intercept
    );
}

static void on_adc_value_updated(uint32_t value)
{
    ret_code_t err_code = calibrate_srv_update_raw_adc(value);
    APP_ERROR_CHECK(err_code);
}
