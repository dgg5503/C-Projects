/// 
/// File: triangle.c 
/// 
/// A program to print out multiple triangles "right-up" triangles.
/// of the form
///    *
///   **
///  ***
/// ****
/// 
/// @author dgg5503 : Douglas Gliner
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// TODO_ADD_#INCLUDES_ HERE
#include <stdio.h>

/**
 *  Function: drawTriangle
 *
 *  Description: function to draw a triangle whose height/width is "size"
 * 
 *  @param size  the size of the triangle to draw
 */
// TODO_WRITE_DRAWTRIANGLE_FUNCTION HERE

void drawTriangle(int size)
{
	// if size is even, increment by one so we have a nice looking triangle
	/*
	   Peform bitwise & to see if right most bit of integer is 1 or 0.
	   If the bit is 1 then 2^0 = 1 is part of the final binary -> decimal
	   conversion which clearly means that its odd since every other bit
	   to the left is divisble by 2 and numbers divisible by 2 added together
	   yield a number also divisble by 2. Adding a 1 to an even number will
	   make it odd.

	   NOTE: If this was a signed negative integer, this may not work on some systems
	   like those that have one complement representations for numbers.
	   The usual is two complement...
	*/
	if (!(size & 1))
		size++;

	
	// odd number of stars in every row, just do +=2 starting from 1
	for (int stars = 1; stars <= size; stars += 2)
	{
		// this for loop decides if we should print a space or *
		for (int i = 0; i < size; i++)
		{
			// numOfSpacesInALine = size - stars
			if ((size - stars) > i) // if numOfSpacesInALine > the current line index, then we should print *!
				printf(" ");
			else
				printf("*");
		}

		// after drawing a line, just go to the next line and
		// get the next number of stars to draw
		printf("\n");
	}


	// One loop method, probably not faster.
	/*
	// number of spaces to start with
	int spaces = size - 1;

	// Triangles are not boxes...
	for (int i = 0; i < size * ((size + 1) / 2); i++)
	{
		// we're at the end of the line
		// draw star and line break....
		if ((i + 1) % size == 0)
		{
			printf("*\n");

			// Start from new line offset "i" and add to it the next highest future
			// location of spaces.
			// i.e. on first line, we will have size - 1 spaces where the newline offset is i
			// (0 in this case)
			// to determine # of stars for this line, simply do 2*(current line number) - 1
			// We ignore the last star on a line since we're printing it here
			// We increment i so we can get a number divis by size
			// We offset the line number n by +1 (in 2n - 1) so we can obtain the correct amount of stars 
			// We subtract by size to obtain number of spaces and add to I to get our new max space limit.
			//spaces = i + (size - (2 * (((i + 1) / size) + 1) - 1));

			// below is a simplified version of above
			spaces = i + size - (2 * (i + 1) / size);
			continue;
		}

		// as long as our current index is less than the max #
		// of spaces for this line, keep drawing spaces!
		// other wise do *'s
		if(spaces >= i)
			printf(" ");
		else
			printf("*");
		
	}
	*/
	//printf("%i\n", size);
}

/**
 *  Function: main 
 *
 *  Description: program to draw 3 triangles of size 1, 4, and 6.
 *
 *  @returns errorCode  error Code; EXIT_SUCCESS if no error
 */
// TODO_WRITE_MAIN_FUNCTION HERE

int main(void)
{
	drawTriangle(1);
	printf("\n");

	drawTriangle(4);
	printf("\n");

	drawTriangle(6);
	printf("\n");

	return 0;
}


// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
