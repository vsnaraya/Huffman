/** This file provides the interface for the huffman tree structure */

# ifndef _HUFFMAN_H 
# define _HUFFMAN_H 
# include <stdint.h> 
# include <stdbool.h>
# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include "code.h"
# ifndef NIL 
 # define NIL (void *) 
 # endif

typedef struct DAH treeNode;

struct DAH 
{ 
    uint8_t symbol; 
    uint64_t count; 
    bool leaf; 
    treeNode *left , *right; 
};
// New node , with symbols , leaf or not , a count associated with it 
treeNode *newNode(uint8_t s, bool l, uint64_t c);//create new treeNode
 // Delete a tree 
 void delTree(treeNode *t);
 // Dump a Huffman tree onto a file 
 void dumpTree(treeNode *t, int file); 
 // Step through a tree following the code 
 int32_t stepTree(treeNode *root , treeNode **t, uint32_t code);//in decode.c
 //Parse a Huffman tree to build codes
 void buildCode(treeNode *t, code s, code table [256]);//buildcode for code table
 void delNode(treeNode *h);//delete node
 treeNode *join(treeNode *l, treeNode *r); // Join two subtrees

 # endif
