#include "calibrate_srv.h"

#include <stdbool.h>
#include <string.h>
#include "nrf_sdh_ble.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_gatts.h"
#include "ble_subsystem.h"

static void on_connect();
static void on_disconnect();

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

ret_code_t calibrate_srv_update_raw_adc(int32_t value)
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

static void on_connect()
{
}

static void on_disconnect()
{
}
