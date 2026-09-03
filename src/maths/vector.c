#include <math.h>

#include "maths/vectors.h"

t_vec_4f	vec_4f(float x, float y, float z, float w)
{
	return (t_vec_4f){ x, y, z, w };
}

t_vec_4f	vec_4f_add(t_vec_4f a, t_vec_4f b)
{
	return (t_vec_4f){ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

t_vec_4f	vec_4f_sub(t_vec_4f a, t_vec_4f b)
{
	return (t_vec_4f){ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

t_vec_4f	vec_4f_mul(t_vec_4f a, t_vec_4f b)
{
	return (t_vec_4f){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

t_vec_4f	vec_4f_div(t_vec_4f a, t_vec_4f b)
{
	return (t_vec_4f){ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

t_vec_4f	vec_4f_cross(t_vec_4f a, t_vec_4f b)
{
	return (t_vec_4f){ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0 };
}

float	vec_4f_dot(t_vec_4f a, t_vec_4f b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float	vec4f_len(t_vec_4f a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}
