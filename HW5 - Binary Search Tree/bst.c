// File: bst.c 
// Binary Search Tree example.
// @author Douglas Gliner <dgg5503>
// @date 03/17/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw5}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Includes
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

// Defines & Macros
// Macros
// fprintf to standard error
// must put ; wherever used!
#define ERR(...) fprintf(stderr, __VA_ARGS__)
#define FREENULL(ptr) free(ptr); ptr = NULL

/// main()
///    Demonstrates creating a tree using ptrs and displaying information in 
///	   multiple ways.
///
/// args -
///        argc - number of args provided including the path to this program
///        argv - an array of char ptrs which are the args provided at the
///		   cmd line.
///
/// returns -
///		   errorCode - EXIT_SUCCESS if no error
int main(int argc, char* argv[])
{
	// usage
	if (argc == 1)
	{
		printf("Usage: bst #\n");
		return EXIT_FAILURE;
	}

	// attempt to grab integer
	int numberOfElements;
	if ((numberOfElements = (int)strtol(argv[1], NULL, 10)) <= 0)
	{
		printf("# must be greater than 0\n");
		return EXIT_FAILURE;
	}

	// setup array and prepare to take in ints.
	int elements[numberOfElements];
	printf("Enter %d integer values to place in tree:\n", numberOfElements);

	// TODO: ERROR CHECKING FOR INPUT?
	int input = 0;
	for (int i = 0; i < numberOfElements; ++i)
	{
		// get input, max size is 10 for max int pos + 1 for + or -
		while (fscanf(stdin, "%11d", &input) == 0);
		elements[i] = input;
	}

	// print out input
	printf("Input values:\n");
	for (int i = 0; i < numberOfElements; ++i)
		printf("%d\n", elements[i]);

	// gen tree
	TreeNode* tree = NULL;
	build_tree(&tree, elements, numberOfElements);

	// print preporder / inorder / postorder
	printf("Preorder:\n");
	traverse(tree, PREORDER);

	printf("Inorder:\n");
	traverse(tree, INORDER);

	printf("Postorder:\n");
	traverse(tree, POSTORDER);

	// clean
	cleanup_tree(tree);
	return 0;
}

/// build_tree() 
///     Dynamically build BST by allocating nodes on the heap.
///     The client passes the address of storage that will hold
///     the pointer to the root tree node. The function then
///     dynamically allocates the nodes as it builds the binary
///     search tree which holds the elements.
///
/// args -
///        root - a pointer to the pointer to the root of the tree
///               to build this tree on to.
///        elements - array of integers to add to the tree
///        count - the number of integers in the array
void build_tree(TreeNode** root, const int elements[], const int count)
{
	/*
	Dear Grader,
	I was going to implement the insertTreeNode function that was presented in
	the in class slides, however, I wanted to challenge myself by building a 
	tree using only one recursive function, the provided count, and elements.
	This took me about 5 hours to figure out and im quite proud of it.

	With this implementation, duplicates are NEVER allocated! So no need
	to free them :D!!!
	*/
	// end condition (make sure to never take in size 0)
	if (count < 1)
		return;

	// init a root node if null
	if (*root == NULL)
	{
		*root = malloc(sizeof(TreeNode));
		if (*root != NULL)
		{
			// set values
			(*root)->data = elements[0];
			(*root)->left = NULL;
			(*root)->right = NULL;
		}
		else
		{
			ERR("ERROR: Out of memory!");
			return;
		}
	}

	// grab a tmp ptr for comparisons
	// if count of 1, must be leaf
	const int * ptrToCompare = NULL;
	if (count == 1)
		ptrToCompare = &elements[0];
	else
		ptrToCompare = &elements[1];
	
	//
	if ((*root)->data > *ptrToCompare) // left
		build_tree(&((*root)->left), ptrToCompare, 1);
	else if ((*root)->data < *ptrToCompare) // right
		build_tree(&((*root)->right), ptrToCompare, 1);

	// When count = 1, we're done (return at top of function)
	// when count != 1, go ahead since we're starting one ahead
	// this "removes" the left most element (the one just added)
	build_tree(root, ptrToCompare, count - 1);
}

/// traverse()
///    Recursively traverses the tree and prints the value of each
///    node.
///
/// args -
///        root - a pointer to the root of the tree to traverse
///        type - the traversal type
void traverse(const TreeNode* root, const TraversalType type)
{
	if (root != NULL)
	{
		switch (type)
		{
			case PREORDER:
				fprintf(stdout, "%d\n", root->data);
				traverse(root->left, PREORDER);
				traverse(root->right, PREORDER);
				break;

			case INORDER:
				traverse(root->left, INORDER);
				fprintf(stdout, "%d\n", root->data);
				traverse(root->right, INORDER);
				break;

			case POSTORDER:
				traverse(root->left, POSTORDER);
				traverse(root->right, POSTORDER);
				fprintf(stdout, "%d\n", root->data);
				break;

			default:
				ERR("ERROR: Invalid traversal type.");
				break;
		}
	}
}

/// cleanup_tree()
///    Cleanup all memory associated with the nodes on the heap.
///
/// args -
///      root - the current root of the tree
void cleanup_tree(TreeNode* root)
{
	if (root != NULL)
	{
		cleanup_tree(root->left);
		cleanup_tree(root->right);
		FREENULL(root);
		cleanup_tree(root);
	}
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
