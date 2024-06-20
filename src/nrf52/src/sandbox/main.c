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
#include "sensor_subsystem.h"
#include "led_control.h"

#define HI_FREQ_CLK_HZ          100                     // Frequency (Hz) of the high speed timer
#define HI_FREQ_CLK_PERIOD_MS   1000 / HI_FREQ_CLK_HZ   // High speed timer period in ms

APP_TIMER_DEF(timer_10ms);

static const sensor_config_t sensor_config =
{
    .i2c_scl_pin    = I2C_SCL_PIN,
    .i2c_sda_pin    = I2C_SDA_PIN,
    .spi_sck_pin    = SPI_SCK_PIN,
    .spi_mosi_pin   = SPI_MOSI_PIN,
    .spi_miso_pin   = SPI_MISO_PIN,
    .spi_cs_pin     = SPI_CS_PIN,
};

static void board_init();
static void softdevice_init();
static void start_timers();
static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event);
static void callback_10ms(void* context);

int main()
{
    board_init();
    NRF_LOG_INFO("Board initialised");

    softdevice_init();
    NRF_LOG_INFO("Soft device initialised");
    
    start_timers();
    led_control_set(LED_STATE_SLOW_PULSE);
    NRF_LOG_INFO("JPower fully initialised");

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
    bsp_board_init(BSP_INIT_LEDS);

    // Sensors init
    err_code = sensor_subsystem_init(&sensor_config);
    APP_ERROR_CHECK(err_code);

    // Clock + timers init
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = 
        app_timer_create(
            &timer_10ms,
            APP_TIMER_MODE_REPEATED,
            callback_10ms
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
}

static void start_timers()
{
    ret_code_t err_code = 
        app_timer_start(
            timer_10ms, 
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

static void callback_10ms(void* context)
{
    NRF_TIMER1->TASKS_CAPTURE[1] = 1;
    uint32_t interval_us = NRF_TIMER1->CC[1];
    NRF_TIMER1->TASKS_CLEAR = 1;
    float time_delta = (float)interval_us / 1000000.0f;

    led_control_update_10ms();
    //sensor_subsystem_update_10ms(time_delta);
}
