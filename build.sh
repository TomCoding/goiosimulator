#!/bin/bash
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

BIN_NAME="goiovalues"
CPPCHECK_DIR="cppcheck"
CLANGCHECK_DIR="clangcheck"
BUILD_DIR="build"
SOURCE_DIR="$(pwd)"

function help() {
  echo "Usage: $0 [options]
Options:
  --gcc, --g++        Use gcc/g++ compiler.
  --clang, --clang++  Use clang/clang++ compiler.
  --clangcheck,       Use clang/clang++ compiler and static analyzer.
  --clangcheckopen,   Same as --clangcheck, additionally open report in browser.
  --cppcheck          Execute cppcheck and generate report after build.
  --cppcheckopen      Same as --cppcheck, additionally open report in browser.
  --api-sanity-checks Run API sanity checks.
  --cpplint           Run cpplint.
  --lcov              Run lcov.
  --lcovopen          Same as --lcov, additionally open report in browser.
  -n, --no-debug      Compile without debug flags.
  --doc               Create documentation.
  --no-tests          Don't run tests.
  -r, --run           Run program after compilation.
  -v, --valgrind      Run program after compilation with valgrind.
  -c, --clean         Use clean build environment.
  -w, --wait          Wait for input before running program.
  -h, --help          Print this message and exit.
"
}

function wait_input() {
  if [ "$WAIT" == 1 ]; then
    read -p "
press enter to run ./$BIN_NAME..."
  fi
}

HELP=0
CPPCHECK=0
CPPCHECK_OPEN=0
CLANGCHECK=0
CLANGCHECK_OPEN=0
RUN=0
VALGRIND=0
NO_TESTS=0
API_SANITY_CHECKS=0
CPPLINT=0
LCOV=0
LCOV_OPEN=0
NO_DEBUG=0
DOC=0
WAIT=0
CLEAN=0
for i in "$@"; do
  case "$i" in
    "--gcc"|"--g++")
      CXX_COMPILER="g++"
      C_COMPILER="gcc"
      shift
      ;;
    "--clang"|"--clang++")
      CXX_COMPILER="clang++"
      C_COMPILER="clang"
      shift
      ;;
    "--clangcheck")
      CLANGCHECK=1
      shift
      ;;
    "--clangcheckopen")
      CLANGCHECK=1
      CLANGCHECK_OPEN=1
      shift
      ;;
    "--cppcheck")
      CPPCHECK=1
      shift
      ;;
    "--cppcheckopen")
      CPPCHECK=1
      CPPCHECK_OPEN=1
      shift
      ;;
    "--doc")
      DOC=1
      shift
      ;;
    "-r"|"--run")
      RUN=1
      shift
      ;;
    "-v"|"--valgrind")
      RUN=0
      VALGRIND=1
      shift
      ;;
    "--no-tests")
      NO_TESTS=1
      shift
      ;;
    "--api-sanity-checks")
      API_SANITY_CHECKS=1
      shift
      ;;
    "--cpplint")
      CPPLINT=1
      shift
      ;;
    "--lcov")
      LCOV=1
      shift
      ;;
    "--lcovopen")
      if [ "$NO_DEBUG" == 1 ]; then
        echo "Can't use lcov without debug mode."
        help
        exit 1
      fi
      LCOV=1
      LCOV_OPEN=1
      shift
      ;;
    "--no-debug"|"-n")
      if [ "$LCOV" == 1 ]; then
        echo "Can't use lcov without debug mode."
        help
        exit 1
      fi
      NO_DEBUG=1
      shift
      ;;
    "-w"|"--wait")
      WAIT=1
      if [ "$VALGRIND" == 0 ]; then
        RUN=1
      fi
      shift
      ;;
    "-c"|"--clean")
      CLEAN=1
      shift
      ;;
    "-h"|"--help")
      HELP=1
      shift
      ;;
    *)
      help
      exit 1
      ;;
  esac
done

if [ "$HELP" == 1 ]; then
  help
  exit 0;
fi

CMAKE_OPTIONS=
if [ "$NO_DEBUG" == 0 ]; then
  CMAKE_OPTIONS="-DCMAKE_BUILD_TYPE=Debug"
fi
if [ "$LCOV" == 1 ]; then
  CMAKE_OPTIONS="$CMAKE_OPTIONS -DCOVERAGE=ON"
fi

if [ "$CLEAN" == 1 ]; then
  if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
  fi
