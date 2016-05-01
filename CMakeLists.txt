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

configure_file(
  "${PROJECT_SOURCE_DIR}/goiovalues.cfg"
  "${PROJECT_BINARY_DIR}/goiovalues.cfg"
  COPYONLY
)

option(Coverage "Enable gcov support." OFF)

include(FindPkgConfig)
pkg_check_modules(libconfig REQUIRED "libconfig++>=1.5")

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp")
include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(goiovalues "${sources}")
target_link_libraries(goiovalues ${libconfig_LIBRARIES})

target_compile_options(goiovalues PUBLIC
  -std=c++1y -O3 -ffast-math -pedantic -Wall -Wextra -Wcast-align
  -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2
  -Winit-self -Wmissing-declarations -Wmissing-include-dirs
  -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
  -Wsign-conversion -Wsign-promo -Wswitch-default
  -Wundef -Weffc++
)
if(CMAKE_COMPILER_IS_GNUCC)
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 4.9.3)
    message(FATAL_ERROR "GCC version must be at least 4.9.3!")
  elseif("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 5)
    target_compile_options(goiovalues PUBLIC
      -DGCC_4_9
    )
  endif()

  target_compile_options(goiovalues PUBLIC
    -Wlogical-op
    -Wnoexcept
    -Wstrict-null-sentinel
    -fdiagnostics-color=always
  )

  if(NOT "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 5)
    target_compile_options(goiovalues PUBLIC
      -Wswitch-bool
      -Wlogical-not-parentheses
      -Wsizeof-array-argument
      -Wbool-compare
    )
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3.4)
    message(FATAL_ERROR "Clang version must be at least 3.4!")
  endif()
  target_compile_options(goiovalues PUBLIC
    -DCLANG
  )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR COVERAGE)
  target_compile_options(goiovalues PUBLIC
    -g
    -DSTRONG_TYPE_COMPILE
  )
  if(COVERAGE)
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    include(CodeCoverage)
    target_compile_options(goiovalues PUBLIC
      -O0 -fprofile-arcs -ftest-coverage
      --coverage
      -fno-inline -fno-inline-small-functions -fno-default-inline
    )
    target_link_libraries(goiovalues gcov)
    setup_target_for_coverage(goiovalues_coverage goiovalues
                              "${PROJECT_BINARY_DIR}/coverage")
  endif()
endif()


find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes \
                                 --leak-check=full \
                                 --show-reachable=yes"
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} "${PROJECT_SOURCE_DIR}/Doxyfile"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Generating API documentation with Doxygen." VERBATIM
  )
endif()

include(CTest)
add_test(run goiovalues)
# add_test(api_sanity_checker
#   api-sanity-checker -l "${CMAKE_PROJECT_NAME}"
#                      -d "${PROJECT_SOURCE_DIR}/api-sanity-checks.xml"
#                      -relpath "${PROJECT_SOURCE_DIR}"
#                      -gen -build -run
# )