#ifndef BASE_PLATFORM_WIN32_H
#define BASE_PLATFORM_WIN32_H

typedef struct Win32WindowHandle Win32WindowHandle;
struct Win32WindowHandle {
    HWND window_handle;
    HINSTANCE instance;
    HDC dc;
};

internal b32 win32_os_exit();
internal void win32_handle_error(MemArena * arena, DWORD error);
internal u64 safe_truncate_64(u64 value);
internal b32 win32_file_exists(u8 * path);
internal b32 win32_file_write(String8 data, String8 path);
internal String8 win32_file_load(MemArena * arena, String8 file);
internal void* win32_load_library();
internal void win32_library_get_proc();
internal LRESULT CALLBACK win32_window_proc(HWND handle, UINT messages, WPARAM wparam, LPARAM lparam);
internal HWND win32_create_window(HINSTANCE instance, int height, int width, int display_x, int display_y, const char* window_name);

#define Win32CreateWindow(instance, height, width, display_x, display_y)  win32_create_window(instance, height, width, display_x, display_y)
#define Win32FileExists(path) win32_file_exists(path)

#endif // BASE_PLATFORM_WI32_H
