# base project setup
cmake_minimum_required(VERSION 3.20)
PROJECT(voxel-cpu C)

# Export compile commands for use with tools like clangd
# (in fact some IDEs need it to handle code navigation)
# (without it, IDEs may generate an error on your includes)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)

# Gather all source files and headers
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.c"
    "include/*.h"
)

# Add the executable
add_executable(voxel-cpu ${SOURCES})

# Add include directories
target_include_directories(
    voxel-cpu
    PRIVATE
    include
)

# adding AVX2 and FMA compile options
target_compile_options(voxel-cpu PRIVATE
    -mavx2
    -mfma
)

# Optionnal, generate Doxygen documentation
# Doxygen gives a nice HTML documentation from the comments in the code
# (see: https://www.doxygen.nl/)

find_package(Doxygen)

if (DOXYGEN_FOUND)
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

	add_custom_target(doxygen
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Doxygen doc"
	)
endif()

target_link_libraries(voxel-cpu PRIVATE m)

# no other libs needed (this project only uses the standard C library)
