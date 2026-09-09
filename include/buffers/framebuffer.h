#ifndef FRAMEBUFFER_H
# define FRAMEBUFFER_H

#include <stdint.h>
typedef struct s_framebuffer {
    int				width;
    int				height;
    uint32_t		*pixels;
}	t_framebuffer;

t_framebuffer	*framebuffer_allocate(int width, int height);
void			framebuffer_free(t_framebuffer *framebuffer);

/**
 * Clears the framebuffer with the specified color.
 *
 * @param framebuffer The framebuffer to clear.
 * @param color The color to clear the framebuffer with.
 *
 * @note to ensure wayland compatibility, the color is packed into a 32-bit integer in the format 0xAARRGGBB.
 */
void			framebuffer_clear(t_framebuffer *framebuffer, uint32_t color);

/**
 * Draws a pixel at the specified coordinates with the given color.
 *
 * @param framebuffer The framebuffer to draw on.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 *
 * @note to ensure wayland compatibility, the color is packed into a 32-bit integer in the format 0xAARRGGBB.
 */
void			framebuffer_draw(t_framebuffer *framebuffer, int x, int y, uint32_t color);

#endif
