#include "mymalloc.h"

int myinit(int *array, int size){
	array[0] = size;
	if(size > 127) return 1;
	else return 0;
}
