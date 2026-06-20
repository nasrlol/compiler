#ifndef BASE_STACK_WINDOWS_H
#define BASE_STACK_WINDOWS_H

internal mem_stack *
_stack_create(u64 capacity)
{
  mem_stack *stack = (mem_stack*)VirtualAlloc(
                                              NULL,
                                              capacity + sizeof(mem_stack),
                                              MEM_COMMIT | MEM_RESERVE,
                                              PAGE_READWRITE);


  return stack;
}

internal void *
_stack_destroy(mem_stack* stack)

b32 result = VirtualFree(stack,
                         0, // stands for the byte of commited pages
                         MEM_RELEASE);

if (!result) {
  // TODO: Logging
}

}



#endif /* BASE_STACK_WINDOWS_H  */
