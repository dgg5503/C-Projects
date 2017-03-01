// File: wildfire.h
// header file to wildfire.c containing prototype functions, structs, and enums
// @author Douglas Gliner <dgg5503>
// @date 02/17/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project1}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef WILDFIRE_H
#define WILDFIRE_H

// Defines
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

/// Applies spread to the provided tree based on surrounding neighbors. Will
/// return if provided a tree marked as EMPTY or BURNED_TREE. Given tree will
/// catch fire if probability from WildFireArgs is met.
///
/// @param tree  pointer to tree in forest
/// @param neighbors  neighbors of the tree pointer provided
/// @param wfArgs  settings and arguments for the wildfire program
///
void spread(char *tree,
	struct Neighbors *neighbors,
	struct WildFireArgs *wfArgs);

/// Gets neighbors of a cell at a given x, y location with a provided matrix
/// and its size.
///
/// @param x  the x location of the cell you want to the neighbors of
/// @param y  the y location of the cell you want to the neighbors of
/// @param size  the size of the provided matrix
/// @param matrix  the matrix containing the cell you want to get the neighbors
/// of.
///	@returns neighbors  neighbors struct containing an array of pointers to the
///	neighbors and the number of neighbors found.
///
struct Neighbors get_neighbors(const int x,
	const int y,
	const int size,
	char matrix[size][size]);

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
