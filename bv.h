# ifndef _BVector
# define _BVector
# include <stdint.h>
# include "code.h"
/* This file contains the interface for a BitVector structure*/


/* defintion for bitvector structure*/
typedef struct bitV 
{
  uint8_t *v;
  uint32_t l;
} bitV;

/*creates new bitvector*/
bitV *newVec(uint32_t);

/*finds value of specific bitposition in bitvectore*/
uint8_t valBit(code  , uint32_t);

/*sets the value of a specific bitposition in a bitvector to 1*/
void setBit(bitV*, uint32_t);

/*sets the value of a spevific bitposition in a bitvectore to 0*/
void clrBit(bitV*, uint32_t);

/*deletes a bitvector structure*/
void delVec(bitV *);

# endif                
