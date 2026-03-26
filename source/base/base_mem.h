#ifndef BASE_MEM_H
#define BASE_MEM_H

#define ARENA_ALIGN (2 * sizeof(void *))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

internal inline b8
is_pow(umm x)
{
    return (x & (x - 1)) == 0;
}

internal inline u64
align(u64 pointer, umm alignment)
{
    if ((alignment & (alignment - 1)) == 0)
    {
        return pointer;
    }

    return (pointer + alignment - 1) & ~(alignment - 1);
}


#endif
