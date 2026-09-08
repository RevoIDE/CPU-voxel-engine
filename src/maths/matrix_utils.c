#include <immintrin.h>

#include "maths/matrix_utils.h"
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

/// TODO: create a batch version of matrix_4f_transform_vec_4f to handle transformations of multiple vectors at once
