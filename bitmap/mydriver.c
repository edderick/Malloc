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
	int *mem2;
	for(int i=0; i<1;i++){
		mem = mymalloc(myarray, 10);
		mem2 = mymalloc(myarray, 40);
		printf("%d", myarray[2]);
		if(mem != 0){
			for(int i=0; i<10; i++){
				*(mem+i)=1;
			}
		}
		if(myfree(myarray, mem) == 0){
			printf("Not a pointer!");
		}
		if(myfree(myarray, mem2) == 0){
			printf("Not a pointer2!");
		}
	}
	printf("%d\n", myarray[3]);
	for(int i=0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}
}
