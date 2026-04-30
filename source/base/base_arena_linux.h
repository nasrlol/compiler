#ifndef BASE_ARENA_LINUX_H
#define BASE_ARENA_LINUX_H

internal mem_arena *
_arena_create(u64 capacity)
{
    mem_arena *arena = (mem_arena *)mmap(
    /* kernel decides where to throw the arena */
    NULL,
    capacity + sizeof(mem_arena),
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANONYMOUS,
    -1,
    0);

    if (arena == MAP_FAILED)
    {
        return NULL;
    }

    arena->capacity          = capacity;
    arena->base_position     = (u8 *)arena + sizeof(mem_arena);
    arena->current_position  = 0;
    arena->previous_position = 0;

    return arena;
}

internal void
_arena_destroy(mem_arena *arena)
{
    if (!arena)
    {
        return;
    }
    munmap(arena, arena->capacity + sizeof(mem_arena));
}

#endif /* BASE_ARENA_LINUX_H */