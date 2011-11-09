#include <stdio.h>

/**
 * Inserts a node into the free linked list
 * @param head Pointer to the head of the list
 * @param start Pointer to the start of the node
 * @param size size of the node
 * @return 1 if succsessful 0 if fails  
 */
int insertNode(int *head, int *node, int size);

/**
 * Removes a node from the free linked list
 * @param start Pointer to the start of the node
 * @return 1 if sucsessful 0 if fail
 */
int removeNode(int *node);

/**
 * Follows the linked list to the next node
 * @return the next node in the list or NULL
 */
int * getNextNode(int *node);

/**
 * Finds the smallest node that is large enough to fit the request in
 * @param head Pointer to the head of the list
 * @param size The size requested
 * @param the best fitting node or NULL
 */
int * findBestFit(int *head, int size);

/**
 * Splits one node into two smaller nodes
 * @param 
 */
int *splitNode(int *node, int size);


