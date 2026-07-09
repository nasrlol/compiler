#ifndef BASE_STRING_H
#define BASE_STRING_H

// - str8
typedef struct String8  String8;
typedef struct String16 String16;
typedef struct String32 String32;

typedef struct String8Node  String8Node;
typedef struct String16Node String16Node;
typedef struct String32Node String32Node;

typedef struct String8List  String8List;
typedef struct String16List String16List;
typedef struct String32List String32List;

//- strings

struct String8
{
    u8 *data;
    u64 size;
};

struct String16
{
    u16 *data;
    u64  size;
};

struct String32
{
    u32 *data;
    u64  size;
};

//- nodes
struct String8Node
{
    String8Node *next;
    String8      value;
};

struct String16Node
{
    String16Node *next;
    String16      string;
};

struct String32Node
{
    String32Node *next;
    String32      value;
};

//- lists
struct String8List
{
    String8Node *first;
    String8Node *last;
    u64          count;
};

struct String16List
{
    String16Node *first;
    String16Node *last;
    u64           count;
};

struct String32List
{
    String32Node *first;
    String32Node *last;
    u64           count;
};

//- cstring implementations

internal b32 cstring_is_alnum(u8 point);
internal b32 cstring_is_nil(u8 point);
internal b32 cstring_is_whitespace(u8 point);
internal b32 cstring_is_slash(u8 point);
internal b32 cstring_is_alpha(u8 point);
internal b32 cstring_is_digit(u8 point);
internal b32 cstring_is_newline(u8 point);
internal b32 cstring_is_match(u8 *a, u8 *b);
internal u64 cstring_find_element(u8 *buffer, u64 start_pos, u8 element);

//- string8 implementations

internal b32     str8_match(String8 a, String8 b);
internal u64     str8_skip_element(String8 *buffer, u64 start_pos, String8 *element);
internal u64     str8_find_element(String8 *buffer, u64 start_pos, String8 *element);
internal void    str8_append_u8_element(String8 *buf, u8 c);
internal void    str8_append_str8_element(String8 *a, String8 *b);
internal String8 str8_chop_left(String8 buffer, u64 count);
internal String8 str8_chop_right(String8 buffer, u64 count);
internal String8 str8_zero();
internal void    str8_trim_right();

//- macros

#define PushString8(arena, count) \
    String8 \
    { \
        (u8 *)(PushArrayZero(arena, u8, (count))), (u64)(count) \
    }
#define PushString16(arena, count) \
    String16 \
    { \
        (u16 *)(PushArrayZero(arena, u16, (count))), (u64)(count) \
    }
#define PushString32(arena, count) \
    String32 \
    { \
        (u32 *)(PushArrayZero(arena, u32, (count))), (u64)(count) \
    }

#define ToString8(data, size) \
    String8 \
    { \
        (u8 *)(data), (u64)(size) \
    }
#define ToString16(data, size) \
    String16 \
    { \
        (u16 *)(data), (u64)(size) \
    }
#define ToString32(data, size) \
    String32 \
    { \
        (u32 *)(data), (u64)(size) \
    }

#define StringFmt "%.*s"
#define ULongFmt "%lu"
#define ULLongFmt "%llu"

#define CStringLen(s) (sizeof(s) - 1) // minus null terminator
#define String8Len(s) (s.size)

//-

ReadOnly global_variable String8  str8_nil  = {NULL, 0};
ReadOnly global_variable String16 str16_nil = {NULL, 0};
ReadOnly global_variable String32 str32_nil = {NULL, 0};

ReadOnly global_variable String8Node  str8_nil_node  = {NULL, NULL};
ReadOnly global_variable String16Node str16_nil_node = {NULL, NULL};
ReadOnly global_variable String32Node str32_nil_node = {NULL, NULL};

#endif /* BASE_STRING_H */
