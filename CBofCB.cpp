/*
    Name:    Anna Devadas
    Email:   adev1@umbc.edu
    Section: 01
    Project: CMSC 341 Project 1
*/

#include "CBofCB.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// default constructor
CBofCB::CBofCB(){
    // initializes member variables 
    m_obSize = 1;
    m_oldest = 0;
    m_newest = 0;

    // creates a default InnerCB in the first index
    m_buffers[m_oldest] = new InnerCB;
    // iterates through rest of indices in outerCB and sets them equal to NULL
    for (int i = 1; i < m_obCapacity; i++){
        m_buffers[i] = NULL;
    }
}

// copy constructor
CBofCB::CBofCB(const CBofCB& other){
    // intializes member variables with same values as other
    m_obSize = other.m_obSize;
    m_oldest = other.m_oldest;
    m_newest = other.m_newest;

    // iterates through m_buffers and sets each ptr to NULL
    for (int i = 0; i < m_obCapacity; i++){
        if(other.m_buffers[i]){
          m_buffers[i] = new InnerCB(*other.m_buffers[i]);
        }
        else{
          m_buffers[i] = NULL;
        }
    }
}

//destructor, deletes dynamically allocated InnerCBs
CBofCB::~CBofCB(){
    // iterates through m_buffers
   for(int i=0;i < m_obCapacity; i++){
    // if InnerCB object exists at i index, delete
    if(m_buffers[i]){
      delete m_buffers[i];
      m_buffers[i] = NULL;
    }
  }
}


// add item to this data structure
void CBofCB::enqueue(int data){
    // if entire data structure is full throw error
    if(isFull()){
        throw overflow_error("Outer buffer is full. Overflow error.");
    }
    // if the newest InnerCB is full, create a new one
    else if(m_buffers[m_newest]->isFull()){
        // capacity of new InnerCB is twice that of existing one
        int newCap = m_buffers[m_newest]->capacity() * 2;
        // increments m_newest accordingly
        m_newest = (m_newest + 1) % m_obCapacity;
        //creates newest InnerCB, increments m_obSize, and enqueues data
        m_buffers[m_newest] = new InnerCB(newCap);
        m_obSize++;
        m_buffers[m_newest]->enqueue(data);
    }
    // just enqueue data
    else{
        m_buffers[m_newest]->enqueue(data);
    }
}

// remove item from this data structure
int CBofCB::dequeue(){
    // initialzes item to be returned
    int item;
    // if the whole data structure is empty throw error
    if(isEmpty()){
        throw underflow_error("Outer buffer is empty. Underflow error.");
    }
    // dequeue from oldest InnerCB
    else{
        item = m_buffers[m_oldest]->dequeue();
    }
    // after dequeue, if oldest is now empty, deallocate it
    if(m_buffers[m_oldest]->isEmpty()){
        // check to see that it's not only InnerCB left
        if(m_obSize != 1){
            delete m_buffers[m_oldest];
            m_buffers[m_oldest] = NULL;
            m_obSize -= 1;
            //increments oldest accordingly
            m_oldest = (m_oldest + 1) % m_obCapacity;
        }
    }
    return item;
}

// returns true if cannot add more items
bool CBofCB::isFull(){
    // if the size is not equal to the cpacity return false
    if(m_obSize != m_obCapacity){
        return false;
    }
    // if size is equal and the newest buffer is full return true
    else if(m_buffers[m_newest]->isFull()){
        return true;
    }
    else{
        return false;
    }
} 

// returns true if no items stored in data structure
bool CBofCB::isEmpty(){
    // intializes temp variables to be used in loop to check for empty
    bool empty = true;
    int itr = 0;
    int curr = m_oldest;

    // while bool is still true and hasn't iterated through size
    while(empty && itr < m_obSize){
        empty = m_buffers[curr]->isEmpty();
        curr = (curr + 1) % m_obCapacity;
        itr++;
    }
    return empty;
}

// number of items in the data structure as a whole.
// Note: not the number of InnerCB's
int CBofCB::size(){
    // intializes temp variables
    int total = 0;
    int itr = 0;
    int curr = m_oldest;

    // loop iterates through m_buffers that have InnerCBs
    while (itr < m_obSize){
        // sums total with size of each InnerCB
        total += m_buffers[curr]->size();

        // increments curr accordingly
        curr = (curr + 1) % m_obCapacity;
        itr++;
    }

    return total;
}

// overloaded assignment operator
const CBofCB& CBofCB::operator=(const CBofCB& rhs){
    // checks first for self-assignment
    if(this == &rhs){
        return *this;
    }

    // sets member variables equal to variables of rhs
    m_obSize = rhs.m_obSize;
    m_oldest = rhs.m_oldest;
    m_newest = rhs.m_newest;

     // iterates through m_buffers and deletes currently existing objects          
    for (int i=0; i < m_obCapacity; i++){
        if(m_buffers[i]){
          delete m_buffers[i];
        }
    }
      // iterates through capacity of m_buffers                                     
    for (int j = 0; j < m_obCapacity; j++){
    // if rhs object exists use InnerCB copy constructor to create new m_buffers object                                                                          
        if(rhs.m_buffers[j]){
          m_buffers[j] = new InnerCB(*rhs.m_buffers[j]);
        }
        // else set to NULL                                                         
        else{
          m_buffers[j] = NULL;
        }
    }

    return *this;
}

// debugging function, prints out contents of data structure
void CBofCB::dump(){
    cout << "-----------------------------------------" << endl;
    cout << "Outer Circular buffer dump(), m_obSize = ";
    cout << m_obSize << ":" << endl;

    // prints out only indices that point to created InnerCBs
    // calls InnerCB dump function
    int curr = m_oldest;
    while (curr != m_newest){
        cout << "[" << curr << "] "; 
        m_buffers[curr]->dump();

        curr = (curr + 1) % m_obCapacity;
    }
    cout << "[" << m_newest << "] ";
    m_buffers[m_newest]->dump();
    cout << "-----------------------------------------" << endl;
}

