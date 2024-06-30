#include "ble_subsystem.h"

#include "app_error.h"
#include "app_button.h"
#include "boards.h"
#include "bsp.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "ble_subsystem_settings.h"
#include "ble_gap.h"
#include "ble_advertising.h"
#include "ble_srv_helper.h"
#include "ble_dfu.h"
#include "ble_nus.h"
#include "ble_bas.h"
#include "led_control.h"
#include "battery.h"

#pragma region Private Data

static bool is_advertising = false;
static bool is_connected = false;
static ble_subsystem_config_t ble_config = {0};                         // Configuration params of the ble subsystem
static ble_nus_data_handler_t ble_nus_data_handler;                     // Data handler for the nus service
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                // Handle of the current connection
static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;           // Advertising handle used to identify an advertising set
static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];            // Buffer for storing an encoded advertising set
static uint8_t m_enc_scan_response_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX]; // Buffer for storing an encoded scan data

static ble_gap_adv_data_t m_adv_data =
{
    .adv_data =
    {
        .p_data = m_enc_advdata,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
    },
    .scan_rsp_data =
    {
        .p_data = m_enc_scan_response_data,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
    }
};

NRF_BLE_GATT_DEF(m_gatt);                                               // GATT module instance
NRF_BLE_QWR_DEF(m_qwr);                                                 // Context for the Queued Write module
BLE_ADVERTISING_DEF(m_advertising);                                     // Advertising instance
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                       // BLE NUS service instance
BLE_BAS_DEF(m_bas);                                                     // Battery service instance

#pragma endregion

#pragma region Private Function Defs

static void init_ble_stack();
static void init_gap_params();
static void init_gatt();
static void init_standard_services();
static void init_advertising();
static void init_conn_params();
static void ble_event_handler(
    ble_evt_t const* p_ble_evt, 
    void* p_context
);
static void conn_params_event_handler(ble_conn_params_evt_t * p_evt);
static void conn_params_error_handler(uint32_t nrf_error);
static void nrf_qwr_error_handler(uint32_t nrf_error);
static void ble_dfu_buttonless_evt_handler(ble_dfu_buttonless_evt_type_t event);
static void buttonless_dfu_sdh_state_observer(
    nrf_sdh_state_evt_t state, 
    void * p_context
);
static void bas_event_handler(
    ble_bas_t * p_bas, 
    ble_bas_evt_t * p_evt
);

NRF_SDH_STATE_OBSERVER(buttonless_dfu_state_obs, 0) =
{
    .handler = buttonless_dfu_sdh_state_observer,
};

#pragma endregion

#pragma region Public Function Impl

ret_code_t blesub_init(ble_subsystem_config_t* ble_subsystem_config)
{
    ble_config = *ble_subsystem_config;

    init_ble_stack();
    init_gap_params();
    init_gatt();
    init_standard_services();
    init_conn_params();

    return NRF_SUCCESS;
}

bool blesub_is_advertising()
{
    return is_advertising;
}

bool blesub_is_connected()
{
    return is_connected;
}

void blesub_start_advertising()
{
    if (is_advertising)
    {
        return;
    }

    init_advertising();

    ret_code_t err_code = 
        sd_ble_gap_adv_start(
            m_adv_handle, 
            APP_BLE_CONN_CFG_TAG
        );
    APP_ERROR_CHECK(err_code);

    is_advertising = true;
}

void blesub_stop_advertising()
{
    if (!is_advertising)
    {
        return;
    }

    ret_code_t err_code = 
        sd_ble_gap_adv_stop(
            m_adv_handle
        );
    APP_ERROR_CHECK(err_code);

    is_advertising = false;
}

