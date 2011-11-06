/*
This program was written by:
Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
Ben Clive bac2g10@ecs.soton.ac.uk
*/
#include "mymalloc.h"

int *freePtr;
int myinit(int *array, int size){
	array[0] = size;

	if(size > 127) return 1;

	/*set up original free pointer, and size of free space */
	freePtr = array + 1;
	*freePtr = size-1; //Size of free space
	*freePtr = size+1; //Pointer to outside of array
	return 0;
}

static int getSize(int *size){
	/* Pass in a int + taken bit pair and it returns the size. */
	return (int) (*size & 0x7FFFFFFF);
}

static void setTaken(int size, int *ptr){
	*ptr = (*ptr - (2 * *ptr)); //Flip negativity
}
	
static int* findNext(int *array, int size){
	/*Next fit algorithm*/
	int currentPlace = 0;
	while(1){
		if(*freePtr > size){
			int *space = freePtr; //Put at start of free space
			if(*space == (size + 1)){
				//Find new!!
			} else {
				//Set up the nexzt free...
				freePtr = freePtr + size;
				*freePtr = *space - size - 1;
				*(freePtr + 1) = *(space + 1) - size - 1;
			}
			//Initial value should be the size, and taken.
			*space = (size - 2*size); //Negative
		} else {
			currentPlace = *(freePtr+1);
			if(currentPlace > *array){
				//No more free space (points above array)
				return '\0';
			}
		}
	}
}

int * mymalloc(int *array, int size) {
	/*We have an array, I wonder what it is...*/

	/* Find a suitable piece of free space here */

	int *space = findNext(array, size); /* Use Next fit */
	return space+1;
}
		

