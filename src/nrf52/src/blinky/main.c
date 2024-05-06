#include <stdint.h>
#include <stdbool.h>
#include "boards.h"
#include "seeed_xiao_sense.h"
#include "nrf_delay.h"

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