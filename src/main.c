#include "rendering/shaders.h"
#include "textures/texture_pool.h"
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
#include "utils/math_utils.h"

#include <bits/time.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wayland-client-core.h>

double get_time_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

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

#define DEBUG_TEXTURE_SIZE 16

t_texture_source	debug_texture_source(void)
{
	static uint32_t		buf[DEBUG_TEXTURE_SIZE * DEBUG_TEXTURE_SIZE];
	t_texture_source	src;
	int					y;
	int					x;

	y = -1;
	while (++y < DEBUG_TEXTURE_SIZE)
	{
		x = -1;
		while (++x < DEBUG_TEXTURE_SIZE)
			buf[y * DEBUG_TEXTURE_SIZE + x] = ((x ^ y) & 8) ? 0xFF00EE : 0xFF000000;
	}
	src.buffer = buf;
	src.width = DEBUG_TEXTURE_SIZE;
	src.height = DEBUG_TEXTURE_SIZE;
	return (src);
}

t_fragment_shader	frag =
{
	.name = "frag",
	.shade = shader_opaque_geometry
};

static void	sync_framebuffer_size(t_xway_app *xapp, t_framebuffer **fb,
	t_camera *camera, t_vec_3f cam_pos, float yaw, float pitch)
{
	int32_t	win_w;
	int32_t	win_h;

	if (xway_get_size(xapp, &win_w, &win_h) != 0)
		return ;

	if (win_w == (*fb)->width && win_h == (*fb)->height)
		return ;

	framebuffer_free(*fb);

	*fb = framebuffer_allocate(win_w, win_h);

	if (!*fb)
		ERROR("Cannot reallocate framebuffer");

	*camera = camera_create(win_w, win_h, deg_to_rad(110.0f));
	camera_update(camera, cam_pos, yaw, pitch);
}

//-------------------- END

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

	t_texture_pool	*pool;

	t_texture_source srcs[] = { debug_texture_source() };
	texture_pool_build(&pool, srcs, 1);

	double start_time = get_time_seconds();

	double fps_timer = start_time;
	unsigned int frame_count = 0;
	double last_frame_time = start_time;

	t_camera	camera = camera_create(app_infos.width, app_infos.height, deg_to_rad(110.0f));

	camera_update(&camera, (t_vec_3f) { -2.5f, 5.0f, 0.0f }, deg_to_rad(0), deg_to_rad(0));

	xway_set_key_callback(xapp, keyboard_callback, NULL);

	while (xway_is_running(xapp) != 0)
	{
		sync_framebuffer_size(xapp, &fb, &camera,
			camera.pos, camera.yaw, camera.pitch);

		framebuffer_clear(fb, 0xEEE7D5);

		double now_time = get_time_seconds();
		float delta_time = (float)(now_time - last_frame_time);

		last_frame_time = now_time;

		if (delta_time > 0.25f)
			delta_time = 0.25f;

		camera_handle_input(xapp, &camera, delta_time);

		t_matrix_4f mvp = compute_mvp(&camera);

		draw_mesh(&cube, &mvp, &camera, fb, &frag, (void *) pool);
		xway_blit(xapp, fb->pixels, fb->stride, fb->height);

		if (xway_present(xapp) == -1)
			ERROR("Cannot present xapp");

		if (xway_wait_frame(xapp) == -1)
			ERROR("Failed to wait for frame");

		frame_count++;
		double now = get_time_seconds();
		if (now - fps_timer >= 1.0)
		{
			printf("FPS: %.2f\n", frame_count / (now - fps_timer));
			frame_count = 0;
			fps_timer = now;
		}
	}

	mesh_destroy	(&cube);
	texture_pool_free(pool);
	framebuffer_free(fb);
	xway_destroy	(xapp);

	return EXIT_SUCCESS;
}
