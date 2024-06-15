#include "jp_state.h"

#include "jp_state_srv.h"

static jp_state_t current_state = JP_STATE_STARTUP;

jp_state_t jp_state_get_current_state()
{
    return current_state;
}

void jp_state_change_state(jp_state_t new_state)
{
    current_state = new_state;
    jp_state_srv_update_current_state(new_state);
}