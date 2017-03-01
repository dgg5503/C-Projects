// File: expNode.c 
// Expression defintions.
// @author Douglas Gliner <dgg5503>
// @date 3/10/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros

// Includes
#include "expNode.h"
#include "utils.h"
#include <limits.h>
#include <string.h>
#include <errno.h>

// Construct an expression node dynamically on the heap.
// Assume the following order when determining which
// type to create:
//	1. the operators
//  	2. literal
//	3. integer literal
//	4. symbol
// @param token The next C string token in the expression
// @param left Pointer to left node (NULL if none)
// @param right Pointer to right node (NULL if none)
// @return The new node
ExpNode* makeExpNode(char token[], ExpNode* left, ExpNode* right)
{
	// create on heap!
	ExpNode* expNode;
	if ((expNode = malloc(sizeof(ExpNode))) == NULL)
	{
		ERR("OUT OF MEMORY!\n");
		//return NULL;
		exit(EXIT_FAILURE);
	}

	// init struct
	expNode->type = UNKNOWN;
	expNode->value = (Value) { 0, { 0 } };
	expNode->symbol[0] = '\0';
	expNode->left = left;
	expNode->right = right;
	
	// TODO: WHAT IF UKNOWN?
	// see if operator token (len of 1 always!)
	if (!strcmp(token, ADD_OP_STR)) // +
		expNode->type = ADD_OP;
	else if (!strcmp(token, SUB_OP_STR)) // - 
		expNode->type = SUB_OP;
	else if (!strcmp(token, MUL_OP_STR)) // *
		expNode->type = MUL_OP;
	else if (!strcmp(token, DIV_OP_STR)) // / 
		expNode->type = DIV_OP;
	else if (!strcmp(token, MOD_OP_STR)) // %
		expNode->type = MOD_OP;
	else if (!strcmp(token, ASSIGN_OP_STR)) // =
		expNode->type = ASSIGN_OP;
	else
	{
		// see if double / int 
		// attempt to parse as (int), if that fails, try double
		// if there is a decimal (must be double) OR if > max_int,
		// if this fails, assume symbol
		//long int tmpInt = 0;
		//double tmpDbl = 0.0;

		// number range check, ensure int is within limit
		// TODO: IGNORE MAX INT CHECK?
		
		char *endPtr = NULL;
		long int tmpInt = strtol(token, &endPtr, 10);
		if (strchr(token, '.') == NULL &&
			endPtr != token &&
			tmpInt <= INT_MAX &&
			tmpInt >= INT_MIN &&
			errno != ERANGE)
		{
			expNode->type = INTEGER;
			expNode->value.type = TYPE_INT;
			expNode->value.value.iVal = (int)tmpInt;
		}
		else
		{
			double tmpDbl = strtod(token, &endPtr);
			if (endPtr != token)
			{
				if (errno == ERANGE)
				{
					ERR("ERROR, number out of range!\n");
					FREENULL(expNode);
					//return NULL;
					exit(EXIT_FAILURE);
				}

				expNode->type = DOUBLE;
				expNode->value.type = TYPE_DOUBLE;
				expNode->value.value.dVal = tmpDbl;
			}
			else
			{
				// overflow size check
				if (strlen(token) > MAX_SYM_LENGTH)
				{
					ERR("Symbol too long!\n");
					FREENULL(expNode);
					//return NULL;
					exit(EXIT_FAILURE);
				}

				expNode->type = SYMBOL; // must be a symbol
				strcpy(expNode->symbol, token); // copy it over
			}
		}
	}
	return expNode;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
