#ifndef RASTERIZER_H
# define RASTERIZER_H

# include "buffers/framebuffer.h"
# include "maths/matrix.h"
# include "maths/vectors.h"
# include "objects/camera.h"
# include "objects/mesh.h"
# include "rendering/shaders.h"

# include <stdint.h>

typedef struct s_triangle
{
	t_vec_2f	a, b, c;
	t_vec_2f	uv_a, uv_b, uv_c;
	t_vec_3f	invw;
	t_vec_3f	face_normal;
	float		area;
}	t_triangle;

void	draw(t_framebuffer *restrict fb, const t_triangle *restrict t,
			t_fragment_shader *shader_prgm, t_fragment_shader_data *data);
void	draw_mesh	(t_mesh *mesh, t_matrix_4f *mvp, t_camera *cam, t_framebuffer *fb, t_fragment_shader *shader_prgm, t_fragment_shader_data *data);

#endif
