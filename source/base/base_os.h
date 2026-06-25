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
#define Log(format, ...) os_write_stdout((char *)(format), ##__VA_ARGS__)
#define LogS8(buffer)    os_write_stdout(buffer.data, buffer.size);
#else
#define Log(buffer)   // stub
#define LogS8(buffer) // stub
#endif

// @TODO: this should be put in a platform guard
#define FileLoad(arena, path) _file_load_into_memory_win32(arena, path)
#define FileWrite(data, path) _file_write_win32(data, path)
#define FileExists(path)      _file_exists_win32(path)

//- file reading, writing and checking
#if defined(__linux__)
// @TODO:
#elif defined(_WIN32) || defined(_WIN64)
// @TODO:
#endif // defined(__linux__)

#endif /* BASE_OS_H */