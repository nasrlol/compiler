#ifndef BASE_OS_H
#define BASE_OS_H

#if 1
extern long syscall(long number, ...);
#endif


//- file stuff
internal string8
_file_load_linux(MemArena *arena, const char *path)
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
_file_write_linux(const char *path, string8 data)
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

//- stdout stuff etc

//
// TODO: improve the API interface for a better linux logging system
// and prepare to seperate it from the windows api system
//

#if 1
internal s64
os_write(int fd, const void *buf, u64 count)
{
    return (u64)syscall(SYS_write, (int)fd, buf, count);
}
#endif

#if 0
internal ssize_t
os_write(int fd, const void *buf, size_t count)
{
    return (ssize_t) syscall(SYS_write, fd, buf, count);
}
#endif

internal s64
os_read(int fd, const void *buf, u64 count)
{
    return (u64)syscall(SYS_read, fd, buf, count);
}

internal void
os_print_stdout_format(char *format, ...)
{

#ifdef BASE_LOGGING
    va_list arguments;
    va_start(arguments, format);
    vprintf(format, arguments);
    va_end(arguments);
#endif
}



#endif
