#ifndef BASE_ARENA_WIN32_H
#define BASE_ARENA_WIN32_H

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

#endif /* BASE_ARENA_WIN32 */
