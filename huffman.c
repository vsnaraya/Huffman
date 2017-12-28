# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include "huffman.h"
# include "code.h"


/*This function allocates memory for a new treeNode and returns the node. 
 * @param s is the symbol or character in the file that the node will represent. 
 * @param l is whether the node is a leaf node or not. 
 * @param count is the number of occurences that symbol appears in the file .
 * @return tn is the treeNode.
 */
treeNode *newNode(uint8_t s, bool l, uint64_t count)//allocate memory for a treeNode and return 
{ 
   	treeNode *tn = calloc(1,sizeof(treeNode));
   	tn->symbol =s;
   	tn->leaf =l;
   	tn->count =count;
   	tn->left = NULL;
   	tn->right = NULL;
   	return tn;
}
/*This function dumps a created tree into an output file so the decoder can rebuild the same exact tree.It does a post-order traversal of the tree.  
 * @t is the root of the file 
 * @file is the Ofile.
 */
void dumpTree( treeNode *t, int file)//post order traversal
{
   	if (t->leaf)//if t is a leaf 
   	{
      		write(file,"L",sizeof(uint8_t));//write a L to the ofile
      		write(file,&t->symbol,sizeof(uint8_t));//followed by the symbol
      		return;
   	}
   	else 
   	{
      		dumpTree(t->left,file);//recursion to move through tree
      		dumpTree(t->right,file);
      		write(file,"I",sizeof(uint8_t));//write I if come across an interior node 
   	}         	   
}

/*This function will join two leaf nodes together by creating an interior node. 
 * @param l is one leaf node. 
 * @param r is another leaf node. 
 * @return tn is the interior node containing the summed up counts of both the leaf nodes. 
 */
treeNode *join(treeNode *l, treeNode *r)//joins two subtrees
{
   treeNode *tn = newNode('$',false,l->count + r->count);//creates newNode with counts of left and right
   tn->left = l;//set pointers correspondingly
   tn->right = r;
   return tn;
}

/*This function uses recursion and stacks to build the codes for each symbol in the tree. 
 * @param t is the root of the tree
 * @s is the code structure being built
 * @table is to hold all of our codes to ge to each leaf 
 */
void buildCode(treeNode *t, code s, code table [256])//recursion to find codes for each node post order traversal
{
   bool check = true;
   uint32_t bit;
   if (t->leaf == true && check == true)//if tree is a leaf
   { 
      table[t->symbol]= s; //copy the stack into the code table
      return;
   }
   else 
   {
      check = pushCode(&s,0);
      buildCode(t->left,s, table);
      check = popCode(&s,&bit);
      check = pushCode(&s,1);
      buildCode(t->right,s,table);
      popCode(&s,&bit);
   }
}
/* This function uses the codes that were made to navigate through the huffman tree to reach the correct symbols.
 * @ root is the root of the huffman tree that was built
 * @ param t is a pointer to where the program is in the tree 
 * @ param code are the instructions to navigate through the tree 
 * @ return int32_t is the symbol that the code is referring too.
 */ 
int32_t stepTree(treeNode *root, treeNode **t, uint32_t code)//stepTree function used to traverse through the huffman tree
{
   	if(code ==0)//if code given is == to 0 move the node to the left pointer of the node
   	{
      		*t = (*t)->left;
   	}
   	else//if the code is 1 move the node to the right pointer of the node
   	{
      		*t = (*t)->right;
   	}
   	if ((*t)->leaf ==true)//if the node is a leaf node find the symbol and then returned the symbol
   	{
     		 int32_t value = (*t)->symbol;
      		*t = root;
      		return value;
   	}
   	return -1;//if interior node return -1
}
 

/*This function frees the allocated memory for a treeNode.
 * @param n is the node to be freed.
 */
void delNode(treeNode *n)//free node
{
   free(n);
}

/*This function frees the memory of a whole tree using recursion.
 *@param t is the root of the tree.
 */
void delTree( treeNode *t)
{
   if (t->leaf == true)
   {
      delNode(t);
      return;
   }
   else
   {
      delTree(t->left);
      delTree(t->right);
   }
   delNode(t);//free root
}
