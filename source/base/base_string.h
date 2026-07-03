#ifndef BASE_STRING_H
#define BASE_STRING_H

// - str8
typedef struct String8 String8;
typedef struct String16 String16;
typedef struct String32 String32;

typedef struct String8Node String8Node;
typedef struct String16Node String16Node;
typedef struct String32Node String32Node;

typedef struct String8List String8List;
typedef struct String16List String16List;
typedef struct String32List String32List;

//- strings

struct String8 {
	u8* data;
	u64  size;
};

struct String16 {
	u16* data;
	u64  size;
};

struct String32 {
	u32* data;
	u64  size;
};

//- nodes
struct String8Node {
	String8Node* next;
	String8 value;

};

struct String16Node {
	String16Node* next;
	String8 string;

};

struct String32Node {
	String32Node* next;
	String32     value;
};


//- lists
struct String8List {
	String8Node* first;
	String8Node* last;
	u64 count;
};

struct String16List {
	String16Node* next;
	String16Node string;
};

struct string32_list {
	String32Node* first;
	String32Node* last;
	u64 count;
};

//- string tools

internal inline b8
str8_match(String8 a, String8 b) {
	if (a.size != b.size) return 0;
	return (b8)(memcmp(a.data, b.data, a.size) == 0);
}

internal inline void
str8_append_u8_element(String8* buf, u8 c) {
	buf->data[buf->size] = c;
	buf->size += 1;
}


internal inline String8
str8_zero() {
	return String8{ 0 };
}

//- cstring implementations

internal inline  b32
cstring_is_digit(u8 point) {
	return (point >= '0' && point <= '9');
}

internal inline b32
cstring_is_alpha(u8 point) {
	return ((point >= 'a' && point <= 'z') || (point >= 'A' && point <= 'Z') || (point == '_'));
}

internal inline b32
cstring_is_alnum(u8 point) {
	return (cstring_is_alpha(point) || cstring_is_digit(point));
}

internal inline b32
cstring_is_nl(u8 point) {
	return (point == '\n' || point == '\r');

}

internal inline b32
cstring_is_whitespace(u8 point) {
	return (point == ' ' || point == '\t');
}

internal inline b32
cstring_is_slash(u8 point) {
	return (point == '/' || point == '\\');
}

internal inline b32
cstring_is_newline(u8 point) {
	return (point == '\n');
}

//-

internal b32
cstring_is_match(u8* a, u8* b) {

	u64 a_size = sizeof(a);
	u64 b_size = sizeof(b);

	if (a_size != b_size) return false;

	for (s32 idx = 0; idx < a_size; ++idx) {
		if (a[idx] != b[idx]) return false;
	}

	return true;
}

internal u64
cstring_find_u8_character(u8* buffer, u64 start_pos, u8 element) {
	u8* data = buffer;
	u64 offset;
	while (!cstring_is_match(data, &element)) {
		++offset;
		++data;
	}

	return offset + start_pos;
}

internal inline u64
cstring_find_element(u8* buffer, u64 start_pos, u8 element) {
	for (u64 idx = start_pos; buffer[idx]; ++idx) {
		if (buffer[idx] == element) {
			return true;
		}
	}
	return false;
}

//- cstring implementations

internal inline b32 cstring_is_alnum(u8 point);
internal inline b32 cstring_is_nil(u8 point);
internal inline b32 cstring_is_whitespace(u8 point);
internal inline b32 cstring_is_slash(u8 point);
internal inline b32 cstring_is_alpha(u8 point);
internal inline b32 cstring_is_digit(u8 point);
internal inline b32 cstring_is_newline(u8 point);
internal        b32 cstring_is_match(u8* a, u8* b);
internal inline u64 cstring_find_element(u8* buffer, u64 start_pos, u8 element);


//- string8 implementations

internal inline String8 str8_set_zero();
internal inline b8      str8_is_match(String8 a, String8 b);

internal inline void    str8_append_u8_element(String8* buf, u8* c);
internal inline void    str8_append_str8_element(String8* a, String8* b);

internal inline u64     str8_skip_element(String8* buffer, u64 start_pos, String8* element);
internal inline u64     str8_find_element(String8* buffer, u64 start_pos, String8* element);

//- macros

#define PushString8(arena, count) (String8){ .data = (PushArrayZero(arena, u8, (count))), .size = (count) }
#define PushString16(arena, count) (String16){ .data = (PushArrayZero(arena, u16, (count))), .size = (count) }
#define PushString32(arena, count) (String32){ .data = (PushArrayZero(arena, u32, (count))), .size = (count) }

#define ToString8(data, size)  String8{(u8  *)(data),  (u64)(size) }
#define ToString16(data, size) String16{(u16 *)(data), (u64)(size) }
#define ToString32(data, size) String32{(u32 *)(data), (u64)(size) }

#define StringFmt "%.*s"
#define ULongFmt  "%lu"
#define ULLongFmt "%llu"

#define CStringLen(s) (sizeof(s) - 1) // minus null terminator
#define String8Len(s) (s.size)

//-

ReadOnly global_variable String8  str8_nil = { NULL, 0 };
ReadOnly global_variable String16 str16_nil = { NULL, 0 };
ReadOnly global_variable String32 str32_nil = { NULL, 0 };

ReadOnly global_variable String8Node str8_nil_node = { NULL, NULL };
ReadOnly global_variable String16Node str16_nil_node = { NULL, NULL };
ReadOnly global_variable String32Node str32_nil_node = { NULL, NULL };


#endif /* BASE_STRING_H */