#include <stdio.h>
#include "bitmapmymalloc.h"

int main(){
	int myarray[512] = {0};
	int mysize = 512;

	int result = myinit(myarray, mysize);

	printf("Initializing... \n");
	for(int i = 0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}	
	printf("\n");
	int *mem;
	for(int i=0; i<1;i++){
		mem =  mymalloc(myarray, 240);
		myfree(myarray, mem);
		mem = mymalloc(myarray, 480);
		myfree(myarray, mem);
	}
	
	for(int i=0; i<480; i++){
		*(mem+i)=1;
	}
	for(int i=0; i<mysize; i++){
		printf("%d,", myarray[i]);
	}
}
