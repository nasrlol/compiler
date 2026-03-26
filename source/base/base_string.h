#ifndef BASE_STRING_H
#define BASE_STRING_H

#include <string.h>

#define StringLit(string) \
    (string8){ .data = (u8 *)(string), .size = (sizeof(string) - 1) }

 #define PushString(arena, size) \
    (string8){ (u8 *)PushArray((arena), u8, (size)), (u64)(size) }

#define StringFmt "%.*s"
#define ULongFmt  "%lu"
#define ULLongFmt "%llu"

typedef struct string8 string8;
struct string8
{
    u8  *data;
    u64  size;
};

internal b8
string8_cmp(string8 a, string8 b)
{
    if (a.size != b.size) return 0;
    return (b8)(memcmp(a.data, b.data, a.size) == 0);
}

internal u64
string8_to_u64(u8 *buf, umm len)
{
    u64 value = 0;
    for (umm i = 0; i < len; ++i)
    {
        u8 c = buf[i];
        if (c < '0' || c > '9') break;
        value = value * 10 + (c - '0');
    }
    return value;
}

internal void
string8_append_char(string8 *buf, u8 c)
{
    buf->data[buf->size] = c;
    buf->size += 1;
}

read_only global_variable
string8 nil_string =
{

    .data = NULL,
    .size = 0,

};

#endif /* BASE_STRING_H */
