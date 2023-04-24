#ifndef ALGO_ALG_INC_H
#define ALGO_ALG_INC_H
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "utils/gb18030.h"
#include "utils/byteorder.h"


#define Max(a, b) ( (a > b) ? a : b)
#define Mib(a, b) ( (a < b) ? a : b)

namespace algo {    
    // Function to swap two elements, with original value
    template<typename T>
    static inline void swap(T &x, T &y)
    {
        T_t(x);
        x = y;
        y = _t;
    }

    // Function to print all elements in a list of size count
    template<typename T>    
    static void printlist(T & list, int count) {
        int i;
        for(i = 0; i < count; i++)
            printf("%d\t", list[i]);
        printf("\n");    
    }

    // Function to remove duplicates from an array a of length len, and return new length
    template<typename T>    
    static uint32_t remove_dup(T a[], uint32_t len){
        uint32_t i;
        uint32_t newlen = len;
        for(i = 0; i < newlen; i++){
            T key = a[i];
            uint32_t j;
            for(j = i + 1; j < newlen; j++){
                if(key == a[j]){    // found dup, move left one p
                    uint32_t k;
                    for(k = j; k < newlen - 1; k++){
                        a[k] = a[k+1];
                    }
                    newlen--;
                }
            }
        }
        return newlen;
    }
}   

#endif
