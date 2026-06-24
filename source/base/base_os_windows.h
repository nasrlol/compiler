// credits: luca
#ifndef BASE_OS_WINDOWS_H
#define BASE_OS_WINDOWS_H

internal inline
s32 safe_truncate_64(u64 value) {
  Assert(value <= (u64)~0);
  return (u32)value;
}

internal b32
_file_exists_win32(u8 *path) {
  DWORD attributes = GetFileAttributes(path);
  //
  // make sure that the file has proper attributes and is not a directory
  //
  return ( attributes != INVALID_FILE_ATTRIBUTES || !(Attributes & FILE_ATTRIBUTES_DIRECTORY));
}

//- file stuff

// TODO: fix
internal b32
_file_write_win32(MemArena *arena, u8 *file) {

  b32 result = false;

  HANDLE file_handle =  CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

  if(file_handle != INVALID_HANDLE_VALUE) {
    DWORD bytes_writen;
    if(WriteFile(file_handle, file.data, (DWORD)file.size, &bytes_written, 0)) {
      result = (bytes_writen == File.Size);
    } else {
      // TODO: Logging
    }
    CloseHandle(file_handle);
  } else {
    // TODO: Logging
  }
  return Result;

  return {0};

}

internal String8
_file_load_into_memory_win32(MemArena *arena, u8 *file) {

  String8 result = {0};

  HANDLE file_handle = CreateFileA(file,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   0,
                                   OPEN_EXISTING,
                                   0,
                                   0);

  if(file_handle != INVALID_HANDLE_VALUE) { // success

    u64 size;
    if(GetFileSizeEx(file_handle, &size)) {
      u32 file_size32 = safe_trunctateU64(size.quadpart);
      if(result.Data) {

        DWORD bytes_read;
        if(ReadFile(file, result.data, file_size32, &bytes_read, 0) &&
           (file_size32 == bytes_read)) {
          result.size = file_size32;
          CloseHandle(file_handle);
        } else {
          // TODO: Logging

          if(result.data) {
            VirtualFree(result.data, 0, MEM_RELEASE);
          }

          //
          // TODO: Luca says in his code result.data = 0; but doesn't he mean the size. seeying that
          // we just freed that memory? Or is he zero'ing the memory? we don't have access to it any more
          // so i don't see the point?
          // TODO: also why aren't we using memory arenas in this case and just doing a new allocation/
          //

          // result.Data = 0;
          result.size = 0;

        }
      } else {
        // TODO: Logging
      }
    } else {
      // TODO: Logging
    }
  } else {
    // TODO: Logging
  }

}

#endif