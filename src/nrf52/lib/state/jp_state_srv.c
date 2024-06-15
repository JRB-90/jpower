#include "jp_state_srv.h"

#include <stdint.h>
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "nrf_sdh_ble.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_gatts.h"
#include "ble_subsystem.h"

static void on_connect();
static void on_disconnect();
static void on_state_request_write(uint8_t* data);

static ble_uuid128_t base_uuid = { JP_STATE_SRV_BASE_UUID };

static ble_uuid_t srv_uuid =
{
    .uuid = JP_STATE_SRV_SERVICE_UUID,
    .type = BLE_UUID_TYPE_UNKNOWN,
};

static ble_srv_desc_t jp_state_service_desc =
{
    .srv_description = "Calibrate Service",
    .srv_uuid = JP_STATE_SRV_SERVICE_UUID,
    .srv_char_count = JP_STATE_SRV_CHAR_COUNT,
    .on_connect = &on_connect,
    .on_disconnect = &on_disconnect,
};

static ble_srv_dyn_desc_t jp_state_service =
{
    .service_descriptor = &jp_state_service_desc,
    .uuid = &srv_uuid,
    .service_handler = 0,
    .conn_handle = 0,
};

static ble_srv_char_desc_t state_req_char_desc =
{
    .char_description = "State Service Request",
    .char_uuid = JP_STATE_SRV_STATE_REQ_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE),
    .char_data_len = sizeof(jp_state_request_t),
    .char_data_init = { JP_STATE_REQUEST_NONE },
    .on_read = NULL,
    .on_write = on_state_request_write,
};

static ble_srv_char_desc_t current_state_char_desc =
{
    .char_description = "Current State",
    .char_uuid = JP_STATE_SRV_CURRENT_STATE_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(jp_state_t),
    .char_data_init = { JP_STATE_STARTUP },
    .on_read = NULL,
    .on_write = NULL,
};

NRF_SDH_BLE_OBSERVER(
    jp_state_srv_observer,
    APP_BLE_OBSERVER_PRIO,
    ble_srv_on_event, 
    &jp_state_service
);

static jp_state_request_cb on_state_request = NULL;

ret_code_t jp_state_srv_init()
{
    ret_code_t err_code;

    jp_state_service.chars_handler[JP_STATE_SRV_STATE_REQ_INDEX].char_descriptor = &state_req_char_desc;
    jp_state_service_desc.srv_chars[JP_STATE_SRV_STATE_REQ_INDEX] = state_req_char_desc;

    jp_state_service.chars_handler[JP_STATE_SRV_CURRENT_STATE_INDEX].char_descriptor = &current_state_char_desc;
    jp_state_service_desc.srv_chars[JP_STATE_SRV_CURRENT_STATE_INDEX] = current_state_char_desc;

    err_code =
        sd_ble_uuid_vs_add(
            &base_uuid,
            &jp_state_service.uuid->type
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        ble_srv_init_dyn_service(
            &jp_state_service
        );
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

void jp_state_srv_register_enter_state_cb(jp_state_request_cb callback)
{
    on_state_request = callback;
}

ret_code_t jp_state_srv_update_current_state(jp_state_t state)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &jp_state_service,
            &jp_state_service.chars_handler[JP_STATE_SRV_CURRENT_STATE_INDEX],
            (uint8_t*)&state
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

static void on_state_request_write(uint8_t* data)
{
    jp_state_request_t state_request;
    memcpy(&state_request, data, sizeof(jp_state_request_t));

    if (on_state_request == NULL)
    {
        return;        
    }

    if (state_request == JP_STATE_REQUEST_NONE)
    {
        return;
    }
    else if (state_request == JP_STATE_REQUEST_PUBLISH_STATE)
    {
        jp_state_srv_update_current_state(jp_state_get_current_state());
        return;
    }
    else if (state_request == JP_STATE_REQUEST_SWITCH_TO_RUNNING ||
             state_request == JP_STATE_REQUEST_SWITCH_TO_CALIBRATE)
    {
        on_state_request(state_request);
        return;
    }
}