#ifndef JP_STATE_H__
#define JP_STATE_H__

#include "app_error.h"

typedef enum
{
    JP_STATE_STARTUP                = 0,
    JP_STATE_INITIALISED            = 1,
    JP_STATE_RUNNING                = 2,
    JP_STATE_CALIBRATING            = 3,
} jp_state_t;

extern jp_state_t jp_state_get_current_state();
extern void jp_state_change_state(jp_state_t new_state);

#endif // JP_STATE_H__