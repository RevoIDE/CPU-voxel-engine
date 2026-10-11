#include "textures/texture_pool.h"
#include "err_handler.h"
#include "maths/vectors.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wctype.h>
#include <stdbool.h>

#define PREFIX "[Texture Pool]"

#define CACHE_LINE	64
#define LINE_TEXELS	(CACHE_LINE / sizeof(uint32_t)) /* 16 */

static bool	is_valid_size(uint32_t n)
{
	return (n != 0 && (n & (n -1)) == 0);
}

static uint32_t	align_up(uint32_t n)
{
	return ((n + LINE_TEXELS - 1) & ~(LINE_TEXELS - 1));
}

static uint8_t uint32_log2(uint32_t n)
{
	uint8_t	r = 0;

	while (n > 1)
	{
		n >>= 1;
		r++;
	}

	return (r);
}

void	texture_pool_build(t_texture_pool **pool, const t_texture_source *srcs, size_t count)
{
	if (!pool)
		ERROR(PREFIX "Invalid texture pool pointer !");

	*pool = malloc(sizeof(t_texture_pool));

	if (!*pool)
		ERROR("Unable to allocate texture pool");

	(*pool)->descriptors = malloc(sizeof(t_texture_descriptor) * count);

	if (!(*pool)->descriptors)
		ERROR(PREFIX "Unable to allocate descriptors");

	size_t	total = 0;

	for (size_t i = 0; i < count; i++)
	{
		uint32_t	w = srcs[i].width;
		uint32_t	h = srcs[i].height;

		if (!is_valid_size(w) || !is_valid_size(h))
			ERROR(PREFIX "Unable to fill texture pool, invalid size (must be a power of 2) !");

		t_texture_descriptor	*desc = &((*pool)->descriptors[i]);

		desc->offset 	= (uint32_t)total;

		desc->width 	= (uint32_t)w;
		desc->height 	= (uint32_t)h;

		desc->mask_x	= (uint16_t)(w - 1);
		desc->mask_y	= (uint16_t)(h - 1);

		desc->shift		= uint32_log2(w);

		total = align_up(total + (size_t)w * h);
	}

	size_t	bytes = total * sizeof(uint32_t);

	if (bytes == 0)
		bytes = CACHE_LINE;

	(*pool)->texels = aligned_alloc(CACHE_LINE, bytes);

	if (!(*pool)->texels)
		ERROR(PREFIX "Unable to allocate texture pool texels");

	for (size_t i = 0; i < count; i++)
	{
		const t_texture_descriptor	*desc = &(*pool)->descriptors[i];
		size_t						n = (size_t)desc->width * desc->height;

		memcpy((*pool)->texels + desc->offset, srcs[i].buffer, n * sizeof(uint32_t));
	}

	(*pool)->texel_count	= total;
	(*pool)->tex_count		= count;
}

void	texture_pool_free(t_texture_pool *pool)
{
	if (!pool)
		return ;

	free(pool->descriptors);
	free(pool->texels);
	free(pool);
}
