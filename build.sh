#!/bin/bash

# bash "scrict mode"
set -e
#set -u
set -o pipefail

# Build fresh
if [[ -d build ]];then
  rm -rf build/*
fi

source scl_source enable devtoolset-8

cmake \
  -S src \
  -B build
cmake build -L -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -v -j$(($(nproc) - 1))
