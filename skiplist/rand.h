/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                Copyright (c) 2020 to present, All rights reserved          *
 ******************************************************************************
 *                                                                            *
 * Description: 						                                      *
 *                                                                            *
 * Author     : Jancd                                                         *
 *                                                                            *
 * Version    : v0.0.1                                                        *
 *                                                                            *
 * modification history                                                       *
 * --------------------                                                       *
 * Version    :                                                               *
 *                                                                            *
 *                                                                            *
******************************************************************************/

#ifndef SKIPLIST_RAND_H
#define SKIPLIST_RAND_H

#include <cstdint>

class RandGenerator {
public:
    explicit RandGenerator(uint32_t s) : seed(s & 0x7fffffffu) {
        // Avoid bad seeds.
        if (seed == 0 || seed == 2147483647L) {
            seed = 1;
        }
    }

    uint32_t Next();

    uint32_t Uniform(int);

    bool OneIn(int);

    uint32_t Skewed(int maxLog);

private:
    uint32_t seed;
};


uint32_t RandGenerator::Next() {
    static const uint32_t M = 2147483647L;   // 2^31-1
    static const uint64_t A = 16807;  // bits 14, 8, 7, 5, 2, 1, 0
    // We are computing
    //       seed_ = (seed_ * A) % M,    where M = 2^31-1
    //
    // seed_ must not be zero or M, or else all subsequent computed values
    // will be zero or M respectively.  For all other values, seed_ will end
    // up cycling through every number in [1,M-1]
    uint64_t product = seed * A;

    // Compute (product % M) using the fact that ((x << 31) % M) == x.
    seed = static_cast<uint32_t>((product >> 31) + (product & M));
    // The first reduction may overflow by 1 bit, so we may need to
    // repeat.  mod == M is not possible; using > allows the faster
    // sign-bit-based test.
    if (seed > M) {
        seed -= M;
    }
    return seed;
}

uint32_t RandGenerator::Uniform(int n) {
    return (RandGenerator::Next() % n);
}

bool RandGenerator::OneIn(int n) {
    return (RandGenerator::Next() % n) == 0;
}

uint32_t RandGenerator::Skewed(int maxLog) {
    return RandGenerator::Uniform(1 << RandGenerator::Uniform(maxLog + 1));
}


#endif //SKIPLIST_RAND_H
