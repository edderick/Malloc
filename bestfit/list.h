#include <stdio.h>

int insertNode(int *array, int node, int size);

int removeNode(int *array, int node);

int getNextNode(int *array, int node);

int getPreviousNode(int *array, int node);

int getStartSize(int *array, int node);

int getEndSize(int *array, int node);

int setNextNode(int *array, int node, int nextNode);

int setPreviousNode(int *array, int node, int previousNode);

int findBestFit(int *array, int size);

int splitNode(int *array, int node, int size);

int getHead(int *array);

int setHead(int *array, int head);

int testNode();
