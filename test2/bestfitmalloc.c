/*
This program was written by:
Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
Ben Clive bac2g10@ecs.soton.ac.uk

This test implements the best fit algorithm using a linked list :(

|_Pointer___|_Block 1___________________________________|_Block 2__________|
|_Next free_|_Size/free_|_Next Free ptr_|_Prev Free ptr_|_Size/free_|_Data_|

+ Array[0] contains pointer to the next free location after 
  the previously allocated location
+ Array[BlockNo] contains a positive size if free and a 
  negative size if allocated. This works nicely for checking
  that a free block is big enough. 
+ Array[BlockNo + 1] of a free block - contains a pointer to
  the next free block
+ Array[BlockNo + 1] is the pointer returned by my malloc

Note: Size does not include overheads

TODO: Remove magic 1, 2 and 3 relating to offsets of ptrs

*/
#include "bestfitmalloc.h"
#define MIN_ARRAY_SIZE 128
#define OVERHEADS 3
#define NEXT 1
#define PREV 2


int myinit(int *array, int size){

	//Array must be big enough to initalize
	if(size >= MIN_ARRAY_SIZE) {
		//The first block starts in position 1
		array[0] = 1;
		//The first entry of block 1 is it's size
		array[1] = size - 2;
		//Only the first block exists so send it back to the start
		array[2] = 1;
		array[3] = 1;
		return 1;
	}
	else return 0;
}
	
//Compaction doesn't work due to pointers!!!!!!!!
//
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
				//Set previous blocks pointer to next block
				if(array[next + 2] != next) array[array[next + 2]] = array[0];
			}
			//If its a bit bigger split the block in two
			else{
				array[0] = next + size + 1;
				
				array[next + size + 1] = blockSize - (size + 1);

				if (array[next + 1] == next) array[next + size + 2] = next + size + 1;
				else array[next + size + 2] = array[next + 1];
				
				if (array[next + 2] == next) array[next + size + 3] = next + size + 1;
				else array[next + size + 3] = array[next + 2];

				//Set previous blocks pointer to next block
				if(array[next + 2] != next) array[array[next + size + 3] + 2] = array[0];
	
			}

			//The user is returned a reference to the data section
			return &array[next + 1];
		}
	
	}while (next != start);

	//If that fails, consider some compacting! :/
	//Oh mate - You can't compact :(

	return 0;
}
		
int myfree( int *array, int *pointer){

	//pointer points to one place higher
	int currentNode = pointer - array - 1;

	//set size back to positive	
	array[currentNode] = -array[currentNode];

	//coalescesing should happen now
	int size = array[currentNode];
	while(array[size + currentNode + 1] > 0){
		if(array[0] == size + currentNode + 1) array[0] = currentNode;
	//Alright boys, lets boost the size
		size = size + array[size + currentNode + 1];
		//bridge pointers 
		//prev -> next
		array[array[size + 2]] = array[array[size + 1]];
		//next -> prev
		array[array[size + 1]] = array[array[size + 2]];
		
	}
	array[currentNode] = size;

	//start at 0 follow the free blocks to the first one that is larger
	int nextNode = array[0];
	int previousNode = 0;

	while ((array[nextNode] <= array[currentNode]) && (array[nextNode + 1] != nextNode)){
		previousNode = nextNode;
		nextNode = array[nextNode + 1];
	}

	//insert it at this point
	//prev -> curr and curr -> prev 
	if (previousNode == 0){ 
		array[0] = currentNode;
		array[currentNode + PREV] = currentNode;
	}
	else{
		array[previousNode + NEXT] = currentNode;
		array[currentNode + PREV] = previousNode;
	}
	//next -> curr and curr -> next
	if((nextNode = array[nextNode + NEXT]) && (array[nextNode] < array[currentNode])){
		array[currentNode + NEXT] = currentNode;
	}
	else{
		array[nextNode + PREV] = currentNode;
		array[currentNode + NEXT] = nextNode;
	}



	



	


}

