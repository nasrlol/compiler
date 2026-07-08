#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H

#define BASE_LOGGING

#if defined(__linux__)
#define PLATFORM_LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#else
#define PLATFORM_UNKNOWN
#endif

#if defined(PLATFORM_LINUX)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <memoryapi.h>
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

//-
#include "base_core.h"
#include "base_arena.h"
#include "base_string.h"
#include "base_io.h"
#include "base_stack.h"


//-


#include "base_arena.cpp"
#include "base_string.cpp"
#if defined(PLATFORM_LINUX)

#include "base_io_unix.cpp"
#include "base_stack_unix.cpp"

#elif defined(PLATFORM_WINDOWS)
#include "base_io_win32.cpp"
#include "base_stack_win32.cpp"

#include "base_rand.cpp"
#include "base_simd.cpp"

#endif

internal void
Assert(b32 expression)
{
    if(!(expression))
    {
        Log("[ASSERT] %s : %d : %s \n", __FILE__ , __LINE__, __func__ );
        OSExit();
    }
}



#endif