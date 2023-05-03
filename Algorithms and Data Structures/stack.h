#ifndef ALGO_STACK_H__
#define ALGO_STACK_H__

#include <stdint.h>
#include <stdbool.h>
#include <exception>

namespace algo {
    template<typename T = uintptr_t>
        class Stack
        {
        private:
            // Define a custom exception class for empty stack
            class StackEmptyException: public std::exception {
                public:
                    virtual const char * what() const throw()
                    {
                        return "stack is empty";
                    }
            } excp_empty;

            // Define a custom exception class for out of bound index
            class StackIndexOutOfBoundException: public std::exception {
                public:
                    virtual const char * what() const throw() 
                    {
                        return "Index out of bound";
                    }   
            } excp_ioob;

            uint32_t m_capacity;
            uint32_t m_size;
            T * m_elements;

        public: 
            // Constructor to initialize the stack with a given capacity
            Stack(uint32_t capacity) {
                this->m_capacity = capacity;
                this->m_size = 0;
                this->m_elements = new T[capacity];
            }

            // Destructor to release the memory allocated for the stack
            ~Stack() {
                delete[] m_elements;
            }            
        
        private: 
            // Make the copy constructor and assignment operator private to disallow copying of the stack
            Stack(const Stack&);
            Stack& operator=(const Stack&);

        public:
            // Check if the stack is empty
            inline bool is_empty() const {return m_size == 0?true:false;}

            // Remove the top element from the stack
            inline void pop() {
                if(m_size!=0) m_size--;
                return;
            }

            // Return the top element from the stack
            inline const T& top() const {
                if(m_size == 0) throw excp_empty;
                return m_elements[m_size-1];
            }

            // Push a new element to the top of the stack
            inline bool push(const T& value) {
                if(m_size == m_capacity) {return false;}
                else{
                    m_elements[m_size++] =value;
                    return true;
                }
            }

            // Return the number of elements in the stack
            inline uint32_t count() const{return m_size;}

            // Access an element at a given index (0-based indexing from the top of the stack)
            inline const T& operator[] (uint32_t idx) const{
                if(idx >= m_capacity) throw excp_ioob;
                return m_elements[m_size-1-idx];
            }
        };            
}
#endif
