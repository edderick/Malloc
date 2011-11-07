#include <stdio.h>
#include "bestfitmalloc.h"

int main(){
	int myarray[128] = {0};
	int mysize = 128;

	int result = myinit(myarray, mysize);

	printf("Initializing... \n");
	printf("Mem Dump: \n");
	for(int x = 0; x<30; x++){
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
	
	int *mem5 = mymalloc(myarray, 7);

	mem5[6] = 20;
	
	printf("%d", myarray - mem5); 

//	for (i =0; i < 8; i++) mem[i] = 0;

	printf("\nmem dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}




	printf("\n \n reverse \n \n");

	for (i = 1; i < 5; i++){
		myfree(myarray, mems[i]);

		printf("\nMem Dump: \n");
		for(int x = 0; x<128; x++){
			printf("%d,", myarray[x]);
		}
		
	}

	
	int *mem = mymalloc(myarray, 7);

	printf("%d", mem - myarray); 

	for (i =0; i < 7; i++) mem[i] = 0;

	printf("\nmem dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}

	
		

	int *mem2 = mymalloc(myarray, 27);

	printf("%d", (int) mem2); 

	//for (i =0; i < 27; i++) mem2[i] = 0;



	printf("\nmem dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}

	//int *mem3 = mymalloc(myarray, 27);

	//printf("\n Should be zero: %d",(int) mem3 ); 

	//myfree(myarray, mem2);

	printf("\nmem dump: \n");
	for(int x = 0; x<128; x++){
		printf("%d,", myarray[x]);
	}



/*	printf("\nMallocating 4 blocks, setting [0] = 5... \n");
	int *mem1 = mymalloc(myarray, 4);
	*mem1 = 5;
	
	printf("Dump of allocated mem1: ");
	printf("%d", mem1[0]);


	printf("\nMem Dump: \n");
	for(int x = 0; x<30; x++){
		printf("%d,", myarray[x]);
	}
	

	printf("\nFreeing those 4 blocks...");
	myfree(myarray, mem1);

	printf("\nMem Dump: \n");
	for(int x = 0; x<30; x++){
		printf("%d,", myarray[x]);
	}
	
	printf("\nAllocating annother 4 blocks, setting [0] = 7... \n");
	int *mem2 = mymalloc(myarray, 4);
	*mem2 = 7;

	printf("Dump of allocated mem2: ");
	printf("%d", mem2[0]);

	printf("\nMem Dump: \n");
	for(int x = 0; x<30; x++){
		printf("%d,", myarray[x]);
	}
	
	printf("\nAllocating annother 6 blocks, setting [0] = 8... \n");
	int *mem3 = mymalloc(myarray, 6);
	*mem3 = 8;

	printf("Dump of allocated mem3: ");
	printf("%d", mem3[0]);

	printf("\nMem Dump: \n");
	for(int x = 0; x<30; x++){
		printf("%d,", myarray[x]);
	}

	printf("\nFreeing those 6 blocks...");
	myfree(myarray, mem3);

	printf("\nMem Dump: \n");
	for(int x = 0; x<30; x++){
		printf("%d,", myarray[x]);
	}
	
	printf("\n");

*/


}
