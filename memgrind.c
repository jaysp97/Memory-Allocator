// A1: UMalloc.h
// CS518
// Group Members: Jay Patil (jsp255) and Aishwarya Harpale (ach149)

#include<stdio.h>
#include<time.h>
#include "umalloc.h"

void consistency(){
	
	char* a = (char*)malloc(sizeof(char*));
	printf("After allocating A, address is: %p \n", a);
	free(a);
	printf("After freeing A\n");
	
	char* b = (char*)malloc(sizeof(char*));
	printf("After allocating B, address is: %p \n", b);
	if(*a == *b)
		printf("Test passed\n");
	
	free(b);

}

/*void maximization(){
	int val = 1;
	void * ptr = malloc(val);
	int size = 0;
	val *=2;
	void *prev = ptr;
	void *addresses[50];
	int i =0;
	while(ptr!=NULL || (val!=1 && val!=0)){
		
		printf("attempting to alloc %d\n", val);
		
		ptr = malloc(val);
		if(ptr==NULL)
			val /=2;
		else{
			//printf("size is %d\n", size);
			size += val;
			addresses[i] = prev;
			i++;
			prev=ptr;
			printf("allocated %d \n", val);
			val *=2;
		}
			
	}
	if(ptr==NULL && (val==0 || val==1))
	{
		printf("max allocation %d\n",ptr);
		printf("max allocation size %d\n",size);
		printf("num allocations: %d\n",i);
	}
	int j=0;
	while(j<i){
		//printf("Freeing %d\n",j);
		free(addresses[j]);
		j++;
	}
	printf("Freed\n");
}*/

void maximization(){
	int val = 1;
	void *addresses[50];
	void * ptr = malloc(val);
	int size=0;
	
	while(ptr!=NULL){
		free(ptr);
		size+=val;
		val = val*2;
		ptr=malloc(val);
	}
	size = size- val/2;
	
	ptr = malloc(val/2);
	free(ptr);
	
	printf("Maximum allocation size %d\n",size);
	printf("Max value %d\n",val);
	
	
}

void basic_coalescence(){
	int half = MAX_BLOCK_SIZE/2;
	int quarter = half/2;
	
	void *ptr1 = malloc(half);
	printf("Half allocated\n");
	void *ptr2 = malloc(quarter);
	printf("Quarter allocated\n");
	
	free(ptr1);
	printf("Half freed\n");
	free(ptr2);
	printf("Quarter freed\n");
	
	void *ptr3 = malloc(MAX_BLOCK_SIZE - 17);
	printf("Full allocated\n");
	free(ptr3);
	printf("Full freed\n");

}

void* saturation(void *addresses[]){
	int i=0, j=0;
	void *ptr, *prev;
	//void *temp = &mem[0];
	while(i<9216){
		prev=ptr;
		//printf("allocation num: %d\n",i);
		
		ptr = malloc(1024);
		addresses[j] = ptr;
		//temp = temp + sizeof(metaData) + ((metaData*)temp)->blockSize;
		//printf("is free: %d and blocksize %d \n", ((metaData*)temp)->isFree,((metaData*)temp)->blockSize);	
		i++;
		j++;
	}
	printf("Done %d allocations\n", i);

	i=0;
	while(ptr!=NULL){
		prev=ptr;
		//temp = temp + sizeof(metaData) + ((metaData*)temp)->blockSize;
		ptr=malloc(1);
		addresses[j] = ptr;
		i++;
		j++;
		//printf("1 bit allocation: %d\n",i);
		//printf("is free: %d and blocksize %d \n", ((metaData*)temp)->isFree,((metaData*)temp)->blockSize);
	}
	if(ptr==NULL){
		printf("Saturation point reached\n");
		printf("Total %d number of allocations\n",i+9216);
	}
	return prev;
}

void time_overhead(void *prev){
	struct timespec start,end;
	//printf("Prev : is free: %d and blocksize %d \n", ((metaData*)prev)->isFree,((metaData*)prev)->blockSize);
	free(prev);
	printf("Last 1B block freed\n");
	clock_gettime(CLOCK_MONOTONIC, &start);
	malloc(1);
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Allocated 1B \n");
	printf("Time elapsed = %ld nanoseconds \n",end.tv_nsec-start.tv_nsec);
}


void intermediate_coalescence(){
	void *addresses[117700];
	
	printf("---------------SATURATION-----------------\n");
	void *prev = saturation(addresses);
	printf("--------------------------------------------\n");
	
	printf("---------------TIME OVERHEAD-----------------\n");
	time_overhead(prev);
	printf("--------------------------------------------\n");
	
	printf("-----------INTERMEDIATE COALESCENCE------------\n");
	int j=0;
	printf("free started \n");
	while(j<117531){
		free(addresses[j]);
		//printf("free-ed %d\n",j);
		j++;
	}
	printf("free ended \n");
	void *ptr3 = malloc(MAX_BLOCK_SIZE - 17);
	printf("Full malloced\n");
	free(ptr3);
	printf("Full freed\n");
}


int main(){
	printf("---------------CONSISTENCY-----------------\n");
	consistency();
	printf("--------------------------------------------\n");
	
	printf("---------------MAXIMIZATION-----------------\n");
	maximization();
	printf("--------------------------------------------\n");
	
	printf("---------------BASIC COALESCENCE-----------------\n");
	basic_coalescence();
	printf("--------------------------------------------\n");
	
	
	//saturation();
	//time_overhead();
	
	intermediate_coalescence();
	printf("--------------------------------------------\n");
	return 0;
}
