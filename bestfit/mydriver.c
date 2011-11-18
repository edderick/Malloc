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
	printf("%d", mydispose(myarray));
	int myarray2[512] = {0};
	myinit(myarray2, mysize);
	printf("\n%d",mydispose(myarray2));

}
