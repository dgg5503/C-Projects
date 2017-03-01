// File: valueMath.h
// Series of functions which perform calculations on value structs.
// @author Douglas Gliner <dgg5503>
// @date 4/05/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef VALMATH_H
#define VALMATH_H

// Includes
#include "utils.h"
#include "symTbl.h"

// Macros

// Utils Functions
/// Adds two values taking type into account. 
/// @param v1 first value to add
/// @param v2 second value to add
/// @return the evaluation as a value
Value addOp(Value v1, Value v2);

/// Subtracts two values taking type into account. 
/// @param v1 first value to subtract
/// @param v2 second value to subtract (v2 - v1)
/// @return the evaluation as a value
Value subOp(Value v1, Value v2);

/// Multiplies two values taking type into account. 
/// @param v1 first value to multiply
/// @param v2 second value to multiply
/// @return the evaluation as a value
Value multOp(Value v1, Value v2);

/// Divides two values taking type into account. Division by zero is also
/// checked.
/// @param v1 first value to divide
/// @param v2 second value to divide (v2 / v1)
/// @return the evaluation as a value
Value divOp(Value v1, Value v2);

/// Mods two values taking type into account. Both given values must be int!
/// Mod by 0 is also checked.
/// @param v1 first value to mod
/// @param v2 second value to mod (v2 % v1)
/// @return the evaluation as a value
Value modOp(Value v1, Value v2);

/// Assigns a value to a given symbol or creates a new one.
/// Checks to see if symTab is full and if types match.
/// @param symbol the symbol to create / modify
/// @param valToAdd the value to add/modify to the given symbol
/// @return the value that was assigned to the symbol
Value assignOp(char* symbol, Value valToAdd);

/// Looks up a provided symbol and returns its value if it exists.
/// @param symbol symbol to look up
/// @return the symbols value, if it exists
Value symbolOp(char* symbol);

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
