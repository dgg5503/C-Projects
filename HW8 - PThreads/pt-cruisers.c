// File: pt-cruisers.c
// Main program where the race will be executed.
// @author Douglas Gliner <dgg5503>
// @date 04/29/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw8}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Pre-Reqs
#define _BSD_SOURCE

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "display.h"
#include "racer.h"

// Macros
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#define FREENULL(ptr) free(ptr); ptr = NULL

/// Execute the race depending on the number of names of cars given.
/// Also pay attention to optional parameter of speed!
///
/// @param argc  number of arguments provided
/// @param argv  arguments in string form, expecting one file provided.
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
///
int main(int argc, char* argv[])
{
	// set seed
	srand(time(NULL));

	// usage
	if (argc == 1)
	{
		ERR("Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
		return EXIT_FAILURE;
	}

	// see if first string can be parsed as a float, truncate after
	int offset = 1;
	double maxSpeedDelay;
	char* endPtr;
	if ((maxSpeedDelay = strtod(argv[1], &endPtr)) > 0 &&
		*endPtr == '\0')
	{
		assert(maxSpeedDelay >= 0);
		initRacers((long)maxSpeedDelay);
		offset++;
	}

	// ensure at least 2 racers
	unsigned int numOfRacers = argc - offset;
	if (numOfRacers < 2)
	{
		ERR("Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
		return EXIT_FAILURE;
	}

	// allocate racer ptr array
	Racer** racers = calloc(numOfRacers, sizeof(Racer));
	assert(racers != NULL);

	// check at last spot forward.
	for (unsigned int i = 0; i < numOfRacers; ++i)
	{
		// make sure none of the strlens are > 6 chars
		if ((racers[i] = makeRacer(argv[i + offset], i)) == NULL)
		{
			// free all racers
			for (unsigned int z = i; z; z--)
				destroyRacer(racers[z - 1]);
			FREENULL(racers);
			ERR("Error: racer names must not exceed length 6.\n");
			return EXIT_FAILURE;
		}
	}

	// clear terminal
	clear();

	// BEGIN THREADING STUFF
	pthread_t* threadIDS = calloc(numOfRacers, sizeof(pthread_t));
	assert(threadIDS != NULL);
	void *exitStatus;

	// malloc thread id array
	//threadIDS = calloc(numOfRacers, sizeof(pthread_t));

	// create threads and look to join them
	for (int i = numOfRacers; i; i--)
		pthread_create(&threadIDS[i - 1], NULL, run, racers[i - 1]);
	
	for (int i = numOfRacers; i; i--)
		pthread_join(threadIDS[i - 1], &exitStatus);

	// process racers here
	for (int i = numOfRacers; i; i--)
		destroyRacer(racers[i - 1]);
	FREENULL(racers);
	FREENULL(threadIDS);

	// new line for neatness
	printf("\n");

	return EXIT_SUCCESS;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
