#include <stdio.h>
#include "nextfreemalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);
	
	int *mem = mymalloc(myarray, 23);
	*mem = 5;
	
	for(int x = 0; x<10; x++){
		printf("%d,", myarray[x]);
	}
	
}
