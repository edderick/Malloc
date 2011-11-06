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
	*mem = 5;	
	for(int i=0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}
	printf("\n");
}
