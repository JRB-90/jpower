#include <stdint.h>
#include <string.h>
#include <math.h>
#include "boards.h"
#include "jpower_v1.h"
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
#include "nrf_drv_twi.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_rng.h"
#include "ble_subsystem.h"
#include "ant_subsystem.h"
#include "jpow_math.h"
#include "imu.h"
#include "imu_srv.h"
#include "strain.h"
#include "storage_helper.h"
#include "calibrate.h"
#include "calibrate_srv.h"
#include "jp_state.h"
#include "jp_state_srv.h"
#include "jp_utils.h"

#define DEVICE_NAME             "JPower"                // Name of device. Will be included in the advertising data
#define TWI_INSTANCE_ID         0                       // Internal TWI device to use
#define SPI_INSTANCE_ID         1                       // Internal SPI device to use
#define ADVERTISING_LED         BSP_BOARD_LED_0         // Is on when device is advertising
#define CONNECTED_LED           BSP_BOARD_LED_0         // Is on when device has connected
#define HI_FREQ_CLK_HZ          100                     // Frequency (Hz) of the high speed timer
#define HI_FREQ_CLK_PERIOD_MS   1000 / HI_FREQ_CLK_HZ   // High speed timer period in ms

APP_TIMER_DEF(high_freq_timer);

static ble_subsystem_config_t ble_subsystem_config =
{
    .device_name = DEVICE_NAME,
    .advertising_led_idx = ADVERTISING_LED,
    .connected_led_idx = CONNECTED_LED,
};

static nrf_drv_twi_t twi        = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static nrf_drv_spi_t spi        = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE_ID);

static void board_init();
static void softdevice_init();
static void ble_init();
static void start_timers();
static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event);
static void high_freq_callback(void* context);
static void enter_calibrate_mode();
static void enter_run_mode();
static void on_state_request(jp_state_request_t request);

int main()
{
    board_init();
    NRF_LOG_INFO("Board initialised");

    softdevice_init();
    NRF_LOG_INFO("Soft device initialised");

    ble_init();    
    NRF_LOG_INFO("BLE initialised");

    //antsub_init();
    NRF_LOG_INFO("ANT Plus initialised");
    
    start_timers();

    jp_state_change_state(JP_STATE_INITIALISED);
    NRF_LOG_INFO("JPower fully initialised");

    if (!calibrate_get_is_calibrated())
    {
        NRF_LOG_INFO("No calibration data found");

        calibration_data_t dummy_cal =
        {
            .cal_id = jp_utils_gen_new_guid(),
            .slope = 123.456f,
            .intercept = 42.42f
        };

        calibrate_set_calibration(dummy_cal);
        enter_calibrate_mode();
    }
    else
    {
        NRF_LOG_INFO("Calibration data found");
        enter_run_mode();
    }

    while (true)
    {
        if (NRF_LOG_PROCESS() == false)
        {
            nrf_pwr_mgmt_run();
        }
    }
}

static void board_init()
{
    ret_code_t err_code;

    // Logging init
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // IO init
    err_code = ble_dfu_buttonless_async_svci_init();
    APP_ERROR_CHECK(err_code);

    bsp_board_init(BSP_INIT_LEDS);

    err_code =
        imu_init(
            &twi,
            I2C_SCL_PIN,
            I2C_SDA_PIN
        );
    APP_ERROR_CHECK(err_code);

    err_code =
        strain_init(
            &spi,
            SPI_SCK_PIN,
            SPI_MOSI_PIN,
            SPI_MISO_PIN,
            SPI_CS_PIN
        );
    APP_ERROR_CHECK(err_code);

    // Timers init
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = 
        app_timer_create(
            &high_freq_timer,
            APP_TIMER_MODE_REPEATED,
            high_freq_callback
        );

    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_hfclk_request(NULL);
    while (!nrf_drv_clock_hfclk_is_running()) { }

    NRF_TIMER1->TASKS_STOP = 1;	
    NRF_TIMER1->MODE = 0; // Timer mode
    NRF_TIMER1->PRESCALER = 4;	// 16 MHz / 2^PRESCALER 
	NRF_TIMER1->CC[0] = 0;
	NRF_TIMER1->BITMODE = (TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos);
	NRF_TIMER1->TASKS_CLEAR = 1;

    // Power management init
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    NRF_PWR_MGMT_HANDLER_REGISTER(app_shutdown_handler, 0);
}

