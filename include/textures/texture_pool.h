#ifndef TEXTURE_POOL_H
# define TEXTURE_POOL_H

# include <stddef.h>
# include <stdint.h>

typedef struct s_texture_source
{
	const uint32_t	*buffer;
	uint16_t		width, height;
}	t_texture_source;

typedef struct s_texture_descriptor
{
	uint32_t	offset;
	uint16_t	width, height;
	uint16_t	mask_x, mask_y; // width - 1, height - 1
	uint8_t		shift;
}	t_texture_descriptor;

typedef struct s_texture_pool
{
	uint32_t				*texels;
	size_t					texel_count;
	t_texture_descriptor 	*descriptors;
	uint32_t				tex_count;
}	t_texture_pool;

typedef struct s_texture_view
{
	uint32_t	*buffer;
	uint32_t	mask_x, mask_y;
	uint32_t	shift;
}	t_texture_view;

void	texture_pool_build(t_texture_pool **pool, const t_texture_source *srcs, size_t count);
void	texture_pool_free(t_texture_pool *pool);

#endif
