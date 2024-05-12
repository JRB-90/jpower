#ifndef BLE_HELPER_H__
#define BLE_HELPER_H__

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
#include "ble_lbs.h"
#include "ble_nus.h"
#include "ble_dfu.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"

// ======== BLE Settings ========

#define APP_BLE_OBSERVER_PRIO           3                                       // Application's BLE observer priority. You shouldn't need to modify this value
#define APP_BLE_CONN_CFG_TAG            1                                       // A tag identifying the SoftDevice BLE configuration
#define APP_ADV_INTERVAL                64                                      // The advertising interval (in units of 0.625 ms; this value corresponds to 40 ms)
#define APP_ADV_DURATION                BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED   // The advertising time-out (in units of seconds). When set to 0, we will never time out

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)        // Minimum acceptable connection interval (0.5 seconds)
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)        // Maximum acceptable connection interval (1 second)
#define SLAVE_LATENCY                   0                                       // Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(1000, UNIT_10_MS)         // Connection supervisory time-out (1 seconds)
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(20000)                  // Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds)
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000)                   // Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds)
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                       // Number of attempts before giving up the connection parameter negotiation

// ======== BLE Types ========

typedef enum ble_subsystem_service_t
{
    BLE_SRV_LBS     = 0b00000001,
    BLE_SRV_NUS     = 0b00000010,
    BLE_SRV_BLE_DFU = 0b00000100,
} ble_subsystem_service_t;

typedef struct ble_srv_lbs_config_t
{
    uint8_t button_pin;
    uint32_t button_detection_delay_timer_ticks;
    uint32_t led_idx;
    ble_lbs_led_write_handler_t led_write_handler;
} ble_srv_lbs_config_t;

typedef struct ble_srv_nus_config_t
{
    ble_nus_data_handler_t data_handler;
} ble_srv_nus_config_t;

typedef struct ble_subsystem_config_t
{
    const char* device_name;
    uint32_t advertising_led_idx;
    uint32_t connected_led_idx;
} ble_subsystem_config_t;

// ======== BLE Functions ========

extern void blesub_enable_lbs(ble_srv_lbs_config_t* ble_srv_lbs_config);
extern void blesub_enable_nus(ble_srv_nus_config_t* ble_srv_nus_config);
extern void blesub_enable_ble_dfu();
extern void blesub_service_init(
    ble_uuid128_t base_uuid,
    uint16_t service_uuid,
    uint16_t* service_handle
);
extern void blesub_init(ble_subsystem_config_t* ble_subsystem_config);
extern void blesub_start_advertising();
extern void blesub_nus_send_string(const char* string);
extern void blesub_nus_default_data_handler(ble_nus_evt_t* event);
extern void blesub_lbs_default_led_write_handler(
    uint16_t conn_handle, 
    ble_lbs_t* p_lbs,
    uint8_t led_state
);

#endif /* BLE_HELPER_H__ */