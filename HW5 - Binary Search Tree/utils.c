// File: utils.c
// a general utility library including some functions I use a bunch.
// @author Douglas Gliner <dgg5503>
// @date 02/29/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw5}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes
#include "utils.h"

// Prototypes
// TODO COMMENT
static void quicksort_hidden(int numbers[], int low, int high);
static int partition(int numbers[], int low, int high);

/// COMMENT TODO
// based off of pseudocode in https://en.wikipedia.org/wiki/Quicksort
void quicksort(int numbers[], int size)
{
	// make sure size is somehting sortable.
	if (size < 2)
		return;

	// use hidden functions
	quicksort_hidden(numbers, 0, size - 1);
}

/// COMMENT TODO
void quicksort_hidden(int numbers[], int low, int high)
{
	if (low < high)
	{
		int p = partition(numbers, low, high);
		quicksort_hidden(numbers, low, p - 1);
		quicksort_hidden(numbers, p + 1, high);
	}
}

/// COMMENT TODO
int partition(int numbers[], int low, int high)
{
	int pivot = numbers[high];
	int i = low;
	for (int j = low; j < high; ++j)
	{
		if (numbers[j] <= pivot)
		{
			// swap num[i] with num[j]
			swap(&numbers[i], &numbers[j]);
			i++;
		}
	}
	// swap num[i] with num[high]
	swap(&numbers[i], &numbers[high]);
	return i;
}

/// TODO COMMENT
void swap(int * const num1, int * const num2)
{
	int tmpInt = *num1;
	*num1 = *num2;
	*num2 = tmpInt;
}
// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 

