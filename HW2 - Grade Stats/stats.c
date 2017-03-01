// File: stats.c 
// provides helper functions for simple statistics.
// @author Douglas Gliner <dgg5503>
// @date 02/05/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes & Defines
#include <stdio.h>
#include <math.h>
#include "stats.h"

#define SQUARE(x) (x) * (x)
#define GRADE_VALS 10

/// Calculates and returns the average / mean from a given array of integer
/// scores based on its given length.
///
/// @param count  number of scores in given scores array
/// @param scores  array of scores represented as numbers
/// @returns average  average of the entire scores array
///
double average(int count, const int scores[])
{
	// if count is zero (or negative), return 0;
	if (count <= 0)
		return 0;

	// for loop through each score, add to temp var
	int total = 0;
	for (int i = 0; i < count; ++i)
		total += scores[i];

	// cast total to double and return final value.
	return (double)total / count;
}

/// Calculates and returns the sample standard deviation from a given array of
/// integer scores based on its given length and provided mean.
///
/// @param count  number of scores in given scores array
/// @param scores  array of scores represented as numbers
/// @param mean  mean of the array of scores
/// @returns stdDev  sample standard deviation based of provided scores and
///					 mean
///
double std_dev(int count, const int scores[], double mean)
{
	// ensure count is > 1 so we dont div by 0 :)
	if (count <= 1)
		return 0;

	// declare vars to be used
	double div_summation = 0, summation = 0;

	// some var += SQAURE(scores[i] - (mean * mean))
	for (int i = 0; i < count; ++i)
		summation += (SQUARE(scores[i] - mean));

	// doubleVar = some var / div by count - 1
	div_summation = summation / (count - 1);

	// return sqrt(doubleVar)
	return sqrt(div_summation);
}

/// Prints out a histogram of the scores based off of RIT's plus/minus letter
/// grading system.
///
/// @param count  number of scores in given scores array
/// @param scores  array of scores represented as numbers
///
void histogram(int count, const int scores[])
{
	// ensure count is above 0
	if (count <= 0)
		return;

	// bin array corresponding to grade letter and initalize
	int i, gradeBin[GRADE_VALS] = { 0 };

	// NOTE: Must be corrected if GRADE_VALS changes!!
	const char* gradeLetters[GRADE_VALS] = {
		"A", "A-", "B+", "B", "B-",
		"C+", "C", "C-", "D", "F"
	};

	// range checking within for loop
	// NOTE: Must be corrected if GRADE_VALS changes!!
	for (i = 0; i < count; ++i)
	{
		if (scores[i] >= 92) // A
			gradeBin[0]++;
		else if (scores[i] < 92 && scores[i] >= 89) // A-
			gradeBin[1]++;
		else if (scores[i] < 89 && scores[i] >= 85) // B+ 
			gradeBin[2]++;
		else if (scores[i] < 85 && scores[i] >= 82) // B
			gradeBin[3]++;
		else if (scores[i] < 82 && scores[i] >= 79) // B-
			gradeBin[4]++;
		else if (scores[i] < 79 && scores[i] >= 75) // C+
			gradeBin[5]++;
		else if (scores[i] < 75 && scores[i] >= 72) // C
			gradeBin[6]++;
		else if (scores[i] < 72 && scores[i] >= 69) // C-
			gradeBin[7]++;
		else if (scores[i] < 69 && scores[i] >= 60) // D
			gradeBin[8]++;
		else
			gradeBin[9]++;
	}

	// print grades
	for (i = 0; i < GRADE_VALS; ++i)
		fprintf(stdout, "%s:\t%i\n", gradeLetters[i], gradeBin[i]);
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