void blesub_custom_service_init(
    ble_uuid128_t base_uuid,
    uint16_t service_uuid,
    uint16_t* service_handle)
{
    ble_uuid_t ble_service_uuid;
    ble_uuid128_t bsrv_base_uuid = base_uuid;
    ble_service_uuid.uuid = service_uuid;
    
    ret_code_t err_code =
        sd_ble_uuid_vs_add(
            &bsrv_base_uuid,
            &ble_service_uuid.type
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        sd_ble_gatts_service_add(
            BLE_GATTS_SRVC_TYPE_PRIMARY,
            &ble_service_uuid,
            service_handle
        );
    APP_ERROR_CHECK(err_code);

    ble_service_uuid.type = BLE_UUID_TYPE_VENDOR_BEGIN;
}

void blesub_nus_send_string(const char* string)
{
    uint16_t length = (uint16_t)strlen(string);

    ret_code_t err_code =
        ble_nus_data_send(
            &m_nus,
            (uint8_t*)string,
            &length,
            m_conn_handle 
        );

    if ((err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != NRF_ERROR_RESOURCES) &&
        (err_code != NRF_ERROR_NOT_FOUND))
    {
        APP_ERROR_CHECK(err_code);
    }
}

void blesub_nus_default_data_handler(ble_nus_evt_t* event)
{
    if (event->type == BLE_NUS_EVT_RX_DATA)
    {
        // Handle receive of data over nus
        NRF_LOG_INFO("Received data from BLE NUS");
        NRF_LOG_HEXDUMP_INFO(
            event->params.rx_data.p_data, 
            event->params.rx_data.length
        );
    }
}

void blesub_bas_update_battery_level(uint8_t level)
{
    ret_code_t err_code = 
        ble_bas_battery_level_update(
            &m_bas, 
            level, 
            m_conn_handle
        );
    
    if ((err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != NRF_ERROR_RESOURCES) &&
        (err_code != NRF_ERROR_NOT_FOUND) &&
        (err_code != BLE_ERROR_INVALID_CONN_HANDLE))
    {
        APP_ERROR_CHECK(err_code);
    }
}

#pragma endregion

#pragma region Private Function Impl

static void init_ble_stack()
{
    ret_code_t err_code;

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = 
        nrf_sdh_ble_default_cfg_set(
            APP_BLE_CONN_CFG_TAG, 
            &ram_start
        );
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(
        m_ble_observer, 
        APP_BLE_OBSERVER_PRIO, 
        ble_event_handler, 
        NULL
    );
}

static void init_gap_params()
{
    ret_code_t              err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = 
        sd_ble_gap_device_name_set(
            &sec_mode,
            (const uint8_t *)ble_config.device_name,
            strlen(ble_config.device_name)
        );
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

static void init_gatt()
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}

static void init_standard_services()
{
    ret_code_t         err_code;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Init Queued Write Module
    memset(&qwr_init, 0, sizeof(qwr_init));
    qwr_init.error_handler = nrf_qwr_error_handler;
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    if (ble_config.standard_services.with_dfu_enabled)
    {
        // Initialize the DFU service
        ble_dfu_buttonless_init_t dfus_init =
        {
            .evt_handler = ble_dfu_buttonless_evt_handler
        };
        err_code = ble_dfu_buttonless_init(&dfus_init);
        APP_ERROR_CHECK(err_code);
    }

    if (ble_config.standard_services.with_nus_enabled)
    {
        // Init Nordic Uart Service
        ble_nus_init_t nus_init;
        memset(&nus_init, 0, sizeof(nus_init));
        nus_init.data_handler = ble_nus_data_handler;
        err_code = ble_nus_init(&m_nus, &nus_init);
        APP_ERROR_CHECK(err_code);
    }

    if (ble_config.standard_services.with_bas_enabled)
    {
        // Init Battery service
        ble_bas_init_t bas_init;
        memset(&bas_init, 0, sizeof(bas_init));

        
        bas_init.evt_handler = bas_event_handler;
        bas_init.support_notification = true;
        bas_init.p_report_ref = NULL;
        bas_init.initial_batt_level = battery_get_level_percentage();

        bas_init.bl_cccd_wr_sec = SEC_OPEN;
        bas_init.bl_rd_sec = SEC_OPEN;
        bas_init.bl_report_rd_sec = SEC_OPEN;

        err_code = ble_bas_init(&m_bas, &bas_init);
        APP_ERROR_CHECK(err_code);
    }
}

static void init_advertising()
{
    ret_code_t    err_code;
    ble_advdata_t advdata;
    ble_advdata_t srdata;

    // TODO - Should work like below, but need to figure out the out of mem
    //        error for more than one advertised service uuid..
    // ble_uuid_t adv_uuids[NRF_SDH_BLE_VS_UUID_COUNT];

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    memset(&srdata, 0, sizeof(srdata));
    //srdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    //srdata.uuids_complete.p_uuids  = adv_uuids;
    srdata.name_type = BLE_ADVDATA_FULL_NAME;

    err_code = 
        ble_advdata_encode(
            &advdata, 
            m_adv_data.adv_data.p_data, 
            &m_adv_data.adv_data.len
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        ble_advdata_encode(
            &srdata,
            m_adv_data.scan_rsp_data.p_data, 
            &m_adv_data.scan_rsp_data.len
        );
    APP_ERROR_CHECK(err_code);

    ble_gap_adv_params_t adv_params;

    // Set advertising parameters.
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.primary_phy     = BLE_GAP_PHY_1MBPS;
    adv_params.duration        = APP_ADV_DURATION;
    adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
    adv_params.p_peer_addr     = NULL;
    adv_params.filter_policy   = BLE_GAP_ADV_FP_ANY;
    adv_params.interval        = APP_ADV_INTERVAL;

    err_code = 
        sd_ble_gap_adv_set_configure(
            &m_adv_handle, 
            &m_adv_data, 
            &adv_params
        );
    APP_ERROR_CHECK(err_code);
}

static void init_conn_params()
{
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = conn_params_event_handler;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

static void ble_event_handler(
    ble_evt_t const* p_ble_evt, 
    void* p_context)
{
    ret_code_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            is_connected = true;
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            if (ble_config.conn_state_handler != NULL)
            {
                ble_config.conn_state_handler(is_connected);
            }
            NRF_LOG_INFO("Connected");
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            is_connected = false;
            is_advertising = false;
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            blesub_start_advertising();
            if (ble_config.conn_state_handler != NULL)
            {
                ble_config.conn_state_handler(is_connected);
            }
            NRF_LOG_INFO("Disconnected");
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code =
                sd_ble_gap_sec_params_reply(
                    m_conn_handle,
                    BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP,
                    NULL,
                    NULL
                );
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = 
                sd_ble_gap_phy_update(
                    p_ble_evt->evt.gap_evt.conn_handle, 
                    &phys
                );
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored
            err_code = 
                sd_ble_gatts_sys_attr_set(
                    m_conn_handle,
                    NULL, 
                    0, 
                    0
                );
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event
            NRF_LOG_DEBUG("GATT Client Timeout");
            err_code = 
                sd_ble_gap_disconnect(
                    p_ble_evt->evt.gattc_evt.conn_handle,
                    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION
                );
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event
            NRF_LOG_DEBUG("GATT Server Timeout");
            err_code = 
                sd_ble_gap_disconnect(
                    p_ble_evt->evt.gatts_evt.conn_handle,
                    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION
                );
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed
            break;
    }
}

static void conn_params_event_handler(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = 
            sd_ble_gap_disconnect(
                m_conn_handle, 
                BLE_HCI_CONN_INTERVAL_UNACCEPTABLE
            );

        APP_ERROR_CHECK(err_code);
    }
}

static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

static void ble_dfu_buttonless_evt_handler(ble_dfu_buttonless_evt_type_t event)
{
    switch (event)
    {
        case BLE_DFU_EVT_BOOTLOADER_ENTER_PREPARE:
            NRF_LOG_INFO("Device is preparing to enter bootloader mode\r\n");
            break;
 
        case BLE_DFU_EVT_BOOTLOADER_ENTER:
            NRF_LOG_INFO("Device will enter bootloader mode\r\n");
            break;
 
        case BLE_DFU_EVT_BOOTLOADER_ENTER_FAILED:
            NRF_LOG_ERROR("Device failed to enter bootloader mode\r\n");
            break;
        default:
            NRF_LOG_INFO("Unknown event from ble_dfu.\r\n");
            break;
    }
}

static void buttonless_dfu_sdh_state_observer(nrf_sdh_state_evt_t state, void * p_context)
{
    if (state == NRF_SDH_EVT_STATE_DISABLED)
    {
        nrf_power_gpregret2_set(BOOTLOADER_DFU_SKIP_CRC);
        nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
    }
}

static void bas_event_handler(ble_bas_t * p_bas, ble_bas_evt_t * p_evt)
{
    ret_code_t err_code;

    switch (p_evt->evt_type)
    {
        case BLE_BAS_EVT_NOTIFICATION_ENABLED:
            break; // BLE_BAS_EVT_NOTIFICATION_ENABLED

        case BLE_BAS_EVT_NOTIFICATION_DISABLED:
            break; // BLE_BAS_EVT_NOTIFICATION_DISABLED

        default:
            // No implementation needed.
            break;
    }
}

#pragma endregion
