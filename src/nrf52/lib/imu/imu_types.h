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

typedef enum
{
    IMU_ACTIVITY_EVENT_SLEEP,
    IMU_ACTIVITY_EVENT_WAKE_UP,
} imu_activity_event_t;

typedef void (*activity_event_cb)(imu_activity_event_t event);

#endif // IMU_TYPES_H__