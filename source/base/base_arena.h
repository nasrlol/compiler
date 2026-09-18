#ifndef BASE_ARENA_H
#define BASE_ARENA_H

//- arena
typedef struct MemArena MemArena;
typedef struct ScratchArena ScratchArena;

struct MemArena {
    u64 current_position;
    u64 previous_position;
    u64 capacity;
    u8 *base_position;
};


struct ScratchArena {
    MemArena *arena;
    u64       start_position;
};

//-



#if defined(PLATFORM_UNIX)

internal MemArena *unix_arena_create(u64 capacity) ;
internal void unix_arena_destroy(MemArena *arena);

#define ArenaCreate(capacity) unix_arena_create(capacity)
#define ArenaDestroy(arena)   unix_arena_destroy(arena)
#elif defined(PLATFORM_WINDOWS)

internal void win32_arena_destroy(MemArena *arena);
internal MemArena *win32_arena_create(u64 capacity);

#define ArenaCreate(capacity) win32_arena_create(capacity)
#define ArenaDestroy(arena)   win32_arena_destroy(arena)
#endif

internal void* arena_alloc(MemArena *arena, u64 size, b32 zero);
internal void arena_pop(MemArena *arena, u64 size);
internal void arena_pop_to(MemArena *arena, u64 pos);
internal void arena_clear(MemArena *arena);
internal ScratchArena scratch_start(MemArena *arena);
internal void scratch_end(ScratchArena *scratch);
internal MemArena *arena_resize_align(MemArena *arena, void *old_memory, u64 new_size, u64 old_size, umm alignment);
internal MemArena *arena_resize(MemArena *arena, void *old_memory, u64 new_size, u64 old_size);
internal u64 arena_calculate_used_capacity(MemArena *arena);

#define Align(pointer, alignment) align((u64)(pointer), (umm)(alignment))
#define PushStruct(arena, type) (type *)arena_alloc((arena), sizeof(type), 0)
#define PushStructZero(arena, type) (type *)arena_alloc((arena), sizeof(type), 1)
#define PushArray(arena, type, len) (type *)arena_alloc((arena), sizeof(type) * (len), 0)
#define PushArrayZero(arena, type, len) (type *)arena_alloc((arena), sizeof(type) * (len), 1)


#endif /* BASE_ARENA_H */
