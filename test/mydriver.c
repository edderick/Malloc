#include <stdio.h>
#include "nextfreemalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);

	printf("Testing Init: ");
	
	for(int x = 0; x<10; x++){
		printf("%d,", myarray[x]);
	}

	int *mem = mymalloc(myarray, 4);
	*mem = 5;
	
	printf("\nTesting mymalloc: ");

	for(int x = 0; x<10; x++){
		printf("%d,", mem[x]);
	}

	printf("\nDumping array: ");

	for(int x = 0; x<10; x++){
		printf("%d,", myarray[x]);
	}
	
	printf("\n");

}
