#ifndef USB_SUBSYSTEM_H__
#define USB_SUBSYSTEM_H__

#include <stdint.h>
#include <stdbool.h>
#include "app_error.h"

extern ret_code_t usbsub_init();
extern bool usbsub_event_processing();
extern ret_code_t usbsub_send_string(const char* string);

#endif // USB_SUBSYSTEM_H__