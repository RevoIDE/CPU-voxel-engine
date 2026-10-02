#ifndef VERTEXBUFFER_H
# define VERTEXBUFFER_H

#include "objects/mesh.h"

typedef struct s_vertexbuffer_out
{
	float	*x, *y;
	float	*invw;
	float	*clipW;
	int		count;
	int		capacity;
}	t_vertexbuffer_out;

typedef struct s_vertexbuffer_in
{
	float	*x, *y, *z, *w;
	int		count;
}	t_vertexbuffer_in;

int		vertex_buffer_out_update(t_vertexbuffer_out *out, int count);
void	vertex_buffer_out_free(t_vertexbuffer_out *out);

#endif
