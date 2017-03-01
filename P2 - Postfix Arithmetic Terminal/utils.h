// File: utils.h
// a general utility library including some functions I use a bunch.
// @author Douglas Gliner <dgg5503>
// @date 02/29/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef UTILS_H
#define UTILS_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include "expNode.h"
#include "parser.h"

// Macros
// fprintf to standard error
// must put ; wherever used!
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#define FREENULL(ptr) free(ptr); ptr = NULL
#define MAC2STR(x) MACVAR(x)
#define MACVAR(x) #x

// Utils Functions
/// Prints nodes relevant information based on its type.
/// @param node the node to print
void printNode(ExpNode* node);

/// Sets the error message if current error message is already NONE
/// @param setError the error to set
void setErrorMessage(ParserError setError, char* symbol);

/// Get the current parser error.
/// @return returns the parser error.
ParserError getUtilsParserError();

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
