#ifndef SEEED_STUDIO_XIAO_SENSE_H
#define SEEED_STUDIO_XIAO_SENSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

#define LEDS_NUMBER             3
#define LED_1                   NRF_GPIO_PIN_MAP(0,26)
#define LED_2                   NRF_GPIO_PIN_MAP(0,30)
#define LED_3                   NRF_GPIO_PIN_MAP(0,06)
#define LED_4                   NRF_GPIO_PIN_MAP(0,17)

#define LED_RED                 LED_1
#define LED_GREEN               LED_2
#define LED_BLUE                LED_3
#define LED_PWR                 LED_4

#define LEDS_ACTIVE_STATE       0

#define LEDS_LIST               { LED_1, LED_2, LED_3 }

#define LEDS_INV_MASK           LEDS_MASK

#define BSP_LED_0               LED_1
#define BSP_LED_1               LED_2
#define BSP_LED_2               LED_3

#define BSP_LED_RED             0
#define BSP_LED_GREEN           1
#define BSP_LED_BLUE            2

#define BUTTONS_NUMBER          1

#define BUTTON_1                NRF_GPIO_PIN_MAP(0,3)
#define BUTTON_PULL             NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE    0

#define BUTTONS_LIST            { BUTTON_1 }

#define BSP_BUTTON_0            BUTTON_1

#define I2C_SCL_INT_PIN         NRF_GPIO_PIN_MAP(0,27)
#define I2C_SDA_INT_PIN         NRF_GPIO_PIN_MAP(0,7)
#define I2C_SDA_EXT_PIN         NRF_GPIO_PIN_MAP(0,4)
#define I2C_SCL_EXT_PIN         NRF_GPIO_PIN_MAP(0,5)

#define SPI_SCK_PIN             NRF_GPIO_PIN_MAP(1,13)
#define SPI_MISO_PIN            NRF_GPIO_PIN_MAP(1,14)
#define SPI_MOSI_PIN            NRF_GPIO_PIN_MAP(1,15)

#define UART_RX_PIN             NRF_GPIO_PIN_MAP(1,12)
#define UART_TX_PIN             NRF_GPIO_PIN_MAP(1,11)

#define DI0_PIN                 NRF_GPIO_PIN_MAP(0,2)
#define DI1_PIN                 NRF_GPIO_PIN_MAP(0,2)
#define DI2_PIN                 NRF_GPIO_PIN_MAP(0,28)
#define DI3_PIN                 NRF_GPIO_PIN_MAP(0,29)
#define DI4_PIN                 NRF_GPIO_PIN_MAP(0,4)
#define DI5_PIN                 NRF_GPIO_PIN_MAP(0,5)
#define DI6_PIN                 NRF_GPIO_PIN_MAP(1,11)
#define DI7_PIN                 NRF_GPIO_PIN_MAP(1,12)
#define DI8_PIN                 NRF_GPIO_PIN_MAP(1,13)
#define DI9_PIN                 NRF_GPIO_PIN_MAP(1,14)
#define DI10_PIN                NRF_GPIO_PIN_MAP(1,15)

#define DO0_PIN                 NRF_GPIO_PIN_MAP(0,2)
#define DO1_PIN                 NRF_GPIO_PIN_MAP(0,2)
#define DO2_PIN                 NRF_GPIO_PIN_MAP(0,28)
#define DO3_PIN                 NRF_GPIO_PIN_MAP(0,29)
#define DO4_PIN                 NRF_GPIO_PIN_MAP(0,4)
#define DO5_PIN                 NRF_GPIO_PIN_MAP(0,5)
#define DO6_PIN                 NRF_GPIO_PIN_MAP(1,11)
#define DO7_PIN                 NRF_GPIO_PIN_MAP(1,12)
#define DO8_PIN                 NRF_GPIO_PIN_MAP(1,13)
#define DO9_PIN                 NRF_GPIO_PIN_MAP(1,14)
#define DO10_PIN                NRF_GPIO_PIN_MAP(1,15)

#define AI0_PIN                 NRF_GPIO_PIN_MAP(0,2)
#define AI1_PIN                 NRF_GPIO_PIN_MAP(0,2)
#define AI2_PIN                 NRF_GPIO_PIN_MAP(0,28)
#define AI3_PIN                 NRF_GPIO_PIN_MAP(0,29)
#define AI4_PIN                 NRF_GPIO_PIN_MAP(0,4)
#define AI5_PIN                 NRF_GPIO_PIN_MAP(0,5)
#define AI6_PIN                 NRF_GPIO_PIN_MAP(1,11)
#define AI7_PIN                 NRF_GPIO_PIN_MAP(1,12)
#define AI8_PIN                 NRF_GPIO_PIN_MAP(1,13)
#define AI9_PIN                 NRF_GPIO_PIN_MAP(1,14)
#define AI10_PIN                NRF_GPIO_PIN_MAP(1,15)

#define DAC_PIN                 NRF_GPIO_PIN_MAP(0,2)

#define IMU_PWR_PIN             NRF_GPIO_PIN_MAP(1,8)
#define IMU_INT1_PIN            NRF_GPIO_PIN_MAP(0,11)
#define VBAT_ENABLE             NRF_GPIO_PIN_MAP(0,31)
#define CHRG_CUR_PIN            NRF_GPIO_PIN_MAP(0,13)
#define QSPI_CS_PIN             NRF_GPIO_PIN_MAP(0,25)

#define ADC_PWR_PIN             DO3_PIN

#ifdef __cplusplus
}
#endif

#endif // SEEED_STUDIO_XIAO_SENSE_H