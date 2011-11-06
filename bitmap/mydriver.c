#include <stdio.h>
#include "bitmapmymalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);

	printf("Initializing... \n");
	for(int i = 0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}	
	printf("\n");
	int* mem =  mymalloc(myarray, 3);
	if(mem == 0){
		printf("No space found!\n");
		for(int i=0; i<mysize; i++){
			printf("%d,", myarray[i]);
		}
		printf("\n");
		return 0;
	}
	*mem = 30;	
	*(mem+3) = -30;
	int *mem2 = mymalloc(myarray, 6);
	*mem2 = 88;
	int *mem3 = mymalloc(myarray, 35);
	for(int i=0; i<35; i++){
		*(mem3+i) = i;
	}
	for(int i=0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}
	mymalloc(myarray, 20);
	myfree(myarray, mem);
	int *mem4 = mymalloc(myarray, 2);
	*mem4 = 333;
	printf("\n");
}
