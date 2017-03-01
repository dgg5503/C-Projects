// File: wildfire.c 
// simulates a wildfire based off of Shiflets description and
// associated modifications according to the prompt
// @author Douglas Gliner <dgg5503>
// @date 02/17/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project1}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros
#define _BSD_SOURCE

// Includes
#include <getopt.h> // getopt
#include <unistd.h> // usleep

#include "forest.h"
#include "display.h"
#include "utils.h"

/// Performs a simulation of Shiflets modified wildfire according to RIT CS
/// dept.
///
/// @param argc  number of grades entered, including program name
/// @param argv  supplied grades, including program name
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
///
int main(int argc, char* argv[])
{
	// set default print mode to inplace
	enum PrintMode printMode = INPLACE;

	// check to see if optinal arg exists.
	// - if the optional character exists, function will return the ascii
	//	 code of that character.
	// - double : to declare as optional.
	// - while loop until we go through all args in the array
	int opt;
	int numOfCycles = -1;

	while ((opt = getopt(argc, argv, "p::")) != -1)
	{
		switch (opt)
		{
			case 'p':
				// found our number!
				numOfCycles = str_to_int(optarg);
				printMode = SEQUENTIAL;

				// check for no value.
				if (numOfCycles == 0)
				{
					ERR("The -pN option was invalid.\n");
					return EXIT_FAILURE;
				}
				else if (numOfCycles < 0) // would this ever happen?
				{
					ERR("The -pN option was negative.\n");
					return EXIT_FAILURE;
				}
				break;

			default:
				// unknown error!!!
				// IS THIS SUPPOSE TO BE HERE???
				ERR("The - pN option was invalid.\n");
				return EXIT_FAILURE;
				break;
		}
	}

	// no args generic output
	if (argc - optind < 4)
	{
		usage();
		return EXIT_FAILURE;
	}

	// -- arg array format --
	// size [5, 40]
	// prob [0, 100]
	// dens [0, 100]
	// prop [0, 100]
	// hold these in variables only after we've parsed.
	// output error to stderr
	// keep these as int since rand() is going to return int anyway.
	int size;

	// init struct for passing of const args
	struct WildFireArgs wfArgs = { 0, 0, 0, 0, 0, 0 };

	// we can increment here since we've already done the checks for correct
	// # of args. Optind is set via getopt.c as the last index to not contain an
	// optional arg.
	size = str_to_int(argv[optind++]);
	if (size < 5 || size > 40)
	{
		ERR("The size (%i) must be an integer in [5...40].\n",
			size);

		usage();
		return EXIT_FAILURE;
	}
	
	wfArgs.probability = str_to_int(argv[optind++]);
	if (wfArgs.probability < 0 || wfArgs.probability > 100)
	{
		ERR("The probability (%i) must be an integer in [0...100].\n",
			(int)wfArgs.probability);

		usage();
		return EXIT_FAILURE;
	}
	// convert to dec form
	wfArgs.probability /= 100.0;

	wfArgs.treeDensity = str_to_int(argv[optind++]);
	if (wfArgs.treeDensity < 0 || wfArgs.treeDensity > 100)
	{
		ERR("The density (%i) must be an integer in [0...100].\n",
			(int)wfArgs.treeDensity);

		usage();
		return EXIT_FAILURE;
	}
	// convert to dec form
	wfArgs.treeDensity /= 100.0;

	wfArgs.proportionBurning = str_to_int(argv[optind++]);
	if (wfArgs.proportionBurning < 0 || wfArgs.proportionBurning > 100)
	{
		ERR("The proportion (%i) must be an integer in [0...100].\n",
			(int)wfArgs.proportionBurning);

		usage();
		return EXIT_FAILURE;
	}
	// convert to dec form
	wfArgs.proportionBurning /= 100.0;

	// create our forest :D
	char forest[size][size];

	// init all cells as empty
	int x, y;
	for (y = 0; y < size; ++y)
		for (x = 0; x < size; ++x)
			forest[y][x] = EMPTY;

	// place trees according to density
	int i, randX, randY;
	wfArgs.numOfTrees = (size * size) * wfArgs.treeDensity;
	wfArgs.numOfTreesBurning = wfArgs.numOfTrees * wfArgs.proportionBurning;

	for (i = 0; i < wfArgs.numOfTreesBurning; ++i)
	{
		// get a random location thats empty
		while (forest[randY = rand() % size][randX = rand() % size] != EMPTY);

		// place burning tree
		forest[randY][randX] = BURNING_TREE;
	}

	for (i = 0; i < wfArgs.numOfTrees - wfArgs.numOfTreesBurning; ++i)
	{
		// get a random location thats empty
		while (forest[randY = rand() % size][randX = rand() % size] != EMPTY);

		// place tree
		forest[randY][randX] = TREE;
	}

	// keep track of cycle num
	int cycle = 0;

	// clear once only if no given num of cycles.
	if(printMode == INPLACE)
		clear();
	
	// print initial state
	print_matrix(size, forest, printMode);

	// print info
	printf("cycle %i, size %i, probability %.2f, "
		"density %.2f, proportion %.2f, changes %i\n",
		cycle, size, wfArgs.probability,
		wfArgs.treeDensity, wfArgs.proportionBurning, wfArgs.changes);

	// first sleep
	usleep(800000);

	// cum changes var
	int totalChanges = 0;

	// loop until no more burning trees or cycle # reaches num of cycles
	do
	{
		++cycle;

		update(&wfArgs, size, forest);
		
		// recalc proportionBurning
		// TO-DO: DOES NUM OF TREES ALSO DECREASE???
		wfArgs.proportionBurning = (float)wfArgs.numOfTreesBurning
			/ wfArgs.numOfTrees;

		// add changes to total
		totalChanges += wfArgs.changes;

		// print matrix	
		print_matrix(size, forest, printMode);

		// print info
		printf("cycle %i, size %i, probability %.2f, "
			"density %.2f, proportion %.2f, changes %i\n",
			cycle, size, wfArgs.probability,
			wfArgs.treeDensity, wfArgs.proportionBurning, wfArgs.changes);

		// reset changes
		wfArgs.changes = 0;

		// sleep
		usleep(800000);
	} while (wfArgs.numOfTreesBurning > 0 && cycle != numOfCycles);

	// end message
	printf("fires are out after %i cumulative changes.\n", totalChanges);
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
