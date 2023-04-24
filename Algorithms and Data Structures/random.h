#ifndef ALGO_RANDOM_H__
#define ALGO_RANDOM_H__

#include <stdio.h>
#include <stdint.h>

namespace algo{     //Linear congruential generator
    static uint32_t LCG() {
        static uint32_t a = 1664525U;
        static uint32_t c = 1013904223U;
        static uint32_t X0 = 0;

        X0 = a*X0+c;
        return X0;
    }
}

#endif
