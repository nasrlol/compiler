#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H

#include <dirent.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "base.h"
#include "base_mem.h"
#include "base_arena.h"
#include "base_stack.h"
#include "base_test.h"
#include "base_string.h"
#include "base_os.h"

#ifdef BASE_UNITY

#include "base_arena.c"
#include "base_stack.c"

#endif
#endif
