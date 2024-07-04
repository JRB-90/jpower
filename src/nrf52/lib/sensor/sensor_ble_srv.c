#include "sensor_ble_srv.h"

#include <stdbool.h>
#include "nrf_sdh_ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_subsystem_settings.h"

static void on_connect();
static void on_disconnect();

static ble_uuid128_t base_uuid = { BLE_SENSOR_SRV_BASE_UUID };

static ble_uuid_t srv_uuid =
{
    .uuid = BLE_SENSOR_SRV_SERVICE_UUID,
    .type = BLE_UUID_TYPE_UNKNOWN,
};

static ble_srv_desc_t sensor_service_desc =
{
    .srv_description = "Sensor Service",
    .srv_uuid = BLE_SENSOR_SRV_SERVICE_UUID,
    .srv_char_count = BLE_SENSOR_SRV_CHAR_COUNT,
    .on_connect = &on_connect,
    .on_disconnect = &on_disconnect,
};

static ble_srv_dyn_desc_t sensor_service =
{
    .service_descriptor = &sensor_service_desc,
    .uuid = &srv_uuid,
    .service_handler = 0,
    .conn_handle = 0,
};

static ble_srv_char_desc_t diag_data_char_desc =
{
    .char_description = "Sensor Diagnostics",
    .char_uuid = BLE_SENSOR_SRV_DIAG_DATA_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(sensor_diag_data_t),
    .char_data_init = { 0 },
    .on_read = NULL,
    .on_write = NULL,
};

NRF_SDH_BLE_OBSERVER(
    calibrate_srv_observer,
    APP_BLE_OBSERVER_PRIO,
    ble_srv_on_event, 
    &sensor_service
);

ret_code_t sensor_srv_init()
{
    ret_code_t err_code;

    sensor_service.chars_handler[BLE_SENSOR_SRV_DIAG_DATA_INDEX].char_descriptor = &diag_data_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_DIAG_DATA_INDEX] = diag_data_char_desc;

    err_code =
        sd_ble_uuid_vs_add(
            &base_uuid,
            &sensor_service.uuid->type
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        ble_srv_init_dyn_service(
            &sensor_service
        );
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

void sensor_srv_update_diag_data(const sensor_diag_data_t *const diag_data)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_DIAG_DATA_INDEX],
            (uint8_t*)diag_data
        );
    APP_ERROR_CHECK(err_code);
}

static void on_connect()
{
}

static void on_disconnect()
{
}
