/*
   This program was written by:
   Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
   Ben Clive -  bac2g10@ecs.soton.ac.uk
	
   This implementation of malloc uses a best fit allocation algorithm.
   A linked list maintained in order of decreasing size allows us to 
   quickly allocate the best fit - If the block is too big, then we split
   it into two smaller blocks, one the required size, and one the 
   remainder.
   Upon freeing the blocks, they are coalesced with any of their 
   neighbours if they are also free. This is done by making use of sizes
   (+ve for free, -ve for allocated) at each end of the block. 
   Our approach aims to be very modular (ie Abstract) and very readable.

*/
#include "mymalloc.h"
#define MIN_ARRAY_SIZE 6
#define OVERHEADS 4
#define NEXT 1
#define PREV 2
#include <pthread.h>
pthread_mutex_t mutex;


static int getPreviousEnd(int *array, int size);
static int insertNode(int *array, int node, int size);
static int removeNode(int *array, int node);
static int getNextNode(int *array, int node);
static int getPreviousNode(int *array, int node);
static int setNextNode(int *array, int node, int nextNode);
static int setPreviousNode(int *array, int node, int previousNode);
static int findBestFit(int *array, int size);
static int splitNode(int *array, int node, int size);
static int getHead(int *array);
static int setHead(int *array, int head);

static int getBlockSize(int *array, int block);
static int isBlock(int *array, int block);
static int setBlockSize(int *array, int block, int size, int free);
static int getBlockIsFree(int *array, int block);
static int coalesceBlocks(int *array, int block1, int block2);
static int coalesceWithNeighbours(int *array, int block);
static int getTotalArraySize(int *array);
static int setBlockFree(int *array, int block, int free);

/**
 * Sets up the array for use with mymalloc() and myfree()
 * @param array An array from which to allocate memory
 * @param size The size of the given array
 * @return 1 on success, 0 on fail 
 */
int myinit(int *array, int size){
	//check that the array is large enough
	if(size < MIN_ARRAY_SIZE){
		return 0;
	}
	//point head to first linked list entry
	array[0] = 2;
	//store total array size
	array[1] = size;
	//Set up first linked list entry
	//1 Overhead for first linked list, 2 for each allocation
	array[2] = size - 2;
	//Tail size
	array[size - 1] = array[2]; 
	//Next Pointer
	array[3] = 0;
	//Previous pointer
	array[4] = 0;
	return 1;
}

/**
 * Allocates a chunk of memory of the requested size
 * @param array A initialized array to allocate memory from
 * @param size The size of block to be allocated
 * @return a pointer to the allocated memory
 */
int *mymalloc(int *array, int size){
	//Convert from user size to backend size
	size = size + 2;
	//Ensure we can put linked list pointer back into this block
	if (size < OVERHEADS) size = OVERHEADS;
	int bestFitNode = findBestFit(array, size);
	//If there isn't space in the array
	if (bestFitNode == 0) return (int *) 0;
	//Must be big enough to fit pointers in the remainder
	if (getBlockSize(array, bestFitNode) >= size + OVERHEADS){
		splitNode(array, bestFitNode, size);
	} 
	//UnFree the block and node 
	setBlockFree(array, bestFitNode, 0);
	removeNode(array, bestFitNode);
	//Return a pointer compensating for boundary tags
	return &array[bestFitNode + 1];
}


/**
 * Returns a previously allocated block of memory to 
 * the pool of memory that can be allocated from 
 * @param array The initialized array containing the memory to free
 * @param block A pointer to the block to free
 * @return 1 on success, 0 on fail
 */
int myfree( int *array, int *block){
	//Calculate node index
	int node = block - array - 1;
	//Ensure node is in bounds
	if(node < 0 || node >= getTotalArraySize(array)) return 0;
	if(isBlock(array, node) == 0) return 0;
	//Free the block
	setBlockFree(array, node, 1);
	insertNode(array, node, getBlockSize(array, node));
	coalesceWithNeighbours(array, node);
	return 1;
}


/**
 * Ensures an array has no allocated blocks
 * @param array The array to check
 * @return 1 on success, 0 on fail 
 */
int mydispose(int *array){
	//Head points to first node
	//And first node fills the whole array
	if(array[0] == 2 && array[2] == array[1]-2){
		return 1;
	}
	return 0;
}


/**
 * A thread safe version of myinit()
 */
int myinit_mt(int *array, int size){
	pthread_mutex_lock(&mutex);
	int result = myinit(array, size);
	pthread_mutex_unlock(&mutex);
	return result;
}

/**
 * A thread safe version of mymalloc()
 */
