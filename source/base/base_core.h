#ifndef BASE_CORE_H
#define BASE_CORE_H

//- compiler definitions
#if   defined(_MSC_VER)
#define COMPILER_MSVC
#elif defined(__GNUC__)
#define COMPILER_GCC
#elif defined(__clang__)
#define COMPILER_CLANG
#endif

#ifndef COMPILER_CLANG
#define COMPILER_CLANG
#endif

#ifndef COMPILER_GCC
#define COMPILER_GCC
#endif

#ifndef COMPILER_MSVC
#define COMPILER_MSVC
#endif

//- architecture definitions

#if defined(__arm__) || defined(__aarch64__)
#define ARCH_ARM
#elif defined(__i386__) || defined(__x86_64__)
#define ARCH_x84_64
#endif


//- compiler definitions

//- core stuff

/* assert an expression and output the file and the line */
#define internal        static
#define global_variable static
#define local_persist   static

#define arena_align (2 * sizeof(void *))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define unused(x) (void)(x)
#define NIL 0

#define deprecated __attribute__((__deprecated__))

//- brakpoints
#if defined(__arm__) || defined(__aarch64__)
#define breakpoint __asm__ volatile("brk #0");
#define temp_breakpoint __asm__ volatile("udf #0");

#elif defined(__i386__) || defined(__x86_64__)

#ifdef COMPILER_GCC
#define breakpoint __asm__ volatile("int3"); // TODO: put this in the architecture guard
#endif


#ifdef COMPILER_MSVC
#include <intrin.h>
#define breakpoint() __debugbreak()
#endif


//-

#define MemCpy(dest, src, len) memcpy((dest), (src), (len))
#define MemSetZero(dest, len) memset((dest), (0), (len))
#define MemSetZero(dest, len) memset((dest), (1), (len))

#pragma section("read_only",read)

#if defined(_MSC_VER)
#define ReadOnly __declspec(allocate("read_only"))
#elif defined(__GNUC__) || defined(__clang__)
#define ReadOnly __attribute__((section(".rodata")))
#endif

//- core types

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float  f32;
typedef double f64;

typedef s32 b32;
typedef s16 b16;
typedef s8  b8;

typedef uintptr_t umm;
typedef intptr_t  smm;

#define True    (1 == 1)
#define False   (1 != 1)

#endif //  BASE_CORE_H