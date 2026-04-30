/* base library internal error checking system */
#ifndef BASE_ERROR_H
#define BASE_ERROR_H

#define _stringify(x) #x
#define _str(x) _stringify(x)

#define _write(buffer) _os_write(STDERR_FD, (buffer), Len(buffer))

#define error_at(msg)                                                          \
  do {                                                                         \
    _write("[ERROR] " __FILE__ ":" _str(__LINE__) ":");                        \
    _write(__func__);                                                          \
    _write(": ");                                                              \
    _write((msg));                                                             \
    _write("\n");                                                              \
  } while (0)

#define warn(msg)                                                              \
  do {                                                                         \
    _write("[WARN] " __FILE__ ":" _str(__LINE__) ":");                         \
    _write(__func__);                                                          \
    _write(": ");                                                              \
    _write((msg));                                                             \
    _write("\n");                                                              \
  } while (0)

#define assert_msg(expr, msg)                                                  \
  do {                                                                         \
    if (!(expr)) {                                                             \
      _write("[ASSERT] " __FILE__ ":" _str(__LINE__) ":");                     \
      _write(__func__);                                                        \
      _write(": ");                                                            \
      _write((msg));                                                           \
      _write("\n");                                                            \
      breakpoint;                                                              \
    }                                                                          \
  } while (0)

#define test(expr)                                                             \
  do {                                                                         \
    if ((expr) != 0) {                                                         \
      _write("[FAILED] " __FILE__ ":" _str(__LINE__) ":");                     \
      _write(__func__);                                                        \
      _write(": ");                                                            \
      _exit(1);                                                                \
    }                                                                          \
  } while (0)

#endif /* BASE_ERROR_H */
