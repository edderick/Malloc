/*
   This program was written by:
   Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
   Ben Clive bac2g10@ecs.soton.ac.uk
 
 */
#include "mymalloc.h"
#define MIN_ARRAY_SIZE 128
#define OVERHEADS 4
#include <stdio.h>

#include "block.h"
#include "list.h"


int myinit(int *array, int size){
	
	//check that the array is large enough
	if(size < MIN_ARRAY_SIZE){
		return 0;
	}

	//point head to first linked list entry
	array[0] = 2;

	//store total array size
	array[1] = size;

	//set up first linked list entry
	array[2] = size - 2; //1 Overhead for first linked list, 2 for each allocation
	array[size-1] = array[2]; //Tail size;
	array[3] = 0; //Next pointer
	array[4] = 0; //Tail pointer
	return 1;

}

int *mymalloc(int *array, int size){
	//I'm so scared :s
	//Convert from user size to backend size
	size = size + 2;
	
	//Ensure we can put linked list pointer back into this block
	if (size < OVERHEADS) size = OVERHEADS;

	int bestFitNode = findBestFit(array, size);

	//If there isn't space in the array
	if (bestFitNode == 0) return (int *) 0;
	
	//Must be big enough to fit pointers in the remainder
	if (getBlockSize(array, bestFitNode) >= size + OVERHEADS){
		//******** I have a slight problem with the cohesion here
		//I feel maybe a function split block would we nice
		//Possibly grouped together in a splitMemory()
		splitNode(array, bestFitNode, size);
	} 

	//UnFree the block and node -- A grouping as unfreeMemory() might be nice?
	setBlockFree(array, bestFitNode, 0);
	removeNode(array, bestFitNode);

	return &array[bestFitNode + 1];

}

int myfree( int *array, int *pointer){
	/* 1. -1 from pointer
	   2. Check pointer is valid
	   3. Set it to free
	   4. Insert it into the list of frees
	   5. Coalesce with neighbours
        */	
	int node = pointer - array;
	node--;	
	if(node < 0 || node >= getTotalArraySize(array)) return 0;
	if(isBlock(array, node) == 0) return 0;
	
	setBlockFree(array, node, 1);
	insertNode(array, node, getBlockSize(array, node));
	coalesceWithNeighbours(array, node);
	return 1;
}
