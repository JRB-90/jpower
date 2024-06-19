#ifndef IMU_TYPES_H__
#define IMU_TYPES_H__

#include <stdint.h>
#include "Fusion.h"

typedef struct
{
    float accel[3];
    float gyro[3];
} imu_reading_t;

typedef struct
{
    float angular_velocity;
    uint16_t cadence;
} pedal_state_t;

typedef struct
{
    float time;
    FusionQuaternion attitude;
} timestamped_reading_t;

#endif // IMU_TYPES_H__