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
#include <stdio.h>


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

	if (size == 0) return (int *) 0;
	if (array[0] == 0) return (int *) 0;

	int next = array[0];
	int last = next;

	//make sure the block can be free'd
	if (size < 3) size = 3;

	//as long as the next 
	do{
		int blockSize = array[next];
		//If the block is big enough, give out a chunk
		if (blockSize >= size) {
			//Tell the world that it is not free anymore 
			array[next] = -size;
			//If it fits perfectly just give it the block
			if (blockSize <= size + 1){
				//		if (array[next + 1] != next) array[0] = array[next + 1];
				//		else array[0] = 0;
				//		//Set previous blocks pointer to next block
				//		if(array[next + 2] != next) array[array[next + 2] + 1] = array[0];



				int coalescePtr = next;
				int previousPtr = next + PREV;
				int nextPtr =  next +  NEXT;

				if(array[nextPtr] == array[previousPtr]){
					array[0] = 0;
				}
				else{
					if(array[0] == coalescePtr) array[0] = array[nextPtr];
					//bridge pointers 
					//prev -> next
					if (array[nextPtr] ==  coalescePtr) {
						array[array[previousPtr] + NEXT] = array[previousPtr];
					}else{
						array[array[previousPtr] + NEXT] = array[nextPtr];
					}
					//next -> prev
					if (array[previousPtr] ==  coalescePtr) {
						array[0] = array[nextPtr];
						array[array[nextPtr] + PREV] = array[nextPtr];
					}else{
						array[array[nextPtr] + PREV] = array[previousPtr];
					}
				}



			}
			//If its a bit bigger split the block in two
			else{
				

				int nextBlock = next + size + 1;
				int nextPtr = array[next + NEXT];
				int prevPtr = array[next + PREV];
				array[nextBlock] = blockSize - (size + 1);



				if (prevPtr == next){ 
					array[0] = nextBlock;
					array[nextBlock + PREV] = nextBlock;
				}
				else{ 
					array[prevPtr + NEXT] = nextBlock;
					array[nextBlock + PREV] = prevPtr;
				}

				if (nextPtr == next){
					array[nextBlock + NEXT] = nextBlock;
				}
				else{
					array[nextBlock + NEXT] = nextPtr;
					array[nextPtr + PREV] = nextBlock; 
				}

			}

			//The user is returned a reference to the data section
			return &array[next + 1];
		}

		last = next;
		next = array[next + 1];	

	}while (next != last);
	//If that fails, consider some compacting! :/
	//Oh mate - You can't compact :(

	return (int *) 0;
}

int myfree( int *array, int *pointer){


	//pointer points to one place higher
	int currentNode = pointer - array - 1;

	printf("CurrentNode: %d",currentNode);
	fflush(stdout);


	if(array[currentNode] > 0) return 0;
	else{
		

		//set size back to positive	
		array[currentNode] = -array[currentNode];

		//coalescesing should happen now
		int size = array[currentNode];
						
/*
		while(array[size + currentNode + 1] > 0){


			int coalescePtr = currentNode + size + 1;
			int previousPtr = currentNode + size + PREV + 1;
			int nextPtr = currentNode + size + NEXT + 1;

			if(array[0] == coalescePtr) array[0] = array[nextPtr];
			//bridge pointers 
			//prev -> next
			if (array[nextPtr] ==  coalescePtr) {
				array[array[previousPtr] + NEXT] = array[previousPtr];
			}else{
				array[array[previousPtr] + NEXT] = array[nextPtr];
			}
			//next -> prev
			if (array[previousPtr] ==  coalescePtr) {
				if (array[nextPtr] != coalescePtr) array[0] = array[nextPtr];
				else array[0] = 0;
				array[array[nextPtr] + PREV] = array[nextPtr];
			}else{
				array[array[nextPtr] + PREV] = array[previousPtr];
			}
			//Alright boys, lets boost the size
			size = size + array[size + currentNode + 1] + 1;	
		}
		array[currentNode] = size;
*/

		//start at 0 follow the free blocks to the first one that is larger
		int nextNode =  array[0];
		int previousNode = 0;

		while ((array[nextNode] <= array[currentNode]) && (previousNode != nextNode)){
			previousNode = nextNode;
			nextNode = array[nextNode + NEXT];
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
		if (previousNode == nextNode){
			array[currentNode + PREV] = currentNode;
			array[currentNode + NEXT] = currentNode;
		}else{
			array[nextNode + PREV] = currentNode;
			array[currentNode + NEXT] = nextNode;
		}







		return 1;

	}

}

