#include <stdio.h>
#include "mymalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);
	
	int *mem = mymalloc(myarray, 3);
	int *mem2 = mymalloc(myarray, 4);
	*mem = 5;
	*mem2 = 6;
	
	for(int x = 0; x<10; x++){
		printf("%d,", myarray[x]);
	}
	
}
