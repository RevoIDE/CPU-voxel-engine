#include "objects/camera.h"
#include "maths/vectors.h"
#include "utils/math_utils.h"

#include <xway.h>
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

void	camera_handle_input(t_xway_app *app, t_camera *cam, float dt)
{
	t_vec_3f	move_dir = {0.0f, 0.0f, 0.0f};
	float		yaw = cam->yaw;
	float		pitch = cam->pitch;

	if (xway_key_down(app, XWAY_KEY_W) || xway_key_down(app, XWAY_KEY_Z))
	{
		move_dir.x += cam->forward.x;
		move_dir.y += cam->forward.y;
		move_dir.z += cam->forward.z;
	}
	if (xway_key_down(app, XWAY_KEY_S))
	{
		move_dir.x -= cam->forward.x;
		move_dir.y -= cam->forward.y;
		move_dir.z -= cam->forward.z;
	}

	if (xway_key_down(app, XWAY_KEY_D))
	{
		move_dir.x += cam->right.x;
		move_dir.y += cam->right.y;
		move_dir.z += cam->right.z;
	}
	if (xway_key_down(app, XWAY_KEY_A) || xway_key_down(app, XWAY_KEY_Q))
	{
		move_dir.x -= cam->right.x;
		move_dir.y -= cam->right.y;
		move_dir.z -= cam->right.z;
	}

	if (xway_key_down(app, XWAY_KEY_SPACE))
		move_dir.y += 1.0f;
	if (xway_key_down(app, XWAY_KEY_LEFT_SHIFT))
		move_dir.y -= 1.0f;

	float len = sqrtf(move_dir.x * move_dir.x
			+ move_dir.y * move_dir.y
			+ move_dir.z * move_dir.z);

	if (len > 1e-6f)
	{
		move_dir.x = move_dir.x / len * CAM_MOVE_SPEED * dt;
		move_dir.y = move_dir.y / len * CAM_MOVE_SPEED * dt;
		move_dir.z = move_dir.z / len * CAM_MOVE_SPEED * dt;
	}

	t_vec_3f new_pos = {
		cam->pos.x + move_dir.x,
		cam->pos.y + move_dir.y,
		cam->pos.z + move_dir.z
	};

	if (xway_key_down(app, XWAY_KEY_RIGHT))
		yaw += CAM_ROT_SPEED * dt;

	if (xway_key_down(app, XWAY_KEY_LEFT))
		yaw -= CAM_ROT_SPEED * dt;

	if (xway_key_down(app, XWAY_KEY_UP))
		pitch += CAM_ROT_SPEED * dt;

	if (xway_key_down(app, XWAY_KEY_DOWN))
		pitch -= CAM_ROT_SPEED * dt;

	float max_pitch = deg_to_rad(89.0f);
	if (pitch > max_pitch)
		pitch = max_pitch;
	if (pitch < -max_pitch)
		pitch = -max_pitch;

	camera_update(cam, new_pos, yaw, pitch);
}
