
#include "objects/chunk.h"
#include "maths/vectors.h"
#include "objects/mesh.h"

int	mesh_create_debug_chunk(t_mesh *mesh, t_vec_3f center)
{
	static const t_block_face	faces[6] = {FACE_TOP, FACE_BOTTOM,
		FACE_NORTH, FACE_SOUTH, FACE_EAST, FACE_WEST};
	static const int			dir[6][3] = {{0, 1, 0}, {0, -1, 0},
		{0, 0, -1}, {0, 0, 1}, {1, 0, 0}, {-1, 0, 0}};
	static unsigned char		blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

	if (mesh_create(mesh, 12000, 12000 * 2) != 0)
		return (-1);

	for (int x = 0; x < CHUNK_X; x++)
		for (int z = 0; z < CHUNK_Z; z++)
		{
			int h = get_height(center.x + x, center.z + z);
			for (int y = 0; y < CHUNK_Y; y++)
				blocks[x][y][z] = (y < h);
		}

	for (int x = 0; x < CHUNK_X; x++)
		for (int y = 0; y < CHUNK_Y; y++)
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (!blocks[x][y][z])
					continue ;
				for (int f = 0; f < 6; f++)
					if (!is_solid(blocks, x + dir[f][0], y + dir[f][1], z + dir[f][2]))
						mesh_add_quad(mesh, (t_vec_3f){center.x + x,
							center.y + y, center.z + z},
							(t_vec_2f){1, 1}, faces[f]);
			}

	return (0);
}
