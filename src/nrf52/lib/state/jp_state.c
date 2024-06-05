#include "jp_state.h"

#include "jp_state_srv.h"

static jp_state_t current_state;

ret_code_t jp_state_init()
{
    current_state = JP_STATE_STARTUP;

    return NRF_SUCCESS;
}

jp_state_t jp_get_current_state()
{
    return current_state;
}

void jp_change_state(jp_state_t new_state)
{
    current_state = new_state;
    jp_state_srv_update_current_state(new_state);
}