/*  
    This is a C++ code for a shuffle function that shuffles a list
    of elements randomly using the Fisher-Yates shuffle algorithm.
*/

#ifndef ALGO_SHUFFLE_H__    // include guard
#define ALGO_SHUFFLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace algo {     // define a namespace
    template <typename T>       // define a template function for any data type T
        static void shuffle(T* list, int len) {     // define a shuffle function that takes a list and its length as parameters
            srand(time(NULL));      // seed the random number generator
            int i = len, j;     // initialize two integer variables
            T temp;     // create a temporary variable of data type T

            if(i == 0)      // check if the length of the list is zero
                return;     // return if the list is empty
            while( --i){    // iterate through the list from the end to the beginning
                j = rand() %(i+1);      // generate a random index between 0 and i (inclusive)
                temp = list[i];     // swap the elements at index i and j
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }

#endif      // end of include guard and the code for the shuffle function.