int * mymalloc_mt(int *array, int size){
	pthread_mutex_lock(&mutex);
	int * result = mymalloc(array, size);
	pthread_mutex_unlock(&mutex);
	return result;
}

/**
 * A thread safe version of myfree
 */
int myfree_mt(int *array, int * block){
	pthread_mutex_lock(&mutex);
	int result = myfree(array, block);
	pthread_mutex_unlock(&mutex);
	return result;
}

/**
 * A thread safe version of mydispose()
 */
int mydispose_mt(int *array){
	pthread_mutex_lock(&mutex);
	int result = mydispose(array);
	pthread_mutex_unlock(&mutex);
	return result;
}


/*Get the last item in the list, because its needed for inserting at the end of the list*/


/**
 * Gets the last item of the linked list
 * @param array The array being malloced from
 * @param size The size of the block
 * @return The index of the last item
 */
static int getPreviousEnd(int *array, int size){
	int currentNode = getHead(array);
	int previousNode = getPreviousNode(array, currentNode);
	while((currentNode != 0) && (getBlockSize(array, currentNode) <= size)){
		previousNode = currentNode;
		currentNode = getNextNode(array, currentNode);
	}
	return previousNode;
}


/**
 * Inserts a node into the free linked list
 * Node is inserted to maintain smallest first
 * @param array The array being malloced from
 * @param node The index of the node to insert
 * @param size Size of the node
 * @return 1 if succsessful 0 if fails  
 */
static int insertNode(int *array, int node, int size){
	//Find best fit returns the smallest node that is bigger
	int nextNode = findBestFit(array, size);
	//for a node at the end, previous should be 260.
	int previousNode = getPreviousNode(array, nextNode);

	if (nextNode == 0){ 
		//At the end of the list	
		previousNode = getPreviousEnd(array, size);
		setNextNode(array, node, 0);
	}else{
		//In the middle (or first) so set the next pointers
		setPreviousNode(array, nextNode, node);
		setNextNode(array, node, nextNode);
	}	

	if (previousNode == 0) {
		//At the start of the list	
		setHead(array, node);
		setPreviousNode(array, node, 0);
	}else{
		setNextNode(array, previousNode, node);
		setPreviousNode(array, node, previousNode);
	}

	return 1;
}


/**
 * Removes a node from the free linked list
 * @param array The array being malloced from
 * @param node The node to remove
 * @return 1 if sucsessful 0 if fail
 */
static int removeNode(int *array, int node){
	//Ensure the node is block 
	if(isBlock(array, node) == 0) return 0; 
	
	//Bridge pointers
	int nextNode = getNextNode(array, node);
	int prevNode = getPreviousNode(array, node);
	setNextNode(array, prevNode, nextNode);
	setPreviousNode(array, nextNode, prevNode);
	
	//Node is the first in the list
	if(node == getHead(array)){
		setHead(array, nextNode);
	}
	return 1;
}


/**
 * Follows the linked list to the next node
 * @param array The array being malloced from
 * @param node The current node
 * @return the next node in the list or 0
 */
static int getNextNode(int *array, int node){
	//if it's the head pointer
	if (node == 0) return array[0];

	int nextNode = array[node + NEXT];
	if (nextNode != node) return nextNode;
	else return 0;
}


/**
 * Follows the linked list to the previous node
 * @param array The array being malloced from
 * @param node The current Node
 * @return the next node in the list or 0
 */
static int getPreviousNode(int *array, int node){
	//if it's the head pointer
	if (node == 0) return 0;
	
	int previousNode = array[node + PREV];
	if (previousNode != node) return previousNode;
	else return 0;
}


/**
 * Sets the next node pointer to the given value
 * @param array The array being malloced from
 * @param node The node to change the pointer of
 * @param nextNode The node to point to
 * @return 1 if successful, 0 if fail
 */
static int setNextNode(int *array, int node, int nextNode){
	//Ensure it is not the head pointer or size
	if(node <= 1){
		return 0;
	}
	array[node + NEXT] = nextNode;
	return 1;
}


/**
 * Sets the previous node pointer to the given value 
 * @param array The array being malloced from
 * @param node The node to change the pointer of
 * @param nextNode The node to point to
 * @return 1 if successful, 0 if fail
 */
static int setPreviousNode(int *array, int node, int previousNode){;
	//Ensure it is not the head pointer or size
	if(node <= 1){
		return 0;
	}
	array[node + PREV] = previousNode;
	return 1;
}


/**
 * @param array The array being malloced from
 * @return the index of the first element of the list
 */
static int getHead(int *array){
	return array[0];
}


/**
 * @param array The array being malloced from
 * @param node The new head
 */
