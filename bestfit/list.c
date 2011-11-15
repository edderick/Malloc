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

int findBestFit(int *array, int size){
	//We want to find the best fit, iterate over frees and find smallest
	/** 1. Get First Node
	 *  2. Store it's size and index
	 *  3. Move to next node
	 *  4. Compare size of nodes
	 *  5. If node > size but smaller than stored size, store this node instead.
	 *  6. Continue until end of list
	 *  7. Return node index
	 */
	int firstNode = array[0];
	int currentNodeIndex = array[0];
	int currentBestNode = array[0];
	int currentBestSize = array[currentNodeIndex];
	int nextNodeIndex = 0;
	if(currentBestSize < size){
		currentBestSize = -1; //Marker to indicate that we didn't find any space
	}

	do{
		nextNodeIndex = getNextNode(array, currentNodeIndex);
		if(nextNodeIndex == 0){
			break;
		}
		if(array[nextNodeIndex] >= size && (array[nextNodeIndex] < currentBestSize || currentBestSize == -1)){
			//Better fit, replace items
			currentBestNode = nextNodeIndex;
			currentBestSize = array[nextNodeIndex];
		}
		currentNodeIndex = nextNodeIndex;
	} while(currentNodeIndex != firstNode);
	
	//Should have best node!
	if(currentBestSize > -1) return currentBestNode;
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
	
	//Test findBestFit
	printf("\n\n");
	int array2[10] = {0};
	array2[0] = 1;
	array2[1] = 3;
	array2[2] = 6;
	array2[4] = 3;
	array2[5] = 300000;
	array2[6] = 4;
	array2[7] = 1;
	array2[9] = 4;
	
	printf("Testing findBestFit\n1) Expected Value: 1\n");
	printf("Actual value: %d\n", findBestFit(array2, 2));
	printf("2) Expected Value: 6\n");
	printf("Actual value: %d\n", findBestFit(array2, 4));
}


