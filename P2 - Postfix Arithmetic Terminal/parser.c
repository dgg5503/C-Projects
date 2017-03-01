// File: parser.c 
// It takes expressions
// in postfix order, tokenizes them, builds a binary expression
// tree, evaluates the tree, and also converts it into infix
// @author Douglas Gliner <dgg5503>
// @date 3/10/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Projects/project2}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// Defines and Macros

// Includes
#include <string.h>
#include "utils.h"
#include "stack.h"
#include "parser.h"
#include "valueMath.h"

/// The main parse routine that handles the entire parsing
/// process, using the rest of the routines defined here.
/// @param exp The expression as a string
void parse(char exp[])
{
	// reset parser error
	setErrorMessage(NONE, NULL);
	//parserError = NONE;

	// get root
	ExpNode* root = NULL;
	if ((root = parseTree(exp)) != NULL)
	{
		// eval
		Value value = evalTree(root);

		// ensure no error
		if (getUtilsParserError() == NONE)
		{
			// print results
			infixTree(root);
			printf(" = ");
			switch (value.type)
			{
				case TYPE_INT:
					printf("%d", value.value.iVal);
					break;
				case TYPE_DOUBLE:
					printf("%f", value.value.dVal);
					break;
				default:
					break;
			}
			printf("\n");
		}		

		// always clean tree
		cleanupTree(root);
	}
}

/// Constructs the expression tree recursively.
///
/// @param stack the double ptr to the stack
/// @return the root of the expression tree
ExpNode* recurseTree(StackNode** stack)
{
	ExpNode* rootNode = NULL;
	if (getUtilsParserError() == NONE)
	{
		// grab token
		char* token = top(*stack);
		rootNode = makeExpNode(token, NULL, NULL);
		pop(stack);

		// only left/right if not a num
		if (rootNode->type != DOUBLE &&
			rootNode->type != INTEGER &&
			rootNode->type != SYMBOL)
		{
			// not enough check
			if (emptyStack(*stack))
			{
				//ERR("Invalid expression, not enough tokens\n");
				//parserError = TOO_FEW_TOKENS;
				setErrorMessage(TOO_FEW_TOKENS, NULL);
				return rootNode;
			}

			// recurse right
			rootNode->right = recurseTree(stack);

			// not enough check
			if (emptyStack(*stack))
			{
				//ERR("Invalid expression, not enough tokens\n");
				//parserError = TOO_FEW_TOKENS;
				setErrorMessage(TOO_FEW_TOKENS, NULL);
				return rootNode;
			}

			// recurse left
			rootNode->left = recurseTree(stack);
		}
	}

	return rootNode;
}

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
ExpNode* parseTree(char expr[])
{
	// split at white space
	// create a stack
	StackNode* exprStack = NULL;

	// If strtok is at the end. stop.
	push(&exprStack, strtok(expr, " "));

	// split until null (include null)
	char* tmpSplit = NULL;
	while ((tmpSplit = strtok(NULL, " ")) != NULL)
		push(&exprStack, tmpSplit);

	// construct tree
	ExpNode* root = recurseTree(&exprStack);

	// few token check
	if (getUtilsParserError() == TOO_FEW_TOKENS)
	{
		cleanupTree(root);
		return NULL;
	}

	// too many check
	if (!emptyStack(exprStack))
	{
		//ERR("Invalid expression, too many tokens\n");
		//parserError = TOO_MANY_TOKENS;
		setErrorMessage(TOO_MANY_TOKENS, NULL);
		cleanupTree(root);

		// empty stack
		while (!emptyStack(exprStack))
			pop(&exprStack);

		return NULL;
	}

	return root;
}

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
Value evalTree(ExpNode* node)
{
	// precondition, DOES IT APPLY HERE?
	if(getUtilsParserError() != NONE)
		return (Value) { TYPE_UNKNOWN , { 0 } };

	// left ROOT right
	// node->left node node->right
	// if node->left is not a num, recurse
		// if both l and r are ints, result is int, otherwise dbl
		// look out for UPGRADES or DOWNGRADES (double should never go to int!)
		// (max_int + max_int should go to double?)

	// detect if double or int, then perform function.
	switch (node->type)
	{
		case ADD_OP:
			return addOp(evalTree(node->right), evalTree(node->left));
		case SUB_OP:
			return subOp(evalTree(node->right), evalTree(node->left));
		case MUL_OP:
			return multOp(evalTree(node->right), evalTree(node->left));
		case DIV_OP:
			return divOp(evalTree(node->right), evalTree(node->left));
		case MOD_OP:
			return modOp(evalTree(node->right), evalTree(node->left));
		case ASSIGN_OP:
			// make sure rigt is a var
			if (node->left->type != SYMBOL)
			{
				//parserError = INVALID_ASSIGNMENT;
				setErrorMessage(INVALID_ASSIGNMENT, "Invalid assignment");
				return (Value) { TYPE_UNKNOWN, { 0 } };
			}

			return assignOp(node->left->symbol, evalTree(node->right));
		case DOUBLE:
		case INTEGER: // UNKNOWN VAL CHECK?
			return node->value;
		case SYMBOL:
			return symbolOp(node->symbol);

		default:
			return (Value) { TYPE_UNKNOWN, { 0 } };
	}
	

	return (Value) { TYPE_UNKNOWN, { 0 } };
}

/// Displays the infix expression for the tree, using
/// parentheses to indicate the precedence, e.g.:
///
/// expression: 10 20 + 30 *
/// infix string: ((10 + 20) * 30) 
///
/// @precondition:  This routine should not be called if there
/// 	is a parser error.
void infixTree(ExpNode* node)
{
	// TODO: PRECONDITION????
	// precondition, DOES IT APPLY HERE?
	if (getUtilsParserError() != NONE)
		return;

	if (node != NULL)
	{
		if (node->left != NULL)
			printf("(");

		infixTree(node->left);

		if(node->left != NULL)
			printf(" ");

		printNode(node);

		if(node->right != NULL)
			printf(" ");

		infixTree(node->right);

		if(node->right != NULL)
			printf(")");
	}
}

/// Cleans up all dynamic memory associated with the expression tree.
/// @param node The current node in the tree
void cleanupTree(ExpNode* node)
{
	// TODO: REFINE?
	if (node != NULL)
	{
		cleanupTree(node->left);
		cleanupTree(node->right);
		FREENULL(node);
		cleanupTree(node);
	}
}

/// Tells the main program whether there was an error with either
/// the parsing or evaluation of the tree.
/// @return parser error
ParserError getParserError(void)
{
	// HOW IS THIS USED?
	return getUtilsParserError();
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
