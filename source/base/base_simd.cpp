#if 1
#include <arm_neon.h>
#endif // ARCH_ARM

#define u8x16 uint8x16_t

#define CheckSIMDNeon() do { CheckFPAdvSIMDEnabled64(); } while(0);
internal void neon_compare_u8(u8 *a,u8 *b) {

    // TODO: ...

}



#if 0
#include <intrin.h>
#endif