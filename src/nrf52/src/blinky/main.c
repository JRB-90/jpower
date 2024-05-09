#include <stdint.h>
#include <stdbool.h>
#include "boards.h"
#include "seeed_xiao_sense.h"
#include "nrf_delay.h"

#include "jpow_math.h"
#include "i2c_helper.h"
#include "spi_helper.h"
#include "Fusion.h"
#include "imu.h"

int main()
{
    bsp_board_init(BSP_INIT_LEDS);

    while (true)
    {
        for (int i = 0; i < LEDS_NUMBER; i++)
        {
            bsp_board_led_invert(i);
            nrf_delay_ms(500);
        }
    }
}