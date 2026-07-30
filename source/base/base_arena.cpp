#if PLATFORM_UNIX

internal MemArena *
unix_arena_create(u64 capacity)
{
    MemArena *arena = (MemArena *)mmap(
	/* kernel decides where to throw the arena */
	NULL,
	capacity + sizeof(MemArena),
	PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_ANONYMOUS,
	-1,
	0);

    if(arena == MAP_FAILED)
    {
	return NULL;
    }

    arena->capacity          = capacity;
    arena->base_position     = (u8 *)arena + sizeof(MemArena);
    arena->current_position  = 0;
    arena->previous_position = 0;

    return arena;
}

internal void
unix_arena_destroy(MemArena *arena)
{
    if(!arena)
    {
	return;
    }

    munmap(arena, arena->capacity + sizeof(MemArena));
}

#elif PLATFORM_WINDOWS

internal MemArena *
win32_arena_create(u64 capacity)
{
    MemArena *arena = (MemArena *)VirtualAlloc(
	NULL,
	capacity + sizeof(MemArena),
	MEM_COMMIT | MEM_RESERVE,
	PAGE_READWRITE);
    if(!arena)
    {
	return NULL;
    }

    arena->capacity          = capacity;
    arena->base_position     = (u8 *)arena + sizeof(MemArena);
    arena->current_position  = 0;
    arena->previous_position = 0;
    return arena;
}

internal void
win32_arena_destroy(MemArena *arena)
{
    if(!arena)
    {
	return;
    }

    VirtualFree(arena, 0, MEM_RELEASE);
    return;
}

#endif

internal void *
arena_alloc(MemArena *arena, u64 size, b32 zero)
{
    if(!arena)
    {
	return NULL;
    }
    u64 aligned = Align(arena->current_position, arena_align);
    u64 new_pos = aligned + size;
    if(new_pos > arena->capacity)
    {
	return NULL;
    }

    void *out = arena->base_position + aligned;

    arena->previous_position = arena->current_position;
    arena->current_position  = aligned + size;

    if(zero)
    {
	MemSetZero(out, size);
    }

    return out;
}

internal void
arena_pop(MemArena *arena, u64 size)
{
    size = Min(size, arena->current_position);
    arena->current_position -= size;
}

internal void
arena_pop_to(MemArena *arena, u64 pos)
{
    u64 size = pos < arena->current_position ? arena->current_position - pos : 0;
    arena_pop(arena, size);
}

internal void
arena_clear(MemArena *arena)
{
    arena->current_position = 0;
}

internal MemArena *
arena_resize_align(MemArena *arena, void *old_memory, u64 new_size, u64 old_size, umm alignment)
{
    u8 *old_mem = (u8 *)old_memory;

    if(is_pow(alignment))
    {
	arena->current_position = Align(arena->current_position, alignment);
    }

    if(old_memory == NULL || old_size == 0)
    {
	return (MemArena *)arena_alloc(arena, new_size, 0);
    }
    else if((old_mem >= arena->base_position && old_mem < arena->base_position + arena->capacity))
    {
	if((arena->base_position + arena->previous_position) == old_memory)
	{
	    arena->current_position = arena->previous_position + new_size;
	    if(new_size > old_size)
	    {
		MemSet(&arena->current_position, new_size - old_size);
	    }

	    return (MemArena *)old_memory;
	}
	else
	{
	    void *new_memory = arena_alloc(arena, new_size, 0);
	    umm   copy_size  = old_size < new_size ? old_size : new_size;
	    memmove(new_memory, old_mem, copy_size);
	}
    }
    else
    {
	// Log("yeah something in the mem allocator failed i guess"); // TODO: find a way to properly include the logging
    }

    return NULL;
}

internal MemArena *
arena_resize(MemArena *arena, void *old_memory, u64 new_size, u64 old_size)
{
    return arena_resize_align(arena, old_memory, new_size, old_size, arena_align);
}

internal ScratchArena
scratch_start(MemArena *arena)
{
    ScratchArena sa;
    sa.arena          = arena;
    sa.start_position = arena->current_position;

    return sa;
}

internal void
scratch_end(ScratchArena *sa)
{
    sa->arena->current_position = sa->start_position;
}

//-

internal b32
is_pow(umm x)
{
    return (x & (x - 1)) == 0;
}

internal u64
align(u64 pointer, umm alignment)
{
    if((alignment & (alignment - 1)) == 0)
    {
	return pointer;
    }
    return (pointer + alignment - 1) & ~(alignment - 1);
}
