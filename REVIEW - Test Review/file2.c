#define __STRUCT_IMPL__

typedef struct {
	// root
	void** heapArray;

	// comparator
	int (*func)(const void *a, const void *b);

	// size of dynamic array
	unsigned int size;

	// # of elements
	unsigned int count;
} HeapRoot, *HeapADT;

#include "file2.h"

HeapADT createHeap(int(*comparator)(const void *a, const void *b))
{
	// malloc heap
	HeapRoot* heapADT = malloc(sizeof(HeapRoot));
	if (heapADT == NULL ||
		comparator == NULL)
		return NULL;

	// init
	heapADT->heapArray = calloc(INIT_SIZE, sizeof(void*));
	heapADT->func = comparator;
	heapADT->size = INIT_SIZE;
	heapADT->count = 0;

	// return
	return heapADT;
}

void addData(HeapADT heap, void * data)
{
	// if heap empty, just add at top.
	if (!heap->count)
	{
		heap->heapArray[0] = data;
		heap->count++;
	}
	else
	{
		// compare against root then place at child if...
		// child spot is NULL
		unsigned int currSpot = 0;
		while (1)
		{
			if (heap->func(data, heap->heapArray[currSpot]) > 0)
			{
				if ((currSpot = ((2 * currSpot) + 1)) >= heap->size)
				{
					// realloc and add to spot
					heap->size *= heap->size;
					heap->heapArray = realloc(heap->heapArray, sizeof(void*) * heap->size);
					heap->heapArray[currSpot] = data;
					heap->count++;
					return;
				}
				else
				{
					// if there is something in the spot, recurse.
					if (heap->heapArray[currSpot] == NULL)
					{
						// nothing in spot, just place it!
						heap->heapArray[currSpot] = data;
						heap->count++;
						return;
					}
				}
			}
			else // less than or equal to...
			{
				if ((currSpot = ((2 * currSpot) + 2)) >= heap->size)
				{
					// realloc and add to spot
					heap->size *= heap->size;
					heap->heapArray = realloc(heap->heapArray, sizeof(void*) * heap->size);
					heap->heapArray[currSpot] = data;
					heap->count++;
					return;
				}
				else
				{
					// if there is something in the spot, recurse.
					if (heap->heapArray[currSpot] == NULL)
					{
						// nothing in spot, just place it!
						heap->heapArray[currSpot] = data;
						heap->count++;
						return;
					}
				}
			}
		}
	}
}
