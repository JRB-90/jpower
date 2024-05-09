#include "i2c_helper.h"

#include <string.h>

static volatile bool isXferDone = false;

static void i2c_event_handler(
    const nrf_drv_twi_evt_t* event, 
    void* context
);

ret_code_t i2c_init(
    const nrf_drv_twi_t* twi,
    uint32_t scl_pin,
    uint32_t sda_pin)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config =
    {
        .scl = scl_pin,
        .sda = sda_pin,
        .frequency = NRF_TWI_FREQ_400K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init = true,
        .hold_bus_uninit = false
    };

    err_code =
        nrf_drv_twi_init(
            twi,
            &twi_config,
            i2c_event_handler,
            NULL
        );

    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(twi);

    return NRF_SUCCESS;
}

ret_code_t i2c_write(
    const nrf_drv_twi_t* twi, 
    uint8_t device_addr, 
    uint8_t register_addr, 
    const uint8_t* data, 
    uint16_t length)
{
    ret_code_t err_code;

    uint8_t buffer[255] = {0};
    buffer[0] = register_addr;
    memcpy(&buffer[1], data, length);
    isXferDone = false;

    err_code = 
        nrf_drv_twi_tx(
            twi, 
            device_addr, 
            buffer, 
            sizeof(length), 
            false
        );

    APP_ERROR_CHECK(err_code);

    while (isXferDone == false) { };
    isXferDone = false;

   return err_code;
}

ret_code_t i2c_read(
    const nrf_drv_twi_t* twi, 
    uint8_t device_addr, 
    uint8_t register_addr, 
    uint8_t* data, 
    uint16_t length)
{
    ret_code_t err_code;

    isXferDone = false;
    err_code = 
        nrf_drv_twi_tx(
            twi, 
            device_addr, 
            &register_addr, 
            sizeof(register_addr), 
            true
        );

    APP_ERROR_CHECK(err_code);

    while (isXferDone == false) { };
    isXferDone = false;

    err_code = 
        nrf_drv_twi_rx(
            twi, 
            device_addr, 
            data, 
            length
        );

    APP_ERROR_CHECK(err_code);

    while (isXferDone == false) { };
    isXferDone = false;

    return err_code;
}

uint8_t i2c_read_uint8(
    const nrf_drv_twi_t* twi,
    uint8_t device_addr, 
    uint8_t register_addr)
{
    ret_code_t err_code;
    uint8_t data;

    err_code =
        i2c_read(
            twi,
            device_addr,
            register_addr,
            &data,
            sizeof(uint8_t)
        );

    APP_ERROR_CHECK(err_code);

    return data;
}

void i2c_write_uint8(
    const nrf_drv_twi_t* twi,
    uint8_t device_addr, 
    uint8_t register_addr,
    uint8_t value)
{
    ret_code_t err_code;

    err_code =
        i2c_write(
            twi,
            device_addr,
            register_addr,
            &value,
            sizeof(uint8_t)
        );

    APP_ERROR_CHECK(err_code);
}

static void i2c_event_handler(
    const nrf_drv_twi_evt_t* event, 
    void* context)
{
    switch (event->type)
    {
    case NRF_DRV_TWI_EVT_DONE:
        isXferDone = true;
        break;

    default:
        break;
    }
}
