#include "rendering/rasterizer.h"
#include "buffers/framebuffer.h"
#include "buffers/vertexbuffer.h"
#include "maths/matrix.h"
#include "maths/vectors.h"
#include "maths/transform.h"
#include "objects/camera.h"
#include "objects/mesh.h"

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

static const t_vec_3f light_dir = { 0.453f, 0.2f, 0.9f };

void	draw(t_framebuffer *fb, t_triangle *triangle, uint32_t color)
{
	t_triangle	t;
	int			xmin, xmax, ymin, ymax;

	if (!triangle)
		return;

	t = *triangle;
	t.area = triangle->area;

	if (FEQ(t.area, 0.0f))
		return ;

	xmin = (int)fmaxf(0.0f, floorf(fminf(t.a.x, fminf(t.b.x, t.c.x))));
	xmax = (int)fminf((float)fb->width - 1, ceilf(fmaxf(t.a.x, fmaxf(t.b.x, t.c.x))));
	ymin = (int)fmaxf(0.0f, floorf(fminf(t.a.y, fminf(t.b.y, t.c.y))));
	ymax = (int)fminf((float)fb->height - 1, ceilf(fmaxf(t.a.y, fmaxf(t.b.y, t.c.y))));

	for (int y = ymin; y <= ymax; y++)
	{
		for (int x = xmin; x <= xmax; x++)
		{
			t_vec_2f	p;
			float		w0, w1, w2;
			int			idx;

			p.x = x + 0.5f;
			p.y = y + 0.5f;

			w0 = edge_function(t.b, t.c, p);
			w1 = edge_function(t.c, t.a, p);
			w2 = edge_function(t.a, t.b, p);

			if ((w0 >= 0 && w1 >= 0 && w2 >= 0 && t.area > 0)
				|| (w0 <= 0 && w1 <= 0 && w2 <= 0 && t.area < 0))
			{
				w0 /= t.area;
				w1 /= t.area;
				w2 /= t.area;

				float invw_pix = w0 * t.invw.x + w1 * t.invw.y + w2 * t.invw.z;

				idx = y * fb->width + x;
				if (invw_pix > fb->depth[idx])
				{
					fb->depth[idx] = invw_pix;
					fb->pixels[idx] = color;
				}
			}
		}
	}
}

static uint32_t	shade_face(t_vec_3f normal, uint32_t base_color)
{
	float dot;
	float ambient;
	float intensity;
	uint8_t r, g, b;

	dot = vec_3f_dot(vec_3f_normalize(normal), vec_3f_normalize(light_dir));
	ambient = 0.15f;
	intensity = ambient + (1.0f - ambient) * fmaxf(dot, 0.0f);
	intensity = fminf(intensity, 1.0f);

	r = (uint8_t)(((base_color >> 16) & 0xFF) * intensity);
	g = (uint8_t)(((base_color >> 8) & 0xFF) * intensity);
	b = (uint8_t)((base_color & 0xFF) * intensity);

	return ((r << 16) | (g << 8) | b);
}

void	draw_mesh(t_mesh *mesh, t_matrix_4f *mvp, t_camera *cam, t_framebuffer *fb)
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

		t_vec_3f normal = mesh->norms[i0];

		t_triangle	triangle =
		{
			.a =
			{
				out.x[i0],
				out.y[i0],
			},
			.b =
			{
				out.x[i1],
				out.y[i1],
			},
			.c =
			{
				out.x[i2],
				out.y[i2],
			},
			.invw =
			{
				out.invw[i0],
				out.invw[i1],
				out.invw[i2]
			}
		};

		float area = edge_function(triangle.a, triangle.b, triangle.c);

		if (area <= 0.0f)
			continue;

		triangle.area = area;

		uint32_t base_color = 0xFFFFFF;
		uint32_t color = shade_face(normal, base_color);

		draw(fb, &triangle, color);
	}
}
