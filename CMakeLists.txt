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
set(goiovalues_VERSION_MAJOR 0)
set(goiovalues_VERSION_MINOR 1)

configure_file(
  "${PROJECT_SOURCE_DIR}/goiovalues.cfg"
  "${PROJECT_BINARY_DIR}/goiovalues.cfg"
  COPYONLY
)

option(COVERAGE "Enable gcov support." OFF)
option(TESTS "Disable tests." ON)

include(FindPkgConfig)
pkg_check_modules(libconfig REQUIRED "libconfig++>=1.4.9")

file(GLOB sources_lib "${PROJECT_SOURCE_DIR}/lib/*.cpp")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(goiovalues_lib "${sources_lib}")
target_link_libraries(goiovalues_lib ${libconfig_LIBRARIES})

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(goiovalues "${sources}")
target_link_libraries(goiovalues goiovalues_lib)

include(CTest)
# add_test(run goiovalues)
# add_test(api_sanity_checker
#   api-sanity-checker -l "${CMAKE_PROJECT_NAME}"
#                      -d "${PROJECT_SOURCE_DIR}/api-sanity-checks.xml"
#                      -relpath "${PROJECT_SOURCE_DIR}"
#                      -gen -build -run
# )

if(TESTS)
  include(ExternalProject)

  set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/googletest)
  ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.7.0
    TIMEOUT 10
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
  )

  file(GLOB sources_tests "${PROJECT_SOURCE_DIR}/tests/*.cpp")
  add_executable(goiovalues_tests "${sources_tests}")
  add_dependencies(goiovalues_tests googletest)

  ExternalProject_Get_Property(googletest source_dir)
  target_include_directories(goiovalues_tests SYSTEM PUBLIC "${source_dir}/include")

  if(MSVC)
    set(Suffix ".lib")
  else()
    set(Suffix ".a")
    set(Pthread "-pthread")
  endif()
  ExternalProject_Get_Property(googletest binary_dir)
  target_link_libraries(goiovalues_tests goiovalues_lib
    debug "${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}"
    optimized "${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}"
    ${Pthread}
  )

  add_test(Ammunitions goiovalues_tests --gtest_filter=Ammunitions.*)
  add_test(Balloon goiovalues_tests --gtest_filter=Balloon.*)
endif()

set(CXX_FLAGS "")
list(APPEND CXX_FLAGS
  -std=c++1y -O3 -ffast-math -pedantic -Wall -Wextra -Wcast-align
  -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2
  -Winit-self -Wmissing-declarations -Wmissing-include-dirs
  -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
  -Wsign-conversion -Wsign-promo -Wswitch-default
  -Wundef -Weffc++
)

if("${libconfig_VERSION}" VERSION_LESS 1.5)
  list(APPEND CXX_FLAGS
    -DLIBCONFIG_COMP
  )
endif()

if(CMAKE_COMPILER_IS_GNUCC)
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 4.9.3)
    message(FATAL_ERROR "GCC version must be at least 4.9.3!")
  elseif("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 5)
    list(APPEND CXX_FLAGS
      -DGCC_4_9
    )
  endif()

  list(APPEND CXX_FLAGS
    -Wlogical-op
    -Wnoexcept
    -Wstrict-null-sentinel
    -fdiagnostics-color=always
  )

  if(NOT "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 5)
    list(APPEND CXX_FLAGS
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
  list(APPEND CXX_FLAGS
    -DCLANG
  )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR COVERAGE)
  list(APPEND CXX_FLAGS
    -g
    -DSTRONG_TYPE_COMPILE
  )
  if(COVERAGE)
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    include(CodeCoverage)
    list(APPEND CXX_FLAGS
      -O0 -fprofile-arcs -ftest-coverage
      --coverage
      -fno-inline -fno-inline-small-functions -fno-default-inline
    )
    target_link_libraries(goiovalues_lib gcov)
    setup_target_for_coverage(goiovalues_coverage goiovalues_tests
                              "${PROJECT_BINARY_DIR}/coverage")
    target_compile_options(goiovalues_coverage PUBLIC ${CXX_FLAGS})
  endif()
endif()

target_compile_options(goiovalues_lib PUBLIC ${CXX_FLAGS})
target_compile_options(goiovalues PUBLIC ${CXX_FLAGS})
target_compile_options(goiovalues_tests PUBLIC ${CXX_FLAGS})


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