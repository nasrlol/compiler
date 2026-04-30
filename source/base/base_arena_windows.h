#ifndef BASE_ARENA_WINDOWS_H
#define BASE_ARENA_WINDOWS_H

internal mem_arena *
_arena_create(u64 capacity)
{
	mem_arena *arena = (mem_arena *)VirtualAlloc(
	NULL,
	capacity + sizeof(mem_arena),
	MEM_COMMIT | MEM_RESERVE,
	PAGE_READWRITE);
	if (!arena)
	{
		return NULL;
	}
	arena->capacity          = capacity;
	arena->base_position     = (u8 *)arena + sizeof(mem_arena);
	arena->current_position  = 0;
	arena->previous_position = 0;
	return arena;
}

internal mem_arena *
_arena_destroy(mem_arena *arena)
{
	if (!arena)
	{
		return NULL;
	}
	VirtualFree(arena, 0, MEM_RELEASE);
	return NULL;
}

#endif /* BASE_ARENA_WINDOWS_H */