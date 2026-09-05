#ifndef TEXTURES_H
# define TEXTURES_H

# include <stdint.h>

typedef struct s_tex_mapping {
	char		name[64];
	uint32_t	text_id;

	// atlas properties
	float		u0, v0;
	float		u1, v1;
} t_tex_mapping;

#endif
