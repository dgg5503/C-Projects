// File: stack.c 
// Interface to a node based stack implementation.
// For this project, the tokens (C strings) from the 
// postfix expression are pushed onto the stack.
// @author Douglas Gliner <dgg5503>
// @date 3/10/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros

// Includes
#include "utils.h"
#include "stack.h"

/// Add an element to the top of the stack (stack is changed).
/// This routine should dynamically allocate a new node.
/// @param stack Points to the top of the stack
/// @param data The token (C String)
void push(StackNode** stack, char* data)
{
	// if start is NULL, init the stack.
	if (*stack == NULL)
		*stack = makeStackNode(data, NULL);
	else
		*stack = makeStackNode(data, *stack);
}

/// Return the top element from the stack (stack is unchanged)
/// @param stack Points to the top of the stack
/// @return the top element on the stack
/// @exception If the stack is empty, the program should 
///	exit with EXIT_FAILURE
char* top(StackNode* stack)
{
	// If stack is empty (stack is null) exit condition
	if (stack == NULL)
		exit(EXIT_FAILURE);

	return stack->data;
}

/// Removes the top element from the stack (stack is changed).
/// This routine should free the memory associated with the top node.
/// @param stack Points to the top of the stack
/// @exception If the stack is empty, the program should 
///	exit with EXIT_FAILURE
void pop(StackNode** stack)
{
	// stack is NULL, cant pop! Exit.
	if (*stack == NULL)
		exit(EXIT_FAILURE);

	// store head ptr as a tmp ptr.
	StackNode* tmpPtr = *stack;

	// free and set data ptr to null
	FREENULL(tmpPtr->data);

	// set top of stack to the next thing
	*stack = (*stack)->next;
	
	// free and set tmpPtr to null
	FREENULL(tmpPtr);
}

/// Tells whether the stack is empty or not (stack is unchanged)
/// @param stack Points to the top of the stack
/// @return 0 if not empty, any other value otherwise
int emptyStack(StackNode* stack)
{
	// see if stack is null....
	if (stack == NULL)
		return 1;
	return 0;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
