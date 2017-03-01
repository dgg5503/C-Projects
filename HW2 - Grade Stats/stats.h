// File: stats.h 
// stats.h is the interface for a simple statistics module.
// @author bks: ben k steele
// @author Douglas Gliner <dgg5503>
// @date 02/02/2016
// 
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw2}
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#ifndef STATS_H
#define STATS_H

/// Calculates and returns the average / mean from a given array of integer
/// scores based on its given length.
///
/// @param count  number of scores in given scores array
/// @param scores  array of scores represented as numbers
/// @returns average  average of the entire scores array
///
double average( int count, const int scores[] ) ;

/// Calculates and returns the sample standard deviation from a given array of
/// integer scores based on its given length and provided mean.
///
/// @param count  number of scores in given scores array
/// @param scores  array of scores represented as numbers
/// @param mean  mean of the array of scores
/// @returns stdDev  sample standard deviation based of provided scores and
///					 mean
///
double std_dev( int count, const int scores[], double mean ) ;

/// Prints out a histogram of the scores based off of RIT's plus/minus letter
/// grading system.
///
/// @param count  number of scores in given scores array
/// @param scores  array of scores represented as numbers
///
void histogram( int count, const int scores[] ) ;

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
