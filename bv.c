# include <errno.h>
# include "bv.h"
# include "code.h"
/*
 * This function allocates the memory for a bitVector Structure 
 * @param x is the length of the bitvector 
 * return V is skeleton of bitVector.
 */
bitV *newVec(uint32_t x)
{
   struct bitV *V=(bitV*)malloc(sizeof(struct bitV));
   if(V==NULL)
   {
      perror("error w malloc");
      exit(errno);
   }
   V->v=(uint8_t *)calloc(x,(x/8)*sizeof(uint8_t));
   V->l=x;
   return V;
}

/*This function will use bitwise shifts to find the value of a bit in a code struct
 * @param table is the code table being used 
 * @param pos is the position of the  bit the value is being sought. 
 * return value is 0 or 1 depending on the result of the bitwise shift. 
*/
uint8_t valBit(code table, uint32_t pos)
{
   uint8_t value = table.bits[pos>>3]&(1<<(pos&7))>>(pos&7);  
   return value;
}

/* This function will use bitwise shifts to set a bit in a bitvector to 1
 * @bitVptr is a pointer to a bitVector that contains the bits.
 * @pos is the position of the bit in the bitVector that is being set to 1
 */
void setBit(bitV *bitVptr, uint32_t pos )
{
   int index = pos/8;
   int bit = pos%8;
   bitVptr->v[index]|=(01<<(pos&bit));
}

/* This function uses bitwise shifts to clear a bit in a bitvector to 0 
 * @bitVptr is a pointer to a bitVector that contains the bits.
 * @pos is the position of the bit in the bitVector that is being set to 0.
 */

void clrBit(bitV *bitVptr, uint32_t pos)//uses bitwise shift to clr the bit of a bitVector given a certain position
{
   int index = pos / 8;
   int bit = pos % 8;
   bitVptr->v[index]&=~(1<<(pos&bit));
}

/* This function frees the allocated memory for the bitVector 
 * @param bitVptr is the bitVector being deleted 
 */

void delVec(bitV *bitVptr)//free bitvector structure
{
   free(bitVptr->v);
   free(bitVptr);   
}  