fi

if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi
cd "$BUILD_DIR"
if [ "$CLANGCHECK" == 1 ]; then
  analyzer_dir="$(dirname $(readlink -f $(which scan-build)))"
  CXX_COMPILER="$analyzer_dir/c++-analyzer"
  C_COMPILER="$analyzer_dir/ccc-analyzer"
  scan-build -o "../$CLANGCHECK_DIR" cmake $CMAKE_OPTIONS \
                     -DCMAKE_CXX_COMPILER="$CXX_COMPILER" \
                     -DCMAKE_C_COMPILER="$C_COMPILER" ..
elif [ "$CXX_COMPILER" != "" ]; then
  cmake -DCMAKE_CXX_COMPILER="$CXX_COMPILER" $CMAKE_OPTIONS \
                     -DCMAKE_C_COMPILER="$C_COMPILER" ..
elif [ ! -f Makefile ]; then
  cmake $CMAKE_OPTIONS ..
else
  make -n goiovalues_coverage &>/dev/null
  LCOV_RET=$?
  if [ "$LCOV" == 1 ]; then
    if [ "$LCOV_RET" != 0 ]; then
      cmake $CMAKE_OPTIONS ..
    fi
  else
    if [ "$LCOV_RET" == 0 ]; then
      cmake -DCOVERAGE=OFF $CMAKE_OPTIONS ..
    fi
  fi
fi

SUCCESS="$?"
if [ "$SUCCESS" == 0 ]; then
  if [ "$CLANGCHECK" == 1 ]; then
    if [ "$CLANGCHECK_OPEN" == 1 ]; then
      OPTION="-V"
    else
      OPTION=""
    fi
    if [ -f "$BIN_NAME" ]; then
      make clean
    fi
    scan-build -enable-checker core -enable-checker security \
               -enable-checker deadcode -enable-checker llvm \
               -enable-checker cplusplus -enable-checker unix \
               $OPTION -o "../$CLANGCHECK_DIR" make
  else
    make
  fi
fi

if [ "$?" == 0 ]; then
  SUCCESS=0
  if [ "$NO_TESTS" == 0 ]; then
    echo
    ctest --output-on-failure -R run -D ExperimentalMemCheck
    # ctest -V -R api_sanity_checker
  fi
fi
cd ..

if [ "$CPPCHECK" == 1 ]; then
  if [ ! -d "$CPPCHECK_DIR" ]; then
    mkdir "$CPPCHECK_DIR"
  else
    rm "$CPPCHECK_DIR"/*.html 2>/dev/null
  fi
  echo
  cppcheck -i "$BUILD_DIR" --enable=all --suppress=missingIncludeSystem --xml . \
                                               2>"$CPPCHECK_DIR/report.xml" &&
  cppcheck-htmlreport --title="$BIN_NAME" --file "$CPPCHECK_DIR/report.xml" \
                      --report-dir "$CPPCHECK_DIR" --source-dir "src" &&
  if [ "$CPPCHECK_OPEN" == 1 ]; then
    xdg-open "$CPPCHECK_DIR/index.html"
  fi
fi

if [ "$API_SANITY_CHECKS" == 1 ]; then
  cd "$BUILD_DIR"
  echo
  api-sanity-checker -l "$BIN_NAME" -d "$SOURCE_DIR/api-sanity-checks.xml" \
                     -relpath "$SOURCE_DIR" -gen -build -run
  cd ..
fi

if [ "$CPPLINT" == 1 ]; then
  echo
  cpplint.py --linelength=84 src/*.h src/*.cpp
fi

if [ "$LCOV" == 1 ]; then
  echo
  cd "$BUILD_DIR"
  make "${BIN_NAME}_coverage"
  if [ "$?" == 0 -a "$LCOV_OPEN" == 1 ]; then
    xdg-open "coverage/index.html"
  fi
  cd ..
fi

if [ "$DOC" == 1 ]; then
  echo
  cd "$BUILD_DIR"
  make doc
  cd ..
fi

if [ "$SUCCESS" == 0 ]; then
  if [ "$RUN" == 1 ]; then
    wait_input
    echo
    "$BUILD_DIR/./$BIN_NAME"
  elif [ "$VALGRIND" == 1 ]; then
    wait_input
    echo
    valgrind --trace-children=yes --leak-check=full --show-reachable=yes "$BUILD_DIR/./$BIN_NAME"
  fi
fi
exit $SUCCESS
