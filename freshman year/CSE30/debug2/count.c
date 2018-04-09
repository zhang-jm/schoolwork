/*
 * Filename: count.c
 * Description: Helper function that counts the characters.
 */ 

#include <stdio.h>
#include <string.h>
#define ASCII 256

/*
 * Function name: count()
 * Function prototype: int* count( char *str );
 * Description: Helper function that counts the characters in the string.
 * Parameters: char *str - the string you want to count
 * Side Effects: none
 * Error Conditions: none
 * Return Value: a int pointer to the count array
 */


int numOfChars;

int* count( char* str ) {
  static int asctb[ASCII] = {0};

  int i;
  int c;
  for (i = 0; i < strlen(str); i++) {
    c = str[i];
    asctb[c]++;
  }

  numOfChars += strlen(str);

  return asctb;
}

