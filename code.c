# include "code.h"
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>


/* creates a new code stucture*/
code newCode ()
{
    code t;
    for (int i = 0; i < 32; i += 1) { t.bits[i] = 0; }
    t.l = 0;
    return t;
}


/* pushes a 1 or 0 onto the code stack
 * @param c is the code structure 
 * @param k is the 0 or 1 being pushed onto the stack
 * @return bool is whether the stack is full or not. 
 */
bool pushCode(code *c, uint32_t k)
{
   if (c->l > 256)//if codestack is full return false
   {
        return false;
   }
   else if (k == 0)
   {
      c->bits[c->l / 8] &= ~(0x1 << (c->l % 8));
      c->l += 1;
   }
   else
   {
      c->bits[c->l / 8] |= (0x1 << (c->l % 8));
      c->l += 1;
   }
    return true;
}

/* pops a 1 or 0 from the code stack
 * @param c is the code structure 
 * @param k is the pointer that the 0 or 1 will be copied into.
 * @return bool is whether the code stack is empty or not. 
 */
bool popCode(code *c, uint32_t *k)
{
   if (c->l == 0)
   {
      return false;
   }
   else
   {
      c->l -= 1;
      *k = ((0x1 << (c->l % 8)) & c->bits[c->l / 8]) >> (c->l % 8); 
      return true;
   }
}


void printCode(code table)//prints the values of a code struct (debug)
{
   for (uint32_t x = 0; x < table.l; x++)
   {
      printf("%d",(table.bits[x>>3]&(1<<(x&7)))>>(x&7) );
   }
}

bool emptyCode(code *c) { return c->l == 0; }//not used 

bool fullCode (code *c) { return c->l == 256; } 
