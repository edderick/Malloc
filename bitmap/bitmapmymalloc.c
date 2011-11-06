/*
This program was written by:
Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
Ben Clive bac2g10@ecs.soton.ac.uk

Attempt to work the bitmap magic... First few blocks are checked with a bit for each (0 or 1 signifying taken).


TODO: Remove magic 1, 2 and 3 relating to offsets of ptrs

*/
#include "bitmapmymalloc.h"
#include <stdio.h> //For testing
#define MIN_ARRAY_SIZE 128
#define OVERHEADS 3
#define NEXT 1
#define PREV 2

static int rotateRight( int x, int shift){
	return ((unsigned int)x >> shift) | ((unsigned int)x << (32 - shift));
}
static int rotateLeft( int x, int shift){
	return ((unsigned int)x << shift) | ((unsigned int)x >> (32 - shift));
}
int myinit(int *array, int size){

	//Array must be big enough to initalize
	if(size >= MIN_ARRAY_SIZE) {
		//The first block starts in position 1
		//Number of blocks needed to store array...
		int bitmap = size / 32;
		array[0] = bitmap;
		int number = array[1];
		for(int i=0; i<32; i++){
			if(i <= bitmap){
				number |= 0x1;
			}
			printf("%d", number&0x1);
			number = rotateRight(number, 1);	
		}
		array[1] = number;
		return 1;
		
	}
	else return 0;
}
	
//Compaction doesn't work due to pointers!!!!!!!!
//
int * mymalloc(int *array, int size) {

	//go through bitmap to find the biggest set of zeros closest to it.
	if(size == 0 || size < 0){
		return 0;
	}
	int block=0;
	int blockNumber=0;
	int zerocounter=0;
	int currentBestSize=999999;
	int ourBlock=-1;
	int counter=0;
	while(counter < array[0]){
		int bits = array[counter+1];
		printf("\n++%d++\n", counter+1);
		for(int i=0; i<32; i++){
			printf("%d", bits&0x1);
			if(!(bits & 0x1)){
				//Found a 0
				if(zerocounter == 0) { 
					//record first 0.
					printf("Found a zero at block %d\n", (counter*32)+i);
					block = (counter*32)+i ;
				}
				zerocounter++;
			}
			 if(bits & 0x1 || (counter==array[0]-1 && i==31)){
				//Found a 1 or end of array.
				if(zerocounter >= size && zerocounter < currentBestSize){
					currentBestSize = zerocounter;
					ourBlock = block;
				}
				if(!zerocounter == 0){
					printf("\nFinished a 0 stream, found %d\n", zerocounter);
					zerocounter = 0;
				}
			}
			bits = rotateRight(bits, 1);
		}
		counter++;
	}
	if(ourBlock == -1){
		return 0;
	}
	//Finished looking through bitmap;
	//We have our pointer!
	printf("Our Block: %d\n", ourBlock);
	counter = ourBlock / 32 + 1;
	int startBit = ourBlock % 32;
	printf("Counter =%d, startBit=%d\n", counter, startBit); 
	int number = array[counter];
	for(int i=1; i<=32; i++){
		number = rotateRight(number, 1);
		if(i < startBit + size && i >= startBit){
			number |= 0x1;
			printf("Edited %d in block %d\n", i, counter);
		}	
	}
	array[counter] = number;
	return array+ourBlock;
}
		
int myfree( int *array, int *pointer){

	//pointer points to one place higher
	int currentNode = pointer - array - 1;

	//set size back to positive	
	array[currentNode] = -array[currentNode];

	//coalescesing should happen now
	int size = array[currentNode];
	while(array[size + currentNode + 1] > 0){
		if(array[0] == size + currentNode + 1) array[0] = currentNode;
	//Alright boys, lets boost the size
		size = size + array[size + currentNode + 1];
		//bridge pointers 
		//prev -> next
		array[array[size + 2]] = array[array[size + 1]];
		//next -> prev
		array[array[size + 1]] = array[array[size + 2]];
		
	}
	array[currentNode] = size;

	//start at 0 follow the free blocks to the first one that is larger
	int nextNode = array[0];
	int previousNode = 0;

	while ((array[nextNode] <= array[currentNode]) && (array[nextNode + 1] != nextNode)){
		previousNode = nextNode;
		nextNode = array[nextNode + 1];
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
	if((nextNode = array[nextNode + NEXT]) && (array[nextNode] < array[currentNode])){
		array[currentNode + NEXT] = currentNode;
	}
	else{
		array[nextNode + PREV] = currentNode;
		array[currentNode + NEXT] = nextNode;
	}
}

