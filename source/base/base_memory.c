internal void
__asan_posion_memory_region(void const volatile *addr, u64 size);


internal void
__asan_unposion_memory_region(void const volatile *addr, u64 size);


// TODO: add these after the arena allocatoins  to tell the address sanitizer that the meory is okay or  not okay