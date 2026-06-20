#ifndef BASE_STACK_LINUX_H
#define BASE_STACK_LINUX_H

internal mem_stack *
stack_create(u64 capacity)
{
  mem_stack *stack = (mem_stack *)mmap(
                                       0,
                                       capacity + sizeof(mem_stack),
                                       PROT_READ | PROT_WRITE,
                                       MAP_SHARED | MAP_ANONYMOUS,
                                       -1,
                                       0);

  if (stack == MAP_FAILED)
  {
    return NULL;
  }

  stack->capacity       = capacity;
  stack->base_position  = (u8 *)stack + sizeof(mem_stack);
  stack->current_offset = 0;

  return stack;
}

internal void
stack_destroy(mem_stack *stack)
{
  if (!stack)
  {
    return;
  }

  munmap(stack, stack->capacity + sizeof(mem_stack));
}

#endif /* BASE_STACK_LINUX_H  */
