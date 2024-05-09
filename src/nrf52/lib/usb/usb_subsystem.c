#include "usb_subsystem.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "bsp.h"
#include "app_error.h"
#include "app_util.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"

#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

#define LED_USB_RESUME              (BSP_BOARD_LED_0)
#define LED_CDC_ACM_OPEN            (BSP_BOARD_LED_1)
#define LED_CDC_ACM_RX              (BSP_BOARD_LED_2)
#define LED_CDC_ACM_TX              (BSP_BOARD_LED_3)
#define BTN_CDC_DATA_SEND           0
#define BTN_CDC_NOTIFY_SEND         1
#define BTN_CDC_DATA_KEY_RELEASE    (bsp_event_t)(BSP_EVENT_KEY_LAST + 1)

#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1

#define RX_BUF_SIZE             1

// ==== Private Funciton Definitions

static void usbd_user_ev_handler(app_usbd_event_type_t event);
static void cdc_acm_user_ev_handler(
    app_usbd_class_inst_t const* p_inst,
    app_usbd_cdc_acm_user_event_t event
);

// ==== Private Data

static char rx_buffer[RX_BUF_SIZE];
static char tx_buffer[NRF_DRV_USBD_EPSIZE];

APP_USBD_CDC_ACM_GLOBAL_DEF(
    app_cdc_acm,
    cdc_acm_user_ev_handler,
    CDC_ACM_COMM_INTERFACE,
    CDC_ACM_DATA_INTERFACE,
    CDC_ACM_COMM_EPIN,
    CDC_ACM_DATA_EPIN,
    CDC_ACM_DATA_EPOUT,
    APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);

// ==== Public Funciton Definitions

ret_code_t usbsub_init()
{
    ret_code_t err_code;

    static const app_usbd_config_t usbd_config = 
    {
        .ev_state_proc = usbd_user_ev_handler
    };

    app_usbd_serial_num_generate();

    err_code = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("USBD CDC ACM example started.");

    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&app_cdc_acm);
    err_code = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(err_code);

    if (USBD_POWER_DETECTION)
    {
        err_code = app_usbd_power_events_enable();
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        NRF_LOG_INFO("No USB power detection enabled\r\nStarting USB now");

        app_usbd_enable();
        app_usbd_start();
    }

    return NRF_SUCCESS;
}

bool usbsub_event_processing()
{
    return app_usbd_event_queue_process();
}

ret_code_t usbsub_send_string(const char* string)
{
    size_t size = 
        sprintf(
            tx_buffer, 
            "%s\r\n",
            string
        );

    return 
        app_usbd_cdc_acm_write(
            &app_cdc_acm, 
            tx_buffer,
            size
        );
}

// ==== Private Funciton Definitions

static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            bsp_board_led_off(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            bsp_board_led_on(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            bsp_board_leds_off();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            NRF_LOG_INFO("USB power detected");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            NRF_LOG_INFO("USB power removed");
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
            NRF_LOG_INFO("USB ready");
            app_usbd_start();
            break;
        default:
            break;
    }
}

static void cdc_acm_user_ev_handler(
    app_usbd_class_inst_t const* p_inst,
    app_usbd_cdc_acm_user_event_t event)
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            bsp_board_led_on(LED_CDC_ACM_OPEN);

            ret_code_t ret = 
                app_usbd_cdc_acm_read(
                    &app_cdc_acm,
                    rx_buffer,
                    RX_BUF_SIZE
                );
            UNUSED_VARIABLE(ret);
            break;
        }
        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            bsp_board_led_off(LED_CDC_ACM_OPEN);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
            bsp_board_led_invert(LED_CDC_ACM_TX);
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
            NRF_LOG_INFO("Bytes waiting: %d", app_usbd_cdc_acm_bytes_stored(p_cdc_acm));

            do
            {
                // Get amount of data transfered
                size_t size = app_usbd_cdc_acm_rx_size(p_cdc_acm);
                NRF_LOG_INFO("RX: size: %lu char: %c", size, rx_buffer[0]);

                // Fetch data until internal buffer is empty
                ret = 
                    app_usbd_cdc_acm_read(
                        &app_cdc_acm,
                        rx_buffer,
                        RX_BUF_SIZE
                    );
            } while (ret == NRF_SUCCESS);

            bsp_board_led_invert(LED_CDC_ACM_RX);
            break;
        }
        default:
            break;
    }
}