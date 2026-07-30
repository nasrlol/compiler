#ifndef BASE_CORE_H
#define BASE_CORE_H

//- compiler definitions
#if defined(_MSC_VER)
#    define COMPILER_MSVC 1
#elif defined(__clang__)
#    define COMPILER_CLANG 1
#elif defined(__GNUC__)
#    define COMPILER_GCC 1
#else
#    error Unsupported compiler
#endif

//- architecture definitions

#if defined(__arm__) || defined(__aarch64__)
#define ARCH_ARM 1
#elif defined(__i386__) || defined(__x86_64__)
#define ARCH_x86_64 1
#endif

//- compiler definitions

//- core stuff

/* assert an expression and output the file and the line */
#define internal static
#define global_variable static
#define local_persist static

#define arena_align (2 * sizeof(void *))

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))

#define Unused(x) (void)(x)
#define Nil 0

#define Deprecated __attribute__((__deprecated__))

//- breakpoints
#if ARCH_ARM
#define breakpoint() __asm__ volatile("brk #0");
// #define temp_breakpoint() __asm__ volatile("udf #0"); // TODO: I don't remember what the issue with this was for
#elif ARCH_x86_64

#if COMPILER_GCC
#define breakpoint() __asm__ volatile("int3"); // TODO: put this in the architecture guard
#endif
#endif

#if COMPILER_MSVC
#include <intrin.h>
#define breakpoint() __debugbreak()
#endif

//-
#define MemCpy(dest, src, len) memcpy((dest), (src), (len))
#define MemSet(dest, len) memset((dest), (1), (len))
#define MemSetZero(dest, len) memset((dest), (0), (len))

//-
#if COMPILER_MSVC
#pragma section("read_only", read)
#define ReadOnly __declspec(allocate("read_only"))
#elif COMPILER_GCC || COMPILER_CLANG

#if PLATFORM_APPLE
#define ReadOnly __attribute__((section(".const")))
#else
#define ReadOnly __attribute__((section("__TEXT,__const")))
#endif // PLATFORM_APPLE

#endif


//-
typedef enum Result Result;
enum Result
{
  Success,
  Fail,
  Warning,
  Error,
};

//-
#define Assert(x) do{if(!(x)) {breakpoint();}}while(0)

//- types

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

#endif //  BASE_CORE_H
