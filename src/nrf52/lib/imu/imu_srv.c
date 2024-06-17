#include "imu_srv.h"

#include <stdbool.h>
#include <string.h>
#include "nrf_log.h"
#include "nrf_sdh_ble.h"
#include "ble_srv_common.h"
#include "ble_srv_helper.h"
#include "ble_gatts.h"
#include "ble_subsystem.h"

static void on_connect();
static void on_disconnect();
static void on_accel_value_updated(float value);
static void on_gyro_value_updated(float value);

static bool isConnected = false;

static ble_uuid128_t base_uuid = { BLE_IMU_SRV_BASE_UUID };

static ble_uuid_t srv_uuid =
{
    .uuid = BLE_IMU_SRV_SERVICE_UUID,
    .type = BLE_UUID_TYPE_UNKNOWN,
};

static ble_srv_desc_t imu_service_desc =
{
    .srv_description = "Calibrate Service",
    .srv_uuid = BLE_IMU_SRV_SERVICE_UUID,
    .srv_char_count = BLE_IMU_SRV_CHAR_COUNT,
    .on_connect = &on_connect,
    .on_disconnect = &on_disconnect,
};

static ble_srv_dyn_desc_t imu_service =
{
    .service_descriptor = &imu_service_desc,
    .uuid = &srv_uuid,
    .service_handler = 0,
    .conn_handle = 0,
};

static ble_srv_char_desc_t accel_char_desc =
{
    .char_description = "Raw Accel stream",
    .char_uuid = BLE_IMU_SRV_ACCEL_RAW_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(float),
    .char_data_init = { 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

static ble_srv_char_desc_t gyro_char_desc =
{
    .char_description = "Zero Gyro stream",
    .char_uuid = BLE_IMU_SRV_GYRO_RAW_CHAR_UUID,
    .char_access_rights = (BLE_SRV_READ | BLE_SRV_NOTIFY),
    .char_data_len = sizeof(float),
    .char_data_init = { 0x00, 0x00, 0x00, 0x00 },
    .on_read = NULL,
    .on_write = NULL,
};

NRF_SDH_BLE_OBSERVER(
    imu_srv_observer,
    APP_BLE_OBSERVER_PRIO,
    ble_srv_on_event, 
    &imu_service
);

ret_code_t imu_srv_init()
{
    ret_code_t err_code;

    imu_service.chars_handler[BLE_IMU_SRV_ACCEL_RAW_INDEX].char_descriptor = &accel_char_desc;
    imu_service_desc.srv_chars[BLE_IMU_SRV_ACCEL_RAW_INDEX] = accel_char_desc;

    imu_service.chars_handler[BLE_IMU_SRV_GYRO_RAW_INDEX].char_descriptor = &gyro_char_desc;
    imu_service_desc.srv_chars[BLE_IMU_SRV_GYRO_RAW_INDEX] = gyro_char_desc;

    err_code =
        sd_ble_uuid_vs_add(
            &base_uuid,
            &imu_service.uuid->type
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        ble_srv_init_dyn_service(
            &imu_service
        );
    APP_ERROR_CHECK(err_code);

    imu_register_accel_value_updated_cb(on_accel_value_updated);
    imu_register_gyro_value_updated_cb(on_gyro_value_updated);

    return NRF_SUCCESS;
}

ret_code_t imu_srv_update_raw_accel(float value)
{
    if (isConnected)
    {
        ret_code_t err_code = imu_srv_update_raw_accel(value);
        APP_ERROR_CHECK(err_code);
    }

    return NRF_SUCCESS;
}

ret_code_t imu_srv_update_raw_gyro(float value)
{
    if (isConnected)
    {
        ret_code_t err_code = imu_srv_update_raw_accel(value);
        APP_ERROR_CHECK(err_code);
    }

    return NRF_SUCCESS;
}

static void on_connect()
{
    isConnected = true;
}

static void on_disconnect()
{
    isConnected = false;
}

static void on_accel_value_updated(float value)
{
    imu_srv_update_raw_accel(value);
}

static void on_gyro_value_updated(float value)
{
    imu_srv_update_raw_gyro(value);
}
