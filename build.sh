#!/bin/bash
set -e

echo
date
echo "Compiling compiler"
echo

BUILD_DIR=build
SOURCE_DIR=source
INCLUDES="-I${SOURCE_DIR}/code -I${SOURCE_DIR} -I${SOURCE_DIR}/base"
CFLAGS="-fno-rtti -fno-exceptions -Wall -Wundef -Werror -fanalyzer -no-pie -Wno-unused-function -Wno-unused-variable -O0"
LIBS="-pthread"

DEBUG=1

if [ $DEBUG -eq 1 ]; then
	CFLAGS+=" -g -DDEBUG=1"
fi

mkdir -p "$BUILD_DIR"

echo "Building ${BUILD_DIR}/compiler..."
g++ $INCLUDES $CFLAGS "${SOURCE_DIR}/code/compiler.cpp" -o "${BUILD_DIR}/compiler" $LIBS
