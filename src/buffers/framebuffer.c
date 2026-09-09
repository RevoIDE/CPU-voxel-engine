#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "buffers/framebuffer.h"
#include "err_handler.h"

t_framebuffer	*framebuffer_allocate(int width, int height)
{
	t_framebuffer *buffer = malloc(sizeof(t_framebuffer));
	if (!buffer)
		ERROR("[Framebuffer] Unable to allocate framebuffer !");

	if (width <= 0 || height <= 0)
		ERROR("[Framebuffer] Invalid framebuffer dimensions !");

	buffer->pixels = malloc(sizeof(uint32_t) * (size_t) width * (size_t) height);
	if (!buffer->pixels)
	{
		free(buffer);
		ERROR("[Framebuffer] Unable to allocate framebuffer !");
	}

	buffer->width  = width;
	buffer->height = height;

	return buffer;
}

void			framebuffer_free(t_framebuffer *framebuffer)
{
	if (!framebuffer)
		return;

	free(framebuffer->pixels);
	free(framebuffer);
}

void	framebuffer_clear(t_framebuffer *framebuffer, uint32_t color)
{
	if (!framebuffer)
		ERROR("Cannot clear, unallocated framebuffer !");
	if (!framebuffer->pixels)
		ERROR("Cannot clear, partially allocated framebuffer !");

	size_t count = (size_t)framebuffer->width * (size_t)framebuffer->height;

	for (size_t i = 0; i < count; i++)
		framebuffer->pixels[i] = color;
}

void			framebuffer_draw(t_framebuffer *framebuffer, int x, int y, uint32_t color)
{
	if (!framebuffer)
		ERROR("[Framebuffer] Cannot draw, unallocated framebuffer !");

	if (!framebuffer->pixels)
		ERROR("[Framebuffer] Cannot draw, partially allocated framebuffer !");

	if (x < 0 || x >= framebuffer->width || y < 0 || y >= framebuffer->height)
		ERROR("[Framebuffer] Cannot draw, unreachable coordinates !");

	framebuffer->pixels[y * framebuffer->width + x] = color;
}
