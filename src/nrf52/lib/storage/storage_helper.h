#ifndef STORAGE_HELPER_H__
#define STORAGE_HELPER_H__

#include "app_error.h"
#include "fds.h"

extern ret_code_t storage_init();
extern ret_code_t storage_write(const fds_record_t* record);
extern ret_code_t storage_write_async(const fds_record_t* record);
extern ret_code_t storage_read(
    uint16_t file_id, 
    uint16_t record_key,
    fds_flash_record_t* read_record
);
extern ret_code_t storage_delete(
    uint16_t file_id,
    uint16_t record_key
);

#endif // STORAGE_HELPER_H__