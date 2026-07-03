#ifndef BASE_OS_LINUX_H
#define BASE_OS_LINUX_H

//-

internal b32
unix_file_exists(String8 path);
internal String8
unix_file_write(String8 path);

//- implementations
internal b32
unix_file_exists(String8 path)
{
 return false;
}

//- file stuff
internal String8
unix_file_load(MemArena *arena, String8 path)
{
 String8     result = {0};
 struct stat sbuf   = {0};

 s32 file = open((const char *)path.data, O_RDONLY);
 if(file == -1)
 {
  Log("file not loaded");
  return (String8){0};
 }

 if(fstat(file, &sbuf) == -1)
 {
  close(file);
  return str8_zero();
 }

 result      = PushString8(arena, (u64)sbuf.st_size);
 result.size = (u64)sbuf.st_size;
 if(result.size != 0)
  result.data = (u8 *)mmap(0, result.size, PROT_READ, MAP_PRIVATE, file, 0);

 close(file);
 return result;
}

internal String8
unix_file_write(String8 path, String8 data)
{
 String8 result = {0};
 s32     file   = open((const char *)path.data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 if(file == -1)
 {
  return (String8){0};

  u64 written = 0;
  s64 err     = write(file, data.data + written, data.size - written);
  while(written < data.size)
  {
   if(err == -1)
   {
    close(file);
    return str8_zero();
   }
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

// TODO: can this be replaced with the regular open?
// and then just passing a refernce to the STDIN File
internal s64
unix_read(int fd, const void *buf, u64 count)
{
 return (u64)syscall(SYS_read, fd, buf, count);
}

internal void
unix_log_with_args(char *format, ...)
{
 va_list arguments;
 va_start(arguments, format);
 vprintf(format, arguments);
 va_end(arguments);
}

internal s64
unix_log(int fd, const void *buf, u64 count)
{
 return (u64)syscall(SYS_write, (int)fd, buf, count);
}

#endif