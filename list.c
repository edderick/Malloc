#include "list.h"
#include "block.h"
#define NEXT 1
#define PREV 2
////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//IMPORTANT
	//Start of list has a prev pointer to 0
	//End of list has a next pointer to 0
	//Next time, be consistant.
	//Size is the total size, user size is size -2
	//This is for easy block access (and I've coded that way)

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*Get the last item in the list, because its needed for inserting at the end of the list*/

int getPreviousEnd(int *array, int size){
	int currentNode = getHead(array);
	int previousNode = getPreviousNode(array, currentNode);
	while((currentNode != 0) && (getBlockSize(array, currentNode) <= size)){
		previousNode = currentNode;
		currentNode = getNextNode(array, currentNode);
	}
	return previousNode;
}

/**
 * Inserts a node into the free linked list
 * Node is inserted to maintain smallest first
 * @param array Pointer to the head of the list
 * @param start Pointer to the start of the node
 * @param size size of the node
 * @return 1 if succsessful 0 if fails  
 */

int insertNode(int *array, int node, int size){
	//Find best fit returns the smallest node that is bigger
	int nextNode = findBestFit(array, size);
	//for a node at the end, previous should be 260.
	int previousNode = getPreviousNode(array, nextNode);

	if (nextNode == 0){ //|| nextNode == node){ //Needed if combining block
		//At the end of the list	
		previousNode = getPreviousEnd(array, size);
		setNextNode(array, node, 0);
	}else{
		//In the middle (or first) so set the next pointers
		setPreviousNode(array, nextNode, node);
		setNextNode(array, node, nextNode);
	}	

	if (previousNode == 0) {
		//At the start of the list	
		setHead(array, node);
		setPreviousNode(array, node, 0);
	}else{
		setNextNode(array, previousNode, node);
		setPreviousNode(array, node, previousNode);
	}

	return 1;
}


/**
 * Removes a node from the free linked list
 * @param start Pointer to the start of the node
 * @return 1 if sucsessful 0 if fail
 */
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
 * @return the next node in the list or 0
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
 * @return the next node in the list or 0
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
 * @param node The node to change the pointer of
 * @param nextNode The node to point to
 * @return 1 if successful, 0 if fail
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
 * @param node The node to change the pointer of
 * @param nextNode The node to point to
 * @return 1 if successful, 0 if fail
 */
int setPreviousNode(int *array, int node, int previousNode){;
	if(node == 0){
		return 0;
	}
	array[node + PREV] = previousNode;
	return 1;
}


/**
 * @return the index of the first element of the list
 */
int getHead(int *array){
	return array[0];
}


/**
 * @param node The new head
 */
int setHead(int *array, int node){
	array[0] = node;
	return 1;
}


/**
 * Finds the smallest node that is large enough to fit the request in
 * @param head Pointer to the head of the list
 * @param size The size requested
 * @return the best fitting node or 0 if won't fit.
 */
int findBestFit(int *array, int size){
	int currentNode = getHead(array);
	while((currentNode != 0) && (getBlockSize(array, currentNode) < size)){
		currentNode = getNextNode(array, currentNode);
	}
	return currentNode;
}


/**
 * Splits one node into two smaller nodes
 * The the node reffered to in the parameters will have size size
 * @param node The node to be split
 * @param size The size the first node will be
 * @return 1 if success 0 if fail
 */
int splitNode(int *array, int node, int size){
	int currentNodeSize = getBlockSize(array, node);
	int newNode = node + size;
	
	//Make sure it's actually big enough
	if (size > currentNodeSize) return 0;

	//Split the block up
	setBlockSize(array, newNode, currentNodeSize - size, 1);
	setBlockSize(array, node, size, 1);
	
	//Juggle the linked list
	removeNode(array, node);
	insertNode(array, newNode, currentNodeSize - size);
	insertNode(array, node, size);
	
	return 1;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//HERE ENDS REAL CODE :)
//????????????????????????????????????????????????????????????

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
	return 1;
}


