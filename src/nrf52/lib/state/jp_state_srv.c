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
static void on_enter_state_write(uint8_t* data);

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

static ble_srv_char_desc_t get_state_char_desc =
{
    .char_description = "Raw ADC Stream",
    .char_uuid = JP_STATE_SRV_GET_STATE_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ),
    .char_data_len = sizeof(jp_state_t),
    .char_data_init = { JP_STATE_STARTUP },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t enter_state_char_desc =
{
    .char_description = "Enter State Request",
    .char_uuid = JP_STATE_SRV_ENTER_STATE_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE),
    .char_data_len = sizeof(jp_enter_state_request_t),
    .char_data_init = { JP_STATE_ENTER_CALIBRATING },
    .on_read = NULL,
    .on_write = on_enter_state_write,
};

NRF_SDH_BLE_OBSERVER(
    jp_state_srv_observer,
    APP_BLE_OBSERVER_PRIO,
    ble_srv_on_event, 
    &jp_state_service
);

ret_code_t jp_state_srv_init()
{
    ret_code_t err_code;

    jp_state_service.chars_handler[JP_STATE_SRV_GET_STATE_INDEX].char_descriptor = &get_state_char_desc;
    jp_state_service_desc.srv_chars[JP_STATE_SRV_GET_STATE_INDEX] = get_state_char_desc;

    jp_state_service.chars_handler[JP_STATE_SRV_ENTER_STATE_INDEX].char_descriptor = &enter_state_char_desc;
    jp_state_service_desc.srv_chars[JP_STATE_SRV_ENTER_STATE_INDEX] = enter_state_char_desc;

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

ret_code_t jp_state_srv_update_current_state(jp_state_t state)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &jp_state_service,
            &jp_state_service.chars_handler[JP_STATE_SRV_GET_STATE_INDEX],
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

static void on_enter_state_write(uint8_t* data)
{
    jp_enter_state_request_t enter_state_request;
    memcpy(&enter_state_request, data, sizeof(jp_enter_state_request_t));

    NRF_LOG_INFO("Enter state %i requested", (int)enter_state_request);
}