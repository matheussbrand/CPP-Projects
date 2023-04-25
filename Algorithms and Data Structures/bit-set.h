#ifndef ALGO_BIT_SET_H__
#define ALGO_BIT_SET_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

namespace algo {    //definition of bitset class
    class BitSet{
        
        private:
            uint32_t m_size;    // size in bits
            uint32_t m_bytes;   // size in bytes
            unsigned char * m_bits;     // the bits
        
        public:      
            BitSet(uint32_t num_bits){      // construct BitSet by a give number of bits
                // round up
                m_bytes = num_bits/8+1;
                m_size = m_bytes * 8;
                m_bits = new unsigned char[m_bytes];
            }
            ~BitSet(){      // safely free
                delete[] m_bits;
            }
        
        private:
            inline void ste(uint32_t bit){      // set 1 to position [bit]
                if(bit >= m_size) return;

                uint32_t n = bit/8;
                uint32_t off = bit%8;

                m_bits[n] |= 128U >> off;
            }

            inline void unset(uint32_t bit){    // set 0 to position [bit]
                if(bit >= m_size) return;

                uint32_t n = bit/8;
                uint32_t off = bit%8;

                m_bits[n] &= ~(128U >> off);
            }   

            inline bool test(uint32_t bit){     // test a bit , true if set, false if not.
                if(bit >= m_size) return false;

                uint32_t n = bit/8;
                uint32_t off = bit%8;

                if(m_bits[n] & (128U >> off)) return true;
                return false;
            } 
    };
}
#endif