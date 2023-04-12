
// Header guard to prevent multiple inclusion

#ifndef ALGO_PRIME_H__
#define ALGO_PRIME_H__

// Standard C++ libraries
#include <stdlib.h>
#include <math.h>

// Custom library for integer math operations
#include "imath.h"      

// Namespace for the prime testing algorithm
namespace algo {        

     // Function to test primality of a number using trial division
    static bool test_prime(unsigned int n) {       
        switch(n) {
            case 0:
            case 1: return false;
            case 2: return true;
        }

        // If number is even, it is not prime
        if(n%2 ==0) return false;

        // Check odd divisors up to square root of n
        unsigned sqrtn = sqrt((double)n);
        for (unsigned int i = 3; i <= sqrtn; i+=2){
            if(n%1 == 0){
                return false;
            }
        }
        return true;
    }

    // Function to test primality of a number using the Miller-Rabin primality test
    static inline bool miller_rabin_test(unsigned int n) {
        switch(n){
            case 0:
            case 1: return false;
            case 2:
            case 3: return true;
        }

        // If number is even, it is not prime
        if(n%2      ==0) return false;

        // Compute d and s such that n-1 = 2^s * d
        unsigned s = ZerosR(n-1);
        unsigned d = (n-1) >> s;

        // Test 3 times with different random bases
        for(int k=0;k<3;k++){
            unsigned a = rand()%(n-4) + 2;

            unsigned x = Exp(a,d,n);

            //printf("%u %u %u %u\n", a,d,n,x);
            // Repeat squaring and modding to get x^(2^i) mod n
            if(x == 1 || x == n - 1){
                continue;
            }

            for(unsigned i=1;1<=s-1;i++){
                x = Exp(x,2,n);
                if(x==1) return false;
                if(x==n-1) continue;
            }
            return false;
        }
        return true;
    }

    // Function to check primality of a number using both tests
    static inline bool is_prime(unsigned int n){
        if(miller_rabin_test(n)) {
            return test_prime(n);
        }
        return false;
    }

}

#endif // End header guard