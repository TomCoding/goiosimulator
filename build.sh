#!/bin/sh

if [ ! -d build ]; then
  mkdir build
  cd build
  cmake ..
else
  cd build
fi
make && echo "" &&
ctest -R run -D ExperimentalMemCheck
