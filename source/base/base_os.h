#ifndef BASE_OS_H
#define BASE_OS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#if defined(PLATFORM_LINUX)
#include "base_os_unix.h"
#elif defined(PLATFORM_WINDOWS)
#include "base_os_windows.h"
#endif // PLATFORM_LINUX || PLATFORM_WINDOWS


//-
#if defined(PLATFORM_WINDOWS)

// File I/O
#define FileLoad(arena, path) win32_file_load(arena, path)
#define FileWrite(data, path) win32_file_write(data, path)
#define FileExists(path)      win32_file_exists(path)

#define OSExit()              win32_os_exit();

//- Logging
#if defined(BASE_LOGGING)
#define Log(format, ...)      win32_log_with_args((char *)(format), ##__VA_ARGS__)
#else
#define Log(format, ...)
#endif

#elif defined(PLATFORM_LINUX) // PLATFORM_WINDOWS

//- File I/O
#define FileLoad(arena, path) unix_file_load(arena, path)
#define FileWrite(data, path) unix_file_write(data, path)
#define FileExists(path)      unix_file_exists(path)

#define OSExit() unix_os_exit();

#if defined(BASE_LOGGING)
#define Log(format, ...) unix_log_with_args((char *)(format), ##__VA_ARGS__)
#else
#define Log(format, ...)
#endif // BASE_LOGGING

#endif // File I/O

internal void
Assert(b32 expression)
{
    if(!(expression))
    {
        Log("[ASSERT] %s : %s : %s \n", __FILE__ , __LINE__, __func__ );
        OSExit();
    }
}

#endif /* BASE_OS_H */
