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
    str8_match(String8 a, String8 b)
    {
    if(a.size != b.size)
    {
    return 0;
    }
    return (b8)(memcmp(a.data, b.data, a.size) == 0);
    }

    internal String8
    str8_append_u8_element(String8 buffer, u8 c)
    {
    buffer.data[buffer.size] = c;
    buffer.size += 1;
    return buffer;
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
    cstring_is_match(void *a, void *b)
    {
    u64 a_size = CStringLen(a);
    u64 b_size = CStringLen(b);

    if(a_size != b_size)
    {
    return false;
    }

    for(s32 idx = 0; idx < a_size; ++idx)
    {
    if(a[idx] != b[idx])
    {
    return false;
    }
    }
    return true;
    }

    internal u64
    cstring_find_u8_character(u8 *buffer, u64 start_pos, u8 element)
    {
    u8 *data   = buffer;
    u64 offset = 0;
    while(!cstring_is_match(data, &element))
    {
    ++offset;
    ++data;
    }

    return offset + start_pos;
}

internal u64
cstring_find_element(u8 *buffer, u64 start_pos, u8 element)
{
    for(u64 idx = start_pos; buffer[idx]; ++idx)
    {
        if(buffer[idx] == element)
        {
            return idx;
        }
    }
    return 0;
}
