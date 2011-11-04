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

	for(int x = 0; x<1; x++){
		printf("%d,", mem[x]);
	}

	printf("\nDumping array: ");

	for(int x = 0; x<10; x++){
		printf("%d,", myarray[x]);
	}
	
	printf("\n");

	int *mem2 = mymalloc(myarray, 4);
	*mem2 = 5;
	
	printf("\nTesting mymalloc: ");

	for(int x = 0; x<1; x++){
		printf("%d,", mem2[x]);
	}

	printf("\nDumping array: ");

	for(int x = 0; x<20; x++){
		printf("%d,", myarray[x]);
	}
	
	printf("\n");




}
