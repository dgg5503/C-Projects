// File: racer.c
// a racer who will participate in the big race!
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
#include <unistd.h> // usleep
#include <string.h>
#include <pthread.h>
#include "display.h"
#include "racer.h"

// Macros
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#define FREENULL(ptr) free(ptr); ptr = NULL

// Static variable (present in all threads)
static long waitTime = DEFAULT_WAIT;

// mutex lock for display
pthread_mutex_t displayLock = PTHREAD_MUTEX_INITIALIZER;

//const static char emptyReplacement[MAX_CAR_LEN] = { ' ' };

/// initRacers - Do setup work for all racers at the start of the program.
/// @param milliseconds length of pause between steps in animation 
///
void initRacers(long milliseconds)
{
	waitTime = milliseconds;
}

/// makeRacer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param position the row in which to race
/// @return Racer pointer a dynamically allocated Racer object
/// @pre strlen( name ) < MAX_NAME_LEN, for display reasons.
///
Racer *makeRacer(char *name, int position)
{
	// precondition
	unsigned int nameLen = strlen(name);
	if (nameLen > MAX_NAME_LEN)
		return NULL;
	//assert(nameLen <= MAX_NAME_LEN);

	// Allocate a racer
	Racer* racer = malloc(sizeof(Racer));
	assert(racer != NULL);

	// init its information
	racer->dist = 0;
	racer->row = position;

	// setup racer information so we dont have to calc length every update.
	// malloc MAX_CAR_LEN + 1 chars (for null byte)
	char* onscreenName = calloc(MAX_CAR_LEN + 1, sizeof(char));
	assert(onscreenName != NULL);

	// set up the on screen name
	onscreenName[0] = '~';
	onscreenName[1] = 'O';
	onscreenName[2] = '=';

	// copy name
	strcpy(&onscreenName[3], name);

	// fill with - based on max car len.
	unsigned int dashEnd = (MAX_NAME_LEN - nameLen) + nameLen + 3;
	unsigned int i = nameLen + 3;
	for (; i < dashEnd; ++i)
		onscreenName[i] = '-';

	// create header
	//unsigned int endLength = nameLen + dashEnd;
	onscreenName[i] = '-';
	onscreenName[i + 1] = 'o';
	onscreenName[i + 2] = '>';
	onscreenName[i + 3] = '\0';
	
	// assign name
	racer->graphic = onscreenName;

	// return our racer!!
	return racer;
}

/// destroyRacer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated
///
void destroyRacer(Racer *racer)
{
	// deallocate racer info
	FREENULL(racer->graphic);
	FREENULL(racer);
}

/// Run one racer in the race.
/// Initialize the display of the racer*:
///   The racer starts at the start position, column 1.
///   The racer's graphic (text name ) is displayed.
///
/// These actions happen repetitively, until its position is at FINISH_LINE:
///
///   Calculate a random waiting period between 0 and 
///     the initial delay value given to initRacers at the start of the race.
///   Sleep for that length of time.
///   If the sleep value is less than 3, racer gets a flat and cannot finish.
///     A flat tire is displayed by 'X' in the second character of the graphic.
///     Display the car with the flat tire and stop further racing.
///   Change the display position of this racer by +1 column*:
///     Erase the racer's car and name from the display.
///     Update the racer's dist field by +1.
///     Display the racer's graphic (car and name) at the new position.
///
/// The intention is to execute this function many times simultaneously,
/// each in its own thread.
///
/// Note: Be sure to keep the update of the display by one racer "atomic".
///
/// @pre racer cannot be NULL.
/// @param racer Racer object declared as void* for pthread compatibility
/// @return void pointer to status. A NULL represents success.
///
void *run(void *racer)
{
	// precondition
	assert(racer != NULL);
	
	// cast and store ref
	Racer* currRacer = (Racer*)racer;
	
	// store calc'd random
	long randomWait = 0;

	// print at starting point
	// MUTEX LOCK
	pthread_mutex_lock(&displayLock);
	set_cur_pos(currRacer->row + 1, 0);
	printf("%s\n", currRacer->graphic);
	pthread_mutex_unlock(&displayLock);

	// while not < 3 and not at finish line
	while ((currRacer->dist) < FINISH_LINE)
	{
		// calc a random wait
		// 0 - waitTime - 1 (add 1????) TODO
		randomWait = rand() % waitTime;

		// increment dist by 1
		(currRacer->dist)++;

		// check sleep value
		if (randomWait <= 3)
		{
			// set car to broken and RETURN
			currRacer->graphic[1] = 'X';

			// MUTEX LOCK
			pthread_mutex_lock(&displayLock);
			set_cur_pos(currRacer->row + 1, currRacer->dist);
			printf("%s\n", currRacer->graphic);
			pthread_mutex_unlock(&displayLock);
			pthread_exit(NULL);
		}

		// MUTEX LOCK
		pthread_mutex_lock(&displayLock);
		set_cur_pos(currRacer->row + 1, currRacer->dist);

		// "erase"
		put(' ');

		// reprint
		printf("%s\n", currRacer->graphic);
		pthread_mutex_unlock(&displayLock);

		// sleep
		usleep(randomWait * 1000);
	} 

	pthread_exit(NULL);
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
