# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include "stack.h"
# include "huffman.h"

# define MIN_STACK 10

/* This function allocates memory for a new stak structure and returns an epty stack.
 * @return s is the empty stack structure.
 */
stack *newStack()
{
        stack *s = (stack *) calloc(MIN_STACK, sizeof(stack));
        s->size = MIN_STACK;
        s->top  = 0;
        s->entries = (treeNode**) calloc(MIN_STACK, sizeof(treeNode*));
        return s;
}
/* This function frees all off the memory allocated for a stack structure. 
 * @param s is the stack structure being freed. 
 */

void delStack(stack *s) { free(s->entries); free(s); return; }

/* This function pops the first node off the stack and changes the treeNode pointed to that node. 
 * @param s is the stack  changes the treeNode pointed to that node.  
 * @param i is the treeNode pointer that the top node will be copied into.
 * @return bool is whether the stack is empty or not.
 */
bool  pop(stack *s, treeNode **i)
{
      
        if (s->top > 0)
        {
                
                s->top -=1;//decrease top by 1
                *i = s->entries[s->top];//copies top node into the empty node given 
                return true;
        } 
        else
        {
                return false;
        }
}
/* This function pushes a treeNode onto a stack structure. 
 * @param s is pointer to the stack structure.  
 * @param i is the treeNode being pushed onto the stack. 
 */
void push(stack *s, treeNode *i)
{
        if (s->top == s->size)//allocates more memory for stack if needed
        {
                s->size *= 2;
                s->entries = (treeNode**) realloc(s->entries, s->size * sizeof(treeNode*));
        }
        s->entries[s->top] = i;//makes top of stack equal to treeNode given
        s->top += 1; //increases the top by 1
        return;

}

bool empty(stack *s) { return s->top == 0; }
