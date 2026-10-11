#include "rendering/rasterizer.h"
#include "buffers/framebuffer.h"
#include "buffers/vertexbuffer.h"
#include "maths/matrix.h"
#include "maths/vectors.h"
#include "maths/transform.h"
#include "objects/camera.h"
#include "objects/mesh.h"
#include "rendering/shaders.h"

#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wayland-util.h>

#define NEAR_W 1e-6

#define FEQ(a, b) (fabsf((a) - (b)) < 1e-6f)

static float	edge_function(t_vec_2f a, t_vec_2f b, t_vec_2f c)
{
	return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x));
}

void	draw(t_framebuffer *restrict fb, const t_triangle *restrict t,
			t_fragment_shader *shader_prgm, t_fragment_shader_data *data)
{
	const int	stride = fb->stride;
	int			xmin, xmax, ymin, ymax;

	xmin = (int)fmaxf(0.0f, floorf(fminf(t->a.x, fminf(t->b.x, t->c.x))));
	xmax = (int)fminf((float)fb->width - 1, ceilf(fmaxf(t->a.x, fmaxf(t->b.x, t->c.x))));
	ymin = (int)fmaxf(0.0f, floorf(fminf(t->a.y, fminf(t->b.y, t->c.y))));
	ymax = (int)fminf((float)fb->height - 1, ceilf(fmaxf(t->a.y, fmaxf(t->b.y, t->c.y))));

	if (xmin > xmax || ymin > ymax)
		return ;

	xmin &= ~7; // aligned on 8 pixels

	const float	inv_area = 1.0f / t->area;

	// edge increment
	const float	A0 = (t->c.y - t->b.y) * inv_area, B0 = (t->b.x - t->c.x) * inv_area;
	const float	A1 = (t->a.y - t->c.y) * inv_area, B1 = (t->c.x - t->a.x) * inv_area;
	const float	A2 = (t->b.y - t->a.y) * inv_area, B2 = (t->a.x - t->b.x) * inv_area;

	t_vec_2f	p = { xmin + 0.5f, ymin + 0.5f };
	float		w0_row = edge_function(t->b, t->c, p) * inv_area;
	float		w1_row = edge_function(t->c, t->a, p) * inv_area;
	float		w2_row = edge_function(t->a, t->b, p) * inv_area;

	const __m256	lane	= _mm256_setr_ps(0, 1, 2, 3, 4, 5, 6, 7);
	const __m256	zero	= _mm256_setzero_ps();
	const __m256	eight	= _mm256_set1_ps(8.0f);

	const __m256	off0	= _mm256_mul_ps(lane, _mm256_set1_ps(A0));
	const __m256	off1	= _mm256_mul_ps(lane, _mm256_set1_ps(A1));
	const __m256	off2	= _mm256_mul_ps(lane, _mm256_set1_ps(A2));

	const __m256	step0	= _mm256_set1_ps(A0 * 8.0f);
	const __m256	step1	= _mm256_set1_ps(A1 * 8.0f);
	const __m256	step2	= _mm256_set1_ps(A2 * 8.0f);

	const __m256	iw0		= _mm256_set1_ps(t->invw.x);
	const __m256	iw1		= _mm256_set1_ps(t->invw.y);
	const __m256	iw2		= _mm256_set1_ps(t->invw.z);

	const __m256	ua		= _mm256_set1_ps(t->uv_a.x);
	const __m256	ub		= _mm256_set1_ps(t->uv_b.x);
	const __m256	uc		= _mm256_set1_ps(t->uv_c.x);

	const __m256	va		= _mm256_set1_ps(t->uv_a.y);
	const __m256	vb		= _mm256_set1_ps(t->uv_b.y);
	const __m256	vc		= _mm256_set1_ps(t->uv_c.y);

	const __m256	xlim	= _mm256_set1_ps((float)xmax);
	const __m256	one		= _mm256_set1_ps(1.0f);

	for (int y = ymin; y <= ymax; y++)
	{
		__m256	w0 = _mm256_add_ps(_mm256_set1_ps(w0_row), off0);
		__m256	w1 = _mm256_add_ps(_mm256_set1_ps(w1_row), off1);
		__m256	w2 = _mm256_add_ps(_mm256_set1_ps(w2_row), off2);
		__m256	xs = _mm256_add_ps(_mm256_set1_ps((float)xmin), lane);

		float *restrict		depth = fb->depth + (size_t)y * stride;
		uint32_t *restrict	pix = fb->pixels + (size_t)y * stride;

		for (int x = xmin; x <= xmax; x += 8)
		{
			// cover
			// w0, w1, w2 >= 0 and x <= xmax
			__m256	mask = _mm256_and_ps(
				_mm256_and_ps(_mm256_cmp_ps(w0, zero, _CMP_GE_OQ),
							  _mm256_cmp_ps(w1, zero, _CMP_GE_OQ)),
				_mm256_and_ps(_mm256_cmp_ps(w2, zero, _CMP_GE_OQ),
							  _mm256_cmp_ps(xs, xlim, _CMP_LE_OQ))
			);

			if (_mm256_movemask_ps(mask))
			{
				__m256	iw = _mm256_fmadd_ps(w0, iw0,
							 _mm256_fmadd_ps(w1, iw1, _mm256_mul_ps(w2, iw2)));

				__m256	simd_depth = _mm256_load_ps(depth + x);

				mask = _mm256_and_ps(mask, _mm256_cmp_ps(iw, simd_depth, _CMP_GT_OQ));
				int	ma = _mm256_movemask_ps(mask);

				if (ma)
				{
					_mm256_store_ps(depth + x, _mm256_blendv_ps(simd_depth, iw, mask));

					__m256	w = _mm256_div_ps(one, iw);

					__m256	u = _mm256_mul_ps(_mm256_fmadd_ps(w0, ua, _mm256_fmadd_ps(w1, ub, _mm256_mul_ps(w2, uc))), w);
					__m256	v = _mm256_mul_ps(_mm256_fmadd_ps(w0, va, _mm256_fmadd_ps(w1, vb, _mm256_mul_ps(w2, vc))), w);

					_Alignas(32) float	us[8], vs[8], iws[8]; // memory aligned arrays

					_mm256_store_ps(us, u);
					_mm256_store_ps(vs, v);
					_mm256_store_ps(iws, w);

					while (ma)
					{
						int	i = __builtin_ctz(ma);

						ma &= ma -1;

						t_fragment_shader_in	in =
						{
							.x = x + i,
							.y = y,
							.invw = iws[i],
							.u = us[i],
							.v = vs[i],
							.normal = t->face_normal,
							.data = data
						};

						pix[x + i] = shader_prgm->shade(&in);
					}
				}
			}

			w0 = _mm256_add_ps(w0, step0);
			w1 = _mm256_add_ps(w1, step1);
			w2 = _mm256_add_ps(w2, step2);
			xs = _mm256_add_ps(xs, eight);
		}

		w0_row += B0;
		w1_row += B1;
		w2_row += B2;
	}
}

