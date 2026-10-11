#include "objects/mesh.h"
#include "err_handler.h"
#include "maths/vectors.h"
#include "objects/textures.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline size_t	align32(size_t n)
{
	return (n + 31) & ~(size_t)31;
}

int	mesh_create(t_mesh *mesh, uint32_t vertex_capacity, uint32_t index_capacity)
{
	vertex_capacity = (vertex_capacity + 7) & ~7u;

	size_t	stride = align32(vertex_capacity * sizeof(float));
	float	*block = aligned_alloc(32, stride * 4);

	if (!block)
		return (-1);

	mesh->x = block;
	mesh->y = (float *)((char *) block + stride);
	mesh->z = (float *)((char *) block + stride * 2);
	mesh->w = (float *)((char *) block + stride * 3);

	mesh->norms     = malloc(sizeof(t_vec_3f) * vertex_capacity);
	mesh->uvs       = malloc(sizeof(t_vec_2f) * vertex_capacity);
	mesh->atlas_min = malloc(sizeof(t_vec_2f) * vertex_capacity);
	mesh->atlas_max = malloc(sizeof(t_vec_2f) * vertex_capacity);
	mesh->indices   = malloc(sizeof(uint32_t) * index_capacity);

	if (!mesh->norms || !mesh->uvs || !mesh->atlas_min
		|| !mesh->atlas_max || !mesh->indices)
	{
		free(block);

		free(mesh->norms);
		free(mesh->uvs);
		free(mesh->atlas_min);
		free(mesh->atlas_max);
		free(mesh->indices);
		return (-1);
	}

	mesh->vertex_capacity = vertex_capacity;
	mesh->index_capacity  = index_capacity;

	mesh->vertex_count    = 0;
	mesh->index_count     = 0;

	return (0);
}

void	mesh_destroy(t_mesh *mesh)
{
	free(mesh->x);   // sigle block sharing
	free(mesh->norms);
	free(mesh->uvs);
	free(mesh->atlas_min);
	free(mesh->atlas_max);
	free(mesh->indices);

	*mesh = (t_mesh) {0};
}

void	mesh_add_quad(t_mesh *mesh, t_vec_3f pos, t_vec_2f size, t_block_face face)
{
	if (mesh->vertex_count + 4 > mesh->vertex_capacity
		|| mesh->index_count + 6 > mesh->index_capacity)
		return;

	const float	w = size.x;
	const float	h = size.y;

	t_vec_3f	o;
	t_vec_3f	du;
	t_vec_3f	dv;
	t_vec_3f	n;

	if (face == FACE_TOP)
	{
		o  = (t_vec_3f){pos.x, pos.y + 1, pos.z + h};
		du = (t_vec_3f){w, 0, 0};
		dv = (t_vec_3f){0, 0, -h};
		n  = (t_vec_3f){0, 1, 0};
	}
	else if (face == FACE_BOTTOM)
	{
		o  = (t_vec_3f){pos.x, pos.y, pos.z};
		du = (t_vec_3f){w, 0, 0};
		dv = (t_vec_3f){0, 0, h};
		n  = (t_vec_3f){0, -1, 0};
	}
	else if (face == FACE_NORTH)
	{
		o  = (t_vec_3f){pos.x + w, pos.y, pos.z};
		du = (t_vec_3f){-w, 0, 0};
		dv = (t_vec_3f){0, h, 0};
		n  = (t_vec_3f){0, 0, -1};
	}
	else if (face == FACE_SOUTH)
	{
		o  = (t_vec_3f){pos.x, pos.y, pos.z + 1};
		du = (t_vec_3f){w, 0, 0};
		dv = (t_vec_3f){0, h, 0};
		n  = (t_vec_3f){0, 0, 1};
	}
	else if (face == FACE_EAST)
	{
		o  = (t_vec_3f){pos.x + 1, pos.y, pos.z + w};
		du = (t_vec_3f){0, 0, -w};
		dv = (t_vec_3f){0, h, 0};
		n  = (t_vec_3f){1, 0, 0};
	}
	else // FACE_WEST
	{
		o  = (t_vec_3f){pos.x, pos.y, pos.z};
		du = (t_vec_3f){0, 0, w};
		dv = (t_vec_3f){0, h, 0};
		n  = (t_vec_3f){-1, 0, 0};
	}

	const t_vec_2f	uv[4] = {{0, 0}, {w, 0}, {w, h}, {0, h}};

	t_vec_2f	amin = { 0.0f, 0.0f }; // placeholder
	t_vec_2f	amax = { 1.0f, 1.0f }; // placeholder
	// block_face_atlas_rect(face, &amin, &amax);

	const uint32_t	base = mesh->vertex_count;

	const float		cx[4] = {0, 1, 1, 0};
	const float		cy[4] = {0, 0, 1, 1};

	for (int i = 0; i < 4; i++)
	{
		const uint32_t	v = base + i;

		mesh->x[v] = o.x + du.x * cx[i] + dv.x * cy[i];
		mesh->y[v] = o.y + du.y * cx[i] + dv.y * cy[i];
		mesh->z[v] = o.z + du.z * cx[i] + dv.z * cy[i];
		mesh->w[v] = 1.0f;

		mesh->norms		[v]	= n;
		mesh->uvs		[v]	= uv[i];
		mesh->atlas_min	[v]	= amin;
		mesh->atlas_max	[v]	= amax;
	}

	uint32_t	*idx = mesh->indices + mesh->index_count;

	idx[0] = base;
	idx[1] = base + 1;
	idx[2] = base + 2;
	idx[3] = base;
	idx[4] = base + 2;
	idx[5] = base + 3;

	mesh->vertex_count += 4;
	mesh->index_count  += 6;
}
