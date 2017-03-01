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
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw3/act3}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines & Macros
#define _BSD_SOURCE

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "display.h"

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
int getNeighbors(const int row, const int col, 
	const int width, const int height, char life[height][width])
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
					if(life[row][col] == ' ')
						tmpLife[row][col] = '*';
					break;

				// any other case means dead :(
				default:					
					tmpLife[row][col] = ' ';
					break;
			}
		}
	}

	// RANDOM BIRTH???
	// 5 percent chance and only if that spot is empty! [0-100)
	if ((rand() % 100) < 5)
	{
		row = rand();
		row %= height;
		col = rand();
		col %= width;

		if(tmpLife[row][col] == ' ')
			tmpLife[row][col] = '*';
	}

	// Copy values from T + 1 to T
	for (row = 0; row < height; ++row)
		for (col = 0; col < width; ++col)
			life[row][col] = tmpLife[row][col];
}

/// Spawns a special organism starting at corner of given location.
/// NOTE: Special organism will overwrite any cells at given location!!
///
/// @param row  row to spawn special organism
/// @param col  column to spawn special organism
/// @param width  width of provided matrix
/// @param height  height of provided matrix
/// @param life  matrix of organisms where each organism is an asterisk
/// @param specWidth  width of provided special organism template
/// @param specHeight  height of provided special organism template
/// @param specialTemplate  matrix of characters for a special organism
void spawnSpecial(const int row, const int col,
	const int width, const int height, char life[height][width],
	const int specWidth, const int specHeight,
	const char specTemplate[specHeight][specWidth])
{
	int x, y;

	// oob checks
	if (row > height || row < 0 || col > width || col < 0)
		return;

	// place our template in our location and account for overlap!
	// probably slow since %'ing :(...
	for (y = row; y < row + specHeight; ++y)
		for (x = col; x < col + specWidth; ++x)
			life[y % height][x % width] = 
			specTemplate
				[specHeight - ((row + specHeight) - y)]
				[specWidth - ((col + specWidth) - x)];

}


/// Prints all characters within given character matrix relative to the top
/// left corner of the console window.
///
/// @param width  width of provided matrix
/// @param height  height of provided matrix
/// @param matrix  matrix of characters
void printMatrix(const int width, const int height, char matrix[height][width])
{
	int row, col;

	// print matrix
	for (row = 0; row < height; ++row)
	{
		// row + 1 since 1 is the start of rows, not 0
		// col starts at 0 tho!
		//set_cur_pos(row + 1, 0);
		for (col = 0; col < width; ++col)
		{
			// only need to set cursor once since "put" moves over by 1!
			set_cur_pos(row + 1, col);
			put(matrix[row][col]);
			
		}
	}
}

/// str_to_int converts the input C string to an integer value.
///
/// @param str_score the NUL-terminated C String to convert
/// @return integer value of str_score
int str_to_int(char str_score[]) {

	int value = 0;
	for (int j = 0; str_score[j] != '\0'; ++j) {

		if (isdigit(str_score[j])) {
			value = value * 10 + (str_score[j] - '0');

		}
		else {
			// stop on finding the first non-numeric digit.
			break;   // the value is not an integer.
		}
	}
	return value;
}

/// Program to run simulate Conway's Game of Life starting with a user given
/// number of organisms.
///
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
int main(int argc, char* argv[])
{
	// static vars for matrix size
	static int matWidth_g = 20;
	static int matHeight_g = 20;

	// Setup width and height before hand...
	if (argc > 1)
	{
		// if just one arg, set height and width to the values
		if (argc == 2)
		{
			int squareValue = str_to_int(argv[1]);
			matWidth_g = squareValue;
			matHeight_g = squareValue;
		}
		else
		{
			// if two, set width to first and height to second
			matWidth_g = str_to_int(argv[1]);
			matHeight_g = str_to_int(argv[2]);
		}

		// checks (dont these are possible tho...)
		if (matHeight_g <= 0)
			matHeight_g = 20;

		if (matWidth_g <= 0)
			matWidth_g = 20;
	}

	// Templates
	// a glider!
	static const char gliderTemplate[3][3] = {
		{ ' ', '*', ' ' },
		{ ' ', ' ', '*' },
		{ '*', '*', '*' }
	};

	// spaaaaaace
	static const char spaceshipTemplate[4][5] = {
		{ ' ', '*', '*', '*', '*' },
		{ '*', ' ', ' ', ' ', '*' },
		{ ' ', ' ', ' ', ' ', '*' },
		{ '*', ' ', ' ', '*', ' '}
	};

	// BLINKEH BLINKEH 
	static const char blinkerTemplate[3][3] = {
		{ ' ', '*', ' ' },
		{ ' ', '*', ' ' },
		{ ' ', '*', ' ' }
	};

	// vars
	char life[matHeight_g][matWidth_g];
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
		row %= matHeight_g;
		col = rand();
		col %= matWidth_g;
		life[row][col] = '*';
	}
	

	// spawn a glider, spaceship and blinker!
	spawnSpecial(0, 18, matWidth_g, matHeight_g, life, 3, 3, gliderTemplate);
	spawnSpecial(10, 10, matWidth_g, matHeight_g, life, 5, 4, spaceshipTemplate);
	spawnSpecial(3, 3, matWidth_g, matHeight_g, life, 3, 3, blinkerTemplate);

	// initalize all locations in the matrix that arent organisms to blank spaces
	for(row = 0; row < matHeight_g; ++row)
	{
		for(col = 0; col < matWidth_g; ++col)
		{
			if(life[row][col] != '*')
				life[row][col] = ' ';
		}
	}
	
	// clear screen once since entire matrix is redrawing everything
	clear();

	// print matrix initial gen
	printMatrix(matWidth_g, matHeight_g, life);

	// sleep :)
	usleep(81000);
		
	// inf loop!
	while (1)
	{
		// run rules for life
		runRules(matWidth_g, matHeight_g, life);

		// print matrix
		printMatrix(matWidth_g, matHeight_g, life);

		// set to bottom of display and print gen num
		set_cur_pos(matHeight_g + 1, 0);
		printf("%i\n", gen);
		
		// after one run through, a generation has occured!
		++gen;
		
		// sleep :)
		usleep(81000);
	}

	return 0;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 