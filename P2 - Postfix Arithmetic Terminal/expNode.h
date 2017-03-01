// File: $Id: expNode.h,v 1.3 2013/10/17 04:22:27 csci243 Exp $
// Description: Expression defintions.
// Author: sps

#ifndef EXP_NODE_H
#define EXP_NODE_H

#include "expNode.h"
#include "symTbl.h"

// Operator tokens
#define ADD_OP_STR	"+"
#define SUB_OP_STR	"-"
#define MUL_OP_STR	"*"
#define DIV_OP_STR	"/"
#define MOD_OP_STR	"%"
#define ASSIGN_OP_STR	"="

// Valid token types in the expression
typedef enum ExpType {
		ADD_OP,		// addition
		SUB_OP,		// subtraction
		MUL_OP,		// multiplication
		DIV_OP,		// division
		MOD_OP,		// modulus
		ASSIGN_OP,	// assignment
		DOUBLE,     	// constant double literal
		INTEGER,	// constant integer literal
		SYMBOL,		// a variable name
		UNKNOWN		// something else?  Shouldn't have to deal with
} ExpType;

// Represents a single expression node
typedef struct ExpNode {
	ExpType type;			// the type of expression
	Value value;			// the value (DOUBLE/INTEGER/SYMBOL only)
	char symbol[MAX_SYM_LENGTH];	// symbol name (SYMBOL only)
	struct ExpNode* left;		// pointer to left node
	struct ExpNode* right;		// pointer to right node
} ExpNode;

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
ExpNode* makeExpNode(char token[], ExpNode* left, ExpNode* right);

#endif

