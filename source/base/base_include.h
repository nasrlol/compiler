#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#if 1
extern long syscall(long number, ...);
#endif

#if defined(__linux__)
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#elif defined(_WIN32)
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

#if 0
#include <arm_neon.h>
#endif

#include "base_core.h"
#include "base_arena.h"
#include "base_stack.h"
#include "base_string.h"
#include "base_os.h"
#include "base_test.h"
#include "base_rand.h"

#endif
