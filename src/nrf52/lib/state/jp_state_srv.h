#ifndef JP_STATE_SRV_H__
#define JP_STATE_SRV_H__

#include "jp_state.h"
#include "app_error.h"

// 128-bit base UUID
#define JP_STATE_SRV_BASE_UUID                  { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }
#define JP_STATE_SRV_SERVICE_UUID               0xB000
#define JP_STATE_SRV_STATE_REQ_CHAR_UUID        0xBB01
#define JP_STATE_SRV_CURRENT_STATE_CHAR_UUID    0xBB02
#define JP_STATE_SRV_CHAR_COUNT                 2
#define JP_STATE_SRV_STATE_REQ_INDEX            0
#define JP_STATE_SRV_CURRENT_STATE_INDEX        1

typedef enum
{
    JP_STATE_REQUEST_NONE                       = 0,
    JP_STATE_REQUEST_PUBLISH_STATE              = 1,
    JP_STATE_REQUEST_SWITCH_TO_RUNNING          = 2,
    JP_STATE_REQUEST_SWITCH_TO_CALIBRATE        = 3,
} jp_state_request_t;

typedef void (*jp_state_request_cb)(jp_state_request_t request);

extern ret_code_t jp_state_srv_init();
extern void jp_state_srv_register_enter_state_cb(jp_state_request_cb callback);
extern ret_code_t jp_state_srv_update_current_state(jp_state_t state);

#endif // JP_STATE_SRV_H__