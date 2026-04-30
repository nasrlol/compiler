#ifndef BASE_OS_H
#define BASE_OS_H

#define STDIN_FD  0
#define STDOUT_FD 1
#define STDERR_FD 2

#define os_write_stdout(buf, count) _os_write(STDOUT_FD, (buf), (count))
#define os_write_stderr(buf, count) _os_write(STDERR_FD, (buf), (count))
#define os_write_stdin(buf, count)  _os_write(STDIN_FD,  (buf), (count))

#define os_read_stdout(buf)

#ifdef BASE_LOGGING
#define _log(format, ...) os_print_stdout_format((char *)(format), ##__VA_ARGS__)
#define _logs8(buffer) os_write_stdout(buffer.data, buffer.size);
#else
#define _log(buffer)
#define _logs8(buffer)
#endif


internal string8
file_load(mem_arena *arena, const char *path)
{
    string8 result = {0};
    struct stat sbuf = {0};

    s32 file = open(path, O_RDONLY);
    if(file == -1)
    {
#if 0
        _log("file not loaded");
#endif
        return (string8){0};
    }

    if(fstat(file, &sbuf) == -1)
    {
        close(file);
        return (string8){0};
    }

    result = PushString8(arena, sbuf.st_size);
    result.size = (u64)sbuf.st_size;
    if(result.size != 0)
    {
        result.data = (u8 *)mmap(0, result.size, PROT_READ, MAP_PRIVATE, file, 0);
    }

    close(file);
    return result;
}

internal string8
file_write(const char *path, string8 data)
{
    string8 result = {0};
    s32 file = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(file == -1)
    {
        return (string8){0};
    }

    u64 written = 0;
    while(written < data.size)
    {
        s64 err = write(file, data.data + written, data.size - written);
        if(err == -1)
        {
            close(file);
            return (string8){0};
        }
        written += err;
    }

    close(file);
    result = data;
    return result;
}

#if 1
internal s64
_os_write(int fd, const void *buf, u64 count)
{
    return (u64)syscall(SYS_write, (int)fd, buf, count);
}
#endif

#if 0
internal ssize_t
_os_write(int fd, const void *buf, size_t count)
{
    return (ssize_t) syscall(SYS_write, fd, buf, count);
}
#endif

internal s64
_os_read(int fd, const void *buf, u64 count)
{
    return (u64)syscall(SYS_read, fd, buf, count);
}

internal void
os_print_stdout_format(char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    vprintf(format, arguments);
    va_end(arguments);
}

#endif /* BASE_OS_H */
