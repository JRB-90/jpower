#include "spi_helper.h"

#include <string.h>
#include "nrf_gpio.h"

static volatile bool isXferDone = false;

static void spi_event_handler(
    const nrf_drv_spi_evt_t* event, 
    void* context
);

ret_code_t spi_init(
    const nrf_drv_spi_t* spiInstance,
    const nrf_drv_spi_config_t* const config)
{
    ret_code_t err_code;

    nrf_gpio_cfg(
        config->sck_pin,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_CONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_H0H1,
        NRF_GPIO_PIN_NOSENSE
    );

    nrf_gpio_cfg(
        config->mosi_pin,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_H0H1,
        NRF_GPIO_PIN_NOSENSE
    );

    err_code =
        nrf_drv_spi_init(
            spiInstance,
            config,
            spi_event_handler,
            NULL
        );

    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

ret_code_t spi_read(
    const nrf_drv_spi_t* spiInstance,
    uint8_t* rx_buffer,
    uint8_t rx_buffer_length)
{
    ret_code_t err_code;
    isXferDone = false;

    err_code = 
        nrf_drv_spi_transfer(
            spiInstance,
            NULL,
            0,
            rx_buffer,
            rx_buffer_length
        );

    while (isXferDone == false)
    {
        __WFE();
    }

    isXferDone = false;

    return err_code;
}

ret_code_t spi_write(
    const nrf_drv_spi_t* spiInstance,
    const uint8_t* tx_buffer,
    uint8_t tx_buffer_length)
{
    ret_code_t err_code;
    isXferDone = false;

    err_code = 
        nrf_drv_spi_transfer(
            spiInstance,
            tx_buffer,
            tx_buffer_length,
            NULL,
            0
        );

    while (isXferDone == false)
    {
        __WFE();
    }

    isXferDone = false;

    return err_code;
}

static void spi_event_handler(
    const nrf_drv_spi_evt_t* event, 
    void* context)
{
    switch (event->type)
    {
    case NRF_DRV_SPI_EVENT_DONE:
        isXferDone = true;
        break;

    default:
        break;
    }
}