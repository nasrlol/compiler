#ifndef BASE_OS_H
#define BASE_OS_H

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
#define Log(format, ...) os_write_stdout((char *)((format), ##__VA_ARGS__)
#define LogS8(buffer)    os_write_stdout(buffer.data, buffer.size);
#else
#define Log(buffer)
#define LogS8(buffer)
#endif


//- file reading, writing and checking
#if PLATFORM_LINUX
#define FileLoad(arena, path) win32_file_load(arena, path)
#define FileWrite(data, path) win32_file_write(data, path)
#define FileExists(path)      win32_file_exists(path)
#elif PLATFORM_WINDOWS
#define FileLoad(arena, path) unix_file_load(arena, path)
#define FileWrite(data, path) unix_file_write(data, path)
#define FileExists(path)      unix_file_exists(path)
#endif // PLATFORM IO

#endif /* BASE_OS_H */