#if ARCH_ARM
#include <arm_neon.h>
#

#define u8x16 uint8x16_t

#define CheckSIMDNeon() do { CheckFPAdvSIMDEnabled64(); } while(0);
internal void neon_compare_u8(u8 *a,u8 *b) {

 // TODO: ...

}

#elif ARCH_X86

#include <intrin.h>
// TODO: ...

#endif // ARCH