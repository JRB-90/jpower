#include "ble_srv_helper.h"

#include <string.h>
#include "app_error.h"

static void add_char(
    char_def_t* char_definition,
    uint8_t length,
    uint8_t* initial_value
);

void ble_srv_add_int8_char(char_def_t* char_definition)
{
    uint8_t value[1] = { 0x0 };

    add_char(
        char_definition,
        1,
        value
    );
}

void ble_srv_add_uint8_char(char_def_t* char_definition)
{
    uint8_t value[1] = { 0x0 };

    add_char(
        char_definition,
        1,
        value
    );
}

void ble_srv_add_int16_char(char_def_t* char_definition)
{
    uint8_t value[2] = { 0x0, 0x0 };

    add_char(
        char_definition,
        2,
        value
    );
}

void ble_srv_add_uint16_char(char_def_t* char_definition)
{
    uint8_t value[2] = { 0x0, 0x0 };

    add_char(
        char_definition,
        2,
        value
    );
}

void ble_srv_add_int32_char(char_def_t* char_definition)
{
    uint8_t value[4] = { 0x0, 0x0, 0x0, 0x0 };

    add_char(
        char_definition,
        4,
        value
    );
}

void ble_srv_add_uint32_char(char_def_t* char_definition)
{
    uint8_t value[4] = { 0x0, 0x0, 0x0, 0x0 };

    add_char(
        char_definition,
        4,
        value
    );
}

void ble_srv_add_int64_char(char_def_t* char_definition)
{
    uint8_t value[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    add_char(
        char_definition,
        8,
        value
    );
}

void ble_srv_add_uint64_char(char_def_t* char_definition)
{
    uint8_t value[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    add_char(
        char_definition,
        8,
        value
    );
}

static void add_char(
    char_def_t* char_definition,
    uint8_t length,
    uint8_t* initial_value)
{
    ble_uuid_t char_uuid;
    char_uuid.uuid = char_definition->srv_uuid;
    *char_definition->srv_conn_handle = BLE_CONN_HANDLE_INVALID;

    ret_code_t err_code = 
        sd_ble_uuid_vs_add(
            &char_definition->base_uuid, 
            &char_uuid.type
        );
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));

    if (char_definition->wr_type == ReadOnly ||
        char_definition->wr_type == ReadWrite)
    {
        char_md.char_props.read = 1;
    }

    if (char_definition->wr_type == WriteOnly ||
        char_definition->wr_type == ReadWrite)
    {
        char_md.char_props.write = 1;
    }

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md = &cccd_md;

    if (char_definition->is_notifying)
    {
        char_md.char_props.notify = 1;
    }

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.max_len = length;
    attr_char_value.init_len = length;
    attr_char_value.p_value = initial_value;

    err_code =
        sd_ble_gatts_characteristic_add(
            char_definition->srv_handle,
            &char_md,
            &attr_char_value,
            char_definition->char_handles
        );
    APP_ERROR_CHECK(err_code);
}
