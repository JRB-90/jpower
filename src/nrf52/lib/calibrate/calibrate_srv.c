#include "calibrate_srv.h"

#include <stdbool.h>
#include <string.h>
#include "ble_srv_common.h"
#include "ble_srv_helper.h"

void calibrate_srv_init(calibrate_srv_t* service)
{
    ble_uuid128_t base_uuid = { BLE_UUID_CALIBRATE_SRV_BASE_UUID };

    char_def_t char_def =
    {
        .base_uuid = base_uuid,
        .srv_uuid = BLE_UUID_ADC_RAW_CHARACTERISTC_UUID,
        .srv_handle = service->service_handle,
        .srv_conn_handle = &service->conn_handle,
        .char_handles = &service->char_handles,
        .wr_type = ReadWrite,
        .is_notifying = true
    };

    ble_srv_add_int32_char(&char_def);
}

void calibrate_srv_update_raw_adc(
    calibrate_srv_t* service, 
    int32_t* value)
{
    if (service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
	    uint16_t len = 4;
        ble_gatts_hvx_params_t hvx_params;
    	memset(&hvx_params, 0, sizeof(hvx_params));

		hvx_params.handle = service->char_handles.value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len  = &len;
		hvx_params.p_data = (uint8_t*)value;  

    	sd_ble_gatts_hvx(service->conn_handle, &hvx_params);
	}
}