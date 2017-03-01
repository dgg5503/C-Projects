// File: interp.c 
// Interpreter main program file
// @author Douglas Gliner <dgg5503>
// @date 3/10/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes
#include <string.h>
#include "symTbl.h"
#include "utils.h"
#include "stack.h"
#include "interp.h"
#include "parser.h"
#include "expNode.h"

/// Interprets commands and processes them according to predefined actions
///
/// @param argc  number of commands entered
/// @param argv  supplied commands entered + location of executable
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
///
int main(int argc, char* argv[])
{
	// opt arg check
	if (argc > 2)
	{
		ERR("Usage: interp [sym-table]\n");
		return EXIT_FAILURE;
	}

	// if argc is 2, assume 1st arg is path to symtab and try to load!
	if (argc == 2)
	{
		// probably a huge security flaw?
		buildTable(argv[1]);
	}
	else
	{
		// pass no file and init empty table
		buildTable("");
	}	

	//int ch;
	char inputBuffer[MAX_LINE] = { '\0' }; // init entire buffer as null char

	dumpTable();

	// begin print start info
	printf("Enter postfix expressions (CTRL-D to exit):\n");
	printf("> ");

	// ctrl D is EOF!
	while (fgets(inputBuffer, MAX_LINE, stdin) != NULL)
	{
		// rid of \n
		inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

		// print table?
		if (inputBuffer[0] == '\0')
			dumpTable();
		else
			parse(inputBuffer);

		// error checks
		/*
		switch (getParserError())
		{
		
		}
		*/

		// cursor...
		printf("> ");
	}
	
	// dump table at end
	printf("\n");
	dumpTable();

	return 0;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
