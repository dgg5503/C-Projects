/// http://www.dreamincode.net/forums/topic/55572-conways-game-of-life/
/// downloaded by copy-paste on 8/19/2015 and modified to use an infinite loop.
///
/// This code needs serious work in these areas:
/// <ol>
/// <li>  syntax: there are a number of warnings that cause errors.
/// </li>
/// <li>  semantics: the game algorithm implementation is incorrect.
/// </li>
/// <li>  design: the implementation needs function and value refactoring.
/// </li>
/// <li>  style: formatting is poor; e.g. mix of TAB and spaces indentation
/// </li>
/// <li>  documentation: documentation is non-existent except for this block.
/// </li>
/// </ol>
///

// File: good-life.c 
// attempt at fixing someones code for Conway's Game of Life
// @author Douglas Gliner <dgg5503>
// @date 02/12/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw3}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes
#include <stdio.h>
#include <stdlib.h>

// Defines & Macros
#define MAT_WIDTH 20
#define MAT_HEIGHT 20

/// Prints welcome message to the console
void header(void)
{
	printf("\n\t..Welcome to the Game of life..\n");
}

/// Gets the number of neighbors at a specified row and column within a given
/// matrix. A neighbor is considered to be an asterisk adjacent to provided
/// location.
///
/// @param row  row to check for neighbors
/// @param col  column to check for neighbors
/// @param width  width of provided matrix
/// @param height  height of provided matrix
/// @param life  matrix of organisms where each organism is an asterisk
/// @returns neighbors  number of neighbors adjacent to given organism location
int getNeighbors(const int row, const int col, const int width, const int height, char life[height][width])
{
	int rowMinus, rowPlus, colMinus, colPlus;
	int neighbors = 0;

	// compensate for wrap around!
	if ((rowMinus = row - 1) < 0)
		rowMinus = height - 1;

	if ((rowPlus = row + 1) >= height)
		rowPlus = 0;

	if ((colMinus = col - 1) < 0)
		colMinus = width - 1;

	if ((colPlus = col + 1) >= width)
		colPlus = 0;

	// check for neighbors
	if (life[rowMinus][colMinus] == '*')
		++neighbors;
	if (life[rowMinus][col] == '*')
		++neighbors;
	if (life[rowMinus][colPlus] == '*')
		++neighbors;
	if (life[row][colMinus] == '*')
		++neighbors;
	if (life[row][colPlus] == '*')
		++neighbors;
	if (life[rowPlus][colMinus] == '*')
		++neighbors;
	if (life[rowPlus][col] == '*')
		++neighbors;
	if (life[rowPlus][colPlus] == '*')
		++neighbors;

	return neighbors;
}

/// Runs rules for Conway's Game of Life based on "B3/S23." Rules consist of
/// birth (exactly 3 neighbors), survival (2 or 3 neighbors) and death
/// (all other cases).
///
/// @param width  width of provided matrix
/// @param height  height of provided matrix
/// @param life  matrix of organisms where each organism is an asterisk
void runRules(const int width, const int height, char life[height][width])
{
	int row, col;
	char tmpLife[height][width];

	// Time T + 1 matrix
	for (row = 0; row < height; ++row)
		for (col = 0; col < width; ++col)
			tmpLife[row][col] = life[row][col];

	// double for through all cells (not asterisks) in the matrix
	// apply rules to time T + 1 matrix
	for (row = 0; row < height; ++row)
	{
		for (col = 0; col < width; ++col)
		{
			// rule check
			switch (getNeighbors(row, col, width, height, life))
			{
				// if 2 neighbors, do nothing
			case 2:
				break;

				// if 3 neighbors, spawn a cell.
				// if a cell already exists here, essentially nothing is happening.
				// (implement if check to not assign an already assigned cell???)
			case 3:
				if (life[row][col] == ' ')
					tmpLife[row][col] = '*';
				break;

				// any other case means dead :(
			default:
				tmpLife[row][col] = ' ';
				break;
			}
		}
	}

	// Copy values from T + 1 to T
	for (row = 0; row < height; ++row)
		for (col = 0; col < width; ++col)
			life[row][col] = tmpLife[row][col];
}


/// Prints all characters within given character matrix.
///
/// @param width  width of provided matrix
/// @param height  height of provided matrix
/// @param matrix  matrix of characters
void printMatrix(int width, int height, char matrix[height][width])
{
	int row, col;

	// print matrix
	for (row = 0; row < height; ++row)
	{
		for (col = 0; col < width; ++col)
		{
			printf("%c", matrix[row][col]);
		}
		puts(" "); // \n???
	}
}

/// Program to run simulate Conway's Game of Life starting with a user given
/// number of organisms.
///
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
int main(void)
{
	char life[MAT_HEIGHT][MAT_WIDTH];
	int orgs;
	int i, row, col;
	int gen = 0;

	// print welcome message
	header();
	
	// ask for starting num of orgs
	printf("\nPlease enter the initial number of organisms: ");
	scanf("%i", &orgs);

	// set random seed to 31
	srand(31);

	// initalize organisms in "random" locations
	for(i = 0; i < orgs; ++i)
	{
		row = rand();
		row %= MAT_HEIGHT;
		col = rand();
		col %= MAT_WIDTH;
		life[row][col] = '*';
	}

	// initalize all locations in the matrix that arent organisms to blank spaces
	for(row = 0; row < MAT_HEIGHT; ++row)
	{
		for(col = 0; col < MAT_WIDTH; ++col)
		{
			if(life[row][col] != '*')
				life[row][col] = ' ';
		}
	}

	// print matrix
	printMatrix(MAT_WIDTH, MAT_HEIGHT, life);

	// step through 100 generations
	while (gen < 100)
	{
		// run rules for life
		runRules(MAT_WIDTH, MAT_HEIGHT, life);

		// print matrix
		printMatrix(MAT_WIDTH, MAT_HEIGHT, life);

		// increment generation and let the use know what generation we're up to
		printf("\ngeneration: %i\n", gen);
		++gen;
	}

	return 0;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 