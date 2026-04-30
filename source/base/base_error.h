/* base library internal error checking system */
#ifndef BASE_ERROR_H
#define BASE_ERROR_H

internal inline void
write_int(s32 num)
{
    if (num < 0)
    {
        write(STDERR_FILENO, "-", 1);
        num = -num;
    }
    if (num >= 10) write_int(num / 10);

    char digit = '0' + (num % 10);
    write(STDERR_FILENO, &digit, 1);
}



#define error_at(msg)                                                   \
    do {                                                                \
        os_write(2, Red "[ERROR] " __FILE__ ":");                       \
        write_int(2, __LINE__);                                         \
        os_write(2, ":" __func__ ": " Reset);                           \
        os_write(2, (msg));                                             \
        os_write(2, "\n");                                              \
    } while (0)

#define warn(msg)                                                       \
    do {                                                                \
        os_write(2, Yellow "[WARN] " __FILE__ ":");                     \
        write_int(2, __LINE__);                                         \
        os_write(2, ":" __func__ ": " Reset);                           \
        os_write(2, (msg));                                             \
        os_write(2, "\n");                                              \
    } while (0)

#define assert_msg(expr, msg)                                           \
    do {                                                                \
        if (!(expr)) {                                                  \
            write_int(2, __LINE__);                                     \
            os_write(2, ":" __func__ ": " Reset);                       \
            os_write(2, (msg));                                         \
            os_write(2, "\n");                                          \
            _exit(1);                                                   \
        }                                                               \
    } while (0)


#define test(expr)                                                      \
    do {                                                                \
        if ((expr) != 0) {                                              \
            os_write(2, "[FAILED] " __FILE__ ":");                      \
            write_int(2, __LINE__);                                     \
            os_write(2, ":" __func__ "\n");                             \
            _exit(1);                                                   \
        }                                                               \
    } while (0)


#endif /* BASE_ERROR_H */
