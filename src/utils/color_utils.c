#include "utils/color_utils.h"
#include <stdint.h>

uint32_t interpolate_color(uint32_t c1, uint32_t c2, float x)
{
	uint32_t b = (uint32_t)(((c1 >> 0)  & 0xFF) * (1 - x) + ((c2 >> 0)  & 0xFF) * x) & 0xFF;
	uint32_t g = (uint32_t)(((c1 >> 8)  & 0xFF) * (1 - x) + ((c2 >> 8)  & 0xFF) * x) & 0xFF;
	uint32_t r = (uint32_t)(((c1 >> 16) & 0xFF) * (1 - x) + ((c2 >> 16) & 0xFF) * x) & 0xFF;
	uint32_t a = (uint32_t)(((c1 >> 24) & 0xFF) * (1 - x) + ((c2 >> 24) & 0xFF) * x) & 0xFF;

	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}
