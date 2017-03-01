// Fie: linkedList.c
// Basic linked list that will end up being turned into an array.
//
// @author Douglas Gliner <dgg5503>
// @date 04/25/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project3}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#define _LL_IMPL_

// Includes
#include <stdio.h>
#include <stdlib.h>

// node struct
typedef struct NodeStruct {
	struct NodeStruct* next;
	void* data;
} NodeStruct, *LLNode;

#include "linkedList.h"
#include "utils.h"

/// Creates a node to be used for the linked list.
/// @param dataToAdd pointer to data to add
/// @param next the next node in the linked list
/// @return node pointer to the node in the list
static LLNode makeNode(void* dataToAdd, LLNode next)
{
	// allocate space for node
	LLNode llNode = malloc(sizeof(NodeStruct));
	if (llNode == NULL)
	{
		ERR("ERROR: Failed to create node, out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	// set data
	llNode->data = dataToAdd;
	llNode->next = next;

	// return note
	return llNode;
}

/// Add data to the provided Linked List. This data will be placed at the
/// head of the list.
/// @param linkedList the list to add data to
/// @param dataToAdd void ptr of data to add.
void addNode(LLNode* linkedList, void* dataToAdd)
{
	// if start is null, init the list.
	if (*linkedList == NULL)
		*linkedList = makeNode(dataToAdd, NULL);
	else
		*linkedList = makeNode(dataToAdd, *linkedList);
}

/// Provides the linked list in array form for easy access of elements.
/// @param linkedList the list to turn into an array
/// @param length the amount of data in the linked list to return in the array.
/// @return a void* array of data found in this linked list
void* convertToArray(LLNode* linkedList, unsigned int length)
{
	// null check
	if (linkedList == NULL)
	{
		ERR("ERROR: Provided linked list is NULL!\n");
		return NULL;
	}

	// allocate array
	void** voidArray = malloc(sizeof(void*) * length);
	if (voidArray == NULL)
	{
		ERR("ERROR: Failed to allocate linked list array!\n");
		return NULL;
	}

	// get data pointer, set it to an array element
	// and then free the node!
	for (unsigned int i = 0; i < length && (*linkedList) != NULL; ++i)
	{
		LLNode tmpNode = *linkedList;
		*(voidArray + i) = (*linkedList)->data;
		*linkedList = (*linkedList)->next;
		FREENULL(tmpNode);
	}

	// set linked list to null, free it and return the void array
	return voidArray;
}

/// Deallocs the nodes found in this list. Does NOT dealloc the data ptrs.
/// @param linkedList the list to destroy
void destroyList(LLNode* linkedList)
{
	// NULL check
	if (*linkedList == NULL)
		return;

	// store tmp node and free the current node
	LLNode nextTmpNode = (*linkedList)->next;
	FREENULL(*linkedList);
	destroyList(&nextTmpNode);
}


// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
