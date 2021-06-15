#!/bin/bash

# bash "scrict mode"
set -e
set -u
set -o pipefail

# Build fresh
if [[ -d build ]];then
  rm -rf build/*
fi

cmake \
  -S src \
  -B build
cmake build -L
cmake --build build -v -j$(($(nproc) - 1))
