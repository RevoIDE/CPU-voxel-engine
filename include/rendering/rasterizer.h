#ifndef RASTERIZER_H
# define RASTERIZER_H

# include "buffers/framebuffer.h"
# include "maths/matrix.h"
# include "maths/vectors.h"
# include "objects/camera.h"
# include "objects/mesh.h"

# include <stdint.h>

typedef struct s_triangle
{
	t_vec_2f	a, b, c;
	t_vec_3f	invw;
	float		area;
}	t_triangle;

void			draw		(t_framebuffer *fb, t_triangle *triangle, uint32_t color);
void			draw_mesh	(t_mesh *mesh, t_matrix_4f *mvp, t_camera *cam, t_framebuffer *fb);

#endif
