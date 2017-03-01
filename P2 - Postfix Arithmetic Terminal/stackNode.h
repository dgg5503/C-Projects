// File: $Id: stackNode.h,v 1.1 2013/10/13 04:49:51 csci243 Exp $
// Description: A stack node.
// Author: sps
#ifndef STACK_NODE_H
#define STACK_NODE_H

// Represents a single node in the stack
typedef struct StackNode {
	char* data;		// data associated with the node	
	struct StackNode* next;	// pointer to next node (NULL if none)
} StackNode;

/// Dynamically allocate a new stack node
/// @param data The data value
/// @param next Pointer to next node (NULL if none)
/// @return The new stack node
StackNode* makeStackNode(char* data, StackNode* next);

#endif

