#include "buffers/vertexbuffer.h"
#include "err_handler.h"
#include "maths/matrix.h"
#include <immintrin.h>

void	transform_vertices(t_vertexbuffer_in *in, t_vertexbuffer_out *out, t_matrix_4f *mvp, int fbwidth, int fbheight)
{
	__m256	row0[4];
	__m256	row1[4];
	__m256	row3[4];

	// assert on input alignment (32 byte aligned)
	ASSERT_ALIGNED32(in->x);
	ASSERT_ALIGNED32(in->y);
	ASSERT_ALIGNED32(in->z);
	ASSERT_ALIGNED32(in->w);

	// mvp broadcast on lanes
	for (int c = 0; c < 4; c++)
	{
		row0[c] = _mm256_set1_ps(mvp->m[0][c]);
		row1[c] = _mm256_set1_ps(mvp->m[1][c]);
		row3[c] = _mm256_set1_ps(mvp->m[3][c]);
	}

	const __m256	one 	= _mm256_set1_ps(1.0f);
	const __m256	halfW 	= _mm256_set1_ps(fbwidth  * 0.5f);
	const __m256	halfH 	= _mm256_set1_ps(fbheight * 0.5f);

	const int	scount = in->count & ~7; // greatest 8 multiple

	int	i = 0;

	for (; i < scount; i += 8)
	{
		// load vectors
		__m256 vx = _mm256_load_ps(&in->x[i]);
		__m256 vy = _mm256_load_ps(&in->y[i]);
		__m256 vz = _mm256_load_ps(&in->z[i]);
		__m256 vw = _mm256_load_ps(&in->w[i]);

		// compute clip space coordinates
		__m256 clipX = 	_mm256_fmadd_ps	(vx, row0[0],
						_mm256_fmadd_ps	(vy, row0[1],
						_mm256_fmadd_ps	(vz, row0[2],
						_mm256_mul_ps	(vw, row0[3]))));

		// little word about clipZ
		// 		clipZ is intentionally omitted, the zbuffer is directly using invw
		// 		with a reversedZ (far swapped with near)
		// 		it economises on memory bandwidth and SIMD computation
		// 		and avoid near plane Z imprecision
		//

		__m256 clipY = 	_mm256_fmadd_ps	(vx, row1[0],
						_mm256_fmadd_ps	(vy, row1[1],
						_mm256_fmadd_ps	(vz, row1[2],
						_mm256_mul_ps	(vw, row1[3]))));

		__m256 clipW = 	_mm256_fmadd_ps	(vx, row3[0],
						_mm256_fmadd_ps	(vy, row3[1],
						_mm256_fmadd_ps	(vz, row3[2],
						_mm256_mul_ps	(vw, row3[3]))));

		// compute invW
		__m256	invW = _mm256_div_ps(one, clipW);

		__m256	ndcX = _mm256_mul_ps(clipX, invW);
		__m256	ndcY = _mm256_mul_ps(clipY, invW);

		__m256	screenX = _mm256_fmadd_ps(ndcX, halfW, halfW);
		__m256	screenY = _mm256_fnmadd_ps(ndcY, halfH, halfH);

		// storing
		_mm256_storeu_ps(&out->invw[i], invW);

		_mm256_storeu_ps(&out->x[i], screenX);
		_mm256_storeu_ps(&out->y[i], screenY);
	}

	// remaining vertices (scalar fallback)
	for (; i < in->count; i++)
	{
		float x		= in->x[i], y = in->y[i], z = in->z[i], w = in->w[i];

		float cx	= mvp->m[0][0] * x + mvp->m[0][1] * y + mvp->m[0][2] * z + mvp->m[0][3] * w;
		float cy	= mvp->m[1][0] * x + mvp->m[1][1] * y + mvp->m[1][2] * z + mvp->m[1][3] * w;
		float cw	= mvp->m[3][0] * x + mvp->m[3][1] * y + mvp->m[3][2] * z + mvp->m[3][3] * w;

		float iw 	= 1.0f / cw;

		out->invw[i] = iw;

		out->x[i] = cx * iw * (fbwidth * 0.5f) + fbwidth * 0.5f;
		out->y[i] = -cy * iw * (fbheight * 0.5f) + fbheight * 0.5f;
	}
}
