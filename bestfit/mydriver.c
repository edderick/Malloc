#include <stdio.h>
#include "mymalloc.h"
#include "list.h"

int main(){
	//testNode();


	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);

	printf("Initializing... \n");
	printf("Mem Dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}   

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

/*	int *mem5 = mymalloc(myarray, 7); 

	mem5[6] = 20; 

	printf("%d", myarray - mem5); 

	//      for (i =0; i < 8; i++) mem[i] = 0;

	printf("\nmem dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}   

*/

}
