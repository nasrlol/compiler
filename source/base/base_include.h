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
#endif // _GNU_SOURCE
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <memoryapi.h>
#endif // WIN32

#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#ifdef BASE_LOGGING
#include <stdio.h>
#include <stdarg.h>
#endif // BASE_LOGGING

#include "base_core.h"
#include "base_rand.h"
#include "base_os.h"
#include "base_string.h"
#include "base_arena.h"
#include "base_stack.h"

#endif // BASE_INCLUDE_H