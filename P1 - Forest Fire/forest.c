// File: forest.c 
// holds functions relating to forest related stuff
// @author Douglas Gliner <dgg5503>
// @date 02/17/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project1}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes
#include "forest.h"

// Proto-types
static void spread(char *tree,
	struct Neighbors *neighbors,
	struct WildFireArgs *wfArgs);

static struct Neighbors get_neighbors(const int x,
	const int y,
	const int size,
	char matrix[size][size]);



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
	char matrix[size][size])
{
	struct Neighbors neighbors;
	int y, x;

	// store a copy of the matrix (T+1)
	char matrixCopy[size][size];
	for (y = 0; y < size; ++y)
		for (x = 0; x < size; ++x)
			matrixCopy[y][x] = matrix[y][x];

	// grab neighbors from T, apply changes to T+1 (matrixCopy)
	for (y = 0; y < size; ++y)
	{
		for (x = 0; x < size; ++x)
		{
			// make sure we're looking at a non empty cell
			if (matrix[y][x] != EMPTY)
			{
				neighbors = get_neighbors(x, y, size, matrix);
				spread(&matrixCopy[y][x], &neighbors, wfArgs);
			}
		}
	}

	// copy T+1 to T
	for (y = 0; y < size; ++y)
		for (x = 0; x < size; ++x)
			matrix[y][x] = matrixCopy[y][x];
}

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
	struct WildFireArgs *wfArgs)
{
	// can only spread to a tree that isnt burning/empty/burned
	if (*tree == EMPTY || *tree == BURNED_TREE)
		return;

	float random_float = 0;

	// if tree is burning, see if it should burn out now.
	if (*tree == BURNING_TREE)
	{
		random_float = rand() / (float)RAND_MAX;
		if (random_float < wfArgs->probability)
		{
			*tree = BURNED_TREE;
			--wfArgs->numOfTreesBurning;
			++wfArgs->changes;
		}
	}
	else
	{
		// min number of cells to burn is 25% of neighbors (includes tree or space)
		const int minCellsBurning = neighbors->numOfNeighbors * .25f;
		int cellsBurning = 0;

		// calc % of neighbors burning
		for (int i = 0; i < neighbors->numOfNeighbors; ++i)
		{
			// if tree burning, increment burning and check to see if we've met the min
			if (*(neighbors->neighborPtrs[i]) == BURNING_TREE
				&& (++cellsBurning >= minCellsBurning))
			{
				// we're at min, apply random percent and compare to given prob
				random_float = rand() / (float)RAND_MAX;
				if (random_float < wfArgs->probability)
				{
					// tree should now catch fire
					*tree = BURNING_TREE;
					++wfArgs->numOfTreesBurning;
					++wfArgs->changes;

					// set the tree to burning an return since we've reached our goal.
					return;
				}
			}
		}
		// not enough neighbors, nothing happens.
	}
}

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
	char matrix[size][size])
{
	// init blank neighbor struct
	struct Neighbors neighbors = { 0,{ 0 } };

	// neighbors will NOT include wrap around.
	for (int row = y - 1; row <= y + 1; ++row)
	{
		// check to see if oob for row
		if (row < size && row >= 0)
		{
			for (int col = x - 1; col <= x + 1; ++col)
			{
				// check to see if oob for col
				if (col < size && col >= 0)
				{
					// ensure that we skip the tree itself.
					if (row != y || col != x)
					{
						// finally good!
						neighbors.neighborPtrs[neighbors.numOfNeighbors] = &matrix[row][col];
						++(neighbors.numOfNeighbors);
					}
				}
			}
		}
	}

	//return neighbors for this x,y
	return neighbors;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
