#!/bin/bash

set -e


BUILD_DIR=./build
SOURCE_DIR=source
PROGRAM_DIR=test

INCLUDES="-I${SOURCE_DIR}/code -I${SOURCE_DIR} -I${SOURCE_DIR}/base"
CFLAGS="-g -fno-rtti -fno-exceptions -Wall -Werror -fanalyzer -no-pie -Wno-unused-function -Wno-unused-variable"

mkdir -p "$BUILD_DIR"

g++ $INCLUDES $CFLAGS "tests/base/test.cpp" -o "${BUILD_DIR}/test"
