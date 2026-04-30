#ifndef BASE_STRING_H
#define BASE_STRING_H

#define PushString8(arena, count) (string8){ .data = (PushArrayZero(arena, u8, (count))), .size = (count) }
#define PushString16(arena, count) (string16){ .data = (PushArrayZero(arena, u16, (count))), .size = (count) }
#define PushString32(arena, count) (string32){ .data = (PushArrayZero(arena, u32, (count))), .size = (count) }

#define String8(data, size) (string8){(u8 *)(data), (u64)(size) }
#define String16(data, size) (string16){(u16 *)(data), (u64)(size) }
#define String32(data, size) (string32){(u32 *)(data), (u64)(size) }



#define StringFmt "%.*s"
#define ULongFmt  "%lu"
#define ULLongFmt "%llu"

typedef struct string8 string8;
struct string8
{
    u8  *data;
    u64  size;
};

typedef struct string16 string16;
struct string16
{
    u16  *data;
    u64  size;
};

typedef struct string32 string32;
struct string32
{
    u32  *data;
    u64  size;
};

//- string linked list implementation
typedef struct string8_node string8_node;
struct string8_node
{
    string8 *next;
    string8 string;
};

typedef struct string8_list string8_list;
struct string8_list
{
    string8 *first;
    string8 *last;
    u64 count;
};

typedef struct string16_list string16_list;
struct string16_list
{
    string16 *next;
    string16 string;
};

typedef struct string32_list string32_list;
struct string32_list
{
    string32 *first;
    string32 *last;
    u64 count;
};

internal b8
string8_cmp(string8 a, string8 b)
{
    if (a.size != b.size) return 0;
    return (b8)(memcmp(a.data, b.data, a.size) == 0);
}

internal void
string8_appendc(string8 *buf, u8 c)
{
    buf->data[buf->size] = c;
    buf->size += 1;
}

read_only global_variable
string8 nil_string = {

    .data = NULL,
    .size = 0,
};

#endif /* BASE_STRING_H */

#ifdef BASE_IMPLEMENTATION

internal b32
is_alpha(u8 point)
{
    return ((point >= 'a' && point <= 'z') || (point >= 'A' && point <= 'Z') || (point == '_'));
}

internal b32
is_digit(u8 point)
{
    return (point >= '0' && point <= '9');
}

internal b32
is_alpha_num(u8 point)
{
    return (is_alpha(point) || is_digit(point));
}

internal b32 is_whitespace(u8 point)
{
    return (point == '\n' || point == '\r' || point == ' ' || point == '\t');
}

internal b32
is_slash(u8 point)
{
  return (point == '/' || point == '\\');
}


#endif
