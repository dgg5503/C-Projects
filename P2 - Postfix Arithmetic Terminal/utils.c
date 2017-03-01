// File: utils.c 
// a general utility library including some functions I use a bunch.
// @author Douglas Gliner <dgg5503>
// @date 4/05/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros

// Includes
#include "utils.h"

// statics
static ParserError parserError = NONE;

/// Prints nodes relevant information based on its type.
/// @param node the node to print
void printNode(ExpNode* node)
{
	switch (node->type)
	{
	case ADD_OP:
		printf(ADD_OP_STR);
		break;
	case SUB_OP:
		printf(SUB_OP_STR);
		break;
	case MUL_OP:
		printf(MUL_OP_STR);
		break;
	case DIV_OP:
		printf(DIV_OP_STR);
		break;
	case MOD_OP:
		printf(MOD_OP_STR "%"); // simple mod fix
		break;
	case ASSIGN_OP:
		printf(ASSIGN_OP_STR);
		break;
	case DOUBLE:
		printf("%f", node->value.value.dVal);
		break;
	case INTEGER:
		printf("%d", node->value.value.iVal);
		break;
	case SYMBOL:
		printf("%s", node->symbol);
		break;
	default:
		break;
	}
}

/// Sets the error message if current error message is already NONE
/// @param setError the error to set
void setErrorMessage(ParserError setError, char* symbol)
{
	// if none, set error 
	if (parserError == NONE || setError == NONE)
	{
		parserError = setError;

		switch (parserError)
		{
		case NONE:
			break;
		case TOO_FEW_TOKENS:
			ERR("Invalid expression, not enough tokens\n");
			break;
		case TOO_MANY_TOKENS:
			ERR("Invalid expression, too many tokens\n");
			break;
		case UNKNOWN_SYMBOL:
			if (symbol != NULL)
				ERR("Unknown symbol: %s\n", symbol);
			break;
		case INVALID_ASSIGNMENT:
			if (symbol != NULL)
				ERR("%s\n", symbol);
			else
				ERR("Assignment type mismatch\n");
			break;
		case SYMBOL_TABLE_FULL:
			ERR("Symbol table full\n");
			break;
		case INVALID_MODULUS:
			ERR("Modulus requires both types to be int\n");
			break;
		case DIVISION_BY_ZERO:
			ERR("Division by zero\n");
			break;
		default:
			break;
		}
	}
}

/// Get the current parser error.
/// @return returns the parser error.
ParserError getUtilsParserError()
{
	return parserError;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
