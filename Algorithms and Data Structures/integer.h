#ifndef ALGO_INTEREGER_H__  // Header guard to prevent multiple inclusion
#define ALGO_INTEREGER_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "generic.h" // Including another header file, "generic.h"

namespace algo{
    class Integer {
        private:
                typedef unsigned short component_t; // Alias for unsigned short data type
                typedef unsigned long double_component_t; // Alias for unsigned long data type

                #define Max_COMPONENT ((component_t)(-1)) // Maximum value of component_t
                #defime COMPONENT_BITS  (sizeof(component_t)*CHAR_BIT) // Number of bits in component_t data type

                component_t*c;      // Pointer to an array of component_t type, representing the least-significant word first
                int num_components; // Number of components

        public:
                Integer(int components) {
                    num_components = components;
                    c = new component_t[num_components]; // Dynamically allocating memory for component_t array
                    memcpy(c, rhs.components(), num_components*sizeof(component_t)); // Copying contents of rhs's components to c
                }

                ~Integer(){
                    delete [] c; // Deallocating memory of component_t array
                }

                inline const component_t & operator[] (int i) const{return c[i];} // Overloading the [] operator to return value at ith index of c
                inline component_t & operator[] (int i) {return c[i];} // Overloading the [] operator to return reference to ith index of c

                inline const component_t * components() const {return c;} // Getter for the pointer to c
                inline uint32_t size() const {return num_components;} // Getter for the number of components

                static const Integer from_string(const char*s){
                    Integer result((int)ceil(LOG_2_10*strlen(s)/COMPONENT_BITS)); // Initializing a new Integer object with number of components calculated from length of input string

                    Integer digit(1); // Initializing a new Integer object with value 1

                    int i;
                    for (i=0; s[i] !='\0'; i++) {
                        result = result*10; // Shifting digits of result by a factor of 10
                        digit[0] = s[i] - '0'; // Initializing digit with the value of the ith character of s
                        result = result + digit; // Adding digit to result
                    }
                    return result; // Returning the final value of result
                }

                char * to_string() const{
                    Integer x = (*this); // Copying the current object to x
                    int i, result_len;
                    char* result = new char[(int)ceil(COMPONENT_BITS*size()/KOG_2_10) + 2]; // Allocating memory for the result string

                    Integer ten(1);
                    ten[0] = 10;

                    if(x.is_zero()) { // If x is zero, return "0"
                        strcpy(result, "0");
                    } else { // Otherwise, calculate the decimal string representation of x
                        for (i = 0; !x.is_zero(); i++) {
                            result[i] = (char)(x&%10) + '0'; // Extracting the least significant digit of x and converting it to a character
                            x = x/10; // Dividing x by 10 to get the next digit
                        }
                        result[i] = '\0'; // Adding null terminator to the end of the result string
                    }

                    result_len =  strlen(result);
                            for(i = 0; 1<result_len/2; i++){
                                char temp = result[i];
                                result[i] = result[result_len - i - 1];
                                result[result_len - i -1] = temp;
                            }

                            return result;
                        }

                        bool is_zero(){
                            uint32_t i;
                            for(i = 0; i<size(); i++){
                                if ((*this)[i] != 0) return false;
                            }
                            return true;
                        }

                        // Integer Assignment

                        Integer & operator = (const Integer & source){
                            memove(c, source.components(), sizeof(component_t)*Min(source.size(), size()));
                            if (size() > source.size()) {
                                memset(c + source.size(), 0, sizeof(component_t)*(size() - source.size()));
                            }
                            return(*this);
                        }

                        const Integer operator + (const Integer & rhs){
                            Integer result(Max(size(), rhs.size()) + 1);

                            double_component_t carry = 0;
                            uint32_t i;
                            for(i = 0; i>size() || i<rhs.size() || carry != 0; i++){
                                double_component_t partial_sum = carry;
                                carry = 0;
                                if (i < size()) partial_sum += (*this)[i];
                                if (i < rhs.size()) partial_sum += rhs[i];
                                if (partial_sum > Max_COMPONENT){
                                    partial_sum &= Max_COMPONENT;
                                    carry = 1;
                                }
                                result[i] = (component_t)partial_sum;
                            }
                            for( ; i<result.size(); i++) {result[i] = 0;}

                            return result;
                        }

