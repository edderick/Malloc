#include "list.h"
#include "block.h"
#define NEXT 1
#define PREV 2
////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//IMPORTANT
	//Start of list has a prev pointer to 0
	//End of list has a next pointer to 0
	//Next time, be consistant.
	
	//Mmmmmm.. consistency


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
		//At the start of the list	
		setHead(array, node);
		setPreviousNode(array, node, 0);
	}else{
		setNextNode(array, previousNode, node);
		setPreviousNode(array, node, previousNode);
	}
	if (nextNode == 0){ //|| nextNode == node){ //Needed if combining blocks
		//At the end of the list
		setNextNode(array, node, 0);
	}else{
		//In the middle (or first) so set the next pointers
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
	if(isBlock(array, node) == 0){
		return 0; //Not a node!
	}
	int nextNode = getNextNode(array, node);
	int prevNode = getPreviousNode(array, node);
	setNextNode(array, prevNode, nextNode);
	setPreviousNode(array, nextNode, prevNode);
	
	if(node == getHead(array)){
		//Node is the first in the list
		setHead(array, nextNode);
	}
	return 1;
}
	 
/**
 * Follows the linked list to the next node
 * @return the next node in the list or NULL
 */
int getNextNode(int *array, int node){
	//if it's the head pointer
	if (node == 0) return array[0];

	int nextNode = array[node + NEXT];
	if (nextNode != node) return nextNode;
	else return 0;
}


/**
 * Follows the linked list to the previous node
 * @return the next node in the list or NULL
 */
int getPreviousNode(int *array, int node){
	//if it's the head pointer
	if (node == 0) return 0;
	
	int previousNode = array[node + PREV];
	if (previousNode != node) return previousNode;
	else return 0;
}

/**
 * Sets the next node pointer to the given value
 * @return the next node in the list or NULL
 */
int setNextNode(int *array, int node, int nextNode){
	if(node == 0){
		return 0;
	}
	array[node + NEXT] = nextNode;
	return 1;
}

/**
 * Sets the previous node pointer to the given value
 * @return the next node in the list or NULL
 */
int setPreviousNode(int *array, int node, int previousNode){;
	if(node == 0){
		return 0;
	}
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
	while((currentNode != 0) && (getBlockSize(array, currentNode) < size)){
		currentNode = getNextNode(array, currentNode);
	}

	return currentNode;

}

int splitNode(int *array, int node, int size){
	int currentNodeSize = getBlockSize(array, node);
	int newNode = node + size;
	setBlockSize(array, newNode, currentNodeSize - size,1 );
	setBlockSize(array, node, size, 1);
	removeNode(array, node);
	insertNode(array, newNode, currentNodeSize - size);
	insertNode(array, node, size);
	
	return 1;
}

int testNode(){
	
	//Test getNextNode 
	int array[11] = {0};
	array[1] = 5;
	array[5] = 1;
	array[7] = 6;

	printf("Testing getNextNode\n1) Expected value: 1\n");
	printf("Actual value: %d\n", array[getNextNode(array, 0)]);
	printf("2) Expected value: 0 \n");
	printf("Actual value: %d\n", array[getNextNode(array, 6)]);
	
	//Test findBestFit
	printf("\n\n");
	int array2[11] = {0};
	array2[0] = 7; //First free
	array2[1] = 11;
	array2[2] = 5; //Size of block
	array2[3] = 0; //Next free
	array2[4] = 7; //Prev free
	array2[5] = 88; //free block
	array2[6] = 5; //Size of block
	array2[7] = 4; //Size of block
	array2[8] = 2; //Next free
	array2[9] = 0; //Prev free
	array2[10] = 4; //Size of block
	
	printf("Testing findBestFit\n1) Expected Value: 2\n");
	printf("Actual value: %d\n", findBestFit(array2, 5));
	printf("2) Expected Value: 7\n");
	printf("Actual value: %d\n", findBestFit(array2, 4));
	
	//Test removeNode
	printf("\n\n");
	printf("Testing removeNode\n1) Expected Value: 1\n");
	printf("Actual Value: %d\n", removeNode(array2, 7));
	printf("Expected: 2|11|5|0|0|88|5|4|2|0|4|\n");
	printf("Actual:   ");
	for(int x = 0; x < 11; x++){
		printf("%d|", array2[x]);
	}	

	//Test insertNode
	printf("\n\n");
	printf("Testing insertNode\n1) Expected value: 1\n");
	printf("Actual Value: %d\n", insertNode(array2, 7, 4));
	printf("Expected: 7|11|5|0|7|88|5|4|2|0|4|\n");
	printf("Actual:   ");
	for(int x = 0; x < 11; x++){
		printf("%d|", array2[x]);
	} 
	printf("\n");
	
	//Test splitNode
	int array3[11]= {0};
	array3[0] = 2;
	array3[1] = 11;
	array3[2] = 9;
	array3[3] = 0;
	array3[4] = 0;
	array3[10] = 9;
	printf("\n\n");
	printf("Testing splitNode\n1) Expected: 2|11|4|6|0|4|5|0|2|0|5|\nActual:      ");
	splitNode(array3, 2, 4);
	for(int x=0; x<11;x++){
		printf("%d|", array3[x]);
	}
	printf("\n");
	
	printf("\n\n");
	//Test coalesce
	printf("Testing coalesceBlocks\nExpected: 2|11|9|0|0|4|5|0|2|0|9|\nActual:   ");
	coalesceBlocks(array3, 2, 6);
	for(int x=0; x<11;x++){
		printf("%d|", array3[x]);
	}
	printf("\n");
	
	printf("\n\n");
	//Test coalesce with neighbours
	printf("Testing coalesce with neighbours\nExpected: 2|11|9|0|0|88|5|4|0|0|9|\nActual:   ");
	coalesceWithNeighbours(array2, 7);
	for(int x=0; x<11; x++){
		printf("%d|", array2[x]);
	}
	printf("\n");
}


