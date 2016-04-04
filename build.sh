#!/bin/sh

export CCC_CC=clang
export CCC_CXX=clang++

if [ ! -d build ]; then
  mkdir build
  cd build
  scan-build cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
else
  cd build
fi
scan-build make && echo "" &&
ctest -R run -D ExperimentalMemCheck
