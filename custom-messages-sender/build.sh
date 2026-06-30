#!/bin/bash

SRC_DIR='.'
BUILD_DIR='build'

# Build
cmake -S "$SRC_DIR" -B "$BUILD_DIR"
cd build && make -j4
