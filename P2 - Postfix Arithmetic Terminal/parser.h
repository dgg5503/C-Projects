// File: $Id: parser.h,v 1.7 2016/03/03 16:42:44 csci243 Exp $
// Description: Definitions for the parser.  It takes expressions
// in postfix order, tokenizes them, builds a binary expression
// tree, evaluates the tree, and also converts it into infix
// string form.
// Author: sps

#ifndef PARSER_H
#define PARSER_H

#include "expNode.h"

// The types of errors that can be run into while parsing
// or evaluating the tree
typedef enum ParserError {
	NONE,			// no problems
	TOO_FEW_TOKENS,		// not enough tokens in expression
	TOO_MANY_TOKENS,	// too many tokens in expression
	UNKNOWN_SYMBOL,		// an unknown variable name outside of assignment
	INVALID_ASSIGNMENT,	// assign to left hand side not a variable
	SYMBOL_TABLE_FULL,  	// during assignment to a new symbol, the table is full
	INVALID_MODULUS,     	// tried to do modulus with a double
	DIVISION_BY_ZERO	// trying to divide (or modulus) by 0
} ParserError;

/// The main parse routine that handles the entire parsing
/// process, using the rest of the routines defined here.
/// @param exp The expression as a string
void parse(char exp[]);


/// Constructs the expression tree from the expression.  It
/// must use the stack to order the tokens.  It must also
/// deallocate the memory associated with the stack in all cases.
/// If a symbol is encountered, it should be stored in the node
/// without checking if it is in the symbol table - evaluation will
/// resolve that issue.
/// @param expr the postfix expression as a C string
/// @return the root of the expression tree
/// @exception There are 2 error conditions that you must deal
/// 	with.  In each case, the memory associated with the
///	tree must be cleaned up before returning.  Neither 
///	stops execution of the program. 
///
///	1. If there are too few tokens, set the parser error
///	to TOO_FEW_TOKENS and display the message to standard error:
///
///	Invalid expression, not enough tokens
///
///	2. If there are many tokens (stack is not empty after building),
///	set the parser error to TOO_MANY_TOKENS and display the message
///	to standard error:
///
/// 	Invalid expression, too many tokens
ExpNode* parseTree(char expr[]);

/// Evaluates the tree and returns the result.
/// @param node The node in the tree
/// @precondition:  This routine should not be called if there
/// 	is a parser error.
/// @return the evaluated value.  Note:  A symbol evaluates
///	to its stored value.  
///
///	The evaluator needs to be able to deal with the multiple
///	types our interpreter supports (TYPE_INT and TYPE_DOUBLE).
///
///	1. If a symbol is not in the table, and the table isn't full
///	It should be added to the table taking on the type of the
///	rhs (right hand side) of the assignment.  Note that
///	a symbol will evaluate to its stored value (and type).
///
///	2. When performing the math operations (except for modulus),
///	the following rule applies.  If both lhs and rhs are int's,
///	the result is an int, otherwise the result is a double.
///
/// @exceptions There are 6 error conditions that can occur.  If
///	either occurs, set the correct parser error and display
///	an error message and return a Value of TYPE_UNKNOWN.   The main program
///	should check the parser error state before using the
///	return result.
///
/// 1. If a symbol is referenced on the left hand side during an
/// 	assignment, it should be added to the symbol table with
/// 	the value being the evaluation of the right hand side.  If
/// 	there is no more room in the symbol table, it should set
/// 	a SYMBOL_TABLE_FULL parser error and display the following
/// 	message to standard error
///
/// 	Symbol table full, cannot create new symbol
///
/// 2. An assignment is made to a left hand side expression
///	node that is not a symbol node.  It should set an 
///	INVALID_ASSIGNMENT parser error and display the following
///	message to standard error:
///
///	Invalid assignment
///
/// 3. An assignment is made to a symbol with a value whose
///	type does not match the symbol's type (e.g. assigning
///	a double value to an int symbol).  It should set
///	an INVALID_ASSIGNMENT parser error, and display the following
/// 	message to standard error:
///
///	Assignment type mismatch
///
/// 4. If a math operation is being performed on a symbol that
///	that does not exist, you should set an UKNOWN_SYMBOL
/// 	parser error and display the following message to standard error
///     (where <symbol-name> is the name of the symbol):
///
///	Unknown symbol: <symbol-name>
///
/// 5. If modulus (%) is performed, both left and right hand side nodes
///	must be TYPE_INT.  If this happens, do not perform the operation,
///	instead set the parser error to INVALID_MODULUS and display
/// 	the following error to standard error:
///
///	Modulus requires both types to be int
///
/// 6. If division by zero (or modulus) is being performed, do not do it.  
///     Instead set the parser error to DIVISION_BY_ZERO and display the
///	following error to standard error:
///
///	Division by zero
///
Value evalTree(ExpNode* node);

/// Displays the infix expression for the tree, using
/// parentheses to indicate the precedence, e.g.:
///
/// expression: 10 20 + 30 *
/// infix string: ((10 + 20) * 30) 
///
/// @precondition:  This routine should not be called if there
/// 	is a parser error.
void infixTree(ExpNode* node);

/// Cleans up all dynamic memory associated with the expression tree.
/// @param node The current node in the tree
void cleanupTree(ExpNode* node);

/// Tells the main program whether there was an error with either
/// the parsing or evaluation of the tree.
/// @return parser error
ParserError getParserError(void);

#endif

