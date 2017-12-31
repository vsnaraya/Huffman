/** This file contains the main interface for the code structure*/



# ifndef _CODE_H 
# define _CODE_H
# include <stdint.h> 
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>


/*code struct */
typedef struct code 
{ 
    uint8_t bits [32]; 
    uint32_t l; 
} code;

/*creates a code struct*/
code newCode ();

/*pushes a number onto the stack*/
bool pushCode(code *c, uint32_t k);  

/*pops a numberfrom the stock*/
bool popCode(code *c, uint32_t *k); 

/*debug helper function*/
void printCode(code table);  
# endif
