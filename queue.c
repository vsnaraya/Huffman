# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include "queue.h"
# include "huffman.h"
# include "code.h"


static void sortQueue(queue *q);

/* 
 * This function creates memory and space for a queue. 
 * @param size is desired size for queue. 
 * @return q - skeleton for queue. 
 */

queue *newQueue(uint32_t size)//creates a new queue and returns pointer to queue
{
	queue *q = (queue *) malloc(sizeof(queue));
	q->size = size;
	q->head = 0  ;
        q->tail = 0 ;
	q->Q = calloc(size, sizeof(treeNode *));
	return q;

}
/*
 * This function creates a queue and enqueues all nodes to the priority queue. 
 * @param size is the desired size of queue. 
 * @param bytes is the histogram the priority queue is going to be based off. 
 * return q filled queue
 */
queue *createQueue(uint32_t size, uint64_t bytes [])
{	
	queue *q = newQueue( size );
        bool enqueue1 = true;//enqueue boolean value       
        for ( uint32_t i = 0; i <256 ; i++)//enqueue all used characters into priority queue
        {
                if (enqueue1==true && bytes[i] > 0)//run through histogram
                {
                        char symbol = i;//store symbol of that number
                        treeNode *tn = newNode(symbol, true, bytes[i]);//creates new Node for each symbol
                        enqueue1 = enqueue(q, tn);//enquee that node
                }
        }
        return q;
}

/* This function frees all the allocated memory for the queue 
 * @param q  queue to be freed 
 */
void delQueue(queue *q) { free(q->Q); free(q); return; }//deletes a queue

bool empty(queue *q) { return q->head == q->tail ; }
bool full (queue *q) { return (q->head +1  ) % q->size == q->tail % q->size; }

/*This function is the enqueue function used to put nodes into the priority queue. Uses sortqueue function to sort the queue from lowest priority to highest priority. 
 * @param q is the queue that the item is going to be put in.
 * @param item is the treeNode that is going to be enqueued into the queue. 
 * @return bool is true if the queue is not false. If the queue is full it will return false. 
 */
bool enqueue(queue *q, item  i)//enqueue an item into the queue
{

	if (full(q))//check if the queue is full
	{
              return false;
	}
	else
	{
                	   
                q->Q[q->head] = i;//copy the item into the head of the queue
                sortQueue(q);//sort the queue
	        q->head = (q->head +1 ) % q->size;//increment the head by 1
               return true;
	}
}

/*This function takes an item pointer and assigns it an the item of lowest priority in the queue. 
 * @param q is the queue that is being dequeued from. 
 * @param item is the pointer that is going to be assigned a new node. 
 * @return bool is true if there is an item to be dequeued, and false if the queue is empty. 
 */
bool dequeue(queue *q, item *i)//deques an item from the queue
{
	if (empty(q))
        {
          	return false;
	}
	else
	{
                *i = q->Q[q->tail]; 
		q->tail = (q->tail + 1) % q->size;//increment tail
		return true;
	}
}

/*This function sorts the priority of items in the queue using an insertion sort method. 
 * @param q is the queue to be sorted. 
 */
 
static void sortQueue(queue *q)//INSERTION SORT VARIATION FOR QUEUE OF TREENODES
{
   for(uint32_t i = q->tail +1   ; i < q->head+1 ; i++)
   {
      treeNode *temp = q->Q[i];
      int j = i-1;
      while ( j >= 0  && (((q->Q[j])->count) > (temp)->count))
      {
         q->Q[j+1] = q->Q[j];
         j = j-1;
      }
      q->Q[j+1] = temp;
   }
}
