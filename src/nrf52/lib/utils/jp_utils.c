#include "jp_utils.h"

#include <string.h>
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_drv_rng.h"

guid_t jp_utils_gen_new_guid()
{
    guid_t guid;
    memset(&guid, 0, sizeof(guid_t));

    nrf_drv_rng_block_rand(guid.guid128, GUID_SIZE_BYTES);

    return guid;
}
