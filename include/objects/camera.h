#ifndef CAMERA_H
# define CAMERA_H

# include "maths/vectors.h"

typedef struct s_camera
{
	t_vec_3f	pos;
	t_vec_3f	forward;
	t_vec_3f	up;
	t_vec_3f	right;

	float		yaw;
	float		pitch;

	float		fov;
	float		aspect;
	float		Znear;
	float		Zfar;

	float		tanHalfFov;
}	t_camera;

t_camera	camera_create(int width, int height, float fov);

int			camera_update(t_camera *cam, t_vec_3f pos, float yaw, float pitch);

#endif
