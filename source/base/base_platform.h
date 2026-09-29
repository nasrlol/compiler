#ifndef BASE_PLATFORM_H
#define BASE_PLATFORM_H

#include "base_platform_render.h"

//- Linux

#if PLATFORM_LINUX
#include "base_platform_linux.h"
#elif PLATFORM_WINDOWS
#include "base_platform_win32.h"
#endif // PLATFORM_LINUX || PLATFORM_WINDOWS

#if PLATFORM_LINUX || PLATFORM_UNIX
#define ENTRYPOINT() int main(int count, char **args)

#define PlatformCreateWindow(arena, height, width, display_x, display_y)                           \
    LinuxCreateWindow(arena, height, width, display_x, display_y)

#define PlatformGetDisplayWidth(display, screen_num)                                               \
    LinuxDisplayWdith(display, screen_num) // defined externally

#define PlatformGetDisplayHeight(display, screen_num)                                              \
    LinuxDisplayHeight(display, screen_num) // defined externally

#define PlatformLoadBitmapFromFile(display, window, path, width, height, out, hotspot_x,           \
                                   hotspot_y)                                                      \
    LinuxLoadBitMapFromFile(display, window, path, width, height, out, hotspot_x, hotspot_y)

//- windows layer
#elif PLATFORM_WINDOWS

global_variable b32 Win32Running = true;

#if COMPILER_MSVC
#pragma warning(disable : 28251) // disabled the annotations warning that are defined in winbase.h
#define ENTRYPOINT(hinstance, prev_instance, cmd_line, show_cmd)                                   \
    int WINAPI WinMain(hinstance, prev_instance, cmd_line, show_cmd)
#endif // COMPIPILER_MSVC DISABLE WARNING

#define PlatformCreateWindow(instance, height, width, display_x, display_y)                        \
    Win32CreateWindow(instance, height, width, display_x, display_y)

#define STDIN 0
#define STDOUT 1
#define STDERR 2

internal String8List win32_open_dir(MemArena *arena, String8 *original_path); // TODO: ...
internal void win32_log(String8List messages);
internal b32 win32_os_exit();
internal b32 win32_file_exists(u8 *path);
internal b32 win32_file_write(String8 data, String8 path);
internal u64 safe_truncate_64(u64 value);
internal String8 win32_file_load(MemArena *arena, String8 file);

#define FileLoad(arena, path) win32_file_load(arena, path)
#define FileWrite(data, path) win32_file_write(data, path)
#define FileExists(path) win32_file_exists(path)
#define DirLoad(arena, path) win32_open_dir(arena, original_path);
#define OSExit() win32_os_exit();

#if COMPILER_GCC || COMPILER_CLANG

#define PRINTF_FORMAT(fmt, args) __attribute__((format(printf, fmt, args)))
internal void win32_logf(const char *fmt, ...) PRINTF_FORMAT;

#elif COMPILER_MSVC
internal void win32_logf(_Printf_format_string_ const char *fmt, ...);

#define Log(fmt, ...) win32_logf(fmt, __VA_ARGS__)

#endif // COMPILER_GCC || COMPILER_CLANG
#endif // PLATFORM_WINDOWS

#if PLATFORM_LINUX || PLATFORM_UNIX

typedef struct Directory Directory;
struct Directory {
    String8List *file_names;
};

//-  File I/O
internal b32 unix_file_write(String8 data, String8 path);
internal String8 unix_file_load(MemArena *arena, String8 path);
internal b32 unix_file_exists(String8 path);
internal String8List *unix_open_dir(MemArena *arena,
                                    String8 *original_path); // TODO: change the return value

internal s64 unix_read(int fd, const void *buf, u64 count);
internal void unix_os_exit() __attribute__((noreturn));

internal s64 unix_log(int fd, const void *buf, u64 count);
internal void unix_log_with_args(const char *format, ...);

//- File I/O
#define FileLoad(arena, path) unix_file_load(arena, path)
#define FileWrite(data, path) unix_file_write(data, path)
#define FileExists(path) unix_file_exists(path)
#define DirLoad(arena, path) unix_open_dir(arena, path)
#define OSExit() unix_os_exit();

#if COMPILER_GCC || COMPILER_CLANG
// TODO: __VA_ARGS__ CLANG
#define Log(...) unix_log_with_args(__VA_ARGS__)
#define LogStr8(message)                                                                           \
    Log(Str8Fmt, Str8Arg(message)) // NOTE: for this to wwork we need the /analyze argument
#endif                             // COMPILER_GCC || COMPILER_CLANG
#endif                             // PLATFORM_UNIX

#endif // BASE_PLATFORM_H
