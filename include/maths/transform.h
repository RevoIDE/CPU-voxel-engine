#ifndef TRANSFORM_H
# define TRANSFORM_H

# include "buffers/vertexbuffer.h"
# include "maths/matrix.h"

void	transform_vertices(t_vertexbuffer_in *in, t_vertexbuffer_out *out, t_matrix_4f *mvp, int fbwidth, int fbheight);

#endif
