#include "app.h"
#include "xway.h"

#include "app_properties.h"
#include "buffers/framebuffer.h"
#include "buffers/vertexbuffer.h"
#include "err_handler.h"
#include "maths/matrix.h"
#include "maths/matrix_utils.h"
#include "maths/vectors.h"
#include "objects/camera.h"
#include "objects/mesh.h"
#include "objects/chunk.h"
#include "rendering/rasterizer.h"

#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wayland-client-core.h>

double get_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static inline float deg_to_rad(float deg)
{
	return deg * M_PI / 180;
}

#define CAM_MOVE_SPEED   4.0f
#define CAM_ROT_SPEED    deg_to_rad(90.0f)

static void	keyboard_callback(
	t_xway_app *app,
	t_xway_key key,
	t_xway_key_action action,
	void *user_data)
{
	(void)app;
	(void)user_data;

	if (action != XWAY_KEY_PRESSED)
		return ;

	switch (key)
	{
		case XWAY_KEY_ESCAPE:
			xway_quit(app);
			break ;
		default:
			break ;
	}
}

static void	camera_handle_input(t_xway_app *app, t_camera *cam, float dt)
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

/**
 * Entry point for the voxel CPU engine
 * @brief Entry point
 */
int	main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	INFO("Starting ...");

	// creating app details
	t_app_infos app_infos = {
		.title = "Voxel CPU engine",
		.width = 1280,
		.height = 720,
		.debug = 1,
		.vsync = 0
	};

	// engine entry point
	t_xway_app		*xapp;
	t_framebuffer	*fb;

	xapp 	= xway_create			(app_infos.width, app_infos.height, app_infos.title);
	fb		= framebuffer_allocate	(app_infos.width, app_infos.height);

	if (!xapp || !fb)
		ERROR("Cannot create xapp or framebuffer");

	t_mesh	cube;

	if (mesh_create_debug_chunk(&cube, (t_vec_3f){0, 0, 0}) != 0)
		ERROR("Cannot create cube mesh");

	double start_time = get_time_seconds();

	double fps_timer = start_time;
	unsigned int frame_count = 0;
	double last_frame_time = start_time;

	t_camera	camera = camera_create(app_infos.width, app_infos.height, deg_to_rad(60.0f));

	camera_update(&camera, (t_vec_3f) { -2.5f, 5.0f, 0.0f}, deg_to_rad(0), deg_to_rad(0));

	xway_set_key_callback(xapp, keyboard_callback, NULL);

	while (xway_is_running(xapp) != 0)
	{
	    framebuffer_clear(fb, 0xEEE7D5);

	    double now_time = get_time_seconds();
	    float delta_time = (float)(now_time - last_frame_time);
	    last_frame_time = now_time;

	    if (delta_time > 0.25f)
	        delta_time = 0.25f;

	    camera_handle_input(xapp, &camera, delta_time);

	    t_matrix_4f mvp = compute_mvp(&camera);

	    draw_mesh(&cube, &mvp, &camera, fb);
	    xway_blit(xapp, fb->pixels);

	    if (xway_present(xapp) == -1)
	        ERROR("Cannot present xapp");

	    if (xway_wait_frame(xapp) == -1)
	        ERROR("Failed to wait for frame");

	    frame_count++;

	    double now = get_time_seconds();

	    if (now - fps_timer >= 1.0)
	    {
	        double fps = frame_count / (now - fps_timer);

	        printf("FPS: %.2f\n", fps);

	        frame_count = 0;
	        fps_timer = now;
	    }
	}

	mesh_destroy	(&cube);
	framebuffer_free(fb);
	xway_destroy	(xapp);

	return EXIT_SUCCESS;
}
