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
    while(cstring_is_whitespace(buffer->data[index]))
    {
        ++index;
    }

    str8_chop_left(*buffer, index);
    }

    internal void
    str8_trim_right(String8 *buffer)
    {
    u64 index = buffer->size;
    while(cstring_is_whitespace(buffer->data[index]))
    {
    --index;
    }

    str8_chop_right(*buffer, index);
    }

    internal b32
    str8_any_match(String8 a, String8 b)
    {
    b32 result = (memcmp(a.data, b.data, b.size) == 0);
    return false;
    }

    internal b32
    str8_is_match(String8 a, String8 b)
    {
    if(a.size != b.size)
    {
    return 0;
    }
    return (b32)(memcmp(a.data, b.data, a.size) == 0);
    }

    internal u64
    str8_find(String8 *buffer, u64 start_pos, String8 element)
    {

    b32 result = false;
    for(u64 index = start_pos; index < buffer->size - element.size; ++index)
    {
    String8 view = str8_chop_left(*buffer, index);
    result = str8_is_match(element, view);
    if(result)
    {
    return index;
    }
    }

    return 0;
    }


    internal b32
    str8_is_digit(String8 a, u64 start_pos)
    {
    for(u64 index = start_pos; index < a.size; ++index)
    {
    if(!cstring_is_digit(a.data[index])) return false;
    }

    return true;
    }

    internal String8
    str8_append_u8_element(String8 buffer, u8 c)
    {
    buffer.data[buffer.size] = c;
    buffer.size += 1;
    return buffer;
    }

    internal b32
    str8_append(String8 *a, String8 *b)
    {
    if(is_nil_str8(a) || is_nil_str8(b) || a == NULL || b == NULL)
    {
    breakpoint();
    return false;
    }

    MemCpy(a->data + a->size, b->data, b->size);
    a->size += b->size;

    return true;

    }


    internal String8
    str8_zero()
    {
    return String8{0};
    }

    internal String8
    str8_chop_left(String8 buffer, u64 count)
    {
    if(buffer.size < count)
    {
    return buffer;
    }

    for(u64 idx = 0; idx < count; ++idx)
    {
    buffer.data++;
    }
    return buffer;
    }

    internal String8
    str8_chop_right(String8 buffer, u64 count)
    {
    if(buffer.size < count)
    {
    return buffer;
    }

    buffer.size -= count;
    return buffer;
    }

    internal void
    str8_list_push_node(String8List *list, String8Node *node)
    {
    if(list->first == NULL)
    {
    list->first = node;
    list->last  = node;
    }
    else
    {
    list->last->next = node;
    list->last        = node;
    }
    list->count += 1;
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
    cstring_is_alnum(u8 point)
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
    return (point == '\n');
    }


    internal b32
    cstring_is_match(u8 *a, u8 *b)
    {
    u64 as = cstring8_length(a);
    u64 bs = cstring8_length(b);

    if(as != bs)
    {
    return false;
    }

    for(u64 idx = 0; idx < as; ++idx)
    {
    if(a[idx] != b[idx])
    {
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
    if ((c.size > 0) && (c.data[c.size - 1] == '\0'))
    {
    String8 chopped = {c.data, c.size - 1};
    return chopped;
    }


    return c;
    }

    internal b32
    str8_first_character_is_match(String8 buffer, u8 character)
    {
    buffer.size = 0;

    return str8_is_match(buffer, str8(character));
}