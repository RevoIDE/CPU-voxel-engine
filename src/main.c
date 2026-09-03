#include "err_handler.h"
#include "app.h"

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
		.width = 1920,
		.height = 1080,
		.debug = 1,
		.vsync = 0
	};

	// engine entry point
}
