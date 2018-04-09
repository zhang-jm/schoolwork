/*
 * Filename: main.c
 * Description: Program to read in a single string from the command line
 * and count the characters in this string.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGS 2
#define ASCII 256
#define BUFSIZE 80

/*
 * Function name: main()
 * Function prototype: int main( int argc, char *argv[] );
 * Description: C main driver which reads in a single string from command 
 * line and count the characters in the string.
 * Parameters: int argc - number of arguments
 *             char *argv[] - arguments as a list of strings
 * Side Effects: Print out how many times each character appeared in the string,
 * the total number of characters, and the total number of unique characters.
 * Error Conditions: Not entering any strings on the command line.
 * Return Value: 0 indicating successful execution.
 */

extern int* count( char* str );

int main( int argc, char* argv[] ) {

  /* Check commandline argument */
  if (argc != ARGS) {
    (void) fprintf(stderr, "Usage: ./countChars STRING\n");
    exit(1);
  }

  int numOfUniqueChars = 0;
  char str[BUFSIZE];
  int* counts;
  extern int numOfChars;

  /* Get string and print it out */
  (void) strcpy(str, argv[1]);
  (void) printf("Counting number of characters for \"%s\"...\n", str);
  (void) printf("\n");

  /* Count the characters */
  counts = count(str);

  /* Print the counts */
  int j;
  for (j = 0; j < ASCII; j++) {
    if (counts[j]) {
      (void) printf("Character %c ", j);
      (void) printf("appeared %d times\n", counts[j]);

      numOfUniqueChars++;
    }
  }

  (void) printf("\n");
  (void) printf("\"%s\" has a total of %d character(s)\n", str, numOfChars);

  /* Print number of unique characters in the string */
  (void) printf("wow %d different ascii character(s) much unique so skill\n", 
                                                          numOfUniqueChars);

  return 0;
}
