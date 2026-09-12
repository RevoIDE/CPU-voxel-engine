#include "buffers/framebuffer.h"
#include "err_handler.h"
#include "app_properties.h"
#include "rendering/rasterizer.h"
#include "xway.h"

#include <stdlib.h>
#include <wayland-client-core.h>

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
		.width = 720,
		.height = 480,
		.debug = 1,
		.vsync = 0
	};

	(void) app_infos;

	// engine entry point
	//
	t_xway_app		*xapp;
	t_framebuffer	*fb;

	xapp 	= xway_create			(app_infos.width, app_infos.height, app_infos.title);
	fb		= framebuffer_allocate	(app_infos.width, app_infos.height);

	if (!xapp || !fb)
		ERROR("Cannot create xapp or framebuffer");

	t_triangle tri = {
		.a = {10.0f, 10.0f, 0.0f},
		.b = {5.0f, 80.0f, 0.0f},
		.c = {150.0f, 80.0f, 0.0f}
	};

	while (xway_is_running(xapp) != 0)
	{
		framebuffer_clear(fb, 0x00FF1FF5);
		draw(fb, &tri, 0x00FFFFFF);
		xway_blit(xapp, fb->pixels);

		if (xway_present(xapp) == -1)
			ERROR("Cannot present xapp");

		if (xway_dispatch(xapp) == -1)
			ERROR("Failed to dispatch");
	}

	framebuffer_free(fb);
	xway_destroy	(xapp);

	return EXIT_SUCCESS;
}
