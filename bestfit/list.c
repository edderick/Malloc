#include "list.h"
#define NEXT 1

/**
 * Follows the linked list to the next node
 * @return the next node in the list or NULL
 */
int getNextNode(int *array, int node){
	int nextNode = array[node + NEXT];
	if (nextNode != node) return nextNode;
	else return 0;
}

int testNode(){
	
	//Test getNextNode 
	int array[10] = {0};
	array[1] = 5;
	array[5] = 1;
	array[7] = 6;

	printf("Testing getNextNode\n1) Expected value: 1\n");
	printf("Actual value: %d\n", array[getNextNode(array, 0)]);
	printf("2) Expected value: 0 \n");
	printf("Actual value: %d\n", array[getNextNode(array, 6)]);
}


