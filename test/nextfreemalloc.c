/*
This program was written by:
Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
Ben Clive bac2g10@ecs.soton.ac.uk

This test implements the next free algorithm

|_Pointer___|_Block 1___________________|_Block 2__________|
|_Next free_|_Size/free_|_Next Free ptr_|_Size/free_|_Data_|

+ Array[0] contains pointer to the next free location after 
  the previously allocated location
+ Array[BlockNo] contains a positive size if free and a 
  negative size if allocated. This works nicely for checking
  that a free block is big enough. 
+ Array[BlockNo + 1] of a free block - contains a pointer to
  the next free block
+ Array[BlockNo + 1] is the pointer returned by my malloc

Note: Size does not include overheads

*/
#include "nextfreemalloc.h"
#define MIN_ARRAY_SIZE 128
#define OVERHEADS 2

int myinit(int *array, int size){

	//Array must be big enough to initalize
	if(size >= MIN_ARRAY_SIZE) {
		//The first block starts in position 1
		array[0] = 1;
		//The first entry of block 1 is it's size
		array[1] = size - OVERHEADS;
		//Only the first block exists so send it back to the start
		array[2] = 1;
		return 1;
	}
	else return 0;
}
	

int * mymalloc(int *array, int size) {

	int next = array[0];
	int start = next;

	//as long as the next 
	do{
		int blockSize = array[next];
		//If the block is big enough, give out a chunk
		if (blockSize >= size) {
			//Tell the world that it is not free anymore 
			array[next] = -size;
			//If it fits perfectly just give it the block
			if (blockSize <= size + OVERHEADS){
				array[0] = array[next + 1];
			}
			//If its a bit bigger split the block in two
			else{
				array[0] = next + size + 1;
				array[next + size + 1] = blockSize - size;
				array[next + size + 2] = array[next + 1];
			}
			
			//The user is returned a reference to the data section
			return &array[next + 1];
		}
		//See if we can coalesce two free blocks
		
		//Go try the next free block
		else next = array[next + 1];	
	}while (next != start);

	//If that fails, consider some compacting! :/
}
		
