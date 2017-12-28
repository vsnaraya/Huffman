/** 
 * This file provides the interface to create and a functional priority queue. 
 */

 # ifndef _QUEUE_H
 # define _QUEUE_H
 # include <stdint.h>
 # include <stdbool.h>
 # include "huffman.h" 
 # include "code.h"

typedef treeNode* item ; // treeNode defined in huffman .h

typedef struct queue
{
    uint32_t size ; 
    uint32_t head , tail; 
    item *Q;  
} queue; 


 queue *newQueue ( uint32_t size ); // Constructor
 queue *createQueue(uint32_t, uint64_t []);
 void delQueue ( queue *q) ; // Destructor

 bool empty ( queue *q) ; 
 bool full ( queue *q) ; 

 bool enqueue ( queue *q, item       ) ; // Add an item

 bool dequeue ( queue *q, item * ) ; // Remove from the rear
 # endif

