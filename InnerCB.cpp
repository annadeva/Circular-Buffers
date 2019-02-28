/*
    Name:    Anna Devadas
    Email:   adev1@umbc.edu
    Section: 01
    Project: CMSC 341 Project 1
*/

#include "InnerCB.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// Constructor, default size is 10.
// Dynaimcally creates buffer and initializes member variables
InnerCB::InnerCB(int n){
    // Initializes new buffer of size n
    m_buffer = new int[n];
    // Initializes member vairable of buffer
    m_capacity = n;
    m_size = 0;
    m_start = 0;
    m_end = 0;
}

// Copy constructor
InnerCB::InnerCB(const InnerCB& other){
    // initializes member variable to same values as other
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_start = other.m_start;
    m_end = other.m_end;

    // allocates memory for new buffer
    m_buffer = new int[m_capacity];
    // copies values from other buffer into same indices in new buffer
    for (int i=0; i < m_capacity; i++){
        m_buffer[i] = other.m_buffer[i];
    }
}

// Destructor for InnerCB
InnerCB::~InnerCB(){
    // Deletes dynamically allocated buffer
    delete [] m_buffer;
    m_buffer = NULL;
}

// Add item to circular buffer
void InnerCB::enqueue(int data){
    // if the buffer is full, throw overflow error
    if (isFull()){
        throw overflow_error("Inner buffer is full. Overflow error.");
    }
    if (isEmpty()){
        m_buffer[m_end] = data;
    }
    else{
        // if the m_end is currently at the last index of buffer, reset to 0
        if(m_end == m_capacity - 1){
            m_end = 0;
            m_buffer[m_end] = data;
        }
        // else increment m_end and load data
        else{
            m_end += 1;
            m_buffer[m_end] = data;
        }
    }    
    m_size++;
    
}

// Remove item from circular buffer
// Removes oldest item from buffer (m_start) and returns value
int InnerCB::dequeue(){
    // if the buffer is empty throw underflow error
    if (isEmpty()){
        throw underflow_error("Inner buffer is empty. Underflow error.");
    }
    // else create temp int to hold value at m_start
    int temp = m_buffer[m_start];
    
    // then increment m_start accordingly
    if(m_start == m_capacity - 1){
        m_start = 0;
    }
    else{
        m_start += 1;
    }
    // decrement size of buffer and return the oldest value
    m_size -= 1;
    return temp; 

}

// True if no space left in buffer
bool InnerCB::isFull(){
    // if the size of buffer is equal to its capacity, buffer is full
    if (m_size == m_capacity){
        return true;
    }
    else{
        return false;
    }
}

// True if buffer holds no items
bool InnerCB::isEmpty(){
    // if the buffer has zero items, size = 0, buffer is empty
    if (m_size == 0){
        return true;
    }
    else{
        return false;
    }
}

// return maximum number of items this buffer can hold
int InnerCB::capacity(){
    return m_capacity;
}

// return number of items currently held in the buffer
int InnerCB::size(){
    return m_size;
}

// overloaded assignment operator
const InnerCB& InnerCB::operator=(const InnerCB& rhs){
    // checks first for self-assignment, if true returns object
    if(this == &rhs){
        return *this;
    }

    // deletes LHS buffer, and re-initializes member variables
    delete [] m_buffer;
    //m_buffer = NULL;
    m_capacity = rhs.m_capacity;
    m_size = rhs.m_size;
    m_start = rhs.m_start;
    m_end = rhs.m_end;

    // reallocates memory for the buffer 
    m_buffer = new int[m_capacity];
    // sets values & indices in LHS buffer equal to RHS buffer
    for (int i=0; i < m_capacity; i++){
        m_buffer[i] = rhs.m_buffer[i];
    }

    // returns LHS buffer
    return *this;
} 

// debugging function. Prints out contents.
void InnerCB::dump(){
    cout << "InnerCB dump(): m_size = " << m_size << endl;

    int curr = m_start;
    // while loop iterates through the buffer starting at first element 
    // stops before reaching the last element 
    while(curr != m_end){
        cout << "[" << curr << "] " << m_buffer[curr] << ", ";

        if(curr == m_capacity - 1){
            curr = 0;
        }
        else{
            curr++;
        }    
    }
    // prints the last element is desired format
    cout << "[" << m_end << "] " << m_buffer[m_end] << ", ";
    cout << endl;
}
