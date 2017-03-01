// File: utils.h
// a general utility library including some functions I use a bunch.
// @author Douglas Gliner <dgg5503>
// @date 02/29/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw7}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef UTILS_H
#define UTILS_H

// Includes
#include <stdio.h>
#include <stdlib.h>

// Macros
// fprintf to standard error
// must put ; wherever used!
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#define FREENULL(ptr) free(ptr); ptr = NULL
#define MAC2STR(x) MACVAR(x)
#define MACVAR(x) #x

// Utils Functions

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
