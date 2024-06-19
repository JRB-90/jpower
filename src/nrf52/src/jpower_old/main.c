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
#include "nrf_drv_twi.h"
#include "ble_subsystem.h"
#include "ant_subsystem.h"
#include "jpow_math.h"
#include "imu.h"
#include "Fusion.h"

#define DEVICE_NAME             "JPower"                // Name of device. Will be included in the advertising data
#define IMU_SCL_PIN             I2C_SCL_INT_PIN         // The i2c scl pin that is on the imu i2c bus
#define IMU_SDA_PIN             I2C_SDA_INT_PIN         // The i2c sda pin that is on the imu i2c bus
#define ADVERTISING_LED         BSP_BOARD_LED_2         // Is on when device is advertising
#define CONNECTED_LED           BSP_BOARD_LED_1         // Is on when device has connected
#define TWI_INSTANCE_ID         0                       // Instance ID for the i2c device to read the IMU
#define HI_FREQ_CLK_HZ          100                     // Frequency (Hz) of the high speed timer
#define HI_FREQ_CLK_PERIOD_MS   1000 / HI_FREQ_CLK_HZ   // High speed timer period in ms
#define PWR_UPDATE_HZ           10                      // Frequency (Hz) to update the power status
#define PWR_UPDATE_PERIOD_MS    1000 / PWR_UPDATE_HZ    // Power update period in ms

#define CALLBACK_COUNT_TICKS    HI_FREQ_CLK_HZ / PWR_UPDATE_HZ
#define ANG_TO_RPM_RATIO        0.16666666f
#define EPS_F                   0.001f
#define CRANK_LENGTH            172.5f

APP_TIMER_DEF(high_freq_timer);

typedef struct
{
    float time;
    FusionQuaternion attitude;
} timestamped_reading_t;

typedef struct
{
    float angular_velocity;
    uint16_t cadence;
} pedal_state_t;

static nrf_drv_twi_t twi                    = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static FusionAhrs ahrs                      = { 0 };
static timestamped_reading_t last_hi_speed  = { 0.0f, {{ 1.0f, 0.0f, 0.0f, 0.0f }}};
static timestamped_reading_t last_lo_speed  = { 0.0f, {{ 1.0f, 0.0f, 0.0f, 0.0f }}};
static pedal_state_t pedal_state            = { 0.0f, 0 };
static float total_time_s                      = 0;
static uint16_t callback_count              = 0;

static FusionAhrsSettings settings =
{
    .convention = FusionConventionNwu,
    .gain = 0.5f,
    .gyroscopeRange = 2000.0f,
    .accelerationRejection = 90.0f,
    .magneticRejection = 90.0f,
    .recoveryTriggerPeriod = 0.25f,
};

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

static void log_init();
static void io_init();
static void timers_init();
static void power_management_init();
static void idle_state_handler();
static void softdevice_init();
static void start_timers();
static void high_freq_callback(void* context);
static void read_imu(float time_delta);
static void update_power();
static pedal_state_t calculate_pedal_state(
    float time_delta,
    const FusionQuaternion attitude_start,
    const FusionQuaternion attitude_end
);
static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event);
static void buttonless_dfu_sdh_state_observer(
    nrf_sdh_state_evt_t state, 
    void * p_context
);

NRF_SDH_STATE_OBSERVER(m_buttonless_dfu_state_obs, 0) =
{
    .handler = buttonless_dfu_sdh_state_observer,
};

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

    antsub_init();

    FusionAhrsInitialise(&ahrs);
    FusionAhrsSetSettings(&ahrs, &settings);

    NRF_LOG_INFO("Logger app initialised");
    start_timers();
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

    err_code =
        imu_init(
            &twi,
            IMU_PWR_PIN,
            IMU_SCL_PIN,
            IMU_SDA_PIN
        );
    APP_ERROR_CHECK(err_code);
}

static void timers_init()
{
    ret_code_t err_code = app_timer_init();
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

static void idle_state_handler()
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

static void high_freq_callback(void* context)
{
    NRF_TIMER1->TASKS_CAPTURE[1] = 1;
    uint32_t interval_us = NRF_TIMER1->CC[1];
    NRF_TIMER1->TASKS_CLEAR = 1;

    float time_delta = (float)interval_us / 1000000.0f;
    total_time_s += time_delta;

    read_imu(time_delta);

    if (callback_count > CALLBACK_COUNT_TICKS)
    {
        update_power();
        callback_count = 0;
    }

    callback_count++;
}

static void read_imu(float time_delta)
{
    FusionVector accel = {{ 0.0f, 0.0f, 0.0f }};
    FusionVector gyro = {{ 0.0f, 0.0f, 0.0f }};

    ret_code_t err_code =
        imu_take_reading_raw(
            accel.array, 
            gyro.array
        );

    APP_ERROR_CHECK(err_code);

    FusionAhrsUpdateNoMagnetometer(
        &ahrs,
        gyro,
        accel,
        time_delta
    );

    last_hi_speed.attitude = FusionAhrsGetQuaternion(&ahrs);
    last_hi_speed.time = total_time_s;
}

static void update_power()
{
    pedal_state =
        calculate_pedal_state(
            total_time_s - last_lo_speed.time,
            last_lo_speed.attitude,
            last_hi_speed.attitude
        );

    last_lo_speed.attitude = last_hi_speed.attitude;
    last_lo_speed.time = last_hi_speed.time;

    float distance = to_rad(pedal_state.angular_velocity) * M_PI * 0.1f;

    bike_power_data_t power_state =
    {
        .power = 120 * distance,
        .cadence = pedal_state.cadence
    };

    antsub_update_power(&power_state);
}

static pedal_state_t calculate_pedal_state(
    float time_delta,
    const FusionQuaternion attitude_start,
    const FusionQuaternion attitude_end)
{
    FusionQuaternion conj =
	{{
		attitude_start.element.w,
		-attitude_start.element.x,
        -attitude_start.element.y,
        -attitude_start.element.z,
	}};

	FusionQuaternion relative = 
        FusionQuaternionMultiply(
            attitude_end, 
            conj
        );

    float angle = 0.0f;

    if (relative.element.w <= (0.0f + EPS_F))
    {
        angle = M_PI;
    }
    else if (relative.element.w >= (1.0f - EPS_F))
    {
        angle = 0.0f;
    }
    else
    {
        angle = 2.0f * acosf(relative.element.w);
    }

    float angular_velocity = FusionRadiansToDegrees(angle) / time_delta;

    pedal_state_t state =
    {
        .angular_velocity = angular_velocity,
        .cadence = (uint16_t)fabsf(angular_velocity * ANG_TO_RPM_RATIO)
    };

    return state;
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
