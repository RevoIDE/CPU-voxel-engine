#include "objects/mesh.h"
#include "err_handler.h"
#include "maths/vectors.h"
#include "objects/textures.h"
#include <stdint.h>
#include <stdlib.h>

t_mesh	*mesh_create(int vertex_capacity, int index_capacity)
{
	t_mesh *mesh = malloc(sizeof(t_mesh));

	if (!mesh)
	{
		WARN("Unable to allocate mesh !");
		return NULL;
	}

	mesh->index_capacity  = index_capacity;
	mesh->vertex_capacity = vertex_capacity;

	mesh->index_count  = 0;
	mesh->vertex_count = 0;

	mesh->indices  = malloc(sizeof(uint32_t) * index_capacity);

	if (!mesh->indices)
	{
		free(mesh);
		WARN("Unable to allocate mesh indices !");
		return NULL;
	}

	mesh->vertices = malloc(sizeof(t_vertex) * vertex_capacity);

	if (!mesh->vertices)
	{
		free(mesh->indices);
		free(mesh);
		WARN("Unable to allocate mesh vertices !");
		return NULL;
	}

	return mesh;
}

void	mesh_destroy(t_mesh *mesh)
{
	if (!mesh)
		return;

	free(mesh->indices);
	free(mesh->vertices);
	free(mesh);
}

void	mesh_add_vertex(t_mesh *mesh, t_vertex vertex)
{
	if (!mesh || !mesh->vertices)
		return;

	if (mesh->vertex_count >= mesh->vertex_capacity)
	{
		WARN("Vertex capacity exceeded !");
		return;
	}

	mesh->vertices[mesh->vertex_count++] = vertex;
}

void	mesh_add_index(t_mesh *mesh, uint32_t index)
{
	if (!mesh || !mesh->indices)
		return;

	if (mesh->index_count >= mesh->index_capacity)
	{
		WARN("Vertex capacity exceeded !");
		return;
	}

	mesh->indices[mesh->index_count++] = index;
}

