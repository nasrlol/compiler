#if UI
#if PLATFORM_LINUX
internal LinuxWindowHandle x11_create_window(MemArena *arena, u32 window_height, u32 window_width,
                                             u32 display_x, u32 display_y)
{

    Display *main_display = XOpenDisplay(0);
    if (!main_display) {
        Log("Error! Failed to open x11 display. Are you on X11?");
    } else {
        Log("Successfully opened x11 display?\n");
        Log("Display: %p\n", main_display);
    }

    Window root = XDefaultRootWindow(main_display);
    int screen  = DefaultScreen(main_display);

    Visual *v = DefaultVisual(main_display, screen);

    XSetWindowAttributes wa = {.background_pixmap = None,
                               .background_pixel =
                                   BlackPixel(main_display, DefaultScreen(main_display)),
                               .border_pixmap         = CopyFromParent,
                               .border_pixel          = 0,
                               .bit_gravity           = ForgetGravity,
                               .win_gravity           = NorthWestGravity,
                               .backing_store         = NotUseful,
                               .backing_planes        = 1,
                               .backing_pixel         = 0,
                               .save_under            = False,
                               .event_mask            = 0,
                               .do_not_propagate_mask = 0,
                               .override_redirect     = False,
                               .colormap              = CopyFromParent,
                               .cursor                = None};

    Window window = XCreateWindow(main_display,   // display
                                  root,           // parent
                                  display_x,      // x
                                  display_y,      // y
                                  window_width,   // width
                                  window_height,  // height
                                  0,              // border_width
                                  CopyFromParent, // depth
                                  CopyFromParent, // class
                                  v,              // depth
                                  CWBackPixel,    // visual
                                  &wa);

    XSetWindowBorder(main_display, window, 60);
    XSelectInput(main_display, window,
                 ExposureMask | StructureNotifyMask | KeyReleaseMask | KeyPressMask);
    XMapWindow(main_display, window);

    LinuxWindowHandle out = {main_display, window};
    return out;
}

internal void x11_draw(LinuxWindowHandle *s, s32 start_x, s32 end_x, s32 start_y, s32 end_y)
{

    u32 color = 0x55ffaaff;
    GC gc     = XCreateGC(s->display, s->window, 0, Nil);
    XSetForeground(s->display, gc, color);

    XFreeGC(s->display, gc);
}

internal s32 x11_cleanup(LinuxWindowHandle *s)
{
    XCloseDisplay(s->display);
    return 0;
}

internal s32 x11_read_input() { return 0; }

#endif // PLATFORM_LINUX
#endif

//-

internal Library unix_library_load(MemArena *arena, String8 path)
{

    const char *cstring_path = null_terminate(arena, &path);
    void *lib                = dlopen((const char *)path.data, RTLD_LAZY | RTLD_LOCAL);
    return {(u64)lib};
}

internal void *unix_library_get_proc(Library *library, String8 proc_name) { return NULL; }

internal int unix_library_close(Library *library)
{
    int result = dlclose(library);

    if (result) {
    }

    return result;
}

//- implementations
internal b32 unix_file_exists(String8 path) { return access((const char *)path.data, F_OK) == 0; }

internal b32 unix_relative_path(MemArena *arena, String8 *path) {

String8 forward_slash = str8("./");
b32 insertion_result = str8_insert(arena, path, &forward_slash, 0);
if(!insertion_result) {
breakpoint();
}
return insertion_result;

}

internal String8 unix_file_load(MemArena *arena, String8 path)
{
    if (path.size <= 0) {
Log("Can't find the file we we're supposed to open. This is an issue because we we're supposed to break way earlier");
        return {};
    }

    String8 result   = {0};
    struct stat sbuf = {0};

    if (!path.data) {
        Log("Invalid path file");
        return str8_zero();
    }


const char *null_terminated_path = null_terminate(arena, &path);

    s32 file = open(null_terminated_path, O_RDONLY);
    if (file == -1) {
        Log("An error occurred. File was not loaded. error code: ", Str8Fmt , Str8Arg(path));
        return str8_zero();
    }

    if (fstat(file, &sbuf) == -1) {
        close(file);
        return str8_zero();
    }

    result      = PushString8(arena, (u64)sbuf.st_size);

    u64 file_size =  (u64)sbuf.st_size;
result.size = file_size;

u64 total_bytes_read = 0;

while(total_bytes_read < file_size) {

 s64 bytes = read(file, result.data + total_bytes_read, file_size - total_bytes_read);
if (bytes == -1) {
Log("Failed, reading file was interrupted");
return str8_zero();
}

if(bytes == 0) break;
total_bytes_read += (u64)bytes;
}

result.size = total_bytes_read;

    close(file);
    return result;
}

internal String8 unix_file_write(String8 data, String8 path)
{
    String8 result = {0};
    s32 file       = open((const char *)path.data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1) {
        return (String8){0};
    }

    u64 written = 0;
    while (written < data.size) {
        s64 err = write(file, data.data + written, data.size - written);
        if (err == -1) {
            close(file);
            return str8_zero();
        }
        written += err;
    }

    close(file);
    result = data;
    return result;
}

internal void unix_log_with_args(const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    vprintf(format, arguments);
	fflush(stdout);
    va_end(arguments);
}

internal void unix_os_exit() { _exit(0); }

// TODO: rewrite this
internal String8List *unix_open_dir(MemArena *arena, String8 *path)
{
    String8List *files_list = PushStructZero(arena, String8List);
    files_list->first       = &nil_str8_node;
    files_list->last        = &nil_str8_node;
	
    cstring cpath = null_terminate(arena, path);
    DIR *dir     = opendir(cpath);
    if (!dir) {
        Log("opendir(%s) failed\n", cpath);
        return files_list;
    }
	
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) { // TODO: should symbolic files be handles aswell?
            continue;
        }

        String8 name           = str8(entry->d_name);
        String8Node *file_node = PushStruct(arena, String8Node);
        *file_node             = ToString8Node(str8_clone(arena, &name));
        str8_list_push_node(files_list, file_node);
    }

FOR_STR8_NODE(files_list, current) {
Log("Logging files names");
LogStr8(current->value);
}
	
    closedir(dir);
    return files_list;
}