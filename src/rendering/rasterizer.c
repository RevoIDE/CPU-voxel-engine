#include "rendering/rasterizer.h"
#include "buffers/framebuffer.h"
#include "maths/vectors.h"

#include <math.h>
#include <stdint.h>
#include <wctype.h>

static inline void	vec_swap(t_vec_3f *v1, t_vec_3f *v2)
{
	t_vec_3f	tmp;

	tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

static inline void	float_swap(float *v1, float *v2)
{
	float	tmp;

	tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

float	interpolate_x(t_vec_3f *p1, t_vec_3f *p2, int y)
{
	float	t;

	t = (y - p1->y) / (p2->y - p1->y);

	return (p1->x + t * (p2->x - p1->x));
}

void	draw(t_framebuffer *fb, t_triangle *triangle, uint32_t color)
{
	t_triangle	t;
	int			y;
	float		x1, x2;

	if (!triangle)
		return;

	t = *triangle;

	if (t.a.y > t.b.y)
		vec_swap(&t.a, &t.b);

	if (t.b.y > t.c.y)
		vec_swap(&t.b, &t.c);

	if (t.a.y > t.b.y)
		vec_swap(&t.a, &t.b);

	y = fmaxf(t.a.y, 0);

	while (y <= t.c.y && y < (int)fb->height)
	{
		if (t.a.y == t.c.y)
			break ;

		if (y < t.b.y && t.a.y != t.b.y)
			x1 = interpolate_x(&t.a, &t.b, y);
		else if (t.b.y != t.c.y)
			x1 = interpolate_x(&t.b, &t.c, y);
		else
			x1 = t.b.x;

		x2 = interpolate_x(&t.a, &t.c, y);

		if (x1 > x2)
			float_swap(&x1, &x2);

		x1 = fmaxf(x1, 0);
		x2 = fminf(x2, (float)fb->width - 1);

		for (int x = ceilf(x1); x <= floorf(x2); x++)
			fb->pixels[y * fb->width + x] = color;

		y++;
	}
}
