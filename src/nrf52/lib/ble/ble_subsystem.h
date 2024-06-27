#ifndef BLE_HELPER_H__
#define BLE_HELPER_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_timer.h"
#include "nrf_power.h"
#include "nrf_bootloader_info.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_dfu_ble_svci_bond_sharing.h"
#include "nrf_svci_async_function.h"
#include "nrf_svci_async_handler.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"

typedef struct
{
    bool with_dfu_enabled;
    bool with_nus_enabled;
    bool with_bas_enabled;
} ble_standard_srv_config_t;

typedef void (*ble_subsystem_conn_state_changed_t)(bool is_connected);

typedef struct
{
    const char* device_name;
    ble_standard_srv_config_t standard_services;
    ble_subsystem_conn_state_changed_t conn_state_handler;
} ble_subsystem_config_t;

// ======== BLE Functions ========

extern ret_code_t blesub_init(ble_subsystem_config_t* ble_subsystem_config);
extern bool blesub_is_advertising();
extern bool blesub_is_connected();
extern void blesub_start_advertising();
extern void blesub_stop_advertising();
extern void blesub_custom_service_init(
    ble_uuid128_t base_uuid,
    uint16_t service_uuid,
    uint16_t* service_handle
);

extern void blesub_nus_send_string(const char* string);
extern void blesub_bas_update_battery_level(uint8_t level);

#endif /* BLE_HELPER_H__ */