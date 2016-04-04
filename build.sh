#!/bin/bash

BIN_NAME="goiovalues"
CPPCHECK_DIR="cppcheck"
BUILD_DIR="build"

function help() {
  echo "Usage: $0 [options]
Options:
  --gcc, --g++        Use gcc/g++ compiler.
  --clang, --clang++  Use clang/clang++ compiler.
  --cppcheck          Execute cppcheck and generate report after build.
  --cppcheckopen      Same as --cppcheck, additionally open report in browser.
  -r, --run           Run program after compilation.
  -v, --valgrind      Run program after compilation with valgrind.
  -w, --wait          Wait for input before running program.
  -h, --help          Print this message and exit.
"
}

HELP=0
CPPCHECK=0
CHECK_OPEN=0
RUN=0
VALGRIND=0
WAIT=0
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
    "--cppcheck")
      CPPCHECK=1
      shift
      ;;
    "--cppcheckopen")
      CPPCHECK=1
      CHECK_OPEN=1
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
    "-w"|"--wait")
      WAIT=1
      if [ "$VALGRIND" == 0 ]; then
        RUN=1
      fi
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

if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi
cd "$BUILD_DIR"
if [ "$CXX_COMPILER" == "clang++" ]; then
  export CCC_CC=clang
  export CCC_CXX=clang++
  scan-build cmake -DCMAKE_CXX_COMPILER="$CXX_COMPILER" -DCMAKE_C_COMPILER="$C_COMPILER" ..
elif [ "$CXX_COMPILER" != "" ]; then
  cmake -DCMAKE_CXX_COMPILER="$CXX_COMPILER" -DCMAKE_C_COMPILER="$C_COMPILER" ..
fi

SUCCESS="$?"
if [ "$SUCCESS" == 0 ]; then
  if [ "$CXX_COMPILER" == "clang++" ]; then
    scan-build make
  else
    make
  fi
fi

if [ "$?" == 0 ]; then
  SUCCESS=1
  echo
  ctest -R run -D ExperimentalMemCheck
fi
cd ..

if [ "$CPPCHECK" == 1 ]; then
  if [ ! -d "$CPPCHECK_DIR" ]; then
    mkdir "$CPPCHECK_DIR"
  else
    rm "$CPPCHECK_DIR"/*.html 2>/dev/null
  fi
  echo
  cppcheck -i "$BUILD_DIR" --enable=all --xml . 2>"$CPPCHECK_DIR/report.xml" &&
  cppcheck-htmlreport --title="$BIN_NAME" --file "$CPPCHECK_DIR/report.xml" --report-dir "$CPPCHECK_DIR" --source-dir . &&
  if [ "$CHECK_OPEN" == 1 ]; then
    x-www-browser "$CPPCHECK_DIR/index.html"
  fi
fi

if [ "$SUCCESS" == 1 ]; then
  if [ "$WAIT" ]; then
    read -p "
press enter to run ./$BIN_NAME..."
  fi
  if [ "$RUN" == 1 ]; then
    echo
    "$BUILD_DIR/./$BIN_NAME"
  elif [ "$VALGRIND" == 1 ]; then
    echo
    valgrind --trace-children=yes --leak-check=full --show-reachable=yes "$BUILD_DIR/./$BIN_NAME"
  fi
fi
