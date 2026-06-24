#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H


#if defined(__linux__) //- linux stuff

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#if 1
extern long syscall(long number, ...);
#endif

#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#elif defined(_WIN32) || defined(_WIN64) //- windows stuff
#include <windows.h>
#endif

#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#ifdef BASE_LOGGING
#include <stdio.h>
#include <stdarg.h>
#endif

#ifdef BASE_ARM
#include <arm_neon.h>
#endif

//- start base includes

#include "base_core.h"


//- arena includes
#define BASE_ARENA_IMPLEMENTATION
#ifdef  BASE_ARENA_IMPLEMENTATION

#include "base_arena.h"

// platform specific allocation implementations
#if defined(_WIN32)
#include "base_arena_windows.h"
#elif defined(__linux__)
#include "base_arena_linux.h"
#endif // #if defined(_WIN32)


//- stack includes

#define BASE_STACK_IMPLEMENTATION
#ifdef  BASE_STACK_IMPLEMENTATION

#if defined(__linux__)

#include "base_stack_linux.h"

#elif defined(_WIN32) || defined(_WIN64)

#include "base_stack_windows.h"

#endif // #if defined(__linux__)

#include "base_stack.h"

#endif // BASE_STACK_IMPLEMENTATION


//- string includes

#include "base_string.h"

//- os includes
#ifdef  BASE_OS_IMPLEMENTATION
#define BASE_OS_IMPLEMENTATION

#if defined(__linux__)
#include "base_os_linux.h"
#elif defined(_WIN32) || defined(_WIN64)
#include "base_os_windows.h"
#endif

#include "base_os.h"

#endif


//- misc.
#include "base_test.h"
#include "base_rand.h"

#endif // BASE_INCLUDE_H