#ifndef BATTERY_H__
#define BATTERY_H__

#include <stdint.h>
#include "app_error.h"

extern ret_code_t battery_init();
extern void battery_update();
extern int16_t battery_get_level_mv();
extern float battery_get_level_v();
extern uint8_t battery_get_level_percentage();

#endif // BATTERY_H__