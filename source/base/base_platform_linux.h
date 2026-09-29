#ifndef BASE_LINUX_H
#define BASE_LINUX_H

typedef struct Library Library;
struct Library {
    u64 data;
};

#define X11 0
#define WAYLAND 1

typedef struct LinuxWindowHandle LinuxWindowHandle;
struct LinuxWindowHandle {
    Display *display;
    Window window;
};

#if X11
internal LinuxWindowHandle x11_create_window(MemArena *arena, u32 window_height, u32 window_width,
                                             u32 disp_x, u32 disp_y);
internal void x11_draw(LinuxWindowHandle *s, s32 start_x, s32 end_x, s32 start_y, s32 end_y);
internal s32 x11_cleanup(LinuxWindowHandle *s);
internal s32 x11_read_input();

#define LinuxCreateWindow(arena, height, width, x, y) x11_create_window(arena, height, width, x, y)
#define LinuxGetDisplayWdith(display, screen_num) DisplayWidth(display, screen_num)
#define LinuxGetDisplayHeight(display, screen_num) DisplayHeight(display, screen_num);
#define LinuxLoadBitMapFromFile(display, window, path, width, height, out, hotspot_x, hotspot_y)   \
    XReadBitmapFile(display, window, )

#elif WAYLAND

#else
#error compositor not defined
#endif

internal void unix_log_with_args(const char *format, ...);
internal void *unix_library_get_proc(Library *library, String8 proc_name);
internal Library unix_library_load(MemArena *arena, String8 path);
internal int unix_library_close(Library *library);
internal void unix_os_exit() __attribute__((noreturn));
internal b32 unix_file_exists(String8 path);
internal String8 unix_file_load(MemArena *arena, String8 path);
internal b32 unix_file_write(String8 data, String8 path);

#endif // BASE_LINUX_H
