#include "list.h"
#include "block.h"
#define NEXT 1
#define PREV 2

/** Ben's implementation that inserts in blockbased order
int insertNode(int *array, int node, int size){
	**
	 * 1. Find the node to insert after
	 * 2. Set our size up
	 * 3. Set our next using preceding's next.
	 * 4. Set the preceding node's next to point to new
	 * 5. Set following node's prev to us.
	 *
	int firstNode = array[0];
	int currentNodeIndex = array[0];
	array[node] = size;
	array[node+size -1] = size;

	while(array[currentNodeIndex + NEXT] < node){
		if(array[currentNodeIndex + NEXT]  == firstNode){
			//Been around linked list, we must be the last block.
			break; //Current node will be the last in the list
			
		}
			
		currentNodeIndex = getNextNode(array, currentNodeIndex);		
	}
	//Should have our node...
	array[node + NEXT] = array[currentNodeIndex + NEXT];
	array[node + PREV] = currentNodeIndex;
	array[currentNodeIndex + NEXT] = node;
	array[ array[node + NEXT] + PREV ] = node;
	return 1;
}
*/

//Edwards Implementation that inserts in the order of block size
//Node that block stuff should be done using block functions (size etc)
int insertNode(int *array, int node, int size){

	//Find best fit returns the smallest node that is bigger
	int nextNode = findBestFit(array, size);
	int previousNode = getPreviousNode(array, nextNode);
	
	
	if (previousNode == 0) {
		setHead(array, node);
	}else{
		setNextNode(array, previousNode, node);
		setPreviousNode(array, node, previousNode);
	}

	if (nextNode == 0){
		setNextNode(array, node, node);
	}else{
		setPreviousNode(array, nextNode, node);
		setNextNode(array, node, nextNode);
	}	

	return 1;

}

int removeNode(int *array, int node){
	/**
	 * 1. Update previous's next pointer, following's previous pointer
	 * 2. Return success.
	 */
	if(getEndSize(array,node) != getStartSize(array, node)){
		return 0; //Not a node!
	}
	int nextNode = getNextNode(array, node);
	int prevNode = getPreviousNode(array, node);
	setNextNode(array, prevNode, nextNode);
	setPreviousNode(array, nextNode, prevNode);
	return 1;
}
	 
/**
 * Follows the linked list to the next node
 * @return the next node in the list or NULL
 */
int getNextNode(int *array, int node){
	int nextNode = array[node + NEXT];
	if (nextNode != node) return nextNode;
	else return 0;
}


/**
 * Follows the linked list to the previous node
 * @return the next node in the list or NULL
 */
int getPreviousNode(int *array, int node){
	int previousNode = array[node + PREV];
	if (previousNode != node) return previousNode;
	else return 0;
}

/**
 * Sets the next node pointer to the given value
 * @return the next node in the list or NULL
 */
int setNextNode(int *array, int node, int nextNode){
	array[node + NEXT] = nextNode;
	return 1;
}

/**
 * Sets the previous node pointer to the given value
 * @return the next node in the list or NULL
 */
int setPreviousNode(int *array, int node, int previousNode){;
	array[node + PREV] = previousNode;
	return 1;
}

int getHead(int *array){
	return array[0];
}

int setHead(int *array, int node){
	array[0] = node;
	return 1;
}

/* Ben's Implementation assuming the list is not sorted.
int findBestFit(int *array, int size){
	//We want to find the best fit, iterate over frees and find smallest
	** 1. Get First Node
	 *  2. Store it's size and index
	 *  3. Move to next node
	 *  4. Compare size of nodes
	 *  5. If node > size but smaller than stored size, store this node instead.
	 *  6. Continue until end of list
	 *  7. Return node index
	 *
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
*/

//Edwards Implementation assuming the list is sorted
int findBestFit(int *array, int size){
	int currentNode = getHead(array);
	
	while((currentNode != 0) && (getBlockSize(&currentNode) < size)){
		currentNode = getNextNode(array, currentNode);
	}

	return currentNode;

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
	array2[0] = 1; //First free
	array2[1] = 4; //Size of block
	array2[2] = 6; //Next free
	array2[3] = 6; //Prev free
	array2[4] = 4; //Size of block
	array2[5] = 88; //Empty
	array2[6] = 4; //Size of block
	array2[7] = 1; //Next free
	array2[8] = 1; //Prev free
	array2[9] = 4; //Size of block
	
	printf("Testing findBestFit\n1) Expected Value: 1\n");
	printf("Actual value: %d\n", findBestFit(array2, 2));
	printf("2) Expected Value: 6\n");
	printf("Actual value: %d\n", findBestFit(array2, 4));
	
	//Test removeNode
	printf("\n\n");
	printf("Testing removeNode\n1) Expected Value: 1\n");
	printf("Actual Value: %d\n", removeNode(array2, 6));
	printf("Dumping array...\n");
	for(int x = 0; x < 10; x++){
		printf("%d|", array2[x]);
	}	

	//Test insertNode
	printf("\n\n");
	printf("Testing insertNode\n1) Expected value: 1\n");
	printf("Actual Value: %d\n", insertNode(array2, 6, 4));
	printf("1|4|6|6|4|88|4|1|1|4|\n");
	for(int x = 0; x < 10; x++){
		printf("%d|", array2[x]);
	} 
	printf("\n");
	
}


