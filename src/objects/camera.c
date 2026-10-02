#include "objects/camera.h"
#include "maths/vectors.h"
#include <math.h>

t_camera	camera_create(int width, int height, float fov)
{
	return (t_camera)
		{
			.Zfar = 100,
			.Znear = 0.5f,
			.aspect = (float) width / height,
			.fov = fov,
			.tanHalfFov = tanf(fov * 0.5f)
		};
}

int camera_update(t_camera *cam, t_vec_3f pos, float yaw, float pitch)
{
	if (!cam)
		return (-1);

	cam->pos = pos;

	const t_vec_3f	world_up = (t_vec_3f){0.0f, 1.0f, 0.0f};

	cam->forward = vec_3f_normalize((t_vec_3f){
		cosf(pitch) * cosf(yaw),
		sinf(pitch),
		cosf(pitch) * sinf(yaw)
	});

	cam->right 	= vec_3f_normalize(vec_3f_cross(cam->forward, world_up));
	cam->up 	= vec_3f_cross(cam->right, cam->forward);

	cam->yaw	= yaw;
	cam->pitch	= pitch;

	return (0);
}
