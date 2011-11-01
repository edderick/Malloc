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

static void init(int *start, int size) {
	*start = 0; /* 0 for taken */
	int *next = start + size + 3; /* 3 for overheads */
	int *last = next - 1;
	*last = size;
}

int * mymalloc(int *array, int size) {
	/*We have an array, I wonder what it is...*/
	/* Find a suitable piece of free space... */
	int *start = array + 4; /* Use position 3 of array for testing */
	init( start, size );
	return start+2;
}
		

