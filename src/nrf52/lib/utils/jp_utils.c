#include "jp_utils.h"

#include <string.h>
#include "app_error.h"
#include "nrf_drv_rng.h"

guid_t jp_utils_gen_new_guid()
{
    guid_t guid;
    memset(&guid, 0, sizeof(guid_t));

    ret_code_t err_code = 
        nrf_drv_rng_rand(
            guid.guid128,
            GUID_SIZE_BYTES
        );
    APP_ERROR_CHECK(err_code);

    return guid;
}