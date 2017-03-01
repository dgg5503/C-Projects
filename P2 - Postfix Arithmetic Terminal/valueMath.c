// File: valueMath.c 
// Series of functions which perform calculations on value structs.
// @author Douglas Gliner <dgg5503>
// @date 4/05/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros

// Includes
#include <string.h>
#include "valueMath.h"
#include "parser.h"

// TODO: OVERFLOW CHECKS?

/// Adds two values taking type into account. 
/// @param v1 first value to add
/// @param v2 second value to add
/// @return the evaluation as a value
Value addOp(Value v1, Value v2)
{
	switch (v1.type)
	{
		case TYPE_INT:
			switch (v2.type)
			{
			case TYPE_INT:
				return (Value) { TYPE_INT, { .iVal =
					v1.value.iVal + v2.value.iVal } };
				break;
			case TYPE_DOUBLE:
				return (Value) { TYPE_DOUBLE, { .dVal = 
					v1.value.iVal + v2.value.dVal } };
				break;
			default: // more types here
				break;
			}
			break;
		
		case TYPE_DOUBLE:
			switch (v2.type)
			{
			case TYPE_INT:
				return (Value) { TYPE_DOUBLE, { .dVal = 
					v1.value.dVal + v2.value.iVal } };
				break;
			case TYPE_DOUBLE:
				return (Value) { TYPE_DOUBLE, { .dVal = 
					v1.value.dVal + v2.value.dVal } };
				break;
			default: // more types here
				break;
			}
			break;

		default: // more types here
			break;
	}
	
	// unknow added
	return (Value) { TYPE_UNKNOWN, { 0 } };
}

/// Subtracts two values taking type into account. 
/// @param v1 first value to subtract
/// @param v2 second value to subtract (v2 - v1)
/// @return the evaluation as a value
Value subOp(Value v1, Value v2)
{
	switch (v1.type)
	{
	case TYPE_INT:
		switch (v2.type)
		{
		case TYPE_INT:
			return (Value) { TYPE_INT, { .iVal = 
				v2.value.iVal - v1.value.iVal } };
			break;
		case TYPE_DOUBLE:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v2.value.dVal - v1.value.iVal } };
			break;
		default: // more types here
			break;
		}
		break;

	case TYPE_DOUBLE:
		switch (v2.type)
		{
		case TYPE_INT:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v2.value.iVal - v1.value.dVal } };
			break;
		case TYPE_DOUBLE:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v2.value.dVal - v1.value.dVal } };
			break;
		default: // more types here
			break;
		}
		break;

	default: // more types here
		break;
	}

	// unknow added
	return (Value) { TYPE_UNKNOWN, { 0 } };
}

/// Multiplies two values taking type into account. 
/// @param v1 first value to multiply
/// @param v2 second value to multiply
/// @return the evaluation as a value
Value multOp(Value v1, Value v2)
{
	switch (v1.type)
	{
	case TYPE_INT:
		switch (v2.type)
		{
		case TYPE_INT:
			return (Value) { TYPE_INT, { .iVal = 
				v1.value.iVal * v2.value.iVal } };
			break;
		case TYPE_DOUBLE:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v1.value.iVal * v2.value.dVal } };
			break;
		default: // more types here
			break;
		}
		break;

	case TYPE_DOUBLE:
		switch (v2.type)
		{
		case TYPE_INT:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v1.value.dVal * v2.value.iVal } };
			break;
		case TYPE_DOUBLE:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v1.value.dVal * v2.value.dVal } };
			break;
		default: // more types here
			break;
		}
		break;

	default: // more types here
		break;
	}

	// unknow added
	return (Value) { TYPE_UNKNOWN, { 0 } };
}

