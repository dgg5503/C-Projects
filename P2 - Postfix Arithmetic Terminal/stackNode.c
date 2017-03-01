// File: stackNode.c 
// A stack node.
// @author Douglas Gliner <dgg5503>
// @date 3/10/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros

// Includes
#include <string.h>
#include "utils.h"
#include "stackNode.h"

/// Dynamically allocate a new stack node
/// @param data The data value
/// @param next Pointer to next node (NULL if none)
/// @return The new stack node
StackNode* makeStackNode(char* data, StackNode* next)
{
	// MALLOC a stack node
	StackNode* stackNodePtr = malloc(sizeof(StackNode));

	// Check if it was a success, otherwise throw error and return.
	if (stackNodePtr == NULL)
	{
		ERR("WARNING: Failed to create stack node, out of memory!\n");
		return NULL;
	}

	// set next to next.
	stackNodePtr->next = next;

	// memcpy the data, strlen + 1 for null char!
	char* dataCopyPtr = malloc((strlen(data) + 1) * sizeof(char));

	// make sure not null
	if (dataCopyPtr == NULL)
	{
		ERR("WARNING: Failed to create data copy ptr, out of memory!\n");
		return NULL;
	}

	// set data ptr and copy!
	stackNodePtr->data = dataCopyPtr;
	strcpy(stackNodePtr->data, data);

	return stackNodePtr;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
