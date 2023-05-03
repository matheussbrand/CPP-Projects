#ifndef ALGO_HEAP_H__
#define ALGO_HEAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "generic.h" 

namespace algo {

    template<typename T>
    class Heap {
        public:
            // Structure that represents an element in the heap
            struct elem {
                public:
                    int key; // Key used for sorting
                    T data; // Payload data associated with the element
            };

        private:
            int m_size; // Number of elements in the heap
            int m_max; // Maximum number of elements in the heap
            elem * m_heap; // Pointer to the heap data

        public:
            // Constructor
            Heap(int max) {
                m_size = 0;
                m_max = max;
                m_heap = new elem[m_max];
            };

            // Destructor
            ~Heap() {
                delete[] m_heap;
            };

        private:
            // Copy constructor and assignment operator are disabled
            Heap(const Heap &);
            Heap& operator=(const Heap&);

        public:
            // Returns the number of elements in the heap
            inline int count() const { return m_size; };

            // Inserts an element with a given key and data
            void push(int key, const T& data) {
                if (m_size == m_max) {
                    return; // Heap is full, cannot push more elements
                }

                m_heap[m_size].key = key;
                m_heap[m_size].data = data;
                up(m_size); // Fix the heap property by moving the newly inserted element up
                m_size++; // Increment the size of the heap
            }

            // Returns true if the heap is empty, false otherwise
            inline bool is_empty() const {
                return (m_size == 0) ? true : false;
            }

            // Removes all elements from the heap
            inline void clear() {
                m_size = 0;
            }

            // Returns true if the heap contains an element with the given data
            bool contains(const T& data) {
                for (int i = 0; i < m_size; i++) {
                    if (m_heap[i].data == data) {
                        return true;
                    }
                }
                return false;
            }

            // Removes and returns the element with the minimum key
            elem pop() {
                int n = m_size - 1;
                swap(m_heap[0], m_heap[n]); // Swap the root with the last element
                down(0, n); // Fix the heap property by moving the root down
                m_size--; // Decrement the size of the heap
                return m_heap[m_size]; // Return the element that was removed
            }

            // Removes the element with the given data from the heap
            bool remove(const T& data) {
                for (int i = 0; i < m_size; i++) {
                    if (m_heap[i].data == data) {
                        int n = m_size - 1;
                        if (n != i) {
                            swap(m_heap[i], m_heap[n]); // Swap the element with the last element
                            down(i, m_size); // Fix the heap property by moving the swapped element down
                            up(i); // Fix the heap property by moving the swapped element up
                        }

                        m_size--; // Decrement the size of the heap
                        return true;
                    }
                }
                return false; // Element not found in the heap
            }
             // Decrease the key of an element in the heap
            void decrease_key(const T& data, int newkey) {
                // Remove the element from the heap and push it back with the new key
                if(remove(data)) {
                    push(newkey, data);
                }
            }
            void up(int j) {
               for(;;) {
                 int i = (j-1)/2;
                 if(i == j || !less(j,i)) {
                    break;
                    }
                 swap(m_heap[i], m_heap[j]);
                 j = i;
                }
             }

             void print_heap() {
               for(int i = 0; i<m_size; i++) {
                  printf("key:%d value:%d", m_heap[i].key, m_heap[i].data);
                  }
                  printf("\n");
            }

            bool less(int i, int j) {
               return m_heap[i].key < m_heap[j].key;
            }

        
    };    
}

#endif
