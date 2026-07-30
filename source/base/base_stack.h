#ifndef BASE_STACK_H
#define BASE_STACK_H

#if defined(PLATFORM_UNIX)
#define StackCreate(size) unix_stack_create(size)
#define StackDestroy(stack) unix_stack_destroy(stack)
#elif defined(PLATFORM_WINDOWS)
#define StackCreate(size) win32_stack_create(size)
#define StackDestroy(stack) win32_stack_destroy(stack)
#endif

//-

typedef struct MemStackHeader MemStackHeader;
typedef struct MemStack MemStack;

struct MemStackHeader
{
    u8 padding;
    u8 previous_offset;
};

// TODO: fix all this dumb mem stack include issues where is it even coming from
struct MemStack
{
    MemStackHeader *header;
    u64             current_offset;
    u64             capacity;
    u8             *base_position;
};

//-

internal MemStack *stack_push_align(MemStack *stack, u64 size, u8 alignment);
internal MemStack *stack_resize_align(MemStack *stack, void *pointer, u64 old_size, u64 new_size, u8 alignment);
internal void     *stack_push(MemStack *stack, u64 size);
internal void     stack_pop(MemStack *stack, void *pointer);
internal void     stack_clear(MemStack *stack);
internal u8       calculate_padding(u64 pointer, u8 alignment, u64 header_size);


#endif // BASE_STACK_H
