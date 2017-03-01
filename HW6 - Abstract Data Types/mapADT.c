// File: mapADT.c 
// TODO
// @author Douglas Gliner <dgg5503>
// @date 4/12/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw6}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros
#define _MAP_IMPL_
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#define FREENULL(ptr) free(ptr); ptr = NULL

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// KeyValuePair struct holds a key, a value and the next ptr in the
// linked list. I will be using a linked list for this implementation
// since hashing functions looked a little too complicated...
typedef struct KeyValuePair {
	void* key;
	void* value;
	struct KeyValuePair* next;
} KeyValuePair;

// MapADT split into a MapStruct and its pointer such that we can get
// the sizeof the map struct instead of getting just the size of a 
// pointer!
typedef struct {
	// list of KVPs
	KeyValuePair* kvpList;

	// function ptr for comparisons
	bool(*equals)(const void *a, const void *b);
} MapStruct, *MapADT;

#include "mapADT.h"

// Helper functions
/// Add a key and its associated value to the given "list."
/// 
/// @param key the key to associate with this KeyValuePair
/// @param value the value to associate with this KeyValuePair and the provided
///        key.
/// @param next a pointer to the next item in the "list."
/// @return a pointer to a KeyValuePair containing the provided key and value
KeyValuePair* makeKeyValuePairNode(void* key, void* value)
{
	// malloc kvp node
	KeyValuePair* kvpNode = malloc(sizeof(KeyValuePair));

	// ensure not null
	if (kvpNode == NULL)
	{
		ERR("ERROR: Failed to create kvp node, out of memory!\n");
		exit(EXIT_FAILURE);
	}

	// set next, key and value
	kvpNode->next = NULL;
	kvpNode->key = key;
	kvpNode->value = value;

	// return kvp
	return kvpNode;
}

/// Add a key and its associated value to the given "list."
/// 
/// @param list the address to the pointer of the lists head.
/// @param key the key to add to the map
/// @param value the value to add to the map associated with the given key
void add(KeyValuePair** list, void* key, void* value)
{
	// add head and move back.
	// although this is the same as creating a stack, it does not
	// matter since the way we retrieve items will be via search.
	// Order doesnt matter
	/*
	if (*list == NULL)
		*list = makeKeyValuePairNode(key, value, NULL);
	else
		*list = makeKeyValuePairNode(key, value, *list);
	*/

	// build list, this will preserve order.
	// null list check
	if (*list == NULL)
	{
		*list = makeKeyValuePairNode(key, value);
	}
	else
	{
		// find next node with null next
		KeyValuePair* prevPtr = *list;
		KeyValuePair* nextPtr;
		while ((nextPtr = prevPtr->next) != NULL)
			prevPtr = nextPtr;

		// set that next to new value
		prevPtr->next = makeKeyValuePairNode(key, value);
	}
}

/// Free all keys, values and KeyValuePair structs associated with the given
/// "list."
/// 
/// @param head the address to the pointer of the lists head.
void freeList(KeyValuePair** head)
{
	// null check
	if (*head == NULL)
		return;

	// if there is a next, go to it and free
	// then set next to null
	if ((*head)->next != NULL)
		freeList(&(*head)->next);

	// as we're moving back
	// free key, value
	// next will be freed implicitly since we're
	// looking at a pointer to the structs ptr.
	FREENULL((*head)->key);
	FREENULL((*head)->value);

	// finally free the head
	FREENULL((*head));
}

/// Loops through the "list" and checks to see if a matching
/// key exists.
/// 
/// @param head the head of the list
/// @param key the key to search for
/// @param equals the address of the equals function to be used for
///    determining if two keys are equal
/// @return a pointer to a KeyValuePair containing the given key if found
///	   otherwise, return NULL if nothing found.
KeyValuePair* containsKey(KeyValuePair* head,
	void* key,
	bool(*equals)(const void *a, const void *b))
{
	// see if we reached end of list
	if (head == NULL)
		return NULL;
	else
	{
		// head isnt null, do the check!
		// make sure key isnt null either
		if (head->key != NULL
			&& equals(head->key, key))
			return head;
	}

	// recurse if both conditions havent been met.
	// when head->next is NULL, we'll know next call
	// that we're at the end of the list.
	return containsKey(head->next, key, equals);
}

/// Loops through the "list" and checks to see if a matching
/// key exists. If one exists, this function will remove that entry and return
/// the value from within that entry.
/// 
/// @param list the address to the pointer of the lists head.
/// @param key the key to remove
/// @param equals the address of the equals function to be used for
///    determining if two keys are equal
/// @return a pointer to the original value held within the removed entry
void* RemoveKey(KeyValuePair** list,
	void* key,
	bool(*equals)(const void *a, const void *b))
{
	// null check
	if (*list == NULL)
		assert("Cant remove a key from NULL list!\n");

	// top check
	if (equals((*list)->key, key))
	{
		KeyValuePair* tmpPtr = *list;
		void* oldValue = (*list)->value;
		*list = (*list)->next;
		FREENULL(tmpPtr->key);
		FREENULL(tmpPtr);
		return oldValue;
	}
	
	// find next node with null next
	KeyValuePair* prevPtr = *list;
	KeyValuePair* currPtr = (*list)->next;
	while (currPtr != NULL &&
		!equals(currPtr->key, key))
	{
		prevPtr = currPtr;
		currPtr = currPtr->next;
	}

	// ensure keys match
	//assert(!equals(key, nextPtr->key));
	//if (currPtr != NULL)
	//	printf("Rem KEY: %d\nVAL: %d\n", currPtr->key, currPtr->value);
	//assert(currPtr != NULL);

	if (currPtr == NULL)
		return NULL;

	// TODO: THIS!!
	// remove the entire entry!!
	// get one before
	// get one after
	// set one before->next after
	// freenull kvpToRemove->key
	// freenull kvpToRemove
	prevPtr->next = currPtr->next;
	
	void* oldValue = currPtr->value;

	// free next ptr
	FREENULL(currPtr->key);
	FREENULL(currPtr);

	// return old value
	return oldValue;
}

