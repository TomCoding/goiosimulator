#
# goiovalues - Guns of Icarus Online damage simulator
# Copyright (C) 2016  Dominique Lasserre
#
# This file is part of goiovalues.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
    -Winit-self -Wmissing-declarations -Wmissing-include-dirs
    -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
    -Wsign-conversion -Wsign-promo -Wswitch-default
    -Wundef -Weffc++
)
if("${CMAKE_CXX_COMPILER}" MATCHES ".*/g\\+\\+.*")
target_compile_options(goiovalues PUBLIC
    -Wlogical-op
    -Wnoexcept
    -Wstrict-null-sentinel
)
endif()

find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --show-reachable=yes")

include(CTest)
add_test(run goiovalues)