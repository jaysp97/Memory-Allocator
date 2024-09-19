// A1: UMalloc.h
// CS518
// Group Members: Jay Patil (jsp255) and Aishwarya Harpale (ach149)

#include <stdbool.h>
#include <stdio.h>

#define MAX_BLOCK_SIZE 10485760 // 10 MB 
#define malloc( x ) umalloc( ( x ) , __FILE__, __LINE__ ) // macro to replace malloc with umalloc
#define free( x ) ufree( ( x ) , __FILE__ , __LINE__ ) // macro to replace free with ufree

void init(); // check if the heap or memory is already initialized
void* umalloc(size_t size, char* file, int line); // main function of umalloc
void ufree(void* ptr, char* file, int line); // main function of ufree
char * getNextBlock(char *ptr); // get the next block of space
void mergeFreedBlocks(); // merge consecutive free blocks in the complete memory space
void prettyPrint(int max_count); // function to print the memory

typedef struct _metaData {
    bool isFree; // used to check if block is free
    int blockSize; // used to maintain current block size
}metaData;





