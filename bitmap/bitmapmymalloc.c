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
		array[1] = bitmap;
		array[0] = size;
		int number = array[2];
		for(int i=0; i<32; i++){
			if(i <= bitmap+1){
				number |= 0x1;
			}
			printf("%d", number&0x1);
			number = rotateRight(number, 1);	
		}
		array[2] = number;
		return 1;
		
	}
	else return 0;
}
	
//Compaction doesn't work due to pointers!!!!!!!!
//
int * mymalloc(int *array, int size) {

	//go through bitmap to find the biggest set of zeros closest to it.
	if(size <= 0){
		return 0;
	}
	if(size > array[0]){
		return 0;
	}
	int block=0;
	int blockNumber=0;
	int zerocounter=0;
	int currentBestSize=999999;
	int ourBlock=-1;
	int counter=0;
	while(counter < array[1]){
		int bits = array[counter+2];
		printf("\n++%d++\n", counter+2);
		for(int i=0; i<32; i++){
			printf("%d", bits&0x1);
			if(!(bits & 0x1)){
				//Found a 0
				if(zerocounter == 0) { 
					//record first 0.
					//printf("Found a zero at block %d\n", (counter*32)+i);
					block = (counter*32)+i ;
				}
				zerocounter++;
			}
			 if(bits & 0x1 || (counter==array[1]-1 && i==31)){
				//Found a 1 or end of array.
				if(zerocounter >= size+1 && zerocounter < currentBestSize){
					currentBestSize = zerocounter;
					ourBlock = block;
				}
				if(!zerocounter == 0){
					//printf("\nFinished a 0 stream, found %d\n", zerocounter);
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
	//printf("Our Block: %d, size of %d\n", ourBlock, size+1);
	array[ourBlock] = size+1;
	counter = ourBlock / 32 + 2;
	int startBit = (ourBlock % 32);
	//printf("Counter=%d, startBit=%d\n", counter, startBit); 
	int bitsToChange = size+1;
	while(bitsToChange > 0){
		int number = array[counter];
		for(int i=0; i<32; i++){
			if(i < startBit + bitsToChange && i >= startBit){
				number |= 0x1;
	//			printf("Edited %d in block %d\n", i, counter);
	//			printf("bitsToChange: %d, startBit: %d\n", bitsToChange, startBit);
				bitsToChange--;
				startBit++;
			}	
			number = rotateRight(number, 1);
		}
		array[counter] = number;
		if(bitsToChange > 0){
			counter++;
			//bitsToChange--;
			startBit = 0;	
		}
	}

	return array+ourBlock+1;
}
		
int myfree( int *array, int *pointer){
	//free stuff
	//check pointer
	printf("Free!");
	int counter = pointer - array;
	if(counter > array[0]){
		return 0;
	}
	
	//Start checking if it is a relevant pointer.
	
	int index = 1; //Initial array index
	int blockSize = array[index]; //size of bitmap
	while(blockSize+index+1 < array[0]){
		if(index == counter-1){
			break;
		} else {
			index += blockSize+1; //Set array index to current array index + size of block.
			blockSize = array[index]; //Set size
		}
		if(index + blockSize+1 >= array[0]){
			//Not a pointer!
			return 0;
		}
	}
	//move back 1 from user block
	counter -= 1;
	int bitmapbit = (counter % 32);
	int bitsToChange = array[counter];
	int bitmapblock = (counter / 32)+2;
	printf("\nNumber of bits to free: %d, from block: %d, starting at bit %d\n", array[counter], bitmapblock, bitmapbit);
	while(bitsToChange > 0){
		int bits = array[bitmapblock];
		printf("\nblock: %d, bit: %d\n", bitmapblock, bitmapbit);
		for(int i=0; i<32; i++){
			if(i >= bitmapbit && i < bitmapbit + bitsToChange){
				bits &= 0xFFFFFFFE;
				bitsToChange--;
				bitmapbit++;
			}
			bits = rotateRight(bits, 1);
			printf("%d",bits & 0x1);
		}
		array[bitmapblock] = bits;
		if(bitsToChange > 0){
			bitmapblock++;
			bitsToChange--;
			bitmapbit = 0;
		}
		
	}
}
