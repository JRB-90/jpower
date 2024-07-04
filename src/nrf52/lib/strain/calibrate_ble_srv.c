#include "calibrate_ble_srv.h"

#include <stdbool.h>
#include <string.h>
#include "nrf_log.h"
#include "nrf_sdh_ble.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_gatts.h"
#include "ble_subsystem.h"
#include "ble_subsystem_settings.h"

typedef enum
{
    CALIBRATE_SRV_PULL_CAL      = 0x01,
    CALIBRATE_SRV_ZERO_OFFSET   = 0x02,
    CALIBRATE_SRV_MEASURE       = 0x03,
} calibrate_srv_request_t;

typedef enum
{
    CALIBRATE_SRV_OK            = 0x01,
    CALIBRATE_SRV_NOK           = 0x02,
} calibrate_srv_response_t;

static void on_connect();
static void on_disconnect();
static void on_req_received(uint8_t* data);
static void on_cal_pushed(uint8_t* data);

static pull_cal_cb pull_calibration_callback = NULL;
static cal_pushed_cb cal_pushed_callback = NULL;
static measure_cb measure_callback = NULL;
static zero_offset_cb zero_offset_callback = NULL;

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

static ble_srv_char_desc_t request_cal_char_desc =
{
    .char_description = "Send Cal service Request",
    .char_uuid = BLE_CAL_SRV_SEND_REQ_UUID,
    .char_access_rights = (BLE_SRV_WRITE | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(calibrate_srv_request_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = &on_req_received,
};

static ble_srv_char_desc_t push_cal_char_desc =
{
    .char_description = "Push Calibration",
    .char_uuid = BLE_CAL_SRV_PUSH_CAL_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(calibration_data_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = &on_cal_pushed,
};

static ble_srv_char_desc_t pull_cal_res_char_desc =
{
    .char_description = "Pull Calibration Result",
    .char_uuid = BLE_CAL_SRV_PULL_CAL_RES_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(calibration_data_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t measure_char_desc =
{
    .char_description = "Calibration Measurement Result",
    .char_uuid = BLE_CAL_SRV_MEASURE_RES_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint32_t),
    .char_data_init = { 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t zero_char_desc =
{
    .char_description = "Zero ADC Offset Result",
    .char_uuid = BLE_CAL_SRV_ZERO_RES_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(calibrate_srv_response_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = NULL,
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

    cal_service.chars_handler[BLE_CAL_SRV_SEND_REQ_INDEX].char_descriptor = &request_cal_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_SEND_REQ_INDEX] = request_cal_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_PUSH_CAL_INDEX].char_descriptor = &push_cal_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_PUSH_CAL_INDEX] = push_cal_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_PULL_CAL_RES_INDEX].char_descriptor = &pull_cal_res_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_PULL_CAL_RES_INDEX] = pull_cal_res_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_MEASURE_RES_INDEX].char_descriptor = &measure_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_MEASURE_RES_INDEX] = measure_char_desc;

    cal_service.chars_handler[BLE_CAL_SRV_ZERO_RES_INDEX].char_descriptor = &zero_char_desc;
    cal_service_desc.srv_chars[BLE_CAL_SRV_ZERO_RES_INDEX] = zero_char_desc;

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

    return NRF_SUCCESS;
}

void calibrate_reg_pull_cal_cb(pull_cal_cb callback)
{
    pull_calibration_callback = callback;
}

void calibrate_reg_cal_pushed_cb(cal_pushed_cb callback)
{
    cal_pushed_callback = callback;
}

void calibrate_reg_measure_cb(measure_cb callback)
{
    measure_callback = callback;
}

void calibrate_reg_zero_offset_cb(zero_offset_cb callback)
{
    zero_offset_callback = callback;
}

static void on_connect()
{
}

static void on_disconnect()
{
}

static void on_cal_pushed(uint8_t* data)
{
    if (cal_pushed_callback == NULL)
    {
        return;
    }

    calibration_data_t cal_data = { 0 };
    memcpy(&cal_data, data, sizeof(calibration_data_t));
    cal_pushed_callback(&cal_data);

    NRF_LOG_INFO("Cal Pushed");
}

static void on_req_received(uint8_t* data)
{
    NRF_LOG_INFO("Request Received");
    uint8_t request = data[0];
    ret_code_t err_code;
    
    switch (request)
    {
        case CALIBRATE_SRV_PULL_CAL:
            if (pull_calibration_callback == NULL)
            {
                return;
            }
            calibration_data_t cal_to_pull;
            pull_calibration_callback(&cal_to_pull);
            err_code =
                ble_srv_update_dyn_char(
                    &cal_service,
                    &cal_service.chars_handler[BLE_CAL_SRV_PULL_CAL_RES_INDEX],
                    (uint8_t*)&cal_to_pull
                );
            APP_ERROR_CHECK(err_code);
            NRF_LOG_INFO("Cal Pulled");
            break;

        case CALIBRATE_SRV_MEASURE:
            if (measure_callback == NULL)
            {
                return;
            }
            uint32_t meas_result = measure_callback(10);
            err_code =
                ble_srv_update_dyn_char(
                    &cal_service,
                    &cal_service.chars_handler[BLE_CAL_SRV_MEASURE_RES_INDEX],
                    (uint8_t*)&meas_result
                );
            APP_ERROR_CHECK(err_code);
            NRF_LOG_INFO("Measure Requested");
            break;

        case CALIBRATE_SRV_ZERO_OFFSET:
            if (zero_offset_callback == NULL)
            {
                return;
            }
            zero_offset_callback();
            calibrate_srv_response_t zero_res = CALIBRATE_SRV_OK;
            err_code =
                ble_srv_update_dyn_char(
                    &cal_service,
                    &cal_service.chars_handler[BLE_CAL_SRV_ZERO_RES_INDEX],
                    (uint8_t*)&zero_res
                );
            APP_ERROR_CHECK(err_code);
            NRF_LOG_INFO("Zero Requested");
            break;

        default:
            NRF_LOG_ERROR("Cal request not recognised");
            break;
    }
}
