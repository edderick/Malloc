#include <stdio.h>

int getBlockSize(int *array, int block);

int isBlock(int *array, int block);

int setBlockSize(int *array, int block, int size, int free);

int getBlockIsFree(int *array, int block);

int coalesceBlocks(int *array, int block1, int block2);

int coalesceWithNeighbours(int *array, int block);

int getTotalArraySize(int *array);

int setBlockFree(int *array, int block, int free);
