#ifndef BLE_SRV_HELPER_H__
#define BLE_SRV_HELPER_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "ble_srv_common.h"

#define BLE_SRV_MAX_SERVICES            10  // Maximum defineable services
#define BLE_SRV_MAX_CHARACTERISTICS	    5   // Maximum characteristics per service
#define BLE_SRV_MAX_TOTAL_CHARACT		20  // Maximum characteristics definable overall
#define BLE_SRV_MAX_STRING_SIZE			64	// Maximum size of the String type structures
#define BLE_SRV_MAX_DATA_SIZE			16  // Maximum size of the service data type

typedef enum
{
    BLE_SRV_NONE    = 0b0000,
    BLE_SRV_READ    = 0b0001,
    BLE_SRV_WRITE   = 0b0010,
    BLE_SRV_NOTIFY  = 0b0100,
} ble_srv_char_access_t;

typedef struct
{
	char                        char_description[BLE_SRV_MAX_STRING_SIZE];
	uint16_t                    char_uuid;
	uint8_t                     char_access_rights;
	uint8_t	                    char_data_len;
	uint8_t	                    char_data_init[BLE_SRV_MAX_DATA_SIZE];
	bool                        (*on_read)(uint8_t* value);
	void                        (*on_write)(uint8_t* value);
} ble_srv_char_desc_t;

typedef struct
{
    const ble_srv_char_desc_t*  char_descriptor;
	ble_uuid_t					char_uuid;
	ble_gatts_char_md_t			char_md;
	ble_gatts_attr_md_t			char_attr;
	ble_gatts_attr_md_t			char_cccd;
	ble_gatts_attr_t			char_value;
	uint8_t						char_data[BLE_SRV_MAX_DATA_SIZE];
	ble_gatts_char_handles_t	char_handler;
} ble_srv_dyn_char_desc_t;

typedef struct
{
    char                        srv_description[BLE_SRV_MAX_STRING_SIZE];
    uint16_t                    srv_uuid;
    uint8_t		                srv_char_count;
    ble_srv_char_desc_t         srv_chars[BLE_SRV_MAX_CHARACTERISTICS];
    void                        (*on_connect)();
    void                        (*on_disconnect)();
} ble_srv_desc_t;

typedef struct
{
    const ble_srv_desc_t*       service_descriptor;
    ble_uuid_t*                 uuid;
    uint16_t		            service_handler;
    uint16_t		            conn_handle;
    ble_srv_dyn_char_desc_t     chars_handler[BLE_SRV_MAX_CHARACTERISTICS];
} ble_srv_dyn_desc_t;

extern ret_code_t ble_srv_init_dyn_service(ble_srv_dyn_desc_t* srv_desc);
extern ret_code_t ble_srv_update_dyn_char(
    ble_srv_dyn_desc_t* dyn_srv_desc,
    ble_srv_dyn_char_desc_t* dyn_char_desc,
    uint8_t* value
);
extern void ble_srv_on_event(
    ble_evt_t const* ble_evt, 
    void* context
);

#endif // BLE_SRV_HELPER_H__