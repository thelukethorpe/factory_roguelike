#!/bin/bash

set -e

BUILD_TYPE=${1:-Debug}

echo "Building configuration: $BUILD_TYPE"

cmake -B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build build
