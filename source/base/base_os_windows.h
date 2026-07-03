// credits: luca
#ifndef BASE_OS_WINDOWS_H
#define BASE_OS_WINDOWS_H

internal inline s32
safe_truncate_64(u64 value)
{
#if 0
 Assert(value <= (u64)~0);
 return (u32)value;

 #endif
 return 0;
 }

 internal b32
 win32_file_exists(u8 *path)
 {
 #if 0
 DWORD attributes = GetFileAttributes(path);
 return ( attributes != INVALID_FILE_ATTRIBUTES || !(Attributes & FILE_ATTRIBUTES_DIRECTORY));
 #endif

 return false;
 }

 //- file stuff

 // TODO: fix
 internal b32
 win32_file_write(MemArena *arena, String8 file)
 {
 #if 0
 b32 result = false;

 HANDLE file_handle =  CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

 if(file_handle != INVALID_HANDLE_VALUE) {
 DWORD bytes_writen;
 if(WriteFile(file_handle, file.data, (DWORD)file.size, &bytes_written, 0)) {
 result = (bytes_writen == file.size);
 } else {
 // TODO: Logging
 }
 CloseHandle(file_handle);
 } else {
 // TODO: Logging
 }

 #endif

 return false;
 }

 // IN: MemArena, u8 *
 // OUT: String8
 internal String8
 win32_file_load(MemArena *arena, u8 *file)
 {
 #if 0

 String8 result = {0};

 HANDLE file_handle = CreateFileA(file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
 if(file_handle != INVALID_HANDLE_VALUE) { // success

 u64 size;
 if(GetFileSizeEx(file_handle, &size)) {
 u32 file_size32 = safe_trunctateU64(size.quadpart);
 if(result.data) {

 DWORD bytes_read;
 if(ReadFile(file, result.data, file_size32, &bytes_read, 0) && (file_size32 == bytes_read)) {
 result.size = file_size32;
 CloseHandle(file_handle);
 } else {

 Log("Failed to read file");
 if(result.data)  VirtualFree(result.data, 0, MEM_RELEASE);

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
 Log("received data is NULL");
 }
 } else {
 Log("received data is of size 0");
 }
 } else {
 Log("received invalid handle");
 }

 return result;
 #endif

 return {0};
 }


 // TODO: rewrite this buillshit
 internal void
 win32_log_with_args(char *format, ...)
 {
 char buffer[4096];
 va_list args;
 va_start(args, format);
 int len = vsnprintf(buffer, sizeof(buffer), format, args);
 va_end(args);

 if (len < 0) return;
 if ((u64)len >= sizeof(buffer)) len = sizeof(buffer) - 1;

 HANDLE handle_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
 DWORD written = 0;
 WriteConsoleA(handle_stdout, buffer, (DWORD)len, &written, NULL);
 }

 internal void
 win32_log(String8List messages)
 {
 u64 count = 0;
 for(String8Node *node = node->current; count < messages.length; node = node->next){
 HANDLE handle_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
 DWORD written = 0;
 u8 *message = node->value.data;
 WriteConsole(handle_stdout, (const)message.data, (DWORD)(message.size), &written, NULL);
 Assert(written > 0);
 ++count;
 }
 }

 //
 // exit win32
 // returns a boolean. i don't know what to do with the boolean if it fails hahahahhaa
 // not like you can exit hahahahaha0
 //
 internal b32
 win32_os_exit()
 {
 BOOL result =  ExitWindows(0, 0);
 return result;
}
#endif
