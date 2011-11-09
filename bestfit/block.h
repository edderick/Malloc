#include <stdio.h>

/**
 * Finds the smallest node that is large enough to fit the request in
 * @param array the array that is passed to the system
 * @param size The size requested
 * @param the best fitting node or NULL
 */
int * findBestFit(int *array, int size);

/**
 * Sets the blocks size to indicate free, returns it to the list and 
 * coalesces wit adjacent blocks if possible
 */
int freeBlock(int *block);

/**
 * @param block Pointer to a block
 * @return the size of that block
 */
int getBlockSize(int *block);

/**
 * @param block Pointer to a block
 * @return 1 if block is free 0 otherwise
 */
int getBlockIsFree(int *block);

/**
 * Coalesces two blocks into one
 * @param block pointer to a block
 * @param block pointer to annother block
 * return 1 if success, 0 if fail
 */
int coalesceBlocks(int *block1, int *block2);

/**
 * Coalesces block with all neighbours
 * @param block pointer to a block
 * return 1 on success, 0 on fail
 */
int coalesceWithNeighbours(int *block);