void	draw_mesh(t_mesh *mesh, t_matrix_4f *mvp, t_camera *cam, t_framebuffer *fb, t_fragment_shader *shader_prgm, t_fragment_shader_data *data)
{
	t_vertexbuffer_in	in =
	{
		.count = mesh->vertex_count,
		.x = mesh->x,
		.y = mesh->y,
		.z = mesh->z,
		.w = mesh->w
	};

	static t_vertexbuffer_out	out;

	out.count = mesh->vertex_count;
	vertex_buffer_out_update(&out, mesh->vertex_count);

	transform_vertices(
		&in, &out,
		mvp,
		fb->width, fb->height
	);

	for (uint32_t i = 0; i + 2 < mesh->index_count; i += 3)
	{
		uint32_t	i0 = mesh->indices[i + 0];
		uint32_t	i1 = mesh->indices[i + 1];
		uint32_t	i2 = mesh->indices[i + 2];

		if (i0 >= (uint32_t) out.count ||
			i1 >= (uint32_t) out.count ||
			i2 >= (uint32_t) out.count)
			continue;

		if (out.clipW[i0] < NEAR_W || out.clipW[i1] < NEAR_W || out.clipW[i2] < NEAR_W)
    		continue;

		const t_vec_2f a = { out.x[i0], out.y[i0] };
		const t_vec_2f b = { out.x[i1], out.y[i1] };
		const t_vec_2f c = { out.x[i2], out.y[i2] };

		const float area = edge_function(a, b, c);

		if (area <= 0.0f)
			continue;

		t_vec_3f normal = mesh->norms[i0];

		const t_vec_2f	uv0 = mesh->uvs ? mesh->uvs[i0] : (t_vec_2f){0};
		const t_vec_2f	uv1 = mesh->uvs ? mesh->uvs[i1] : (t_vec_2f){0};
		const t_vec_2f	uv2 = mesh->uvs ? mesh->uvs[i2] : (t_vec_2f){0};

		t_triangle	triangle =
		{
			.a = a,
			.b = b,
			.c = c,
			.invw =
			{
				out.invw[i0],
				out.invw[i1],
				out.invw[i2]
			},
			.uv_a =
			{
				uv0.x * out.invw[i0],
				uv0.y * out.invw[i0]
			},
			.uv_b =
			{
				uv1.x * out.invw[i1],
				uv1.y * out.invw[i1]
			},
			.uv_c = {
				uv2.x * out.invw[i2],
				uv2.y * out.invw[i2]
			},
			.face_normal = normal
		};

		triangle.area = area;

		draw(fb, &triangle, shader_prgm, data);
	}
}
