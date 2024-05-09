#include "jpow_math.h"

#include <stdint.h>
#include <math.h>

axis_angle_t to_axis_angle(quaternion_t *q)
{
	axis_angle_t axis_angle;

	if (are_floats_equal(q->w, 0.0f))
	{
		axis_angle.angle = M_PI;
		axis_angle.x = q->x;
		axis_angle.y = q->y;
		axis_angle.z = q->z;
	}
	else if (are_floats_equal(q->w, 1.0f))
	{
		axis_angle.angle = 0.0f;
		axis_angle.x = 1.0f;
		axis_angle.y = 0.0f;
		axis_angle.z = 0.0f;
	}
	else
	{
		axis_angle.angle = 2.0f * acosf(q->w);
		axis_angle.x = q->x / sqrtf(1.0f - q->w * q->w);
		axis_angle.y = q->y / sqrtf(1.0f - q->w * q->w);
		axis_angle.z = q->z / sqrtf(1.0f - q->w * q->w);
	}

	return axis_angle;
}

euler_xyz_t to_euler_xyz(quaternion_t *q)
{
	euler_xyz_t euler;

	float halfMinusQySquared = 0.5f - q->y * q->y;
	euler.rx = atan2f(q->w * q->x + q->y * q->z, halfMinusQySquared - q->x * q->x);
	euler.ry = asinf(2.0f * (q->w * q->y - q->z * q->x));
	euler.rz = atan2f(q->w * q->z + q->x * q->y, halfMinusQySquared - q->z * q->z);

	return euler;
}

quaternion_t q_mul(
    const quaternion_t *const q1, 
    const quaternion_t *const q2)
{
	quaternion_t res =
	{
		.w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z,
        .x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y,
        .y = q1->w * q2->y - q1->x * q2->z + q1->y * q2->w + q1->z * q2->x,
        .z = q1->w * q2->z + q1->x * q2->y - q1->y * q2->x + q1->z * q2->w
	};

	return res;
}

extern quaternion_t q_rel_rot(
	const quaternion_t* const q1,
	const quaternion_t* const q2)
{
	quaternion_t conj_q1 =
	{
		.w = q1->w,
		.x = -q1->x,
		.y = -q1->y,
		.z = -q1->z
	};

	return q_mul(q2, &conj_q1);
}

float calc_anglular_vel(
    const quaternion_t *const q1, 
    const quaternion_t *const q2, 
    const float delta)
{
	quaternion_t qdelta = q_rel_rot(q1, q2);
	axis_angle_t aa = to_axis_angle(&qdelta);

	return aa.angle / delta;
}

//---------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float inv_sqrt(float x)
{
	union 
	{
		float f;
		int32_t i;
	} y;

	float halfx = 0.5f * x;

	y.f = x;
	y.i = 0x5f375a86 - (y.i >> 1);
	y.f = y.f * (1.5f - (halfx * y.f * y.f));
	y.f = y.f * (1.5f - (halfx * y.f * y.f));
	y.f = y.f * (1.5f - (halfx * y.f * y.f));

	return y.f;
}
