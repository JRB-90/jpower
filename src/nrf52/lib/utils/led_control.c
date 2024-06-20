#include "led_control.h"

#include <stdint.h>
#include "jpower_v1.h"
#include "bsp.h"

#define LED_IDX                 BSP_LED_0
#define DESIRED_SLOW_HZ         0.25
#define DESIRED_FAST_HZ         2
#define COUNTER_TRIGGER_SLOW    (100 / DESIRED_SLOW_HZ) / 2
#define COUNTER_TRIGGER_FAST    (100 / DESIRED_FAST_HZ) / 2

static led_state_t current_led_state = LED_STATE_OFF;
static uint32_t counter = 0;

void led_control_update_10ms()
{
    if (current_led_state == LED_STATE_OFF)
    {
        bsp_board_led_off(LED_IDX);
    }
    else if(current_led_state == LED_STATE_SOLID)
    {
        bsp_board_led_on(LED_IDX);
    }
    else if (current_led_state == LED_STATE_SLOW_PULSE &&
             counter >= COUNTER_TRIGGER_SLOW)
    {
        counter = 0;
        bsp_board_led_invert(LED_IDX);
    }
    else if (current_led_state == LED_STATE_FAST_PULSE &&
             counter >= COUNTER_TRIGGER_FAST)
    {
        counter = 0;
        bsp_board_led_invert(LED_IDX);
    }
    else
    {
        counter++;
    }
}

void led_control_set(led_state_t led_state)
{
    current_led_state = led_state;
    counter = 0;
}