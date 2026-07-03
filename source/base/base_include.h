#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H

#ifdef DEV
#define BASE_IMPLEMENTATION
#define BASE_LOGGING
#endif

#ifdef RELEASE
#define BASE_IMPLEMENTATION
#endif


//- platform definitios

#if   defined(__linux__)
#define PLATFORM_LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#else
#define PLATFORM_UNKNOWN
#endif

//-

#if   defined(PLATFORM_LINUX)

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE

#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#elif defined(PLATFORM_WINDOWS)
#include <windows.h>

#endif // PLATFORM_LINUX || PLATFORM_WINDOWS

//- Standard Library
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#ifdef BASE_LOGGING
#include <stdio.h>
#include <stdarg.h>
#endif // BASE_LOGGING

#ifdef BASE_ARM
#ifdef BASE_SIMD

#include <arm_neon.h>

#endif // BASE_SIMD
#endif // BASE_ARM

//-

#include "base_core.h"

//- Arena Allocator

#define BASE_ARENA_IMPLEMENTATION
#ifdef  BASE_ARENA_IMPLEMENTATION

#include "base_arena.h"

// platform specific allocation implementations
#if   defined(PLATFORM_WINDOWS)
#include "base_arena_windows.h"
#elif defined(PLATFORM_LINUX)
#include "base_arena_linux.h"
#endif // PLATFORM

#endif // BASE_ARENA_IMPLEMENTATION


//- stack includes

#include "base_stack.h"

#if   defined(PLATFORM_LINUX)
#include "base_stack_linux.h"
#elif defined(PLATFORM_WINDOWS)
#include "base_stack_windows.h"
#endif // defined(PLATFORM_LINUX)


//- string includes

#include "base_string.h"

#if   defined(PLATFORM_LINUX)
#include "base_os_linux.h"
#elif defined(PLATFORM_WINDOWS)
#include "base_os_windows.h"
#endif // PLATFORM_LINUX || PLATFORM_WINDOWS

#include "base_os.h"

//- misc.

#if      defined(BASE_MISC)

#if      defined(PLATFORM_WINDOWS)
#define  _write(buffer) unix_file_write(STDERR, (buffer), CStringLen(buffer))
#elif    defined(PLATFORM_LINUX)
#define  _write(buffer) win32_file_load(STDERR, (buffer), CStringLen(buffer))
#endif

#include "base_test.h"
#include "base_rand.h"

#endif // BASE_MISC

#endif // BASE_INCLUDE_H