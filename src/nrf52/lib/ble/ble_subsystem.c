#include "ble_subsystem.h"

#include "app_error.h"
#include "app_button.h"
#include "boards.h"
#include "bsp.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "ble_advertising.h"
#include "ble_srv_helper.h"

// ======== BLE Data ========

NRF_BLE_GATT_DEF(m_gatt);                                               // GATT module instance
NRF_BLE_QWR_DEF(m_qwr);                                                 // Context for the Queued Write module

static ble_subsystem_config_t* ble_config = {0};                        // Configuration params of the ble subsystem
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                // Handle of the current connection
static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;           // Advertising handle used to identify an advertising set
static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];            // Buffer for storing an encoded advertising set
static uint8_t m_enc_scan_response_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX]; // Buffer for storing an encoded scan data

static ble_subsystem_config_t default_ble_subsystem_config =
{
    .device_name = DEVICE_NAME,
    .advertising_led_idx = BSP_BOARD_LED_2,
    .connected_led_idx = BSP_BOARD_LED_1,
};

static ble_srv_nus_config_t default_ble_srv_nus_config =
{
    .data_handler = blesub_nus_default_data_handler,
};

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

// ======== Services ========

static uint32_t enabled_services = 0;                                   // Enabled services bitmask
static ble_srv_lbs_config_t* lbs_config = NULL;                         // Configuration params of the lbs service
static ble_srv_nus_config_t* nus_config = NULL;                         // Configuration params of the nus service

BLE_ADVERTISING_DEF(m_advertising);                                     // Advertising instance
BLE_LBS_DEF(m_lbs);                                                     // LED Button Service instance
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                       // BLE NUS service instance

// ======== Function Defs ========

static void init_ble_stack();
static void init_gap_params();
static void init_gatt();
static void init_services();
static void init_advertising();
static void init_conn_params();
static void ble_event_handler(
    ble_evt_t const* p_ble_evt, 
    void* p_context
);
static void conn_params_event_handler(ble_conn_params_evt_t * p_evt);
static void conn_params_error_handler(uint32_t nrf_error);
static void nrf_qwr_error_handler(uint32_t nrf_error);
static void button_handler(
    uint8_t pin_no, 
    uint8_t button_action
);
static void ble_dfu_buttonless_evt_handler(ble_dfu_buttonless_evt_type_t event);
static void buttonless_dfu_sdh_state_observer(
    nrf_sdh_state_evt_t state, 
    void * p_context
);

NRF_SDH_STATE_OBSERVER(buttonless_dfu_state_obs, 0) =
{
    .handler = buttonless_dfu_sdh_state_observer,
};

// ======== Functions Impl ========

void blesub_enable_lbs(ble_srv_lbs_config_t* ble_srv_lbs_config)
{
    lbs_config = ble_srv_lbs_config;
    enabled_services |= BLE_SRV_LBS;

    NRF_LOG_INFO("BLE enabled LBS service");
}

void blesub_enable_nus(ble_srv_nus_config_t* ble_srv_nus_config)
{
    if (ble_srv_nus_config == NULL)
    {
        nus_config = &default_ble_srv_nus_config;
    }
    else
    {
        nus_config = ble_srv_nus_config;
    }

    enabled_services |= BLE_SRV_NUS;

    NRF_LOG_INFO("BLE enabled NUS service");
}

void blesub_enable_ble_dfu()
{
    enabled_services |= BLE_SRV_BLE_DFU;

    NRF_LOG_INFO("BLE enabled BLE DFU service");
}