static int setHead(int *array, int node){
	array[0] = node;
	return 1;
}


/**
 * Finds the smallest node that is large enough to fit the request in
 * @param array The array being malloced from
 * @param head Pointer to the head of the list
 * @param size The size requested
 * @return the best fitting node or 0 if won't fit.
 */
static int findBestFit(int *array, int size){
	int currentNode = getHead(array);
	while((currentNode != 0) && (getBlockSize(array, currentNode) < size)){
		currentNode = getNextNode(array, currentNode);
	}
	return currentNode;
}


/**
 * Splits one node into two smaller nodes
 * The the node reffered to in the parameters will have size size
 * @param array The array being malloced from
 * @param node The node to be split
 * @param size The size the first node will be
 * @return 1 if success 0 if fail
 */
static int splitNode(int *array, int node, int size){
	int currentNodeSize = getBlockSize(array, node);
	int newNode = node + size;
	
	//Make sure it's actually big enough
	if (size > currentNodeSize) return 0;

	//Split the block up
	setBlockSize(array, newNode, currentNodeSize - size, 1);
	setBlockSize(array, node, size, 1);
	
	//Juggle the linked list
	removeNode(array, node);
	insertNode(array, newNode, currentNodeSize - size);
	insertNode(array, node, size);
	
	return 1;
}


/**
 * @param array The array being malloced from
 * @param block Pointer to a block
 * @return the size of that block
 */
static int getBlockSize(int *array, int block){
	if (array[block] >= 0) return array[block];
	else return -array[block];
}


/**
 * Ensures the boundary tags are consistent
 * @param array The array being malloced from
 * @param block The block to check 
 * @return 1 if the boundary tags match, 0 if they do not
 */
static int isBlock(int *array, int block){
	if(array[block] == array[block + getBlockSize(array, block) - 1]){
		return 1;
	}
	else return 0;
}


/**
 * @param array The array being malloced from
 * @param block Pointer to a block
 * @param size The new USER size of the block
 * @return 1 if successful, else 0
 */
static int setBlockSize(int *array, int block, int size, int free){
	//overheads
	if (size < 4) return 0;
	
	if (free > 0) {
		array[block] = size; 
		array[block + size - 1] = size;
	}
	else {
		array[block] = -size;
		array[block + size - 1] = -size;
	}

	return 1;
}

/**
 * Sets the free status of a block based on parameter
 * @param array The array being malloced from
 * @param block The block to set free
 * @param free whether to free it or not
 * @return status
 */
static int setBlockFree(int *array, int block, int free){
		setBlockSize(array, block, getBlockSize(array, block), free);
		return 1;
}


/**
 * @param array The array being malloced from
 * @param block Index of a block
 * @return 1 if block is free 0 otherwise
 */
static int getBlockIsFree(int *array, int block){
	if (array[block] >= 0) return 1;
	else return 0;
}


/**
 * Coalesces two blocks into one
 * @param array The array being malloced from
 * @param block pointer to a block
 * @param block pointer to annother block
 * return 1 if success, 0 if fail
 */
static int coalesceBlocks(int *array, int block1, int block2){
	//Ensure they are both blocks
	if(!(isBlock(array, block1) && isBlock(array, block2))){
		return 0;
	}

	int block1Size = getBlockSize(array, block1);
	int block2Size = getBlockSize(array, block2);
	int node;
	//Sort out ordering
	if(block1 < block2) node = block1;
	else node = block2;
	//Remove the old blocks...
	removeNode(array, block2);
	removeNode(array, block1);
	setBlockSize(array, node, block1Size + block2Size, 1);
	insertNode(array, node, block1Size + block2Size);
	return 1;
} 


/**
 * Coalesces block with all neighbours
 * @param array The array being malloced from
 * @param block pointer to a block
 * @return new block index on success, 0 on fail
 */
static int coalesceWithNeighbours(int *array, int block){
	//Check left and right.
	int blockSize = getBlockSize(array, block);
	int nextBlock = block + blockSize;
	int lastBlock = block - getBlockSize(array, block - 1);
	//Ensure it doesn't try going over the end
	if(nextBlock < getTotalArraySize(array)){
		if(getBlockIsFree(array, nextBlock)){
			coalesceBlocks(array, block, nextBlock);
		}
	}
	//Ensure it doesn't try going over the beginning
	if(lastBlock > 1){
		if(getBlockIsFree(array, lastBlock)){
			coalesceBlocks(array, block, lastBlock);
			return lastBlock;
		}
	}
	return block;
}


/**
 * @param array The array being malloced from
 * @return the size of array passed into myinit
 */
static int getTotalArraySize(int *array){
	return array[1];
}
