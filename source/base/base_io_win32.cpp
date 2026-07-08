// TODO: rewrite this bullshit
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
    HANDLE handle_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    u64 count = 0;
    for(String8Node *node = messages.first; count < messages.count; node = node->next)
    {
        DWORD written = 0;
        WriteConsoleA(handle_stdout, node->value.data, (DWORD)(node->value.size), &written, NULL);
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
    ExitProcess(0);
    return true;
}

internal u64
safe_truncate_64(u64 value)
{
    if(value <= (u64)~0)
    {
        return 0; // TODO: this needs to be an assert. removed it for now to fix the include issues.
    }
    return (u64)value;
}

internal b32
win32_file_exists(u8 *path)
{
    DWORD attributes = GetFileAttributesA((LPCSTR)path);
    return ((attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

internal b32
win32_file_write(String8 data, String8 path)
{
    b32 result = false;

    HANDLE file_handle = CreateFileA((LPCSTR)path.data, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

    if(file_handle != INVALID_HANDLE_VALUE) {
        DWORD bytes_written;
        if(WriteFile(file_handle, data.data, (DWORD)data.size, &bytes_written, 0)) {
            result = (bytes_written == data.size);
        } else {
            Log("Writing the file was unsuccessful");
        }
        CloseHandle(file_handle);
    } else {
        Log("File handle has the INVALID_HANDLE_VALUE flag.");
    }

    return result;
}

internal String8
win32_file_load(MemArena *arena, u8 *file)
{
    String8 result = {0};

    HANDLE file_handle = CreateFileA((LPCSTR)file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(file_handle != INVALID_HANDLE_VALUE) { // success

        LARGE_INTEGER size;
        if(GetFileSizeEx(file_handle, &size)) {
            u32 file_size32 = safe_truncate_64(size.QuadPart);
            result = PushString8(arena, file_size32);
            if(result.data) {

                DWORD bytes_read;
                if(ReadFile(file_handle, result.data, file_size32, &bytes_read, 0) && (file_size32 == bytes_read)) {
                    result.size = file_size32;
                    CloseHandle(file_handle);
                } else {

                    Log("Failed to read file");
                    if(result.data)  VirtualFree(result.data, 0, MEM_RELEASE);

                    //
                    // TODO: Luca says in his code result.data = 0; but doesn't he mean the size. seeing that
                    // we just freed that memory? Or is he zeroing the memory? we don't have access to it any more
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
}
