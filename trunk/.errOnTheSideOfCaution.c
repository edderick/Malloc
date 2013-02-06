// Ben's implementation that inserts in blockbased order
int insertNode(int *array, int node, int size){
	/**
	 * 1. Find the node to insert after
	 * 2. Set our size up
	 * 3. Set our next using preceding's next.
	 * 4. Set the preceding node's next to point to new
	 * 5. Set following node's prev to us.
	 */
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

//Ben's Implementation assuming the list is not sorted.
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


