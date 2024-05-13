#include <stdint.h>
#include <string.h>
#include <math.h>
#include "boards.h"
#include "seeed_xiao_sense.h"
#include "nordic_common.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_button.h"
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"
#include "nrf_power.h"
#include "nrf_sdh.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_clock.h"
#include "ble_subsystem.h"
#include "ant_subsystem.h"
#include "storage_helper.h"
#include "calibrate.h"

#define COM_PORT                "COM6"                  // The COM port that the NRF BLE dongle is attached to
#define DEVICE_NAME             "JPower"                // The BLE advertising device name
#define ADVERTISING_LED         BSP_BOARD_LED_2         // Is on when device is advertising
#define CONNECTED_LED           BSP_BOARD_LED_1         // Is on when device has connected

static ble_subsystem_config_t ble_subsystem_config =
{
    .device_name = DEVICE_NAME,
    .advertising_led_idx = ADVERTISING_LED,
    .connected_led_idx = CONNECTED_LED,
};

static ble_srv_nus_config_t ble_srv_nus_config =
{
    .data_handler = blesub_nus_default_data_handler,
};

static calibration_data_t calibration_data =
{
    .cal_id = 0,
    .slope = 0.0f
};

static void log_init();
static void io_init();
static void timers_init();
static void power_management_init();
static void idle_state_handler();
static void softdevice_init();
static void calibration_init();
static void start_timers();
static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event);
static void buttonless_dfu_sdh_state_observer(
    nrf_sdh_state_evt_t state, 
    void * p_context
);
static void slow_timer_callback(void* context);

NRF_SDH_STATE_OBSERVER(m_buttonless_dfu_state_obs, 0) =
{
    .handler = buttonless_dfu_sdh_state_observer,
};

APP_TIMER_DEF(slow_timer);

int main()
{
    log_init();
    io_init();
    timers_init();
    power_management_init();
    softdevice_init();

    blesub_enable_nus(&ble_srv_nus_config);
    blesub_enable_ble_dfu();
    blesub_init(&ble_subsystem_config);

    calibration_init();
    //antsub_init();

    start_timers();

    NRF_LOG_INFO("Logger app initialised");
    blesub_start_advertising();
    NRF_LOG_INFO("BLE started advertising...");

    while (true)
    {
        idle_state_handler();
    }
}

static void log_init()
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void io_init()
{
    ret_code_t err_code = ble_dfu_buttonless_async_svci_init();
    APP_ERROR_CHECK(err_code);

    bsp_board_init(BSP_INIT_LEDS);

    err_code = storage_init();
    APP_ERROR_CHECK(err_code);
}

static void timers_init()
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = 
        app_timer_create(
            &slow_timer,
            APP_TIMER_MODE_REPEATED,
            slow_timer_callback
        );

    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_hfclk_request(NULL);
    while (!nrf_drv_clock_hfclk_is_running()) { }
}

static void power_management_init()
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    NRF_PWR_MGMT_HANDLER_REGISTER(app_shutdown_handler, 0);
}

static void softdevice_init()
{
    ret_code_t err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    ASSERT(nrf_sdh_is_enabled());
}

static void calibration_init()
{
    ret_code_t err_code = calibrate_init();
    APP_ERROR_CHECK(err_code);

    if (!calibrate_get_is_calibrated())
    {
        // TODO - In future, go into awaiting cal state
        //        For now, put in some dummy data
        calibration_data_t calibration =
        {
            .cal_id = 0x0001,
            .slope = 0.5f
        };

        calibrate_set_calibration(calibration);
    }

    calibration_data = calibrate_get_calibration();

    NRF_LOG_INFO(
        "Cal ID: %lu - Slope: %f", 
        calibration_data.cal_id,
        calibration_data.slope
    );
}

static void start_timers()
{
    ret_code_t err_code = 
        app_timer_start(
            slow_timer, 
            APP_TIMER_TICKS(1000),
            NULL
        );

    APP_ERROR_CHECK(err_code);
}

static void idle_state_handler()
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event)
{
    switch (event)
    {
        case NRF_PWR_MGMT_EVT_PREPARE_DFU:
            NRF_LOG_INFO("Power management wants to reset to DFU mode.");
            break;
        default:
            return true;
    }
    NRF_LOG_INFO("Power management allowed to reset to DFU mode.");
    return true;
}

static void buttonless_dfu_sdh_state_observer(nrf_sdh_state_evt_t state, void * p_context)
{
    if (state == NRF_SDH_EVT_STATE_DISABLED)
    {
        nrf_power_gpregret2_set(BOOTLOADER_DFU_SKIP_CRC);
        nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
    }
}

static void slow_timer_callback(void* context)
{
    calibrate_update();
}