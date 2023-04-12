/*
    This is a C++ code for a prime number testing algorithm that uses the Miller-Rabin
    primality test algorithm and the trial division algorithm.
*/

#ifndef ALGO_PRIME_H__       // include guard
#define ALGO_PRIME_H__

#include <stdlib.h>
#include <math.h>
#include "imath.H"      // include a custom header file

namespace algo{     // define a namespace
    static bool test_prime(unsigned int n){      // define a function that checks if a number is prime using the trial division algorithm
        switch(n){       // handle the base cases
            case 0:
            case 1: return false;
            case 2: return true;
        }
        if(n%2  ==0) return false;      // check if the number is even
        unsigned sqrtn=sqrt((double)n);      // find the square root of the number
        for(unsigned int i=3; i<=sqrtn; i+=2){      // iterate through all odd numbers up to the square root of the number
            if(n%i ==0){         // check if the number is divisible by i
                return false;
            }
        }
        return true;
    }
    static inline bool miller_rabin_test(unsigned int n){       // define a function that uses the Miller-Rabin primality test algorithm to check if a number is prime
        switch(n){      // handle the base cases
            case 0:
            case 1: return false;
            case 2:
            case 3: return true;
        }
        if (n%2 == 0) return false;     // check if the number is even
        unsigned s=ZerosR(n-1);     // calculate the highest power of two that divides n-1
        unsigned d=(n-1)>>s;        // calculate the odd part of n-1
        //Test 3-times
        for (int k=0; k<3; k++) {       // iterate 3 times
            unsigned a=rand()%(n-4)+2;  // choose a random integer a between 2 and n-2
            unsigned x=Exp(a,d,n);      // calculate a^d mod n
            //printf("%u%u%u%u\n",a,d   n,x);
            for(unsigned i=0;i<n;i++){      // iterate through all the bits of n-1
                x=Exp(x,2,n);       // calculate x^2 mod n
                if(x==1) return false;      // if x is 1, n is not prime
                if(x== n-1) continue;       // if x is -1 (mod n), continue with the next iteration
            }
            return false;       // if none of the iterations result in a conclusive answer, return false
        }
        return true;        // if all the iterations result in inconclusive answers, return true
    }
    /* Mixed Implementation */
    static inline bool is_prime(unsigned int n){        // define a function that uses both algorithms to check if a number is prime
        if(miller_rabin_test(n)){       // check if the number is prime using the Miller-Rabin primality test algorithm
            return test_prime(n);       // if it passes the test, check if it's prime using the trial division algorithm
        }
        return false;       // if it fails the test, return false
    }
}
#endif       // end of include guard and the code for the prime number testing algorithm 
