#include "sensor_ble_srv.h"

#include <stdbool.h>
#include "nrf_sdh_ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_subsystem_settings.h"

static void on_connect();
static void on_disconnect();
static void on_zero_requested(uint8_t* data);

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

static ble_srv_char_desc_t zero_char_desc =
{
    .char_description = "Zero ADC Offset",
    .char_uuid = BLE_SENSOR_SRV_ZERO_OFFSET_CHAR_UUID,
    .char_access_rights = (BLE_SRV_WRITE | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint8_t),
    .char_data_init = { 0x00 },
    .on_read = NULL,
    .on_write = on_zero_requested,
};

static ble_srv_char_desc_t adc_char_desc =
{
    .char_description = "Raw ADC Value",
    .char_uuid = BLE_SENSOR_SRV_ADC_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint32_t),
    .char_data_init = { 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t power_char_desc =
{
    .char_description = "Power",
    .char_uuid = BLE_SENSOR_SRV_POWER_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint16_t),
    .char_data_init = { 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t accel_char_desc =
{
    .char_description = "Accelerometer",
    .char_uuid = BLE_SENSOR_SRV_ACCEL_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(float) * 3,
    .char_data_init = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t gyro_char_desc =
{
    .char_description = "Gyroscope",
    .char_uuid = BLE_SENSOR_SRV_GYRO_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(float) * 3,
    .char_data_init = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t orient_char_desc =
{
    .char_description = "Orientation",
    .char_uuid = BLE_SENSOR_SRV_ORIENT_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(float) * 4,
    .char_data_init = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t cadence_char_desc =
{
    .char_description = "Cadence",
    .char_uuid = BLE_SENSOR_SRV_CADENCE_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(uint16_t),
    .char_data_init = { 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t temp_char_desc =
{
    .char_description = "Temperature",
    .char_uuid = BLE_SENSOR_SRV_TEMP_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(float),
    .char_data_init = { 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

NRF_SDH_BLE_OBSERVER(
    calibrate_srv_observer,
    APP_BLE_OBSERVER_PRIO,
    ble_srv_on_event, 
    &sensor_service
);

static zero_offset_request_cb zero_offset_cb = NULL;

ret_code_t sensor_srv_init()
{
    ret_code_t err_code;

    sensor_service.chars_handler[BLE_SENSOR_SRV_ZERO_OFFSET_INDEX].char_descriptor = &zero_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_ZERO_OFFSET_INDEX] = zero_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_ADC_INDEX].char_descriptor = &adc_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_ADC_INDEX] = adc_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_POWER_INDEX].char_descriptor = &power_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_POWER_INDEX] = power_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_ACCEL_INDEX].char_descriptor = &accel_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_ACCEL_INDEX] = accel_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_GYRO_INDEX].char_descriptor = &gyro_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_GYRO_INDEX] = gyro_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_ORIENT_INDEX].char_descriptor = &orient_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_ORIENT_INDEX] = orient_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_CADENCE_INDEX].char_descriptor = &cadence_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_CADENCE_INDEX] = cadence_char_desc;

    sensor_service.chars_handler[BLE_SENSOR_SRV_TEMP_INDEX].char_descriptor = &temp_char_desc;
    sensor_service_desc.srv_chars[BLE_SENSOR_SRV_TEMP_INDEX] = temp_char_desc;

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

void sensor_srv_reg_zero_offset_cb(zero_offset_request_cb callback)
{
    zero_offset_cb = callback;
}

void sensor_srv_update_adc(uint32_t value)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_ADC_INDEX],
            (uint8_t*)&value
        );
    APP_ERROR_CHECK(err_code);
}

void sensor_srv_update_power(uint16_t value)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_POWER_INDEX],
            (uint8_t*)&value
        );
    APP_ERROR_CHECK(err_code);
}

void sensor_srv_update_accel(float* values)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_ACCEL_INDEX],
            (uint8_t*)values
        );
    APP_ERROR_CHECK(err_code);
}

void sensor_srv_update_gyro(float* values)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_GYRO_INDEX],
            (uint8_t*)values
        );
    APP_ERROR_CHECK(err_code);
}

void sensor_srv_update_orient(float* values)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_ORIENT_INDEX],
            (uint8_t*)values
        );
    APP_ERROR_CHECK(err_code);
}

void sensor_srv_update_cadence(uint16_t value)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_CADENCE_INDEX],
            (uint8_t*)&value
        );
    APP_ERROR_CHECK(err_code);
}

void sensor_srv_update_temp(float value)
{
    ret_code_t err_code =
        ble_srv_update_dyn_char(
            &sensor_service,
            &sensor_service.chars_handler[BLE_SENSOR_SRV_TEMP_INDEX],
            (uint8_t*)&value
        );
    APP_ERROR_CHECK(err_code);
}

static void on_connect()
{
}

static void on_disconnect()
{
}

static void on_zero_requested(uint8_t* data)
{
    if (zero_offset_cb != NULL)
    {
        zero_offset_cb();
    }
}
