#ifndef BASE_H
#define BASE_H

/* assert an expression and output the file and the line */
#define internal        static
#define global_variable static
#define local_persist   static

#define arena_align (2 * sizeof(void *))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define unused(x) (void)(x)
#define NIL 0

#define DEPRECATED __attribute__((__deprecated__))

#if defined(__arm__) || defined(__aarch64__)
#define breakpoint __asm__ volatile("brk #0");
#define temp_breakpoint __asm__ volatile("udf #0");
#elif defined(__i386__) || defined(__x86_64__)
#define breakpoint __asm__ volatile("int3");
#endif


#define MemCpy(dest, src, len) memcpy((dest), (src), (len))
#define MemSet(dest, len) memset((dest), (0), (len))

#if COMPILER_MSVC
    #pragma section(".rdata$", read)
    #define read_only __declspec(allocate(".rdata$"))
#elif defined(__GNUC__) || defined(__clang__)
    #define read_only __attribute__((section(".rodata")))
#else
    #define read_only
#endif

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

#if 0
#define Red         "\x1b[31m"
#define Green       "\x1b[32m"
#define Reset       "\x1b[0m"
#define Blue        "\x1b[34m"
#define Yellow      "\x1b[33m"
#endif

#define Len(s) (sizeof(s) - 1)

internal inline b8
is_pow(umm x)
{
    return (x & (x - 1)) == 0;
}

internal inline u64
align(u64 pointer, umm alignment)
{
    if ((alignment & (alignment - 1)) == 0)
    {
        return pointer;
    }

    return (pointer + alignment - 1) & ~(alignment - 1);
}


#endif


