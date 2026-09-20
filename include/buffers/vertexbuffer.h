#ifndef VERTEXBUFFER_H
# define VERTEXBUFFER_H

#include "objects/mesh.h"

typedef struct s_vertexbuffer_out
{
	float	*x, *y;
	float	*invw;
	int		count;
}	t_vertexbuffer_out;

typedef struct s_vertexbuffer_in
{
	float	*x, *y, *z, *w;
	int		count;
}	t_vertexbuffer_in;

#endif
