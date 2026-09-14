#!/bin/bash

date
BUILD_DIR=build
SOURCE_DIR=source
INCLUDES="-I${SOURCE_DIR}/code -I${SOURCE_DIR} -I${SOURCE_DIR}/base"
CFLAGS="-g -fno-rtti -fno-exceptions -Wall -Werror -fanalyzer -no-pie -Wno-unused-function -Wno-unused-variable"

mkdir -p "$BUILD_DIR"

echo "Building ${BUILD_DIR}/program.exe..."
g++ $INCLUDES $CFLAGS "${SOURCE_DIR}/code/compiler.cpp" -o "${BUILD_DIR}/compiler"