#include "calibrate_srv.h"

void calibrate_srv_init(calibrate_srv_t* service)
{
    ret_code_t err_code;
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = { BLE_UUID_CALIBRATE_SRV_BASE_UUID };
    char_uuid.uuid = BLE_UUID_ADC_RAW_CHARACTERISTC_UUID;
    service->conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code = 
        sd_ble_uuid_vs_add(
            &base_uuid, 
            &char_uuid.type
        );
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md = &cccd_md;
    char_md.char_props.notify = 1;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.max_len = 4;
    attr_char_value.init_len = 4;
    uint8_t value[4] = { 0x12, 0x34, 0x56, 0x78 };
    attr_char_value.p_value = value;

    err_code =
        sd_ble_gatts_characteristic_add(
            service->service_handle,
            &char_md,
            &attr_char_value,
            &service->char_handles
        );
    APP_ERROR_CHECK(err_code);
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