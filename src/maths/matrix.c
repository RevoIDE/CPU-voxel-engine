#include "maths/matrix.h"
#include <immintrin.h>
#include <math.h>
#include <xmmintrin.h>

t_matrix_4f	matrix_4f_identity()
{
	t_matrix_4f mat = { 0 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			mat.m[i][j] = (i == j) ? 1.0f : 0.0f;

	return mat;
}

t_matrix_4f	matrix_4f_mult(t_matrix_4f a, t_matrix_4f b)
{
	t_matrix_4f r;

	__m128 b0 = _mm_load_ps(b.m[0]);
	__m128 b1 = _mm_load_ps(b.m[1]);
	__m128 b2 = _mm_load_ps(b.m[2]);
	__m128 b3 = _mm_load_ps(b.m[3]);

	for (int i = 0; i < 4; i++)
	{
		__m128 x = _mm_load_ps(a.m[i]);

		__m128 r0 = _mm_mul_ps(_mm_shuffle_ps(x, x, _MM_SHUFFLE(0, 0, 0, 0)), b0);
		__m128 r1 = _mm_fmadd_ps(_mm_shuffle_ps(x, x, _MM_SHUFFLE(1, 1, 1, 1)), b1, r0);
		__m128 r2 = _mm_fmadd_ps(_mm_shuffle_ps(x, x, _MM_SHUFFLE(2, 2, 2, 2)), b2, r1);
		__m128 r3 = _mm_fmadd_ps(_mm_shuffle_ps(x, x, _MM_SHUFFLE(3, 3, 3, 3)), b3, r2);

		_mm_store_ps(r.m[i], r3);
	}

	return r;
}

t_matrix_4f	matrix_4f_transpose(t_matrix_4f mat)
{
	t_matrix_4f result = { 0 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.m[j][i] = mat.m[i][j];

	return result;
}

t_matrix_4f	matrix_4f_translate(t_matrix_4f mat, float x, float y, float z)
{
	t_matrix_4f T = matrix_4f_identity();

	T.m[0][3] = x;
	T.m[1][3] = y;
	T.m[2][3] = z;

	return matrix_4f_mult(mat, T);
}

t_matrix_4f	matrix_4f_projection(float fov, float aspect, float near, float far)
{
	t_matrix_4f result = { 0 };

	float tan_half_fov = tanf(fov * 0.5f);

	result.m[0][0] = 1.0f / (aspect * tan_half_fov);
	result.m[1][1] = 1.0f / tan_half_fov;
	result.m[2][2] = -(far + near) / (far - near);
	result.m[2][3] = -1.0f;
	result.m[3][2] = -(2.0f * far * near) / (far - near);

	return result;
}
