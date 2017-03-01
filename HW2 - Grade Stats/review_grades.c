// File: review_grades.c 
// computes statistics on a sequence of grades entered on the command line,
// and reports the results on standard output (stdout)
// @author Douglas Gliner <dgg5503>
// @date 02/05/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes & Defines
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stats.h"

#define MAX_NUM_GRADES 32

// Proto-functions
int str_to_int(char str_score[]);

/// Display statistics for provided grades within the programs arguments
///
/// @param argc  number of grades entered, including program name
/// @param argv  supplied grades, including program name
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
///
int main(int argc, char* argv[])
{
	// Show usage
	if (argc == 1)
	{
		fprintf(stderr, "usage: review_grades score1 [score2 ...]\n");
		fprintf(stderr, "note: score values assumed to be non-negative.\n");
		return EXIT_FAILURE;
	}
	
	// Make sure not too many args are given!
	if (argc - 1 >= MAX_NUM_GRADES)
	{
		fprintf(stderr, "too many arguments!");
		return EXIT_FAILURE;
	}

	// Define grades array and init.
	int grades[MAX_NUM_GRADES] = { 0 };

	// 1. a line displaying the population
	fprintf(stdout, "population: %i\n", argc - 1);  // PRINT POPULATION TWICE?

	// 2. a line displaying the score input values, each separated by a single
	// space
	// Process and print the array.
	for (int i = 1; i < argc; ++i)
	{
		// ALERT: starting at 1 so we need to be i - 1 for grades!	
		grades[i - 1] = str_to_int(argv[i]);
		fprintf(stdout, "%i ", grades[i - 1]); // ALERT: MIGHT NEED TO REMOVE SPACE
	}
	fprintf(stdout, "\n");

	// Statistics!!!
	// 3. a line displaying the average of those values
	// (use the default number of digits of precision)
	double mean = average(argc - 1, grades);
	fprintf(stdout, "mean: %f\n", mean);

	// 4. a line displaying the standard deviation of those values
	// (use the default number of digits of precision)
	double standardDeviation = std_dev(argc - 1, grades, mean);
	fprintf(stdout, "std deviation: %g\n", standardDeviation);

	// 5. a histogram table of counts of 'A' grades, 'A-' grades
	// down to the count of the 'F' grades.
	histogram(argc - 1, grades);

	return EXIT_SUCCESS;
}

/// str_to_int converts the input C string to an integer value.
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

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
