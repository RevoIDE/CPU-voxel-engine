#ifndef MESH_H
# define MESH_H

# include "maths/vectors.h"
# include <stdint.h>

typedef struct s_mesh
{
	// data
	float	*x, *y, *z, *w;

	t_vec_3f	*norms;
	t_vec_2f	*uvs;
	t_vec_2f	*atlas_min;
	t_vec_2f	*atlas_max;

	uint32_t	*indices;

	uint32_t	vertex_capacity, index_capacity;
	uint32_t	vertex_count, index_count;

}	t_mesh;

typedef enum BlockFace
{
	FACE_TOP,		// +Y
	FACE_BOTTOM,	// -Y
	FACE_NORTH,		// -Z
	FACE_SOUTH,		// +Z
	FACE_EAST,		// +X
	FACE_WEST		// -X
}	t_block_face;

int		mesh_create(t_mesh *mesh, uint32_t vertex_capacity, uint32_t index_capacity);

void	mesh_destroy(t_mesh *mesh);
void	mesh_add_quad(t_mesh *mesh, t_vec_3f pos, t_vec_2f size, t_block_face face);

#endif
