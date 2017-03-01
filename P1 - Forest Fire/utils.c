// File: utils.c 
// a general utility library including some functions I use a bunch.
// @author Douglas Gliner <dgg5503>
// @date 02/29/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project1}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#include "utils.h"

/// prints usage information of this program to stderr.
void usage(void)
{
	ERR(
		"usage: wildfire [-pN] size probability treeDensity proportionBurning\n"
		"The -pN option tells the simulation to print N cycles and stop.\n"
		"The probability is the probability a tree will catch fire.\n"
		);
}

/// Prints the provided matrix either sequentially or in place. If printing
/// inplace, the cursor is placed at the bottom of the printed matrix.
///
/// @param size  the size of the provided matrix
/// @param matrix  a char matrix
/// @param mode  the printing mode that should be used
///	INPLACE - prints the matrix relative to the top left of the console
/// SEQUENTIAL - prints the matrix using printf
///
void print_matrix(const int size, char matrix[size][size], enum PrintMode mode)
{
	// make sure size is valid
	if (size <= 0)
	{
		ERR("Invalid matrix size!");
		return;
	}

	// print depending on provided mode
	int y, x;
	switch (mode)
	{
	case SEQUENTIAL:
		// print matrix
		for (y = 0; y < size; ++y)
		{
			for (x = 0; x < size; ++x)
			{
				printf("%c", matrix[y][x]);
			}
			printf("\n");
		}
		break;

		// console update!
	case INPLACE:
		// print matrix
		for (y = 0; y < size; ++y)
		{
			// row + 1 since 1 is the start of rows, not 0
			// col starts at 0 tho
			// only need to set cursor once since "put" moves over by 1!
			set_cur_pos(y + 1, 0);
			for (x = 0; x < size; ++x)
			{
				put(matrix[y][x]);
			}
		}
		
		// set cursor to bottom of matrix.
		set_cur_pos(size + 1, 0);
		break;

	default:
		ERR("Invalid print mode!");
		break;
	}
}

/// str_to_int converts the input C string to an integer value.
///
/// @param str_score the NUL-terminated C String to convert
/// @return integer value of str_score
///
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

/// Calculates mod the same way Google does!
///
/// @param a  the left side of the mod
/// @param b  the right side of the mod
/// @return  mod as calculated by Google 
///
int negMod(int a, int b)
{
	// cant mod two neg numbers (?)
	if (a < 0 && b < 0)
		return 0;

	if (a < 0)
		return b - (abs(a) % b);

	if (b < 0)
		return (a % abs(b)) - abs(b);

	return a % b;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
