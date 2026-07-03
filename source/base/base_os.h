#ifndef BASE_OS_H
#define BASE_OS_H

#define     STDIN        0
#define     STDOUT       1
#define     STDERR       2

//-
#if defined(PLATFORM_WINDOWS)

// File I/O
#define FileLoad(arena, path) win32_file_load(arena, path)
#define FileWrite(data, path) win32_file_write(data, path)
#define FileExists(path)      win32_file_exists(path)

//- logging
#if defined(BASE_LOGGING)
#define Log(format, ...)   unix_log_with_args((char *)((format), ##__VA_ARGS__)
#define LogStr8(buffer)    unix_log(buffer.data, buffer.size);
#else
#define Log(buffer, ...)
#define LogStr8(buffer)
#endif


#elif defined(PLATFORM_LINUX)

//- file I/O
#define FileLoad(arena, path) unix_file_load(arena, path)
#define FileWrite(data, path) unix_file_write(data, path)
#define FileExists(path)      unix_file_exists(path)

//- logging
#if defined(BASE_LOGGING)
#define Log(format, ...)   unix_log_with_args((char *)((format), ##__VA_ARGS__)
#define LogStr8(buffer)    unix_log(STDOUT ,buffer.data, buffer.size);
#else
#define Log(buffer, ...)
#define LogStr8(buffer)
#endif


#endif // PLATFORM IO


#endif /* BASE_OS_H */