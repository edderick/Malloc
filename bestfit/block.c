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

int isBlock(int *array, int block){
	if(array[block] == array[block + array[block] - 1]){
		return 1;
	}
	else return 0;
}

/**
 * @param block Pointer to a block
 * @param size The new USER size of the block
 */
int setBlockSize(int *array, int block, int size, int free){
	if (free > 0) {
		array[block] = size; 
		array[block + size - 1] = size;
	}
	else {
		array[block] = -size;
		array[block + size - 1] = -size;
	}
}

/**
 * @param block Pointer to a block
 * @return 1 if block is free 0 otherwise
 */
int getBlockIsFree(int *array, int block){
	if (array[block] >= 0) return 1;
	else return 0;
}

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

int coalesceWithNeighbours(int *array, int block){
	//check left and right.
	int blockSize = getBlockSize(array, block);
	int nextBlock = block + blockSize;
	int lastBlock = block - array[block-1];
	//HackedyHax
	if(nextBlock < getTotalArraySize(array)){
		if(getBlockIsFree(array, nextBlock)){
			coalesceBlocks(array, block, nextBlock);
		}
	}
	//Nasty magic number is for array header
	if(lastBlock > 1){
		if(getBlockIsFree(array, lastBlock)){
			coalesceBlocks(array, block, lastBlock);
		}
	}
}

/**
 * @return the size of array passed into myinit
 */
int getTotalArraySize(int *array){
	return array[1];
}
