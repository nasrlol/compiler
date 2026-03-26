internal mem_stack *
stack_create(u64 capacity)
{
    mem_stack *stack = (mem_stack *)mmap(
    0,
    capacity + sizeof(mem_stack),
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANONYMOUS,
    -1,
    0);

    if (stack == MAP_FAILED)
    {
        return NULL;
    }

    stack->capacity       = capacity;
    stack->base_position  = (u8 *)stack + sizeof(mem_stack);
    stack->current_offset = 0;

    return stack;
}

internal u8
calculate_padding(u64 pointer, u8 alignment, u64 header_size)
{
    u8 modulo, padding;

    if (!is_pow(alignment))
    {
        return 0;
    }

    modulo = pointer & (u8)(alignment - 1);

    padding = 0;

    if (0 == modulo)
    {
        padding = alignment - modulo;
    }

    if (padding < header_size)
    {
        header_size -= padding;

        if ((header_size & (alignment - 1)) != 0)
        {
            padding += alignment * (1 + (header_size / alignment));
        }
        else
        {
            padding += alignment * (header_size / alignment);
        }
    }

    return padding;
}

internal mem_stack *
stack_push_align(mem_stack *stack, u64 size, u8 alignment)
{
    u8 padding = 0;

    if (!is_pow(alignment))
    {
        return (0);
    }

    if (alignment > 128)
    {
        alignment = 128;
    }

    u64 current_address = (u64)stack->base_position + stack->current_offset;
    padding             = calculate_padding(current_address, alignment, sizeof(mem_stack_header));

    if (stack->current_offset + padding + size > stack->capacity)
    {
        return 0;
    }

    stack->current_offset += padding;

    u64               next_address = current_address + (u64)padding;
    mem_stack_header *header       = (mem_stack_header *)(next_address - sizeof(mem_stack_header));
    header->padding                = padding;

    stack->current_offset += size;

    return MemSet((void *)next_address, size);
}
internal void *
stack_push(mem_stack *stack, u64 size)
{
    return stack_push_align(stack, size, ARENA_ALIGN);
}

internal void
stack_pop(mem_stack *stack, void *pointer)
{
    if (pointer != NULL)
    {
        u64               start, end, current_address;
        mem_stack_header *header;
        u64               prev_offset;

        start           = (u64)stack->base_position;
        end             = start + (u64)stack->capacity;
        current_address = (u64)pointer;

        if (!(start <= current_address && current_address < end))
        {
            if (0 && "Out of bounds memory address passed to stack allocator (free)")
            {
                return;
            }
            return;
        }

        if (current_address >= start + (u64)stack->base_position)
        {
            return;
        }

        header                = (mem_stack_header *)(current_address - sizeof(mem_stack_header));
        prev_offset           = (size_t)(current_address - (u64)header->padding - start);
        stack->current_offset = prev_offset;
    }
}

internal mem_stack *
stack_resize_align(mem_stack *stack, void *pointer, u64 old_size, u64 new_size, u8 alignment)
{
    if (pointer == NULL)
    {
        return stack_push_align(stack, new_size, alignment);
    }
    else if (new_size == 0)
    {
        stack_pop(stack, pointer);
        return NULL;
    }

    u64   start, end, current_address;
    u64   min_size = old_size < new_size ? old_size : new_size;
    void *new_pointer;

    start           = (u64)stack->base_position;
    end             = start + (u64)stack->capacity;
    current_address = (u64)pointer;
    if (!(start <= current_address && current_address < end))
    {
        return NULL;
    }

    if (current_address >= start + (u64)stack->current_offset)
    {
        return NULL;
    }

    if (old_size == new_size)
    {
        return pointer;
    }

    new_pointer = stack_push_align(stack, new_size, alignment);
    memmove(new_pointer, pointer, min_size);
    return new_pointer;
}

internal void
stack_pop_all(mem_stack *stack)
{
    stack->current_offset = 0;
}

internal void
stack_destroy(mem_stack *stack)
{
    if (!stack)
    {
        return;
    }

    munmap(stack, stack->capacity + sizeof(mem_stack));
}
