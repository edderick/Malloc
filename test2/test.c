/* Basic heap correctness test harness.
 * tjeh 02/12/2008, based on test harness by dan.
 */
#include <stdlib.h>
#include <stdio.h>
#include "bestfitmalloc.h"
#define SIZE (1000000)
#define PSIZE (SIZE/50)
//#define SEED (<secret value>)  /* You are not told this value */
#define SEED (2)
static int block[SIZE];
static int * pointers[PSIZE];
static int sizes[PSIZE];
static int valid_pointers=0;
static int count = 0;
int main() {
	
	srand(SEED);
	if (myinit(block, SIZE)==0) exit(EXIT_FAILURE);
	while(1) {
		if (valid_pointers==0 || ((valid_pointers < PSIZE) && ((rand()&7) <5))) {
			/* Allocate a new block */
			int size = rand() % (1<<(rand()%12)) + 1;
			if (size >0) {
				int * fp = mymalloc(block, size);
				printf("%d", (int) fp);
				if (fp != (int*)0) {
					/* TJEH 02/12 - Keep track of block size */
					sizes[valid_pointers] = size;

					pointers[valid_pointers++] = fp;
					count++;

					/* TJEH 02/12 - Fill the new block with data - awkward negative numbers */
					for (int i = 0; i < size; i++) fp[i] = -(i + 42);
				} else {
					printf("Successful mymallocs = %d\n",count);
					exit(EXIT_SUCCESS);
				}
			} 
		} else {
			/* free a block  */
			int fp = rand()%valid_pointers;

			/* TJEH 02/12 - Check the block still contains the data we put there */
			for (int i = 0; i < sizes[fp]; i++) {
				if (pointers[fp][i] != -(i + 42)) {
					printf("!!!! Heap corrupted !!!!\n");
					exit(EXIT_FAILURE);
				}
			}

			if(myfree(block, pointers[fp])==0) exit(EXIT_FAILURE);
			while (++fp < valid_pointers) {
				pointers[fp-1] = pointers[fp];

				/* TJEH 02/12 - Also move the size information */
				sizes[fp-1] = sizes[fp];
			}
			valid_pointers--;
		}
	}
}
