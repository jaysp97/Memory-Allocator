// A1: UMalloc.c
// CS518
// Group Members: Jay Patil (jsp255) and Aishwarya Harpale (ach149)

#include <stdbool.h>
#include <stdio.h>

#include "umalloc.h"

static char mem[MAX_BLOCK_SIZE]; // global declaration for our logical memory
bool isMemoryInitialized = false; // initially memory is not initialized. Contains garbage values

void init(){
    ((metaData*)&mem[0])->isFree = true; // we start allocating from the first address of memory
    ((metaData*)&mem[0])->blockSize = MAX_BLOCK_SIZE - sizeof(metaData)-1; // we set the block size available
    isMemoryInitialized = true;
}

void * umalloc(size_t size, char * file, int line){

    if(size == 0){ // Checking base condition for undefined size 
        printf("%s:%d Undefined size for allocation. Please provide correct size to be allocated\n", file,line);
        return NULL;
    }
    if(size> MAX_BLOCK_SIZE - sizeof(metaData)){ // Checking base condition for overcommittal of memory
    	
    	printf("%s:%d Unable to allocate the given block size\n", file,line);
    	return NULL;
    }
	
    if(!isMemoryInitialized){
        init();
    }
    
    char * temp = &mem[0]; // we use a temporary element to iterate over our memory until we find a free block
    char *prev = temp;
    int requiredSize = size + sizeof(metaData);
    char * lastAddress = &mem[MAX_BLOCK_SIZE]; // used to check while iterating temporary element 

    while(temp < lastAddress){
        if(((metaData*)temp)->isFree && ((metaData*)temp)->blockSize >= requiredSize){
        	if(((metaData*)temp)->blockSize == requiredSize){
        		((metaData*)temp)->isFree = false;
        		return (void*)temp + sizeof(metaData);
        	}
        	break;
        }else if(((metaData*)temp)->isFree && ((metaData*)temp)->blockSize < requiredSize){
        	printf("%s: %d Memory is not full but sufficient memory is not available for allocation\n",file, line);
        	return NULL;
        }else if(!((metaData*)temp)->isFree && ((metaData*)temp)->blockSize == 0){
        	printf("%s: %d Sufficient memory not available\n",file, line);
        	return NULL;
        }
        prev = temp;
        temp = getNextBlock(temp); 
    }

    if(temp == NULL){
        printf("%s: %d Sufficient memory not available\n",file, line);
        return NULL;
    }
    else{
    	int remainingSpace = ((metaData*)temp)->blockSize - sizeof(metaData) - size;
        ((metaData*)temp)->isFree = false; // set current block as in use
        ((metaData*)temp)->blockSize = size; // set current blockSize equal to the requested size
	
       // initialize the next free block

        char* newFreeBlock = temp + sizeof(metaData) + size; 
        ((metaData*)newFreeBlock)->isFree = true;
        ((metaData*)newFreeBlock)->blockSize = remainingSpace;

        return (void*)(temp + sizeof(metaData));
    }

}

char* getNextBlock(char *ptr){ // returns the next free block
    char *nextBlock2 = ptr + ((metaData*)ptr)->blockSize + sizeof(metaData); // increment by blocksize
    if(nextBlock2>= &mem[MAX_BLOCK_SIZE]){
        return NULL;
    }else{
        return nextBlock2;
    }
}

void mergeFreedBlocks(){
    char *temp2 = &mem[0];
    char *nextBlock = NULL;
    size_t mergedSize = 0;

    while(temp2!=NULL && temp2 < &mem[MAX_BLOCK_SIZE]){
	
        if(((metaData*)temp2)->isFree){ // if the current block is free
            nextBlock = getNextBlock(temp2); // get next block

            if(nextBlock==NULL || nextBlock >= &mem[MAX_BLOCK_SIZE]){
                return; // next block out of range, no need to merge
            }
		    mergedSize += ((metaData*)nextBlock)->blockSize + sizeof(metaData);
            if(((metaData*)nextBlock)->isFree){ // if consecutive blocks are free merge them
                 // update block size
                ((metaData*)temp2)->blockSize += mergedSize; // increment size of merged blocks
                mergedSize = 0;
                nextBlock = getNextBlock(nextBlock); // continue with the next blocks	
            }else{
                temp2 = getNextBlock(temp2); // continue till consecutive free blocks are found
            }
        }else{
            temp2 = getNextBlock(temp2);
        }
    }
}

void ufree(void* ptr, char* file, int line){
    if(ptr == NULL){
        printf("%s:%d Incorrect address provided to ufree\n", file,line);
        return ; 
    }
    
    if(!isMemoryInitialized){
        printf("%s:%d Memory is not initialized to be freed\n", file,line);
        return; 
    }
    
    if( (char*)ptr > &mem[MAX_BLOCK_SIZE] - sizeof(metaData) || (char*)ptr < mem + sizeof(metaData)){
        printf("%s:%d Address out of range\n", file,line);
        return ; 
    }
    
    char *temp = &mem[0]; // start checking from the start address
    char *blockAddress = (char *)((char *) ptr-sizeof(metaData)); // type cast the address to char*

    while (temp < &mem[MAX_BLOCK_SIZE])
    {
        if(temp == blockAddress){ // if the block is found
            if(((metaData*)temp)->isFree){
                printf("%s:%d Address specified has already been freed\n", file,line);
                return; 
            }

            ((metaData*)temp)->isFree = true; // set the block as freed
            mergeFreedBlocks(); // if consecutive blocks are free, merge them
            
            return;
        }
        temp = getNextBlock(temp);
    }
    printf("%s:%d Error in free\n", file, line); 
    return ;

}
void prettyPrint(int max_count){
    printf("************Pretty print*********\n");
    void *temp = &mem[0];
    int count = 0;
    while(count<max_count){
        printf("Metadata address : %p\n", temp);
        printf("Is free: %d\n", ((metaData*)temp)->isFree);
        printf("Block size: %d\n\n", ((metaData*)temp)->blockSize);
        temp = temp + sizeof(metaData) + ((metaData*)temp)->blockSize;
        count++;
    }
}


