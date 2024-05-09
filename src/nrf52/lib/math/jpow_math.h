#ifndef JPOW_MATH_H__
#define JPOW_MATH_H__

#include <stdbool.h>
#include <float.h>

// =====   Defines    =====

#define M_PI        3.14159265358979323846

// ===== Common Types =====

typedef struct
{
	float x;
	float y;
	float z;
} point_t;

typedef struct
{
	float w; // w
	float x; // x
	float y; // y
	float z; // z
} quaternion_t;

typedef struct
{
    float x;
    float y;
    float z;
    float angle;
} axis_angle_t;

typedef struct
{
    float rx;
    float ry;
    float rz;
} euler_xyz_t;

typedef struct
{
    float ax;
    float ay;
    float az;
} accel_data_t;

typedef struct
{
    float gx;
    float gy;
    float gz;
} gyro_data_t;

// ===== Functions =====

extern axis_angle_t to_axis_angle(quaternion_t *q);
extern euler_xyz_t to_euler_xyz(quaternion_t *q);
extern quaternion_t q_mul(
    const quaternion_t *const q1, 
    const quaternion_t *const q2
);
extern quaternion_t q_rel_rot(
    const quaternion_t* const q1,
    const quaternion_t* const q2
);
extern float calc_anglular_vel(
    const quaternion_t *const q1, 
    const quaternion_t *const q2, 
    const float delta
);
extern float inv_sqrt(float x);

inline float to_rad(const float degrees)
{
    return degrees * ((float) M_PI / 180.0f);
}

inline float to_deg(const float radians)
{
    return radians * (180.0f / (float) M_PI);
}

inline bool are_floats_equal(const float f1, const float f2)
{
    // return
    //     f1 < (f2 + FLT_EPSILON) &&
    //     f1 > (f2 - FLT_EPSILON);

    return
        f1 < (f2 + 0.001f) &&
        f1 > (f2 - 0.001f);
}

#endif // JPOW_MATH_H__