#ifndef BLE_SRV_HELPER_H__
#define BLE_SRV_HELPER_H__

#include <stdbool.h>
#include <stdint.h>
#include "ble_srv_common.h"

typedef enum
{
    ReadOnly,
    WriteOnly,
    ReadWrite
} char_wr_t;

typedef struct
{
    ble_uuid128_t base_uuid;
    uint16_t srv_uuid;
    uint16_t srv_handle;
    uint16_t* srv_conn_handle;
    ble_gatts_char_handles_t* char_handles;
    char_wr_t wr_type;
    bool is_notifying;
} char_def_t;

extern void ble_srv_add_int8_char(char_def_t* char_definition);
extern void ble_srv_add_uint8_char(char_def_t* char_definition);
extern void ble_srv_add_int16_char(char_def_t* char_definition);
extern void ble_srv_add_uint16_char(char_def_t* char_definition);
extern void ble_srv_add_int32_char(char_def_t* char_definition);
extern void ble_srv_add_uint32_char(char_def_t* char_definition);
extern void ble_srv_add_int64_char(char_def_t* char_definition);
extern void ble_srv_add_uint64_char(char_def_t* char_definition);

#endif // BLE_SRV_HELPER_H__