#ifndef ANT_PLUS_SUB_H__
#define ANT_PLUS_SUB_H__

#include <stdint.h>

typedef struct
{
    uint16_t power;
    uint8_t cadence;
} bike_power_data_t;

extern void antsub_init();
extern void antsub_update_power_cadence(
    uint16_t power,
    uint8_t cadence
);

#endif // ANT_PLUS_SUB_H__