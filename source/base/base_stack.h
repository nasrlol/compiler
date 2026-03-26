#ifndef STACK_H
#define STACK_H

typedef struct mem_stack_header mem_stack_header;
struct mem_stack_header
{
    u8 padding;
    u8 previous_offset;
};


typedef struct mem_stack mem_stack;
struct mem_stack
{
    mem_stack_header *header;

    u64 current_offset;
    u64 capacity;
    u8 *base_position;
};

#endif
