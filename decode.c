# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include "huffman.h"
# include "stack.h"
# include <ctype.h>
# include <errno.h>
# include "bv.h"//include files for decode.c

/*This function will generate the huffman tree for the file that is being decompressed using the instructions from the encoder
 * @param savedTree is the array with the 
 * @param treeSize is amount of leaves in the tree
 * return root of the generated tree  
*/
treeNode *loadTree(uint8_t savedTree[], uint16_t treeSize)
{
   	stack *treestack = newStack();
   	bool pop1 = true;;
   	uint32_t arrayposition = 0;
   	treeNode *left = malloc(sizeof(treeNode *));
   	treeNode *right = malloc(sizeof(treeNode *));
   	treeNode *root = malloc(sizeof(treeNode *));
   	while (arrayposition < treeSize && pop1 ==true)
   	{

      		if (savedTree[arrayposition] == 'L')
      		{
         		uint8_t symbol = savedTree[arrayposition+1];
         		treeNode *tn = newNode(symbol,true,0);
         		push(treestack,tn);
        		arrayposition+=2;
		} 
      		else
      		{
         		pop1 = pop(treestack,&right);
         		pop1 = pop(treestack,&left);
         		treeNode *interior = join(left,right);
         		push(treestack,interior);
         		arrayposition++;
      		}
   	}
   	pop1 = pop(treestack,&root);
   	delStack(treestack);
   	return root;
}

/* This is the main fucntion for the decoder. 
 */

int main(int argc, char*argv[])
{   
	int filein,option,closed;
	int fileout = STDOUT_FILENO;
      
	uint32_t magicnumber;//magic number check for encoded file
      	uint64_t filesize;//filesize from encoded file
     	uint16_t treeSize;//treesize from encoded file
      
	ssize_t bytesRead;//error checking for reading bytes
      	const char *infile, *outfile;
      
	bool verbose = false;      
      	while (( option = getopt(argc,argv,"i:o:vp"))!= -1)//optarg used with while and switch
      	{
         	switch (option)
         	{
            	case 'i'://Mandatory case 'i' takes in an input file from command line and opens it
            	{
               		infile = optarg;
               		filein = open(infile,O_RDONLY);//open input file
               		if (filein == -1)//error checking
               		{
                  		perror("error in opening file");
                  		exit(errno);
               		}	
               		break;
            	}
	    	case 'o'://optional case 'o' takes in an output file for program to write the output too
            	{
               		outfile = optarg;
               		fileout = open(outfile,O_WRONLY|O_CREAT,0644);
               		if (fileout == -1) 
               		{
                  		perror("error in opening file");
                  		exit(errno);
               		}
             	  	break;
            	}
            	case 'v':{ verbose = true; break; }
         	}
      	}
      
	read (filein, &magicnumber, sizeof(magicnumber));//read in magic number from input file to make sure it has been encoded by the correct encoder
      	if (magicnumber != 0xdeadd00d)
      	{
         	printf("wrong magic number");
         	return 1;
      	}
      	read (filein,&filesize,sizeof(filesize));
      	read (filein,&treeSize,sizeof(treeSize));
      
      	uint8_t  *savedTree = calloc(treeSize,sizeof(uint8_t));
      	
	uint16_t index = 0;//index to move along the savedTree array
     	while (index < treeSize)
      	{
         	bytesRead = read(filein,&savedTree[index],1);
        	if (bytesRead == -1)
         	{
            		perror("error in reading");
            		exit(errno);
         	}
         	index++;
      	}
      	treeNode *root = loadTree(savedTree,treeSize);
      	uint8_t byte;
      	treeNode *t = root;
      	uint64_t leavesfound = 0;//leaves found should be less then fileseize until all characters have been found
      	while (leavesfound < filesize)
      	{
         	(bytesRead = read(filein,&byte,1));//read in one byte to byte variable 
         	if (bytesRead == -1)
         	{
            		perror("error in reading file");
            		exit(errno);
         	}
         	for (uint32_t bp = 0; bp < 8; bp++ )//iterate through ever bit in the byte
         	{
            		uint32_t code = (byte &(1<<bp))>>bp;
            		int32_t value = stepTree(root,&t,code);
            		if( value != -1)
            		{
               			write(fileout,&value,1); //write that symbol to the Ofile or stdout
               			leavesfound ++;
            		}
            		if (leavesfound == filesize)
            		{
               			break;
            		} 
         	} 
      	}
      	if (verbose == true)//if verbose flag turned on then print Verbose
      	{
         	uint64_t bits = filesize * 8;
	 	printf("Orginal bits:%lu tree (%d)\n ",bits,treeSize);
      	}
      
      	delTree(root);
      	free(savedTree);
      
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

