#ifndef RASTERIZER_H
# define RASTERIZER_H

# include "buffers/framebuffer.h"
# include "maths/vectors.h"

# include <stdint.h>

typedef struct s_triangle
{
	t_vec_3f a, b, c;
}	t_triangle;

void	draw(t_framebuffer *fb, t_triangle *triangle, uint32_t color);

#endif
