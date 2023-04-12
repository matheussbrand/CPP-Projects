#ifndef ALGO_IMATH_H__
#define ALGO_IMATH_H__

#include <stdint.h>
#include <string.h>
#include <limits.h>

namespace algo {
    static uint32_t dot_product(const uint32_t * K, const uint32_t * A, uint32_t len){
        uint32_t i;
        uint32_t sum = 0;
        for (i = 0; i < len; i++){
            sum += A[i] * K[i];
        }
        return sum;
    }


#define KLEN 64
    static void m_based(uint64_t key, int m, uint32_t k, uint32_t K[]){
        memset(K, 0, sizeof(uint32_t)* KLEN);
        int quotient;
        int i = 0;
        while((quotient = k/m)!=0){
            K[i] = key%m;
            key = quotient;
            i++;
        }
        K[i] = key;
        }

    static unsigned Exp(unsigned base, unsigned exponent, unsigned modulus) {
        unsigned result = 1;
        while(exponent>0){
            if(exponent%2 == 1){
                result = (uint64_t(result)*base)%modulus;
            }
            exponent = exponent >> 1;
            base = (uint64_t(base)*base)%modulus;
        }
        return result;
    }

    static inline int ZerosR(unsigned int v){
        int c;
        if(v)
        {
            v = (v ^ (v-1)) >> 1;
            for(c = 0; v; c++){
                v >>= 1;
            }
        }
        else {
            c = CHAR_BIT * sizeof(v);
        }
        return c;
    }   
}

#endif