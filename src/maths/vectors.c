#include <math.h>
#include "maths/vectors.h"

t_vec_2f	vec_2f(float x, float y)
{
	return (t_vec_2f){ x, y };
}

t_vec_2f	vec_2f_add(t_vec_2f a, t_vec_2f b)
{
	return (t_vec_2f){ a.x + b.x, a.y + b.y };
}

t_vec_2f	vec_2f_sub(t_vec_2f a, t_vec_2f b)
{
	return (t_vec_2f){ a.x - b.x, a.y - b.y };
}

t_vec_2f	vec_2f_mul(t_vec_2f a, t_vec_2f b)
{
	return (t_vec_2f){ a.x * b.x, a.y * b.y };
}

t_vec_2f	vec_2f_div(t_vec_2f a, t_vec_2f b)
{
	return (t_vec_2f){ a.x / b.x, a.y / b.y };
}

float	vec_2f_dot(t_vec_2f a, t_vec_2f b)
{
	return a.x * b.x + a.y * b.y;
}

float	vec_2f_len(t_vec_2f a)
{
	return sqrtf(a.x * a.x + a.y * a.y);
}

t_vec_3f	vec_3f(float x, float y, float z)
{
	return (t_vec_3f){ x, y, z };
}

t_vec_3f	vec_3f_add(t_vec_3f a, t_vec_3f b)
{
	return (t_vec_3f){ a.x + b.x, a.y + b.y, a.z + b.z };
}

t_vec_3f	vec_3f_sub(t_vec_3f a, t_vec_3f b)
{
	return (t_vec_3f){ a.x - b.x, a.y - b.y, a.z - b.z };
}

t_vec_3f	vec_3f_mul(t_vec_3f a, t_vec_3f b)
{
	return (t_vec_3f){ a.x * b.x, a.y * b.y, a.z * b.z };
}

t_vec_3f	vec_3f_div(t_vec_3f a, t_vec_3f b)
{
	return (t_vec_3f){ a.x / b.x, a.y / b.y, a.z / b.z };
}

t_vec_3f	vec_3f_cross(t_vec_3f a, t_vec_3f b)
{
	return (t_vec_3f){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

float	vec_3f_dot(t_vec_3f a, t_vec_3f b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float	vec_3f_len(t_vec_3f a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

t_vec_3f	vec_3f_normalize(t_vec_3f a)
{
	float len = vec_3f_len(a);

	if (len == 0.0f)
		return a;

	return (t_vec_3f){ a.x / len, a.y / len, a.z / len };
}

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

float		vec_4f_dot(t_vec_4f a, t_vec_4f b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float		vec_4f_len(t_vec_4f a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}