// Required Functions
/// Create a MapADT which uses the supplied function as a equals
/// routine.
///
/// The comparison function takes two void* parameters, and returns
/// a bool result which indicates if the two things are identical: 
///
///	Result	Relationship
///	======	===========
///	 true	a == b
///	 false	a != b
///
/// @param equals the address of the equals function to be used for
///    determining if two keys are equal
/// @return a pointer to a MapADT, or NULL if the allocation fails
MapADT map_create(bool(*equals)(const void *a, const void *b))
{
	// Create mapADT ptr based off of the struct size
	MapADT mapADT = malloc(sizeof(MapStruct));

	// null check
	if (mapADT == NULL)
		return NULL;

	// set values
	mapADT->kvpList = NULL;
	mapADT->equals = equals;

	// return
	return mapADT;
}

/// Tear down and deallocate the supplied MapADT.
///
/// @param map - the MapADT to be manipulated
void map_destroy(MapADT map)
{
	// remove all nodes
	freeList(&map->kvpList);

	// deallloc map
	FREENULL(map);
}

/// Remove all contents from the supplied MapADT.
///
/// @param map - the MapADT to be manipuated
void map_clear(MapADT map)
{
	// remove all nodes
	freeList(&map->kvpList);
}

/// Check if the specified entry exists in the map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then return
/// true.
///
/// @param key the key to lookup in the map
/// @return true if the entry exists
bool map_contains(MapADT map, void *key)
{
	if (containsKey(map->kvpList, key, map->equals) != NULL)
		return true;
	return false;
}

/// Put the specified entry into the Map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then replace it 
/// and return a pointer to the old value.
///
/// @param map the MapADT into which the value is to be inserted
/// @param key the key to be inserted
/// @param value the value to be inserted
/// @exception If the value cannot be inserted into the map for
///     whatever reason (usually implementation-dependent), the program
///     should terminate with an error message.  This can be done by
///     printing an appropriate message to the standard error output and
///     then exiting with EXIT_FAILURE, or by having an assert() fail.
void* map_put(MapADT map, void *key, void *value)
{
	// Traverse head -> tail looking to see if keys match
		// if keys DO match, stop and replace value and return
	// If reach the end of list and keys DONT match, add to end of list
		// return ptr

	KeyValuePair* dupeSearch = containsKey(map->kvpList, key, map->equals);

	// found a dupe, just replace the value
	if (dupeSearch != NULL)
	{
		FREENULL(key);
		void* oldValue = dupeSearch->value;
		dupeSearch->value = value;
		return oldValue;
	}

	// if the search was null, just add new key to list
	add(&map->kvpList, key, value);

	return NULL;
}

/// Find the entry specified by the key and return a pointer to the value
///
/// @param map the MapADT to be manipulated
/// @return the value that corresponds to the key
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
const void *map_get(MapADT map, void *key)
{
	// empty check
	if (map_empty(map))
	{
		//FREENULL(key);
		ERR("ERROR: Provided map is empty\n");
		exit(EXIT_FAILURE);
	}

	// make sure not invalid!
	KeyValuePair* foundKey = containsKey(map->kvpList, key, map->equals);
	if (foundKey == NULL)
	{
		//FREENULL(key);
		//ERR("ERROR: Provided key does not exist GET!\n");
		//exit(EXIT_FAILURE);
		return NULL;
	}

	// get data with given key
	return foundKey->value;
}

/// Find the entry specified by the key and remove it from the MapADT,
/// returning the value pointer. 
///
/// @param map the MapADT to be manipulated
/// @return the value that was removed from the map
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
void *map_delete(MapADT map, void *key)
{
	// empty check
	if (map_empty(map))
	{
		//FREENULL(key);
		ERR("ERROR: Provided map is empty!\n");
		exit(EXIT_FAILURE);
	}

	// find node to remove
	//KeyValuePair* kvpToRemove = containsKey(map->kvpList, key, map->equals);

	// key not found, assert
	//assert(NULL != kvpToRemove);
	
	/*
	if (kvpToRemove == NULL)
	{
		//FREENULL(key);
		ERR("ERROR: Provided key does not exist DELETE!\n");
		exit(EXIT_FAILURE);
		//return NULL;
	}
	*/

	// save value, set empty, and return
	//void* oldValue = kvpToRemove->value;
	//kvpToRemove->value = NULL;

	// TODO: THIS!!
	// remove the entire entry!!
	// get one before
	// get one after
	// set one before->next after
	// freenull kvpToRemove->key
	// freenull kvpToRemove
	return RemoveKey(&map->kvpList, key, map->equals);
}

/// Indicate whether or not the supplied Map is empty
///
/// @param the MapADT to be tested
/// @return true if the map is empty, otherwise false
bool map_empty(MapADT map)
{
	if (map == NULL ||
		map->kvpList == NULL)
		return true;
	return false;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
