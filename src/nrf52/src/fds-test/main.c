#include <stdint.h>
#include <stdbool.h>
#include "boards.h"
#include "seeed_xiao_sense.h"
#include "app_error.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"
#include "nrf_sdh.h"
#include "nrf_power.h"
#include "nrf_pwr_mgmt.h"
#include "fds.h"
#include "storage_helper.h"

// File ID and Key used for the configuration record
#define CONFIG_FILE     0x8010
#define CONFIG_REC_KEY  0x7010

/* A dummy structure to save in flash. */
typedef struct
{
    uint32_t boot_count;
    char     device_name[16];
    bool     config1_on;
    bool     config2_on;
} configuration_t;

/* Dummy configuration data. */
static configuration_t dummy_cfg =
{
    .config1_on  = false,
    .config2_on  = true,
    .boot_count  = 0x0,
    .device_name = "dummy",
};

/* A record containing dummy configuration data. */
static fds_record_t const dummy_record_t =
{
    .file_id           = CONFIG_FILE,
    .key               = CONFIG_REC_KEY,
    .data.p_data       = &dummy_cfg,
    /* The length of a record is always expressed in 4-byte units (words). */
    .data.length_words = (sizeof(dummy_cfg) + 3) / sizeof(uint32_t),
};

int main()
{
    ret_code_t err_code;

    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    bsp_board_init(BSP_INIT_LEDS);
    bsp_board_led_on(BSP_BOARD_LED_0);

    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);
    ASSERT(nrf_sdh_is_enabled());

    err_code = storage_init();
    APP_ERROR_CHECK(err_code);

    bsp_board_led_off(BSP_BOARD_LED_0);
    bsp_board_led_on(BSP_BOARD_LED_1);

    fds_stat_t stat = {0};
    err_code = fds_stat(&stat);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Found %d valid records.", stat.valid_records);
    NRF_LOG_INFO("Found %d dirty records (ready to be garbage collected).", stat.dirty_records);

    err_code = storage_write(&dummy_record_t);
    APP_ERROR_CHECK(err_code);

    err_code = fds_stat(&stat);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Found %d valid records.", stat.valid_records);
    NRF_LOG_INFO("Found %d dirty records (ready to be garbage collected).", stat.dirty_records);

    fds_flash_record_t read_record = {0};
    err_code = 
        storage_read(
            CONFIG_FILE,
            CONFIG_REC_KEY,
            &read_record
        );
    APP_ERROR_CHECK(err_code);

    configuration_t config;
    memcpy(&config, read_record.p_data, sizeof(configuration_t));
    NRF_LOG_INFO("Record read: %s", config.device_name);

    err_code =
        storage_delete(
            CONFIG_FILE,
            CONFIG_REC_KEY
        );
    APP_ERROR_CHECK(err_code);

    err_code = fds_stat(&stat);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Found %d valid records.", stat.valid_records);
    NRF_LOG_INFO("Found %d dirty records (ready to be garbage collected).", stat.dirty_records);

    bsp_board_led_off(BSP_BOARD_LED_1);
    bsp_board_led_on(BSP_BOARD_LED_2);

    NRF_LOG_FLUSH();

    while (true)
    {
        if (NRF_LOG_PROCESS() == false)
        {
            nrf_pwr_mgmt_run();
        }
    }
}
