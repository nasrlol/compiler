#ifndef BASE_STRING_H
#define BASE_STRING_H

#define PushString8(arena, count) (String8){ .data = (PushArrayZero(arena, u8, (count))), .size = (count) }
#define PushString16(arena, count) (String16){ .data = (PushArrayZero(arena, u16, (count))), .size = (count) }
#define PushString32(arena, count) (String32){ .data = (PushArrayZero(arena, u32, (count))), .size = (count) }

#define ToString8(data, size)  String8{(u8  *)(data), (u64)(size) }
#define ToString16(data, size) String16{(u16 *)(data), (u64)(size) }
#define ToString32(data, size) String32{(u32 *)(data), (u64)(size) }

#define StringFmt "%.*s"
#define ULongFmt  "%lu"
#define ULLongFmt "%llu"

typedef struct String8 String8;
struct String8 {
 u8  *data;
 u64  size;
};

typedef struct String16 String16;
struct String16 {
 u16  *data;
 u64  size;
};

typedef struct String32 String32;
struct String32 {
 u32  *data;
 u64  size;
};

//- string linked list
typedef struct String8Node String8Node;
struct String8Node {
 String8 *next;
 String8 string;
};

typedef struct String8List String8List;
struct String8List {
 String8 *first;
 String8 *last;
 u64 count;
};

typedef struct String16List String16List;
struct String16List {
 String16 *next;
 String16 string;
};

typedef struct string32_list string32_list;
struct string32_list {
 String32 *first;
 String32 *last;
 u64 count;
};

//- string tools
internal b8
string8_cmp(String8 a, String8 b) {
 if (a.size != b.size) return 0;
 return (b8)(memcmp(a.data, b.data, a.size) == 0);
}

internal void
string8_appendc(String8 *buf, u8 c) {
 buf->data[buf->size] = c;
 buf->size += 1;
}

ReadOnly global_variable
String8 nil_string = { NULL, 0};

#endif /* BASE_STRING_H */

#ifdef BASE_IMPLEMENTATION

internal b32
is_alpha(u8 point) {
 return ((point >= 'a' && point <= 'z') || (point >= 'A' && point <= 'Z') || (point == '_'));
}

internal b32
is_digit(u8 point) {
 return (point >= '0' && point <= '9');
}

internal b32
is_alpha_num(u8 point) {
 return (is_alpha(point) || is_digit(point));
}

internal inline b32
is_newline(u8 point) {
 return (point == '\n' || point == '\r' );
}

internal inline b32
is_whitespace(u8 point) {
 return ( point == ' ' || point == '\t');
}

internal inline b32
is_slash(u8 point) {
 return (point == '/' || point == '\\');
}

#endif
