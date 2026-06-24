#ifndef BASE_STACK_WINDOWS_H
#define BASE_STACK_WINDOWS_H

internal MemStack *
_stack_create(u64 capacity)
{
    MemStack *stack = (MemStack*)VirtualAlloc(
                                              NULL,
                                              capacity + sizeof(mem_stack),
                                              MEM_COMMIT | MEM_RESERVE,
                                              PAGE_READWRITE);


    return stack;
}

internal void *
_stack_destroy(MemStack* stack) {

    b32 result = VirtualFree(stack,
                             0, // stands for the byte of commited pages
                             MEM_RELEASE);

    if (!result) {
        // TODO: Logging
    }

}



#endif /* BASE_STACK_WINDOWS_H  */