void	mesh_add_quad(t_mesh *mesh, t_vec_3f pos, t_vec_2f size, t_block_face face, t_tex_mapping *mapping)
{
	if (!mesh || !mesh->vertices || !mesh->indices)
		return;

	if (mesh->vertex_count + 4 > mesh->vertex_capacity)
	{
		mesh->vertex_capacity *= 2;
		t_vertex *tmp = realloc(mesh->vertices, sizeof(t_vertex) * mesh->vertex_capacity);
		if (!tmp)
		{
			WARN("Unable to reallocate memory for vertices");
			return;
		}
		mesh->vertices = tmp;
	}

	if (mesh->index_count + 6 > mesh->index_capacity)
	{
		mesh->index_capacity *= 2;
		uint32_t *tmp = realloc(mesh->indices, sizeof(uint32_t) * mesh->index_capacity);
		if (!tmp)
		{
			WARN("Unable to reallocate memory for indices");
			return;
		}

		mesh->indices = tmp;
	}

	float u0 = mapping->u0;
	float u1 = mapping->u1;
	float v0 = mapping->v0;
	float v1 = mapping->v1;

	uint32_t base = mesh->vertex_count;
	t_vertex v[4];

	switch (face) {
    case BLOCKFACE_TOP:
        v[0] = (t_vertex) {pos.x, 			pos.y + 1, pos.z, 			0, 1, 0, 0, 		size.y, u1, v1, u0, v0};
        v[1] = (t_vertex) {pos.x, 			pos.y + 1, pos.z + size.y, 	0, 1, 0, 0, 		0, 		u1, v1, u0, v0};
        v[2] = (t_vertex) {pos.x + size.x, 	pos.y + 1, pos.z + size.y, 	0, 1, 0, size.x, 	0, 		u1, v1, u0, v0};
        v[3] = (t_vertex) {pos.x + size.x, 	pos.y + 1, pos.z, 			0, 1, 0, size.x, 	size.y, u1, v1, u0, v0};
        break;
    case BLOCKFACE_BOTTOM:
        v[0] = (t_vertex) {pos.x, 			pos.y, pos.z, 				0, -1, 0, 0, 		size.y, u1, v1, u0, v0};
        v[1] = (t_vertex) {pos.x + size.x, 	pos.y, pos.z, 				0, -1, 0, 0, 		0, 		u1, v1, u0, v0};
        v[2] = (t_vertex) {pos.x + size.x, 	pos.y, pos.z + size.y, 		0, -1, 0, size.x, 	0, 		u1, v1, u0, v0};
        v[3] = (t_vertex) {pos.x, 			pos.y, pos.z + size.y, 		0, -1, 0, size.x, 	size.y, u1, v1, u0, v0};
        break;
    case BLOCKFACE_FRONT:
        v[0] = (t_vertex) {pos.x, 			pos.y, 			pos.z, 	0, 0, -1, 0, 		size.y, u1, v1, u0, v0};
        v[1] = (t_vertex) {pos.x, 			pos.y + size.y, pos.z, 	0, 0, -1, 0, 		0, 		u1, v1, u0, v0};
        v[2] = (t_vertex) {pos.x + size.x, 	pos.y + size.y, pos.z, 	0, 0, -1, size.x, 	0, 		u1, v1, u0, v0};
        v[3] = (t_vertex) {pos.x + size.x, 	pos.y, 			pos.z, 	0, 0, -1, size.x, 	size.y, u1, v1, u0, v0};
        break;
    case BLOCKFACE_BACK:
        v[0] = (t_vertex) {pos.x + size.x, 	pos.y, 			pos.z + 1, 	0, 0, 1, 0, 		size.y, u1, v1, u0, v0};
        v[1] = (t_vertex) {pos.x + size.x, 	pos.y + size.y, pos.z + 1, 	0, 0, 1, 0, 		0, 		u1, v1, u0, v0};
        v[2] = (t_vertex) {pos.x, 			pos.y + size.y, pos.z + 1, 	0, 0, 1, size.x, 	0, 		u1, v1, u0, v0};
        v[3] = (t_vertex) {pos.x, 			pos.y, 			pos.z + 1, 	0, 0, 1, size.x, 	size.y, u1, v1, u0, v0};
        break;
    case BLOCKFACE_LEFT:
        v[0] = (t_vertex) {pos.x, pos.y, 			pos.z + size.y, 	-1, 0, 0, 0, 		size.y, u1, v1, u0, v0};
        v[1] = (t_vertex) {pos.x, pos.y + size.y, 	pos.z + size.y, 	-1, 0, 0, 0, 		0, 		u1, v1, u0, v0};
        v[2] = (t_vertex) {pos.x, pos.y + size.y, 	pos.z, 				-1, 0, 0, size.x, 	0, 		u1, v1, u0, v0};
        v[3] = (t_vertex) {pos.x, pos.y, 			pos.z, 				-1, 0, 0, size.x, 	size.y, u1, v1, u0, v0};
        break;
    case BLOCKFACE_RIGHT:
        v[0] = (t_vertex) {pos.x + 1, pos.y, 			pos.z, 				1, 0, 0, 0, 		size.y, u1, v1, u0, v0};
        v[1] = (t_vertex) {pos.x + 1, pos.y + size.y, 	pos.z, 				1, 0, 0, 0, 		0, 		u1, v1, u0, v0};
        v[2] = (t_vertex) {pos.x + 1, pos.y + size.y, 	pos.z + size.y, 	1, 0, 0, size.x, 	0, 		u1, v1, u0, v0};
        v[3] = (t_vertex) {pos.x + 1, pos.y, 			pos.z + size.y, 	1, 0, 0, size.x, 	size.y, u1, v1, u0, v0};
        break;
	}

	for (int i = 0; i < 4; i++)
		mesh->vertices[mesh->vertex_count++] = v[i];

	mesh->indices[mesh->index_count++] = base;
	mesh->indices[mesh->index_count++] = base + 1;
	mesh->indices[mesh->index_count++] = base + 2;
	mesh->indices[mesh->index_count++] = base;
	mesh->indices[mesh->index_count++] = base + 2;
	mesh->indices[mesh->index_count++] = base + 3;
}
