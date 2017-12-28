 // This file is main interface for stack structure.  
 # ifndef _STACK_H 
 # define _STACK_H 
 # include <stdint.h> 
 # include <stdbool.h> 
 # include "huffman.h" 
 

 typedef struct stack 
 { 
   uint32_t size; // How big? 
    uint32_t top; // Where ’s the top? 
    treeNode  **entries; // Array to hold it (via calloc) 
 } stack;

# define INVALID 0xDeadD00d;

 stack *newStack (); // Constructor 
 void delStack(stack *); // Destructor
 bool pop (stack *, treeNode **i); // Returns the top item 
  void push(stack *, treeNode *i); // Adds an item to the top
 bool empty(stack *); // Is it empty? 
  bool full (stack *); // Is it full?


 # endif