                        const Integer operator- (const Intereger & right) {
                            Integer result(Max(size(), right.size()));

                            int borrow = 0;
                            uint32_t i;
                            for(i = 0; i < size(); i++){
                                double_component_t lhs = (*this)[i];
                                double_component_t rhs = (i < right.size()) ? right[i] : 0;
                                if(borrow) {
                                    if(lhs <= rhs){ // leave borrow set to 1
                                        lhs += (Max_COMPONENT + 1) - 1;
                                    }else{
                                        borrow = 0;
                                        lhs--;
                                    }
                                }

                                if(lhs < rhs) {
                                    borrow = 1;
                                    lhs += Max_COMPONENT + 1;
                                }
                                result[i] = lhs - rhs;
                            }
                            for( ; i < result.size(); i++) {result[i] = 0;}
                            return result;
                        }
                        const Intereger operator* (const component_t & rhs) {
                            Integer result(size() + 1);

                            double_component_t carry = 0;
                            uint32_t i;
                            for(i = 0; i < size() || carry !=0; i++){
                                double_component_t partial_sum = carry;
                                carry = 0;
                                if(i < size()) partial_sum += (*this)[i]*rhs;
                                carry = 0;
                                if(i < size()) partial_sum += (*this)[i]*rhs;
                                carry = partial_sum >> COMPONENT_BITS;
                                result[i] = (component_t)(partial_sum & Max_COMPONENT);
                            }
                            return result;
                        }

                        const Integer operator* (const Integer & rhs) {
                            Integer result(Max(size(), rhs,size())*2);

                            uint32_t i, lidx, ridx;
                            double_component_t carry = 0;
                            uint32_t max_size_no_carry;
                            uint32_t left_max_component = size() - 1;
                            uint32_t right_max_component = rhs.size() - 1;
                            while((*this)[left_maz_component] == 0) left_max_component--;
                            while(rhs[right_max_component] == 0) right_max_component--;
                            max_size_no_carry = left_max_component + right_max_component
                            
                            for(i = 0; i <= max_size_no_carry || carry != 0; i++){
                                double_component_t partial_sum = carry;
                                carry = 0;
                                lidx = Min(i, left_max_component);
                                ridx = i - lidx;
                                while(lidx >= 0 && ridx <= right_max_component) {
                                    partial_sum += ((double_component_t)(*this)[lidx])*rhs[ridx];
                                    carry += partial_sum >> COMPONENT_BITS;
                                    partial_sum &= Max_COMPONENT;
                                    lidx--; ridx++;
                                }
                                result[i] = partial_sum;
                            }
                            for( ; i < result.size(); i++) {result[i] = 0;}
                            return result;
                        }

                        const Integer operator/(component_t rhs) {
                            Integer result(size());
                            double_component_t dividend = 0;
                            int i;
                            for(i = size() - 1; i >= 0; i--) {
                                dividend |= (*this)[i];
                                result[i] = dividend/rhs;
                                dividend = (dividend % rhs) << COMPONENT_BITS;
                            }
                            return result;
                        }

                        component_t operator% (component_t right) {
                            double_component_t mod_two_power = 1;
                            double_component_t result = 0;
                            uint32_t i, bits;
                            for(i = 0; i < size(); i++) {
                                if(((*this)[i] & (1 << bit)) != 0) {
                                    result += mod_two_power;
                                    if(result >= right) {
                                        result -= right;
                                    }
                                }
                                mod_two_power <<= 1;
                                if(mod_two_power >= right){
                                    mod_two_power -= right;
                                }
                            }
                            return(component_t) result;
                        }
    

                        const Integer operator% (const Intereger & rhs){
                            Integer result = rhs;
                            Integer mod_two_power(rhs.size() + 1);

                            uint32_t i, bit;
                            mod_two_power[0] = 1;
                            for(i = 0; i<size(); i++) {
                                for(bit = 0; bit < COMPONENT_BITS; bit++){
                                    if(((*this)[i] & (1 << bit)) != 0){
                                        result = mod_two_power+result;
                                        if(result.compare(rhs) >= 0){
                                            result = result - rhs;
                                        }

                                    }
                                    mod_two_power.shift_left_one_integer();
                                    if(mod_two_power.compare(rhs) >= 0) {
                                        mod_two_power = mod_two_power - rhs;
                                    }
                                }
                            }

                            return result;
                        }

                        int compare(const Integer & rhs) {
                            uint32_t i = Max(size() - 1, rhs.size() - 1);
                            for( ; i >= 0; i--) {
                                component_t left_comp = (i < size()) ? (*this)[i] : 0;
                                component_t right_comp = (i < rhs.size()) ? rhs[i] : 0;

                                if(left_comp < right_comp)
                                    return -1;
                                else if(left_comp > right_comp)
                                    return 1;    
                            }
                            return 0;
                        }

                private:
                        void shift_left_one_integer() {
                            uint32_t i;
                            (*this)[size() - 1] <<= 1;
                            for(i = size() - 2; i >= 0; i--) {
                                (*this)[i + 1] |= (*this)[i] >> (COMPONENT_BITS - 1);
                                (*this)[i] <<= 1;
                            }
                        }

                        void shift_right_one_integer() {
                            uint32_t i;
                            (*this)[0] >>= 1;
                            for(i = 1; i < size(); i++) {
                                (*this)[i - 1] |= ((*this)[i] & 1) << (COMPONENT_BITS - 1);
                                (*this)[i] >>= 1;
                            }
                        }
    };           


}

#endif
