#ifndef ANT_PLUS_SUB_H__
#define ANT_PLUS_SUB_H__

#include <stdint.h>
#include "app_error.h"

typedef struct
{
    uint16_t power;
    uint8_t cadence;
} ant_bike_power_data_t;

extern ret_code_t antsub_init();
extern void antsub_start_broadcasting();
extern void antsub_stop_broadcasting();
extern void antsub_update_power(const ant_bike_power_data_t* const state);

#endif // ANT_PLUS_SUB_H__