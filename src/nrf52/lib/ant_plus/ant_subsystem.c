#include "ant_subsystem.h"

#include <string.h>
#include "app_error.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ant.h"
#include "ant_key_manager.h"
#include "ant_state_indicator.h"
#include "ant_bpwr.h"

// ========      Private Data Types       ========

typedef struct
{
    uint16_t instantanous_power;
    uint16_t accumulated_power;
    uint8_t instantanous_cadence;
    uint8_t event_count;
} bike_power_status_t;


// ======== Private Function Declarations ========

static void softdevice_setup();
static void profile_setup();
static void ant_bpwr_evt_handler(
    ant_bpwr_profile_t * p_profile, 
    ant_bpwr_evt_t event
);
static void ant_bpwr_calib_handler(
    ant_bpwr_profile_t * p_profile, 
    ant_bpwr_page1_data_t * p_page1
);

// ========         Private Data          ========

static ant_bpwr_profile_t m_ant_bpwr;
static bike_power_status_t power_status = { 0 };

BPWR_SENS_CHANNEL_CONFIG_DEF(
    m_ant_bpwr,
    BPWR_CHANNEL_NUM,
    CHAN_ID_TRANS_TYPE,
    CHAN_ID_DEV_NUM,
    ANTPLUS_NETWORK_NUM
);

BPWR_SENS_PROFILE_CONFIG_DEF(
    m_ant_bpwr,
    (ant_bpwr_torque_t)(SENSOR_TYPE),
    ant_bpwr_calib_handler,
    ant_bpwr_evt_handler
);

NRF_SDH_ANT_OBSERVER(
    m_ant_observer,
    ANT_BPWR_ANT_OBSERVER_PRIO,
    ant_bpwr_sens_evt_handler, 
    &m_ant_bpwr
);

// ======== Public Function Definitions ========

void antsub_init()
{
    ret_code_t err_code =
        ant_state_indicator_init(
            m_ant_bpwr.channel_number,
            BPWR_SENS_CHANNEL_TYPE
        );
    
    APP_ERROR_CHECK(err_code);

    softdevice_setup();
    profile_setup();
}

void antsub_update_power(const bike_power_data_t* const state)
{
    power_status.instantanous_power = state->power;
    power_status.accumulated_power += state->power;
    power_status.instantanous_cadence = state->cadence;
    power_status.event_count++;
}

// ======== Private Function Definitions ========

static void softdevice_setup()
{
    ret_code_t err_code = nrf_sdh_ant_enable();
    APP_ERROR_CHECK(err_code);

    err_code = ant_plus_key_set(ANTPLUS_NETWORK_NUM);
    APP_ERROR_CHECK(err_code);
}

static void profile_setup()
{
    ret_code_t err_code = 
        ant_bpwr_sens_init(
            &m_ant_bpwr,
            BPWR_SENS_CHANNEL_CONFIG(m_ant_bpwr),
            BPWR_SENS_PROFILE_CONFIG(m_ant_bpwr)
        );

    APP_ERROR_CHECK(err_code);

    // fill manufacturer's common data page.
    m_ant_bpwr.page_80 = 
        ANT_COMMON_page80(
            BPWR_HW_REVISION,
            BPWR_MANUFACTURER_ID,
            BPWR_MODEL_NUMBER
        );

    // fill product's common data page.
    m_ant_bpwr.page_81 = 
        ANT_COMMON_page81(
            BPWR_SW_REVISION_MAJOR,
            BPWR_SW_REVISION_MINOR,
            BPWR_SERIAL_NUMBER
        );

    m_ant_bpwr.BPWR_PROFILE_auto_zero_status = ANT_BPWR_AUTO_ZERO_OFF;

    err_code = ant_bpwr_sens_open(&m_ant_bpwr);
    APP_ERROR_CHECK(err_code);

    err_code = ant_state_indicator_channel_opened();
    APP_ERROR_CHECK(err_code);
}

static void ant_bpwr_evt_handler(
    ant_bpwr_profile_t * p_profile,
    ant_bpwr_evt_t event)
{
    nrf_pwr_mgmt_feed();

    switch (event)
    {
        case ANT_BPWR_PAGE_16_UPDATED:
            p_profile->page_16.update_event_count = power_status.event_count;
            p_profile->page_16.instantaneous_power = power_status.instantanous_power;
            p_profile->page_16.accumulated_power = power_status.accumulated_power;
            p_profile->common.instantaneous_cadence = power_status.instantanous_cadence;
            p_profile->page_16.pedal_power.items.distribution = 0;
            p_profile->page_16.pedal_power.items.differentiation = 0;
            power_status.event_count = 0;
            power_status.accumulated_power = 0;
            break;

        default:
            break;
    }
}

static void ant_bpwr_calib_handler(
    ant_bpwr_profile_t * p_profile, 
    ant_bpwr_page1_data_t * p_page1)
{
    switch (p_page1->calibration_id)
    {
        case ANT_BPWR_CALIB_ID_MANUAL:
            m_ant_bpwr.BPWR_PROFILE_calibration_id      = ANT_BPWR_CALIB_ID_MANUAL_SUCCESS;
            m_ant_bpwr.BPWR_PROFILE_general_calib_data  = CALIBRATION_DATA;
            break;

        case ANT_BPWR_CALIB_ID_AUTO:
            m_ant_bpwr.BPWR_PROFILE_calibration_id      = ANT_BPWR_CALIB_ID_MANUAL_SUCCESS;
            m_ant_bpwr.BPWR_PROFILE_auto_zero_status    = p_page1->auto_zero_status;
            m_ant_bpwr.BPWR_PROFILE_general_calib_data  = CALIBRATION_DATA;
            break;

        case ANT_BPWR_CALIB_ID_CUSTOM_REQ:
            m_ant_bpwr.BPWR_PROFILE_calibration_id      = ANT_BPWR_CALIB_ID_CUSTOM_REQ_SUCCESS;
            memcpy(
                m_ant_bpwr.BPWR_PROFILE_custom_calib_data, 
                p_page1->data.custom_calib,
                sizeof(m_ant_bpwr.BPWR_PROFILE_custom_calib_data)
            );
            break;

        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE:
            m_ant_bpwr.BPWR_PROFILE_calibration_id      = ANT_BPWR_CALIB_ID_CUSTOM_UPDATE_SUCCESS;
            memcpy(
                m_ant_bpwr.BPWR_PROFILE_custom_calib_data, 
                p_page1->data.custom_calib,
                sizeof(m_ant_bpwr.BPWR_PROFILE_custom_calib_data)
            );
            break;

        default:
            break;
    }
}

