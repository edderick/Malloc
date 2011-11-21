/*
   This program was written by:
   Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
   Ben Clive bac2g10@ecs.soton.ac.uk
 
 */
#include "mymalloc.h"
#define MIN_ARRAY_SIZE 128
#define OVERHEADS 4
#include <stdio.h>
#define NEXT 1
#define PREV 2

//**Prototypes**

static int getPreviousEnd(int *array, int size);
static int insertNode(int *array, int node, int size);
static int removeNode(int *array, int node);
static int getNextNode(int *array, int node);
static int getPreviousNode(int *array, int node);
//static int getStartSize(int *array, int node);
//static int getEndSize(int *array, int node);
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

//**End Prototypes**

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

int mydispose(int *array){
	/* 1. Head pointer is 0
	   2. Array size = size of block onei + 2
	   3. Size of block 1 is consistent
	*/
	printf("array[0]: %d, array[2]: %d ", array[0], array[2]);
	if(array[0] == 2 && array[2] == array[1]-2){
			return 1;
	}
	
	
	return 0;

}

/*Get the last item in the list, because its needed for inserting at the end of the list*/

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
 * @param array Pointer to the head of the list
 * @param start Pointer to the start of the node
 * @param size size of the node
 * @return 1 if succsessful 0 if fails  
 */

static int insertNode(int *array, int node, int size){
	//Find best fit returns the smallest node that is bigger
	int nextNode = findBestFit(array, size);
	//for a node at the end, previous should be 260.
	int previousNode = getPreviousNode(array, nextNode);

	if (nextNode == 0){ //|| nextNode == node){ //Needed if combining block
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
 * @param start Pointer to the start of the node
 * @return 1 if sucsessful 0 if fail
 */
static int removeNode(int *array, int node){
	/**
	 * 1. Update previous's next pointer, following's previous pointer
	 * 2. Return success.
	 */
	if(isBlock(array, node) == 0){
		return 0; //Not a node!
	}
	int nextNode = getNextNode(array, node);
	int prevNode = getPreviousNode(array, node);
	setNextNode(array, prevNode, nextNode);
	setPreviousNode(array, nextNode, prevNode);
	
	if(node == getHead(array)){
		//Node is the first in the list
		setHead(array, nextNode);
	}
	return 1;
}
	 
/**
 * Follows the linked list to the next node
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
 * @param node The node to change the pointer of
 * @param nextNode The node to point to
 * @return 1 if successful, 0 if fail
 */
static int setNextNode(int *array, int node, int nextNode){
	if(node == 0){
		return 0;
	}
	array[node + NEXT] = nextNode;
	return 1;
}


/**
 * Sets the previous node pointer to the given value 
 * @param node The node to change the pointer of
 * @param nextNode The node to point to
 * @return 1 if successful, 0 if fail
 */
static int setPreviousNode(int *array, int node, int previousNode){;
	if(node == 0){
		return 0;
	}
	array[node + PREV] = previousNode;
	return 1;
}


/**
 * @return the index of the first element of the list
 */
static int getHead(int *array){
	return array[0];
}


/**
 * @param node The new head
 */
static int setHead(int *array, int node){
	array[0] = node;
	return 1;
}


/**
 * Finds the smallest node that is large enough to fit the request in
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
 * @param block Pointer to a block
 * @return the size of that block
 */
static int getBlockSize(int *array, int block){
	if (array[block] >= 0) return array[block];
	else return -array[block];
}


/**
 * Ensures the boundary tags are consistent
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
 * @param block The block to set free
 * @param free whether to free it or not
 * @return status
 */
static int setBlockFree(int *array, int block, int free){
		setBlockSize(array, block, getBlockSize(array, block), free);
		return 1;
}


/**
 * @param block Index of a block
 * @return 1 if block is free 0 otherwise
 */
static int getBlockIsFree(int *array, int block){
	if (array[block] >= 0) return 1;
	else return 0;
}


/**
 * Coalesces two blocks into one
 * @param block pointer to a block
 * @param block pointer to annother block
 * return 1 if success, 0 if fail
 */
static int coalesceBlocks(int *array, int block1, int block2){
	if(!(isBlock(array, block1) && isBlock(array, block2))){
		return 0; //Not blocks
	}

	int block1Size = getBlockSize(array, block1);
	int block2Size = getBlockSize(array, block2);
	int node;
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
 * @param block pointer to a block
 * @return new block index on success, 0 on fail
 */
static int coalesceWithNeighbours(int *array, int block){
	//check left and right.
	int blockSize = getBlockSize(array, block);
	int nextBlock = block + blockSize;
	int lastBlock = block - getBlockSize(array, block - 1);
	//Ensure it doesn't try going over the end
	if(nextBlock < getTotalArraySize(array)){
		if(getBlockIsFree(array, nextBlock)){
			coalesceBlocks(array, block, nextBlock);
		}
	}
	//Nasty magic number is for array header
	if(lastBlock > 1){
		if(getBlockIsFree(array, lastBlock)){
			coalesceBlocks(array, block, lastBlock);
			return lastBlock;
		}
	}
	return block;
}


/**
 * @return the size of array passed into myinit
 */
static int getTotalArraySize(int *array){
	return array[1];
}
