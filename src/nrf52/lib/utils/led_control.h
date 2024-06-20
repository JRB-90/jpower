#ifndef LED_CONTROL_H__
#define LED_CONTROL_H__

typedef enum
{
    LED_STATE_OFF,
    LED_STATE_SOLID,
    LED_STATE_SLOW_PULSE,
    LED_STATE_FAST_PULSE,
} led_state_t;

extern void led_control_update_10ms();
extern void led_control_set(led_state_t led_state);

#endif // LED_CONTROL_H__