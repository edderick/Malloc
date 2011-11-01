/*
This program was written by:
Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
Ben Clive bac2g10@ecs.soton.ac.uk
*/
#include "mymalloc.h"

int myinit(int *array, int size){
	array[0] = size;

	if(size > 127) return 1;
	else return 0;
}

static int getSize(int *size){
	return (int) (*size & 0x7FFFFFFF);
}
	

int * mymalloc(int *array, int size) {
	/*We have an array, I wonder what it is...*/

	/* Find a suitable piece of free space here */

	int *start = array + 3; /* Use position 3 of array for testing */
	*start = (size | 0x80000000);
	return start+1;
}
		

