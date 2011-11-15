#include "block.h"

/**
 * @param block Pointer to a block
 * @return the size of that block
 */
int getBlockSize(int *block){
	if (*block >= 0) return *block;
	else return -(*block);
}

/**
 * @param block Pointer to a block
 * @param size The new USER size of the block
 */
int setBlockSize(int *block, int size, int free){
	if (free > 0) {
		*block = size; 
		*(block + size + 1) = size;
	}
	else {
		*block = -size;
		*(block + size + 1) = -size;
	}
}

/**
 * @param block Pointer to a block
 * @return 1 if block is free 0 otherwise
 */
int getBlockIsFree(int *block){
	if (*block >= 0) return 1;
	else return 0;
}

