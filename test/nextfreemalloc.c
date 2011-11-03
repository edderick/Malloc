/*
This program was written by:
Edward Seabrook - ejfs1g10@ecs.soton.ac.uk
Ben Clive bac2g10@ecs.soton.ac.uk
*/
#include "nextfreemalloc.h"

int myinit(int *array, int size){
	array[0] = size;

	if(size > 127) return 1;
	else return 0;
}
	

int * mymalloc(int *array, int size) {


}
		