static void softdevice_init()
{
    ret_code_t err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    ASSERT(nrf_sdh_is_enabled());

    err_code = storage_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_rng_init(NULL);
    APP_ERROR_CHECK(err_code);

    err_code = calibrate_init();
    APP_ERROR_CHECK(err_code);
}

static void ble_init()
{
    ret_code_t err_code;

    blesub_enable_ble_dfu();
    blesub_init(&ble_subsystem_config);

    err_code = imu_srv_init();
    APP_ERROR_CHECK(err_code);

    err_code = calibrate_srv_init();
    APP_ERROR_CHECK(err_code);

    err_code = jp_state_srv_init();
    APP_ERROR_CHECK(err_code);

    jp_state_srv_register_enter_state_cb(&on_state_request);

    blesub_start_advertising();
}

static void start_timers()
{
    ret_code_t err_code = 
        app_timer_start(
            high_freq_timer, 
            APP_TIMER_TICKS(HI_FREQ_CLK_PERIOD_MS),
            NULL
        );

    APP_ERROR_CHECK(err_code);

    NRF_TIMER1->TASKS_START = 1;
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

static uint64_t counter = 0;

static void high_freq_callback(void* context)
{
    NRF_TIMER1->TASKS_CAPTURE[1] = 1;
    //uint32_t interval_us = NRF_TIMER1->CC[1];
    NRF_TIMER1->TASKS_CLEAR = 1;
    //float time_delta = (float)interval_us / 1000000.0f;

    switch (jp_state_get_current_state())
    {
    case JP_STATE_CALIBRATING:
        calibrate_update();
        break;

    case JP_STATE_RUNNING:
        // Update imu power calc
        break;
    
    default:
        break;
    }

    char buf[128];
    float accel, gyro;
    ret_code_t err_code = imu_take_reading_raw(&accel, &gyro);
    APP_ERROR_CHECK(err_code);

    uint32_t adc;
    err_code = strain_get_raw_adc_value(&adc);
    APP_ERROR_CHECK(err_code);

    sprintf(
        buf,
        "Accel: %.6f, Gyro: %.6f, Adc: %ul", 
        accel,
        gyro,
        adc
    );

    if ((counter % 10) == 0)
    {
        NRF_LOG_INFO("%s", buf);
        NRF_LOG_FLUSH();
    }

    counter++;
}

static void enter_calibrate_mode()
{
    jp_state_t current_state = jp_state_get_current_state();

    if (current_state == JP_STATE_CALIBRATING ||
        current_state == JP_STATE_STARTUP)
    {
        NRF_LOG_ERROR("Cannot enter calibrating state");

        return;
    }

    jp_state_change_state(JP_STATE_CALIBRATING);
    NRF_LOG_INFO("Entered calibration mode");
}

static void enter_run_mode()
{
    jp_state_t current_state = jp_state_get_current_state();

    if (current_state == JP_STATE_RUNNING ||
        current_state == JP_STATE_STARTUP)
    {
        NRF_LOG_ERROR(
            "Cannot enter running state from: %i",
            (int)current_state
        );

        return;
    }

    bsp_board_led_off(ADVERTISING_LED);
    bsp_board_led_off(CONNECTED_LED);
    jp_state_change_state(JP_STATE_RUNNING);
    // TODO - Start ANT plus profile
    NRF_LOG_INFO("Entered run mode");
}

static void on_state_request(jp_state_request_t request)
{
    NRF_LOG_INFO("Enter state requested: %i", (int)request);

    switch (request)
    {
    case JP_STATE_REQUEST_SWITCH_TO_RUNNING:
        enter_run_mode();
        break;

    case JP_STATE_REQUEST_SWITCH_TO_CALIBRATE:
        enter_calibrate_mode();
        break;
    
    default:
        NRF_LOG_ERROR("Enter state not recognised: %i", (int)request);
        break;
    }
}
