#include <stdio.h>
#include "mymalloc.h"
#include "list.h"

int main(){
	//testNode();


	int myarray[512] = {0};
	int mysize = 512;

	int result = myinit(myarray, mysize);
	result = result;
	printf("Initializing... \n");
	printf("Mem Dump: \n");
	for(int x = 0; x<mysize; x++){
		printf("%d,", myarray[x]);
	}   
	mymalloc(myarray, 256);
	for(int x = 0; x<mysize; x++){
		printf("%d|", myarray[x]);
	}
/*
	int i = 0;
	int *mems[18];

	while (i < 17){
		i++;
		mems[i] = mymalloc(myarray, 1); 

		for(int j = 0; j < 1; j++){
			mems[i][j] = 0;
		}   

		printf("\nMem Dump: \n");
		for(int x = 0; x<128; x++){
			printf("%d,", myarray[x]);
		}   

	}   
	result = myfree(myarray, mems[1]);
	result = myfree(myarray, mems[2]);
	mems[0] = mymalloc(myarray, 5);
	printf("mems[0]: %li", mems[0]-myarray);
	if(mems[0] == 0) printf("Error!");
	printf("myfree: %d", result);
		printf("\nPost free Mem Dump: \n");
		for(int x = 0; x<128; x++){
			printf("%d,", myarray[x]);
		}
*	int *mem5 = mymalloc(myarray, 7); 

	mem5[6] = 20; 

	printf("%d", myarray - mem5); 

	//      for (i =0; i < 8; i++) mem[i] = 0;

	printf("\nmem dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}   

*/

}
