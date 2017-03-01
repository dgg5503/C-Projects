#include "file1.h"

// this assumes that data has already been properly allocated
StackNode* makeStackNode(Data* data, StackNode* next)
{
	// stack node to make
	StackNode* stackNode = malloc(sizeof(StackNode));

	// ensure stackNode didnt fail to malloc!
	assert(stackNode != NULL);

	// assign data
	stackNode->data = data;
	stackNode->next = next;
	stackNode->regularInt = 0;

	// return
	return stackNode;
}

// head = a ptr to the stack
// data = data to add!!
void push(StackNode** head, Data* data)
{
	// setup the first node!!
	if (*head == NULL)
		*head = makeStackNode(data, NULL);
	else
		// set current head as next head!
		*head = makeStackNode(data, *head);
}

// free the stacknode at the head and return the
// data pointer!
Data* pop(StackNode** head)
{
	// ensure head isnt NULL
	assert(*head != NULL);

	// store data ptr
	Data* data = (*head)->data;

	// store curr head
	StackNode* currHead = *head;

	// set new head to next
	*head = (*head)->next;

	// free the old head
	free(currHead);
	currHead = NULL;

	// return data
	return data;
}