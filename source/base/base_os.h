#ifndef BASE_OS_LINUX_H
#define BASE_OS_LINUX_H

#if defined(__linux__)
#include "base_os_linux.h"
#elif defined(__WIN32)
#include "base_os_windows.h"
#endif

#define STDIN_FD  0
#define STDOUT_FD 1
#define STDERR_FD 2

//- logging helpers
#define os_write_stdout(buf, count) _os_write(STDOUT_FD, (buf), (count))
#define os_write_stderr(buf, count) _os_write(STDERR_FD, (buf), (count))
#define os_write_stdin(buf, count)  _os_write(STDIN_FD,  (buf), (count))

#define os_read_stdout(buf)

//- logging
#ifdef BASE_LOGGING
#define Log(format, ...) os_print_stdout_format((char *)(format), ##__VA_ARGS__)
#define LogS8(buffer) os_write_stdout(buffer.data, buffer.size);
#else
#define Log(buffer)   // stub
#define LogS8(buffer) // stub
#endif


//- file reading and handling
#define FileLoad(arena, path) _file_load(arena, path)
#define FileWrite(data, path) _file_write(data, path)

#endif /* BASE_OS_LINUX_H */