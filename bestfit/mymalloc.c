/*
   This program was written by:
   Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
   Ben Clive bac2g10@ecs.soton.ac.uk
 
 */
#include "mymalloc.h"
#define MIN_ARRAY_SIZE 128
#include <stdio.h>

#include "block.h"
#include "list.h"


int myinit(int *array, int size){
	
	//check that the array is large enough
	if(size < MIN_ARRAY_SIZE){
		return 0;
	}

	//point head to first linked list entry
	array[0] = 1;

	//set up first linked list entry
	array[1] = size - 3; //1 Overhead for first linked list, 2 for each allocation
	array[size-1] = size - 3; //Tail size;
	array[2] = 1; //Next pointer
	array[3] = 1; //Tail pointer
	return 1;

}

int *mymalloc(int *array, int size){
	
}

int myfree( int *array, int *pointer){
	
}
