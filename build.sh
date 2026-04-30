#!/bin/bash

set -eu

DIR="$(dirname "$(readlink -f "$0")")"
cd "$DIR"

BUILD="build"
mkdir -p "$BUILD"

COMPILER="clang"
CLEAN=0
ASAN=0
DEBUG=1
RELEASE=0
TARGET=0

[ "$#" = 0 ] && TARGET=1
for ARG in "$@"; do eval "$ARG=1" 2>/dev/null || :; done

[ "$RELEASE" = 1 ] && DEBUG=0

compile() {
    local SOURCE="$1"
    local OUT="$2"
    local EXTRA="${3:-}"

    local COMMON="-I$DIR -I$DIR/source -D_GNU_SOURCE=1 -fno-threadsafe-statics -nostdinc++"
    local WARNINGS="-Wall -Wextra -Wconversion -Wno-sign-conversion -Wno-unused-parameter -Wno-missing-field-initializers -Wno-main-return-type -Wno-unused-variable -Wno-unused-but-set-variable -Wno-switch -Wno-unused-function"
    local CLANG="-fno-omit-frame-pointer -fdiagnostics-absolute-paths -Wno-null-dereference"
    local LINKER="-lm -ldl -lpthread"

    local FLAGS="$COMMON $WARNINGS $CLANG"

    [ "$DEBUG" = 1 ]   && FLAGS="$FLAGS -g -ggdb3"
    [ "$RELEASE" = 1 ] && FLAGS="$FLAGS -O3"
    [ "$ASAN" = 1 ]    && FLAGS="$FLAGS -fsanitize=address -fsanitize-trap -Isource"

    echo "Building $OUT..."
    $COMPILER $FLAGS $SOURCE $EXTRA $LINKER -o "$BUILD/$OUT"
}

if [ "$CLEAN" = 1 ]; then
    rm -rf "$BUILD"/*
    echo "Cleaned $BUILD"
fi

if [ "$TARGET" = 1 ] || [ "${compiler:-0}" = 1 ]; then
    compile "./source/compiler/compiler_main.c" "compiler"
fi

echo "Done."
