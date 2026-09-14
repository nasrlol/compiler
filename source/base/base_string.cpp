internal b32
is_nil_str8(String8 *a)
{
    return (a == &nil_str8);
}

internal b32
is_nil_str8_node(String8Node *a)
{
    return (a == &nil_str8_node);
}

internal b32
is_nil_str16(String16 *a)
{
    return (a == &nil_str16);
}

internal b32
is_nil_str16_node(String16Node *a)
{
    return (a == &nil_str16_node);
}

internal b32
is_nil_str32(String32 *a)
{
    return (a == &nil_str32);
}

internal b32
is_nil_str32_node(String32Node *a)
{
    return (a == &nil_str32_node);
}

internal void
str8_trim_left(String8 *buffer)
{
    u64 index = 0;
    while(cstring_is_whitespace(buffer->data[index])) {
        ++index;
    }
    
    str8_chop_left(*buffer, index);
}

internal void
str16_trim_left(String16 *buffer)
{
    u64 index = 0;
    while(cstring_is_whitespace(buffer->data[index])) {
        ++index;
    }
    
    str16_chop_left(*buffer, index);
}

internal void
str32_trim_left(String32 *buffer)
{
    u64 index = 0;
    
    while(cstring_is_whitespace(buffer->data[index])) {
        ++index;
    }
    
    str32_chop_left(*buffer, index);
}

internal void
str8_trim_right(String8 *buffer)
{
    u64 index = buffer->size;
    while(cstring_is_whitespace(buffer->data[index])) {
        --index;
    }
    
    str8_chop_right(*buffer, index);
}

// the first the element is the buffer you are searching in and the second buffer is the element you are matching for.
// it's important not to mess this up because it messes with the memcpy size
internal b32
str8_any_match(String8 a, String8 b)
{
    b32 result = (memcmp(a.data, b.data, b.size) == 0);
    return result;
}

internal b32
str8_is_match(String8 a, String8 b)
{
    if(a.size != b.size) {
        return 0;
    }
    return (b32)(memcmp(a.data, b.data, a.size) == 0);
}


internal u64
str8_find(String8 *buffer, u64 start_pos, String8 element)
{
    b32 result = false;
    for(u64 index = start_pos; index < buffer->size - element.size; ++index) {
        String8 view = str8_chop_left(*buffer, index);
        result = str8_any_match(view, element);
        if(result) {
            return index;
        }
    }
    
    return 0;
}


internal b32
str8_is_digit(String8 a, u64 start_pos)
{
    for(u64 index = start_pos; index < a.size; ++index) {
        if(!cstring_is_digit(a.data[index])) return false;
    }
    
    return true;
}

internal void
str8_copy(String8 *dest, String8 *src)
{
    MemCpy(dest->data, src->data, src->size);
    dest->size = src->size;
}


internal String8
str8_clone(MemArena *arena, String8 *src)
{
    String8 clone = PushString8(arena, src->size);
    str8_copy(&clone, src);
    return clone;
}

// you cant just do this you have to reallocat that thing no?
internal b32
str8_append_char(String8 *dest, u8 src)
{
    if(is_nil_str8(dest) || src == 0) return false;
    dest->data[dest->size] = src;
    dest->size += 1;
    return true;
}

// left here 7/9 refactoring the names and parameters
internal b32
str8_append(String8 *dest, String8 src)
{
    if( is_nil_str8(dest) || is_nil_str8(&src)) return false;
    MemCpy(dest->data + dest->size, src.data, src.size);
    dest->size += src.size;
    return true;
}

internal String8
str8_zero()
{
    return String8{0};
}

internal void
 str8_chop_left_ptr(String8 *buffer, u64 count)
{
    if(buffer->size < count) return;
    
    for(u64 idx = 0; idx < count; ++idx) {
        buffer->data++;
    }
}

internal void
str8_chop_right_ptr(String8 *buffer, u64 count)
{
    if(buffer->size < count) return;
    buffer->size -= count;
}


internal String8
str8_chop_left(String8 buffer, u64 count)
{
    if(buffer.size < count) {
        return buffer;
    }
    
    for(u64 idx = 0; idx < count; ++idx) {
        buffer.data++;
    }
    return buffer;
}

internal String8
str8_chop_right(String8 buffer, u64 count)
{
    if(buffer.size < count) {
        return buffer;
    }
    
    buffer.size -= count;
    return buffer;
}

internal void
str8_list_push_node(String8List *list, String8Node *node)
{
    if(list->first == NULL) {
        list->first = node;
        list->last  = node;
    } else {
        list->last->next = node;
        list->last        = node;
    }
    list->count += 1;
}

internal String8
str8_create_substr(String8 *buffer, u64 current_idx, u64 start_idx, u64 end_idx)
{
    return str8_chop_left(str8_chop_right(*buffer, end_idx), (start_idx - current_idx));
}

internal String16
str16_create_substr(String16 *buffer, u64 current_idx, u64 start_idx, u64 end_idx)
{
    return str16_chop_left(str16_chop_right(*buffer, end_idx), (start_idx - current_idx));
}

