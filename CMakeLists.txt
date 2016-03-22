cmake_minimum_required(VERSION 3.0)
project(goiovalues)
# set(goiovalues_VERSION_MAJOR 0)
# set(goiovalues_VERSION_MINOR 1)

# configure_file(
#   "${PROJECT_SOURCE_DIR}/config.h.in"
#   "${PROJECT_BINARY_DIR}/config.h"
# )

file(GLOB sources "${PROJECT_SOURCE_DIR}/*.cpp")
include_directories("${PROJECT_SOURCE_DIR}")
add_executable(goiovalues "${sources}")
target_compile_options(goiovalues PUBLIC
    -fdiagnostics-color=always
    -std=c++11 -O3 -ffast-math -g -pedantic -Wall -Wextra -Wcast-align
    -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2
    -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
    -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
    -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default
    -Wundef -Weffc++
)

find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --show-reachable=yes")

include(CTest)
add_test(run goiovalues)
# add_test(valgrind valgrind --leak-check=full ./goiovalues)