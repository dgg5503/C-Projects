#ifndef FILE2_H
#define FILE2_H

#include <stdio.h>	
#include <stdlib.h>
#include <assert.h>

#define INIT_SIZE 4

#ifndef __STRUCT_IMPL__
typedef struct { } *HeapADT;
#endif // !__STRUCT_IMPL__

HeapADT createHeap(int(*comparator)(const void *a, const void *b));

void destroyHeap(HeapADT heap);

void clearHeap(HeapADT heap);

void addData(HeapADT heap, void* data);

void* removeData(HeapADT heap);

void* peekHeap(HeapADT heap);

#endif