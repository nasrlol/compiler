#!/bin/bash
set -e

date +"%T"

BUILD_DIR=build
SOURCE_DIR=source
INCLUDES="-I${SOURCE_DIR}/code -I${SOURCE_DIR} -I${SOURCE_DIR}/base"

if [0]; then
	gcc_options="-fanalyzer -no-pie"
else
	gcc_options=""
fi

CFLAGS="-g -fno-rtti -fno-exceptions -Wall -Werror  -Wno-unused-function -Wno-unused-variable $gcc_options"
LIBS="-luser32 -lgdi32 -lkernel32 -lshell32"

mkdir -p "$BUILD_DIR"

echo "Building ${BUILD_DIR}/program.exe..."
g++ $INCLUDES $CFLAGS "${SOURCE_DIR}/code/compiler.cpp" -o "${BUILD_DIR}/compiler.exe" $LIBS