internal String16
str16_chop_left(String16 buffer, u64 count)
{
    if(buffer.size < count) {
        return buffer;
    }
    
    for(u64 idx = 0; idx < count; ++idx) {
        buffer.data++;
    }
    return buffer;
}

internal String32
str32_chop_left(String32 buffer, u64 count)
{
    if(buffer.size < count) {
        return buffer;
    }
    
    for(u64 idx = 0; idx < count; ++idx) {
        buffer.data++;
    }
    return buffer;
}


internal String16
str16_chop_right(String16 buffer, u64 count)
{
    if(buffer.size < count) {
        return buffer;
    }
    
    buffer.size -= count;
    return buffer;
}

internal String32
str32_chop_right(String32 buffer, u64 count)
{
    if(buffer.size < count) {
        return buffer;
    }
    
    buffer.size -= count;
    return buffer;
}

internal String16
str16_create_substr(String16 *buffer, u64 start_idx, u64 end_idx)
{
    return str16_chop_left(str16_chop_right(*buffer, end_idx), (start_idx));
    // current idx should signify the calculation from the beginning of the string buffer
    // until it's end
    // but we know its beginnging because that's 0
    // so basically the start idx is already the diff from the currentidx to the start_idx
}

internal String32
str32_create_substr(String32 *buffer, u64 start_idx, u64 end_idx)
{
    return str32_chop_left(str32_chop_right(*buffer, end_idx), (start_idx ));
}

internal b32
str8_is_empty(String8 *buffer)
{
    if(cstring8_length(buffer->data) == 0 || buffer->size == 0) return true; // the values can't be lower than 0
    return false;
}

//- cstring implementations

internal b32
cstring_is_digit(u8 point)
{
    return (point >= '0' && point <= '9');
}

internal b32
cstring_is_alpha(u8 point)
{
    return ((point >= 'a' && point <= 'z') || (point >= 'A' && point <= 'Z') || (point == '_'));
}

internal b32
cstring_is_alpha_num(u8 point)
{
    return (cstring_is_alpha(point) || cstring_is_digit(point));
}

internal b32
cstring_is_nl(u8 point)
{
    return (point == '\n' || point == '\r');
}

internal b32
cstring_is_whitespace(u8 point)
{
    return (point == ' ' || point == '\t');
}

internal b32
cstring_is_slash(u8 point)
{
    return (point == '/' || point == '\\');
}

internal b32
cstring_is_newline(u8 point)
{
    return (point == '\n' || point == '\r');
}

internal b32
cstring_is_match(u8 *a, u8 *b)
{
    u64 as = cstring8_length(a);
    u64 bs = cstring8_length(b);
    
    if(as != bs) {
        return false;
    }
    
    for(u64 idx = 0; idx < as; ++idx) {
        if(a[idx] != b[idx]) {
            return false;
        }
    }
    return true;
}

internal u64
cstring8_length(u8 *c)
{
    u8 *p = c;
    if(!c) return 0;
    for(; *c != 0; ++c);
    return (u64)(c - p);
}

internal u64
cstring16_length(u16 *c)
{
    u16  *p = c;
    
    if(!c) return 0;
    for(; *c != 0; ++c);
    return (u64)(p - c);
    
}

internal u64
cstring32_length(u32 *c)
{
    
    u32  *p = c;
    
    if(!c) return 0;
    for(; *c != 0; ++c);
    return (u64)(p - c);
    
}


internal const char *
null_terminate(MemArena *arena, String8 *c)
{
    ScratchArena scratch = scratch_start(arena);
    char *cstring = (char *)arena_alloc(scratch.arena, c->size + 1, 0);
    MemCpy(cstring, c->data, c->size);
    cstring[c->size] = '\0';
    scratch_end(&scratch);
    
    return (const char *)cstring;
}


internal String8
remove_null_terminator(MemArena *arena, String8 c)
{
    if ((c.size > 0) && (c.data[c.size - 1] == '\0')) {
        String8 chopped = {c.data, c.size - 1};
        return chopped;
    }
    
    
    return c;
}

// returns the index of where to find it. if not found returns 0.
internal u64
cstring8_find(u8 *buffer, u8 *element)
{
    u8* c = buffer;
    while(c != 0) {
        if(!cstring_is_match(buffer, element)) {
            return c - buffer;
        } else {
            ++c;
        }
    }
    
    return 0;
}

// returns the index of where to find it. if not found returns 0.
internal u64
cstring8_find_new_line(u8 *buffer)
{
    u8* c = buffer;
    while(*c != 0) {
        if(cstring_is_nl(*c)) {
            return c - buffer;
        } else {
            ++c;
        }
    }
    
    return 0;
}



// returns the index of where to find it. if not found returns 0.
internal u64
cstring8_find_needle(u8 *buffer, u8 needle)
{
    
    u8* c = buffer;
    while(*c != 0) {
        if(*c == needle) {
            return c - buffer;
        } else {
            ++c;
        }
    }
    
    return 0;
    
}

