#ifndef BASE_IO_H
#define BASE_IO_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2



//-
#if defined(PLATFORM_WINDOWS)

internal void win32_log_with_args(char *format, ...);
internal void win32_log(String8List messages);
internal b32 win32_os_exit();
internal b32 win32_file_exists(u8 *path);
internal b32 win32_file_write(String8 data, String8 path);
internal u64 safe_truncate_64(u64 value);
internal String8 win32_file_load(MemArena *arena, u8 *file);

#define FileLoad(arena, path) win32_file_load(arena, path)
#define FileWrite(data, path) win32_file_write(data, path)
#define FileExists(path) win32_file_exists(path)
#define OSExit() win32_os_exit();

#if defined(BASE_LOGGING)
#define Log(format, ...)      win32_log_with_args((char *)(format), ##__VA_ARGS__)
#else
#define Log(format, ...)
#endif

#elif defined(PLATFORM_LINUX) // PLATFORM_WINDOWS

internal String8   unix_file_write(String8 data, String8 path);
internal String8   unix_file_load(MemArena *arena, String8 path);
internal b32       unix_file_exists(String8 path);
internal b32       unix_os_exit();

internal s64       unix_read(int fd, const void *buf, u64 count);
internal s64       unix_log(int fd, const void *buf, u64 count);
internal void      unix_log_with_args(char *format, ...);


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

#endif /* BASE_IO_H */
