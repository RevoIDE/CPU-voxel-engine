#include <immintrin.h>
#include <stddef.h>
#include <xmmintrin.h>

#include "maths/matrix_utils.h"
#include "err_handler.h"
#include "maths/matrix.h"
#include "maths/vectors.h"

void	matrix_4f_transform_vec_4f(t_vec_4f *restrict result, const t_vec_4f *restrict vec, const t_matrix_4f *restrict mat)
{
	// NOTE: TRANSPOSE the matrix before using it,
	// the needed matrix is a column-major matrix, not a row-major one
	//
	// Using FMA instructions (~ 4 - 5 cycles)
	__m128 vx = _mm_set1_ps(vec->x);
	__m128 vy = _mm_set1_ps(vec->y);
	__m128 vz = _mm_set1_ps(vec->z);
	__m128 vw = _mm_set1_ps(vec->w);

	__m128 res = _mm_mul_ps(vx, _mm_load_ps(mat->m[0]));

	res = _mm_fmadd_ps(vy, _mm_load_ps(mat->m[1]), res);
	res = _mm_fmadd_ps(vz, _mm_load_ps(mat->m[2]), res);
	res = _mm_fmadd_ps(vw, _mm_load_ps(mat->m[3]), res);

	_mm_store_ps(&result->x, res);
}

void	matrix_4f_transform_vec4f_batch(t_vec_4f *restrict result, const t_vec_4f *restrict vec, const t_matrix_4f *restrict mat, size_t count)
{
	// same MVP for all vectors
	// BEFORE USING IT NOTE:
	// count need to be a multiple of 4 for SIMD to be efficient (count % 4 == 0)
	// if you have a non-multiple-of-4 count, use the non-batch version for the few remaining vectors
	// the batch version is faster for large counts, do not use it if you have a small number of vectors
	// otherwise, it will be slower than the non-batch version
	//

	if (count % 4 != 0)
	{
		WARN("[MATHS] provided count must be a multiple of 4 for SIMD");
		return;
	}

	__m128	r0 = _mm_load_ps(mat->m[0]);
	__m128	r1 = _mm_load_ps(mat->m[1]);
	__m128	r2 = _mm_load_ps(mat->m[2]);
	__m128	r3 = _mm_load_ps(mat->m[3]);

	__m128	m00 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0));
	__m128	m01 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(1, 1, 1, 1));
	__m128	m02 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(2, 2, 2, 2));
	__m128	m03 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(3, 3, 3, 3));

	__m128	m10 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128	m11 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1));
	__m128	m12 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 2, 2, 2));
	__m128	m13 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 3, 3, 3));

	__m128	m20 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 0, 0, 0));
	__m128	m21 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 1, 1, 1));
	__m128	m22 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2));
	__m128	m23 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 3, 3, 3));

	__m128	m30 = _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(0, 0, 0, 0));
	__m128	m31 = _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(1, 1, 1, 1));
	__m128	m32 = _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(2, 2, 2, 2));
	__m128	m33 = _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(3, 3, 3, 3));

	// transform each vector
	for (size_t i = 0; i < count; i += 4)
	{
		__m128	v0 = _mm_load_ps(&vec[i + 0].x); // +0 for visual align
		__m128	v1 = _mm_load_ps(&vec[i + 1].x);
		__m128	v2 = _mm_load_ps(&vec[i + 2].x);
		__m128	v3 = _mm_load_ps(&vec[i + 3].x);

		// reach SoA temporary
		_MM_TRANSPOSE4_PS(v0, v1, v2, v3);

		__m128	rx = _mm_fmadd_ps(v3, m30, _mm_fmadd_ps(v2, m20, _mm_fmadd_ps(v1, m10, _mm_mul_ps(v0, m00))));
		__m128	ry = _mm_fmadd_ps(v3, m31, _mm_fmadd_ps(v2, m21, _mm_fmadd_ps(v1, m11, _mm_mul_ps(v0, m01))));
		__m128	rz = _mm_fmadd_ps(v3, m32, _mm_fmadd_ps(v2, m22, _mm_fmadd_ps(v1, m12, _mm_mul_ps(v0, m02))));
		__m128	rw = _mm_fmadd_ps(v3, m33, _mm_fmadd_ps(v2, m23, _mm_fmadd_ps(v1, m13, _mm_mul_ps(v0, m03))));

		// reach AoS
		_MM_TRANSPOSE4_PS(rx, ry, rz, rw);

		_mm_store_ps(&result[i + 0].x, rx); // visual align
		_mm_store_ps(&result[i + 1].x, ry);
		_mm_store_ps(&result[i + 2].x, rz);
		_mm_store_ps(&result[i + 3].x, rw);
	}
}

/// TODO: create a batch version of matrix_4f_transform_vec_4f to handle transformations of multiple vectors at once
