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

// File ID and Key used for the configuration record
#define CONFIG_FILE     0x8010
#define CONFIG_REC_KEY  0x7010

static bool fds_initialized = false;
static bool fds_write_finished = false;
static bool fds_delete_finished = false;

/* Array to map FDS return values to strings. */
static char const* err_str[] =
{
    "FDS_ERR_OPERATION_TIMEOUT",
    "FDS_ERR_NOT_INITIALIZED",
    "FDS_ERR_UNALIGNED_ADDR",
    "FDS_ERR_INVALID_ARG",
    "FDS_ERR_NULL_ARG",
    "FDS_ERR_NO_OPEN_RECORDS",
    "FDS_ERR_NO_SPACE_IN_FLASH",
    "FDS_ERR_NO_SPACE_IN_QUEUES",
    "FDS_ERR_RECORD_TOO_LARGE",
    "FDS_ERR_NOT_FOUND",
    "FDS_ERR_NO_PAGES",
    "FDS_ERR_USER_LIMIT_REACHED",
    "FDS_ERR_CRC_CHECK_FAILED",
    "FDS_ERR_BUSY",
    "FDS_ERR_INTERNAL",
};

/* Array to map FDS events to strings. */
static char const* fds_evt_str[] =
{
    "FDS_EVT_INIT",
    "FDS_EVT_WRITE",
    "FDS_EVT_UPDATE",
    "FDS_EVT_DEL_RECORD",
    "FDS_EVT_DEL_FILE",
    "FDS_EVT_GC",
};

static struct
{
    bool delete_next;   //!< Delete next record.
    bool pending;       //!< Waiting for an fds FDS_EVT_DEL_RECORD event, to delete the next record.
} delete_all_t;

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

static const char *fds_err_str(ret_code_t ret);
static void fds_evt_handler(const fds_evt_t* fds_event);
static void wait_for_fds_ready();
static void wait_for_fds_write();
static void wait_for_fds_delete();

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

    err_code = fds_register(fds_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = fds_init();
    APP_ERROR_CHECK(err_code);

    wait_for_fds_ready();

    bsp_board_led_off(BSP_BOARD_LED_0);
    bsp_board_led_on(BSP_BOARD_LED_1);

    fds_stat_t stat = {0};
    err_code = fds_stat(&stat);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Found %d valid records.", stat.valid_records);
    NRF_LOG_INFO("Found %d dirty records (ready to be garbage collected).", stat.dirty_records);

    fds_record_desc_t desc = {0};

    fds_write_finished = false;
    err_code = fds_record_write(&desc, &dummy_record_t);
    if (err_code == FDS_ERR_NO_SPACE_IN_FLASH)
    {
        NRF_LOG_INFO("No space in flash, delete some records first");
    }
    else
    {
        APP_ERROR_CHECK(err_code);
    }

    while (!fds_write_finished)
    {
        sd_app_evt_wait();
    }

    NRF_LOG_INFO("Record written");

    err_code = fds_stat(&stat);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Found %d valid records.", stat.valid_records);
    NRF_LOG_INFO("Found %d dirty records (ready to be garbage collected).", stat.dirty_records);

    fds_find_token_t tok = {0};

    err_code = fds_record_find(CONFIG_FILE, CONFIG_REC_KEY, &desc, &tok);
    if (err_code == NRF_SUCCESS)
    {
        fds_flash_record_t config = {0};
        err_code = fds_record_open(&desc, &config);
        APP_ERROR_CHECK(err_code);

        configuration_t read_record;
        memcpy(&read_record, config.p_data, sizeof(configuration_t));

        NRF_LOG_INFO("Record read: %s", read_record.device_name);
    }
    else
    {
        APP_ERROR_CHECK(err_code);
    }

    bsp_board_led_off(BSP_BOARD_LED_1);
    bsp_board_led_on(BSP_BOARD_LED_2);

    while (true)
    {
        if (NRF_LOG_PROCESS() == false)
        {
            nrf_pwr_mgmt_run();
        }
    }
}

static const char* fds_err_str(ret_code_t ret)
{
    return err_str[ret - NRF_ERROR_FDS_ERR_BASE];
}

static void fds_evt_handler(const fds_evt_t* fds_event)
{
    if (fds_event->result == NRF_SUCCESS)
    {
        NRF_LOG_INFO(
            "Event: %s received (NRF_SUCCESS)",
            fds_evt_str[fds_event->id]
        );
    }
    else
    {
        NRF_LOG_INFO(
            "Event: %s received (%s)",
            fds_evt_str[fds_event->id],
            fds_err_str(fds_event->result)
        );
    }

    switch (fds_event->id)
    {
        case FDS_EVT_INIT:
            if (fds_event->result == NRF_SUCCESS)
            {
                fds_initialized = true;
            }
            break;

        case FDS_EVT_WRITE:
        {
            if (fds_event->result == NRF_SUCCESS)
            {
                NRF_LOG_INFO("Record ID:\t0x%04x",  fds_event->write.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    fds_event->write.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", fds_event->write.record_key);
                fds_write_finished = true;
            }
        } break;

        case FDS_EVT_DEL_RECORD:
        {
            if (fds_event->result == NRF_SUCCESS)
            {
                NRF_LOG_INFO("Record ID:\t0x%04x",  fds_event->del.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    fds_event->del.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", fds_event->del.record_key);
            }
            delete_all_t.pending = false;
        } break;

        default:
            break;
    }
}

static void wait_for_fds_ready()
{
    while (!fds_initialized)
    {
        sd_app_evt_wait();
    }
}

static void wait_for_fds_write()
{
    while (!fds_write_finished)
    {
        sd_app_evt_wait();
    }
}

static void wait_for_fds_delete()
{
    while (!fds_delete_finished)
    {
        sd_app_evt_wait();
    }
}