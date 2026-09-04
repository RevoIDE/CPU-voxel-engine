#include "objects/mesh.h"
#include "err_handler.h"
#include <stdint.h>
#include <stdlib.h>

t_mesh	*mesh_create(int vertex_capacity, int index_capacity)
{
	t_mesh *mesh = malloc(sizeof(t_mesh));

	if (!mesh)
		ERROR("Unable to allocate mesh !");

	mesh->index_capacity  = index_capacity;
	mesh->vertex_capacity = vertex_capacity;

	mesh->index_count  = 0;
	mesh->vertex_count = 0;

	mesh->indices  = malloc(sizeof(uint32_t) * index_capacity);

	if (!mesh->indices)
	{
		free(mesh);
		ERROR("Unable to allocate mesh indices !");
	}

	mesh->vertices = malloc(sizeof(t_vertex) * vertex_capacity);

	if (!mesh->vertices)
	{
		free(mesh->indices);
		free(mesh);
		ERROR("Unable to allocate mesh vertices !");
	}

	return mesh;
}

void	mesh_destroy(t_mesh *mesh)
{
	free(mesh->indices);
	free(mesh->vertices);
	free(mesh);
}

void	mesh_add_vertex(t_mesh *mesh, t_vertex vertex)
{
	if (mesh->vertex_count >= mesh->vertex_capacity)
		ERROR("Vertex capacity exceeded !");

	mesh->vertices[mesh->vertex_count++] = vertex;
}

void	mesh_add_index(t_mesh *mesh, uint32_t index)
{
	if (mesh->index_count >= mesh->index_capacity)
		ERROR("Index capacity exceeded !");

	mesh->indices[mesh->index_count++] = index;
}

void	mesh_add_quad(t_mesh *mesh, float x, float y, float z, float w, float h, t_block_face face)
{

}
