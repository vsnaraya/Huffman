CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror  -g

.PHONY: all

all: encode decode


encode: encode.o queue.o huffman.o  bv.o code.o 
	$(CC) $(CFLAGS) -g -o encode encode.o  huffman.o queue.o  bv.o code.o

decode: decode.o huffman.o stack.o code.o 
	$(CC) $(CFLAGS) -g -o decode decode.o huffman.o stack.o code.o 

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c


queue: queue.o 
	$(CC) $(CFLAGS) -g -o queue queue.o huffman.o 
queue.o: queue.c 
	$(CC) $(CFLAGS) -c queue.c 


huffman: huffman.o 
	$(CC) $(CFLAGS) -g -o huffman huffman.o

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c


bv.o: bv.c 
	$(CC) $(CFLAGS) -c bv.c 
clean:
	rm -f encode.o queue.o huffman.o code.o bv.o stack.o
