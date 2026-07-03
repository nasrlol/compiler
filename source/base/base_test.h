#ifndef BASE_ERROR_H
#define BASE_ERROR_H

#define _stringify(x) #x
#define _str(x) _stringify(x)

// inserts a breakpoint inf the given value isn't correct
                                                          #define Test(expr, msg) \
 do \
 { \
 if(!(expr)) \
 { \
 _write("[ASSERT] " __FILE__ ":" _str(__LINE__) ":"); \
 _write(__func__); \
 _write(": "); \
 _write((msg)); \
 _write("\n"); \
 breakpoint; \
 } \
 } while(0)


 internal void
 test(b32 expression, String8 message)
 {

}

// exits if the given expresssions is false
internal void
assert(b32 expression)
_write("[FAILED] " __FILE__ ":" _str(__LINE__) ":");
_exit(1);
}

//- keeping these because the current project im working on uses them but they can be removed the future and just use the regular function names instead
#define Assert(expr) assert(expr)
#define Test(expr, msg) test(expr, msg)


#endif /* BASE_ERROR_H */
