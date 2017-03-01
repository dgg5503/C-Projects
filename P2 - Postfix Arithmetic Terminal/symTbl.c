// File: interp.c 
// Definition of a symbol table
// @author Douglas Gliner <dgg5503>
// @date 3/10/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 
// Includes
#include <string.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include "utils.h"
#include "symTbl.h"

// Defines and Macros
// max length is
// TYPE SYM NUM.NUM
// Longest type is double (6chars)
// SPACE
// MAX_SYM_LENGTH (including null terminator so no space needed)
// SPACE (excluded)
// MAX_DEC
// DOT
// MAX_DIG
// null terminator
// TODO: test extreme cases
// FIX DBL_DIG and DBL_DECIMAL_DIG???
// 9+MAX_SYM_LENGTH+17+15
#define MAX_LINE_LEN 128
#define DOUBLE_STR "double"
#define INT_STR "int"

// Statics
// Sym tab pointer array, init all as NULL
static Symbol symTab[MAX_SYMBOLS];

/// Constructs the table by reading the file.  The format is
/// one symbol per line in the format:
///
///	variable-type variable-name	variable-value
///	...
///
/// For this problem we guarantee the variable-names are all
/// unique, and the format of the file is error free.
/// You are allowed to create it statically or
/// dynamically and store it locally.
/// @param filename The name of the file containing the symbols
/// @exception If the file can't be opened, an error message should
/// be displayed to standard error and the program should exit
/// with EXIT_FAILURE.  If no file is specified, an empty table
/// should be built (there is no message or error/exit in this case).
///
/// Error loading symbol table
///
void buildTable(char filename[])
{
	// initialize all structs with unknown type.
	for (int i = 0; i < MAX_SYMBOLS; ++i)
		symTab[i].value.type = TYPE_UNKNOWN;

	// no file specified check
	// init table and stop
	if (strlen(filename) == 0)
		return; 

	// TODO verify valid file format
	// TODO verify valid file path length

	// file open failed check
	FILE* filePtr = NULL;
	if ((filePtr = fopen(filename, "r")) != NULL)
	{
		// iterate through every line in the file and then add to symtab
		// { varName, { Type, Value } }
		//Symbol symTest = { "LOL", { TYPE_INT, 10 } 
		
		char fileLine[MAX_LINE_LEN] = { '\0' };

		// ptr to variable ptrs after splitting
		char* variables[3] = { NULL };

		// tmp symbol type variable
		Type symbolType = TYPE_UNKNOWN;

		// tmp int for symbol length
		int symbolLen = 0;


		// TODO: max line number?
		// keep going until EOF
		while (fgets(fileLine, MAX_LINE_LEN, filePtr) != NULL) // was -1
		{
			// split at space
			// TODO: max # of splits?
			variables[0] = strtok(fileLine, " ");

			//printf("ARG 0: %s\n", variables[0]);

			for (int i = 1; i < 3; ++i)
			{
				variables[i] = strtok(NULL, " ");
				//printf("ARG %d: %s\n", i, variables[i]);
			}

			// attempt to create symbol.
			// ensure valid type
			if (strcmp(variables[0], DOUBLE_STR) == 0) // double!
				symbolType = TYPE_DOUBLE;
			else if (strcmp(variables[0], INT_STR) == 0) // int!
				symbolType = TYPE_INT;
			else
			{
				ERR("Symbol type %s doesnt exist!\n", variables[0]);
				fclose(filePtr);
				exit(EXIT_FAILURE);
				//symbolType = TYPE_UNKNOWN; // UNKNOWN :O!
			}


			// ensure symbol size is correct
			symbolLen = strlen(variables[1]);
			if (symbolLen != 0 && symbolLen < MAX_SYM_LENGTH)
			{
				// init the temp symbol
				Symbol tmpSymbol = { .value = { .type = symbolType } };

				// strcpy variable name
				strcpy(tmpSymbol.varName, variables[1]);

				char *endPtr = NULL;

				// perform # specific case checks
				switch (symbolType)
				{
					
					case TYPE_DOUBLE:
						// attempt to convert to double and set val
						// TODO: error checking?
						tmpSymbol.value.value.dVal =
							strtod(variables[2], &endPtr);
						if (endPtr == variables[2])
							ERR("Invalid assignment\n");
						break;

					case TYPE_INT:
						// attempt to convert to int.
						// cast to int since long ints COULD 
							// be larger than an int BUT
						// an int is always guaranteed to 
							// have less space then a long int
						// TODO COME BACK TO THIS...
						tmpSymbol.value.value.iVal =
							(int)strtol(variables[2], &endPtr, 10);
						if (endPtr == variables[2]) // was 0L
							ERR("Invalid assignment\n");
						break;

					default:
						break;
				}

				// add to symbol table!
				if (!addTable(tmpSymbol))
					ERR("Symbol table full\n");
			}
			else
			{
				// TODO: EXIT OR JUST ERROR?
				ERR("Couldnt load symbol %s, it's too long!\n", variables[1]);
				fclose(filePtr);
				exit(EXIT_FAILURE);
			}
		}
		fclose(filePtr);
	}
	else
	{
		ERR("Error loading symbol table\n");
		exit(EXIT_FAILURE);
	}
}

/// Displays the contents of the symbol table in the following format:
///
/// SYMBOL TABLE:
///	Type: variable-type Name: variable-name, Value: variable-value
///	...
///
/// Each symbol should be printed one per line, tab-indented.
void dumpTable(void)
{
	printf("SYMBOL TABLE: \n");

	for (int i = 0; i < MAX_SYMBOLS; ++i)
	{
		// grab a ptr to value
		Symbol* symbolPtr = &symTab[i];

		// check for type 
		switch (symbolPtr->value.type)
		{
			case TYPE_DOUBLE:
				printf("\tType: double, Name: %s, Value: %.6f\n",
					symbolPtr->varName,
					symbolPtr->value.value.dVal);
				break;

			case TYPE_INT:
				printf("\tType: int, Name: %s, Value: %d\n",
					symbolPtr->varName,
					symbolPtr->value.value.iVal);
				break;

			default:
				break;
		}
	}
}

/// Returns the symbol associated with variable name
/// @param symbol The name of the variable
/// @return The Symbol, if it exists, or NULL otherwise
Symbol* lookupTable(char variable[])
{
	// find the variable
	for (int i = 0; i < MAX_SYMBOLS; ++i)
	{
		if (!strcmp(symTab[i].varName, variable))
		{
			return &symTab[i];
		}
	}
	return NULL;
}

/// Attempt to add a new symbol to the table (if it is not
/// already full).
/// @param Symbol symbol the new symbol to add
/// @return 0 if the symbol could not be added, anything else otherwise
int addTable(Symbol symbol)
{
	// ensure sym length is less than the max and not 0
	int symbolVarLen = strlen(symbol.varName);
	if (symbolVarLen > MAX_SYM_LENGTH || symbolVarLen <= 0)
		return 0;

	// find next empty spot in symTab
	for (int i = 0; i < MAX_SYMBOLS; ++i)
	{
		// check to see if the type is unknown
		// that means its empty!
		if (symTab[i].value.type == TYPE_UNKNOWN)
		{
			// set value (symbol is a copy)
			symTab[i] = symbol;
			return 1;
		}
	}

	// sym tab full!
	return 0;
}

/// Destroys the symbol table (if you built it dynamically).
void destroyTable()
{

}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
