#include "cadence.h"

#define ANG_TO_RPM_RATIO    0.16666666f
#define EPS_F               0.001f

static FusionAhrsSettings settings =
{
    .convention = FusionConventionNwu,
    .gain = 0.5f,
    .gyroscopeRange = 2000.0f,
    .accelerationRejection = 90.0f,
    .magneticRejection = 90.0f,
    .recoveryTriggerPeriod = 0.25f,
};

static FusionAhrs ahrs                      = { 0 };
static timestamped_reading_t last_hi_speed  = { 0.0f, {{ 1.0f, 0.0f, 0.0f, 0.0f }}};
static timestamped_reading_t last_lo_speed  = { 0.0f, {{ 1.0f, 0.0f, 0.0f, 0.0f }}};
static pedal_state_t current_pedal_state    = { 0.0f, 0 };
static float total_time_s                   = 0.0f;

static pedal_state_t calculate_pedal_state(
    float time_delta_s,
    const FusionQuaternion attitude_start,
    const FusionQuaternion attitude_end
);

ret_code_t cadence_init()
{
    FusionAhrsInitialise(&ahrs);
    FusionAhrsSetSettings(&ahrs, &settings);

    return NRF_SUCCESS;
}

void cadence_update(
    float time_delta_s,
    const imu_reading_t* imu_reading)
{
    total_time_s += time_delta_s;

    FusionVector accel =
    {{
        imu_reading->accel[0],
        imu_reading->accel[1],
        imu_reading->accel[2]
    }};

    FusionVector gyro =
    {{
        imu_reading->gyro[0],
        imu_reading->gyro[1],
        imu_reading->gyro[2]
    }};

    FusionAhrsUpdateNoMagnetometer(
        &ahrs,
        gyro,
        accel,
        time_delta_s
    );

    last_hi_speed.attitude = FusionAhrsGetQuaternion(&ahrs);
    last_hi_speed.time = total_time_s;

    current_pedal_state =
        calculate_pedal_state(
            total_time_s - last_lo_speed.time,
            last_lo_speed.attitude,
            last_hi_speed.attitude
        );

    last_lo_speed.attitude = last_hi_speed.attitude;
    last_lo_speed.time = last_hi_speed.time;
}

void cadence_get_attitude(FusionQuaternion* attitude)
{
    *attitude = last_hi_speed.attitude;
}

void cadence_get_pedal_state(pedal_state_t* pedal_state)
{
    *pedal_state = current_pedal_state;
}

static pedal_state_t calculate_pedal_state(
    float time_delta_s,
    const FusionQuaternion attitude_start,
    const FusionQuaternion attitude_end)
{
    FusionQuaternion conj =
	{{
		attitude_start.element.w,
		-attitude_start.element.x,
        -attitude_start.element.y,
        -attitude_start.element.z,
	}};

	FusionQuaternion relative = 
        FusionQuaternionMultiply(
            attitude_end, 
            conj
        );

    float angle = 0.0f;

    if (relative.element.w <= (0.0f + EPS_F))
    {
        angle = M_PI;
    }
    else if (relative.element.w >= (1.0f - EPS_F))
    {
        angle = 0.0f;
    }
    else
    {
        angle = 2.0f * acosf(relative.element.w);
    }

    float angular_velocity = FusionRadiansToDegrees(angle) / time_delta_s;

    pedal_state_t state =
    {
        .angular_velocity = angular_velocity,
        .cadence = (uint16_t)fabsf(angular_velocity * ANG_TO_RPM_RATIO)
    };

    return state;
}