/// Divides two values taking type into account. Division by zero is also
/// checked.
/// @param v1 first value to divide
/// @param v2 second value to divide (v2 / v1)
/// @return the evaluation as a value
Value divOp(Value v1, Value v2)
{
	switch (v1.type)
	{
	case TYPE_INT:
		if (v1.value.iVal == 0)
		{
			//ERR("Division by zero\n");
			//parserError = DIVISION_BY_ZERO;
			setErrorMessage(DIVISION_BY_ZERO, NULL);
			break;
		}

		switch (v2.type)
		{
		case TYPE_INT:
			return (Value) { TYPE_INT, { .iVal = 
				v2.value.iVal / v1.value.iVal } };
			break;
		case TYPE_DOUBLE:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v2.value.dVal / v1.value.iVal } };
			break;
		default: // more types here
			break;
		}
		break;

	case TYPE_DOUBLE:
		if (v1.value.dVal == 0)
		{
			//ERR("Division by zero\n");
			//parserError = DIVISION_BY_ZERO;
			setErrorMessage(DIVISION_BY_ZERO, NULL);
			break;
		}

		switch (v2.type)
		{
		case TYPE_INT:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v2.value.iVal / v1.value.dVal } };
			break;
		case TYPE_DOUBLE:
			return (Value) { TYPE_DOUBLE, { .dVal = 
				v2.value.dVal / v1.value.dVal } };
			break;
		default: // more types here
			break;
		}
		break;

	default: // more types here
		break;
	}

	// unknow added
	return (Value) { TYPE_UNKNOWN, { 0 } };
}

/// Mods two values taking type into account. Both given values must be int!
/// Mod by 0 is also checked.
/// @param v1 first value to mod
/// @param v2 second value to mod (v2 % v1)
/// @return the evaluation as a value
Value modOp(Value v1, Value v2)
{
	if (v1.type == TYPE_INT && v2.type == TYPE_INT)
	{
		if (v1.value.iVal == 0)
		{
			//ERR("Division by zero\n");
			//parserError = DIVISION_BY_ZERO;
			setErrorMessage(DIVISION_BY_ZERO, NULL);
			return (Value) { TYPE_UNKNOWN, { 0 } };
		}

		return (Value) { TYPE_INT, { .iVal = 
			v2.value.iVal % v1.value.iVal } };
	}

	//ERR("Modulus requires both types to be int\n");
	//parserError = INVALID_MODULUS;
	setErrorMessage(INVALID_MODULUS, NULL);

	// unknow added
	return (Value) { TYPE_UNKNOWN, { 0 } };
}

/// Assigns a value to a given symbol or creates a new one.
/// Checks to see if symTab is full and if types match.
/// @param symbol the symbol to create / modify
/// @param valToAdd the value to add/modify to the given symbol
/// @return the value that was assigned to the symbol
Value assignOp(char* symbol, Value valToAdd)
{
	// see if it exists
	Symbol* lookSym;

	if ((lookSym = lookupTable(symbol)) == NULL)
	{
		// not there, make a new one
		Symbol symbolToAdd = { .value = valToAdd };

		// copy name
		strcpy(symbolToAdd.varName, symbol);

		// try to add
		if (!addTable(symbolToAdd))
		{
			//ERR("Symbol table full\n");
			//parserError = SYMBOL_TABLE_FULL;
			setErrorMessage(SYMBOL_TABLE_FULL, NULL);
			return (Value) { TYPE_UNKNOWN, { 0 } };
		}

		// return?
		return valToAdd;
	}

	// exists type check
	if (lookSym->value.type != valToAdd.type)
	{
		//ERR("Assignment type mismatch\n");
		//parserError = INVALID_ASSIGNMENT;
		setErrorMessage(INVALID_ASSIGNMENT, NULL);
		// return?
		return (Value) { TYPE_UNKNOWN, { 0 } };
	}

	// assign value
	lookSym->value = valToAdd;
	return valToAdd;
}

/// Looks up a provided symbol and returns its value if it exists.
/// @param symbol symbol to look up
/// @return the symbols value, if it exists
Value symbolOp(char* symbol)
{
	Symbol* lookSym;
	if ((lookSym = lookupTable(symbol)) == NULL)
	{
		//ERR("Unknown symbol: %s\n", symbol);
		//parserError = UNKNOWN_SYMBOL;
		setErrorMessage(UNKNOWN_SYMBOL, symbol);
		return (Value) { TYPE_UNKNOWN, { 0 } };
	}

	return lookSym->value;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// Revisions: 
// $Log$ 
// 