void blesub_service_init(
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

void blesub_init(ble_subsystem_config_t* ble_subsystem_config)
{
    if (ble_subsystem_config == NULL)
    {
        ble_config = &default_ble_subsystem_config;
    }
    else
    {
        ble_config = ble_subsystem_config;
    }

    init_ble_stack();
    init_gap_params();
    init_gatt();
    init_services();
    init_conn_params();

    NRF_LOG_INFO("BLE init success");
}

void blesub_start_advertising()
{
    init_advertising();

    ret_code_t err_code = 
        sd_ble_gap_adv_start(
            m_adv_handle, 
            APP_BLE_CONN_CFG_TAG
        );
    APP_ERROR_CHECK(err_code);

    bsp_board_led_on(ble_config->advertising_led_idx);
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

void blesub_lbs_default_led_write_handler(
    uint16_t conn_handle, 
    ble_lbs_t * p_lbs, 
    uint8_t led_state)
{
    if (led_state)
    {
        bsp_board_led_on(lbs_config->led_idx);
        NRF_LOG_INFO("Received LED ON!");
    }
    else
    {
        bsp_board_led_off(lbs_config->led_idx);
        NRF_LOG_INFO("Received LED OFF!");
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
            (const uint8_t *)ble_config->device_name,
            strlen(ble_config->device_name)
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

static void init_services()
{
    ret_code_t         err_code;
    nrf_ble_qwr_init_t qwr_init = {0};
    ble_lbs_init_t     lbs_init = {0};
    ble_nus_init_t     nus_init = {0};

    // Init Queued Write Module
    memset(&qwr_init, 0, sizeof(qwr_init));
    qwr_init.error_handler = nrf_qwr_error_handler;
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    if (enabled_services & BLE_SRV_LBS)
    {
        // Init Led Button Service

        // The array must be static because a pointer to it will be saved in the button handler module
        static app_button_cfg_t buttons[] =
        {
            {
                0,
                false,
                BUTTON_PULL,
                button_handler
            }
        };

        buttons->pin_no = lbs_config->button_pin;

        err_code = 
            app_button_init(
                buttons, 
                ARRAY_SIZE(buttons),
                lbs_config->button_detection_delay_timer_ticks
            );
        APP_ERROR_CHECK(err_code);
    
        memset(&lbs_init, 0, sizeof(lbs_init));
        lbs_init.led_write_handler = lbs_config->led_write_handler;

        err_code = ble_lbs_init(&m_lbs, &lbs_init);
        APP_ERROR_CHECK(err_code);
    }

    if (enabled_services & BLE_SRV_NUS)
    {
        // Init Nordic Uart Service
        memset(&nus_init, 0, sizeof(nus_init));
        nus_init.data_handler = nus_config->data_handler;
        err_code = ble_nus_init(&m_nus, &nus_init);
        APP_ERROR_CHECK(err_code);
    }

    if (enabled_services & BLE_SRV_BLE_DFU)
    {
        // Initialize the DFU service
        ble_dfu_buttonless_init_t dfus_init =
        {
            .evt_handler = ble_dfu_buttonless_evt_handler
        };
        err_code = ble_dfu_buttonless_init(&dfus_init);
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
            NRF_LOG_INFO("Connected");
            bsp_board_led_on(ble_config->connected_led_idx);
            bsp_board_led_off(ble_config->advertising_led_idx);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            if (enabled_services & BLE_SRV_LBS)
            {
                err_code = app_button_enable();
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected");
            bsp_board_led_off(ble_config->connected_led_idx);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            if (enabled_services & BLE_SRV_LBS)
            {
                bsp_board_led_off(lbs_config->led_idx);
                err_code = app_button_disable();
                APP_ERROR_CHECK(err_code);
            }
            blesub_start_advertising();
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

static void button_handler(
    uint8_t pin_no, 
    uint8_t button_action)
{
    ret_code_t err_code;

    if (pin_no == lbs_config->button_pin)
    {
        NRF_LOG_INFO("Send button state change.");
        err_code = 
            ble_lbs_on_button_change(
                m_conn_handle, 
                &m_lbs, 
                button_action
            );

        if (err_code != NRF_SUCCESS &&
            err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
            err_code != NRF_ERROR_INVALID_STATE &&
            err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
        {
            APP_ERROR_CHECK(err_code);
        }
    }
    else
    {
        APP_ERROR_HANDLER(pin_no);
    }
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
