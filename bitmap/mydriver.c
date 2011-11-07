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
	int *mem;
	for(int i=0; i<1;i++){
		mem = mymalloc(myarray, 10);
		if(myfree(myarray, mem) == 0){
			printf("\nNot an allocated pointer!\n");
			break;
		} else {
			for(int i=0; i<10; i++){
				*(mem+i)=1;
			}
		}
	}
	
	for(int i=0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}
}
