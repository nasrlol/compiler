#ifndef BASE_TEST_H
#define BASE_TEST_H

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"

#define LEN(s) (sizeof(s) - 1)

internal void
write_int(i32 num)
{

    if (num < 0)
    {
        write(STDERR_FILENO, "-", 1);
        num = -num;
    }
    if (num >= 10)
        write_int(num / 10);
    char digit = '0' + (num % 10);

    write(STDERR_FILENO, &digit, 1);
}

#define show \
    do \
    { \
        write(STDOUT_FILENO, __FILE__, sizeof(__FILE__) - 1); \
        write(STDOUT_FILENO, ":", 1); \
        write(STDOUT_FILENO, __func__, sizeof(__func__) - 1); \
        write(STDOUT_FILENO, ":", 1); \
        write_int(__LINE__); \
        write(STDOUT_FILENO, "\n", 1); \
    } while (0)

#define test(expr) \
    { \
        if ((expr) != 0) \
        { \
            write(STDERR_FILENO, "[FAILED] ", LEN("[FAILED] ")); \
            show; \
            _exit(1); \
        } \
    }

#define check(expr) \
    { \
        if ((expr) != 0) \
        { \
            write(STDERR_FILENO, RED "[ERROR] ", LEN(RED "[ERROR] ")); \
            show; \
            write(STDERR_FILENO, RESET, LEN(RESET)); \
            _exit(1); \
        } \
        else \
        { \
            write(STDERR_FILENO, GREEN "[SUCCESS] ", LEN(GREEN "[SUCCESS] ")); \
            show; \
            write(STDERR_FILENO, RESET, LEN(RESET)); \
        } \
    }

#define checkpoint_output "<<CHECKPOINT>>\n"
#define checkpoint_end_output "^^^^^^^^^^^^^^\n\n\n"
#define checkpoint \
    { \
        write(STDERR_FILENO, BLUE checkpoint_output, LEN(BLUE checkpoint_output)); \
        show; \
        write(STDERR_FILENO, BLUE checkpoint_end_output, LEN(BLUE checkpoint_end_output)); \
    }

#endif /* BASE_TEST_H */
