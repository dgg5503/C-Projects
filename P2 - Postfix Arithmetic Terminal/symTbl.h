// File: $Id: symTbl.h,v 1.7 2016/03/03 20:02:36 csci243 Exp $
// Description: Definition of a symbol table
// Author: sps

#ifndef SYM_TBL_H
#define SYM_TBL_H

// initial number of symbols in the symbol table file
#define MAX_SYMBOLS 10

// maximum length in characters (including null char)
// of any token read from symbol table file
#define MAX_SYM_LENGTH 16

// interpreter supports two types of Symbols (otherwise unknown)
typedef enum Type {
    TYPE_INT,            
    TYPE_DOUBLE,
    TYPE_UNKNOWN
} Type;

// The symbol value stores the int or double
typedef struct {
    Type type;    // the type
    union {
        int iVal;       // integer value
        double dVal;    // double value
    } value;
} Value;

// A single symbol definition
typedef struct Symbol {	
	char varName[MAX_SYM_LENGTH];	// the name of the variable
	Value value;			        // its value
} Symbol;

/// Constructs the table by reading the file.  The format is
/// one symbol per line in the format:
///
///	variable-type variable-name	variable-value
///	...
///
/// For this problem we guarantee the variable-names are all
/// unique, and the format of the file is error free.
/// You are allowed to create it statically or
/// dynamically and store it locally.
/// @param filename The name of the file containing the symbols
/// @exception If the file can't be opened, an error message should
/// be displayed to standard error and the program should exit
/// with EXIT_FAILURE.  If no file is specified, an empty table
/// should be built (there is no message or error/exit in this case).
///
/// Error loading symbol table
///
void buildTable(char filename[]);

/// Displays the contents of the symbol table in the following format:
///
/// SYMBOL TABLE:
///	Type: variable-type Name: variable-name, Value: variable-value
///	...
///
/// Each symbol should be printed one per line, tab-indented.
void dumpTable(void);

/// Returns the symbol associated with variable name
/// @param symbol The name of the variable
/// @return The Symbol, if it exists, or NULL otherwise
Symbol* lookupTable(char variable[]);

/// Attempt to add a new symbol to the table (if it is not
/// already full).
/// @param Symbol symbol the new symbol to add
/// @return 0 if the symbol could not be added, anything else otherwise
int addTable(Symbol symbol);

/// Destroys the symbol table (if you built it dynamically).
void destroyTable();

#endif

