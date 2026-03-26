#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#define Align(pointer, alignment) align((u64)(pointer), (umm)(alignment))
#define PushStruct(arena, type) (type *)arena_alloc((arena), sizeof(type), 0)
#define PushStructZero(arena, type) (type *)arena_alloc((arena), sizeof(type), 1)
#define PushArray(arena, type, len) (type *)arena_alloc((arena), sizeof(type) * (len), 0)
#define PushArrayZero(arena, type, len) (type *)arena_alloc((arena), sizeof(type) * (len), 1)

typedef struct mem_arena  mem_arena;
struct mem_arena
{
    u64 current_position;
    u64 previous_position;
    u64 capacity;
    u8 *base_position;
};

typedef struct temp_arena temp_arena;
struct temp_arena
{
    mem_arena *arena;
    u64        start_position;
};

#endif /* BASE_ARENA_H */
