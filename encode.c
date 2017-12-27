# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include "huffman.h"
# include "queue.h"
# include <ctype.h>
# include "code.h"
# include "bv.h"
# include <errno.h>
# include <getopt.h>

# define ALLCHARACTERS 256


/**
 * This function is used to build a histogram of the characters in the file to be encoded. 
 * @param filein inpute file given  
 * @param histogram  array to keep track of the occurences of all 256 characters  
 * @return uint32_t returns number of characters in the file needed for queue size. 
 */

uint32_t loadHistogram(int filein, uint64_t *histogram)
{
	ssize_t bytesRead = 0;
	uint32_t size = 0;
	unsigned char character;
	while ((bytesRead=read(filein,&character,1))>0)
      	{	
        	if ( bytesRead == -1)//error check
                {
                        perror("error in reading");
                        exit(errno);
                }
                        histogram [character] += 1;//increment the frequency for corresponding symbol
        }
      	for ( int a = 0 ; a<256 ; a++)//determines the size of the buffer by finding which characters are being used
      	{
                if (histogram [a] > 0)
                {
                        size++;
                }
      	}
	return size;
}

/** 
 * This function uses the priority queue that was created to dequeue lowest priority nodes and joining both nodes together then adding it back to the queue. Thus creating a huffman tree at the end of the process. 
 * @param q is priority queue created.
 * @return root treeNode of the tree. 
 */
treeNode *createTree(queue *q )
{
        treeNode *left = calloc(1,sizeof(treeNode *)) ;//create memory for temporary nodes to make huffman tree
        treeNode *right = calloc(1,sizeof(treeNode *)) ;
        treeNode *interior = calloc(1,sizeof(treeNode *));
        bool dequeue1 = true, enqueue1 = true;
        while (q->head-1 != q->tail )//run through priority queue
        {
                dequeue1 = dequeue(q,&left);//dequeue left
                if ( dequeue1 != false && enqueue1 == true)
                {
                        dequeue1 = dequeue(q,&right);//dequeue right
                        interior = join (left,right);//join left and right nodes
                        enqueue1 = enqueue (q, interior);//insert back into priority queue
                }
                else //special case of only one node  left on the queue
                {
                        interior = left;
                        enqueue1 = enqueue(q,interior);
                        break;
                }
        }
        treeNode *root = (q->Q[q->tail]);
        return root;
}

/**
 * This function writes valuable information to the output file needed for the decoder to work 
 * @param filein is input file 
 * @param fileout is output 
 * @param size is size of queue 
 * @param root is root of the tree 
 * @return size of file. 
 */	
uint64_t writeTofile(int filein, int fileout, uint32_t size, treeNode *root)
{
	uint32_t magicNumber = 0xdeadd00d;//magic number so decoder recognizes that the file has been encoded in the write style. 
      	write(fileout,&magicNumber,sizeof(magicNumber));

      	lseek(filein,0,SEEK_SET);
      	uint64_t filesize = lseek(filein,0L,SEEK_END); 
     	write(fileout,&filesize,sizeof(filesize));

      	uint16_t sizeofTree = (3*size)-1;
      	write(fileout,&sizeofTree,sizeof(sizeofTree));

      	dumpTree(root,fileout);
      	lseek(filein,0,SEEK_SET);
	
	return filesize;
}

/** 
 * This function appends codes of each character to the bitvector.
 * @param V is the bitvector containing all the codes. 
 * @param table is the codes for the character. 
 * @param x is the vector position. 
 */

void appendCode(bitV *V, code table, uint32_t x)
{
   	uint8_t value = 0;
   	for(uint32_t i = 0;i < table.l;i++)	
   	{
      		uint32_t bit = x+i;//incremeent bitvector within the forloop
      		if (x ==V->l)//create more memory for bitVector if capacity reached
      		{
         		V->l +=100000;
         		V->v = realloc(V,V->l);

      		}
      		value = (table.bits[i>>3]&(1<<(i&7)))>>(i&7);
      		if(value ==1)
      		{
         		setBit(V,bit); 
      		}
      		else
      		{	
         		clrBit(V,bit);
      		}
   	}	
}

/* 
 * Main function sets appropriate flags and calls appropriate functions to complete the encoding process. 
*/

int main(int argc, char*argv[])
{
      	int filein,option,closed;
	int fileout = STDOUT_FILENO;

      	bool verbose = false;
      	    	
	const char *infile, *outfile;
      	
	unsigned char character;
	uint64_t histogram[ALLCHARACTERS] = {0};

	//Use getopt with while statement to set input/output file flags and verbose flag 	
      	while (( option = getopt(argc,argv,"i:o:vp"))!=-1)
      	{
         	switch(option)
         	{
            	case 'i':
            	{
               		infile = optarg ;
               		filein = open(infile,O_RDONLY);
               		if (filein ==-1)//error check of opening file
               		{
                 	 	perror("error in opening file");
                  		exit(errno);
               		}
               		break;  
            		}
            	case 'o':
            	{
               		outfile = optarg ;              
               		fileout = open(outfile,O_WRONLY|O_CREAT,0644);
               		if (fileout == -1 ) //error checking for opening output file
               		{
                  		perror("error in opening file");
                  		exit(errno);
               		}
               		break;
            	}
            	case 'v': { verbose = true;  break;}            	
         	}
      	}
      		
	histogram [0]  = 1;//increment the first and last to at least have a two nodes created
      	histogram [255] = 1; 
	
	uint32_t size = loadHistogram(filein,&histogram[0]);	
	queue *q = createQueue(2*size,histogram); 
	
	treeNode *root = createTree(q);
      	
      	code s = newCode();//create new code structure
      	struct code mycode[256];//create new code array
      	buildCode((q->Q[q->tail]),s,mycode);	
	
	uint64_t filesize = writeTofile(filein,fileout,size,root);
      	
	bitV *bv = newVec(100000);// initialize a bitvector of 100000 bytes   	
	uint32_t vectorposition = 0;//keep track of position of vector 
      	ssize_t bytesRead = 0; 
	while ((bytesRead = read(filein,&character,1)) > 0)
      	{
         	if (bytesRead == -1)
         	{
            		perror("error reading file");
            		exit(errno);
         	}
         	appendCode(bv,mycode[character],vectorposition);//append the code to the bitVector
         	vectorposition+=mycode[character].l;//increment the vectorposition
      	}
      	write(fileout,bv->v,vectorposition/8+1); //write to Ofile
      	
	if (verbose == true)//if verbose flag turned on make calculations then print statement
      	{
         	uint64_t bits = filesize *8;
         	uint32_t leaves = size;
         	uint32_t bytes = (3*leaves) -1; 
         	uint64_t encoding = vectorposition;
         	float percentage = (float)encoding/bits *100;
         	printf("Original %lu bits: leaves %d (%d bytes) encoding %lu bits(%f%%).\n",bits,leaves,bytes,encoding,percentage);
      	}
      
      	delTree(q->Q[q->tail]);delQueue(q); delVec(bv); 
      	closed = close(filein);
      
	if (closed == -1)
      	{
         	perror("error with closing file");
         	exit(errno);
      	}
      	closed = close(fileout);
      
    	if (closed == -1)
      	{
         	perror("error with closing file");
         	exit(errno);
      	}
}         
