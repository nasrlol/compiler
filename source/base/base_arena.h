#ifndef BASE_ARENA_H
#define BASE_ARENA_H

 typedef struct MemArena MemArena;
    typedef struct ScratchArena ScratchArena;

    internal b32 is_pow(umm x);
    internal u64 align(u64 pointer, umm alignment);

    //-

    struct MemArena
    {
    u64 current_position;
    u64 previous_position;
    u64 capacity;
    u8 *base_position;
    };

    struct ScratchArena
    {
    MemArena *arena;
    u64       start_position;
    };


    #if defined(PLATFORM_WINDOWS)
    #include "base_arena_windows.h"
    #elif defined(PLATFORM_LINUX)
    #include "base_arena_unix.h"
    #endif

    #if defined(PLATFORM_LINUX)
    #define ArenaCreate(capacity) unix_arena_create(capacity)
    #define ArenaDestroy(arena)   unix_arena_destroy(arena)
    #elif defined(PLATFORM_WINDOWS)
    #define ArenaCreate(capacity) win32_arena_create(capacity)
    #define ArenaDestroy(arena)   win32_arena_destroy(arena)
    #endif

    internal void *arena_alloc(MemArena *arena, u64 size, b32 zero);
    internal void arena_pop(MemArena *arena, u64 size);
    internal void arena_pop_to(MemArena *arena, u64 pos);
    internal void arena_clear(MemArena *arena);
    internal void scratch_end(ScratchArena *temp);
    internal ScratchArena scratch_start(MemArena *arena);
    internal MemArena *arena_resize_align(MemArena *arena, void *old_memory, u64 new_size, u64 old_size, umm alignment);
    internal MemArena *arena_resize(MemArena *arena, void *old_memory, u64 new_size, u64 old_size);

    #define Align(pointer, alignment) align((u64)(pointer), (umm)(alignment))
    #define PushStruct(arena, type) (type *)arena_alloc((arena), sizeof(type), 0)
    #define PushStructZero(arena, type) (type *)arena_alloc((arena), sizeof(type), 1)
    #define PushArray(arena, type, len) (type *)arena_alloc((arena), sizeof(type) * (len), 0)
    #define PushArrayZero(arena, type, len) (type *)arena_alloc((arena), sizeof(type) * (len), 1)

    #define KiB(n) (((u64)(n)) << 10)
    #define MiB(n) (((u64)(n)) << 20)
    #define GiB(n) (((u64)(n)) << 30)


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
    MemSet(out, size);
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

    if(!is_pow(alignment))
    {
    Align(arena->current_position, alignment);
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
    Log("yeah something in the mem allocater failed i guess");
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
    sa.arena->current_position = sa->start_position;
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








#endif /* BASE_ARENA_H */
