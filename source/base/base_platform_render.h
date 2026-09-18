#ifndef PLATFORM_RENDER_H
#define PLATFORM_RENDER_H


//- generic window description
typedef struct PlatformWindow PlatformWindow;
struct PlatformWindow {
    void *window_handle;
    int width;
    int height;
    int display_x;
    int display_y;
    const char *name;
};

#endif // PLATFORM_RENDER_H