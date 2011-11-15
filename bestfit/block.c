#include "block.h"

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

