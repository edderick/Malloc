#include <stdio.h>
#include "mymalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);
	
	int *mem = mymalloc(myarray, 10);
	*mem = 5;
	
	printf("%d\nMem is %d, Array is %d\n", result, *mem, myarray[4]);
	for(int x = 0; x<10; x++){
		printf("%d", myarray[x]);
	}
	
}
