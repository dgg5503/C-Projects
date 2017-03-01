// File: forest.h
// header file to forest.c containing prototype functions, structs, and enums
// @author Douglas Gliner <dgg5503>
// @date 02/17/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project1}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef FOREST_H
#define FOREST_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Defines
#define MAX_NEIGHBORS 8
#define EMPTY ' '
#define TREE 'Y'
#define BURNING_TREE '*'
#define BURNED_TREE '_'

/// Used when returning neighbors from the get_neighbors() function. Contains
/// total number of neighbors placed into the pointer array. Pointer array
/// contains all the neighbors pointers allowing for easy modification.
struct Neighbors
{
	int numOfNeighbors;
	char *neighborPtrs[MAX_NEIGHBORS];
};

/// Used to store arguments and other data that is passed between functions
struct WildFireArgs
{
	int changes;
	int numOfTrees;
	int numOfTreesBurning;
	float probability;
	float treeDensity;
	float proportionBurning;
};

// Wildfire Functions

/// Makes a copy of the given matrix, apply spread to the matrix's tree and
/// then place values from the copy matrix back into the provided matrix.
/// Neighbors for a given x,y location are placed in a neighbor struct.
///
/// @param wfArgs  Arguments and settings for this wildfire program
/// @param size  size of the provided matrix
/// @param matrix  matrix that represents the forest
///
void update(struct WildFireArgs *wfArgs,
	const int size,
	char matrix[size][size]);

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
