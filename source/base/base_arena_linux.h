#ifndef BASE_ARENA_LINUX_H
#define BASE_ARENA_LINUX_H

internal MemArena *
_arena_create(u64 capacity)
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
  return NULL;

 arena->capacity          = capacity;
 arena->base_position     = (u8 *)arena + sizeof(MemArena);
 arena->current_position  = 0;
 arena->previous_position = 0;

 return arena;
}

internal void
_arena_destroy(MemArena *arena)
{
 if(!arena)
  return;
 munmap(arena, arena->capacity + sizeof(MemArena));
}

#endif /* BASE_ARENA_LINUX_H */