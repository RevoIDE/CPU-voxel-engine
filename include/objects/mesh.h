#ifndef MESH_H
# define MESH_H

#include "maths/vectors.h"
# include <stdint.h>

typedef struct	s_vertex
{
	float		x,  y,  z;
	float		nx, ny, nz;

	// local uv mapping
	t_vec_2f	uv;

	// atlas uv mapping
	t_vec_2f	atlas_min;
	t_vec_2f	atlas_max;
}	t_vertex;

typedef struct	s_mesh
{
	// data
	t_vertex	*vertices;
	uint32_t	*indices;

	// allocated
	uint32_t 	vertex_count;
    uint32_t 	index_count;

    // current max capacity
    uint32_t 	vertex_capacity;
    uint32_t 	index_capacity;
}	t_mesh;

typedef enum BlockFace
{
	BLOCKFACE_TOP,
	BLOCKFACE_BOTTOM,
	BLOCKFACE_LEFT,
	BLOCKFACE_RIGHT,
	BLOCKFACE_FRONT,
	BLOCKFACE_BACK
}	t_block_face;

#endif
