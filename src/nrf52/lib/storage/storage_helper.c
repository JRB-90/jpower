#include "storage_helper.h"

#include "nrf_log.h"
#include "nrf_sdh.h"

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

static bool fds_initialized = false;
static bool fds_write_finished = false;
static bool fds_delete_finished = false;

static const char* fds_err_str(ret_code_t ret);
static void fds_evt_handler(const fds_evt_t* fds_event);
static void wait_for_fds_ready();
static void wait_for_fds_write();
static void wait_for_fds_delete();

ret_code_t storage_init()
{
    ret_code_t err_code = fds_register(fds_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = fds_init();
    APP_ERROR_CHECK(err_code);

    wait_for_fds_ready();

    return err_code;
}

ret_code_t storage_write(const fds_record_t* record)
{
    fds_write_finished = false;
    fds_record_desc_t desc = {0};

    ret_code_t err_code = 
        fds_record_write(
            &desc, 
            record
        );

    if (err_code == FDS_ERR_NO_SPACE_IN_FLASH)
    {
        NRF_LOG_ERROR("No space in flash, delete some records first");
    }

    wait_for_fds_write();

    return err_code;
}

ret_code_t storage_read(
    uint16_t file_id,
    uint16_t record_key,
    fds_flash_record_t* read_record)
{
    fds_record_desc_t desc = {0};
    fds_find_token_t tok = {0};

    ret_code_t err_code = 
        fds_record_find(
            file_id, 
            record_key, 
            &desc, 
            &tok
        );

    if (err_code == NRF_SUCCESS)
    {
        
        err_code = fds_record_open(&desc, read_record);
        APP_ERROR_CHECK(err_code);
    }
    
    return err_code;
}

ret_code_t storage_delete(
    uint16_t file_id,
    uint16_t record_key)
{
    fds_delete_finished = false;
    fds_record_desc_t desc = {0};
    fds_find_token_t tok = {0};

    ret_code_t err_code = 
        fds_record_find(
            file_id, 
            record_key, 
            &desc, 
            &tok
        );

    if (err_code == NRF_SUCCESS)
    {
        err_code = fds_record_delete(&desc);
        APP_ERROR_CHECK(err_code);
        wait_for_fds_delete();
    }

    err_code = fds_gc();
    
    return err_code;
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
                fds_write_finished = true;
            }
        } break;

        case FDS_EVT_DEL_RECORD:
        {
            if (fds_event->result == NRF_SUCCESS)
            {
                fds_delete_finished = true;
            }
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