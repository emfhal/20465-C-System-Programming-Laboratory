#ifndef NODES_LIST_H
#define NODES_LIST_H

#include <stdlib.h>
#include <string.h>

/* All the functions that we need for a linked list for the Assembler machine code lines dynamic */
typedef struct _node
{
	char* name;
	void* value;
	struct _node* next;
} Node;

typedef struct _nodesList
{
	Node* head;
	Node* tail;
	Node* current;
	int elementSize;
	int length;
} NodesList;

NodesList* createNodesList(int elementSize);
Node* createNode(char* name, void* value, int elementSize);
void addNode(NodesList* nodesList, char* name, void* value);
Node* getNext(NodesList* nodesList);
Node* searchNode(NodesList* nodesList, char* name);
void freeNodesList(NodesList* nodesList);

#endif