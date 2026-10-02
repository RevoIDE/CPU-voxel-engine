#ifndef CHUNK_H
# define CHUNK_H

# include "maths/vectors.h"
# include "objects/mesh.h"

# include <math.h>

# define CHUNK_X 16
# define CHUNK_Y 32
# define CHUNK_Z 16

static inline int	get_height(float wx, float wz)
{
	float	h;

	h = 8.0f;
	h += sinf(wx * 0.20f) * 3.0f;
	h += cosf(wz * 0.15f) * 3.0f;
	h += sinf((wx + wz) * 0.10f) * 4.0f;
	if (h < 1.0f)
		h = 1.0f;
	if (h > CHUNK_Y - 1)
		h = CHUNK_Y - 1;
	return ((int)h);
}

static inline int	is_solid(unsigned char b[CHUNK_X][CHUNK_Y][CHUNK_Z],
				int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0
		|| x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z)
		return (0);
	return (b[x][y][z]);
}

int	mesh_create_debug_chunk(t_mesh *mesh, t_vec_3f center);

#endif
