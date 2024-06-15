#ifndef JPOWER_V1_H
#define JPOWER_V1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

#define LEDS_NUMBER             1
#define LED_1                   NRF_GPIO_PIN_MAP(0,15)

#define LED_RED                 LED_1

#define LEDS_ACTIVE_STATE       0

#define LEDS_LIST               { LED_1 }

#define LEDS_INV_MASK           LEDS_MASK

#define BSP_LED_0               LED_1

#define BSP_LED_RED             0

#define BUTTONS_NUMBER          0

#define BUTTON_PULL             NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE    0

#define BUTTONS_LIST            { }

#define I2C_SCL_PIN             NRF_GPIO_PIN_MAP(0,13)
#define I2C_SDA_PIN             NRF_GPIO_PIN_MAP(0,20)

#define SPI_SCK_PIN             NRF_GPIO_PIN_MAP(0,22)
#define SPI_MISO_PIN            NRF_GPIO_PIN_MAP(0,24)
#define SPI_MOSI_PIN            NRF_GPIO_PIN_MAP(1,00)
#define SPI_CS_PIN              NRF_GPIO_PIN_MAP(1,04)

#define BATT_SENSE_PIN          NRF_GPIO_PIN_MAP(0,02)

#define WAKE_PIN                NRF_GPIO_PIN_MAP(0,30)

#ifdef __cplusplus
}
#endif

#endif // JPOWER_V1_H