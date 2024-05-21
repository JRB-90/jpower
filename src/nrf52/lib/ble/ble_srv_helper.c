#include "ble_srv_helper.h"

#include <string.h>
#include "nrf_sdh_ble.h"

ret_code_t ble_srv_init_dyn_service(ble_srv_dyn_desc_t* srv_desc)
{
    ret_code_t err_code;

    srv_desc->conn_handle = BLE_CONN_HANDLE_INVALID;

    err_code =
        sd_ble_gatts_service_add(
            BLE_GATTS_SRVC_TYPE_PRIMARY,
            srv_desc->uuid,
            &srv_desc->service_handler
        );
    APP_ERROR_CHECK(err_code);

    for (int i = 0; i < srv_desc->service_descriptor->srv_char_count; i++)
    {
        ble_srv_dyn_char_desc_t* dyn_char_desc = &srv_desc->chars_handler[i];
        const ble_srv_char_desc_t* char_desc = dyn_char_desc->char_descriptor;

        if (char_desc->char_access_rights & BLE_SRV_NOTIFY)
        {
            BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dyn_char_desc->char_cccd.read_perm);
            BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dyn_char_desc->char_cccd.write_perm);
            dyn_char_desc->char_cccd.vloc = BLE_GATTS_VLOC_STACK;
            dyn_char_desc->char_md.char_props.notify = 1;
            dyn_char_desc->char_md.p_cccd_md = &dyn_char_desc->char_cccd;
        }
        else
        {
            dyn_char_desc->char_md.char_props.notify = 0;
            dyn_char_desc->char_md.p_cccd_md = NULL;
        }

        if (char_desc->char_access_rights & BLE_SRV_READ)
        {
            dyn_char_desc->char_md.char_props.read = 1;
            BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dyn_char_desc->char_attr.read_perm);
        }
        else
        {
            BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dyn_char_desc->char_attr.read_perm);
        }

        if (char_desc->char_access_rights & BLE_SRV_WRITE)
        {
            dyn_char_desc->char_md.char_props.write = 1;
            BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dyn_char_desc->char_attr.write_perm);
        }
        else
        {
            BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dyn_char_desc->char_attr.write_perm);
        }

        dyn_char_desc->char_md.p_char_user_desc = NULL;
        dyn_char_desc->char_md.p_char_pf = NULL;
        dyn_char_desc->char_md.p_user_desc_md = NULL;
        dyn_char_desc->char_md.p_sccd_md = NULL;

        dyn_char_desc->char_attr.vloc = BLE_GATTS_VLOC_STACK;
        dyn_char_desc->char_attr.rd_auth = 0;
        dyn_char_desc->char_attr.wr_auth = 0;
        dyn_char_desc->char_attr.vlen = 0;

        dyn_char_desc->char_attr.rd_auth = (char_desc->on_read != NULL) ? 1 : 0;
        dyn_char_desc->char_attr.wr_auth = 0;

        BLE_UUID_BLE_ASSIGN(
            dyn_char_desc->char_uuid,
            dyn_char_desc->char_descriptor->char_uuid
        );

        dyn_char_desc->char_value.p_uuid = &dyn_char_desc->char_uuid;
        dyn_char_desc->char_value.p_attr_md = &dyn_char_desc->char_attr;
        dyn_char_desc->char_value.init_len = dyn_char_desc->char_descriptor->char_data_len;
        dyn_char_desc->char_value.init_offs = 0;
        dyn_char_desc->char_value.max_len = dyn_char_desc->char_descriptor->char_data_len;

        memcpy(
            dyn_char_desc->char_data,
            dyn_char_desc->char_descriptor->char_data_init,
            dyn_char_desc->char_descriptor->char_data_len
        );
        dyn_char_desc->char_value.p_value = dyn_char_desc->char_data;

        err_code = 
            sd_ble_gatts_characteristic_add(
                srv_desc->service_handler,
                &dyn_char_desc->char_md,
                &dyn_char_desc->char_value,
                &dyn_char_desc->char_handler
            ); 
		APP_ERROR_CHECK(err_code);
    }

    return NRF_SUCCESS;
}

ret_code_t ble_srv_update_dyn_char(
    ble_srv_dyn_desc_t* dyn_srv_desc,
    ble_srv_dyn_char_desc_t* dyn_char_desc,
    uint8_t* value)
{
    ret_code_t err_code;

    const ble_srv_char_desc_t* char_desc = dyn_char_desc->char_descriptor;
    memcpy(dyn_char_desc->char_data, value, char_desc->char_data_len);

    ble_gatts_value_t gatts_value;
    memset(&gatts_value, 0, sizeof(ble_gatts_value_t));
    gatts_value.len = char_desc->char_data_len;
    gatts_value.offset = 0;
    gatts_value.p_value = dyn_char_desc->char_data;

    err_code =
        sd_ble_gatts_value_set(
            dyn_srv_desc->conn_handle,
            dyn_char_desc->char_handler.value_handle,
            &gatts_value
        );
    APP_ERROR_CHECK(err_code);

    if (dyn_srv_desc->conn_handle != BLE_CONN_HANDLE_INVALID &&
        char_desc->char_access_rights & BLE_SRV_NOTIFY)
	{
        ble_gatts_hvx_params_t hvx_params;
    	memset(&hvx_params, 0, sizeof(ble_gatts_hvx_params_t));

		hvx_params.handle = dyn_char_desc->char_handler.value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.p_len  = &gatts_value.len;
		hvx_params.offset = gatts_value.offset;
		hvx_params.p_data = gatts_value.p_value;

    	err_code = 
            sd_ble_gatts_hvx(
                dyn_srv_desc->conn_handle,
                &hvx_params
            );
        
        // Invalid state can sometimes fire if this notification
        // happens at the wrong time, so ignore this for now
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
            APP_ERROR_CHECK(err_code);
        }
	}

    return NRF_SUCCESS;
}

void ble_srv_on_event(
    ble_evt_t const* ble_evt, 
    void* context)
{
    ret_code_t err_code;
    ble_srv_dyn_desc_t* service = (ble_srv_dyn_desc_t*)context;
	
    switch (ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            service->conn_handle = ble_evt->evt.gap_evt.conn_handle;
            err_code = 
                sd_ble_gatts_sys_attr_set(
                    service->conn_handle,
                    NULL,
                    0,
                    0
                );
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            service->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        default:
            // No implementation needed.
            break;
    }
}
