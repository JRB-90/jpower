#ifndef CADENCE_H__
#define CADENCE_H__

#include <stdint.h>
#include "app_error.h"
#include "imu_types.h"
#include "Fusion.h"

extern ret_code_t cadence_init();
extern void cadence_update(
    float time_delta_s,
    const imu_reading_t* imu_reading
);
extern void cadence_get_attitude(FusionQuaternion* attitude);
extern void cadence_get_pedal_state(pedal_state_t* pedal_state);

#endif // CADENCE_H__