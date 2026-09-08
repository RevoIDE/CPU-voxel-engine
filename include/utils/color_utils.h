#ifndef COLOR_UTILS_H
# define COLOR_UTILS_H

# include <stdint.h>

/*
 * Interpolates between two colors using a given factor x (0.0 to 1.0)
 * x refers to barycentric interpolation made by the rasterizer
 *
 * Returns the interpolated color as a 32-bit 0xAARRGGBB value
 */
uint32_t interpolate_color(uint32_t c1, uint32_t c2, float x);

#endif
