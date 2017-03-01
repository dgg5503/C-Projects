#ifndef FILE1_H
#define FILE1_H

#include <stdio.h>	
#include <stdlib.h>
#include <assert.h>

// Stack
// - Stack is FIFO (first in first out)
// - Stack is PUSH / POP

typedef struct Data {
	// itll hold a string ptr
	char* data;

	// itll call a function with the follow params
	// return int, args are double double
	int(*funFunc)(double, double);

	// also hold an anon union!
	union {
		int iVal;
		double dVal;
	} Number; // remember that a union will alloc memory for the largest of the two types.
	
} Data;

/*
	This struct is PACKED since size of data is in descending order! Memory alignment
	will look good and no bytes will be lost.
*/
typedef struct StackNode {

	// ptr to data, we'll need to alloc this else where manually and assign it!
	// 8 bytes
	Data* data;

	// the next thing on the stack!
	// 8 bytes
	struct StackNode* next; // NOTE: MUST EXPLICITLY SAY WE WANT A STRUCT PTR!

	// a regular integer
	// 4 bytes
	int regularInt;
} StackNode; // To finish a typedef struct, be sure to add the name at the end!

// to typedef
// typedef, thing you want to alias, the alias
typedef int funnyInt;
// can now do this --> funnyInt lol = 10;

// Make a stack node with data, a fun function and its next
StackNode* makeStackNode(Data* data, StackNode* next);

// push data to head of stack
void push(StackNode** head, Data* data);

Data* pop(StackNode** head);

#endif // !FILE1_H