// File: utils.h
// a general utility library including some functions I use a bunch.
// @author Douglas Gliner <dgg5503>
// @date 02/29/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project1}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef UTILS_H
#define UTILS_H

// Includes
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "display.h"

// Enums
/// Hold print mode information for print_matrix
enum PrintMode
{
	SEQUENTIAL = 0,
	INPLACE
};

// Macros
// fprintf to standard error
// must put ; wherever used!
#define ERR(...) fprintf(stderr, __VA_ARGS__)

// Utils Functions

/// prints usage information of this program to stderr.
void usage(void);

/// Prints the provided matrix either sequentially or in place. If printing
/// inplace, the cursor is placed at the bottom of the printed matrix.
///
/// @param size  the size of the provided matrix
/// @param matrix  a char matrix
/// @param mode  the printing mode that should be used
///	INPLACE - prints the matrix relative to the top left of the console
/// SEQUENTIAL - prints the matrix using printf
///
void print_matrix(const int size,
	char matrix[size][size],
	enum PrintMode mode);

/// str_to_int converts the input C string to an integer value.
///
/// @param str_score the NUL-terminated C String to convert
/// @return integer value of str_score
///
int str_to_int(char str_score[]);

/// Calculates mod the same way Google does!
///
/// @param a  the left side of the mod
/// @param b  the right side of the mod
/// @return  mod as calculated by Google 
///
int negMod(int a, int b);

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
