// Description:  test driver for the MapADT module
//
// This test program creates three maps (one using int keys, 
// one using unsigned int keys, and one using C String keys),
// inserts a number of long values into each map, and then
// removes and prints them.
//
// Author: ckd 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "mapADT.h"

//
// Sequence of test values to be inserted and removed
//

long int testData[] = { 42, 17, -12, 9982, 476, 2912, -22, 3291213, 7782 };
int numData = sizeof(testData) / sizeof(long int);
//
// Equals function for C string keys
//

bool equals_str( const void *a, const void *b ) {
	char *key1, *key2;
	
	key1 = (char*)a;
	key2 = (char*)b;

	return (strcmp(key1, key2) == 0);
}

//
// Process insertions/removals using a specific map
//
void process( MapADT map) {
	int i;
	char *key;
	long int *value;
        char *keys[] = {"apple", "banana", "cat", "dog", "red", "blue",
           "green", "water", "table"};

	fputs( "Putting: ", stdout );
	for( i = 0; i < numData; ++i ) {
		key = (char*)malloc(strlen(keys[i])+1);
		assert( NULL != key );
		strncpy( key, keys[i], strlen(keys[i])+1);	
		value = (long int*)malloc(sizeof(long int));
		assert( NULL != value );
		*value = testData[i];
		printf( "[%s, %ld] ", key, *value );
		map_put( map, (void*)key, (void *)value );
	}

	map_clear( map );
	fputs( "Clearing map\n\n", stdout );

	fputs( "Putting: ", stdout );
	for( i = 0; i < numData; ++i ) {
		key = (char*)malloc(strlen(keys[i])+1);
		assert( NULL != key );
		strncpy( key, keys[i], strlen(keys[i])+1);	
		value = (long int*)malloc(sizeof(long int));
		assert( NULL != value );
		*value = testData[i];
		printf( "[%s, %ld] ", key, *value );
		map_put( map, (void*)key, (void *)value );
	}

	fputs( "\nPutting apple\n", stdout );
	key = (char*)malloc(strlen("apple")+1);
	strncpy(key, "apple", strlen("apple")+1);
	value = (long int*)malloc(sizeof(long int));
	*value = 1;
	value = map_put( map, (void*)key, (void*)value );
	assert( NULL != value );
	printf("Old value of apple: %ld\n", *value);
	free(value);

	fputs( "\nPutting apple\n", stdout );
	key = (char*)malloc(strlen("apple")+1);
	strncpy(key, "apple", strlen("apple")+1);
	value = (long int*)malloc(sizeof(long int));
	*value = 250;
	value = map_put( map, (void*)key, (void*)value );
	assert( NULL != value );
	printf("Old value of apple: %ld\n", *value);
	free(value);
		
	fputs( "\nPutting cat\n", stdout );
	key = (char*)malloc(strlen("cat")+1);
	strncpy(key, "cat", strlen("cat")+1);
	value = (long int*)malloc(sizeof(long int));
	*value = 255;
	value = map_put( map, (void*)key, (void*)value );
	assert( NULL != value );
	printf("Old value of cat: %ld\n", *value);
	free(value);	
	
	fputs( "\nPutting chair\n", stdout );
	key = (char*)malloc(strlen("chair")+1);
	strncpy(key, "chair", strlen("chair")+1);
	value = (long int*)malloc(sizeof(long int));
	*value = 255;
	value = map_put( map, (void*)key, (void*)value );
	assert( NULL == value );
	printf("chair was not previously in the map\n");
}


int main( void ) {
	MapADT myMap;

	myMap = map_create( equals_str );
	if( myMap == NULL ) {
		fputs( "Cannot create map with C string keys\n", stderr );
		exit( EXIT_FAILURE );
	}
	puts( "\nTesting the [C string, long int] map" );
	process( myMap );
	map_destroy( myMap );

	return 0;
}

