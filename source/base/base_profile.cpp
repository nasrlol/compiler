#if ARCH_x86_64
internal Profile *x86_start_profile(MemArena *arena, void *callback)
{
 ScratchArena *scratch = scratch_start(arena);
 u64 start_high, start_low, end_high, end_low;
 GetCycles(start_high, start_low);
 Profile *p = PushStruct(arena, Profile);
 p->start = start_high - start_low;
 callback;
 GetCycles(end_high, end_low);
 p->end = end_high - end_low;
 return p;
 }


 internal void x86_end_profile(Profile *profile, ScratchArena *scratch)
 {
 Log("log the profile or something");
 end_scratch(scratch);
}

#elif ARCH_ARM

#else
#error platform not supported for profiling
#endif