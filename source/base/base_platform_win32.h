#ifndef BASE_PLATFORM_WIN32_H
#define BASE_PLATFORM_WIN32_H

typedef struct Win32WindowHandle Win32WindowHandle;
struct Win32WindowHandle
{
    HWND window_handle;
    HINSTANCE instance;
    HDC dc;
};

#define Win32CreateWindow(instance, height, width, display_x, display_y)  win32_create_window(instance, height, width, display_x, display_y)
#define Win32FileExists(path) win32_file_exists(path)

// TODO: ...
#if 0
win32_log(messages)
win32_os_exit()
win32_file_write(data, path)
win32_file_load(arena, file)
win32_load_library()
win32_library_get_proc()
win32_window_proc(handle, messages, wparam, lparam)
#endif

#endif // BASE_PLATFORM_WI32_H
