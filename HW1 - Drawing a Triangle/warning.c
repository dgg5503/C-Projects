/// file: warning.c
/// description: faulty code example that needs fixing.
/// @author RIT CS Dept.

#include <stdio.h>

/// compute a value.
int compute(int a) {
    int b = a * 5;

	// make sure to return a value when the function isnt void!
	return b;
}

/// main function
// main gets either void or 2 parameters
int main(void) {

	// remove unused variables
    int y = compute(20);

    printf("%d\n", y);

    return 0;
}

