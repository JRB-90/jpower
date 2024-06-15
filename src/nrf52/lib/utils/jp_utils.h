#ifndef JP_UTILS_H__
#define JP_UTILS_H__

#include <stdint.h>

#define GUID_SIZE_BYTES         16

typedef struct
{
    uint8_t guid128[GUID_SIZE_BYTES]; // Little-Endian UUID bytes
} guid_t;

extern guid_t jp_utils_gen_new_guid();

#endif // JP_UTILS_H__