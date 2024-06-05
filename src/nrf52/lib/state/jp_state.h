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

typedef enum
{
    JP_STATE_ENTER_CALIBRATING      = 0,
    JP_STATE_ENTER_RUNNING          = 1,
} jp_enter_state_request_t;

extern ret_code_t jp_state_init();
extern jp_state_t jp_get_current_state();
extern void jp_change_state(jp_state_t new_state);

#endif // JP_STATE_H__