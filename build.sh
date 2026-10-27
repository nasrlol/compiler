#!/bin/bash
# author: nasr
set -e

date
echo "Compiling the compiler compiler [Fajr]"
echo

BUILD_DIR=build
SOURCE_DIR=source
INCLUDES="-I${SOURCE_DIR}/code -I${SOURCE_DIR} -I${SOURCE_DIR}/base"
CFLAGS="-fno-rtti -fno-exceptions -Wall -Wundef -Werror -fanalyzer -no-pie -Wno-unused-function -Wno-unused-variable -O0 -fno-omit-frame-pointer"
LIBS="-pthread"
TARGET="compiler"


DEBUG=1

if [ $DEBUG -eq 1 ]; then
	CFLAGS+=" -g3 -DDEBUG=1"
fi

mkdir -p "$BUILD_DIR"

echo "Target: [${TARGET}]"
echo "Debug mode $(if [ "${DEBUG:-0}" -eq 1 ]; then echo [TRUE]; else echo [FALSE]; fi)"
echo "Building ${BUILD_DIR}/compiler..."

g++ $INCLUDES $CFLAGS "${SOURCE_DIR}/code/compiler.cpp" -o "${BUILD_DIR}/${TARGET}" $LIBS