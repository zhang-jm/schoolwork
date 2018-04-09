/*
 * Filename: main.c
 * Description: Program to read in strings from command line and find and
 *              print the reversed string.  Also determines if string is
 *              a palindrome.
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

/*
 * Function name: main()
 * Function prototype: int main( int argc, char *argv[] );
 * Description: C main driver which reads in strings from command line and
 *              calls assembly functions to reverse the string and determine
 *              whether it is a palindrome.
 * Parameters: strings to reverse
 * Side Effects: Outputs original string and reversed string, as well as a
 *               short message if it is a palindrome.
 * Error Conditions: Not entering any strings on the command line.
 * Return Value: 0 indicating successful execution.
 */


int main( int argc, char* argv[] ) {

  /* Check that there are some strings to reverse */
  if (argc == 1) {
    (void) fprintf(stderr, "Please enter at least one string to reverse\n");
    exit(1);
  }

  int numPalindromes = 0;

  /* Loop through all command line arguments */
  int i;
  for (i = 1; i < argc; ++i) {

    /* Get string and print it out */
    char* str = argv[i];
    (void) printf("Before: %s\n", str);

    /* Reverse string and increment count if it's a palindrome */
    if (reverse(str)) {
      ++numPalindromes;
    }

    /* Print reversed string */
    (void) printf("After: %s\n", str);

    (void) printf("\n");
  }

  /* Print number of palindromes found */
  (void) printf("You found %d palindrome(s)\n", numPalindromes);

  return 0;
}
