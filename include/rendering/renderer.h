#ifndef RENDERER_H
# define RENDERER_H

#include "buffers/framebuffer.h"
# include "objects/mesh.h"
# include "maths/matrix.h"

typedef struct s_renderer_state
{
	t_matrix_4f		projection_matrix;
	t_framebuffer	*framebuffer;
}	t_renderer_state;

void	render(t_renderer_state *state, t_mesh *mesh);

#endif
