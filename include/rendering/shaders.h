#ifndef SHADERS_H
# define SHADERS_H

# include "maths/vectors.h"
# include "textures/texture_pool.h"
# include <math.h>
# include <stdint.h>

typedef struct s_fragment_shader_data
{
	void	*data; 		// shader proper struct
	void	*uniforms;
}	t_fragment_shader_data;

typedef struct s_fragment_shader_in
{
	float					x, y, invw;	// clipped, screen coordinates
	float					u, v;
	t_vec_3f				normal;
	t_fragment_shader_data	*data;
}	t_fragment_shader_in;

typedef struct s_fragment_shader
{
	uint32_t	(*shade)(t_fragment_shader_in *);
	const char	*name;
}	t_fragment_shader;

static inline uint32_t	shader_opaque_geometry(t_fragment_shader_in *in)
{
	const t_texture_pool		*pool = (const t_texture_pool *)in->data;
	const t_texture_descriptor	*desc = &pool->descriptors[0];
	const uint32_t				*tex = pool->texels + desc->offset;

	int32_t	ix = (int32_t)(in->u * desc->width + 65536.0f);
	int32_t	iy = (int32_t)(in->v * desc->height + 65536.0f);

	uint32_t	x = (uint32_t)ix & desc->mask_x;
	uint32_t	y = (uint32_t)iy & desc->mask_y;

	uint32_t	color = (tex[(y << desc->shift) | x]);

	static const t_vec_3f	light_dir = { 0.34f, 0.65f, 0.32f };

	float	dot = vec_3f_dot(vec_3f_normalize(in->normal), vec_3f_normalize(light_dir));

	float	ambient = 0.15;
	float	intensity = ambient + (1.0f - ambient) * fmax(dot, 0.0f);
	intensity = fminf(intensity, 1.0f);

	uint8_t	r = (uint8_t)(((color >> 16) & 0xFF) * intensity);
	uint8_t g = (uint8_t)(((color >> 8) & 0xFF) * intensity);
	uint8_t	b = (uint8_t)((color & 0xFF) * intensity);

	return ((r << 16) | (g << 8) | b);
}

#endif
