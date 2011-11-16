#include "block.h"
#include "list.h"

/**
 * @param block Pointer to a block
 * @return the size of that block
 */
int getBlockSize(int *array, int block){
	if (array[block] >= 0) return array[block];
	else return -array[block];
}


/**
 * Ensures the boundary tags are consistent
 * @param block The block to check 
 * @return 1 if the boundary tags match, 0 if they do not
 */
int isBlock(int *array, int block){
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
int setBlockSize(int *array, int block, int size, int free){
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
int setBlockFree(int *array, int block, int free){
		setBlockSize(array, block, getBlockSize(array, block), free);
		return 1;
}


/**
 * @param block Index of a block
 * @return 1 if block is free 0 otherwise
 */
int getBlockIsFree(int *array, int block){
	if (array[block] >= 0) return 1;
	else return 0;
}


/**
 * Coalesces two blocks into one
 * @param block pointer to a block
 * @param block pointer to annother block
 * return 1 if success, 0 if fail
 */
int coalesceBlocks(int *array, int block1, int block2){
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
int coalesceWithNeighbours(int *array, int block){
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
int getTotalArraySize(int *array){
	return array[1];
}
