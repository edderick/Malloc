#include <stdio.h>

/**
 * Inserts a node into the free linked list
 * @param array Pointer to the head of the list
 * @param start Pointer to the start of the node
 * @param size size of the node
 * @return 1 if succsessful 0 if fails  
 */
int insertNode(int *array, int node, int size);

/**
 * Removes a node from the free linked list
 * @param start Pointer to the start of the node
 * @return 1 if sucsessful 0 if fail
 */
int removeNode(int *array, int node);

/**
 * Follows the linked list to the next node
 * @return the next node in the list or 0 if end of list
 */
int getNextNode(int *array, int node);

/**
 * Follows the linked list to the previous node
 * @return the next node in the list or 0 if head
 */
int getPreviousNode(int *array, int node);

/**
 * Get the first size 
 */
int getStartSize(int *array, int node);
/**
 * Get the size at the end
 */
int getEndSize(int *array, int node);
/**
 * Sets the next node pointer to the given value
 * @return 1 if successful
 */
int setNextNode(int *array, int node, int nextNode);

/**
 * Sets the previous node pointer to the given value
 * @return 1 if suucessful
 */
int setPreviousNode(int *array, int node, int previousNode);

/**
 * Finds the smallest node that is large enough to fit the request in
 * @param head Pointer to the head of the list
 * @param size The size requested
 * @param the best fitting node or 0 if won't fit.
 */
int findBestFit(int *array, int size);

/**
 * Splits one node into two smaller nodes
 * @param 
 */
int splitNode(int *array, int node, int size);

/**
 * @return the index of the first element of the list
 */
int getHead(int *array);

/**
 * @param node The new head
 */
int setHead(int *array, int head);

int testNode();
