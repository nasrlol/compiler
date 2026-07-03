#ifndef BASE_STACK_WINDOWS_H
#define BASE_STACK_WINDOWS_H

// request a page from the operating system
internal MemStack *
_stack_create_windows(u64 capacity)
{
 MemStack *stack = (MemStack *)VirtualAlloc(
 NULL,
 capacity + sizeof(MemStack),
 MEM_COMMIT | MEM_RESERVE,
 PAGE_READWRITE);

 return stack;
}

internal void *
_stack_destroy_windows(MemStack *stack)
{
 b32 result = VirtualFree(stack,
 0, // stands for the byte of commited pages
 MEM_RELEASE);

 if(!result)
 {
  // TODO: Logging
 }
}

internal void
_stack_create_arena_windows(MemArena *arena, u64 capacity)
{
}

#endif /* BASE_STACK_WINDOWS_H  */
