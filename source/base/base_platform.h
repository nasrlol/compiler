#ifndef BASE_PLATFORM_H
#define BASE_PLATFORM_H


typedef struct PlatformWindow PlatformWindow;
struct PlatformWindow {
    void *window_handle;
    int width;
    int height;
    int display_x;
    int display_y;
    const char *name;
};


// TODO: merge the 2 differnet header files. they should be working correctly at the moment but it's kind of a mess.

//- linux layer
#if PLATFORM_LINUX
#include "base_platform_linux.h"
#elif PLATFORM_WINDOWS
#include "base_platform_win32.h"
#endif // PLATFORM_LINUX || PLATFORM_WINDOWS

#if PLATFORM_LINUX || PLATFORM_UNIX
#define ENTRYPOINT() int main(int count, char **args)

#define PlatformCreateWindow(arena, height, width, display_x, display_y) \
LinuxCreateWindow(arena, height, width, display_x, display_y)

#define PlatformGetDisplayWidth(display, screen_num ) \
LinuxDisplayWdith(display, screen_num)
#define PlatformGetDisplayHeight(display, screen_num) LinuxDisplayHeight(display, screen_num)
#define PlatformLoadBitmapFromFile(display, window, path, width, height, out, hotspot_x, hotspot_y) LinuxLoadBitMapFromFile(display, window, path, width, height, out, hotspot_x, hotspot_y)

//- windows layer
#elif PLATFORM_WINDOWS

//- global variables

global_variable b32 Win32Running = true;

//-
#if COMPILER_MSVC
#pragma warning(disable : 28251) // disabled the annotations warning that are defined in winbase.h
#define  ENTRYPOINT(hinstance, prev_instance, cmd_line, show_cmd) int WINAPI WinMain(hinstance, prev_instance, cmd_line, show_cmd)

#endif // COMPIPILER_MSVC DISABLE WARNING
#define PlatformCreateWindow(instance, height, width, display_x, display_y) Win32CreateWindow(instance, height, width, display_x, display_y)
#endif // PLATFORM_UNIX && PLATFORM_LINUX

#define STDIN  0
#define STDOUT 1
#define STDERR 2

//-
#if PLATFORM_WINDOWS

typedef enum KeyboardCode KeyboardCode;
enum KeyboardCode {
    
    
};

void win32_handle_keyboard_input_callback();

internal void win32_log(String8List messages);
internal b32  win32_os_exit();
internal b32  win32_file_exists(u8 *path);
internal b32  win32_file_write(String8 data, String8 path);
internal u64  safe_truncate_64(u64 value);
internal String8 win32_file_load(MemArena *arena, String8 file);

#define FileLoad(arena, path) win32_file_load(arena, path)
#define FileWrite(data, path) win32_file_write(data, path)
#define FileExists(path) win32_file_exists(path)
#define OSExit() win32_os_exit();

#if COMPILER_GCC || COMPILER_CLANG

#define PRINTF_FORMAT(fmt, args) __attribute__((format(printf, fmt, args)))
internal void  win32_logf(const char *fmt, ...) PRINTF_FORMAT;

#elif COMPILER_MSVC
internal void  win32_logf(_Printf_format_string_ const char *fmt, ...);

#define Log(fmt, ...) win32_logf(fmt, __VA_ARGS__)

#endif // COMPILER_GCC || COMPILER_CLANG
#endif // PLATFORM_WINDOWS

#if PLATFORM_LINUX || PLATFORM_UNIX

internal String8 unix_file_write(String8 data, String8 path);
internal String8 unix_file_load(MemArena *arena, String8 path);
internal b32 unix_file_exists(String8 path);
internal b32 unix_os_exit();
internal s64 unix_read(int fd, const void *buf, u64 count);
internal s64 unix_log(int fd, const void *buf, u64 count);
internal void unix_log_with_args(const char *format, ...);

//- File I/O
#define FileLoad(arena, path) unix_file_load(arena, path)
#define FileWrite(data, path) unix_file_write(data, path)
#define FileExists(path) unix_file_exists(path)
#define OSExit() unix_os_exit();

#if COMPILER_GCC || COMPILER_CLANG
// TODO: I know that the __VA_ARGS are the correct ones for gcc but also check them out for clang
#define Log(...) unix_log_with_args(__VA_ARGS__)
#elif COMPILER_MSVC
#define Log(message, ...) unix_log_with_args(_Printf_format_string_ message, ...) // TODO: __VA_ARGS what is the implementation for msvc
// TODO: for this to wwork we need the /analyze argument
#endif // COMPILER_GCC || COMPILER_CLANG
#endif // PLATFORM_UNIX

#endif // BASE_PLATFORM_H
