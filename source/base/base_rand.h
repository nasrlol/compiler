#ifndef BASE_RAND_H
#define BASE_RAND_H

// source: https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64

#define RAND_CONSTANT 6364136223846793005
#define GEN_RAND() _generate_random_u64(RAND_CONSTANT)

#endif  /* BASE_RAND_H  */

#ifdef BASE_IMPLEMENTATION

internal u64
generate_random_u64(u64 constant)
{
    time_t current_time = time(0);
    constant = current_time ^ constant;

    constant += 0x9e3779b97f4a7c15;

    u64 z  = constant;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    z = z ^ (z >> 31);

    return z;
}

#endif
