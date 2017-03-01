// File: linkedList.h
// Basic linked list that will end up being turned into an array.
// @author Douglas Gliner <dgg5503>
// @date 04/26/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project3}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// Includes
#include "utils.h"

// Macros

// Structs
#ifndef _LL_IMPL_
typedef struct LLNode *LLNode;
#endif

// Functions
/// Add data to the provided Linked List. This data will be placed at the
/// head of the list.
/// @param linkedList the list to add data to
/// @param dataToAdd void ptr of data to add.
void addNode(LLNode* linkedList, void* dataToAdd);

/// Provides the linked list in array form for easy access of elements.
/// @param linkedList the list to turn into an array
/// @param length the amount of data in the linked list to return in the array.
/// @return a void* array of data found in this linked list
void* convertToArray(LLNode* linkedList, unsigned int length);

/// Deallocs the nodes found in this list. Does NOT dealloc the data ptrs.
/// @param linkedList the list to destroy
void destroyList(LLNode* linkedList);

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
